/*
 *  SelfCorrectingTracker.h
 *
 *  The SelfCorrectingTracker is an advanced Tracker which inherits from
 *  SingleTracker. This Tracker doesn�t change anything the SingleTracker
 *  does, but extends its functionality to be able to make corrections when
 *  deemed applicable. To do this, the SelfCorrectingTracker makes use of
 *  the currently loaded PhysicalMap to map correlations and determine if
 *  the overall localization has peered off in error. If such an error is
 *  determined, it is accordingly corrected by adjusting all the MovementTrackers
 *  positions. This Tracker looks to be promising for future work related to
 *  improving the quality, or accuracy, of the robot localization, and is
 *  described in larger detail than the other Trackers.
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

#ifndef SELFCORRECTINGTRACKER_H_
#define SELFCORRECTINGTRACKER_H_

#include "SingleTracker.h"

struct CorrelationTriangle {
	Trafo2D position;
	Trafo2D collision;
	Trafo2D closestWall;
};

struct SuspectedWall {
	bool				valid;
	CorrelationTriangle start;
	CorrelationTriangle end;
};

class SelfCorrectingTracker : public SingleTracker {

	Q_OBJECT

private:
	QReadWriteLock lock1;								// Used for the correlation log
	QReadWriteLock lock2;								// Used for the suspected wall
	QList<CorrelationTriangle> 	*correlationLog;		// The log of all correlation triangles
	SuspectedWall 				suspectedWall;			// The currently suspected wall

public:
	SelfCorrectingTracker(Create *create);
	~SelfCorrectingTracker();
	QList<CorrelationTriangle> getCorrelationLogSnapshot(int n);
	SuspectedWall getCurrentlySuspectedWall();

public slots:
	virtual void registerObjectDetected(double distance, double angle, double opacity, int size);

};

#endif /* SELFCORRECTINGTRACKER_H_ */
