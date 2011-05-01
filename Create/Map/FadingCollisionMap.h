/*
 * FadingCollisionMap.h
 *
 *  The small error in localization of the robot overtime leads to greater error,
 *  which in turn can result in �phantom objects�. These phantom objects are the
 *  ghosts of previously detected obstacles, where the error in localization has
 *  shifted the robots position over time, misaligning the current position and
 *  the data on the HeatMap compared with the real world. However, where the
 *  HeatMap claims to see an object, there actually is no object anymore relative
 *  to the believed (localized) position. This yields a problem when trying to
 *  safely navigate using the HeatMap: suddenly the robot will avoid these
 *  phantom objects, although nothing is in its way. Using the current sensors
 *  alone is not efficient, as a detected drop needs to be remembered in some
 *  extent to make sure to avoid it completely � some sort of remembering map
 *  must be used. This is where the FadingCollisonMap comes to use. It does
 *  exactly what the name implies: it slowly fades away the detected collisions
 *  over time, only remembering the local most recent collisions. This has proven
 *  to work very well when navigating an unknown environment, even when the
 *  localization is not stable.
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

#ifndef FADINGCOLLISIONMAP_H_
#define FADINGCOLLISIONMAP_H_

#include <QList>

#include "Map.h"

#include "../Library/Math.h"

struct FadingCollision {
	Vector2D position;
	double radius;
	QTime time;
};

class FadingCollisionMap : public Map {

	Q_OBJECT

private:
	QList<FadingCollision> *collisions;
	QMutex _mutex;

public:
	FadingCollisionMap(Create *create);
	~FadingCollisionMap();
	void connectController(QObject *target);
	void fadeOldCollisions();
	void addCollision(FadingCollision collision);
	bool isPointWithinCollisionArea(Vector2D point);

public:
	virtual void paint(QPainter &painter, QRect view);
	virtual long width();
	virtual long height();

public slots:
	void registerObjectDetected(double distance, double angle, double opacity, int size);

};

#endif /* FADINGCOLLISIONMAP_H_ */
