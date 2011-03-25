/*
 *  RawMovementTracker.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include "RawMovementTracker.h"

#include "../Library/Debug.h"

RawMovementTracker::RawMovementTracker(Create *create, long x, long y, double rotation) : MovementTracker("Raw", create) {
	this->transformation = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(rotation)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
}

RawMovementTracker::~RawMovementTracker() {

}

void RawMovementTracker::registerMovedDistance(double dist){
	if(dist != 0) {
		relativeDistance += dist;
		totalDistance += std::abs(dist);
		transformation = transformation * Trafo2D::trans(0, dist); // Move forwards (up/y) in our transformation matrix
		emit moved(this->x(), this->y(), this->rotation());
	}
}

void RawMovementTracker::registerChangedAngle(double angle){
	if(angle != 0) {
		transformation = transformation * Trafo2D::rot(Rad(angle)); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
		emit moved(this->x(), this->y(), this->rotation());
	}
}


void RawMovementTracker::registerCollision() {
	Trafo2D collisionLocation = transformation * Trafo2D::trans(0, create->intSetting("ROBOT_BUMPER_COLLISION_OFFSET_MM"));
	emit collision((long)collisionLocation.trans().x(), (long)collisionLocation.trans().y());
}

void RawMovementTracker::registerObjectDetected(double distance, double angle) {
	Trafo2D objectLocation = transformation * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance); // Core/COIL has a clockwise positive rotation while the math library has a anti-clockwise rotation...
	emit collision((long)objectLocation.trans().x(), (long)objectLocation.trans().y());
}


