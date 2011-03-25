/*
 *  TaskManager.h
 *
 *  The Task Manager is a separate thread which contains a Task List. The
 *  Task List is nothing more than a helper class for sequentially storing
 *  and retrieving Tasks, offering useful functions such as getNextSpecificTask(type).
 *  As soon as the Task Manager receives a Task, appends it to the Task List and
 *  starts to execute it. If the Task Manager receives more than one Task at a
 *  time, it appends it to the back of the Task List, where in time will eventually
 *  be executed. The Task Manager also allows Tasks to be interrupted. In this case
 *  the manager will set the current Task as Interrupted and go to the next Task in
 *  the list.
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include <QThread>

#include "../create.h"
#include "../Task/TaskList.h"

class TaskManager : public QThread {

	Q_OBJECT

private:
	Task *currentTask;
	bool stopRequested;

public:
	Create *create;
	TaskList *tasks;

public:
	TaskManager(Create *create);
	virtual ~TaskManager();
	virtual void run();
	void stop();
	void addTask(Task *task);
	void start(QThread::Priority priority);
};

#endif /* TASKMANAGER_H_ */
