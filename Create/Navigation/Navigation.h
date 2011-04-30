/*
 *  Navigation.h
 *
 *  The Navigation module is responsible for accepting navigation points and
 *  translating them to commands with which the Controller can use to directly
 *  steer the robot through the path of these navigation points.
 *  Any implementation of the Navigation module must inherit from the
 *  Navigation class and implement the method getWheelSpeed(...). Everything
 *  related to the management of navigation points is implemented by the
 *  Navigation class � implementations do not need to toil with this. The
 *  Navigation module by itself cannot actually navigate the robot � that is,
 *  actually send commands to the Controller. This must be done by a Task which
 *  collects the navigational information from the Navigation module and forwards
 *  it to the Controller. Typically such a task will automatically add new
 *  navigational points and make decisions when to abort or finish navigating.
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

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include <QList>
#include <QTime>

#include "../CoreObject.h"
#include "../Library/Math.h"
#include "../Library/Math/trafo2.h"

#include <QReadWriteLock>

class Navigation : public CoreObject {

	Q_OBJECT;


public:
	enum CollisionDirection { CollisionDirectionAhead, CollisionDirectionBackwards };
	enum CollisionSource { CollisionSourceHeatMap, CollisionSourceFadingCollisionMap };

protected:
	QList<Vector2D> *navPoints;	// The navigation points which the robot must drive along
	QReadWriteLock lock;

public:
	int currentNavPoint;		// The last nav node which corresponds to the wheelnode tick, it is calculated backwards...

public:
	Navigation(QString name, Create *create);
	virtual ~Navigation();
	virtual void addNavPoint(Vector2D point);
	virtual void addNavPoint(long x, long y);
	QList<Vector2D> getNavPoints();
	virtual void clearNavPoints();
	virtual Vector2D getWheelSpeed(int tick, int interval) = 0;
	virtual int getCurrentNavPoint();
	virtual bool isAtLastNavPoint();
	virtual Vector2D getCurrentDestination();
	long getMaxOpenAreaDistanceFromMap(CollisionSource source, CollisionDirection direction);
	int getWallDistanceFromMap(CollisionSource source);

protected:
	virtual void processAddNavPoint(Vector2D point);

};

#endif /* NAVIGATION_H_ */
