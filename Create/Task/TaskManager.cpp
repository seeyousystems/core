/*
 *  TaskManager.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"
#include "../Task/Task.h"
#include "../Controller/EmssController.h"

TaskManager::TaskManager(Create *create) {
	this->create = create;

	currentTask = NULL;
	stopRequested = false;
	tasks = new TaskList();
}

TaskManager::~TaskManager() {
	delete tasks;
}

void TaskManager::run() {

	// Enter processing loop...
	stopRequested = false;
	while (stopRequested == false) {

		if (currentTask != NULL && currentTask->status == Task::Running) {

			// Process the task
			currentTask->process();

			// Did we finish?
			if (currentTask->status == Task::Finished) {

				// Get the next task...
				Debug::print("[TaskManager] %1 Task finished", currentTask->name);
				//currentTask = tasks->getNextTask();

			} else {

				// Sleep our task interval...
				this->msleep(currentTask->interval);

			}

		} else {

			// Try to get a new task
			currentTask = tasks->getNextTask();
			if (currentTask == NULL) {
				this->msleep(create->intSetting("TASKMANAGER_INTERVAL"));
			} else if (currentTask->status == Task::Waiting) {
				currentTask->status = Task::Running;
				Debug::print("[TaskManager] %1 Task started", currentTask->name);
			}
		}
	}
}

void TaskManager::stop() {

	// Request a stop and wait for the controller to exit its process loop
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(50);

	Debug::print("[TaskManager] Task Manager stopped");

}

void TaskManager::addTask(Task *task) {
	Debug::print("[TaskManager] %1 Task added", task->name);
	tasks->add(task);
}

void TaskManager::start(QThread::Priority priority) {

	Debug::print("[TaskManager] Task Manager started");

	QThread::start(priority);
}
