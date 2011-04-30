/*
 *  JoystickWeight.h
 *
 *  The JoystickWeight binds the signals from a Joystick widgets into the channel.
 *  When the joystick�s yokes are idle (zero), the JoystickWeight will have no
 *  influence. However, when the yokes are active the Weight will alter the
 *  wheel speeds to correspond to the movement of the joystick.
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

#ifndef JOYSTICKWEIGHT_H_
#define JOYSTICKWEIGHT_H_

#include "Weight.h"

#include "../../Library/Joystick2D.h"

class JoystickWeight : public Weight {

	Q_OBJECT

public:
	JoystickWeight(Create *create) : Weight("JoystickWeight", create) {};

	virtual ~JoystickWeight() {};

	virtual void process(Vector2D &v) {
		double yokeX = create->joystick->getYokeX();
		double yokeY = create->joystick->getYokeY();

		double tolerance = 0.06;

		if (yokeY == 0 && yokeX == 0){
			return;
		}
		else if (yokeY <= tolerance && yokeY >= -tolerance ) {
			v = Vector2D( -yokeX, yokeX );
		} else {
			v = Vector2D(yokeY - (yokeY * yokeX), yokeY + (yokeY * yokeX));
		}
	}
};


#endif /* JOYSTICKWEIGHT_H_ */
