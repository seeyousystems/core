/*
 *  TaskManager.h
 *
 *  The TaskManager is an essential part of the emss Core and is responsible for
 *  scheduling and executing Tasks. It inherits from CoreThread, so it runs in
 *  its own thread. The scheduling and execution of Tasks occur within the same run-loop.
 *  Internally, the TaskManager maintains a special queue called TaskList. This data structure
 *  is similar to a queue, except it extends features to enable the extraction of specific
 *  Tasks, such as ready-for-execution Tasks only. When a Task is added to the TaskList and
 *  scheduled for execution, its process() method is called repetively by the TaskManager
 *  which in turn performs the execution of the Task. Because of this architecture,
 *  the process() method of an individual Task may not block or delay for a long time as
 *  the Watchdog will kick in and report the TaskManager as a runaway CoreThread.
 *
 *
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

#ifndef TASKMANAGER_H_
#define TASKMANAGER_H_

#include "../CoreThread.h"

#include "../Task/TaskList.h"

class ThreadMonitorWatchdogAction;

class TaskManager : public CoreThread {

	Q_OBJECT

private:
	Task *currentTask;
	Task *idleTask;
	bool stopRequested;
	ThreadMonitorWatchdogAction *watchdogAction;

public:
	TaskList *tasks;



public:
	TaskManager(Create *create);
	virtual ~TaskManager();
	virtual void run();
	void stop();
	void addTask(Task *task);
	void waitForAllTasksToFinish();
	void start(QThread::Priority priority);
	Task* getCurrentTask();
	Task* getIdleTask();
	void setIdleTask(Task *idleTask);
	void interruptAllTasks();
	void interruptTask();

private:
	void sendTaskStatusChangeNotification(bool started, Task *task);

signals:
	void taskListChanged();

};

#endif /* TASKMANAGER_H_ */
