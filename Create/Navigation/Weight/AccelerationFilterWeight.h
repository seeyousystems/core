/*
 *  AccelerationFilterWeight.h
 *
 *  The AccelerationFilterWeight regulates the acceleration in the weight-channel.
 *  Upon each process(...), the Weight compares the new incoming value with the
 *  last value. If the difference is greater than the maximum allowed difference,
 *  the new value is cut short. This prevents jumps in velocity from, for example,
 *  0 (stopped) to 1 (full speed). Such jumps are unfriendly for the hardware
 *  and are prevented by the AccelerationFilterWeight.
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

#ifndef ACCELERATIONFILTERWEIGHT_H_
#define ACCELERATIONFILTERWEIGHT_H_

#include "Weight.h"
#include "Library/Debug.h"
#include <stdio.h>

class AccelerationFilterWeight : public Weight {

	Q_OBJECT;

private:
	Vector2D vOld;
	double f;

public:
	AccelerationFilterWeight(Create *create) : Weight("AccelerationFilterWeight", create) {
		vOld = Vector2D(0.0,0.0);

		// The maximum amount of change allowed in one process (based on max of 1.0)
		f = create->doubleSetting("Weight_AccelerationFilterWeight_MaxSpeedChange");
	};

	virtual ~AccelerationFilterWeight() {};

	virtual void process(Vector2D &v) {
		//Debug::print("[AccelerationFilterWeight] SystemOfWeightsNavigation turned Custom Move ON");

		Vector2D dv = v - vOld;

		//printf("f: %f\tx: %f\ty: %f\n", f, dv.x(), dv.y());

		if(dv.x() > f || dv.x() < -f)
		{
//			Debug::print("[AccelerationFilterWeight] First");
			dv = Vector2D(dv.x()*f ,dv.y());
		}
		if(dv.y() > f || dv.y() < -f)
		{
//			Debug::print("[AccelerationFilterWeight] Second");
			dv = Vector2D(dv.x() ,dv.y()*f);
		}
		v = vOld + dv;
		vOld = v;
	};
};

#endif /* ACCELERATIONFILTERWEIGHT_H_ */
