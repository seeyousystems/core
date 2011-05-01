/*
 *  GridMap.cpp
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

#include "GridMap.h"

#include "../Library/Util.h"

GridMap::GridMap(Create *create, long gridSize) : Map("GridMap", create)
{
	this->gridSize = gridSize;
	gridColor = Util::convertStringToRGB(create->stringSetting("Map_GridMap_Color"));
}

GridMap::~GridMap()
{
}

void GridMap::paint(QPainter &painter, QRect view)
{
	long gs = create->mmToPixels(gridSize);

	painter.setPen(gridColor);
	painter.setOpacity(0.3);

	// Draw vertical lines
	for(int i = 0; i < (view.width() / (gs) + 2); i++) {
		int x = (gs * i) - (view.x() % gs);
		painter.drawLine(x, 0, x, (int)(view.height()));
	}

	// Draw horizontal lines
	for(int i = 0; i < (view.height() / (gs) + 2); i++) {
		int y = view.height() - (gs * i) + (view.y() % gs);
		painter.drawLine(0, y, (int)(view.width()), y);
	}
}

long GridMap::width() {
	return 0;
}

long GridMap::height() {
	return 0;
}
