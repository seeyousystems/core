/*
 *  FrenetMovementTracker.h
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

#include "FrenetMovementTracker.h"

#include "../Library/Debug.h"
#include "../Library/Math.h"


// Values needed for curvature function and modified by gui
// These have to be static because of the function pointer in FrenetT
double _vl = 1.0;
double _vr = 1.0;
double _d = 1.0; // diameter

// Our curvature function based on [KrusiGrob08]
double k (double t) {
	// k(t) = ( 2 / d ) * ( vr(t) - vl(t) ) / ( vr(t) + vl(t) )
	return -( 2.0 / _d ) * ( _vr - _vl ) / ( _vr + _vl );
}


FrenetMovementTracker::FrenetMovementTracker(Create *create, long x, long y, double rotation) : MovementTracker("FrenetMovementTracker", create) {
	transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation));
	lastChangedWheelSpeedTimestamp = QTime();
	weight = 1;
	_d = create->doubleSetting("Robot_WheelOffset_mm") * 2;
}

FrenetMovementTracker::FrenetMovementTracker(QString name, Create *create, long x, long y, double rotation) : MovementTracker(name, create) {
	transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation));
	lastChangedWheelSpeedTimestamp = QTime();
	weight = 1;
	_d = create->doubleSetting("Robot_WheelOffset_mm") * 2;
}

FrenetMovementTracker::~FrenetMovementTracker() {

}


void FrenetMovementTracker::registerChangedWheelSpeed(int left, int right){

	lock.lockForWrite(); {

		// Process the movement for this elapsed time
		processMovementByTime((double)lastChangedWheelSpeedTimestamp.restart(), (double)left, (double)right);
		emit moved(transformation.trans().x(),transformation.trans().y(),Deg(transformation.angle()));

	} lock.unlock();
}

void FrenetMovementTracker::processMovementByTime(double t, double vl, double vr) {

	// t: elapsed time (ms)
	// vl: ground speed left wheel (mmps)
	// vr: ground speed right wheel (mmps)
	// ea: angle error (%)

	// We need to return a value (mm) based on the wheel speed (mm/s) and elapsed time (ms)...
	double v = (vl + vr) / 2.0; // Average it out
	double distance = (v * (t / 1000.0));
	double angle = 0.0;

	// Skip the calculation when v == 0
	if(v != 0) {

		// Set init values for frenet formulas
		_vl = vl;
		_vr = vr;
		double hcount = 100; // move to t with h hcount times
		double h = (t/1000.0)/hcount; // h value for runge-kutta
		double phi = Rad(90); // straight ahead along the y-axis (we do calculations on a local scale...
		DiffEqVectorD y0 = DiffEqVector<double>(0, 0, Cos(phi)*v, Sin(phi)*v, Sin(phi)*v, -Cos(phi)*v);

		// Calculate new position using frenet's formulas and our excelent FrenetT class.
		// We will use the ApproximationMethodRungeKutta but from our initial tests
		// ApproximationMethodEuler is also good.
		FrenetD frenet(k, y0);
		DiffEqVectorD newPosition = frenet.ApproximateDiffEqVector(FrenetD::ApproximationMethodRungeKutta, t/1000.0, h);
		transformation = transformation * Trafo2D::trans(newPosition.x0(), newPosition.y0()) ;

		relativeDistance += distance;
		totalDistance += std::abs(distance);
	}

	// Calculate change of angle using physics per [KrusiGrob08] pg 36...
	if (vl != vr) {
		if(vl != 0) {
			// Based the radius of the circular motion on the left wheel. If emulatedLwheel is zero,
			// then we cannot do this because it is not a circle and the change in angle is infinately large...
			double arclenL = vl * (t / 1000.0);
			double radiusL = (vl*_d) / (vr-vl);
			angle = arclenL / radiusL;
		} else {
			// If we are here, then emulatedLwheel is zero so we simplify angleAlpgha accordingly
			double arclenR = vr * (t / 1000.0);
			angle = arclenR / _d;
		}
	}

	if(angle != 0) {
		transformation = transformation * Trafo2D::rot(angle);
		relativeAngle += Deg(angle);
		totalAngle += std::abs(Deg(angle));
	}
}

void FrenetMovementTracker::registerMovedDistance(double dist){
	// No implementation needed
}

void FrenetMovementTracker::registerChangedAngle(double angle){
	// No implementation needed
}




