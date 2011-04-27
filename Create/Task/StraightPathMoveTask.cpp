/*
 *  StraightPathMoveTask.cpp
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

#include "StraightPathMoveTask.h"

#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

StraightPathMoveTask::StraightPathMoveTask(Create *create, long x, long y, int speed, TaskPriority priority) :
	MoveTask("Straight Path Move", create, create->intSetting("STRAIGHTPATHMOVETASK_INTERVAL"), x, y, priority) {
	this->speed = speed;
}

StraightPathMoveTask::~StraightPathMoveTask() {

}

void StraightPathMoveTask::process() {

//	// Calculate movement to destination based on current position
//	Trafo2D move = create->tracker->transformation.inv() * destination;
//	double distance = std::sqrt(move.trans().x()*move.trans().x() + move.trans().y()*move.trans().y());
//	double angle =   std::atan2(move.trans().x(), move.trans().y() );
//
//	// Process movement
//	((SeeYouController*) create->controller)->turn((int)-Deg(angle), this->speed);
//	((SeeYouController*) create->controller)->move((int)distance, this->speed);

	status = Task::Finished;

}

QString StraightPathMoveTask::description() {
	return QString("Dest X:\t%1\nDest Y:\t%2\nSpeed:\t%3").arg(destination.trans().x()).arg(destination.trans().y()).arg(speed);
}

