/*
 * Tracker.h
 *
 *  The Tracker module is responsible for providing other modules with information
 *  about the localization, or positioning data, of the robot. However, this
 *  responsibility only goes so far as to delegating the necessary calculations
 *  to the internal MovementTrackers which it then uses to compile a single resulting
 *  position.
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

#ifndef TRACKER_H_
#define TRACKER_H_

#include <QList>
#include <QReadWriteLock>

#include "MovementTracker.h"

#include "../CoreObject.h"

#include "../Library/Debug.h"
#include "../Library/Math.h"

class Tracker : public CoreObject {

	Q_OBJECT

protected:
	QList<MovementTracker*> *movementTrackers;
	QList<Trafo2D> *movementLog;
	QReadWriteLock lock; // Lock used when accessing movementTrackers. Also to be used by subclasses...

public:
	Tracker(QString name, Create *create);
	~Tracker();
	void connectController(QObject *target);
	void addMovementTracker(MovementTracker* movementTracker);
	long getX(); // X coordinate in milli-meters
	long getY(); // Y coordinate in milli-meters
	double getRotation();
	Vector2D getTranslation();
	Trafo2D getWheelLeftTransformation();
	Trafo2D getWheelRightTransformation();
	void setTransformation(Trafo2D t);
	void setPosition(long x, long y);
	void deleteAllTrackers();
	Trafo2D getTransformation(); // Public method to calculateTransformation(), thread-safe
	Trafo2D getTransformationFromSpecificTracker(QString name); // Public method to get data from specific tracker, thread-safe
	double getTotalDistance(); // Public method to calculateTotalDistance(), thread-safe
	double getTotalAngle(); // Public method to calculateTotalAngle(), thread-safe
	QList<Trafo2D> getMovementLogSnapshot(int n);

private:
	virtual Trafo2D calculateTransformation() = 0; // Actual calculation, to be implemented by subclass, not-thread safe
	virtual double calculateTotalDistance() = 0; // Actual calculation, to be implemented by subclass, not-thread safe
	virtual double calculateTotalAngle() = 0; // Actual calculation, to be implemented by subclass, not-thread safe

protected:
	virtual void registerMovementInLog(Trafo2D movement); // Not-thread safe, can be re-implemented by subclass

public slots:
	/* Slots for movement signals from Controller or other interfaces */
	void registerMovedDistance(double distance);
	void registerChangedAngle(double angle);
	void registerChangedWheelSpeed(int, int);
	virtual void registerObjectDetected(double distance, double angle, double opacity, int size);

signals:
	void moved(long x, long y, double rotation);
};

#endif /* TRACKER_H_ */
