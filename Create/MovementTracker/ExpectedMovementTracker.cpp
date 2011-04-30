/*
 *  ExpectedMovementTracker.h
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

#include "ExpectedMovementTracker.h"

#include "../Library/Debug.h"

ExpectedMovementTracker::ExpectedMovementTracker(Create *create, long x, long y, double rotation) : MovementTracker("ExpectedMovementTracker", create) {
	transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
	lastChangedWheelSpeedTimestamp = QTime();
	weight = 1;
	d = create->doubleSetting("Robot_WheelOffset_mm") * 2;
}

ExpectedMovementTracker::~ExpectedMovementTracker() {

}


void ExpectedMovementTracker::registerChangedWheelSpeed(int left, int right){

	lock.lockForWrite(); {

		// Get the elapsed time since last call
		double t = (double)lastChangedWheelSpeedTimestamp.restart();

		// We need to return a value (mm) based on the wheel speed (mm/s) and elapsed time (ms)...
		double vl = (double)left;
		double vr = (double)right;
		double speed = (vl + vr) / 2.0; // Average it out
		double distance = (speed * (t / 1000.0));
		double angle = 0.0;

		// Calculate change of angle using physics per [KrusiGrob08] pg 36...

		if (vl != vr) {
			if(vl != 0) {
				// Based the radius of the circular motion on the left wheel. If emulatedLwheel is zero,
				// then we cannot do this because it is not a circle and the change in angle is infinately large...
				double arclenL = vl * (t / 1000.0);
				double radiusL = (vl*d) / (vr-vl);
				angle = arclenL / radiusL;
			} else {
				// If we are here, then emulatedLwheel is zero so we simplify angleAlpgha accordingly
				double arclenR = vr * (t / 1000.0);
				angle = arclenR / d;
			}
		}

		if(angle != 0 || distance != 0) {
			// TODO: This is inaccurate due to distance. Yes, we knew that, but actually we could do
			// the correct transformation but just following the logical diagram we drew in [KrusiGrob08]
			// without the need for distance! All we have to do is translate R+1/2d to the left, then
			// rotate the angle, and then translate back right R+1/2d !!!! Damn so simple!
			transformation = transformation * Trafo2D::trans(0, distance);
			transformation = transformation * Trafo2D::rot(angle);

			// Register total/relative dist/angle...
			relativeDistance += distance;
			totalDistance += std::abs(distance);
			relativeAngle += Deg(angle);
			totalAngle += std::abs(Deg(angle));


		}

		emit moved(transformation.trans().x(),transformation.trans().y(),Deg(transformation.angle()));

	} lock.unlock();

}

void ExpectedMovementTracker::registerMovedDistance(double dist){
	// No implementation needed
}

void ExpectedMovementTracker::registerChangedAngle(double angle){
	// No implementation needed
}




