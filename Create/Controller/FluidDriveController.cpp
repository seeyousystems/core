/*
 *  FluidDriveController.cpp
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

#include "FluidDriveController.h"

#include "../COIL/COIL.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

FluidDriveController::FluidDriveController(Create *create, int speed, int interval) :
	Controller("Fluid Drive", create, interval) {

	this->speed = speed;
	this->yokeX = 0.0;
	this->yokeY = 0.0;

}

FluidDriveController::~FluidDriveController() {

}

void FluidDriveController::run() {

	stopRequested = false;
	while (stopRequested == false) {

		// Get movement
		int distanceDelta = create->coil->getDistance();
		int angleDelta = create->coil->getAngle();

		// Emit signals for movement tracker
		emit signalMovedDistance(distanceDelta);
		emit signalChangedAngle(angleDelta);

		// Get other sensor data
		int sharpIRSensor = create->coil->getAnalogSensorDistance();
		int bumpsWheelDrop = create->coil->getBumpsAndWheelDrops();

		// Collision?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
			emit signalCollision();
			if (yokeY > 0) yokeY = 0.0;
			yokeX = 0.0;
		}

		// Object detected?
		if (sharpIRSensor < 500) {
			emit signalObjectDetected(sharpIRSensor, 0); // Angle is 0 because it is strait ahead always!
		}

		// Processs movement
		// Basically here we have a x yoke and y yoke value ranging from -1.0 to 1.0
		// and we want to translate that to the speed of each servo (left and right)
		// relative to the (max)speed given in the constructor...
		short Lwheel = 0;
		short Rwheel = 0;
		short speed = 0;
		if (this->yokeY <= 0.06 && this->yokeY >= -0.06) {
			speed = 150;
			Lwheel = -(short)speed * (this->yokeX);
			Rwheel =  (short)speed * (this->yokeX);
		}
		else {
			short speed = (short) (this->speed * this->yokeY);
			Lwheel = speed - (short)(speed * this->yokeX);
			Rwheel = speed + (short)(speed * this->yokeX);

			// Fix
			if (Lwheel > this->speed) Lwheel = this->speed;
			if (Rwheel > this->speed) Rwheel = this->speed;
			if (Lwheel < -this->speed) Lwheel = -this->speed;
			if (Rwheel < -this->speed) Rwheel = -this->speed;
		}
		create->coil->directDrive(Lwheel, Rwheel);

		Debug::print("[FluidDriveController] Direct drive %1 %2", Lwheel, Rwheel);

		// Sleep our interval...
		this->msleep(interval);

	}
}

void FluidDriveController::setYoke(double yokeX, double yokeY) {
	this->yokeX = yokeX;
	this->yokeY = yokeY;
	Debug::print("[FluidDriveController] Direct drive %1 %2", yokeX, yokeY);

}
