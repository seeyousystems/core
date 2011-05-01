/*
 *  NavigationTask.cpp
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

#include "NavigationTask.h"

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/Navigation.h"
#include "../Navigation/SystemOfWeightsNavigation.h"
#include "../MovementTracker/MovementTracker.h"

#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"



//TODO: Settings, finish check, et cetera... This class is not finished at all!

NavigationTask::NavigationTask(Create *create, int speed, TaskPriority priority) :
	Task("NavigationTask", create, create->intSetting("Task_NavigationTask_Interval"), priority) {
	this->speed = speed;
	tick = 0;
}

NavigationTask::~NavigationTask() {

}

void NavigationTask::preProcess() {
	if(  create->navigation->name == "SystemOfWeightsNavigation"){
		((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new JoystickWeight(create));
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
		((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
	}
	else if (create->navigation->name == "SplineNavigation"){
			// using SplineNavigation
		}
}

void NavigationTask::process() {

	// Send wheel speeds for current tick to controller
	Vector2D wheelSpeed = create->navigation->getWheelSpeed(tick, interval);
	create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	// Finished?
	if(create->navigation->isAtLastNavPoint()) {
		// Reset?
		if(create->boolSetting("Task_NavigationTask_ResetNavPointsWhenDone") == true) create->navigation->clearNavPoints();
		status = Task::Finished;
	}

	tick++;
}

void NavigationTask::postProcess() {
	create->controller->setWheelSpeed(0, 0);
}

QString NavigationTask::description() {
	return QString("Speed:\t%2").arg(speed);
}
