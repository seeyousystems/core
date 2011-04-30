/*
 *  CorrectedFrenetMovementTracker.cpp
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

#include "CorrectedFrenetMovementTracker.h"

#include "../Library/Debug.h"
#include "../Library/Math.h"




CorrectedFrenetMovementTracker::CorrectedFrenetMovementTracker(Create *create, long x, long y, double rotation) : FrenetMovementTracker("CorrectedFrenetMovementTracker", create, x, y, rotation) {
	evl = create->doubleSetting("Tracker_CorrectedFrenetMovementTracker_vl");
	evr = create->doubleSetting("Tracker_CorrectedFrenetMovementTracker_vr");
	dv = create->intSetting("Tracker_CorrectedFrenetMovementTracker_dv");
}

CorrectedFrenetMovementTracker::~CorrectedFrenetMovementTracker() {

}


void CorrectedFrenetMovementTracker::registerChangedWheelSpeed(int left, int right){

	lock.lockForWrite(); {

		// Fix the input speeds to reflect how the hardware would handle this.
		// If the difference in speeds is small, then we mimick the way the
		// hardware ignores these small values...
		if(dv != 0 && abs(left-right) < dv) {
			left = (left+right)/2;
			right = (left+right)/2;
		}

		// Process the movement for this elapsed time
		processMovementByTime((double)lastChangedWheelSpeedTimestamp.restart(), (double)left + (double)left*evl, (double)right + (double)right*evr);
		emit moved(transformation.trans().x(),transformation.trans().y(),Deg(transformation.angle()));

	} lock.unlock();

}

