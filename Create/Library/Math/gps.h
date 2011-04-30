/*
 *  gps.h
 *
 *
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

#ifndef GPS_H_
#define GPS_H_



////////////////////////////////////////////////////////////////////////
///   Globals                                                        ///
////////////////////////////////////////////////////////////////////////

#define emajor  ((double)6378137.0)				// Earth's semi-major axis in meters
#define eflat   ((double)0.00335281068118 )		// Earth flattening factor
#define ZERO    ((double)0.0)
#define ONE     ((double)1.0)
#define TWO     ((double)2.0)
#define THREE   ((double)3.0)
#define FOUR    ((double)4.0)
#define FIVE    ((double)5.0)

////////////////////////////////////////////////////////////////////////
///   Forward declarations                                           ///
////////////////////////////////////////////////////////////////////////

class GeocentricPosition;
class GPSPosition;




////////////////////////////////////////////////////////////////////////
///   Declaration GeocentricPosition                                  ///
////////////////////////////////////////////////////////////////////////

class GeocentricPosition {

public:

	double x;
	double y;
	double z;

public:

	GeocentricPosition();
	GeocentricPosition(double x, double y, double z);
	virtual ~GeocentricPosition() {};
	GPSPosition gpsPosition(); // Convert to GPS lat-long-h

};




////////////////////////////////////////////////////////////////////////
///   Declaration GPSPosition                                        ///
////////////////////////////////////////////////////////////////////////

class GPSPosition {

public:

	double latitude; // In degrees
	double longitude; // In degrees
	double height;

public:

	GPSPosition();
	GPSPosition(double latitude, double longitude, double height);
	virtual ~GPSPosition() {};
	GeocentricPosition geocentricPosition(); // Convert to geocentric x-y-z
	double metersPerLatitude();	// Returns the meters per one degree of latitude at the given position
	double metersPerLongitude(); // Returns the meters per one degree of longitude at the given position
	double latitudesPerMeter(); // Returns the degrees of latitude per one at the given position
	double longitudesPerMeter(); // Returns the degrees of longitude per one at the given position
	double latitudesPerMeters(double meters); // Returns the degrees of latitude per given meters at the given position
	double longitudesPerMeters(double meters); // Returns the degrees of longitude per given meters at the given position
	double metersToDestination(GPSPosition desitination);
};







#endif /* GPS_H_ */
