/*
 *  TestMoveTask.h
 *
 *  Several test movements are provided by the Test Move Task. These test
 *  movements are used for diagnostic purposes, such as calibration, accuracy
 *  observation, and general research. The following self-explaining test
 *  moves are available: Square, Rotate 90, Rotate 360, Triangle, Straight,
 *  Circle, and Vector Circle.
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

#ifndef TESTMOVETASK_H_
#define TESTMOVETASK_H_

#include "Task.h"

#include "../Library/Math.h"

#include <QTime>

#define AVAILABLE_TESTMOVETASKS "Square|Rotate 90|Rotate 360|Triangle|Straight|Circle|Vector Circle|Accuracy Test|WallFollowerAccuracy|Tracker Calibration|Straight with Weights|Custom Move"

class TestMoveTask : public Task {

	Q_OBJECT

private:
	Trafo2D destination;
	double angleToTurn;			// Note: this is double for extra precision during emulation calculation...
	double distanceToMove;		// Note: this is double for extra precision during emulation calculation...

public:
	QString moveName;
	int tick;
	int robotsize;
	int halfstep;
	int quarterstep;
	QTime starttime;
	Trafo2D startpos;
	double startMileage;
	long startdist;
	short customMovevl;
	short customMovevr;
	double customTime;
	double customDistance;
	int resetX;
	int resetY;

public:
	TestMoveTask(Create *create, QString move, TaskPriority priority = Task::Normal);
	virtual ~TestMoveTask();
	virtual void preProcess();
	virtual void process();
	virtual void postProcess();
	virtual QString description();
	void move(int distance);
	void moveBack(int distance);
	void turn(int angle);
	void waitForDistance();
	void waitForAngle();
};

#endif /* TESTMOVETASK_H_ */
