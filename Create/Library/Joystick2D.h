/*
 *  Joystick2D.h
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

#ifndef JOYSTICK2D_H_
#define JOYSTICK2D_H_

#include <QObject>

#include "Debug.h"

class Joystick2D : public QObject {

	Q_OBJECT

private:
	double yokeX;
	double yokeY;

public:
	Joystick2D(){
		yokeX = 0;
		yokeY = 0;
	};
	~Joystick2D() {};
	double getYokeX(){ return yokeX; };
	double getYokeY(){ return yokeY; };


public slots:
	void setYoke(double yokeX, double yokeY) {

		Debug::print("[Joystick2D] Values: %1   %2", yokeX, yokeY);
		this->yokeX = yokeX;
		this->yokeY = yokeY;
	};
};



#endif /* JOYSTICK2D_H_ */
