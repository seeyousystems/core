/*
 *  NavPathMapObject.h
 *
 *  Draws a navigation path which the robot has driven :).
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

#ifndef NAVPATHMAPOBJECT_H_
#define NAVPATHMAPOBJECT_H_

#include "MapObject.h"

#include "../../Library/Math.h"

#include <QPainterPath>
#include <QColor>

class NavPathMapObject : public MapObject
{

	Q_OBJECT

private:
	QPainterPath path;
	QColor color;
	long pathCount;
	Vector2D lastPoint;

public:
	NavPathMapObject(ObjectMap *map, QColor color = Qt::blue);
	virtual ~NavPathMapObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);

public slots:
	void addNavPoint(long x, long y);

};

#endif /* NAVPATHMAPOBJECT_H_ */
