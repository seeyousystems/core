/*
 *  TerrainPoint.h
 *
 *  Utility class for a three dimensional point, where the 3rd dimension is
 *  stricktly height.
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

#ifndef TERRAINPOINT_H_INCLUDED
#define TERRAINPOINT_H_INCLUDED

template<class real>
class TerrainPointT {

public:
	real x;
	real y;
	real height;

public:
	TerrainPointT();
	TerrainPointT(real x, real y, real height);
};

template<class real>
TerrainPointT<real>::TerrainPointT(void) {
	this->x = 0;
	this->y = 0;
	this->height = 0;
}

template<class real>
TerrainPointT<real>::TerrainPointT(real x, real y, real height) {
	this->x = x;
	this->y = y;
	this->height = height;
}

////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef TerrainPointT <float> TerrainPointF;
typedef TerrainPointT <double> TerrainPointD;
typedef TerrainPointT <long double> TerrainPointL;
typedef TerrainPointT <int> TerrainPointI;


#endif /* TERRAINPOINT_H_ */
