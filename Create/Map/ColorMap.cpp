/*
 *  ColorMap.cpp
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

#include "ColorMap.h"

ColorMap::ColorMap(Create *create, QString name, long width, long height) : Map(name, create)
{
	bitmap = QImage((int)create->mmToPixels(width), (int)create->mmToPixels(height), QImage::Format_ARGB32);
	vbounds = QRect(INT_MAX,INT_MAX,INT_MIN,INT_MIN);
}

ColorMap::~ColorMap()
{

}

void ColorMap::paint(QPainter &painter, QRect view) {
	lock.lockForRead();
	{
		// Calculate source x and y
		int sx = view.x();
		int sy = bitmap.height() - view.y() - view.height();
		int sw = view.width();
		int sh = view.height();

		// Check vbounds if we even need to draw anything
		if(		(
					(vbounds.x() >= sx && vbounds.x() <= sx+sw) ||
					(vbounds.x()+vbounds.width() >= sx && vbounds.x()+vbounds.width() <= sx+sw)
				)
					&&
				(
					(vbounds.y() >= sy && vbounds.y() <= sy+sh) ||
					(vbounds.y()+vbounds.height() >= sy && vbounds.y()+vbounds.height() <= sy+sh)
				)
		) {

			// Draw heat map image
			QRectF source = QRect(sx, sy, sw, sh);
			QRectF target = QRect(0, 0, sw, sh);
			painter.drawImage(target, bitmap, source);

		}

		// Draw vbounds rectangle (for debugging)
		//painter.drawRect((vbounds.x())-view.x(),view.height()+(vbounds.y())-(bitmap.height()-view.y()),vbounds.width(),vbounds.height());

	}
	lock.unlock();
}

void ColorMap::resizeIfNeeded(long x, long y) {
	// TODO: Dynamic resizing of heat map
}

void ColorMap::updateVBounds(long x, long y) {
	// Convert to pixel
	x = create->mmToPixels(x);
	y = bitmap.height() - create->mmToPixels(y);

	// Update bounding box
	if(x < vbounds.x()) 					vbounds.setX(x);
	if(x > vbounds.x()+vbounds.width()) 	vbounds.setWidth(x-vbounds.x());
	if(y < vbounds.y()) 					vbounds.setY(y);
	if(y > vbounds.y()+vbounds.height()) 	vbounds.setHeight(y-vbounds.y());

	//Debug::print("%1 %2 %3 %4", vbounds.x(),vbounds.y(),vbounds.width(),vbounds.height());
}

// Return heat in percent
double ColorMap::getChannelValue(ColorMap::MapChannel channel, long x, long y) {

	QReadLocker locker(&lock);

	// Convert to pixel scale
	x = create->mmToPixels(x);
	y = create->mmToPixels(y);

	// Check bounds
	if(x > bitmap.width() || bitmap.height() - y > bitmap.height()) return 0;

	// See what the physical map states this area is...
	QRgb rgb = bitmap.pixel((int)x, bitmap.height() - (int)y);
	int heat = 0;
	if		(channel == ColorMap::CollisionAreaChannel) 	heat = qRed(rgb);
	else if	(channel == ColorMap::OpenAreaChannel) 			heat = qGreen(rgb);
	else if	(channel == ColorMap::HeightChannel) 			heat = qRed(rgb); // Any color channel will do :)
	return (double)(qAlpha(rgb)*heat) / (255.0*255.0);
}

QRgb ColorMap::getPixelValue(long x, long y) {

	QReadLocker locker(&lock);

	// Convert to pixel scale
	x = create->mmToPixels(x);
	y = create->mmToPixels(y);

	// Check bounds
	if(x > bitmap.width() || bitmap.height() - y > bitmap.height()) return qRgb(0,0,0);

	// See what the physical map states this area is...
	return bitmap.pixel((int)x, bitmap.height() - (int)y);

}

long ColorMap::width() {
	return create->pixelsTomm(this->bitmap.width());
}

long ColorMap::height() {
	return create->pixelsTomm(this->bitmap.height());
}

