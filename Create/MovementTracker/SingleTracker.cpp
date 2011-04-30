/*
 *  SingleTracker.cpp
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


#include "SingleTracker.h"


SingleTracker::SingleTracker(Create *create, QString name) : Tracker(name, create) {
	selectedMovementTracker = NULL;
}

SingleTracker::~SingleTracker(){

}


Trafo2D SingleTracker::calculateTransformation(){
	Trafo2D retVal;
	if(selectedMovementTracker) {
		retVal = selectedMovementTracker->getTransformation();
	}
	return retVal;
}

double SingleTracker::calculateTotalDistance(){
	double retVal = 0;
	if(selectedMovementTracker) {
		retVal = selectedMovementTracker->getTotalDistance();
	}
	return retVal;
}

double SingleTracker::calculateTotalAngle(){
	double retVal = 0;
	if(selectedMovementTracker) {
		retVal = selectedMovementTracker->getTotalAngle();
	}
	return retVal;
}

void SingleTracker::setSelectedMovementTracker(MovementTracker *movementTracker) {
	lock.lockForWrite();
	{
		selectedMovementTracker = movementTracker;
	}
	lock.unlock();
}

void SingleTracker::setSelectedMovementTracker(QString name) {
	// Lock here because we need the list to be locked while we traverse it to find our tracker,
	// thus we don't use the setSelectedMovementTracker(MovementTracker*) method...
	lock.lockForWrite();
	{
		selectedMovementTracker = NULL;
		for (int i = 0; i < movementTrackers->count(); i++){
			if(movementTrackers->at(i)->name == name) {
				selectedMovementTracker = movementTrackers->at(i);
				break;
			}
		}
		if(!selectedMovementTracker) Debug::warning("[SingleTracker] The %1 could not be found!", name);
	}
	lock.unlock();
}

MovementTracker* SingleTracker::getSelectedMovementTracker() {
	// No lock necessary
	return selectedMovementTracker;
}

