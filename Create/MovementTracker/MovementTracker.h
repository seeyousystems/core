/*
 *  MovementTracker.h
 *
 *  A MovementTracker is responsible for receiving the Tracker�s messages regarding
 *  changes in the state of the robot and performing localization based on this
 *  information. The methods a MovementTracker implementation must implement are
 *  registerMovedDistance(...), registerChangedAngle(...), and
 *  registerChangedWheelSpeed(...). However, not all of these methods must react
 *  in some fashion � technically and mathematically it is sufficient to react
 *  solely to the registerChangedWheelSpeed(...) method.
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

#ifndef MOVEMENTTRACKER_H_
#define MOVEMENTTRACKER_H_

#include <QReadWriteLock>

#include "../CoreObject.h"
#include "../Library/Math.h"

class MovementTracker : public CoreObject {

	Q_OBJECT

protected:
	Trafo2D transformation; // Transformation object with double precision
	long totalDistance; //TODO: update to double value
	long relativeDistance; //TODO: update to double value
	double totalAngle;
	double relativeAngle;
	double wheelOffset;
	int weight;

protected:
	QReadWriteLock lock;

public:
	MovementTracker(QString name, Create *create);
	virtual ~MovementTracker();
	long x(); // X coordinate in milli-meters
	long y(); // Y coordinate in milli-meters
	Vector2D translation(); // The current translation in space
	double rotation(); // Rotation as clockwise positive (Core/COIL format) in degrees
	void setTransformation(Trafo2D t);
	Trafo2D getTransformation();
	long getTotalDistance();
	long getRelativeDistance();
	double getTotalAngle();
	double getRelativeAngle();
	int getWeight();

public:
	virtual void registerMovedDistance(double distance) = 0;
	virtual void registerChangedAngle(double angle) = 0;
	virtual void registerChangedWheelSpeed(int, int) = 0;

signals:
	void moved(long x, long y, double rotation);
};

#endif /* MOVEMENTTRACKER_H_ */
