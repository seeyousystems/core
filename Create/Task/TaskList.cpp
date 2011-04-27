/*
 *  TaskList.cpp
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

#include "TaskList.h"

#include <QReadLocker>

TaskList::TaskList() : CoreObject("TaskList"){
	list = new QList<Task*>();
	currentTask = -1;
}

TaskList::~TaskList() {
	delete list;
}

void TaskList::add(Task *task) {
	lock.lockForWrite();
	{
		list->append(task);
	}
	lock.unlock();
}

int TaskList::count() {
	int retVal;
	lock.lockForRead();
	{
		retVal = list->count();
	}
	lock.unlock();
	return retVal;
}

Task* TaskList::getCurrentTask() {
	Task* retVal;
	lock.lockForRead();
	{
		retVal = list->at(currentTask);
	}
	lock.unlock();
	return retVal;
}

Task* TaskList::at(int index) {
	Task* retVal;
	lock.lockForRead();
	{
		retVal = list->at(index);
	}
	lock.unlock();
	return retVal;
}

Task* TaskList::getNextTask() {

	Task *task = NULL;

	lock.lockForRead(); {

		// Make sure there is a next task..
		if(list->count() > 0 && currentTask+1 <= list->count()-1) {

			currentTask++;
			task = this->at(currentTask);
		}

	} lock.unlock();

	return task;
}

QList<Task*> TaskList::getSpecificTasks(QString type) {
	QList<Task*> specificTasks;
	lock.lockForRead(); {
		for(int i = 0; i < this->count(); i++) {
			if(this->at(i)->getName() == type) specificTasks.append(this->at(i));
		}
	} lock.unlock();
	return specificTasks;

}

Task* TaskList::getNextSpecificTask(QString type) {
	lock.lockForRead(); {
		Task *nextTask = this->getNextTask();
		while(nextTask != NULL) {
			if(nextTask->getName() == type) {
				lock.unlock();
				return nextTask;
			}
			nextTask = this->getNextTask();
		}
	} lock.unlock();
	return NULL;

}


void TaskList::deleteAllTasks() {
	lock.lockForWrite(); {
		for(int i = 0; i < list->count(); i++) {
			delete list->at(i);
		}
		list->clear();
	} lock.unlock();
}

void TaskList::setAllTaskStatus(Task::TaskStatus status){
	lock.lockForRead(); {
		for (int i = 0; i < list->count(); i++){
			list->at(i)->status = status;
		}
	} lock.unlock();
}

bool TaskList::hasTaskWithStatus(Task::TaskStatus status) {
	QReadLocker locker(&lock);
	for (int i = 0; i < list->count(); i++){
		if(list->at(i)->status == status) return true;
	}
	return false;
}
