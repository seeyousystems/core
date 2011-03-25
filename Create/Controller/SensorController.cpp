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

SensorController::SensorController(Create *create, int speed, int interval) :
	ArduinoController("Arduino", create, interval) {

	this->speed = speed;
	heading = 0;
	leftPinger = 0;
	rightPinger = 0;
	frontIR = 0;
}

SensorController::~SensorController() {
}

void SensorController::run()
{
	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {

		//heading = arduino_getHeadign();

		//Debug::print("[ArduinoController] Heading %1", heading);

		// Sleep our interval...
		this->msleep(interval);
	}
}

int SensorController::arduino_getHeadign()
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
