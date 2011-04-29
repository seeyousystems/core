/*
 * ArduinoController.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#include "ArduinoController.h"

#include "../COIL/ArduinoCOIL.h"

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

ArduinoController::ArduinoController(QString name, Create *create, int interval) {
	this->name = name;
	this->create = create;
	this->interval = interval;
	this->sensorData = (int*)malloc(8*sizeof(int));
	// TODO Auto-generated constructor stub
	stopRequested = false;
}

ArduinoController::~ArduinoController() {
	// TODO Auto-generated destructor stub
	free(sensorData);
}

void ArduinoController::stop() {

	// Request a stop and wait for the controller to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(create->intSetting("ARDUINOCONTROLLER_STOP_REQUEST_INTERVAL"));

	Debug::print("[ArduinoController] %1 Controller stopped", name);
}

void ArduinoController::start(QThread::Priority priority) {

	Debug::print("[ArduinoController] %1 Controller started", name);

	QThread::start(priority);
}

int ArduinoController::getHeading()
{
	return sensorPacket.heading;
}

int ArduinoController::getRFID()
{
	return sensorPacket.RFID;
}
