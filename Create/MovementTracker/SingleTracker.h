/*
 *  SingleTracker.h
 *
 *  A SingleTracker may contain one or more MovementTrackers but only uses a single
 *  MovementTracker for determining the robots position. The MovementTracker which
 *  is used is specified in the Core configuration file (see 0), and can be changed
 *  during runtime with the method setSelectedMovemenTracker(...).
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

#ifndef SINGLETRACKER_H_
#define SINGLETRACKER_H_

#include "Tracker.h"

class SingleTracker : public Tracker {

	Q_OBJECT

private:
	MovementTracker *selectedMovementTracker; // The currently selected tracker used for returning data


public:
	SingleTracker(Create *create, QString name = "SingleTracker");
	~SingleTracker();
	void setSelectedMovementTracker(MovementTracker *movementTracker);
	void setSelectedMovementTracker(QString name);
	MovementTracker* getSelectedMovementTracker();

private:
	virtual Trafo2D calculateTransformation();
	virtual double calculateTotalDistance();
	virtual double calculateTotalAngle();
};

#endif /* SINGLETRACKER_H_ */
