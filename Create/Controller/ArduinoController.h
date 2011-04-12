/*
 * ArduinoController.h
 *
 *  Created on: Mar 15, 2011
 *      Author: abraham
 */

#ifndef ARDUINOCONTROLLER_H_
#define ARDUINOCONTROLLER_H_

#include <QObject>
#include <QThread>
#include <QQueue>

#include "../create.h"

class ArduinoController : public QThread
{
	Q_OBJECT

protected:
	bool stopRequested;

public:
	Create *create;
	QString name;
	int interval;

public:
	ArduinoController(QString name, Create *create, int interval);
	virtual ~ArduinoController();
	virtual void run() = 0;
	void stop();
	void start(QThread::Priority priority);

signals:
	void signalObjectDetected(double distance, double angle);
	/* Signals for sensors */
	void leftSensorSignal(int left);
	void upperLeftSensorSignal(int upperLeft);
	void frontSensorSignal(int front);
	void upperRightSensorSignal(int upperRight);
	void rightSensorSignal(int right);
};

#endif /* ARDUINOCONTROLLER_H_ */
