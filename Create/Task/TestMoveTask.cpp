/*
 *  TestMoveTask.cpp
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

#include "TestMoveTask.h"

#include "../Controller/EmssController.h"
#include "../Library/Math.h"
#include "../Library/Debug.h"
//#include "../Navigation/Navigation.h"
#include "../MovementTracker/MovementTracker.h"

TestMoveTask::TestMoveTask(Create *create, QString move, int speed, TaskPriority priority) :
	Task("Test Move", create, create->intSetting("TESTMOVETASK_INTERVAL"), priority) {
	this->move = move;
	this->speed = speed;
	tick = 0;
}

TestMoveTask::~TestMoveTask() {

}


void TestMoveTask::process() {

//	int DISTANCE = 1000; // 100 cm
//
//	if(move == "Square") {
//
////		((EmssController*) create->controller)->turn(90, this->speed);
//		((EmssController*) create->controller)->move(1778, this->speed);
//		((EmssController*) create->controller)->turn(-70.0, this->speed);
//		((EmssController*) create->controller)->move(8205, this->speed);
//		((EmssController*) create->controller)->turn(70.0, this->speed);
//		((EmssController*) create->controller)->move(1778, this->speed);
//
//		((EmssController*) create->controller)->turn(159.0, this->speed);
//
//		((EmssController*) create->controller)->move(1778, this->speed);
//		((EmssController*) create->controller)->turn(-70.0, this->speed);
//		((EmssController*) create->controller)->move(8205, this->speed);
//		((EmssController*) create->controller)->turn(70.0, this->speed);
//		((EmssController*) create->controller)->move(1778, this->speed);
//		status = Task::Finished;
//
//	} else if(move == "Rotate 90") {
//
//		((EmssController*) create->controller)->turn(90, this->speed);
//
//		//create->coil->setLEDState(0, 255, 255);
//		//((COIL*) create->coil)->setLEDState(0, 255, 255);
//
//		//((EmssController*) create->controller)->turn(160.0, this->speed);
//		status = Task::Finished;
//
//	} else if(move == "Rotate 360") {
//		//((EmssController*) create->coil)->setLEDState(COIL::LED_ADVANCE | COIL::LED_PLAY, 0, 255);
//
//
////		((EmssController*) create->controller)->turn(360, this->speed);
////		((EmssController*) create->controller)->turn(-360, this->speed);
//		status = Task::Finished;
//
//	} else if(move == "Triangle") {
//
//		((EmssController*) create->controller)->turn(60, this->speed);
//		((EmssController*) create->controller)->move(DISTANCE, this->speed);
//		((EmssController*) create->controller)->turn(120, this->speed);
//		((EmssController*) create->controller)->move(DISTANCE, this->speed);
//		((EmssController*) create->controller)->turn(120, this->speed);
//		((EmssController*) create->controller)->move(DISTANCE, this->speed);
//		status = Task::Finished;
//
//	} else if(move == "Straight") {
//
//		((EmssController*) create->controller)->move(DISTANCE, this->speed);
//		((EmssController*) create->controller)->move(-DISTANCE, -this->speed);
//		status = Task::Finished;
//
//	} else if(move == "Circle") {
//
//		double radius = 100;
//		double b = 20;
//		double len = 10;
//		double circ = 20;
//		double growth = 1.0;
//		double vl = 0.0;
//		double vr = 0.0;
//		int t = tick;
//
//		// Body path
//		{
//			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
//			double px = std::sin(Rad(t))*radius*f;
//			double py = std::cos(Rad(t))*radius*f;
//		}
//
//		// Outer path
//		{
//			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
//			double px = std::sin(Rad(t))*(radius*f + b);
//			double py = std::cos(Rad(t))*(radius*f + b);
//			double dx = std::cos(Rad(t))*(radius*f + b);
//			double dy = -1.0*std::sin(Rad(t))*(radius*f + b);
//			double d = std::sqrt(dx*dx + dy*dy);
//			vl = d / circ;
//		}
//
//		// Inner path
//		{
//			double f = 1+((double)Rad(t)*growth)-(double)Rad(t);
//			double px = std::sin(Rad(t))*(radius*f - b);
//			double py = std::cos(Rad(t))*(radius*f - b);
//			double dx = std::cos(Rad(t))*(radius*f - b);
//			double dy = -1.0*std::sin(Rad(t))*(radius*f - b);
//			double d = std::sqrt(dx*dx + dy*dy);
//			vr = d / circ;
//		}
//
//		vl *= 10.0;
//		vr *= 10.0;
//
//		((EmssController*) create->controller)->wheelDrive((short)vl,(short)vr);
//
//		if(tick > 360) {
//			status = Task::Finished;
//			((EmssController*) create->controller)->mode = EmssController::Idle;
//		}
//
//		Debug::print("%1 %2", (int)vl, (int)vr);
//
//	} else if(move == "Vector Circle") {
//
//		double radius = 100;
//		double b = 20;
//		double circ = 20;
//		double speed = 20;
//
//		Vector2D c(std::sin(Rad(tick))*radius, std::cos(Rad(tick))*radius);
//		Vector2D c1(std::cos(Rad(tick))*radius, -1.0*std::sin(Rad(tick))*radius);
//		Vector2D c2(-1.0 * std::sin(Rad(tick))*radius, -1.0*std::cos(Rad(tick))*radius);
//		//c1 = speed * c1;
//
//		Vector2D n = Trafo2D::rot(Rad(90)) * c1;
//
//		Vector2D l = c + (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
//		Vector2D r = c - (b * (n/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
//
//		Vector2D n1 = Trafo2D::rot(Rad(90)) * c2;
//
//		Vector2D l1 = c1 + (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
//		Vector2D r1 = c1 - (b * (n1/(Sqrt(n.x()*n.x()+n.y()*n.y()))));
//
//		double vl = (Sqrt(l1.x()*l1.x() + l1.y() * l1.y()))/circ;
//		double vr = (Sqrt(r1.x()*r1.x() + r1.y() * r1.y()))/circ;
//
//		vl *= speed;
//		vr *= speed;
//
//		Debug::print("%1 %2", (int)vl, (int)vr);
//
//		((EmssController*) create->controller)->wheelDrive((short)vl,(short)vr);
//
//		if(tick > 360) {
//			status = Task::Finished;
//			((EmssController*) create->controller)->mode = EmssController::Idle;
//		}
//	}

	tick++;

}

QString TestMoveTask::description() {
	return QString("Move:\t%1\nSpeed:\t%2").arg(move).arg(speed);
}

