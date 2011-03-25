/*
 *  FluidDriveController.h
 *
 *  The Fluid Drive Controller is further development of the Block Drive Controller.
 *  It supports control of the robot with a two-dimensional GUI joystick. The
 *  Controller has two yokes, yokeX and yokeY which accept values between -1
 *  and 0. The speed of the robot is determined by the yokeY value, while the
 *  direction of the robot�s movement is set by the yokeX value. The disadvantage
 *  of this controller is that rotations around the center axis of the robot are
 *  not supported due to the nature of the joystick.
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
#ifndef FLUIDDRIVECONTROLLER_H_
#define FLUIDDRIVECONTROLLER_H_

#include "Controller.h"

class FluidDriveController : public Controller
{
	Q_OBJECT

public:
	short speed;
	int interval;
	double yokeX;
	double yokeY;

public:
	FluidDriveController(Create *create, int speed, int interval);
	virtual ~FluidDriveController();
	void run();

public slots:
	void setYoke(double yokeX, double yokeY);

};

#endif /* FLUIDDRIVECONTROLLER_H_ */
