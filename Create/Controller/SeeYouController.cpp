/*
 * SeeYouController.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: abraham
 */

#include "SeeYouController.h"

#include "../COIL/COIL.h"
#include "../COIL/ArduinoCOIL.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

SeeYouController::SeeYouController(Create *create, int speed, int interval) :
	Controller("SeeYou", create, interval) {

	this->speed = speed;
	yokeX = 0.0;
	yokeY = 0.0;
	angleToTurn = 0.0;
	angleTurned = 0.0;
	distanceToMove = 0.0;
	distanceMoved = 0.0;
	mode = Idle;

}

SeeYouController::~SeeYouController() {

}

void SeeYouController::run() {
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

		// Get sensor data from Arduino
		int frontIRSensor = arduino_getFrontIR();

		// Collision?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
			emit signalCollision();
			if(create->boolSetting("SEEYOUCONTROLLER_EMERGENCY_STOP_ENABLED") == true) emergencyStop();
		}

		// Object detected?
		if (sharpIRSensor < create->intSetting("SEEYOUCONTROLLER_SHARP_IR_SENSOR_CUTOFF_VALUE")) {
			emit signalObjectDetected(sharpIRSensor, 0); // Angle is 0 because it is strait ahead always!
			if(create->boolSetting("SEEYOUCONTROLLER_EMERGENCY_STOP_ENABLED") == true && sharpIRSensor < create->intSetting("SEEYOUCONTROLLER_SHARP_IR_SENSOR_EMERGENCYSTOP_BUFFER_MM")) emergencyStop();
		}

		// Object detected with Arduino Sensors?
		if (frontIRSensor < 10) {
			Debug::print("Less than what we need");
		}

		// Processs movement
		distanceMoved += distanceDelta;
		angleTurned += angleDelta;

		// Determine wheel speeds
		if (mode == SeeYouController::Idle || mode == SeeYouController::EmergencyStop) {

			// Idle mode!

			Lwheel = 0;
			Rwheel = 0;

		} else if (mode == SeeYouController::Joystick) {

			// Joystick mode!

//			if (this->yokeY == 0) {
//				// Left or right
//				Lwheel = (short) (this->speed * this->yokeX);
//				Rwheel = -(short) (this->speed * this->yokeX);
//
//			} else {
//				// Move forwards backwards
//				Lwheel = (short) (this->speed * this->yokeY);
//				Rwheel = (short) (this->speed * this->yokeY);
//			}

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

		} else if (mode == SeeYouController::Move) {

			// Move mode!

			Lwheel = speed;
			Rwheel = speed;

		} else if (mode == SeeYouController::Turn) {

			// Turn mode!

			if (angleToTurn > 0) {
				Lwheel = -speed;
				Rwheel = +speed;
			} else {
				Lwheel = +speed;
				Rwheel = -speed;
			}

		} else if (mode == SeeYouController::WheelDrive) {

			// No change, just drive at current wheel values...
		}

		// Send wheel speeds to COIL
		if(mode != SeeYouController::EmergencyStop) coil_directDrive(Lwheel, Rwheel);

		// Sleep our interval...
		this->msleep(interval);

	}
}

void SeeYouController::emergencyStop() {
	mode = SeeYouController::EmergencyStop;
	coil_directDrive(0, 0);
}

void SeeYouController::setYoke(double yokeX, double yokeY) {
	this->yokeX = yokeX;
	this->yokeY = yokeY;
	mode = SeeYouController::Joystick;
	//Debug::print("[SeeYouController] setYoke x:%1  y:%2", this->yokeX, this->yokeY);
}

void SeeYouController::move(int distance, int speed) {

	if(mode == SeeYouController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = 0;
	angleTurned = 0;
	distanceToMove = distance;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = SeeYouController::Move;
	waitForDistance();
	mode = SeeYouController::Idle;

	//TODO: why is this so inaccurate? What is the timing problem?
}

void SeeYouController::waitForDistance() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((distanceToMove > 0 && distanceMoved < distanceToMove) || (distanceToMove < 0 && distanceMoved > distanceToMove)) {
		SleeperThread::msleep(create->intSetting("SEEYOUCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}
}

void SeeYouController::turn(int angle, int speed) {

	if(mode == SeeYouController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = angle;
	angleTurned = 0;
	distanceToMove = 0;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = SeeYouController::Turn;
	waitForAngle();
	mode = SeeYouController::Idle;
}

void SeeYouController::wheelDrive(short Lwheel, short Rwheel) {

	if(mode == SeeYouController::EmergencyStop) return;

	this->mode = SeeYouController::WheelDrive;
	this->Lwheel = Lwheel;
	this->Rwheel = Rwheel;

}

void SeeYouController::waitForAngle() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((angleToTurn > 0 && angleTurned < angleToTurn) || (angleToTurn < 0 && angleTurned > angleToTurn)) {
		SleeperThread::msleep(create->intSetting("SEEYOUCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}

}

void SeeYouController::coil_directDrive(short Lwheel, short Rwheel) {
	create->coil->directDrive(Lwheel, Rwheel);
}

double SeeYouController::coil_getDistance() {
	return (double) create->coil->getDistance();
}

double SeeYouController::coil_getAngle() {
	return (double) create->coil->getAngle();
}

int SeeYouController::coil_getAnalogSensorDistance() {
	return create->coil->getAnalogSensorDistance();
	return 0;
}

int SeeYouController::coil_getBumpsAndWheelDrops() {
	return create->coil->getBumpsAndWheelDrops();
}

// TODO: Understand this part - Arduino Data
int SeeYouController::arduino_getHeading()
{
	return (int) create->arduino->readCompass();
}

int SeeYouController::arduino_getLeftPinger()
{
	return (int) create->arduino->readLeftPinger();
}

int SeeYouController::arduino_getRightPinger()
{
	return (int) create->arduino->readRightPinger();
}

int SeeYouController::arduino_getFrontIR()
{
	return (int) create->arduino->readInfraredFront();
}

int SeeYouController::arduino_getLeftIR()
{
	return (int) create->arduino->readInfraredLeft();
}

int SeeYouController::arduino_getRightIR()
{
	return (int) create->arduino->readInfraredRight();
}


