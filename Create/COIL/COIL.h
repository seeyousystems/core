/*
 *  COIL.h
 *
 *  COIL - Create Open Interface Library. Provides an abstraction layer for the
 *  iRobot Create Open Interface, based largely upon the work of Jesse DeGuire and Nathan Sprague.
 *  The original COIL has been ported to C++ and Qt using QExtSerialPort ports,
 *  thus this class cross-compiles on Windows/OSX/Linux :).
 *
 *  COIL includes functions for opening a serial port, sending navigational
 *  commands to the iRobot Create, and reading sensors.
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Jesse DeGuire and Nathan Sprague
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

#ifndef COIL_H
#define COIL_H

#include <QObject>
#include <QString>
#include <QThread>

#include "qextserialport/qextserialport.h"

/// 8-bit unsigned value.  Called "byte" to keep continuity with iRobot naming
/// (and so I don't have to write "unsigned char" all the time).
typedef unsigned char	byte;

class COIL : public QObject
{
    Q_OBJECT


public:
    COIL(QString portName);
    ~COIL();

private:
	QextSerialPort *port;
	static int debug;			//< debug mode status

public:
	int cwrite (QextSerialPort *port, byte* buf, int numbytes);
	int cread (QextSerialPort *port, byte* buf, int numbytes);
	//static int stopWait();

public:

    /** \brief Command Opcodes
     *
     *  Defines constants for all of the available opcodes for the Create OI.
     */
    typedef enum
    {
    	OPCODE_START				= 128,
    	OPCODE_BAUD,
    	OPCODE_SAFE					= 131,
    	OPCODE_FULL,
    	OPCODE_SPOT					= 134,
    	OPCODE_COVER,
    	OPCODE_DEMO,
    	OPCODE_DRIVE,
    	OPCODE_LOW_SIDE_DRIVERS,
    	OPCODE_LED,
    	OPCODE_SONG,
    	OPCODE_PLAY_SONG,
    	OPCODE_SENSORS,
    	OPCODE_COVER_AND_DOCK,
    	OPCODE_PWM_LOW_SIDE_DRIVERS,
    	OPCODE_DRIVE_DIRECT,
    	OPCODE_DIGITAL_OUTS			= 147,
    	OPCODE_STREAM,
    	OPCODE_QUERY_LIST,
    	OPCODE_PAUSE_RESUME_STREAM,
    	OPCODE_SEND_IR,
    	OPCODE_SCRIPT,
    	OPCODE_PLAY_SCRIPT,
    	OPCODE_SHOW_SCRIPT,
    	OPCODE_WAIT_TIME,
    	OPCODE_WAIT_DISTANCE,
    	OPCODE_WAIT_ANGLE,
    	OPCODE_WAIT_EVENT
    } oi_command;

    /** \brief Sensor packet constants
     *
     *  Contains constants for each of the 42 sensor packets available in the Create.  Note
     *  that not all packets are the same size.  Refer to the Create Open Interface documentation
     *  for more info.
     */
    typedef enum
    {
    	SENSOR_GROUP_0			 	= 0,
    	SENSOR_GROUP_1,
    	SENSOR_GROUP_2,
    	SENSOR_GROUP_3,
    	SENSOR_GROUP_4,
    	SENSOR_GROUP_5,
    	SENSOR_GROUP_ALL,
    	SENSOR_BUMPS_AND_WHEEL_DROPS,
    	SENSOR_WALL,
    	SENSOR_CLIFF_LEFT,
    	SENSOR_CLIFF_FRONT_LEFT,
    	SENSOR_CLIFF_FRONT_RIGHT,
    	SENSOR_CLIFF_RIGHT,
    	SENSOR_VIRTUAL_WALL,
    	SENSOR_OVERCURRENT,
    	SENSOR_INFRARED			= 17,
    	SENSOR_BUTTONS,
    	SENSOR_DISTANCE,
    	SENSOR_ANGLE,
    	SENSOR_CHARGING_STATE,
    	SENSOR_VOLTAGE,
    	SENSOR_CURRENT,
    	SENSOR_BATTERY_TEMP,
    	SENSOR_BATTERY_CHARGE,
    	SENSOR_BATTERY_CAPACITY,
    	SENSOR_WALL_SIGNAL,
    	SENSOR_CLIFF_LEFT_SIGNAL,
    	SENSOR_CLIFF_FRONT_LEFT_SIGNAL,
    	SENSOR_CLIFF_FRONT_RIGHT_SIGNAL,
    	SENSOR_CLIFF_RIGHT_SIGNAL,
    	SENSOR_DIGITAL_INPUTS,
    	SENSOR_ANALOG_SIGNAL,
    	SENSOR_CHARGING_SOURCES_AVAILABLE,
    	SENSOR_OI_MODE,
    	SENSOR_SONG_NUMBER,
    	SENSOR_SONG_IS_PLAYING,
    	SENSOR_NUM_STREAM_PACKETS,
    	SENSOR_REQUESTED_VELOCITY,
    	SENSOR_REQUESTED_RADIUS,
    	SENSOR_REQUESTED_RIGHT_VEL,
    	SENSOR_REQUESTED_LEFT_VEL
    } oi_sensor;

    /** \brief  Baud Rate Codes
     *
     *  Codes to set the baud rate, in bits per second, at which data is sent over the serial
     *  port.  Use with the Baud command.
     */

    /*typedef enum
    {
    	BAUD300,
    	BAUD600,
    	BAUD1200,
    	BAUD2400,
    	BAUD4800,
    	BAUD9600,
    	BAUD14400,
    	BAUD19200,
    	BAUD28800,
    	BAUD38400,
    	BAUD57600,
    	BAUD115200
    } oi_baud;
*/
     /** \brief Demo codes
     *
     *  Codes for the built-in demos.  Use these with the Demo command.
      */
    typedef enum
    {
    	DEMO_COVER,
    	DEMO_COVER_AND_DOCK,
    	DEMO_SPOT_COVER,
    	DEMO_MOUSE,
    	DEMO_FIGURE_EIGHT,
    	DEMO_WIMP,
    	DEMO_HOME,
    	DEMO_TAG,
    	DEMO_PACHELBEL,
    	DEMO_BANJO,
    	DEMO_ABORT		= 255
    } oi_demo;

    /** \brief LED switches
     *
     *  Used for turning off and on the LEDs on top of the Create.  Use with the LEDs command.
     */
    typedef enum
    {
    	LED_ADVANCE 	= 8,
     	LED_PLAY		= 2
    } oi_led;

    /** \brief Output switches
     *
     *  Used for setting the the state of the digital and lowside outputs on the Cargo Bay connector.
     */
    typedef enum
    {
    	OUTPUT_0	= 1,
     	OUTPUT_1	= 2,
     	OUTPUT_2	= 4
    } oi_output;


    /** \brief OI Mode Values
	 *
	 *
	 */
	typedef enum
	{
		OI_MODE_OFF		= 0,
		OI_MODE_PASSIVE	= 1,
		OI_MODE_SAFE	= 2,
		OI_MODE_FULL	= 3
	} oi_mode;


    /** \brief Output switches
	 *
	 *  Used for setting the the state of the digital and lowside outputs on the Cargo Bay connector.
	 */
	typedef enum
	{
		BUMPWHEELDROP_BUMP_RIGHT		= 1,
		BUMPWHEELDROP_BUMP_LEFT			= 2,
		BUMPWHEELDROP_WHEELDROP_RIGHT	= 4,
		BUMPWHEELDROP_WHEELDROP_LEFT	= 8,
		BUMPWHEELDROP_WHEELDROP_CASTER	= 16
	} oi_bumpwheeldrop;


public:
    int startOI ();
    int setBaud (BaudRateType rate);
    int enterPassiveMode ();
    int enterSafeMode ();
    int enterFullMode ();
    int runDemo (oi_demo demo);
    int runCoverDemo ();
    int runCoverAndDockDemo ();
    int runSpotDemo ();
    int drive (short vel, short rad);
    int directDrive (short Lwheel, short Rwheel);
    int driveDistance (short vel, short rad, int dist, int interrupt);
    int turn (short vel, short rad, int angle, int interrupt);
    int setLEDState (short lflags, byte pColor, byte pInten);
    int setDigitalOuts (oi_output oflags);
    int setPWMLowSideDrivers (byte pwm0, byte pwm1, byte pwm2);
    int setLowSideDrivers (oi_output oflags);
    int sendIRbyte (byte irbyte);
    int writeSong (byte number, byte length, byte* song);
    int playSong (byte number);
    int readRawSensor (oi_sensor packet, byte* buffer, int size);
    int readSensor (oi_sensor packet);
    int getDistance ();
    int getAngle ();
    int getVelocity ();
    int getTurningRadius ();
    int getBumpsAndWheelDrops ();
    int getCliffs ();
    int getAnalogSensorDistance ();
    int* getAllSensors ();
    int readRawSensorList (oi_sensor* packet_list, byte num_packets, byte* buffer, int size);
    int writeScript (byte* script, byte size);
    int playScript ();
    byte* getScript ();
    void waitTime (unsigned long msecs);
    int stopWait ();
    int waitDistance (int dist, int interrupt);
    int waitAngle (int angle, int interrupt);
    int stopOI ();
    void enableDebug ();
    void disableDebug ();
    int extractSensorFromData(int* data, oi_sensor packet);

};



#endif // COIL_H
