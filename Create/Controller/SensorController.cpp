/*
 * SensorController.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#include "SensorController.h"

#include "../COIL/ArduinoCOIL.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"
#include "../MovementTracker/MovementTracker.h"


SensorController::SensorController(Create *create, int speed, int interval) :
	ArduinoController("Arduino", create, interval) {

	this->speed = speed;
	heading = 0;
	leftPinger = 0;
	rightPinger = 0;
	frontIR = 0;
	leftIR = 0;
	rightIR = 0;
	RFID = 0;
	// Welcome VFF Algorithm
	//vffAI = VFF();
}

SensorController::~SensorController() {
}

void SensorController::run()
{
	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {
//		static int x = 0;

		// Get sensor data
//		heading = arduino_getHeading();
		frontIR = arduino_getFrontIR();
		leftIR = arduino_getLeftIR();
		rightIR = arduino_getRightIR();
		leftPinger = arduino_getLeftPinger();
		rightPinger = arduino_getRightPinger();
		//RFID = arduino_getRFID();

		// Emit signals for widgets
//		emit leftSensorSignal(leftPinger);
//		emit upperLeftSensorSignal(leftIR);
//		emit frontSensorSignal(frontIR);
//		emit upperRightSensorSignal(rightIR);
//		emit rightSensorSignal(rightPinger);

		// Get global position
		long x = create->movementTracker->x();
		long y = create->movementTracker->y();

		// Update obstacle avoidance algorithm (we are working with cm not mm)
		//create->vffAI->run();
		create->vffAI->run( leftPinger, rightIR, frontIR, leftIR, rightPinger, x/10, y/10 );
//		printf("[ArduinoController] Left: %3d  UpperLeft: %3d  Front: %3d  UpperRight: %3d  Right: %3d\n", leftPinger, leftIR, frontIR, rightIR, rightPinger);
		//create->vffAI->run();
//		Debug::print("[SensorController] RFID: %1", RFID);
//		Debug::print("Test: %1\n", ++x);
//		printf("[SensorController] %d, %d\n", x/10, y/10);
//		//point.print();
//		printf("%f,%f\n", point.trans().x(), point.trans().y());

		//Debug::print("[ArduinoController] Left: %1  Right: %3  Heading: %4", leftPinger, rightPinger, heading);
		//Debug::print("Heading: %1", heading);
		// Sleep our interval...
		this->msleep(interval);
	}
}

int SensorController::arduino_getHeading()
{
	return (int) create->arduino->readCompass();
}

int SensorController::arduino_getLeftPinger()
{
	return (int) create->arduino->readLeftPinger();
}

int SensorController::arduino_getRightPinger()
{
	return (int) create->arduino->readRightPinger();
}

int SensorController::arduino_getFrontIR()
{
	return (int) create->arduino->readInfraredFront();
}

int SensorController::arduino_getLeftIR()
{
	return (int) create->arduino->readInfraredLeft();
}

int SensorController::arduino_getRightIR()
{
	return (int) create->arduino->readInfraredRight();
}

int SensorController::arduino_getRFID()
{
	return (int) create->arduino->readRFID();
}

void SensorController::arduino_resetSensors()
{
	create->arduino->resetvariables();
}

