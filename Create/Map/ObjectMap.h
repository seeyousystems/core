/*
 *  ObjectMap.h
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef OBJECTMAP_H_
#define OBJECTMAP_H_

#include "Map.h"

class MapObject;

class ObjectMap : public Map
{
	Q_OBJECT

public:
	QList<MapObject*> *objects;

public:
	ObjectMap(Create *create);
	virtual ~ObjectMap();
	Create::AreaType checkForCollision(long x, long y, MapObject *exclude = NULL);
	long mostDistantObjectX();
	long mostDistantObjectY();
	void deleteAllObjects();

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();
};

#endif /* OBJECTMAP_H_ */
