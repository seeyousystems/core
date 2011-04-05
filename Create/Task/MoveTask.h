/*
 *  MoveTask.h
 *
 *  Rotation and linear movement to destination.
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

#ifndef MOVETASK_H_
#define MOVETASK_H_

#include "Task.h"

#include "../Library/Math.h"
#include "../MovementTracker/MovementTracker.h"

class MoveTask : public Task
{
	Q_OBJECT

public:
	Trafo2D destination;

public:
	MoveTask(QString name, Create *create, int interval, long x, long y, TaskPriority priority = Task::Normal);
	virtual ~MoveTask();
	virtual void process() = 0;
	virtual QString description() = 0;
	long x();
	long y();
};



#endif /*MOVETASK_H_*/
