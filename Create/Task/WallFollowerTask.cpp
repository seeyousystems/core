/*
 *  WallFollowerTask.cpp
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

#include "WallFollowerTask.h"

#include "../Navigation/Navigation.h"
#include "../Navigation/SystemOfWeightsNavigation.h"
#include "../Navigation/Weight/WallFollowerWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"

#include "../Map/StructureMap.h"
#include "../Controller/EmssController.h"

#include "../Library/Debug.h"

WallFollowerTask::WallFollowerTask(Create *create, int speed, TaskPriority priority) :
	Task("WallFollowerTask", create, create->intSetting("Task_DefaultInterval"), priority) {
	startPosition = Vector2D(0,0);
}

WallFollowerTask::~WallFollowerTask() {

}

void WallFollowerTask::preProcess() {
	if(  create->navigation->name == "SystemOfWeightsNavigation"){
		navigation = (SystemOfWeightsNavigation*) create->navigation;
		navigation->deleteAllWeights();
		navigation->addWeight(new FullSpeedWeight(create));
		navigation->addWeight(new WallFollowerWeight(create));
		navigation->addWeight(new JoystickWeight(create));
		navigation->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionLeft));
		navigation->addWeight(new AccelerationFilterWeight(create));
		navigation->addWeight(new ControllerSpeedWeight(create));
	}
	else {
		Debug::warning("This navigation module is not supported");
		this->status = Interrupted;
	}
}

void WallFollowerTask::process() {

	Vector2D wheelSpeed = navigation->getWheelSpeed(0,0);
	create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	// Is Task finished
	if (create->structureMap->isFinish() == true){
		this->status = Finished;
		//navigation->deleteAllWeights();
		Debug::print("[WallFollowerTask] WallFollower Task Finished");
	}


}

void WallFollowerTask::postProcess() {
	create->controller->setWheelSpeed(0, 0);
}

QString WallFollowerTask::description() {
	return QString("Task:\tWallFollower");
}

