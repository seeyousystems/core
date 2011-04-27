/*
 *  Controller.h
 *
 *  The Controller module is responsible for safely controlling and maneuvering
 *  the hardware. It sends the control commands to COIL which in turn translates
 *  and transmits the commands over the serial interface. The Controller module
 *  is also responsible for receiving sensory data and providing it to other
 *  modules within the emss Core, such as the Tracker module and various Maps.
 *  The Controller class inherits from CoreThread and runs in its own thread.
 *  Implementations of the Controller class must implement the methods process()
 *  and setWheelSpeed(�). The method emergencyStop() must be implemented to
 *  handle an emergency stop in further detail.
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <QQueue>
#include <QReadWriteLock>

#include "../CoreThread.h"
#include "../Library/Math.h"

class ThreadMonitorWatchdogAction;

class Controller : public CoreThread
{
	Q_OBJECT

protected:
	bool stopRequested;
	ThreadMonitorWatchdogAction *watchdogAction;
	QReadWriteLock lock;

public:
	int interval;
	int targetSpeed; // The target speed used by the controller and other components (such as Weights)
	int *sensorData; // Pointer to sensor data which is 36*sizeof(int)

public:
	Controller(QString name, Create *create, int interval);
	virtual ~Controller();
	virtual void process() = 0; // Process method for thread, must be implemented by sub-class
	virtual void emergencyStop() = 0; // Public emergencyStop method, must be implemented by sub-class
	void run();
	void stop();
	void start(QThread::Priority priority);

signals:
/* Signals for movement notification for a MovementTracker object */
	void signalMovedDistance(double distance);
	void signalChangedAngle(double angle);
	void signalCollision();
	void signalObjectDetected(double distance, double angle, double opacity, int size);
	void signalSensorDataUpdated();
	void signalChangedWheelSpeed(int, int);

public slots:
	virtual void setWheelSpeed(int Lwheel, int Rwheel) = 0; // Sets the current wheel speed, must be implemented by sub-class
	virtual void setTargetSpeed(int speed); // Sets the controllers target speed, may be implemented by sub-class
};

#endif /* CONTROLLER_H_ */
