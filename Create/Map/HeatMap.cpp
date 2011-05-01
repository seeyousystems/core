/*
 *  HeatMap.cpp
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

#include <QPainter>

#include "HeatMap.h"

#include "../MovementTracker/Tracker.h"
#include "../Controller/Controller.h"

HeatMap::HeatMap(Create *create, long width, long height) : ColorMap(create, "HeatMap", width, height)
{
	distance = 2.0; // TODO: what is this?
	lastCollisionAreaPosition = Vector2D(0,0);
	lastOpenAreaPosition = Vector2D(0,0);
}

HeatMap::~HeatMap() {

}

void HeatMap::connectController(QObject *target) {
	connect(target, SIGNAL(signalObjectDetected(double,double,double,int)), this, SLOT(registerCollisionAtTrackerLocation(double,double,double,int)));
}

void HeatMap::registerHeat(ColorMap::MapChannel channel, long x, long y) {
	registerHeat(channel, x, y, create->doubleSetting("Map_HeatMap_HeatOpacity"), create->longSetting("Map_HeatMap_HeatSize"));
}

void HeatMap::registerHeat(ColorMap::MapChannel channel, long x, long y, double opacity, long size) {


	lock.lockForWrite();
	{
		if(channel == ColorMap::CollisionAreaChannel && lastCollisionAreaPosition.x() == 0 && lastCollisionAreaPosition.y() == 0){
			lastCollisionAreaPosition = Vector2D(x,y);
		}
		if(channel == ColorMap::OpenAreaChannel && lastOpenAreaPosition.x() == 0 && lastOpenAreaPosition.y() == 0){
			lastOpenAreaPosition = Vector2D(x,y);
		}

		// Resize the map dynamically if needed...
		resizeIfNeeded(x,y);
		updateVBounds(x,y);

		// Convert to pixel
		x = create->mmToPixels(x);
		y = create->mmToPixels(y);
		size = create->mmToPixels(size);

		// Init painter
		QPainter painter(&bitmap);
		painter.setPen(Qt::transparent);
		painter.setOpacity(opacity);

		// Draw heat spot
		if     (channel == ColorMap::CollisionAreaChannel  && distance < dist(lastCollisionAreaPosition, Vector2D(x,y))){
			lastCollisionAreaPosition = Vector2D(x,y);
			painter.setBrush(QBrush(QColor(create->stringSetting("Map_HeatMap_CollisionAreaColor")))); //TODO: refactor to Util method for color loading and cache value...
		}
		else if(channel == ColorMap::OpenAreaChannel && distance < dist(lastOpenAreaPosition, Vector2D(x,y))){
			painter.setBrush(QBrush(QColor(create->stringSetting("Map_HeatMap_OpenAreaColor")))); //TODO: refactor to Util method for color loading and cache value...
			lastOpenAreaPosition = Vector2D(x,y);
		}
		painter.drawEllipse(QRectF(x - (size/2), bitmap.height() - y - (size/2), size, size));
	}
	lock.unlock();
}

void HeatMap::registerHeat(ColorMap::MapChannel channel, Trafo2D position, double distance, double angle, double opacity, int size) {

	Trafo2D objectLocation = position * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance); // create/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
	registerHeat(channel, (long)objectLocation.trans().x(), (long)objectLocation.trans().y(), opacity, size);

}

void HeatMap::registerCollisionAtTrackerLocation(double distance, double angle, double opacity, int size) {
	registerHeat(ColorMap::CollisionAreaChannel, create->tracker->getTransformation(), distance, angle, opacity, size);
}



