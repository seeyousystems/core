/*
 *  Task.h
 *
 *  Core Tasks bind all the components of the emss framework together, enabling
 *  the robot to perform actions in its environment. As already mentioned,
 *  the Task Manager is responsible for executing the Tasks one by one.
 *  Each emss Task must inherit from the class Task and implement the method
 *  process(). When a Task is created, its initial status is Waiting. When
 *  the Task Manager decides to schedule the Task for execution, its status
 *  is set to Running. The process() method of the Task is called by the Task
 *  Manager at its given interval (each Task can define its own interval)
 *  until the task sets its status as Finished.
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

#ifndef TASK_H_
#define TASK_H_

#include <QString>

#include "../CoreObject.h"

// Defines the available tasks to Core, which can be created by the factory etc...
// Each task must be seperated by "|". Classes, such as CoreFactory can use
// this list to automatically generate appropriate GUI's for creating such tasks.
#define AVAILABLE_TASKS "TestMoveTask|WallFollowerTask|RoamingTask|DiscoveryTask|Discovery2Task|NavigationTask|FingerprintNavigationTask|AccuracyTestTask|DockAndChargeTask|UndockTask|ChangeModeTask|PauseTask|ScriptedTask"

class Task : public CoreObject
{
	Q_OBJECT

protected:
	bool interruptOnNextProcess;
	int interval;

public:
	enum TaskStatus { Waiting, Running, Finished, Interrupted } status;
	enum TaskPriority { Normal, Immediate } priority;

public:
	Task(QString name, Create *create, int interval, TaskPriority priority = Task::Normal);
	virtual ~Task();
	virtual void preProcess();
	virtual void process() = 0;
	virtual void postProcess();
	void interrupt();
	bool interruptRequested();
	virtual QString description() = 0;
	int getInterval();
	QString getName();
	QString getStatusAsString();

};

#endif /*TASK_H_*/
