/*
 * SeeYouController.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: abraham
 */

#include "SeeYouController.h"
#include "SensorController.h"
#include "../COIL/COIL.h"
#include "../COIL/ArduinoCOIL.h"
#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"
#include <cfloat>

SeeYouController::SeeYouController(Create *create, int speed, int interval) :
	Controller("SeeYou", create, interval) {

	this->speed = speed;
	yokeX = 0.0;
	yokeY = 0.0;
	angleToTurn = 0.0;
	angleTurned = 0.0;
	distanceToMove = 0.0;
	distanceMoved = 0.0;
	mode = Idle;
	tag = 0;
	tag_buffer = -1;
	test = 0;
	counter = 0;
	RFID_DB db;
	globaltag = -1;

	vector<RFID_DB> temp(100, db);
	predefinedDB = temp;
//
////	for(unsigned int i = 0; i < predefinedDB.size(); i++) {
////		db.id = i;
////		db.x = 0;
////		db.y = 0;
////		predefinedDB[i] = db;
////	}
//
db.id = 1;
db.x = 0;
db.y = 0;
predefinedDB[1] = db;

db.id = 2;
db.x = -122;
db.y = 0;
predefinedDB[2] = db;

db.id = 3;
db.x = -244;
db.y = 0;
predefinedDB[3] = db;

db.id = 4;
db.x = -366;
db.y = 0;
predefinedDB[4] = db;

db.id = 5;
db.x = -488;
db.y = 0;
predefinedDB[5] = db;

db.id = 6;
db.x = -600;
db.y = 0;
predefinedDB[6] = db;

db.id = 7;
db.x = -722;
db.y = 0;
predefinedDB[7] = db;

db.id = 8;
db.x = -844;
db.y = 0;
predefinedDB[8] = db;

db.id = 9;
db.x = -966;
db.y = 0;
predefinedDB[9] = db;

db.id = 10;
db.x = -1088;
db.y = 0;
predefinedDB[10] = db;

db.id = 11;
db.x = -1088;
db.y = -122;
predefinedDB[11] = db;

db.id = 12;
db.x = -1088;
db.y = -244;
predefinedDB[12] = db;

db.id = 13;
db.x = -1088;
db.y = -366;
predefinedDB[13] = db;

db.id = 14;
db.x = -1088;
db.y = -488;
predefinedDB[14] = db;

db.id = 15;
db.x = -1088;
db.y = -600;
predefinedDB[15] = db;

db.id = 16;
db.x = -1088;
db.y = -722;
predefinedDB[16] = db;

db.id = 17;
db.x = -1088;
db.y = 122;
predefinedDB[17] = db;

db.id = 18;
db.x = -1088;
db.y = 244;
predefinedDB[18] = db;
//
//db.id = 19;
//db.x = -1088;
//db.y = 366;
//predefinedDB[19] = db;
//
//db.id = 20;
//db.x = -1088;
//db.y = 488;
//predefinedDB[20] = db;

db.id = 21;
db.x = 122;
db.y = 0;
predefinedDB[21] = db;

db.id = 22;
db.x = 244;
db.y = 0;
predefinedDB[22] = db;

db.id = 23;
db.x = 366;
db.y = 0;
predefinedDB[23] = db;

db.id = 24;
db.x = 488;
db.y = 0;
predefinedDB[24] = db;

db.id = 25;
db.x = 610;
db.y = 0;
predefinedDB[25] = db;

//db.id = 26;
//db.x = 0;
//db.y = 0;
//predefinedDB[26] = db;

db.id = 27;
db.x = 732;
db.y = 0;
predefinedDB[27] = db;

db.id = 28;
db.x = 854;
db.y = 0;
predefinedDB[28] = db;

db.id = 29;
db.x = 976;
db.y = 0;
predefinedDB[29] = db;

db.id = 30;
db.x = 1098;
db.y = 0;
predefinedDB[30] = db;

//db.id = 31;
//db.x = 0;
//db.y = 0;
//predefinedDB[31] = db;

db.id = 32;
db.x = 1220;
db.y = 0;
predefinedDB[32] = db;

db.id = 33;
db.x = 1342;
db.y = 0;
predefinedDB[33] = db;

db.id = 34;
db.x = 1464;
db.y = 0;
predefinedDB[34] = db;

db.id = 35;
db.x = 1586;
db.y = 0;
predefinedDB[35] = db;

db.id = 36;
db.x = 1708;
db.y = 0;
predefinedDB[36] = db;

db.id = 37;
db.x = 1830;
db.y = 0;
predefinedDB[37] = db;

db.id = 38;
db.x = 1952;
db.y = 0;
predefinedDB[38] = db;

db.id = 39;
db.x = 2074;
db.y = 0;
predefinedDB[39] = db;

db.id = 40;
db.x = 2196;
db.y = 0;
predefinedDB[40] = db;

db.id = 41;
db.x = 2318;
db.y = 0;
predefinedDB[41] = db;

db.id = 42;
db.x = 2440;
db.y = 0;
predefinedDB[42] = db;

db.id = 43;
db.x = 2562;
db.y = 0;
predefinedDB[43] = db;

db.id = 44;
db.x = 2562;
db.y = 122;
predefinedDB[44] = db;

db.id = 45;
db.x = 2562;
db.y = 244;
predefinedDB[45] = db;

db.id = 46;
db.x = 610;
db.y = -122;
predefinedDB[46] = db;

db.id = 47;
db.x = 610;
db.y = -244;
predefinedDB[47] = db;

db.id = 48;
db.x = -966;
db.y = -488;
predefinedDB[48] = db;

db.id = 49;
db.x = -844;
db.y = -488;
predefinedDB[49] = db;

db.id = 50;
db.x = -722;
db.y = -488;
predefinedDB[50] = db;

db.id = 51;
db.x = -600;
db.y = -488;
predefinedDB[51] = db;

db.id = 52;
db.x = -478;
db.y = -488;
predefinedDB[52] = db;

db.id = 53;
db.x = -1088;
db.y = -722;
predefinedDB[53] = db;

db.id = 54;
db.x = -1088;
db.y = -844;
predefinedDB[54] = db;

db.id = 55;
db.x = -1210;
db.y = -688;
predefinedDB[55] = db;

db.id = 56;
db.x = -1332;
db.y = -688;
predefinedDB[56] = db;

db.id = 57;
db.x = -1454;
db.y = -688;
predefinedDB[57] = db;

db.id = 58;
db.x = -1088;
db.y = 366;
predefinedDB[58] = db;

db.id = 59;
db.x = -1088;
db.y = 488;
predefinedDB[59] = db;

db.id = 60;
db.x = -1088;
db.y = 610;
predefinedDB[60] = db;

//db.id = 61;
//db.x = 0;
//db.y = 0;
//predefinedDB[61] = db;

db.id = 62;
db.x = -1088;
db.y = 854;
predefinedDB[62] = db;

db.id = 63;
db.x = -1088;
db.y = 732;
predefinedDB[63] = db;

db.id = 64;
db.x = -1088;
db.y = 976;
predefinedDB[64] = db;

db.id = 65;
db.x = 0;
db.y = 0;
predefinedDB[65] = db;

db.id = 66;
db.x = -1088;
db.y = 1098;
predefinedDB[66] = db;

db.id = 67;
db.x = -1088;
db.y = 1220;
predefinedDB[67] = db;

db.id = 68;
db.x = -1088;
db.y = 1342;
predefinedDB[68] = db;

db.id = 69;
db.x = -1088;
db.y = 1464;
predefinedDB[69] = db;

db.id = 70;
db.x = -1088;
db.y = 1586;
predefinedDB[70] = db;

db.id = 71;
db.x = 0;
db.y = 0;
predefinedDB[71] = db;

db.id = 72;
db.x = 0;
db.y = 0;
predefinedDB[72] = db;

db.id = 73;
db.x = 0;
db.y = 0;
predefinedDB[73] = db;

db.id = 74;
db.x = 0;
db.y = 0;
predefinedDB[74] = db;

db.id = 75;
db.x = 0;
db.y = 0;
predefinedDB[75] = db;

//db.id = 76;
//db.x = 0;
//db.y = 0;
//predefinedDB[76] = db;
//
//db.id = 77;
//db.x = 0;
//db.y = 0;
//predefinedDB[77] = db;
//
//db.id = 78;
//db.x = 0;
//db.y = 0;
//predefinedDB[78] = db;
//
//db.id = 79;
//db.x = 0;
//db.y = 0;
//predefinedDB[79] = db;
//
//db.id = 80;
//db.x = 0;
//db.y = 0;
//predefinedDB[80] = db;
//
//db.id = 81;
//db.x = 0;
//db.y = 0;
//predefinedDB[81] = db;
//
//db.id = 82;
//db.x = 0;
//db.y = 0;
//predefinedDB[82] = db;
//
//db.id = 83;
//db.x = 0;
//db.y = 0;
//predefinedDB[83] = db;
//
//db.id = 84;
//db.x = 0;
//db.y = 0;
//predefinedDB[84] = db;
//
//db.id = 85;
//db.x = 0;
//db.y = 0;
//predefinedDB[85] = db;
//
//db.id = 86;
//db.x = 0;
//db.y = 0;
//predefinedDB[86] = db;
//
//db.id = 87;
//db.x = 0;
//db.y = 0;
//predefinedDB[87] = db;
//
//db.id = 88;
//db.x = 0;
//db.y = 0;
//predefinedDB[88] = db;
//
//db.id = 89;
//db.x = 0;
//db.y = 0;
//predefinedDB[89] = db;
//
//db.id = 90;
//db.x = 0;
//db.y = 0;
//predefinedDB[90] = db;
//
//db.id = 91;
//db.x = 0;
//db.y = 0;
//predefinedDB[91] = db;
//
//db.id = 92;
//db.x = 0;
//db.y = 0;
//predefinedDB[92] = db;
//
//db.id = 93;
//db.x = 0;
//db.y = 0;
//predefinedDB[93] = db;
//
//db.id = 94;
//db.x = 0;
//db.y = 0;
//predefinedDB[94] = db;
//
//db.id = 95;
//db.x = 0;
//db.y = 0;
//predefinedDB[95] = db;
//
//db.id = 96;
//db.x = 0;
//db.y = 0;
//predefinedDB[96] = db;
//
//db.id = 97;
//db.x = 0;
//db.y = 0;
//predefinedDB[97] = db;
//
//db.id = 98;
//db.x = 0;
//db.y = 0;
//predefinedDB[98] = db;
//
//db.id = 99;
//db.x = 0;
//db.y = 0;
//predefinedDB[99] = db;
//
//db.id = 100;
//db.x = 0;
//db.y = 0;
//predefinedDB[100] = db;

//	predefinedDB[100] = {
//
//	{1, 0, 0},
//	{2, -122, 0},
//	{3, -244, 0},
//	{4, -366, 0},
//	{5, -488, 0},
//	{6, -600, 0},
//	{7, -722, 0},
//	{8, -844, 0},
//	{9, -966, 0},
//	{10, -1088, 0},
//
//	{11, -1088, -122},
//	{12, -1088, -244},
//	{13, -1088, -366},
//	{14, -1088, -488},
//	{15, -1088, -600},
//	{16, -1088, -722},
//	{17, -1088, 122},
//	{18, -1088, 244},
//	{19, 0, 0},
//
//	{20, 0, 0},
//	{21, 0, 0},
//	{22, 0, 0},
//	{23, 0, 0},
//	{24, 0, 0},
//	{25, 0, 0},
//	{26, 0, 0},
//	{27, 0, 0},
//	{28, 0, 0},
//	{29, 0, 0},
//
//	{30, 0, 0},
//	{31, 0, 0},
//	{32, 0, 0},
//	{33, 0, 0},
//	{34, 0, 0},
//	{35, 0, 0},
//	{36, 0, 0},
//	{37, 0, 0},
//	{38, 0, 0},
//	{39, 0, 0},
//
//	{40, 0, 0},
//	{41, 0, 0},
//	{42, 0, 0},
//	{43, 0, 0},
//	{44, 0, 0},
//	{45, 0, 0},
//	{46, 0, 0},
//	{47, 0, 0},
//	{48, 0, 0},
//	{49, 0, 0},
//
//	{50, 0, 0},
//	{51, 0, 0},
//	{52, 0, 0},
//	{53, 0, 0},
//	{54, 0, 0},
//	{55, 0, 0},
//	{56, 0, 0},
//	{57, 0, 0},
//	{58, 0, 0},
//	{59, 0, 0},
//
//	{60, 0, 0},
//	{61, 0, 0},
//	{62, 0, 0},
//	{63, 0, 0},
//	{64, 0, 0},
//	{65, 0, 0},
//	{66, 0, 0},
//	{67, 0, 0},
//	{68, 0, 0},
//	{69, 0, 0},
//
//	{70, 0, 0},
//	{71, 0, 0},
//	{72, 0, 0},
//	{73, 0, 0},
//	{74, 0, 0},
//	{75, 0, 0},
//	{76, 0, 0},
//	{77, 0, 0},
//	{78, 0, 0},
//	{79, 0, 0},
//
//	{80, 0, 0},
//	{81, 0, 0},
//	{82, 0, 0},
//	{83, 0, 0},
//	{84, 0, 0},
//	{85, 0, 0},
//	{86, 0, 0},
//	{87, 0, 0},
//	{88, 0, 0},
//	{89, 0, 0},
//
//	{90, 0, 0},
//	{91, 0, 0},
//	{92, 0, 0},
//	{93, 0, 0},
//	{94, 0, 0},
//	{95, 0, 0},
//	{96, 0, 0},
//	{97, 0, 0},
//	{98, 0, 0},
//	{99, 0, 0},
//	{100, 0, 0}
//
//	};


}

SeeYouController::~SeeYouController() {

}

void SeeYouController::run() {
	// Enter processing loop
	stopRequested = false;
	while (stopRequested == false) {

		// Get movement...
		double distanceDelta = coil_getDistance();
		double angleDelta = coil_getAngle();

		// Emit signals for movement tracker
		emit signalMovedDistance(distanceDelta);
		emit signalChangedAngle(angleDelta);

		// Get other sensor data
		int sharpIRSensor = coil_getAnalogSensorDistance();
		int bumpsWheelDrop = coil_getBumpsAndWheelDrops();

		// Collision?
		if (((COIL::BUMPWHEELDROP_BUMP_LEFT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_LEFT) || ((COIL::BUMPWHEELDROP_BUMP_RIGHT & bumpsWheelDrop) == COIL::BUMPWHEELDROP_BUMP_RIGHT)) {
			emit signalCollision();
			if(create->boolSetting("SEEYOUCONTROLLER_EMERGENCY_STOP_ENABLED") == true) emergencyStop();
		}

		// Object detected?
		if (sharpIRSensor < create->intSetting("SEEYOUCONTROLLER_SHARP_IR_SENSOR_CUTOFF_VALUE")) {
			emit signalObjectDetected(sharpIRSensor, 0); // Angle is 0 because it is strait ahead always!
			if(create->boolSetting("SEEYOUCONTROLLER_EMERGENCY_STOP_ENABLED") == true && sharpIRSensor < create->intSetting("SEEYOUCONTROLLER_SHARP_IR_SENSOR_EMERGENCYSTOP_BUFFER_MM")) emergencyStop();
		}

		// Processs movement
		distanceMoved += distanceDelta;
		angleTurned += angleDelta;

		// Determine wheel speeds
		if (mode == SeeYouController::Idle || mode == SeeYouController::EmergencyStop) {

			// Idle mode!

			Lwheel = 0;
			Rwheel = 0;

		} else if (mode == SeeYouController::Joystick) {

			if (this->yokeY <= 0.06 && this->yokeY >= -0.06) {
				Lwheel = -(short)this->speed * (this->yokeX);
				Rwheel =  (short)this->speed * (this->yokeX);
			}
			else {
				short speed = (short) (this->speed * this->yokeY);
				Lwheel = speed - (short)(speed * this->yokeX);
				Rwheel = speed + (short)(speed * this->yokeX);

				// Fix
				if (Lwheel > this->speed) Lwheel = this->speed;
				if (Rwheel > this->speed) Rwheel = this->speed;
				if (Lwheel < -this->speed) Lwheel = -this->speed;
				if (Rwheel < -this->speed) Rwheel = -this->speed;
			}

		} else if (mode == SeeYouController::Move) {
//counter++;
			Debug::print("[SeeYouController] Global Tag Before Stop: %1", globaltag);
			Debug::print("[SeeYouController] RFID Tag Before Stop: %1", create->arduinoController->sensorPacket.RFID);
			if (create->stopTask() || globaltag == create->arduinoController->sensorPacket.RFID) {

				if (globaltag == create->arduinoController->sensorPacket.RFID)
				{
					Debug::print("[SeeYouController] You have reached the target tag: %1", globaltag);
					globaltag = -1;
				}
				Debug::print("[SeeYouController] Global Tag: %1", globaltag);
//				globaltag = 0;
				if (distanceToMove > 0)
				{
					distanceMoved = DBL_MAX;
				}
				else
				{
					distanceMoved = -DBL_MAX;
				}

			}
			else
			{
				Lwheel = speed;
				Rwheel = speed;
			}

//			Lwheel = speed;
//			Rwheel = speed;

			//create->arduinoController->sensorPacket.frontIR;
		//	tag = SeeYouController::arduino_getRFID();

//			for (int i = 0; i < 100; i++)
//			{
//				if (predefinedDB[i].id == tag)
//				{
//					test = 1;
//					break;
//				}
//			}//			if (angleToTurn > 0) {
			//				Lwheel = -speed;
			//				Rwheel = +speed;
			//			} else {
			//				Lwheel = +speed;
			//				Rwheel = -speed;
			//			}
//
//			if (test == 1 && tag_buffer != tag)
//			{
//				//emergencyStop();
//				Lwheel = 0;
//				Rwheel = 0;
//
//				//for(int i = 0; i <= 10000000; i++)
//				//{
//				//}
//
//				Debug::print("[SensorController] STOP");
//				Debug::print("[SensorController] ID %l",predefinedDB[tag].id);
//				Debug::print("[SensorController] X %l", predefinedDB[tag].x);
//				Debug::print("[SensorController] Y %l", predefinedDB[tag].y);
//				tag_buffer = tag;
//				test = 0;
//				tag = 0;
//			}s
//			else if (create->vffAI->hist.oMega >= 45 && create->vffAI->hist.oMega <= 90.0 && obuffer != create->vffAI->hist.oMega)
//			{
//				Debug::print("[SeeYouController] Turn Left: %l", create->vffAI->hist.oMega);
//				Debug::print("[SeeYouController] o_buffer %l", obuffer);
//				//mode = SeeYouController::EmergencyStop;
//				//emergencyStop();
//				Lwheel = -speed;
//				Rwheel = +speed;
//				obuffer = create->vffAI->hist.oMega;
//			}
//			else if (create->vffAI->hist.oMega >= 91 && create->vffAI->hist.oMega <= 135.0)
//			{
//				Debug::print("[SeeYouController] Turn Right %l", create->vffAI->hist.oMega);
//				Debug::print("[SeeYouController] o_buffer %l", obuffer);
//				Lwheel = +speed;
//				Rwheel = -speed;
//				obuffer = create->vffAI->hist.oMega;
//			}
//			else
//			{
//				Debug::print("[SeeYouController] Go Straight: %1", create->vffAI->hist.oMega);
//				//Debug::print("[SeeYouController] obuffer %l", obuffer);
//				Lwheel = speed;
//				Rwheel = speed;
//				//obuffer = create->vffAI->hist.oMega;
//			}

		} else if (mode == SeeYouController::Turn) {

			// Turn mode!

			if (create->stopTask())
			{

				if (angleToTurn > 0)
				{
					angleTurned = DBL_MAX;
				}
				else
				{
					angleTurned = -DBL_MAX;
				}
			}
			else
			{
				if (angleToTurn > 0)
				{
					Lwheel = -speed;
					Rwheel = +speed;
				}
				else
				{
					Lwheel = +speed;
					Rwheel = -speed;
				}
			}

		} else if (mode == SeeYouController::WheelDrive) {

			// No change, just drive at current wheel values...
		}
		else if (mode == SeeYouController::Avoidance) {
//			if(create->vffAI->hist.oMega > 60 && create->vffAI->hist.oMega < -70)
//			{
//				Debug::print("[SeeYouController] Vernon sucks");
//				mode = SeeYouController::EmergencyStop;
//			}

		}

		// Send wheel speeds to COIL
		if(mode != SeeYouController::EmergencyStop) coil_directDrive(Lwheel, Rwheel);

		// Sleep our interval...
		this->msleep(interval);

	}
}

void SeeYouController::tags(int tagnumber){//Added by Hanam Apr 21
	globaltag = tagnumber;
}

void SeeYouController::emergencyStop() {
	mode = SeeYouController::EmergencyStop;
	coil_directDrive(0, 0);
}
void SeeYouController::regularStop() {
	mode = SeeYouController::Idle;
}
void SeeYouController::setYoke(double yokeX, double yokeY) {
	this->yokeX = yokeX;
	this->yokeY = yokeY;
	mode = SeeYouController::Joystick;
	//Debug::print("[SeeYouController] setYoke x:%1  y:%2", this->yokeX, this->yokeY);
}

void SeeYouController::move(int distance, int speed) {

	if(mode == SeeYouController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = 0;
	angleTurned = 0;
	distanceToMove = distance;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = SeeYouController::Move;
	waitForDistance();
	mode = SeeYouController::Idle;

	//TODO: why is this so inaccurate? What is the timing problem?
}

void SeeYouController::waitForDistance() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((distanceToMove > 0 && distanceMoved < distanceToMove) || (distanceToMove < 0 && distanceMoved > distanceToMove)) {
		SleeperThread::msleep(create->intSetting("SEEYOUCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}
}


void SeeYouController::turn(int angle, int speed) {

	if(mode == SeeYouController::EmergencyStop) return;

	// Init
	this->speed = speed;
	angleToTurn = angle;
	angleTurned = 0;
	distanceToMove = 0;
	distanceMoved = 0;

	// Move, wait for distance, stop
	mode = SeeYouController::Turn;
	waitForAngle();
	mode = SeeYouController::Idle;
}

void SeeYouController::wheelDrive(short Lwheel, short Rwheel) {

	if(mode == SeeYouController::EmergencyStop) return;

	this->mode = SeeYouController::WheelDrive;
	this->Lwheel = Lwheel;
	this->Rwheel = Rwheel;

}

void SeeYouController::waitForAngle() {

	// Differentiate between negative and positive wanted values and check if the current value has not yet crossed the wanted value...
	while ((angleToTurn > 0 && angleTurned < angleToTurn) || (angleToTurn < 0 && angleTurned > angleToTurn)) {
		SleeperThread::msleep(create->intSetting("SEEYOUCONTROLLER_WAIT_FOR_EVENT_INTERVAL_DELAY"));
	}

}

void SeeYouController::coil_directDrive(short Lwheel, short Rwheel) {
	create->coil->directDrive(Lwheel, Rwheel);
}

double SeeYouController::coil_getDistance() {
	return (double) create->coil->getDistance();
}

double SeeYouController::coil_getAngle() {
	return (double) create->coil->getAngle();
}

int SeeYouController::coil_getAnalogSensorDistance() {
	return create->coil->getAnalogSensorDistance();
	return 0;
}

int SeeYouController::coil_getBumpsAndWheelDrops() {
	return create->coil->getBumpsAndWheelDrops();
}

// TODO: Understand this part - Arduino Data
int SeeYouController::arduino_getHeading()
{
	return (int) create->arduino->readCompass();
}

int SeeYouController::arduino_getLeftPinger()
{
	return (int) create->arduino->readLeftPinger();
}

int SeeYouController::arduino_getRightPinger()
{
	return (int) create->arduino->readRightPinger();
}

int SeeYouController::arduino_getFrontIR()
{
	return (int) create->arduino->readInfraredFront();
}

int SeeYouController::arduino_getLeftIR()
{
	return (int) create->arduino->readInfraredLeft();
}

int SeeYouController::arduino_getRightIR()
{
	return (int) create->arduino->readInfraredRight();
}
int SeeYouController::arduino_getRFID()
{
	return (int) create->arduino->readRFID();
}


