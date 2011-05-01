/*
 *  MapObject.cpp
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

#include "MapObject.h"

#include "../../Map/ObjectMap.h"
#include "../../Library/HeapLogger.h"

MapObject::MapObject(ObjectMap *map, QString name, long x, long y, long width, long height) {
	// Init
	this->map = map;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->rotation = 0.0;
	this->name = name;
	this->visible = true;

	// Register with map
	if(map != NULL) this->map->objects->append(this);

	// Register with heap logger
	HeapLogger::registerEntity(name);
}

MapObject::~MapObject() {
	// Unregister with heap logger
	HeapLogger::unregisterEntity(name);
}

bool MapObject::checkForCollision(long x, long y)
{
	if(		x >= this->x - (width/2) &&
			x <= this->x + (width/2) &&
			y >= this->y - (height/2) &&
			y <= this->y + (height/2)	) 	return true;
	else 									return false;
}

void MapObject::move(long x, long y, double rotation) {
	this->x = x;
	this->y = y;
	this->rotation = rotation;
}

