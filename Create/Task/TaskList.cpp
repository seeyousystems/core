/*
 *  TaskList.cpp
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

#include "TaskList.h"

TaskList::TaskList() {
	list = new QList<Task*>();
	currentTask = -1;
}

TaskList::~TaskList() {
	delete list;
}

void TaskList::add(Task *task) {
	list->append(task);
}

int TaskList::count() {
	return list->count();
}

Task* TaskList::getCurrentTask() {
	return list->at(currentTask);
}

Task* TaskList::at(int index) {
	return list->at(index);
}

Task* TaskList::getNextTask() {

	// Make sure there is a next task..
	if(list->count() == 0) return NULL;
	if(currentTask + 1 > list->count()-1) return NULL;

	// Working on an existing task?
	if(currentTask == -1) {

		// Just get the first task
		currentTask = 0;

	} else {

		// Mark old task status and get next
		if(list->at(currentTask)->status == Task::Running) list->at(currentTask)->status = Task::Interrupted;
		currentTask++;

	}

	// Marks as running and return
	list->at(currentTask)->status = Task::Running;
	return list->at(currentTask);
}

QList<Task*> TaskList::getSpecificTasks(QString type) {

	QList<Task*> specificTasks;
	for(int i = 0; i < list->count(); i++) {
		if(list->at(i)->name == type) specificTasks.append(list->at(i));
	}
	return specificTasks; //TODO: Use templates!

}

Task* TaskList::getNextSpecificTask(QString type) {

	Task *nextTask = getNextTask();
	while(nextTask != NULL) {
		if(nextTask->name == type) return nextTask;
		nextTask == getNextTask();
	}
	return NULL;

}

