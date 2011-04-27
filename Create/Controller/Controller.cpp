/*
 *  Controller.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Controller.h"

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

//#include "../Watchdog/Watchdog.h"
//#include "../Watchdog/ThreadMonitorWatchdogAction.h"


Controller::Controller(QString name, Create *create, int interval) : CoreThread(name, create) {
	this->interval = interval;
	this->targetSpeed = 0;
	this->sensorData = (int*)malloc (36*sizeof(int));
	this->watchdogAction = NULL;

	stopRequested = false;

	// Watchdog this?
//	if(create->watchdog && create->boolSetting("Controller_Watchdog_Enabled")) {
//		watchdogAction = new ThreadMonitorWatchdogAction(QString("%1 Controller").arg(name), interval, create->doubleSetting("Controller_Watchdog_Flexibility"), create);
//		create->watchdog->addAction(watchdogAction);
//	}
}

Controller::~Controller() {
//	if(watchdogAction && create->watchdog) create->watchdog->removeAction(watchdogAction);
//	if(watchdogAction) delete watchdogAction;
	free(sensorData);
}

void Controller::run() {

	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {

		// Lock up for reading...
		lock.lockForRead(); {

			// Call the virtual process method. This must be implemented by a subclass...
			process();

		} lock.unlock(); // Unlock read...

		// Ping our watchdog action and sleep our interval...
//		if(watchdogAction) watchdogAction->ping();
		this->msleep(interval);

	}
}

void Controller::stop() {

	// Request a stop and wait for the controller to exit its process loop.
	// After let the watchdog know we are no longer active...
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(create->intSetting("Controller_StopRequestInterval"));
//	if(watchdogAction) watchdogAction->setActive(false);

	Debug::print("[Controller] %1 stopped", name);

}


void Controller::start(QThread::Priority priority) {

	Debug::print("[Controller] %1 started", name);

//	if(watchdogAction) watchdogAction->setActive(true);
	QThread::start(priority);
}

void  Controller::setTargetSpeed(int speed) {
	lock.lockForWrite(); {
		targetSpeed = speed;
	} lock.unlock();
}


