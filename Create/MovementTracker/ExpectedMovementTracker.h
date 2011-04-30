/*
 *  ExpectedMovementTracker.h
 *
 *  The ExpectedMovementTracker is based on our previous work (Kr�si & Grob, 2008)
 *  related to the movement of a differential steering system over time. The aim
 *  of the ExpectedMovementTracker is to perform localization based solely on
 *  what one would expect the hardware to do, that is, mathematically. While
 *  the results are very satisfactory, the ExpectedMovementTracker however is
 *  not mathematically sound � it is only an approximation.
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

#ifndef EXPECTEDMOVEMENTTRACKER_H_
#define EXPECTEDMOVEMENTTRACKER_H_

#include "MovementTracker.h"

#include <QTime>

class ExpectedMovementTracker : public MovementTracker {

	Q_OBJECT

private:
	QTime lastChangedWheelSpeedTimestamp;
	double d;

public:
	ExpectedMovementTracker(Create *create, long x, long y, double rotation);
	virtual~ ExpectedMovementTracker();
	virtual void registerMovedDistance(double distance);
	virtual void registerChangedAngle(double angle);
	virtual void registerChangedWheelSpeed(int, int);
};
#endif /* EXPECTEDMOVEMENTTRACKER_H_ */
