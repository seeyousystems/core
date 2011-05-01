/*
 *  RobotMapObject.h
 *
 *  Draws the robot to the viewport.
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

#ifndef ROBOTMAPOBJECT_H_
#define ROBOTMAPOBJECT_H_

#include "MapObject.h"

class RobotMapObject : public MapObject
{
	Q_OBJECT

private:
	QColor color;
	double opacity;

public:
	RobotMapObject(ObjectMap *map, long x, long y, long width, long height, QColor color = Qt::white, double opacity = 1.0);
	virtual ~RobotMapObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);
};

#endif /*ROBOTMAPOBJECT_H_*/
