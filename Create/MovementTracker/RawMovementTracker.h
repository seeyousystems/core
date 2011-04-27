/*
 *  RawMovementTracker.h
 *
 *  The RawMovementTracker works with the raw sensor data of the robot. This
 *  means the Controller controls via COIL the robot and determines the
 *  individual wheel speeds. With every process() of the Controller the sensor
 *  data is queried and passed along to the Tracker and eventually the
 *  RawMovementTracker. Within this sensor data is the change in angle and
 *  the distance driven, which is determined by the onboard-controller of the
 *  robot. It is this data which the RawMovementTracker reacts to and performs
 *  its localization via the methods registerChangedAngle(...) and
 *  registerMovedDistance(...).
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

#ifndef RAWMOVEMENTTRACKER_H_
#define RAWMOVEMENTTRACKER_H_

#include "MovementTracker.h"

#include <QMutex>

class RawMovementTracker: public MovementTracker {

Q_OBJECT


public:
	RawMovementTracker(Create *create, long x, long y, double rotation);
	virtual~ RawMovementTracker();
	virtual void registerMovedDistance(double distance);
	virtual void registerChangedAngle(double angle);
	virtual void registerChangedWheelSpeed(int, int);
};

#endif /* RAWMOVEMENTTRACKER_H_ */
