/*
 *  CorrectedFrenetMovementTracker.h
 *
 *  The CorrectedFrenetMovemenTracker extends the FrenetMovemenTracker by
 *  inserting predefined error-values in all movements. This Tracker is used
 *  to mimic any error which is prevalent in the hardware. For example, if the
 *  hardware always tends to peer right, the CorrectedFrenetMovemenTracker can
 *  be used to create a Tracker which represents this behavior of the hardware.
 *  The Callibration TestMoveTask helps in determining the error values needed
 *  by the CorrectedFrenetMovemenTracker.
 *
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


#ifndef CORRECTEDFRENETMOVEMENTTRACKER_H_
#define CORRECTEDFRENETMOVEMENTTRACKER_H_

#include "FrenetMovementTracker.h"

class CorrectedFrenetMovementTracker : public FrenetMovementTracker {

	Q_OBJECT

private:
	double evl;	// The vl error in percent of vl to be added to the vl
	double evr;	// The vr error in percent of vr to be added to the vr
	int dv; // The minimum speed difference for a noticable change on the hardware.

public:
	CorrectedFrenetMovementTracker(Create *create, long x, long y, double rotation);
	virtual~ CorrectedFrenetMovementTracker();
	virtual void registerChangedWheelSpeed(int, int);
};


#endif /* CORRECTEDFRENETMOVEMENTTRACKER_H_ */
