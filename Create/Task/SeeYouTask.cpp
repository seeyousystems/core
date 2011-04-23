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
	Task("SeeYou Task", create, create->intSetting("TESTMOVETASK_INTERVAL"), priority) {
	this->move = move;
	this->speed = speed;
	tick = 0;
}

SeeYouTask::~SeeYouTask() {
	// TODO Auto-generated destructor stub
}

void SeeYouTask::process() {

	int DISTANCE = 1000; // 100 cm

	if(move == "Backwards") {


/* TODO: VERNON'S TASK
 * Use these functions to send commands to the robot.
 * We need to find out how to add intelligence to our navigation.
 *		((SeeYouController*) create->controller)->turn(90, this->speed);
 *		((SeeYouController*) create->controller)->move(1778, this->speed);
 */
//		long x = create->movementTracker->x();
//		long y = create->movementTracker->y();

		//Debug::print("Start:(%1,%2)", x, y);
		//printf("%d,%d", x/1000, y/1000);

		((SeeYouController*) create->controller)->move(-200, -(this->speed));

		//Debug::print("End:(%1,%2)", x, y);


		Debug::print("[SeeYouTask] Avoid Obstacles");
		status = Task::Finished;

	} else if(move == "Rotate 90") {
		((SeeYouController*) create->controller)->turn(90, this->speed);
		Debug::print("[SeeYouTask] Rotate 90 Task");
		status = Task::Finished;

	}
	else if(move == "Rotate -90")
	{
			((SeeYouController*) create->controller)->turn(-90, this->speed);
			Debug::print("[SeeYouTask] Rotate 90 Task");
			status = Task::Finished;

	}
	else if(move == "Straight") {
		((SeeYouController*) create->controller)->move(200, (this->speed));
		Debug::print("[SeeYouTask] Straight");
		status = Task::Finished;

	} else if(move == "Stop") {
			//status = Task::Interrupted;
			//this->status = Task::Immediate;
			//((SeeYouController*) create->controller)->emergencyStop();
		((SeeYouController*) create->controller)->emergencyStop();
				Debug::print("[SeeYouTask] STOP the Robot!");
				status = Task::Finished;
			}
	else if(move == "Rotate 360") {
		Debug::print("[SeeYouTask] Rotate 360 Task");
		status = Task::Finished;

	} else if(move == "Wall Follower") {

		Debug::print("[SeeYouTask] Wall Follower Task");
		status = Task::Finished;


/*=====================================
 * For Network Communication overrides
 =====================================*/
//TODO: Write Stop Command
	} else if(move == "NetComm_stop") {
//
//		if (create->taskManager->getTask() != NULL)
//		{
//			create->interruptTask();
//		}

		//((SeeYouController*) create->controller)->regularStop();

		Debug::print("[SeeYouTask:NETCOMM] Stop Task");
		status = Task::Finished;
	} else if(move == "NetComm_left") {

		((SeeYouController*) create->controller)->turn(23, this->speed);
		Debug::print("[SeeYouTask:NETCOMM] Left: 45 task");
		status = Task::Finished;
	} else if(move == "NetComm_right") {

		((SeeYouController*) create->controller)->turn(-23, this->speed);
		Debug::print("[SeeYouTask:NETCOMM] Right: 45 Task");
		status = Task::Finished;
	} else if(move == "NetComm_forward") {

		((SeeYouController*) create->controller)->move(500, this->speed);
		Debug::print("[SeeYouTask:NETCOMM] Forward Task");
		status = Task::Finished;

	} else if(move == "NetComm_backward") {
		((SeeYouController*) create->controller)->move(-50, -(this->speed));
		Debug::print("[SeeYouTask:NETCOMM] Backward Task");
		status = Task::Finished;
	}


	tick++;

}

QString SeeYouTask::description() {
	return QString("Move:\t%1\nSpeed:\t%2").arg(move).arg(speed);
}
