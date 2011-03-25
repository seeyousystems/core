/*
 *  MovementTracker.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include "MovementTracker.h"

#include "../Library/Debug.h"


MovementTracker::MovementTracker(QString name, Create *create) {
	this->name = name; this->create = create;
}

MovementTracker::~MovementTracker() {

}

void MovementTracker::connectController(QObject *target) {
	connect(target, SIGNAL(signalMovedDistance(double)), this, SLOT(registerMovedDistance(double)));
	connect(target, SIGNAL(signalChangedAngle(double)), this, SLOT(registerChangedAngle(double)));
	connect(target, SIGNAL(signalCollision()), this, SLOT(registerCollision()));
	connect(target, SIGNAL(signalObjectDetected(double,double)), this, SLOT(registerObjectDetected(double,double)));
	Debug::print("[MovementTracker] %1 Movement Tracker connected to controller", name);
}

void MovementTracker::connectMaps(QObject *target) {
	connect(this, SIGNAL(moved(long,long,double)), target, SLOT(robotMoved(long,long,double)));
	connect(this, SIGNAL(collision(long,long)), target, SLOT(robotCollided(long,long)));
	connect(this, SIGNAL(objectDetected(long,long)), target, SLOT(objectDetected(long,long)));
	Debug::print("[MovementTracker] %1 Movement Tracker connected to maps", name);
}

long MovementTracker::x() {
	return (long)transformation.trans().x();
}

long MovementTracker::y() {
	return (long)transformation.trans().y();
}

double MovementTracker::rotation() {
	return Deg(transformation.angle());
}
