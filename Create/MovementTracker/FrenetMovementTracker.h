/*
 *  FrenetMovementTracker.h
 *
 *  Taking the ExpectedMovementTracker to the next step, the FrenetMovementTracker
 *  implements a mathematically sound tracker for the differential steering system
 *  of the robot. This Tracker uses the theory Jean Fr�d�ric Frenet (Frenet, 1847)
 *  and Joseph Alfred Serret (Serret, 1851) formulated in the 19th century regarding
 *  kinematic properties of movement along a continuous, differentiable curve in
 *  three-dimensional Euclidean space R^3. The resulting formulas, now known as the
 *  Frenet-Serret formulas, specifically describe the derivatives of the tangent t,
 *  normal n, and binormal b unit vectors in terms of each other.
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

#ifndef FRENETMOVEMENTTRACKER_H_
#define FRENETMOVEMENTTRACKER_H_

#include "MovementTracker.h"

#include <QTime>

class FrenetMovementTracker : public MovementTracker {

	Q_OBJECT

protected:
	QTime lastChangedWheelSpeedTimestamp;

public:
	FrenetMovementTracker(Create *create, long x, long y, double rotation);
	FrenetMovementTracker(QString name, Create *create, long x, long y, double rotation);
	virtual~ FrenetMovementTracker();
	virtual void registerMovedDistance(double distance);
	virtual void registerChangedAngle(double angle);
	virtual void registerChangedWheelSpeed(int, int);

protected:
	void processMovementByTime(double t, double vl, double vr);

};

#endif /* FRENETMOVEMENTTRACKER_H_ */
