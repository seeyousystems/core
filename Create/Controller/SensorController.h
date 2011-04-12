/*
 * SensorController.h
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

#include "ArduinoController.h"
//#include "../Library/Algorithm/VFF.h"

class SensorController : public ArduinoController {

	Q_OBJECT

private:
	//VFF vffAI;
public:
	short speed;
	int heading;
	int leftPinger;
	int rightPinger;
	int frontIR;
	int leftIR;
	int rightIR;
	int RFID;

public:
	SensorController(Create *create, int speed, int interval);
	virtual ~SensorController();
	virtual void run();

private:
	virtual int arduino_getHeading();
	virtual int arduino_getLeftPinger();
	virtual int arduino_getRightPinger();
	virtual int arduino_getFrontIR();
	virtual int arduino_getLeftIR();
	virtual int arduino_getRightIR();
	virtual int arduino_getRFID();
	virtual void arduino_resetSensors();
};

#endif /* SENSORCONTROLLER_H_ */
