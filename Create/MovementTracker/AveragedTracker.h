/*
 *  AveragedTracker.h
 *
 *  Unlike the SingleTracker, the AveragedTracker makes use of multiple
 *  MovementTrackers at the same time. The resulting localization is the
 *  mean, or average, of all the MovementTrackers. Additionally, each
 *  MovementTracker may have an assigned weight, which will skew the
 *  influence of the MovementTracker on the resulting average.
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

#ifndef AVERAGEDTRACKER_H_
#define AVERAGEDTRACKER_H_

#include "Tracker.h"

class AveragedTracker : public Tracker {

	Q_OBJECT

public:
	AveragedTracker(Create *create);
	~AveragedTracker();

private:
	virtual Trafo2D calculateTransformation();
	virtual double calculateTotalDistance();
	virtual double calculateTotalAngle();
};

#endif /* AVERAGEDTRACKER_H_ */
