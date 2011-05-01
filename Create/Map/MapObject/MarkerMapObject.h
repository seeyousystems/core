/*
 * MarkerMapObject.h
 *
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

#ifndef MARKERMAPOBJECT_H_
#define MARKERMAPOBJECT_H_

#include "MapObject.h"

#include "../../Library/Math.h"

class MarkerMapObject : public MapObject
{
	Q_OBJECT

private:
	int transformationCount;
	Trafo2D transformation[6];
	QColor transformationColors[6];
	long movementScale;


public:
	MarkerMapObject(ObjectMap *map, long movementScale) : MapObject(map, "MarkerMapObject", 0, 0, 0, 0) { transformationCount = 0; this->movementScale = movementScale; };
	virtual ~MarkerMapObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);
	void setAsVector(int index, Vector2D vector);
	void setAsPoint(int index, long x, long y);
	void setAsTransformation(int index, Trafo2D transformation, QColor color);
};

#endif /* MARKERMAPOBJECT_H_ */
