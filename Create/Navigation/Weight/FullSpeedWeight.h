/*
 *  FullSpeedWeight.h
 *
 *  The FullSpeedWeight defines the starting value for any channel which assumes
 *  it should go ahead at full speed. All it does is set the left and right wheel
 *  speeds to full (1.0).
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

#ifndef FULLSPEEDWEIGHT_H_
#define FULLSPEEDWEIGHT_H_

#include "Weight.h"

class FullSpeedWeight : public Weight {

	Q_OBJECT;

public:
	FullSpeedWeight(Create *create) : Weight("FullSpeedWeight", create) {};

	virtual ~FullSpeedWeight() {};

	virtual void process(Vector2D &v) {
		v = Vector2D(1.0,1.0);
	};
};

#endif /* FULLSPEEDWEIGHT_H_ */
