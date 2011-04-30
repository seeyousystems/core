/*
 *  ControllerSpeedWeed.h
 *
 *  The ControllerSpeedWeight is only needed to move the weight-channel
 *  from a spectrum of 0..1 to 0..v_max, where v_max is the maximum desired
 *  speed (or target speed) of the robot. This greatly simplifies the
 *  implementations of other Weights as they need only work with percentages,
 *  and need not to take into account the target speed of the robot.
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

#ifndef CONTROLLERSPEEDWEIGHT_H_
#define CONTROLLERSPEEDWEIGHT_H_

#include "Weight.h"

#include "../../Controller/Controller.h"

class ControllerSpeedWeight : public Weight {

	Q_OBJECT;

public:
	ControllerSpeedWeight(Create *create) : Weight("ControllerSpeedWeight", create) {};

	virtual ~ControllerSpeedWeight() {};

	virtual void process(Vector2D &v) {
		v = v * create->controller->targetSpeed;
	};
};

#endif /* CONTROLLERSPEEDWEIGHT_H_ */
