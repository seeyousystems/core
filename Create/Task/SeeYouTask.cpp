/*
 * SeeYouTask.cpp
 *
 *  Created on: Mar 27, 2011
 *      Author: seeyou
 */

#include "SeeYouTask.h"

#include "../Controller/SeeYouController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
//#include "../Navigation/Navigation.h"
#include "../MovementTracker/MovementTracker.h"

SeeYouTask::SeeYouTask(Create *create, QString move, int speed, TaskPriority priority) :
	Task("Test Move", create, create->intSetting("TESTMOVETASK_INTERVAL"), priority) {
	this->move = move;
	this->speed = speed;
	tick = 0;
}

SeeYouTask::~SeeYouTask() {
	// TODO Auto-generated destructor stub
}

void SeeYouTask::process() {

	int DISTANCE = 1000; // 100 cm

	if(move == "Avoid Obstacles") {

		/* TODO: VERNON'S TASK
		 * Use these functions to send commands to the robot.
		 * We need to find out how to add intelligence to our navigation.
		 *		((SeeYouController*) create->controller)->turn(90, this->speed);
		 *		((SeeYouController*) create->controller)->move(1778, this->speed);
		 */

		Debug::print("[SeeYouTask] Avoid Obstacles");
		status = Task::Finished;

	} else if(move == "Rotate 90") {

		((SeeYouController*) create->controller)->turn(90, this->speed);
		Debug::print("[SeeYouTask] Rotate 90 Task");
		status = Task::Finished;

	} else if(move == "Rotate 360") {
		Debug::print("[SeeYouTask] Rotate 360 Task");
		status = Task::Finished;

	} else if(move == "Wall Follower") {

		Debug::print("[SeeYouTask] Wall Follower Task");
		status = Task::Finished;
	}

	tick++;

}

QString SeeYouTask::description() {
	return QString("Move:\t%1\nSpeed:\t%2").arg(move).arg(speed);
}
