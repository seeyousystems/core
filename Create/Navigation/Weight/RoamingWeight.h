/*
 *  RoamingWeight.h
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

#ifndef ROAMINGWEIGHT_H_
#define ROAMINGWEIGHT_H_

#include "Weight.h"

#include "../../Library/Debug.h"
#include "../../Library/Math.h"



class RoamingWeight : public Weight {

	Q_OBJECT;

public:
	typedef enum  {
		FORWARDS = 0,
		LEFT,
		RIGHT,
		SHARP_LEFT,
		SHARP_RIGHT,
		SPIN,
		MODECOUNT
	} RoamMode;

private:
	RoamMode mode;
	int modeTick;
	int minModeTicks;


public:
	RoamingWeight(Create *create) : Weight("RoamingWeight", create) {
		mode = FORWARDS;
		modeTick = 0;
		minModeTicks = 0;
	};

	virtual ~RoamingWeight() {};

	virtual void process(Vector2D &v) {

		double av = (v.x() + v.y()) / 2.0;
		double fx = 0.7;
		double sfx = 0.3;

		modeTick++;

		// Change mode?
		if(modeTick > minModeTicks && rand() % 10 == 0) {
			int m = rand() % MODECOUNT;
			if(m==FORWARDS) {
				mode = FORWARDS;
				minModeTicks = 20;
			} else if(m==LEFT) {
				mode = LEFT;
				minModeTicks = 60;
			} else if(m==RIGHT) {
				mode = RIGHT;
				minModeTicks = 60;
			} else if(m==SHARP_LEFT) {
				mode = SHARP_LEFT;
				minModeTicks = 50;
			} else if(m==SHARP_RIGHT) {
				mode = SHARP_RIGHT;
				minModeTicks = 50;
			} else if(m==SPIN) {
				mode = SPIN;
				minModeTicks = 30;
			}

			modeTick = 0;

			Debug::print("[RoamingWeight] change mode: %1", mode);
		}

		if(mode == FORWARDS) {
			v = Vector2D(av, av);
		} else if(mode == LEFT) {
			v = Vector2D(av*fx, av);
		} else if(mode == RIGHT) {
			v = Vector2D(av, av*fx);
		} else if(mode == SHARP_LEFT) {
			v = Vector2D(av*sfx, av);
		} else if(mode == SHARP_RIGHT) {
			v = Vector2D(av, av*sfx);
		} else if(mode == SPIN) {
			v = Vector2D(av, -av);
		}





	};
};

#endif /* ROAMINGWEIGHT_H_ */
