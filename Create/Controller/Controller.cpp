/*
 *  Controller.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include "../COIL/COIL.h"

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

/**
 * Constructs a new Controller.
 *
 * \param name		The name of the controller, used for debugging purposes as well as runtime refletion.
 *
 *
 */
Controller::Controller(QString name, Create *create, int interval) {
	this->name = name;
	this->create = create;
	this->interval = interval;
	globaltag = 0;
	stopRequested = false;
}

Controller::~Controller() {

}

void Controller::stop() {

	// Request a stop and wait for the controller to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(create->intSetting("CONTROLLER_STOP_REQUEST_INTERVAL"));

	Debug::print("[Controller] %1 Controller stopped", name);

}

void Controller::emergencyStop() {
	create->coil->directDrive(0, 0);
}

void Controller::regularStop() {
	//create->coil->directDrive(0, 0);
}

void Controller::tags(int tagnumber){//Added by Hanam Apr 21
	globaltag = tagnumber;
}

int Controller::getTags() {
	return globaltag;
}

void Controller::start(QThread::Priority priority) {

	Debug::print("[Controller] %1 Controller started", name);

	QThread::start(priority);
}

