/*
 *  TaskManager.cpp
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

#include "TaskManager.h"

#include <QApplication>

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"
#include "../Task/Task.h"
#include "../Controller/EmssController.h"
//#include "../RemoteInterface/RemoteInterface.h"
//#include "../Watchdog/Watchdog.h"
//#include "../Watchdog/ThreadMonitorWatchdogAction.h"

TaskManager::TaskManager(Create *create) : CoreThread("TaskManager", create){

	currentTask = NULL;
	idleTask = NULL;
	stopRequested = false;
	tasks = new TaskList();
	watchdogAction = NULL;

	// Watchdog this?
//	if(create->watchdog && create->boolSetting("TaskManager_Watchdog_Enabled")) {
//		watchdogAction = new ThreadMonitorWatchdogAction(QString("Task Manager"), core->intSetting("TaskManager_Interval"), core->doubleSetting("TaskManager_Watchdog_Flexibility"), core);
//		core->watchdog->addAction(watchdogAction);
//	}
}

TaskManager::~TaskManager() {
//	if(watchdogAction && core->watchdog) core->watchdog->removeAction(watchdogAction);
//	if(watchdogAction) delete watchdogAction;
	if(idleTask) delete idleTask;
	tasks->deleteAllTasks();
	delete tasks;
}

void TaskManager::run() {

	// Enter processing loop...
	stopRequested = false;
	while (stopRequested == false) {

		// Do we have a task to process (i.e. a running task)?
		if (currentTask != NULL && currentTask->status == Task::Running) {

			// Check if we want an interrupt, otherwise process the task
			if(!currentTask->interruptRequested()) {

				currentTask->process();

			} else {

				currentTask->status = Task::Interrupted;

			}

			// Did we finish?
			if (currentTask->status == Task::Finished) {

				currentTask->postProcess();
				emit taskListChanged();
				sendTaskStatusChangeNotification(false, currentTask);
				Debug::print("[TaskManager] %1 finished", currentTask->getName());

			} else if (currentTask->status == Task::Interrupted) {

				currentTask->postProcess();
				emit taskListChanged();
				sendTaskStatusChangeNotification(false, currentTask);
				Debug::warning("[TaskManager] %1 interrupted", currentTask->getName());

			} else {

				// Sleep our task interval...
//				if(watchdogAction) watchdogAction->ping(currentTask->getInterval());
				this->msleep(currentTask->getInterval());

			}

		} else {

			// Try to get a new task
			currentTask = tasks->getNextTask();

			if (currentTask == NULL) {

				// No task available... either wait the interval or process the idle task
				if(idleTask) {

					// We have a idle task to process

					// Process the idle task. If not already running, pre process...
					if(idleTask->status != Task::Running) {
						idleTask->preProcess();
						idleTask->status = Task::Running;
						emit taskListChanged();
						sendTaskStatusChangeNotification(true, idleTask);
					}
					idleTask->process();
//					if(watchdogAction) watchdogAction->ping(idleTask->getInterval());
					this->msleep(idleTask->getInterval());
				} else {

					// No idle task

					int interval = create->intSetting("TaskManager_Interval");
//					if(watchdogAction) watchdogAction->ping(interval);
					this->msleep(interval);
				}

			} else if (currentTask->status == Task::Waiting) {

				// Found a task which is waiting to be run!
				if(idleTask) {
					// Interrupt the idle task
					idleTask->postProcess();
					idleTask->status = Task::Interrupted;
					sendTaskStatusChangeNotification(false, idleTask);
				}
				currentTask->preProcess();
				currentTask->status = Task::Running;
				emit taskListChanged();
				sendTaskStatusChangeNotification(true, currentTask);
				Debug::print("[TaskManager] %1 started", currentTask->getName());

			}
		}

	}
}

void TaskManager::stop() {

	// Request a stop and wait for the controller to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(50);
//	if(watchdogAction) watchdogAction->setActive(false);

	Debug::print("[TaskManager] stopped");

}

void TaskManager::addTask(Task *task) {
	if(!task) return;

	// Immediate?
	if(task->priority == Task::Immediate) {
		// Interrupt all running or waiting to be run tasks
		// so that ours is executed immediately...
		interruptAllTasks();
	}

	// Add task
	tasks->add(task);

	// Show changes
	Debug::print("[TaskManager] %1 added", task->getName());
	emit taskListChanged();
}

void TaskManager::start(QThread::Priority priority) {

	Debug::print("[TaskManager] started");

//	if(watchdogAction) watchdogAction->setActive(true);
	QThread::start(priority);
}

Task* TaskManager::getCurrentTask() {
	return currentTask;
}

Task* TaskManager::getIdleTask() {
	return idleTask;
}

void TaskManager::setIdleTask(Task *idleTask) {
	this->idleTask = idleTask;
}

// Sends a notification message to all subscribers over the remote interface.
// If started == true, the message is of type "task-started", otherwise "task-finished"
void TaskManager::sendTaskStatusChangeNotification(bool started, Task *task) {
//	if(create->remoteInterface) {
//		RemoteInterfaceMessage *message = new RemoteInterfaceMessage( started == true ? "task-started" : "task-finished" );
//		message->properties->insert("task", task->name);
//		message->properties->insert("status", task->getStatusAsString());
//		create->remoteInterface->broadcastMessage(message);
//	}
}

void TaskManager::interruptAllTasks(){
	tasks->setAllTaskStatus(Task::Interrupted);
}

void TaskManager::interruptTask()
{
	currentTask->status = Task::Interrupted;
	//currentTask->interrupt();
}

// This method blocks until all tasks are finished
void TaskManager::waitForAllTasksToFinish() {
	while(tasks->hasTaskWithStatus(Task::Running) || tasks->hasTaskWithStatus(Task::Waiting)) {
		qApp->processEvents();
		SleeperThread::msleep(create->intSetting("TaskManager_Interval"));
	}
}


