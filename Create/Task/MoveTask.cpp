/*
 *  MoveTask.cpp
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

#include "MoveTask.h"

#include "../Library/Debug.h"

MoveTask::MoveTask(QString name, Create *create, int interval, long x, long y, TaskPriority priority) :
	Task(name, create, interval, priority) {

	// Init
	destination = Trafo2D::trans(x,y);

	// Note: we don't do any calculations here, because the situation may change when the task is actually processed

}

MoveTask::~MoveTask(){

}


long MoveTask::x() {
	return (long)destination.trans().x();
}

long MoveTask::y() {
	return (long)destination.trans().y();
}
