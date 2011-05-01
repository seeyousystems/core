/*
 *  PhysicalMap.h
 *
 *
 *  The emss Physical map represents the real world environment in its physical
 *  state. This Map is currently used for simulation purposes, but in the
 *  future can also be used for various Tasks which would like to combine
 *  a predefined perception of the environment. The Physical Map offers the
 *  method getAreaType(x,y) which returns either Open Area or Collision
 *  Area � just like the Heat Map. The Emulated Emss Controller uses this
 *  Map to emulate the IR Range Finder during simulations, something which
 *  has proven especially useful as the robots environment can now be almost
 *  entirely emulated. To define the areas of a physical map, all one has to
 *  do is draw a RGBA image, such as a 24-bit PNG, and let the Physical Map
 *  load the file. Any area in the image which has the alpha channel not at
 *  255 (full) is regarded as Open Area.
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

#ifndef PHYSICALMAP_H_
#define PHYSICALMAP_H_

#include <QImage>

#include "Map.h"
#include "../Library/KMLGroundOverlay.h"

class PhysicalMap : public Map
{
	Q_OBJECT

private:
	KMLGroundOverlay *kml;

public:
	enum PhysicalMapAreaType { PhysicalMapAreaTypeWall, PhysicalMapAreaTypeStairs, PhysicalMapAreaTypeOpen };

public:
	QString 	filePath;
	QImage 		image;
	double		mapScale;
	QRgb 		wall;
	QRgb 		stairs ;
	int 		tolerance;

public:
	PhysicalMap(Create *create, QString filePath, double mapScale);
	virtual ~PhysicalMap();
	PhysicalMapAreaType getAreaType(long x, long y);
	QRgb getPixelValue(long x, long y);
	GPSPosition mmToGPS(long x, long y);
	QString getFloor();
	QString getMapDescription();

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();
};

#endif /*PHYSICALMAP_H_*/
