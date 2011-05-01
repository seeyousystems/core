/*
 *  TerrainMap.cpp
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

#include "TerrainMap.h"
#include "../Library/Debug.h"

TerrainMap::TerrainMap(Create *create, long width, long height) : ColorMap(create, "TerrainMap", width, height)
{

}

TerrainMap::~TerrainMap() {

}

// This alters the bits in the bitmap. It works on the pixel level instead of the mm level
// for efficiency and memory reasons. Thus it is private.
void TerrainMap::setBitmapHeight(long x, long y, long width, long height, double percent, QColor color) {
	lock.lockForWrite();
	{

		// Calculate color opacity
		double minimum = 0.01;
		double opacity = minimum + percent*(1.0-minimum);

		// Init painter and draw
		QPainter painter(&bitmap);
		painter.setOpacity(opacity);
		painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop); // Overwrite everything
		painter.fillRect(x, bitmap.height() - y, width, height, QBrush(color));
	}
	lock.unlock();
}

void TerrainMap::setHeight(long x, long y, long width, long height, double percent, QColor color) {
	// Convert to pixel
	x = create->mmToPixels(x);
	y = create->mmToPixels(y);
	width = create->mmToPixels(width);
	height = create->mmToPixels(height);

	// Call private method to actually set the bits
	setBitmapHeight(x, y, width, height, percent, color);
}

void TerrainMap::setHeight(long x, long y, double percent) {
	// Perform updates for side-effects
	resizeIfNeeded(x,y);
	updateVBounds(x,y);

	setBitmapHeight(create->mmToPixels(x), create->mmToPixels(y), 1, 1, percent, Qt::white);
}

void TerrainMap::raise(long x, long y) {
	raise(x, y, create->intSetting("Map_TerrainMap_RaiseLowerSize_mm"));
}

void TerrainMap::raise(long x, long y, long size) {

	// Perform updates for side-effects
	resizeIfNeeded(x,y);
	updateVBounds(x,y);
	updateVBounds(x+size,y+size);

	// Convert to pixel
	x = create->mmToPixels(x);
	y = create->mmToPixels(y);
	size = create->mmToPixels(size);

	// Set the height on the given block
	for(long px = x - size/2; px < x + size/2; px++) {
		for(long py = y - size/2; py < y + size/2; py++) {
			double height = getHeight(create->pixelsTomm(px), create->pixelsTomm(py)); // Convert back when querying our underlying ColorMap...
			height += create->doubleSetting("Map_TerrainMap_RaiseLowerIncrement");
			if(height > 1.0) height = 1.0;
			setBitmapHeight(px, py, 1, 1, height, Qt::white);
		}
	}
}

void TerrainMap::lower(long x, long y) {
	lower(x, y, create->intSetting("Map_TerrainMap_RaiseLowerSize_mm"));
}

void TerrainMap::lower(long x, long y, long size) {
	// Convert to pixel
	x = create->mmToPixels(x);
	y = create->mmToPixels(y);
	size = create->mmToPixels(size);

	// Set the height on the given block
	for(long px = x - size/2; px < x + size/2; px++) {
			for(long py = y - size/2; py < y + size/2; py++) {
				double height = getHeight(create->pixelsTomm(px), create->pixelsTomm(py)); // Convert back when querying our underlying ColorMap...
				height -= create->doubleSetting("Map_TerrainMap_RaiseLowerIncrement");
				if(height < 0.0) height = 0.0;
				setBitmapHeight(px, py, 1, 1, height, Qt::white);
			}
	}
}

double TerrainMap::getHeight(long x, long y) {
	return getChannelValue(ColorMap::HeightChannel, x, y);
}


