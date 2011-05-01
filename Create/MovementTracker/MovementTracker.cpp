/*
 *  MovementTracker.cpp
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

#include "MovementTracker.h"

#include "../Library/Debug.h"

#include "../Map/ColorMap.h"
#include "../Map/HeatMap.h"


MovementTracker::MovementTracker(QString name, Create *create) : CoreObject(name, create){
	wheelOffset = create->doubleSetting("Robot_WheelOffset_mm");
	totalDistance = 0;
	relativeDistance = 0;
	totalAngle = 0;
	relativeAngle = 0;
}

MovementTracker::~MovementTracker() {

}

long MovementTracker::x() {
	return getTransformation().trans().x();
}

long MovementTracker::y() {
	return getTransformation().trans().y();
}

Vector2D MovementTracker::translation() {
	return getTransformation().trans();
}

double MovementTracker::rotation() {
	return Deg(getTransformation().angle());
}

void MovementTracker::setTransformation(Trafo2D t){
	lock.lockForWrite();
	{
		transformation = t;
		emit moved(transformation.trans().x(),transformation.trans().y(),Deg(transformation.angle()));
	}
	lock.unlock();
}

Trafo2D MovementTracker::getTransformation(){
	Trafo2D retVal;
	lock.lockForRead();
	{
		retVal = transformation;
	}
	lock.unlock();
	return retVal;
}

long MovementTracker::getTotalDistance(){
	long retVal;
	lock.lockForRead();
	{
		retVal = totalDistance;
	}
	lock.unlock();
	return retVal;
}

double MovementTracker::getTotalAngle(){
	double retVal;
	lock.lockForRead();
	{
		retVal = totalAngle;
	}
	lock.unlock();
	return retVal;
}

long MovementTracker::getRelativeDistance(){
	long retVal;
	lock.lockForRead();
	{
		retVal = relativeDistance;
	}
	lock.unlock();
	return retVal;
}

double MovementTracker::getRelativeAngle(){
	double retVal;
	lock.lockForRead();
	{
		retVal = relativeAngle;
	}
	lock.unlock();
	return retVal;
}

int MovementTracker::getWeight(){
	int retVal;
	lock.lockForRead();
	{
		retVal = weight;
	}
	lock.unlock();
	return retVal;
}


