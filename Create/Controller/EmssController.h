/*
 *  EmssController.h
 *
 *  The role of the EmssController is relatively simple: continuously and
 *  frequently retrieve sensor data and send movement commands while making
 *  sure to operate safely at all times. The main method for controlling
 *  the robot with the EmssController implementation is via the
 *  setWheelSpeed(...) method. This allows a Core module to set the desired
 *  wheel speed which is then transmitted by the controller to the hardware.
 *  If an unsafe movement is detected, such as driving down a drop or against
 *  a wall, the Controller will ignore any further commands until a reversing
 *  command is desired which is deemed safe. The EmssController will never take
 *  any counter-action to an unsafe command, it will just ignore the command.
 *  This delegates the resolving of an unsafe situation to other modules such
 *  as the Navigation module.
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
#ifndef EMSSCONTROLLER_H_
#define EMSSCONTROLLER_H_


#include "Controller.h"

#include <QTime>

class EmssController: public Controller {

	Q_OBJECT

public:
	// Variables
	enum { Idle, WheelDrive, EmergencyStop } mode;
	short Lwheel;
	short Rwheel;
	QTime lastDebugInfo;

	// Configurations / properties / settings
	bool debugInfoEnabled;
	int debugInfoInterval;
	int bumperCollisionOffset;
	double cliffCollisionOpacity;
	int cliffCollisionSize;
	double bumperCollisionOpacity;
	int bumperCollisionSize;
	bool emergencyStopEnabled;
	double robotWallSensorRange;
	int robotDiameter;
	double wallCollisionOpacity;
	int wallCollisionSize;
	double irCollisionOpacity;
	int irCollisionSize;
	double openAreaOpacity;
	int openAreaSize;

public:
	EmssController(Create *create, int speed, int interval);
	~EmssController();
	virtual void process();

private:
	virtual void emergencyStop();

public slots:
	virtual void setWheelSpeed(int Lwheel, int Rwheel);

};

#endif /* EMSSCONTROLLER_H_ */
