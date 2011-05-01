/*
 *  TerrainMap.h
 *
 *  The class provides three main functions: raise(x,y), lower(x,y), and
 *  getHeight(). The higher an area of the map is, the brighter white it
 *  is colored. The raise(x,y) and lower(x,y) methods are overloaded to
 *  allow entire sections of a map to be altered at once.
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

#ifndef TERRAINMAP_H_
#define TERRAINMAP_H_

#include <QImage>

#include "ColorMap.h"

class TerrainMap : public ColorMap
{
	Q_OBJECT

public:
	TerrainMap(Create *create, long width, long height);
	virtual ~TerrainMap();
	void setHeight(long x, long y, double percent);
	void setHeight(long x, long y, long width, long height, double percent, QColor color);
	void raise(long x, long y);
	void lower(long x, long y);
	void raise(long x, long y, long size);
	void lower(long x, long y, long size);
	double getHeight(long x, long y);

private:
	void setBitmapHeight(long x, long y, long width, long height, double percent, QColor color);

};

#endif /*TERRAINMAP_H_*/
