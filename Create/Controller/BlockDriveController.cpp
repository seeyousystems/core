/*
 *  BlockDriveController.cpp
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

#include "BlockDriveController.h"

#include "../COIL/COIL.h"
#include "../Library/Debug.h"

#include <cmath>


BlockDriveController::BlockDriveController(Create *create, int interval, int speed, int angle, int distance, SlowStartMode slowStart) :
	Controller("Block Drive", create, interval) {
	this->speed = speed;
	this->angle = angle;
	this->distance = distance;
	this->slowStart = slowStart;

	action = BlockDriveController::Idle;
}

BlockDriveController::~BlockDriveController() {

}

void BlockDriveController::run() {

	stopRequested = false;
	while(stopRequested == false) {

		if(action == BlockDriveController::Idle) {

			// Idle mode

		} else if(action == BlockDriveController::TurnLeft) {

			// Turn Left mode

			int result = create->coil->turn(speed, 1, angle, true);
			QString s1, s2;
			calculateAccuracy(angle, result);
			emit signalChangedAngle(result);
			action = BlockDriveController::Idle;
			Debug::print("[BlockDriveController] Angle set: %1\tReal Angle: %2", angle, result);

		} else if(action == BlockDriveController::TurnRight) {

			// Turn Right mode

			int result = create->coil->turn(-speed, 1, -angle, true);
			QString s1, s2;
			calculateAccuracy(-angle, result);
			emit signalChangedAngle(result);
			action = BlockDriveController::Idle;
			Debug::print("[BlockDriveController] Angle set: %1\tReal Angle: %2", -angle, result);

		} else if(action == BlockDriveController::MoveBackward) {

			// MoveBackward mode

			int linear = 10;
			int square = 16;
			int currentSpeed = 0;
			int result = 0;
			int minSpeed = -30;
			QString s1;
			switch (slowStart) {
				case BlockDriveController::Linear:
					// Linear motion
					for (int i = 1; i <= 10; i++) {
						currentSpeed = (-speed / linear) * i;
						if (currentSpeed > minSpeed) {
							currentSpeed = minSpeed;
						}
						result += move(currentSpeed, (-distance / linear));
					}
					break;
				case BlockDriveController::Square:
					// Exponential motion
					for (int i = 2; i <= square; i *= i) {
						speed = (-speed / square) * i;
						if (speed > minSpeed) {
							speed = minSpeed;
						}
						result += move(speed, (int) (-distance / (sqrt( square) - 1)));
					}
					break;
				default:
					// No slowstart (off)
					result = move(-speed, -distance);
					break;
			}
			calculateAccuracy(distance, result);
			emit signalMovedDistance(result);
			action = BlockDriveController::Idle;
			Debug::print("[BlockDriveController] Total Distance: " + s1.setNum(result));

		} else if(action == BlockDriveController::MoveForward) {

			// Move Forward mode

			int linear = 10;
			int square = 16;
			int currentSpeed = 0;
			int result = 0;
			int minSpeed = 30;
			QString s1;
			switch (slowStart) {
				case BlockDriveController::Linear:
					// Linear motion
					for (int i = 1; i <= 10; i++) {
						currentSpeed = (speed / linear) * i;
						if (currentSpeed < minSpeed) {
							currentSpeed = minSpeed;
						}
						result += move(currentSpeed, (distance / linear));
					}
					break;
				case BlockDriveController::Square:
					// Exponential motion
					for (int i = 2; i <= 16; i *= i) {
						speed = (speed / square) * i;
						if (speed < minSpeed) {
							speed = minSpeed;
						}
						result += move(speed, (int) (distance / (sqrt( square) - 1)));
					}
					break;
				default:
					// No slow start
					result = move(speed, distance);
					break;
			}
			calculateAccuracy(distance, result);
			emit signalMovedDistance(result);
			action = BlockDriveController::Idle;
			Debug::print("[BlockDriveController] Total Distance: " + s1.setNum(result));

		}


		// Sleep our interval...
		this->msleep(interval);

	}
}


int BlockDriveController::move(int speed, int distance) {
	int result = create->coil->driveDistance(speed, 0, distance, true);
	QString s1, s2, s3;
	Debug::print("[BlockDriveController] Distance set: " + s1.setNum(distance) + "\tReal Distance: " + s2.setNum(result) + " Speed: " + s3.setNum(speed));
	return result;
}

double BlockDriveController::calculateAccuracy(int expected, int result) {
	double accuracy = (double) expected / (double) result;
	Debug::print("Last accuracy: %1", accuracy);
	return accuracy;
}


void BlockDriveController::moveForward() {
	action = BlockDriveController::MoveForward;

}

void BlockDriveController::moveBackward() {
	action = BlockDriveController::MoveBackward;
}


void BlockDriveController::turnLeft() {
	action = BlockDriveController::TurnLeft;
}

void BlockDriveController::turnRight() {
	action = BlockDriveController::TurnRight;
}

void BlockDriveController::setSlowStartMode(QString mode) {
	if(mode == "Linear") {
		slowStart = BlockDriveController::Linear;
	} else if(mode == "Square") {
		slowStart = BlockDriveController::Square;
	} else {
		slowStart = BlockDriveController::Off;
	}
}


