/*
 *  StraightPathMoveTask.h
 *
 *  This Task moves the robot in a linear fashion to the given destination.
 *  Multiple Straight Path Move Tasks can be appended to the Task Manager
 *  in order to create a series of chunky movements.
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

#ifndef STRAIGHTPATHMOVETASK_H_
#define STRAIGHTPATHMOVETASK_H_

#include "MoveTask.h"
#include "../Controller/SeeYouController.h"
#include "../MovementTracker/MovementTracker.h"
#include "../Task/MoveTask.h"
#include "../Library/Algorithm/VFF.h"


class StraightPathMoveTask : public MoveTask {

	Q_OBJECT

public:
	int speed;

public:
	StraightPathMoveTask(Create *create, long x, long y, int speed, TaskPriority priority = Task::Normal);
	virtual ~StraightPathMoveTask();
	virtual void process();
	virtual QString description();
};

#endif /* STRAIGHTPATHMOVETASK_H_ */
