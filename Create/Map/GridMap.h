/*
 *  GridMap.h
 *
 *  Draws a semi-transparent grid based on gridSize.
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

#ifndef GRIDMAP_H_
#define GRIDMAP_H_

#include "Map.h"

#include <QColor>

class GridMap : public Map
{
	Q_OBJECT

private:
	QColor gridColor;

public:
	long gridSize;

public:
	GridMap(Create *create, long gridSize);
	virtual ~GridMap();

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();

};

#endif /*GRIDMAP_H_*/
