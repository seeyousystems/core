/*
 * SensorController.h
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

#include "ArduinoController.h"
#include <QMutex>
//#include "../Library/Algorithm/VFF.h"

class SensorController : public ArduinoController {

	Q_OBJECT

private:
	//VFF vffAI;
	QMutex mutex;

public:
	short speed;

public:
	SensorController(Create *create, int speed, int interval);
	virtual ~SensorController();
	virtual void run();
//	virtual sensor_oi getSensorPacket();

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
