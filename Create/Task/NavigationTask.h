/*
 *  NavigationTask.h
 *
 *  The NavigationTask sequentially navigates through all the navigation points
 *  contained within the Navigation module. Essentially, all the NavigationTask
 *  does is query the Navigation module for the wheel speeds which it then
 *  forwards to the Controller.
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

#ifndef NAVIGATIONTASK_H_
#define NAVIGATIONTASK_H_

#include "Task.h"

class NavigationTask : public Task  {

	Q_OBJECT

	public:
		int speed;
		int tick;

	public:
		NavigationTask(Create *create, int speed, TaskPriority priority = Task::Normal);
		virtual ~NavigationTask();
		virtual void preProcess();
		virtual void process();
		virtual void postProcess();
		virtual QString description();
};

#endif /* NAVIGATIONTASK_H_ */
