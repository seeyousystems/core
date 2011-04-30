/*
 * RFIDDBase.cpp
 *
 *  Created on: Apr 28, 2011
 *      Author: seeyou
 */

#include "RFIDDBase.h"

RFIDDBase::RFIDDBase() {
	// TODO Auto-generated constructor stub
	createDB();
}

RFIDDBase::~RFIDDBase() {
	// TODO Auto-generated destructor stub
	if(!predefinedDB.empty())
		predefinedDB.clear();
}

void RFIDDBase::createDB()
{
	RFID_DB db;

	vector<RFID_DB> temp(100, db);
	predefinedDB = temp;

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

	//db.id = 16;    GONE FOREVER!!!  :( no bueno
	//db.x = -1088;
	//db.y = -722;
	//predefinedDB[16] = db;

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

	//db.id = 21;  lost forever :(
	//db.x = 122;
	//db.y = 0;
	//predefinedDB[21] = db;

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

	//db.id = 25;  gone forever :(
	//db.x = 610;
	//db.y = 0;
	//predefinedDB[25] = db;

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

	//db.id = 45;  GONE FOREVER! :(
	//db.x = 2562;
	//db.y = 244;
	//predefinedDB[45] = db;

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

	db.id = 80;
	db.x = 1098;
	db.y = 0;
	predefinedDB[80] = db;

	//db.id = 81;
	//db.x = 0;
	//db.y = 0;
	//predefinedDB[81] = db;

	db.id = 82;
	db.x = 122;
	db.y = 0;
	predefinedDB[82] = db;

	db.id = 83;
	db.x = 610;
	db.y = 0;
	predefinedDB[83] = db;

	db.id = 84;
	db.x = 2562;
	db.y = 244;
	predefinedDB[84] = db;

	db.id = 85;
	db.x = -1088;
	db.y = -722;
	predefinedDB[85] = db;

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


}

int RFIDDBase::getDBX(int tag)
{
	if (predefinedDB[tag].x == 0)
	{
		return predefinedDB[tag].x * 10;
	}
	else
	{
		return predefinedDB[tag].x * 10 + 330;
	}
}

int RFIDDBase::getDBY(int tag)
{
	if (predefinedDB[tag].y == 0)
	{
		return predefinedDB[tag].y * 10;
	}
	else
	{
		return predefinedDB[tag].y * 10 + 330;
	}
}
