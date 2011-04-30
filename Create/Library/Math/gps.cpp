/*
 *  gps.cpp
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


#include <math.h>

#include "math-ext.h"
#include "gps.h"


////////////////////////////////////////////////////////////////////////
///   Implementation CartesianPosition                               ///
////////////////////////////////////////////////////////////////////////


GeocentricPosition::GeocentricPosition() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

GeocentricPosition::GeocentricPosition(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}


GPSPosition GeocentricPosition::gpsPosition() {
	double B;
	double d;
	double e;
	double f;
	double g;
	double p;
	double q;
	double r;
	double t;
	double v;
	double A = emajor;
	double FL = eflat;
	double zlong;
	GPSPosition gps;

	// 1.0 compute semi-minor axis and set sign to that of z in order
	// to get sign of Phi correct
	B= A * (ONE - FL);
	if( z < ZERO )
			B= -B;

	// 2.0 compute intermediate values for latitude
	r= Sqrt( x*x + y*y );
	e= ( B*z - (A*A - B*B) ) / ( A*r );
	f= ( B*z + (A*A - B*B) ) / ( A*r );

	// 3.0 find solution to:
	// t^4 + 2*E*t^3 + 2*F*t - 1 = 0
	p= (FOUR / THREE) * (e*f + ONE);
	q= TWO * (e*e - f*f);
	d= p*p*p + q*q;

	if( d >= ZERO ) {
			v= pow( (Sqrt( d ) - q), (ONE / THREE) )
			 - pow( (Sqrt( d ) + q), (ONE / THREE) );
	} else {
			v= TWO * Sqrt( -p )
			 * Cos( acos( q/(p * Sqrt( -p )) ) / THREE );
	}

	// 4.0 improve v
	// NOTE: not really necessary unless point is near pole
	if( v*v < fabs(p) ) {
			v= -(v*v*v + TWO*q) / (THREE*p);
	}
	g= (Sqrt( e*e + v ) + e) / TWO;
	t = Sqrt( g*g  + (f - v*g)/(TWO*g - e) ) - g;

	gps.latitude = atan( (A*(ONE - t*t)) / (TWO*B*t) );

	// 5.0 compute height above ellipsoid
	gps.height = (r - A*t)*Cos( gps.latitude ) + (z - B)*Sin( gps.latitude );

	//  6.0 compute longitude east of Greenwich
	zlong = ATan2( y, x );
	if( zlong < ZERO )
			zlong= zlong + 2*PI;
	gps.longitude = zlong;

	//  7.0 convert latitude and longitude to degrees
	gps.latitude = Deg(gps.latitude);
	gps.longitude = Deg(gps.longitude);

	return gps;
}






////////////////////////////////////////////////////////////////////////
///   Implementation GPSPosition                                     ///
////////////////////////////////////////////////////////////////////////

GPSPosition::GPSPosition() {
	this->latitude = 0;
	this->longitude = 0;
	this->height = 0;
}

GPSPosition::GPSPosition(double latitude, double longitude, double height) {
	this->latitude = latitude;
	this->longitude = longitude;
	this->height = height;
}

GeocentricPosition GPSPosition::geocentricPosition() {
	double A = emajor;
	double FL = eflat;
	double flatfn= (TWO - FL)*FL;
	double funsq= (ONE - FL)*(ONE - FL);
	double g1;
	double g2;
	double lat_rad= Rad(latitude);
	double lon_rad= Rad(longitude);
	double sin_lat;
	GeocentricPosition geo;

	sin_lat= Sin( lat_rad );

	g1= A / Sqrt( ONE - flatfn*sin_lat*sin_lat );
	g2= g1*funsq + height;
	g1= g1 + height;

	geo.x = g1 * Cos( lat_rad );
	geo.y = geo.x * Sin( lon_rad );
	geo.x = geo.x * Cos( lon_rad );
	geo.z = g2 * sin_lat;

	return geo;
}

double GPSPosition::metersPerLatitude() {
	// http://calgary.rasc.ca/latlong.htm
	double km = 111.13295 - 0.55982 * Cos(Rad(2.0 * latitude)) + 0.00117 * Cos(Rad(4.0 * latitude));
	return km * 1000.0;
}

double GPSPosition::metersPerLongitude() {
	// http://calgary.rasc.ca/latlong.htm
	// Relies on what our latitude is
	double km = 111.41288 * Cos(Rad(latitude)) - 0.09350 * Cos(Rad(3.0 * latitude)) + 0.00012 * Cos(Rad(5.0 * latitude));
	return km * 1000.0;
}

double GPSPosition::latitudesPerMeter() {
	return 1.0 / metersPerLatitude();
}

double GPSPosition::longitudesPerMeter() {
	return 1.0 / metersPerLongitude();
}

double GPSPosition::latitudesPerMeters(double meters) {
	return (1.0 / metersPerLatitude()) * meters;
}

double GPSPosition::longitudesPerMeters(double meters) {
	return (1.0 / metersPerLongitude()) * meters;
}

double GPSPosition::metersToDestination(GPSPosition dest) {
	// Create referential center point
	GPSPosition refPoint((latitude+dest.latitude)/2.0, (longitude+dest.longitude)/2.0, (height+dest.height)/2.0);
	double dLat_m = (dest.latitude-latitude)*refPoint.metersPerLatitude();
	double dLon_m = (dest.longitude-longitude)*refPoint.metersPerLongitude();
	return Sqrt( dLat_m*dLat_m + dLon_m*dLon_m );
}





