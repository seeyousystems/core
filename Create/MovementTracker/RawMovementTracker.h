/*
 *  RawMovementTracker.h
 *
 *  The Raw Movement Tracker tracks the robot�s movement by geometrically
 *  interpreting the sensor data sent back from the robot. This method
 *  remains accurate (at least as accurate as the sensor data) as long as
 *  the robot�s movement is linear, meaning that the wheels were not
 *  differentially driven. When a differential steering system is made
 *  use of, the accuracy of the tracker declines as the sensor interval
 *  increases.
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

#ifndef RAWMOVEMENTTRACKER_H_
#define RAWMOVEMENTTRACKER_H_

#include "MovementTracker.h"

class RawMovementTracker: public MovementTracker {

Q_OBJECT

private:


public:
	RawMovementTracker(Create *create, long x, long y, double rotation);
	virtual~ RawMovementTracker();

public slots:
	virtual void registerMovedDistance(double distance);
	virtual void registerChangedAngle(double angle);
	virtual void registerCollision();
	virtual void registerObjectDetected(double distance, double angle);
};

#endif /* RAWMOVEMENTTRACKER_H_ */
