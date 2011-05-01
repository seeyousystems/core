/*
 *  EmssController.cpp
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

#include "EmssController.h"

#include "../COIL/COIL.h"

#include "ArduinoController.h"

#include "../COIL/ArduinoCOIL.h"

#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

//#include "../Watchdog/ThreadMonitorWatchdogAction.h"

#include "../Map/HeatMap.h"

#include "../MovementTracker/Tracker.h"


EmssController::EmssController(Create *create, int speed, int interval) :
	Controller("EmssController", create, interval) {

	// Init
	mode = Idle;
	targetSpeed = speed;
	lastDebugInfo.restart();

	// Cache all settings
	debugInfoEnabled = create->boolSetting("Controller_EmssController_DebugInfoEnabled");
	debugInfoInterval = create->intSetting("Controller_EmssController_DebugInfoInterval_ms");
	bumperCollisionOffset = create->intSetting("Robot_BumperCollisionOffset_mm");
	cliffCollisionOpacity = create->doubleSetting("Map_HeatMap_CliffCollisionOpacity");
	cliffCollisionSize = create->intSetting("Map_HeatMap_CliffCollisionSize_mm");
	bumperCollisionOpacity = create->doubleSetting("Map_HeatMap_BumperCollisionOpacity");
	bumperCollisionSize = create->intSetting("Map_HeatMap_BumperCollisionSize_mm");
	emergencyStopEnabled = create->boolSetting("Controller_EmssController_EmergencyStopEnabled");
	robotWallSensorRange = create->doubleSetting("Robot_WallSensorRange_mm");
	robotDiameter = create->intSetting("Robot_Diameter_mm");
	wallCollisionOpacity = create->doubleSetting("Map_HeatMap_WallCollisionOpacity");
	wallCollisionSize = create->intSetting("Map_HeatMap_WallCollisionSize_mm");
	irCollisionOpacity = create->doubleSetting("Map_HeatMap_IRCollisionOpacity");
	irCollisionSize = create->intSetting("Map_HeatMap_IRCollisionSize_mm");
	openAreaOpacity = create->doubleSetting("Map_HeatMap_OpenAreaOpacity");
	openAreaSize = create->intSetting("Map_HeatMap_OpenAreaSize_mm");
}

EmssController::~EmssController() {

}

void EmssController::process() {

	// Determine wheel speeds
	if (mode == EmssController::Idle || mode == EmssController::EmergencyStop) {

		// Idle mode!

		Lwheel = 0;
		Rwheel = 0;


	} else if (mode == EmssController::WheelDrive) {

		// No change, just drive at current wheel values...
	}

	// Get all the sensor data...
	if(sensorData) {
		if(create->coil->getAllSensors(sensorData)) {
			emit signalSensorDataUpdated();
		} else {
			// Return out of the process until Core can process the
			// signal from COIL. This will help COIL disconnect
			// faster...
			return;
		}
	}

	// Get movement...
	double distanceDelta = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_DISTANCE);
	double angleDelta = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_ANGLE);

	// Emit signals for movement tracker
	emit signalMovedDistance(distanceDelta);
	emit signalChangedAngle(angleDelta);

	// Get other sensor data
	int sharpIRSensor = create->arduino->extractSensorFromData(create->arduinoController->sensorData, ArduinoCOIL::SENSOR_IR_2);
	sharpIRSensor *= 25.4;
//	int sharpIRSensor = create->coil->getIRSensorDistanceFromAnalogSignal(create->coil->extractSensorFromData(sensorData,COIL::SENSOR_ANALOG_SIGNAL));
	int wallIRSensor = create->coil->getWallSensorDistanceFromSignal(create->coil->extractSensorFromData(sensorData,COIL::SENSOR_WALL_SIGNAL));
	int bumpsWheelDrop = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_BUMPS_AND_WHEEL_DROPS);
	bool cliffLeft = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_LEFT);
	bool cliffFrontLeft = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_FRONT_LEFT);
	bool cliffFrontRight = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_FRONT_RIGHT);
	bool cliffRight = create->coil->extractSensorFromData(sensorData,COIL::SENSOR_CLIFF_RIGHT);
	bool isForwardsDirection = (Lwheel > 0 && Rwheel > 0);

	//ebug::print("[EmssController] sensor: %1", sharpIRSensor);

	// Drop detected?
	if (cliffLeft || cliffFrontLeft || cliffFrontRight || cliffRight) {
		int angle;
		if (cliffLeft){
			angle = create->intSetting("Robot_SideCliffSensorPositionAngle");
			emit signalObjectDetected(bumperCollisionOffset, angle, cliffCollisionOpacity, cliffCollisionSize );
		} else if (cliffFrontLeft){
			angle = create->intSetting("Robot_FrontCliffSensorPositionAngle");
			emit signalObjectDetected(bumperCollisionOffset, angle, cliffCollisionOpacity, cliffCollisionSize );
		} else if (cliffFrontRight){
			angle = -create->intSetting("Robot_FrontCliffSensorPositionAngle");
			emit signalObjectDetected(bumperCollisionOffset, angle, cliffCollisionOpacity, cliffCollisionSize );
		} else if (cliffRight){
			angle = -create->intSetting("Robot_SideCliffSensorPositionAngle");
			emit signalObjectDetected(bumperCollisionOffset, angle, cliffCollisionOpacity, cliffCollisionSize );
		}
		if(isForwardsDirection && create->boolSetting("Controller_EmssController_EmergencyStopEnabled")) emergencyStop();
	}

	// Bumper Collision?
	if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
		int angle = 0;
		// both bumper?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) && ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)){
			angle = 0;
		}
		// only left bumper?
		else if ((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT){
			angle = 45;
		}
		// only right bumper?
		else if ((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT){
			angle = -45;
		}
		emit signalObjectDetected(bumperCollisionOffset, angle, bumperCollisionOpacity, bumperCollisionSize );
		if(isForwardsDirection && emergencyStopEnabled) emergencyStop();
	}

	// Wall detected?
	if(wallIRSensor < robotWallSensorRange) {
		emit signalObjectDetected(robotDiameter/2 + wallIRSensor, -90, wallCollisionOpacity, wallCollisionSize ); // Angle is -90 because the sensor points straight out to the right...
	}

	// Object detected?



	if (sharpIRSensor < create->intSetting("Robot_SharpIRSensorCutoffValue")) {
		emit signalObjectDetected(sharpIRSensor, 0, irCollisionOpacity, irCollisionSize ); // Angle is 0 because it is straight ahead always!
	}

	// Send wheel speeds to COIL
	if(mode != EmssController::EmergencyStop) {
		// Set new wheel speed
		create->coil->directDrive(Lwheel, Rwheel);
		emit signalChangedWheelSpeed((int)Lwheel, (int)Rwheel);

		// Register on heat map as safe area
		create->heatMap->registerHeat(ColorMap::OpenAreaChannel, create->tracker->getX(), create->tracker->getY(), openAreaOpacity, openAreaSize);
	}

	// Time for debug info?
	if(debugInfoEnabled && lastDebugInfo.elapsed() > debugInfoInterval) {
		lastDebugInfo.restart();
		Debug::print("[EmssController] vl=%1\tvr=%2", Lwheel, Rwheel);
	}

}

void EmssController::emergencyStop() {

	create->coil->directDrive(0, 0);
	mode = EmssController::EmergencyStop;
	emit signalChangedWheelSpeed(0, 0);
	Debug::warning("[EmssController] emergency stop!");

}


void EmssController::setWheelSpeed(int Lwheel, int Rwheel) {

	lock.lockForWrite(); {

		if(mode == EmssController::EmergencyStop && (Lwheel > 0 || Rwheel > 0) ) {
			lock.unlock();
			return;
		}

		this->mode = EmssController::WheelDrive;
		this->Lwheel = (short)Lwheel;
		this->Rwheel = (short)Rwheel;

	} lock.unlock();

}
