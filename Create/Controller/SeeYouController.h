/*
 * SeeYouController.h
 *
 *  Created on: Mar 14, 2011
 *      Author: abraham
 */

#ifndef SEEYOUCONTROLLER_H_
#define SEEYOUCONTROLLER_H_

#include "Controller.h"
#include <vector>


class SeeYouController: public Controller {

	Q_OBJECT

public:
    typedef struct
    {
    	int id;
        double x;
        double y;
    } RFID_DB;
    //RFID_DB *predefinedDB;

    vector <RFID_DB> predefinedDB;

    int test;
	int tag;
	int tag_buffer;
	double obuffer;
	short speed	;
	double yokeX;
	double yokeY;
	short Lwheel;
	short Rwheel;
	double angleToTurn;			// Note: this is double for extra precision during emulation calculation...
	double angleTurned;			// Note: this is double for extra precision during emulation calculation...
	double distanceToMove;		// Note: this is double for extra precision during emulation calculation...
	double distanceMoved;		// Note: this is double for extra precision during emulation calculation...
	enum { Idle, Move, Turn, Joystick, WheelDrive, EmergencyStop, Avoidance} mode;

public:
	SeeYouController(Create *create, int speed, int interval);
	virtual ~SeeYouController();
	virtual void run();
	virtual void emergencyStop();
	void turn(int angle, int speed);
	void move(int distance, int speed);
	void wheelDrive(short Lwheel, short Rwheel);

protected:
	void waitForDistance();
	void waitForAngle();

public slots:
	void setYoke(double yokeX, double yokeY);
	void setSpeed(int value) { this->speed = value; };
	void setAngle(int value) { this->angleToTurn = value; };

private:
	virtual void coil_directDrive(short Lwheel, short Rwheel);
	virtual double coil_getDistance(); // Note: this is double for extra precision during emulation calculation...
	virtual double coil_getAngle(); // Note: this is double for extra precision during emulation calculation...
	virtual int coil_getAnalogSensorDistance();
	virtual int coil_getBumpsAndWheelDrops();

	virtual int arduino_getHeading();
	virtual int arduino_getLeftPinger();
	virtual int arduino_getRightPinger();
	virtual int arduino_getFrontIR();
	virtual int arduino_getLeftIR();
	virtual int arduino_getRightIR();
	virtual int arduino_getRFID();

};

#endif /* SEEYOUCONTROLLER_H_ */
