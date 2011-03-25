/*
 *  MovementTracker.h
 *
 *  Responsible for tracking movements of the robot, and in turn performing
 *  the localization, a Movement Tracker accepts signals from the active
 *  Controller and translates them accordingly. Furthermore, Trackers also
 *  forward changes about the robot position to other components, especially
 *  the Maps, in forms of signals. The Movement Tracker is an abstract class
 *  which needs to be defined by subclasses. The only significant variable
 *  is transformation, which is a Trafo2D object. The input slots, i.e.
 *  signals received from the Controller, are registerMovedDistance(distance),
 *  registerChangedAngle(angle), registerCollision(), and
 *  registerObjectDetected(distance,angle).
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include <QObject>

#include "../create.h"
#include "../Library/Math.h"

class MovementTracker : public QObject {

	Q_OBJECT

public:

	Trafo2D transformation; // Transformation object with double precision
	long totalDistance;
	long relativeDistance;

public:
	QString name;
	Create *create;

public:
	MovementTracker(QString name, Create *create);
	virtual ~MovementTracker();
	void connectController(QObject *target);
	void connectMaps(QObject *target);
	long x(); // X coordinate in milli-meters
	long y(); // Y coordinate in milli-meters
	double rotation(); // Rotation as clockwise positive (Core/COIL format)


public slots:
	/* Slots for movement signals from Controller or other interfaces */
	virtual void registerMovedDistance(double distance) = 0;
	virtual void registerChangedAngle(double angle) = 0;
	virtual void registerCollision() = 0;
	virtual void registerObjectDetected(double distance, double angle) = 0;


signals:
	void moved(long x, long y, double rotation);
	void collision(long x, long y);
	void objectDetected(long x, long y);
};

#endif /* MOVEMENTTRACKER_H_ */
