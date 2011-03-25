/*
 *  BlockDriveController.h
 *
 *  The exact purpose of the Block Drive Controller is encoded in its name.
 *  It provides a function for moving forward, moving backward, turning right,
 *  and turning left with a predefined speed, angle, and distance. The run()
 *  method of the Controller thread executes the set action and sends the
 *  corresponding commands to COIL. The Block Drive Controller also offers an
 *  option for a slow start. Slow start can be set to linear or square. This
 *  means that the speed increases to its predefined value in linear or square
 *  fashion. This is attached for more exactness of driven distance. The
 *  disadvantage of this Controller is its blocking nature. This means that
 *  calling move(speed,distance) with speed set to 100 mm/s and distance set to
 *  1000 mm will make the calling thread block for one second.
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#ifndef BLOCKDRIVECONTROLLER_H_
#define BLOCKDRIVECONTROLLER_H_

#include "Controller.h"

class BlockDriveController : public Controller
{
	Q_OBJECT


public:
	int speed;
	int angle;
	int distance;
	enum SlowStartMode { Off, Linear, Square } slowStart;
	enum Action { Idle, TurnLeft, TurnRight, MoveForward, MoveBackward } action;


public:
	BlockDriveController(Create *create, int interval, int speed, int angle, int distance, SlowStartMode slowStart);
	virtual ~BlockDriveController();
	void run();

// Controller specific functions
public:
	int move(int speed, int distance);
	double calculateAccuracy(int expected, int result);

public slots:
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void setSpeed(int value) { this->speed = value; };
	void setAngle(int value) { this->angle = value; };
	void setDistance(int value) { this->distance = value; };
	void setSlowStartMode(QString mode);


};


#endif /* BLOCKDRIVECONTROLLER_H_ */
