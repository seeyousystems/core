/*
 *  JoystickNavigationTask.h
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

#ifndef JOYSTICKNAVIGATIONTASK_H_
#define JOYSTICKNAVIGATIONTASK_H_


#include "Task.h"
#include "../Navigation/SystemOfWeightsNavigation.h"

class JoystickNavigationTask : public Task{

	Q_OBJECT

private:
	SystemOfWeightsNavigation* navigation;

public:
	JoystickNavigationTask(Create *create, TaskPriority priority = Task::Normal);
	virtual ~JoystickNavigationTask();
	virtual void preProcess();
	virtual void process();
	virtual void postProcess();
	virtual QString description();
};

#endif /* JOYSTICKNAVIGATIONTASK_H_ */
