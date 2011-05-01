/*
 *  SystemOfWeightsNavigation.cpp
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

#include "SystemOfWeightsNavigation.h"

#include "Weight/Weight.h"
#include "Weight/FullSpeedWeight.h"
#include "Weight/ControllerSpeedWeight.h"
#include "Weight/OrientationWeight.h"
#include "Weight/CollisionAvoidanceWeight.h"
#include "Weight/ObstacleAvoidanceWeight.h"
#include "Weight/AccelerationFilterWeight.h"
#include "MovementTracker/Tracker.h"


SystemOfWeightsNavigation::SystemOfWeightsNavigation(Create *create, bool createDefaultWeights) : Navigation("SystemOfWeightsNavigation", create) {
	// Init
	weights = new QList<Weight*>();

	// Default weights...
	if(createDefaultWeights) {
		weights->append(new FullSpeedWeight(create));
		weights->append(new OrientationWeight(create));
		weights->append(new AccelerationFilterWeight(create));
		weights->append(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
		weights->append(new ObstacleAvoidanceWeight(create, ObstacleAvoidanceWeight::AvoidanceDirectionAuto));
		weights->append(new ControllerSpeedWeight(create));
		emit weightsChanged();
	}

	stopAtLastNavPoint = create->boolSetting("Navigation_SystemOfWeightsNavigation_StopAtLastNavPoint");
}

SystemOfWeightsNavigation::~SystemOfWeightsNavigation() {
	deleteAllWeights();
	delete weights;
}

Vector2D SystemOfWeightsNavigation::getWheelSpeed(int tick, int interval) {

	// Are we at a destination?
	Vector2D destination = getCurrentDestination();
	Vector2D position = create->tracker->getTranslation();
	double tolerance = 200;
	if(    position.x() >= destination.x()-tolerance
		&& position.x() <= destination.x()+tolerance
		&& position.y() >= destination.y()-tolerance
		&& position.y() <= destination.y()+tolerance ) {
		// Continue to next destination
		currentNavPoint++;
		if(isAtLastNavPoint() && stopAtLastNavPoint) return Vector2D(0.0,0.0); // Stop at the last point...
	}

	// Go through all the weights, each modifying our velocity vector
	Vector2D v(0.0,0.0);
	lock.lockForRead(); {
		for(int i = 0; i < weights->size(); i++) {
			// Reference to our v, process() will modify it...
			Weight *w = weights->at(i);
			if(w->isActive()) w->process(v);
		}
	} lock.unlock();

	// Return a copy of the final velocity vector
	return v;
}

const QList<Weight*> *SystemOfWeightsNavigation::getWeights() {
	return weights;
}

void SystemOfWeightsNavigation::deleteAllWeights() {
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			delete weights->at(i);
		}
		weights->clear();
	} lock.unlock();
	emit weightsChanged();
}

void SystemOfWeightsNavigation::addWeight(Weight* weight){
	lock.lockForWrite(); {
		weights->push_back(weight);
	} lock.unlock();
	emit weightsChanged();
}

void SystemOfWeightsNavigation::addWeightFront(Weight* weight){
	lock.lockForWrite(); {
		weights->push_front(weight);
	} lock.unlock();
	emit weightsChanged();
}

bool SystemOfWeightsNavigation::deleteWeight(QString name){

	bool retVal = false;
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i)->name == name) {
				delete weights->at(i);
				weights->removeAt(i);
				retVal = true;
			}
		}
	} lock.unlock();
	emit weightsChanged();
	return retVal;

}

bool SystemOfWeightsNavigation::deleteWeight(Weight* weight) {
	bool retVal;
	lock.lockForWrite(); {
		retVal = weights->removeOne(weight);
		delete weight;
	} lock.unlock();
	emit weightsChanged();
	return retVal;
}

void SystemOfWeightsNavigation::setWeightActive(QString name, bool active){
	lock.lockForRead(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i)->name == name) {
				weights->at(i)->setActive(active);
				emit weightsChanged();
			}
		}
	} lock.unlock();
}

void SystemOfWeightsNavigation::setWeightActive(Weight *weight, bool active){
	lock.lockForRead(); {
		weight->setActive(active);
		emit weightsChanged();
	} lock.unlock();
}

void SystemOfWeightsNavigation::moveWeightDown(Weight* weight) {
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i) == weight){
				weights->swap(i,i+1);
				break;
			}
		}
	} lock.unlock();
	emit weightsChanged();
}

void SystemOfWeightsNavigation::moveWeightUp(Weight* weight) {
	lock.lockForWrite(); {
		for(int i = 0; i < weights->count(); i++) {
			if (weights->at(i) == weight){
				weights->swap(i,i-1);
				break;
			}
		}
	} lock.unlock();
	emit weightsChanged();
}

