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
	sensorPacket.heading = 0;
	sensorPacket.leftPinger = 0;
	sensorPacket.rightPinger = 0;
	sensorPacket.frontIR = 0;
	sensorPacket.leftIR = 0;
	sensorPacket.rightIR = 0;
	sensorPacket.RFID = 0;
//	// Welcome VFF Algorithm
//	//vffAI = VFF();
}

SensorController::~SensorController() {
}

void SensorController::run()
{
	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {
//		static int x = 0;



		if(sensorData) {
			if(create->arduino->gellAllSensors(sensorData)) {
				// emit some signal
			}
			else {
				Debug::print("[SensorController] Error with sensorData stuff");
			}
		}
		sensorPacket.heading = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_COMPASS);
		sensorPacket.frontIR = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_IR_2);
		sensorPacket.leftIR = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_IR_3);
		sensorPacket.rightIR = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_IR_1);
		sensorPacket.leftPinger = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_LEFT_PINGER);
		sensorPacket.rightPinger = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_RIGHT_PINGER);
		sensorPacket.RFID = create->arduino->extractSensorFromData(sensorData, ArduinoCOIL::SENSOR_RFID);


		// Get sensor data
//		sensorPacket.heading = arduino_getHeading();
//		sensorPacket.frontIR = arduino_getFrontIR();
//		sensorPacket.leftIR = arduino_getLeftIR();
//		sensorPacket.rightIR = arduino_getRightIR();
//		sensorPacket.leftPinger = arduino_getLeftPinger();
//		sensorPacket.rightPinger = arduino_getRightPinger();
//		sensorPacket.RFID = arduino_getRFID();
////
//		mutex.lock();
//		previousPacket = sensorPacket;
//		mutex.unlock();
//		// Emit signals for widgets
//		emit leftSensorSignal(leftPinger);
//		emit upperLeftSensorSignal(leftIR);
//		emit frontSensorSignal(frontIR);
//		emit upperRightSensorSignal(rightIR);
//		emit rightSensorSignal(rightPinger);

		// Get global position
//		long x = create->movementTracker->x();
//		long y = create->movementTracker->y();

		// Update obstacle avoidance algorithm (we are working with cm not mm)
		//create->vffAI->run();
		//create->vffAI->run( leftPinger, rightIR, frontIR, leftIR, rightPinger, x/10, y/10 );
		if(create->boolSetting("SensorController_Debug"))
			printf("[ArduinoController] Left: %3d  UpperLeft: %3d  Front: %3d  UpperRight: %3d  Right: %3d  Heading: %d\n", sensorPacket.leftPinger, sensorPacket.leftIR, sensorPacket.frontIR, sensorPacket.rightIR, sensorPacket.rightPinger, sensorPacket.heading);
		//create->vffAI->run();
//		Debug::print("[SensorController] RFID: %1", sensorPacket.RFID);
//		Debug::print("Test: %1\n", ++x);
//		printf("[SensorController] %d, %d\n", x/10, y/10);
//		//point.print();
//		printf("%f,%f\n", point.trans().x(), point.trans().y());

	//	Debug::print("[ArduinoController] Left: %1  Middle: %2  Right: %3 ", sensorPacket.leftIR, sensorPacket.frontIR, sensorPacket.rightIR);
	//	Debug::print("[ArduinoController] Left: %1  Right: %2  RFID: %3 ", sensorPacket.leftPinger, sensorPacket.rightPinger, sensorPacket.RFID);
		//Debug::print("Heading: %1", heading);
		// Sleep our interval...
		this->msleep(interval);
	}
}

int SensorController::arduino_getHeading()
{
//	mutex.lock();
//	int t = (int) create->arduino->readCompass();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readCompass();
}

int SensorController::arduino_getLeftPinger()
{
//	mutex.lock();
//	int t = (int) create->arduino->readLeftPinger();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readLeftPinger();
}

int SensorController::arduino_getRightPinger()
{
//	mutex.lock();
//	int t = (int) create->arduino->readRightPinger();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readRightPinger();
}

int SensorController::arduino_getFrontIR()
{
//	mutex.lock();
//	int t =  (int) create->arduino->readInfraredFront();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readInfraredFront();
}

int SensorController::arduino_getLeftIR()
{
//	mutex.lock();
//	int t = (int) create->arduino->readInfraredLeft();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readInfraredLeft();

}

int SensorController::arduino_getRightIR()
{
//	mutex.lock();
//	int t = (int) create->arduino->readInfraredRight();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readInfraredRight();
}

int SensorController::arduino_getRFID()
{
//	mutex.lock();
//	int t = (int) create->arduino->readRFID();
//	mutex.unlock();
//	return t;
	return (int) create->arduino->readRFID();
}

void SensorController::arduino_resetSensors()
{
	create->arduino->resetvariables();
}

//void SensorController::getHeading()
//{
//	return sensorPacket.heading;
//}
//
//void SensorController::getRFID()
//{
//	return sensorPacket.RFID;
//}
