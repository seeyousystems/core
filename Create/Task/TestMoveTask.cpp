/*
 *  TestMoveTask.cpp
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

#include "TestMoveTask.h"

#include <iostream>

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"
#include "../Library/RFIDDBase.h"
#include "../Navigation/Navigation.h"
#include "../Navigation/SystemOfWeightsNavigation.h"

#include "../MovementTracker/Tracker.h"
#include "TaskManager.h"

#include "../Navigation/Weight/WallFollowerWeight.h"
#include "../Navigation/Weight/FullSpeedWeight.h"
#include "../Navigation/Weight/ControllerSpeedWeight.h"
#include "../Navigation/Weight/OrientationWeight.h"
#include "../Navigation/Weight/CollisionAvoidanceWeight.h"
#include "../Navigation/Weight/AccelerationFilterWeight.h"
#include "../Navigation/Weight/JoystickWeight.h"

TestMoveTask::TestMoveTask(Create *create, QString moveName, TaskPriority priority) :
	Task("TestMoveTask", create, create->intSetting("Task_TestMoveTask_Interval"), priority) {
	this->moveName = moveName;
	robotsize = 330;
	halfstep = 610;
	quarterstep = 305;
}

TestMoveTask::~TestMoveTask() {

}

/*
 * TODO Implement OrientationWeight.h Correctly - Needed for Navigation Points
 * Needs #include "../../Tracker/Tracker.h"
 * 		Needs #include "../../MovementTracker.h"
 *
 * 		#include "../../Map/MapObject/MarkerMapObject.h"
 *
 *
 * CollisionAvoidanceWeight.h - Needed for object avoidance
 */

void TestMoveTask::preProcess() {

	if(this->moveName == "Accuracy Test") {
		Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Accuracy Test");

		if(create->navigation->name == "SystemOfWeightsNavigation") {
			Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned ON");
			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
			//((SystemOfWeightsNavigation *)create->navigation)->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
 			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new WallFollowerWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
		} else {
			Debug::warning("This navigation module is not supported");
			this->status = Interrupted;
		}

		// Set navigation points
		Vector2D robot = create->tracker->getTranslation();

		RFIDDBase local;

		//create->navigation->addNavPoint(robot.x() + local.getDBX(1) - halfstep / 2, robot.y() + local.getDBY(2));
		//create->navigation->addNavPoint(robot.x() + local.getDBX(1) - halfstep, robot.y() + local.getDBY(2));
		//create->navigation->addNavPoint(robot.x() + local.getDBX(1) - halfstep * 1.5, robot.y() + local.getDBY(2));

	// 	create->navigation->addNavPoint(robot.x() + local.getDBX(2) , robot.y() + local.getDBY(2));
		//create->navigation->addNavPoint(robot.x() + local.getDBX(2) - halfstep / 2, robot.y() + local.getDBY(2));
		//create->navigation->addNavPoint(robot.x() + local.getDBX(2) - halfstep, robot.y() + local.getDBY(2));
		create->navigation->addNavPoint(robot.x() + local.getDBX(2) - halfstep * 1.5, robot.y() + local.getDBY(2));

		create->navigation->addNavPoint(robot.x() + local.getDBX(3) , robot.y() + local.getDBY(3));
		create->navigation->addNavPoint(robot.x() + local.getDBX(3) - halfstep / 2, robot.y() + local.getDBY(3));
		create->navigation->addNavPoint(robot.x() + local.getDBX(3) - halfstep, robot.y() + local.getDBY(3));
		create->navigation->addNavPoint(robot.x() + local.getDBX(3) - halfstep * 1.5, robot.y() + local.getDBY(3));

		create->navigation->addNavPoint(robot.x() + local.getDBX(4) , robot.y() + local.getDBY(4));
		create->navigation->addNavPoint(robot.x() + local.getDBX(4) - halfstep / 2, robot.y() + local.getDBY(4));
		create->navigation->addNavPoint(robot.x() + local.getDBX(4) - halfstep, robot.y() + local.getDBY(4));
		create->navigation->addNavPoint(robot.x() + local.getDBX(4) - halfstep * 1.5, robot.y() + local.getDBY(4));

		create->navigation->addNavPoint(robot.x() + local.getDBX(5) , robot.y() + local.getDBY(5));
		create->navigation->addNavPoint(robot.x() + local.getDBX(5) - halfstep / 2, robot.y() + local.getDBY(5));
		create->navigation->addNavPoint(robot.x() + local.getDBX(5) - halfstep, robot.y() + local.getDBY(5));
		create->navigation->addNavPoint(robot.x() + local.getDBX(5) - halfstep * 1.5, robot.y() + local.getDBY(5));

		create->navigation->addNavPoint(robot.x() + local.getDBX(6) , robot.y() + local.getDBY(6));
		create->navigation->addNavPoint(robot.x() + local.getDBX(6) - halfstep / 2, robot.y() + local.getDBY(6));
		create->navigation->addNavPoint(robot.x() + local.getDBX(6) - halfstep, robot.y() + local.getDBY(6));
		create->navigation->addNavPoint(robot.x() + local.getDBX(6) - halfstep * 1.5, robot.y() + local.getDBY(6));

		create->navigation->addNavPoint(robot.x() + local.getDBX(7) , robot.y() + local.getDBY(7));
		create->navigation->addNavPoint(robot.x() + local.getDBX(7) - halfstep / 2, robot.y() + local.getDBY(7));
		create->navigation->addNavPoint(robot.x() + local.getDBX(7) - halfstep, robot.y() + local.getDBY(7));
		create->navigation->addNavPoint(robot.x() + local.getDBX(7) - halfstep * 1.5, robot.y() + local.getDBY(7));

		create->navigation->addNavPoint(robot.x() + local.getDBX(8) , robot.y() + local.getDBY(8));
		create->navigation->addNavPoint(robot.x() + local.getDBX(8) - halfstep / 2, robot.y() + local.getDBY(8));
		create->navigation->addNavPoint(robot.x() + local.getDBX(8) - halfstep, robot.y() + local.getDBY(8));
		create->navigation->addNavPoint(robot.x() + local.getDBX(8) - halfstep * 1.5, robot.y() + local.getDBY(8));

		create->navigation->addNavPoint(robot.x() + local.getDBX(9) , robot.y() + local.getDBY(9));
		create->navigation->addNavPoint(robot.x() + local.getDBX(9) - halfstep / 2, robot.y() + local.getDBY(9));
		create->navigation->addNavPoint(robot.x() + local.getDBX(9) - halfstep, robot.y() + local.getDBY(9));
		create->navigation->addNavPoint(robot.x() + local.getDBX(9) - halfstep * 1.5, robot.y() + local.getDBY(9));

		create->navigation->addNavPoint(robot.x() + local.getDBX(10) - halfstep, robot.y() + local.getDBY(10));

		//turn
		create->navigation->addNavPoint(robot.x() + local.getDBX(10) - halfstep, robot.y() + local.getDBY(10) - halfstep / 2);
		create->navigation->addNavPoint(robot.x() + local.getDBX(10) - halfstep, robot.y() + local.getDBY(10) - halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(10) - halfstep, robot.y() + local.getDBY(10) - halfstep * 1.5);

		create->navigation->addNavPoint(robot.x() + local.getDBX(11) - halfstep, robot.y() + local.getDBY(11));
		create->navigation->addNavPoint(robot.x() + local.getDBX(11) - halfstep, robot.y() + local.getDBY(11) - halfstep / 2);
		create->navigation->addNavPoint(robot.x() + local.getDBX(11) - halfstep, robot.y() + local.getDBY(11) - halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(11) - halfstep, robot.y() + local.getDBY(11) - halfstep * 1.5);

		create->navigation->addNavPoint(robot.x() + local.getDBX(12) - halfstep, robot.y() + local.getDBY(12));
		create->navigation->addNavPoint(robot.x() + local.getDBX(12) - halfstep, robot.y() + local.getDBY(12) - halfstep / 2);
		create->navigation->addNavPoint(robot.x() + local.getDBX(12) - halfstep, robot.y() + local.getDBY(12) - halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(12) - halfstep, robot.y() + local.getDBY(12) - halfstep * 1.5);

		create->navigation->addNavPoint(robot.x() + local.getDBX(13) - halfstep, robot.y() + local.getDBY(13));
		create->navigation->addNavPoint(robot.x() + local.getDBX(13) - halfstep, robot.y() + local.getDBY(13) - halfstep / 2);
		create->navigation->addNavPoint(robot.x() + local.getDBX(13) - halfstep, robot.y() + local.getDBY(13) - halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(13) - halfstep, robot.y() + local.getDBY(13) - halfstep * 1.5);

		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - halfstep, robot.y() + local.getDBY(14) - halfstep);

		//turn
		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - halfstep + halfstep / 2, robot.y() + local.getDBY(14)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - halfstep + halfstep, robot.y() + local.getDBY(14)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - halfstep + halfstep * 1.5, robot.y() + local.getDBY(14)- halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(48)- halfstep , robot.y() + local.getDBY(48)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(48) - halfstep+ halfstep / 2, robot.y() + local.getDBY(48)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(48) - halfstep+ halfstep, robot.y() + local.getDBY(48)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(48) - halfstep+ halfstep * 1.5, robot.y() + local.getDBY(48)- halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - halfstep, robot.y() + local.getDBY(49)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - halfstep+ halfstep / 2, robot.y() + local.getDBY(49)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - halfstep+ halfstep, robot.y() + local.getDBY(49)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - halfstep+ halfstep * 1.5, robot.y() + local.getDBY(49)- halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - halfstep, robot.y() + local.getDBY(50)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - halfstep+ halfstep / 2, robot.y() + local.getDBY(50)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - halfstep+ halfstep, robot.y() + local.getDBY(50)- halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - halfstep+ halfstep * 1.5, robot.y() + local.getDBY(50)- halfstep);

//		create->navigation->addNavPoint(robot.x() + local.getDBX(51) , robot.y() + local.getDBY(51)- halfstep);
//		create->navigation->addNavPoint(robot.x() + local.getDBX(51) + halfstep / 2, robot.y() + local.getDBY(51)- halfstep);
//		create->navigation->addNavPoint(robot.x() + local.getDBX(51) + halfstep, robot.y() + local.getDBY(51)- halfstep);
//		create->navigation->addNavPoint(robot.x() + local.getDBX(51) + halfstep * 1.5, robot.y() + local.getDBY(51)- halfstep);
//
//		create->navigation->addNavPoint(robot.x() + local.getDBX(52) , robot.y() + local.getDBY(52)+ halfstep);


		//create->navigation->addNavPoint(robot.x(), robot.y() + 250);
	//	create->navigation->addNavPoint(robot.x() + 250, robot.y() + 250);

	} else if(this->moveName == "Room201") {
		Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Room201");

		if(create->navigation->name == "SystemOfWeightsNavigation") {
			Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned ON");
			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
			//((SystemOfWeightsNavigation *)create->navigation)->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
 			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new WallFollowerWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
		} else {
			Debug::warning("This navigation module is not supported");
			this->status = Interrupted;
		}

		// Set navigation points
		Vector2D robot = create->tracker->getTranslation();

		RFIDDBase local;

		//Navigation_InitialNavPointsListX_mm = 0;
		//Navigation_InitialNavPointsListY_mm = 0;

		resetX = local.getDBX(94);
		resetY = local.getDBY(94);

		create->controller->setTargetSpeed(300);

		create->navigation->addNavPoint(robot.x() + local.getDBX(94) - resetX, robot.y() + local.getDBY(94) - resetY);
		create->navigation->addNavPoint(robot.x() + local.getDBX(94) - resetX, robot.y() + local.getDBY(94) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(86) - resetX, robot.y() + local.getDBY(86) - resetY);
		create->navigation->addNavPoint(robot.x() + local.getDBX(86) - resetX, robot.y() + local.getDBY(86) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(85) - resetX, robot.y() + local.getDBY(85) - resetY);
		create->navigation->addNavPoint(robot.x() + local.getDBX(85) - resetX, robot.y() + local.getDBY(85) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(15) - resetX, robot.y() + local.getDBY(15) - resetY);
		create->navigation->addNavPoint(robot.x() + local.getDBX(15) - resetX, robot.y() + local.getDBY(15) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - resetX, robot.y() + local.getDBY(14) - resetY);

		//turn

		create->navigation->addNavPoint(robot.x() + local.getDBX(14) - resetX, robot.y() + local.getDBY(14) - resetY + halfstep - 300);

		create->navigation->addNavPoint(robot.x() + local.getDBX(48) - resetX, robot.y() + local.getDBY(48) - resetY + halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(48) - resetX + halfstep, robot.y() + local.getDBY(48) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - resetX, robot.y() + local.getDBY(49) - resetY + halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(49) - resetX + halfstep, robot.y() + local.getDBY(49) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - resetX, robot.y() + local.getDBY(50) - resetY + halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(50) - resetX + halfstep, robot.y() + local.getDBY(50) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(51) - resetX, robot.y() + local.getDBY(51) - resetY + halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(51) - resetX + halfstep, robot.y() + local.getDBY(51) - resetY + halfstep);

		create->navigation->addNavPoint(robot.x() + local.getDBX(52) - resetX, robot.y() + local.getDBY(52) - resetY + halfstep);
		create->navigation->addNavPoint(robot.x() + local.getDBX(52) - resetX + halfstep - 100, robot.y() + local.getDBY(52) - resetY + halfstep);
	}
	 else if(this->moveName == "Offices") {
			Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Room201");

			if(create->navigation->name == "SystemOfWeightsNavigation") {
				Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned ON");
				((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
				((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
				((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
				//((SystemOfWeightsNavigation *)create->navigation)->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionAuto));
				((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
	 			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new WallFollowerWeight(create));
				((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
			} else {
				Debug::warning("This navigation module is not supported");
				this->status = Interrupted;
			}

			// Set navigation points
			Vector2D robot = create->tracker->getTranslation();

			RFIDDBase local;

			resetX = local.getDBX(52);
			resetY = local.getDBY(52);

			create->navigation->addNavPoint(robot.x() + local.getDBX(52) - resetX, robot.y() + local.getDBY(52) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(51) - resetX, robot.y() + local.getDBY(51) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(50) - resetX, robot.y() + local.getDBY(50) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(49) - resetX, robot.y() + local.getDBY(49) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(48) - resetX, robot.y() + local.getDBY(48) - resetY);

			// Turn
			create->navigation->addNavPoint(robot.x() + local.getDBX(14) - resetX, robot.y() + local.getDBY(14) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(15) - resetX, robot.y() + local.getDBY(15) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(85) - resetX, robot.y() + local.getDBY(85) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(86) - resetX, robot.y() + local.getDBY(86) - resetY);

			// Turn
			create->navigation->addNavPoint(robot.x() + local.getDBX(55) - resetX, robot.y() + local.getDBY(56) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(56) - resetX, robot.y() + local.getDBY(56) - resetY);
			create->navigation->addNavPoint(robot.x() + local.getDBX(57) - resetX, robot.y() + local.getDBY(57) - resetY);
		}

	else if(this->moveName == "Straight with Weights") {
		Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Straight with Weights");

		if(create->navigation->name == "SystemOfWeightsNavigation") {
			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
		} else {
			Debug::warning("This navigation module is not supported");
			this->status = Interrupted;
		}

		// Set navigation points
		Vector2D robot = create->tracker->getTranslation();

		create->navigation->addNavPoint(robot.x(), robot.y() + create->doubleSetting("Task_TestMoveTask_Travel_Distance_mm"));

	} else if(this->moveName == "Custom Move") {
		Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Custom Move");
		Debug::print("[TestMoveTask] waiting for console input");
		std::cout << "Please enter target vl (mmps): ";
		std::cin >> customMovevl;
		std::cout << "Please enter target vr (mmps): ";
		std::cin >> customMovevr;
		std::cout << "Please enter target time (s): ";
		std::cin >> customTime;
		std::cout << "Please enter target distance (mm): ";
		std::cin >> customDistance;

	} else if (this->moveName == "WallFollowerAccuracy"){
		if(  create->navigation->name == "SystemOfWeightsNavigation"){

			// Add necessary weights
			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new WallFollowerWeight(create));
//			((SystemOfWeightsNavigation*)create->navigation)->addWeight(new CollisionAvoidanceWeight(create, CollisionAvoidanceWeight::AvoidanceDirectionLeft));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));

			// Save start transforamtion matrix
			startpos = create->tracker->getTransformation();
			startMileage = create->tracker->getTotalDistance();

		} else {
			Debug::warning("[AccuracyTestTask] This navigation module is not supported");
			this->status = Interrupted;
		}
	}  else if(this->moveName == "forward"){

//		if(create->navigation->name == "SystemOfWeightsNavigation") {
//			Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Custom Move ON");
//
//			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
////			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
//		} else {
//			Debug::warning("This navigation module is not supported");
//			this->status = Interrupted;
//		}
//		// Set navigation points
//		Vector2D robot = create->tracker->getTranslation();
//
//		create->navigation->addNavPoint(robot.x() + 0, robot.y() + 300);
	} else if(this->moveName == "backward"){

//		if(create->navigation->name == "SystemOfWeightsNavigation") {
//			Debug::print("[TestMoveTask] SystemOfWeightsNavigation turned Custom Move ON");
//
//			((SystemOfWeightsNavigation *)create->navigation)->deleteAllWeights();
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new FullSpeedWeight(create));
////			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new OrientationWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new AccelerationFilterWeight(create));
//			((SystemOfWeightsNavigation *)create->navigation)->addWeight(new ControllerSpeedWeight(create));
//		} else {
//			Debug::warning("This navigation module is not supported");
//			this->status = Interrupted;
//		}
//		// Set navigation points
//		Vector2D robot = create->tracker->getTranslation();
//
//		create->navigation->addNavPoint(robot.x() + 0, robot.y() + 300);
	}


	// General init
	tick = 0;
	starttime = QTime::currentTime();
	startpos = create->tracker->getTransformation();
	startdist = create->tracker->getTotalDistance();
}

void TestMoveTask::process() {

	int distance = create->intSetting("Task_TestMoveTask_Travel_Distance_mm"); // mm

	if(moveName == "Square") {

		turn(90);
		move(distance);
		turn(90);
		move(distance);
		turn(90);
		move(distance);
		turn(90);
		move(distance);
		status = Task::Finished;

	} else if(moveName == "Rotate 90") {

		turn(90);
		turn(-90);
		status = Task::Finished;

	} else if(moveName == "Rotate 360") {

		turn(360);
		turn(-360);
		status = Task::Finished;

	} else if(moveName == "Triangle") {

		turn(60);
		move(distance);
		turn(120);
		move(distance);
		turn(120);
		move(distance);
		status = Task::Finished;

	} else if(moveName == "Straight") {
		Vector2D wheelSpeed = create->navigation->getWheelSpeed(0,0);
		move(1220);
		//moveBack(100);
		//status = Task::Finished;
//
//		if(create->navigation->isAtLastNavPoint()){
//			wheelSpeed = Vector2D(0,0);
//			this->status = Finished;
//		}

		create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());
	} else if(moveName == "Stop") {
		/*
		 * This command is OPTIONAl
		 */
//		((SeeYouController*) create->controller)->regularStop();
//		if(create->taskManager->getCurrentTask() != NULL)
//		{
//			((Task*)create->taskManager)->interrupt();
//		}
		for(int i = 0; i < create->taskManager->tasks->count(); i++) {
//			addTaskToList(taskManager->tasks->at(i), QString("Task %1").arg(i));
			create->taskManager->tasks->at(i)->interrupt();
		}

		Debug::print("[SeeYouTask:NETCOMM] Stop Taskdss");
		status = Task::Finished;
	} else if(moveName == "left") {
		turn(30);
		Debug::print("[SeeYouTask:NETCOMM] Left: 45 task");
		status = Task::Finished;
	} else if(moveName == "right") {
		turn(-30);
		Debug::print("[SeeYouTask:NETCOMM] Right: 45 Task");
		status = Task::Finished;
	} else if(moveName == "forward") {

		move(110);
		Debug::print("[SeeYouTask:NETCOMM] Forward Task");
		status = Task::Finished;

//		Vector2D wheelSpeed = create->navigation->getWheelSpeed(0,0);

//		if(create->navigation->isAtLastNavPoint()){
//			wheelSpeed = Vector2D(0,0);
//			this->status = Finished;
//		}
//
//		create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	} else if(moveName == "backward") {
		moveBack(110);
		Debug::print("[SeeYouTask:NETCOMM] Backward Task");
		status = Task::Finished;

//		Vector2D wheelSpeed = create->navigation->getWheelSpeed(0,0);
//
//		if(create->navigation->isAtLastNavPoint()){
//			wheelSpeed = Vector2D(0,0);
//			this->status = Finished;
//		}
//
//		create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

	} else if(moveName == "NetComm_tag") {
//		((SeeYouController*) create->controller)->move(tagdistance, (this->speed));
//		Debug::print("[SeeYouTask:NETCOMM] Goto Tag");
//		status = Task::Finished;
	} else if(moveName == "Circle") {
		// Drives a circle with a certain radius, the velocity for
		// each wheel is calculated due p.37 (GrobKrusi08)

		if (starttime.isNull()){
			starttime = QTime::currentTime();
		}

		double r = create->intSetting("Task_TestMoveTask_Circle_Radius_mm"); // mm
		double d = create->doubleSetting("Robot_WheelOffset_mm") * 2; // mm
		double vl = create->controller->targetSpeed; // mm/s

		double vr = vl + (vl *d)/r; //mm /s

		double t = starttime.msecsTo( QTime::currentTime() ) / 1000.0; // s

		double sl = vl * t;

		double alpha = sl / r;

		if(Deg(alpha) > 360){
			vr = 0;
			vl = 0;
			this->status = Finished;
		}

		create->controller->setWheelSpeed(vl, vr);

	} else if(moveName == "Vector Circle") {

		double radius = create->intSetting("Task_TestMoveTask_Circle_Radius_mm"); // mm
		double b = 20;
		double circ = 20;
		double speed = 20;

		Vector2D c(std::sin(Rad(tick))*radius, std::cos(Rad(tick))*radius);
		Vector2D c1(std::cos(Rad(tick))*radius, -1.0*std::sin(Rad(tick))*radius);
		Vector2D c2(-1.0 * std::sin(Rad(tick))*radius, -1.0*std::cos(Rad(tick))*radius);
		//c1 = speed * c1;

		Vector2D n = Trafo2D::rot(Rad(90)) * c1;

		Vector2D l = c + (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
		Vector2D r = c - (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));

		Vector2D n1 = Trafo2D::rot(Rad(90)) * c2;

		Vector2D l1 = c1 + (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
		Vector2D r1 = c1 - (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));

		double vl = (Sqrt(l1.x()*l1.x() + l1.y() * l1.y()))/circ;
		double vr = (Sqrt(r1.x()*r1.x() + r1.y() * r1.y()))/circ;

		vl *= speed;
		vr *= speed;

		create->controller->setWheelSpeed((short)vl,(short)vr);

		if(tick > 360) {
			status = Task::Finished;
			create->controller->setWheelSpeed(0,0);
		}
	} else if(moveName == "Accuracy Test" || moveName == "Straight with Weights" || moveName == "Room201" || moveName == "Offices") {

		Vector2D wheelSpeed = create->navigation->getWheelSpeed(0,0);

		Debug::print("[TestMoveTask] Moving");

		if(create->navigation->isAtLastNavPoint()){
			//Debug::print("[TestMoveTask] Moving");
			wheelSpeed = Vector2D(0,0);
			this->status = Finished;
		}

		create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());

		//status = Task::Finished;

	} else if(moveName == "Tracker Calibration") {

		// Did we reach our total distance to travel?
		if(dist(startpos.trans(), create->tracker->getTranslation()) >= distance) {
			// Stop moving and output infos
			create->controller->setWheelSpeed(0, 0);
			status = Task::Finished;
			double t = starttime.msecsTo( QTime::currentTime() ) / 1000.0; // s
			Debug::print("[TestMoveTask] waiting for console input");
			double alpha;
			std::cout << "Please enter the angle value (deg): ";
			std::cin >> alpha;
			alpha = Rad(alpha);
			double vl = (double)create->controller->targetSpeed - (alpha*distance)/t;
			double vr = (double)create->controller->targetSpeed + (alpha*distance)/t;
			Debug::print("[TestMoveTask] t: %1", t);
			Debug::print("[TestMoveTask] corrected vl for vr=%1: %2", (double)create->controller->targetSpeed, vl);
			Debug::print("[TestMoveTask] corrected vr for vl=%1: %2", (double)create->controller->targetSpeed, vr);
			Debug::print("[TestMoveTask] corrected vle: %1", (vl - (double)create->controller->targetSpeed) / (double)create->controller->targetSpeed);
			Debug::print("[TestMoveTask] corrected vre: %1", (vr - (double)create->controller->targetSpeed) / (double)create->controller->targetSpeed);
		} else {
			// Go straight!
			create->controller->setWheelSpeed((short)create->controller->targetSpeed, (short)create->controller->targetSpeed);
		}

	} else if(moveName == "Custom Move") {

		// Check if we need to stop...
		if((customDistance != 0 && create->tracker->getTotalDistance() >= startdist+customDistance)
				|| (customTime != 0 && starttime.msecsTo(QTime::currentTime()) / 1000.0 >= customTime) ) {

			// Stop!
			create->controller->setWheelSpeed(0, 0);
			status = Task::Finished;

		} else {

			// Keep going...
			create->controller->setWheelSpeed((short)customMovevl, (short)customMovevr);

		}
	} else if (moveName == "WallFollowerAccuracy"){
		Vector2D wheelSpeed = create->navigation->getWheelSpeed(0, 0);
		create->controller->setWheelSpeed((short)wheelSpeed.x(), (short)wheelSpeed.y());
	}

	tick++;

}

void TestMoveTask::postProcess(){

	if (moveName == "WallFollowerAccuracy"){
		create->controller->setWheelSpeed(0, 0);
			Trafo2D endposition = create->tracker->getTransformation();
			Debug::print(QString("[TestMoveTask] Start Trafo: %1,%2 @ %3").arg(startpos.trans().x()).arg(startpos.trans().y()).arg(Deg(startpos.angle())));
			Debug::print(QString("[TestMoveTask] End Trafo: %1,%2 @ %3").arg(endposition.trans().x()).arg(endposition.trans().y()).arg(Deg(endposition.angle())));
			Debug::print("[TestMoveTask] Angle Difference: %1", Deg(endposition.angle() - startpos.angle()));
			Debug::print("[TestMoveTask] Start/End Position Difference (mm): %1", dist(endposition.trans(), startpos.trans() ));
			Debug::print("[TestMoveTask] Mileage (mm): %1", (int)(create->tracker->getTotalDistance() - startMileage));
	}

}

QString TestMoveTask::description() {
	return QString("Move:\t%1").arg(moveName);
}

void TestMoveTask::move(int distance) {

	int direction = 1;

	// Init
	distanceToMove = create->tracker->getTotalDistance() + distance;

	if (direction < 0) direction = -1;

	// Move, wait for distance, stop
	create->controller->setWheelSpeed(create->controller->targetSpeed*direction, create->controller->targetSpeed*direction);
	waitForDistance();
	create->controller->setWheelSpeed(0, 0);
}

void TestMoveTask::moveBack(int distance) {

	int direction = 1;

	// Init
	distanceToMove = create->tracker->getTotalDistance() + distance;

	if (direction < 0) direction = -1;

	// Move, wait for distance, stop
	create->controller->setWheelSpeed(-create->controller->targetSpeed*direction, -create->controller->targetSpeed*direction);
	waitForDistance();
	create->controller->setWheelSpeed(0, 0);

}

void TestMoveTask::waitForDistance() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ( create->tracker->getTotalDistance() < distanceToMove)
	{
		if (create->taskManager->getCurrentTask()->status == Task::Interrupted)
		{
			distanceToMove = create->tracker->getTotalDistance();
		}
		else
		{
		SleeperThread::msleep(create->intSetting("Task_TestMoveTask_WaitForEventIntervalDelay"));
		}
	}
}

void TestMoveTask::turn(int angle) {


	// Init
	angleToTurn = create->tracker->getTotalAngle() + std::abs((double)angle);
	distanceToMove = 0;

	// Move, wait for distance, stop
	if (angle > 0) {
		create->controller->setWheelSpeed(-create->controller->targetSpeed,+create->controller->targetSpeed);
	} else {
		create->controller->setWheelSpeed(+create->controller->targetSpeed,-create->controller->targetSpeed);
	}
	waitForAngle();
	create->controller->setWheelSpeed(0,0);
}

void TestMoveTask::waitForAngle() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ( create->tracker->getTotalAngle() < angleToTurn)
	{
		if (create->taskManager->getCurrentTask()->status == Task::Interrupted)
		{
			angleToTurn = create->tracker->getTotalAngle();
		}
		else
		{
			SleeperThread::msleep(create->intSetting("Task_TestMoveTask_WaitForEventIntervalDelay"));
		}
	}

}

