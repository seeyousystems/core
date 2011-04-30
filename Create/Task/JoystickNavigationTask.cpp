/*
 *  JoystickNavigationTask.cpp
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
#include "JoystickNavigationTask.h"

#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Navigation/Navigation.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"
//#include "../Navigation/Weight/RemoteControlWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"

JoystickNavigationTask::JoystickNavigationTask(Create *create, TaskPriority priority) :
	Task("JoystickNavigationTask", create, create->intSetting("Task_JoystickNavigationTask_Interval"), priority ){
}

JoystickNavigationTask::~JoystickNavigationTask(){

}

void JoystickNavigationTask::preProcess() {
	if(  create->navigation->name == "SystemOfWeightsNavigation"){
		navigation = (SystemOfWeightsNavigation*) create->navigation;
		navigation->deleteAllWeights();
		navigation->addWeight(new JoystickWeight(create));
//		navigation->addWeight(new RemoteControlWeight(create));
		navigation->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
		navigation->addWeight(new AccelerationFilterWeight(create));
		navigation->addWeight(new ControllerSpeedWeight(create));
	} else {
		navigation = NULL;
		Debug::warning("[JoystickNavigationTask] This navigation module is not supported");
		this->status = Interrupted;
	}
}

void JoystickNavigationTask::process() {

	if(!navigation) return;

	Vector2D wheelSpeed = navigation->getWheelSpeed(0,0);
	create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

}

void JoystickNavigationTask::postProcess() {
	create->controller->setWheelSpeed(0, 0);
}

QString JoystickNavigationTask::description() {
	return "";
}
