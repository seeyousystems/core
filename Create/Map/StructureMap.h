/*
 * StructureMap.h
 *
 *
 *  The StructureMap represents the structure of a room as the robot sees it.
 *  It is built up sequentially over time, and requires a certain amount of
 *  exploration to be fully generated. The corner points of the Map are extracted
 *  from the individual collisions the robot detects. The StructureMap has a
 *  special property isFinished() which lets a module know if the Map is complete
 *  or not. The Map is complete when the last added point is within the defined
 *  distance  from the starting point.
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

#ifndef STRUCTUREMAP_H_
#define STRUCTUREMAP_H_

#include <QPoint>
#include <QList>

#include "../Library/Math.h"

#include "Map.h"


class StructureMap : public Map{
	Q_OBJECT

private:
	QList<Vector2D> collisions;
	bool finish;

private:
	void appendPoint(Vector2D p);

public:
	StructureMap(Create *create);
	virtual ~StructureMap();
	void connectController(QObject *target);
	bool isFinish();
	bool isPointOutside(Vector2D p);
	void addStructureMapPoint(Vector2D p);
	void clearStructureMapPoints();
	void setFinish();
	int count();

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();

public slots:
	virtual void registerObjectDetected(double, double, double, int);

};

#endif /* STRUCTUREMAP_H_ */
