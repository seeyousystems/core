/*
 *  TaskList.h
 *
 *  The Task List is nothing more than a helper class for sequentially storing
 *  and retrieving Tasks, offering useful functions such as getNextSpecificTask(type).
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

#ifndef TASKLIST_H_
#define TASKLIST_H_

#include <QList>
#include <QReadWriteLock>

#include "Task.h"

class TaskList : CoreObject {

	Q_OBJECT

public:
	QList<Task*> *list;

private:
	int currentTask;
	QReadWriteLock lock;

public:
	TaskList();
	virtual ~TaskList();
	void add(Task *task);
	int count();
	Task* getCurrentTask();
	Task* getNextTask();
	Task* at(int index);
	QList<Task*> getSpecificTasks(QString type);
	Task* getNextSpecificTask(QString type);
	void deleteAllTasks();
	void setAllTaskStatus(Task::TaskStatus status);
	bool hasTaskWithStatus(Task::TaskStatus status);
};

#endif /* TASKLIST_H_ */
