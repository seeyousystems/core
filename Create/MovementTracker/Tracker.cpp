/*
 *  Tracker.cpp
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


#include "Tracker.h"

Tracker::Tracker(QString name, Create *create) : CoreObject(name, create) {

	movementTrackers = new QList<MovementTracker*>();
	movementLog = new QList<Trafo2D>();

	Debug::print("[Tracker] %1 loaded", name);
}

Tracker::~Tracker(){
	deleteAllTrackers();
	delete movementTrackers;
	delete movementLog;
}

void Tracker::deleteAllTrackers() {
	lock.lockForWrite();
	{
		for (int i = 0; i < movementTrackers->count(); i++){
			delete movementTrackers->at(i);
		}
		movementTrackers->clear();
	}
	lock.unlock();
}

void Tracker::connectController(QObject *target) {
	connect(target, SIGNAL(signalMovedDistance(double)), this, SLOT(registerMovedDistance(double)));
	connect(target, SIGNAL(signalChangedAngle(double)), this, SLOT(registerChangedAngle(double)));
	connect(target, SIGNAL(signalChangedWheelSpeed(int, int)), this, SLOT(registerChangedWheelSpeed(int, int)));
	connect(target, SIGNAL(signalObjectDetected(double, double, double, int)), this, SLOT(registerObjectDetected(double, double, double, int)));
}

void Tracker::addMovementTracker(MovementTracker *movemenTracker){
	lock.lockForWrite();
	{
		movementTrackers->append(movemenTracker);
	}
	lock.unlock();
	Debug::print("[Tracker] added %1", movemenTracker->name);

}

Trafo2D Tracker::getTransformation(){

	QReadLocker locker(&lock);

	return calculateTransformation();
}

Trafo2D Tracker::getTransformationFromSpecificTracker(QString name) {

	QReadLocker locker(&lock);

	QList<MovementTracker*>::const_iterator i = movementTrackers->constBegin();
	while (i != movementTrackers->constEnd()) {
		if((*i)->name == name) return (*i)->getTransformation();
		i++;
	}
	Debug::warning("[Tracker] %1 does not exist in this Tracker!", name);
	return Trafo2D();
}

double Tracker::getTotalDistance(){

	QReadLocker locker(&lock);

	return calculateTotalDistance();
}

double Tracker::getTotalAngle(){

	QReadLocker locker(&lock);

	return calculateTotalAngle();
}

long Tracker::getX(){
	return getTransformation().trans().x();
}

long Tracker::getY(){
	return getTransformation().trans().y();
}

double Tracker::getRotation(){
	return Deg(getTransformation().angle());
}

Vector2D Tracker::getTranslation() {
	return getTransformation().trans();
}

Trafo2D Tracker::getWheelLeftTransformation() {
	return getTransformation() * Trafo2D::trans(-create->doubleSetting("Robot_WheelOffset_mm"), 0);
}


Trafo2D Tracker::getWheelRightTransformation() {
	return getTransformation() * Trafo2D::trans(+create->doubleSetting("Robot_WheelOffset_mm"), 0);
}


void Tracker::registerMovedDistance(double distance){
	lock.lockForRead();
	{
		// Let each movement tracker calculate it's no position by itself...
		QList<MovementTracker*>::const_iterator i = movementTrackers->constBegin();
		while (i != movementTrackers->constEnd()) {
			(*i)->registerMovedDistance(distance);
			i++;
		}

		// Register this movement in the log
		registerMovementInLog(calculateTransformation());
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::registerChangedAngle(double angle){
	lock.lockForRead();
	{
		// Let each movement tracker calculate it's no position by itself...
		QList<MovementTracker*>::const_iterator i = movementTrackers->constBegin();
		while (i != movementTrackers->constEnd()) {
			(*i)->registerChangedAngle(angle);
			i++;
		}

		// Register this movement in the log
		registerMovementInLog(calculateTransformation());
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::registerChangedWheelSpeed(int leftSpeed, int rightSpeed){

	lock.lockForRead();
	{
		// Let each movement tracker calculate it's no position by itself...
		QList<MovementTracker*>::const_iterator i = movementTrackers->constBegin();
		while (i != movementTrackers->constEnd()) {
			(*i)->registerChangedWheelSpeed(leftSpeed, rightSpeed);
			i++;
		}

		// Register this movement in the log
		registerMovementInLog(calculateTransformation());
	}
	lock.unlock();
	emit moved(this->getX(), this->getY(), this->getRotation());
}

void Tracker::registerObjectDetected(double distance, double angle, double opacity, int size) {
	// Nothing to do here. Possibly sub-classes want this
}

void Tracker::setTransformation(Trafo2D t){
	lock.lockForRead();
	{
		QList<MovementTracker*>::const_iterator i = movementTrackers->constBegin();
		while (i != movementTrackers->constEnd()) {
			(*i)->setTransformation(t);
			i++;
		}
	}
	lock.unlock();
}

void Tracker::setPosition(long x, long y){
	setTransformation(Trafo2D::trans(x,y));
}

void Tracker::registerMovementInLog(Trafo2D movement) {

	// Add a transformation object to the log if we have moved a significant distance
	if(movementLog->count() > 0) {
		if(dist(movement.trans(),movementLog->last().trans()) > create->doubleSetting("Tracker_MovementLog_MinimumDistance_mm")) movementLog->append(movement);
	} else {
		movementLog->append(movement);
	}

}

// Returns a portion of the movement log n items counting back from the end.
// That means the returned list contains items t to t-n starting at t (the newest move).
QList<Trafo2D> Tracker::getMovementLogSnapshot(int n) {

	QReadLocker locker(&lock);

	QList<Trafo2D>::const_iterator i = movementLog->end();
	QList<Trafo2D> snapshot;
	while (movementLog->count() > 0 && i != movementLog->constBegin() && n > 0) {
		snapshot.append(*--i);
		n--;
	}
	return snapshot;
}
