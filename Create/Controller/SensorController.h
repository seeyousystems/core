/*
 * SensorController.h
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

#include "ArduinoController.h"

class SensorController : public ArduinoController {

	Q_OBJECT
public:
	short speed;
	int heading;
	int leftPinger;
	int rightPinger;
	int frontIR;
	int leftIR;
	int rightIR;


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
};

#endif /* SENSORCONTROLLER_H_ */
