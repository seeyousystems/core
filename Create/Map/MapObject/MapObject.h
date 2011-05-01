/*
 *  MapObject.h
 *
 *  Core Objects are used to draw different structures on Maps, such as
 *  a navigation spline or the robots current position.
 *
 *  Note: The internal data is kept to the Core size convention of millimeters.
 *  Drawing operations must translate this according to the scale specified in
 *  the paint() method.
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

#ifndef MAPOBJECT_H_
#define MAPOBJECT_H_

#include <QObject>
#include <QPainter>
#include <QString>


class ObjectMap;

class MapObject : public QObject
{
	Q_OBJECT

public:
	long width;
	long height;
	long x;
	long y;
	double rotation;
	ObjectMap *map;
	QString name;
	bool visible;

private:


public:
	MapObject(ObjectMap *map, QString name, long x, long y, long width, long height);
	virtual ~MapObject();
	virtual void paint(QPainter &painter, QRect view, int scale) = 0;
	virtual bool checkForCollision(long x, long y);

public slots:
	void move(long x, long y, double rotation);

};

#endif /*MAPOBJECT_H_*/
