/*
 *  ColorMap.h
 *
 *  The ColorMap incorporates common routines needed by Maps which work in a
 *  two-point-five-dimensional (2.5D) color space, meaning Maps which have a
 *  specific color value for every two-dimensional coordinate. In addition to
 *  providing query functions such as getPixelValue(x,y) and getChannelValue(...),
 *  the ColorMap class manages the memory for the underlying data.
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

#ifndef COLORMAP_H_
#define COLORMAP_H_

#include <QImage>

#include "Map.h"

class ColorMap : public Map {

	Q_OBJECT

public:
	enum MapChannel { CollisionAreaChannel, OpenAreaChannel, HeightChannel };

public:
	QImage 	bitmap;		// The bitmap structure which contains all channel values
	QRect	vbounds;	// The virtual bounds of the bitmap contents used to optimize drawing, in pixels

protected:
	void resizeIfNeeded(long x, long y);
	void updateVBounds(long x, long y);

public:
	ColorMap(Create *create, QString name, long width, long height);
	virtual ~ColorMap();
	double getChannelValue(ColorMap::MapChannel channel, long x, long y);
	QRgb getPixelValue(long x, long y);

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();
};

#endif /* COLORMAP_H_ */
