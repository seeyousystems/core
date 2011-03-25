/*
 *  EmssController.cpp
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

#include "EmssController.h"

#include "../COIL/COIL.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

EmssController::EmssController(Create *create, int speed, int interval) :
	Controller("emss", create, interval) {

	this->speed = speed;
	yokeX = 0.0;
	yokeY = 0.0;
	angleToTurn = 0.0;
	angleTurned = 0.0;
	distanceToMove = 0.0;
	distanceMoved = 0.0;
	mode = Idle;

}

EmssController::~EmssController() {

}

void EmssController::run() {
	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {

		// Get movement...
		double distanceDelta = coil_getDistance();
		double angleDelta = coil_getAngle();

		// Emit signals for movement tracker
		emit signalMovedDistance(distanceDelta);
		emit signalChangedAngle(angleDelta);

		// Get other sensor data
		int sharpIRSensor = coil_getAnalogSensorDistance();
		int bumpsWheelDrop = coil_getBumpsAndWheelDrops();

		// Collision?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
			emit signalCollision();
			if(create->boolSetting("EMSSCONTROLLER_EMERGENCY_STOP_ENABLED") == true) emergencyStop();
		}

		// Object detected?
		if (sharpIRSensor < create->intSetting("EMSSCONTROLLER_SHARP_IR_SENSOR_CUTOFF_VALUE")) {
			emit signalObjectDetected(sharpIRSensor, 0); // Angle is 0 because it is strait ahead always!
			if(create->boolSetting("EMSSCONTROLLER_EMERGENCY_STOP_ENABLED") == true && sharpIRSensor < create->intSetting("EMSSCONTROLLER_SHARP_IR_SENSOR_EMERGENCYSTOP_BUFFER_MM")) emergencyStop();
		}

		// Processs movement
		distanceMoved += distanceDelta;
		angleTurned += angleDelta;

		// Determine wheel speeds
		if (mode == EmssController::Idle || mode == EmssController::EmergencyStop) {

			// Idle mode!

			Lwheel = 0;
			Rwheel = 0;

		} else if (mode == EmssController::Joystick) {

			// Joystick mode!

			if (this->yokeY == 0) {
				// Left or right
				Lwheel = (short) (this->speed * this->yokeX);
				Rwheel = -(short) (this->speed * this->yokeX);

			} else {
				// Move forwards backwards
				Lwheel = (short) (this->speed * this->yokeY);
				Rwheel = (short) (this->speed * this->yokeY);
			}

		} else if (mode == EmssController::Move) {

			// Move mode!

			Lwheel = speed;
			Rwheel = speed;

		} else if (mode == EmssController::Turn) {

			// Turn mode!

			if (angleToTurn > 0) {
				Lwheel = -speed;
				Rwheel = +speed;
			} else {
				Lwheel = +speed;
				Rwheel = -speed;
			}
			//Debug::print("[EmssController] debug");


		} else if (mode == EmssController::WheelDrive) {

			// No change, just drive at current wheel values...
		}

		// Send wheel speeds to COIL
		if(mode != EmssController::EmergencyStop) coil_directDrive(Lwheel, Rwheel);


		//Debug::print("[EmssController] debug");
		// Sleep our interval...
		this->msleep(interval);

	}
}

void EmssController::emergencyStop() {
	mode = EmssController::EmergencyStop;
	coil_directDrive(0, 0);
}

void EmssController::setYoke(double yokeX, double yokeY) {
	this->yokeX = yokeX;
	this->yokeY = yokeY;
	mode = EmssController::Joystick;
}

void EmssController::move(int distance, int speed) {

	if(mode == EmssController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = 0;
	angleTurned = 0;
	distanceToMove = distance;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = EmssController::Move;
	waitForDistance();
	mode = EmssController::Idle;

	//TODO: why is this so inaccurate? What is the timing problem?
}

void EmssController::waitForDistance() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((distanceToMove > 0 && distanceMoved < distanceToMove) || (distanceToMove < 0 && distanceMoved > distanceToMove)) {
		SleeperThread::msleep(create->intSetting("EMSSCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}
}

void EmssController::turn(double angle, int speed) {

	if(mode == EmssController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = angle;
	angleTurned = 0;
	distanceToMove = 0;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = EmssController::Turn;
	waitForAngle();
	mode = EmssController::Idle;
}

void EmssController::wheelDrive(short Lwheel, short Rwheel) {

	if(mode == EmssController::EmergencyStop) return;

	this->mode = EmssController::WheelDrive;
	this->Lwheel = Lwheel;
	this->Rwheel = Rwheel;

}

void EmssController::waitForAngle() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((angleToTurn > 0 && angleTurned < angleToTurn) || (angleToTurn < 0 && angleTurned > angleToTurn)) {
		SleeperThread::msleep(create->intSetting("EMSSCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}

}

void EmssController::coil_directDrive(short Lwheel, short Rwheel) {
	create->coil->directDrive(Lwheel, Rwheel);
}

double EmssController::coil_getDistance() {
	return (double) create->coil->getDistance();
}

double EmssController::coil_getAngle() {
	return (double) create->coil->getAngle();
}

int EmssController::coil_getAnalogSensorDistance() {
	return create->coil->getAnalogSensorDistance();
}

int EmssController::coil_getBumpsAndWheelDrops() {
	return create->coil->getBumpsAndWheelDrops();
}
