/*
 *  Navigation.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Navigation.h"

#include "../create.h"
#include "../Library/Debug.h"
#include "../MovementTracker/Tracker.h"
//#include "../Map/HeatMap.h"
//#include "../Map/FadingCollisionMap.h"

#include <limits.h>

Navigation::Navigation(QString name, Create *create) : CoreObject(name, create){

	navPoints = new QList<Vector2D>();
	currentNavPoint = 0;

	Debug::print("[Navigation] %1 loaded", name);
}

Navigation::~Navigation() {

	delete navPoints;

}

// Wrapper method which checks to see if we need to add some points before
// or after in order to create a valid spline...
void Navigation::addNavPoint(Vector2D point) {

	int count;

	lock.lockForRead();
	{
		count = navPoints->count();
	}
	lock.unlock();

	// Set start point
	if ( count == 0) {

		// Add starting point
		Vector2D startPoint(Vector2D(this->create->tracker->getX(), this->create->tracker->getY()));
		processAddNavPoint(startPoint);

		// Add additional points for a spline start
		if(create->stringSetting("Navigation_AutomaticallyAddStartingNavPoints") == "true") {
			Vector2D splineStartPoint1 = this->create->tracker->getTransformation() * Vector2D(0, 400);
			Vector2D splineStartPoint2 = this->create->tracker->getTransformation() * Vector2D(0, 800);
			processAddNavPoint(splineStartPoint1);
			processAddNavPoint(splineStartPoint2);
		}
	}


	// Call the real method to add the point
	processAddNavPoint(point);
}

void Navigation::addNavPoint(long x, long y) {
	addNavPoint(Vector2D((double) x, (double) y)); // Forward to master function
}

void Navigation::processAddNavPoint(Vector2D point) {
	lock.lockForWrite();
	{
		navPoints->append(point);
	}
	lock.unlock();
}

QList<Vector2D> Navigation::getNavPoints() {
	QList<Vector2D> retVal;

	lock.lockForRead();
	{
		retVal = *navPoints;
	}
	lock.unlock();

	return retVal;
	//return *navPoints;
}

void Navigation::clearNavPoints() {

	lock.lockForWrite();
	{
		navPoints->clear();
		currentNavPoint = 0;
	}
	lock.unlock();

}

int Navigation::getCurrentNavPoint() {
	return currentNavPoint;
}

bool Navigation::isAtLastNavPoint() {
	bool retVal;

	lock.lockForRead();
	{
		retVal = ( getCurrentNavPoint() >= navPoints->count()-1 );
	}
	lock.unlock();

	return retVal;
}

Vector2D Navigation::getCurrentDestination() {
	Vector2D retVal;

	lock.lockForRead();
	{
		if(navPoints->count() == 1)						 		retVal = navPoints->at(0);
		else if(getCurrentNavPoint() < navPoints->count() -1) 	retVal = navPoints->at(getCurrentNavPoint()+1);
		else													retVal = Vector2D(0,0); // Unknown location
	}
	lock.unlock();

	return retVal;
}

// Returns the distance to a possible collision straight ahead or backwards using the heat map.
// Note: If no collision is within the map bounds, INT_MAX is returned.
long Navigation::getMaxOpenAreaDistanceFromMap(CollisionSource source, CollisionDirection direction){
	// Set max range to the longest possible range within map bounds
	//long maxRange = (long)sqrtf((float)core->mapWidth*(float)core->mapWidth + (float)core->mapHeight*(float)core->mapHeight);
	long maxRange = create->longSetting("Navigation_MaxOpenAreaDistanceMaxRange_mm");

	for(long i = 0; i < maxRange; i++) {

		// Set the direction and get the test position
		int dir = (direction == Navigation::CollisionDirectionAhead ? +1 : -1);

		//todo: abraham fix this
		Trafo2D position = create->tracker->getTransformation() * Trafo2D::trans(0, dir*i);

		// Check if out of map bounds...
		if(position.trans().x() < 0 || position.trans().y() < 0 || position.trans().x() > create->mapWidth || position.trans().y() > create->mapHeight) return INT_MAX;

		// Return if i if collision detected at position
//		if(source == CollisionSourceHeatMap) {
//			if (create->heatMap->getChannelValue(HeatMap::CollisionAreaChannel,  (int)(position.trans().x()), (int)(position.trans().y())) > 0.0) return i;
//		} else if (source == CollisionSourceFadingCollisionMap) {
//			if (create->fadingCollisionMap->isPointWithinCollisionArea(position.trans())) return i;
//		}
	}

	return INT_MAX;
}

// Returns the distance to the wall using the heat map. Is now wall inside the reach
// of the wall sensor INT_MAX will be returned.
int Navigation::getWallDistanceFromMap(CollisionSource source){
	Trafo2D position;
	for(int i = 0; i < create->intSetting("Robot_WallSensorRange_mm"); i++){
		//todo:abraham fix this
		position = create->tracker->getTransformation() * Trafo2D::rot(Rad(-90)) * Trafo2D::trans(0, (create->intSetting("Robot_Diameter_mm")/2) + i);
//		if(source == CollisionSourceHeatMap) {
//			if (create->heatMap->getChannelValue(HeatMap::CollisionAreaChannel,  (int)(position.trans().x()), (int)(position.trans().y())) > 0.0) return i;
//		} else if (source == CollisionSourceFadingCollisionMap) {
//			if (create->fadingCollisionMap->isPointWithinCollisionArea(position.trans())) return i;
//		}
	}
	return INT_MAX;
}

