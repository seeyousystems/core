/*
 *  OrientationWeight.h
 *
 *  The OrientationWeight influences the channel value, or left and right wheel
 *  speed, so that the robot will orientate itself towards the next navigational
 *  point. To do this, the current position of the robot must be determined as
 *  well as the position of the destination.
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

#ifndef ORIENTATIONWEIGHT_H_
#define ORIENTATIONWEIGHT_H_

#include "Weight.h"

#include "../../MovementTracker/Tracker.h"
#include "../../Navigation/Navigation.h"
#include "../../Map/MapObject/MarkerMapObject.h"
#include "../../Library/Debug.h"

class OrientationWeight : public Weight {

	Q_OBJECT;

public:
	OrientationWeight(Create *create) : Weight("OrientationWeight", create) {};

	virtual ~OrientationWeight() {};

	virtual void process(Vector2D &v) {
		Vector2D position = create->tracker->getTranslation();
		Vector2D destination = create->navigation->getCurrentDestination();
		double alpha = -Deg(angle(position,destination)) - create->tracker->getRotation();
		if(alpha > 180.0) alpha = -360.0 + alpha;
		if(alpha < -180.0) alpha = 360.0 + alpha;
		v = v + Vector2D( (-alpha/180.0)/1.0 , (+alpha/180.0)/1.0 );
	};
};

#endif /* ORIENTATIONWEIGHT_H_ */
