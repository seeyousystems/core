/*
 *  COIL.cpp
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

#include "COIL.h"

#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)


/**
 * 	Constructs a new COIL object.
 *
 *  \param portName		The name of the port to which COIL will communicate with the iRobot,
 *  					such as 'COM4'.
 */
COIL::COIL(QString portName, Create *create, QString name) : CoreObject(name, create)
{
	port = new QextSerialPort(portName);
	port->setBaudRate(BAUD57600);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);
}

COIL::COIL(Create *create, QString name) : CoreObject(name, create)
{
	this->port = NULL;
}

COIL::~COIL()
{
	if(port) delete port;
}


/** \brief Starts the OI.
 *
 * 	This command opens the serial connection and starts the Open Interface.  The Create
 *	will be started in Passive Mode. You must send this command first before any others.
 *
 * 	\param serial	The location of the serial port device file
 *
 *  \return		0 if successful or -1 otherwise
 */
int COIL::startOI ()
{
	if(!port->isOpen()) {
		if(!port->open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
			Debug::warning("[COIL] could not open serial port");
			return -1;
		}
	}

	byte cmd[1];
	cmd[0] = OPCODE_START;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error("[COIL] could not start OI: unable to send command");
		return -1;
	}

	if (readSensor(SENSOR_OI_MODE) != OI_MODE_PASSIVE) {
		Debug::error("[COIL] could not start OI: unable to enter passive mode");
		return -1;
	}

	return 0;
}

/** \brief	Sets the baud rate for serial port transfer
 *
 * 	This command sets the baud rate between the Create and computer.  This persists
 *  until either this command is called again or until the Create loses power.  This
 *	command will wait for 100ms after setting the baud rate to prevent data loss (this
 *  is in compliance with the OI specification).  The default baud rate is 56kbps.
 *  <b>Note:  At a baud rate of 115200, there must be a 20us gap between each byte or
 *  else data loss will occur.</b>
 *
 * 	\param	rate	New baud rate value
 *
 *  \return			0 if successful or -1 otherwise
 */
int COIL::setBaud (BaudRateType rate)
{
	//struct termios options;
	byte cmd[2];
	int new_baud;

	//used to set baud for PC same as baud for Create
	switch (rate)
	{
		case BAUD300: 	 	new_baud = 300; 	break;
		case BAUD600:		new_baud = 600;		break;
		case BAUD1200:		new_baud = 1200;	break;
		case BAUD2400:		new_baud = 2400;	break;
		case BAUD4800:		new_baud = 4800;	break;
		case BAUD9600:		new_baud = 9600;	break;
		case BAUD14400:		new_baud = 14400;	break;
		case BAUD19200:		new_baud = 19200;	break;
		case BAUD38400:		new_baud = 38400;	break;
		case BAUD57600:		new_baud = 56700;	break;
		case BAUD115200:	new_baud = 115200;	break;
		default:
			Debug::error ( "[COIL] Could not set baud: Invalid argument");
			return -1;
	}

	cmd[0] = OPCODE_BAUD;	cmd[1] = rate;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not set baud");
		return -1;
	}

	port->setBaudRate(rate);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);

	SleeperThread::msleep(100);
	return 0;
}

/** \brief Put Create into Passive Mode
 *
 * 	This puts the Create into Passive Mode.  This mode allows reading sensor data, running demos,
 * 	and changing mode.  All other commands are ignored.
 *
 * 	\return 0 if successful or -1 otherwise
 */
int COIL::enterPassiveMode()
{
	byte cmd[1];
	cmd[0] = OPCODE_START;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not enter Passive Mode: unable to send command");
		return -1;
	}

	if (readSensor(SENSOR_OI_MODE) != OI_MODE_PASSIVE) {
		Debug::error("[COIL] could not enter Passive Mode: unable to enter mode");
		return -1;
	}

	return 0;
}


/** \brief Put Create into Safe Mode
 *
 * 	This puts the Create into Safe Mode, allowing control over the Create.  The Create
 *	will stop and revert to Passive Mode if any of the cliff or wheel drop sensors are
 *  activated or if the charger is plugged in and powered.
 *
 *	\return		0 if successful or -1 otherwise
 */
int COIL::enterSafeMode ()
{
	byte cmd[1];
	cmd[0] = OPCODE_SAFE;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not enter Safe Mode: unable to send command");
		return -1;
	}


	if (readSensor(SENSOR_OI_MODE) != OI_MODE_SAFE) {
		Debug::error("[COIL] could not enter Passive Mode: unable to enter mode");
		return -1;
	}

	return 0;
}

/** \brief Put Create into Full Mode
 *
 * 	The puts the Create into Full Mode, allowing complete control over the Create.  The
 *  Create will turn off all safety features in this mode, giving you control over the
 *  Create's actions should any of the wheel drop or cliff sensor activate.  Be warned,
 *  however, that this also means that nothing will stop the Create from driving off a
 *  cliff.
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::enterFullMode ()
{
	byte cmd[1];
	cmd[0] = OPCODE_FULL;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not enter Full Mode: unable to send command");
		return -1;
	}

	if (readSensor(SENSOR_OI_MODE) != OI_MODE_FULL) {
		Debug::error("[COIL] could not enter Full Mode: unable to enter mode");
		return -1;
	}

	return 0;
}

/** \brief Run a built-in demo
 *
 * 	This starts one of the Create's built-in demo routines.  Information about the demos
 *  is available in the iRobot Create instruction manual.  This will put the Create into
 * 	Passive Mode.
 *
 * 	\param	demo	The value of the demo to run (or 255 to stop the current demo).
 *
 *	\return		 	0 if successful or -1 otherwise
 */
int COIL::runDemo (oi_demo demo)
{
	byte cmd[2];
	cmd[0] = OPCODE_DEMO;	cmd[1] = demo;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not run demo");
	}
	return 0;
}

/** \brief Run the Cover demo
 *
 *	\return 	0 if successful or -1 otherwise
 */
int COIL::runCoverDemo ()
{
	byte cmd[1];
	cmd[0] = OPCODE_COVER;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not start Cover demo");
	}
	return 0;
}

/** \brief Run the Cover and Dock demo
 *
 *	\return 	0 if successful or -1 otherwise
 */
int COIL::runCoverAndDockDemo ()
{
	byte cmd[1];
	cmd[0] = OPCODE_COVER_AND_DOCK;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not start Cover and Dock demo");
		return -1;
	}
	return 0;
}

/** \brief Run the Spot demo
 *
 *	\return 	0 if successful or -1 otherwise
 */
int COIL::runSpotDemo ()
{
	byte cmd[1];
	cmd[0] = OPCODE_SPOT;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not start Spot demo");
		return -1;
	}
	return 0;
}

/** \brief	Drive the robot with a given velocity and turning radius
 *
 * 	Drives the Create with the given velocity (mm/s) and turning radius (mm).  The velocity
 *  ranges from -500 to 500mm/s, with negative velocities making the Create drive backward.
 * 	The radius ranges from -2000 to 2000mm, with positive radii turning the Create left and
 *  negative radii turning it right.
 *
 * 	A radius of -1 makes the Create turn in place clockwise and 1 makes it turn in place
 *	counter-clockwise.  Also, a radius of 0 will make the Create drive straight.
 *
 * 	\param	vel		The velocity, in mm/s, of the robot
 * 	\param	rad		The turning radius, in mm, from the center of the turning circle to the center
 * 					of the Create.
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::drive (short vel, short rad)
{
	byte cmd[5];

	//keep args within Create limits
	vel = MIN(500, vel);
	vel = MAX(-500, vel);
	rad = MIN(2000, rad);
	rad = MAX(-2000, rad);

	if (0 == rad) 			//special case for drive straight (from manual)
		rad = 32768;

	cmd[0] = OPCODE_DRIVE;
	cmd[1] = (vel >> 8) & 0x00FF;
	cmd[2] = vel & 0x00FF;
	cmd[3] = (rad >> 8) & 0x00FF;
	cmd[4] = rad & 0x00FF;

	if (cwrite (port, cmd, 5) < 0)
	{
		Debug::error ("[COIL] could not start drive");
		return -1;
	}
	return 0;
}

/** \brief	Control the Create's wheels directly
 *
 * 	Allows you to control the velocity of each wheel independently, ranging from -500 to
 * 	500mm/s.  A positive velocity makes the wheel drive forward and a negative velocity
 * 	makes it drive backward.
 *
 * 	\param	Lwheel	The velocity of the left wheel
 * 	\param	Rwheel	The velocity of the right wheel
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::directDrive (short Lwheel, short Rwheel)
{
	byte cmd[5];

	//keep args within Create limits
	Lwheel = MIN(500, Lwheel);
	Lwheel = MAX(-500, Lwheel);
	Rwheel = MIN(500, Rwheel);
	Rwheel = MAX(-500, Rwheel);

	cmd[0] = OPCODE_DRIVE_DIRECT;
	cmd[1] = (Rwheel >> 8) & 0x00FF;
	cmd[2] = Rwheel & 0x00FF;
	cmd[3] = (Lwheel >> 8) & 0x00FF;
	cmd[4] = Lwheel & 0x00FF;

	if (cwrite (port, cmd, 5) < 0)
	{
		Debug::error ("[COIL] could not start direct drive");
		return -1;
	}
	return 0;
}

/** \brief	Drive for the specified distance
 *
 *  Moves the Create at the specified velocity and turning radius until the specified
 *  distance is reached, at which point it will stop.  Velocity ranges from -500mm/s to
 *  500mm/s (negative values move backward) and turning radius ranges from -2000mm to
 *  2000mm (negative values turn right; positive values turn left).
 *
 *	A radius of -1 will spin the Create in place clockwise and a radius of 1 will spin
 *	it counter-clockwise.  A radius of 0 will drive straight.
 *
 *	The distance sensor on the Create will be reset to 0 after using this function.
 *
 *	\param	vel		desired velocity in mm/s
 *	\param	rad		desired turning radius in mm
 *	\param	dist	distance the Create should travel before stopping in mm
 * 	\param	interrupt	0 if the movement should be non-interruptible: ignore collisions
 *                              1 if the movement should be interruptible: terminate on collision
 *
 *	\return		Distance travelled or INT_MIN on error
 */
int COIL::driveDistance (short vel, short rad, int dist, int interrupt)
{
	int ret = 0;

	if (drive (vel, rad) == -1 || (ret = waitDistance (dist, interrupt)) == INT_MIN || drive (0, 0) == -1)
	{
		return INT_MIN;
	}

	return ret;
}

/** \brief	Turn for the specified angle
 *
 *  Moves the Create at the specified velocity and turning radius until the specified
 *  angle is reached, at which point it will stop.  Velocity ranges from -500mm/s to
 *  500mm/s (negative values move backward) and turning radius ranges from -2000mm to
 *  2000mm (negative values turn right; positive values turn left).
 *
 *	A radius of -1 will spin the Create in place clockwise and a radius of 1 will spin
 *	it counter-clockwise.  A radius of 0 will drive straight.
 *
 *	The angle sensor on the Create will be reset to 0 after using this function.
 *
 *	\param	vel		desired velocity in mm/s
 *	\param	rad		desired turning radius in mm
 *	\param	angle	distance the Create should travel before stopping in mm
 * 	\param	interrupt	0 if the movement should be non-interruptible: ignore collisions
 *                              1 if the movement should be interruptible: terminate on collision
 *
 *	\return		Angle turned or INT_MIN on error
 */
int COIL::turn (short vel, short rad, int angle, int interrupt)
{
	int ret = 0;

	if (drive (vel, rad) == -1 || (ret = waitAngle (angle, interrupt)) == INT_MIN || drive (0, 0) == -1)
	{
		return INT_MIN;
	}
	return ret;
}


/** \brief	Controls the state of the LEDs on the Create
 *
 * 	Allows you the control the state of the three LEDs on the top of the Create.  The Play
 * 	and Advance LEDs can be on or off and are controlled by setting bit flags.  The Power
 * 	LED is set with two bytes:  one for the color and the other for the light's intensity.
 * 	The color ranges from green (0) to red (255).
 *
 * 	\param	lflags	LED flags for setting the Play and Advance LEDs
 * 	\param	pColor	Sets color for the Power LED
 * 	\param	pInten	Intensity of the Power LED
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::setLEDState (short lflags, byte pColor, byte pInten)
{
	byte cmd[4];
	cmd[0] = OPCODE_LED;
	cmd[1] = lflags;
	cmd[2] = pColor;
	cmd[3] = pInten;

	if (cwrite (port, cmd, 4) < 0)
	{
		Debug::error ("[COIL] could not set LEDs");
		return -1;
	}
	return 0;
}

/** \brief	Sets state of digital ouputs
 *
 * 	Controls the state of the 3 digital outputs in the Cargo Bay Connector.  Each output
 * 	is able to provide up to 20mA of current.  The state is set using bit flags, with 1
 * 	being high (5V) and 0 being low (0V).
 *
 *	\param	oflags	Output flags for setting the state of the 3 outputs
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::setDigitalOuts (oi_output oflags)
{
	byte cmd[2];
	cmd[0] = OPCODE_DIGITAL_OUTS;
	cmd[1] = oflags;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not set digital outs");
		return -1;
	}
	return 0;
}

/** \brief	Control low side drivers with variable power
 *
 * 	Specifies the PWM (pulse width modulation) duty cycle for each of the three low side
 *  drivers in the system, with a maximum value of 128. For example, sending a value of 64
 * 	would control a driver with 50% of battery voltage since 128/64 = .5.
 *
 *	\param	pwm0	duty cycle for driver 0
 * 	\param	pwm1	duty cycle for driver 1
 * 	\param	pwm2	duty cycle for driver 2
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::setPWMLowSideDrivers (byte pwm0, byte pwm1, byte pwm2)
{
	byte cmd[4];

	//max value is 128
	pwm0 = MIN(128, pwm0);
	pwm1 = MIN(128, pwm1);
	pwm2 = MIN(128, pwm2);

	cmd[0] = OPCODE_PWM_LOW_SIDE_DRIVERS;
	cmd[1] = pwm2;
	cmd[2] = pwm1;
	cmd[3] = pwm0;

	if (cwrite (port, cmd, 4) < 0)
	{
		Debug::error ("[COIL] could not set low side driver duty cycle");
		return -1;
	}
	return 0;
}

/**	\brief	Control state of low side drivers
 *
 * 	Controls the state of each of the low side drivers using bit flags.  This command
 * 	turns the drivers on at 100% PWM duty cycle.  Low side drivers 0 and 1 can provide
 * 	up to 0.5A of current while driver 2 can provide up to 1.5A.  If too much current is
 * 	drawn, the current will be limited and the overcurrent flag in sensor packet 14
 * 	(overcurrent packet) is set.
 *
 * 	\param	oflags		Output flags for setting the state of the 3 drivers
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::setLowSideDrivers (oi_output oflags)
{
	byte cmd[2];
	cmd[0] = OPCODE_LOW_SIDE_DRIVERS;
	cmd[1] = oflags;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not set low side driver state");
		return -1;
	}
	return 0;
}

/** \brief	Sends IR signal
 *
 * 	Sends the given IR byte out of low side driver 1, using the format expected by the Create's IR
 * 	receiver.  The Create documentation suggests using a 100ohm resistor in parallel with the IR
 * 	receiver and its resistor.
 *
 * 	\param	irbyte	The byte value to send
 *
 * 	\return 	0 if successfull or -1 otherwise
 */
int COIL::sendIRbyte (byte irbyte)
{
	byte cmd[2];
	cmd[0] = OPCODE_SEND_IR;
	cmd[1] = irbyte;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not write to IR");
		return -1;
	}
	return 0;
}

/** \brief	Create and store a small song
 *
 * 	Writes and stores a small song into the Create's internal memory.  You can store up to 16 songs,
 *	each with up to 16 notes each.  Each note is associated with a sound from the robot's internal
 * 	MIDI sequencer, with 31 being the lowest note and 127 being the highest.  Anything outside that
 * 	range is considered to be silence.  See the iRobot OI Specification for more info.
 *
 * 	The first argument is the song number (0-15).  The second argument is the number of notes in
 * 	the song (1-16).  The third argument is an array of bytes.  The even entries (0, 2, 4, etc.)
 * 	are the notes to play and the odd entries (1, 3, 5. etc.) are the durations of those notes, in
 *	increments of 1/64th of a second (so a value of 32 would play the note for half a second).  The
 * 	size of this array should be equal to twice the second argument.
 *
 * 	\param	number	The song number being stored (0-15)
 * 	\param	length	The number of notes in the song (1-16)
 * 	\param	song	An array containing notes and durations of those notes.  Size = 2 * length
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::writeSong (byte number, byte length, byte* song)
{
	byte cmd[3 + 2*length];
	int i;

	cmd[0] = OPCODE_SONG;
	cmd[1] = number;
	cmd[2] = length;

	for (i = 0; i < 2*length; i++)
		cmd[i + 3] = song[i];

	if (cwrite (port, cmd, 3+2*length) < 0)
	{
		Debug::error ("[COIL] could not write new song");
		return -1;
	}
	return 0;
}

/**	\brief	Play a stored song
 *
 * 	Retrieves a stored song from memory and plays it.  This command will not do anything if a song
 * 	is already playing.  Check the "song playing" sensor packet to check if the Create is ready to
 * 	play a song.  This command will return 0 as long as the command was able to be sent to the
 * 	Create over the serial port, regardless of whether or not the Create is ready to play the song.
 *
 * 	\param	number	The song number to play (0-15)
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::playSong (byte number)
{
	byte cmd[2];
	cmd[0] = OPCODE_PLAY_SONG;
	cmd[1] = number;

	if (cwrite (port, cmd, 2) < 0)
	{
		Debug::error ("[COIL] could not play song");
		return -1;
	}
	return 0;
}

/** \brief	Get raw data from sensor
 *
 * 	Requests the OI to return a packet of sensor data.  There are 42 different packets, each of
 * 	which returns a value of a specific sensor or group of sensors.  Different sensors have
 * 	different packet sizes, so it is up to you to make sure you are requesting the correct size
 * 	and that your buffer is large enough to accept the packets.
 *
 * 	\param[in]	packet	Sensor packet to read
 * 	\param[out]	buffer	Buffer to read raw packets into
 * 	\param[in]	size	Number of bytes to read into buffer
 *
 * 	\return		number of bytes read or -1 on failure
 */
int COIL::readRawSensor (oi_sensor packet, byte* buffer, int size)
{
	int numread = 0;
	byte cmd[2];
	cmd[0] = OPCODE_SENSORS;
	cmd[1] = packet;

	if (cwrite (port, cmd, 2) < 0)
	{
		emit incompleteSensorDataRead();
		Debug::error ("[COIL] could not request sensor");
		return -1;
	}

	SleeperThread::msleep(40); // BUG: Why must we sleep? RTFM...

	numread = cread (port, buffer, size);
	if (numread < 0)
	{
		emit incompleteSensorDataRead();
		Debug::error ("[COIL] could not read sensor");
		return -1;
	}

	return numread;
}

/** \brief  Read formatted data from single sensor
 *
 *  Returns data from the specified sensor in a user-readable format.  This function will not
 *  work for sensor groups, only single sensors.  Parameter can be one of oi_sensor values.
 *
 * \param	sensor	The sensor to get data from
 *
 * \return		Value read from specified sensor or INT_MIN on error
 */
int COIL::readSensor (oi_sensor packet)
{
	int result = 0;
	byte* buffer;

	switch (packet)
	{
		//all one-byte unsigned sensors
		case SENSOR_BUMPS_AND_WHEEL_DROPS:
		case SENSOR_WALL:
		case SENSOR_CLIFF_LEFT:
		case SENSOR_CLIFF_FRONT_LEFT:
		case SENSOR_CLIFF_FRONT_RIGHT:
		case SENSOR_CLIFF_RIGHT:
		case SENSOR_VIRTUAL_WALL:
		case SENSOR_OVERCURRENT:
		case SENSOR_INFRARED:
		case SENSOR_BUTTONS:
		case SENSOR_CHARGING_STATE:
		case SENSOR_DIGITAL_INPUTS:
		case SENSOR_CHARGING_SOURCES_AVAILABLE:
		case SENSOR_OI_MODE:
		case SENSOR_SONG_NUMBER:
		case SENSOR_SONG_IS_PLAYING:
		case SENSOR_NUM_STREAM_PACKETS:
		        buffer = (byte*) malloc (sizeof(byte));
			if (NULL == buffer)
				return INT_MIN;
			*buffer = 0;
			if (-1 == readRawSensor (packet, buffer, 1))
			{
				free (buffer);
				return INT_MIN;
			}
			result = *buffer;
			break;

		//one-byte signed sensor
		case SENSOR_BATTERY_TEMP:
			buffer = (byte*) malloc (sizeof(char));
			if (NULL == buffer)
				return INT_MIN;
			*buffer = 0;
			if (-1 == readRawSensor (packet, buffer, 1))
			{
				free (buffer);
				return INT_MIN;
			}
			result += (char) *buffer;
			break;

		//two-byte unsigned sensors
		case SENSOR_VOLTAGE:
		case SENSOR_BATTERY_CHARGE:
		case SENSOR_BATTERY_CAPACITY:
		case SENSOR_WALL_SIGNAL:
		case SENSOR_CLIFF_LEFT_SIGNAL:
		case SENSOR_CLIFF_FRONT_LEFT_SIGNAL:
		case SENSOR_CLIFF_FRONT_RIGHT_SIGNAL:
		case SENSOR_CLIFF_RIGHT_SIGNAL:
		case SENSOR_ANALOG_SIGNAL:
			buffer = (byte*) malloc (2 * sizeof(byte));
			if (NULL == buffer)
				return INT_MIN;
			buffer[0] = 0; buffer[1] = 0;
			if (-1 == readRawSensor (packet, buffer, 2))
			{
				free (buffer);
				return INT_MIN;
			}
			result = buffer[1] | (buffer[0] << 8);
			break;

		//two-byte signed sensors
		case SENSOR_DISTANCE:
		case SENSOR_ANGLE:
		case SENSOR_CURRENT:
		case SENSOR_REQUESTED_VELOCITY:
		case SENSOR_REQUESTED_RADIUS:
		case SENSOR_REQUESTED_RIGHT_VEL:
		case SENSOR_REQUESTED_LEFT_VEL:
			buffer = (byte*) malloc (2 * sizeof(byte));
			if (NULL == buffer)
				return INT_MIN;
			buffer[0] = 0; buffer[1] = 0;
			if (-1 == readRawSensor (packet, buffer, 2))
			{
				free (buffer);
				return INT_MIN;
			}
			result += (short) (buffer[1] | (buffer[0] << 8));
			break;

		//any other input is invalid (including packet groups)
		default:
			return INT_MIN;
	}

	free (buffer);
	return result;
}

/** \brief  Get current distance
 *
 *	Returns the distance the Create has travelled since the last time this function was called or
 *  the last time the distance sensor was polled.
 *
 *	\return		Distance Create travelled since last reading or INT_MIN on error
 */
int COIL::getDistance ()
{
	return readSensor (SENSOR_DISTANCE);
}

/** \brief	Get current angle
 *
 *	Returns the angle the Create has turned since the last time this function was called or the
 *	last time the angle sensor was polled.
 *
 *	\return		Angle Create turned since last reading or INT_MIN on error
 */
int COIL::getAngle ()
{
	return readSensor (SENSOR_ANGLE);
}

/** \brief  Get current velocity
 *
 *	Returns the Create's current velocity.  Note that read velocity may be different from actual velocity
 *	due to sensor inaccuracy and wheel slippage.
 *
 *	\return		Create's currently requested velocity or INT_MIN on error
 */
int COIL::getVelocity ()
{
	return readSensor (SENSOR_REQUESTED_VELOCITY);
}

/** \brief	Get current turning radius
 *
 *	Returns the Create's current turning radius.  The read value may differ from the actual value slightly
 *	due to sensor inaccuracy and wheel slippage.
 *
 *	\return		Create's currently requested turning radius or INT_MIN on error.
 */
int COIL::getTurningRadius ()
{
	return readSensor (SENSOR_REQUESTED_RADIUS);
}

/**	\brief	Get bumper and wheel drop state
 *
 *	Returns the current state of the bumper and wheel drop sensors.  The state of each is sent as a single
 *	bit.  For example, a return value of 16 means that only the front caster's wheeldrop sensor is activated.
 *	A return value of 3 means that both bumper sensors are active.
 *
 *	\return		Current state of bumper and wheel drops or INT_MIN on error
 */
int COIL::getBumpsAndWheelDrops ()
{
	return readSensor (SENSOR_BUMPS_AND_WHEEL_DROPS);
}

/** \brief	Get state of cliff sensors
 *
 *	Returns the current states of the four cliff sensors under the Create's bumper.  Each sensor's state is
 *	returned as a single bit.  For example, a return value of 8 means the left cliff sensor is activated.
 *
 *	\return		Current state of cliff sensors or INT_MIN on error
 */
int COIL::getCliffs ()
{
	int cliffs[4];

	cliffs[0] = readSensor (SENSOR_CLIFF_LEFT);
	cliffs[1] = readSensor (SENSOR_CLIFF_FRONT_LEFT);
	cliffs[2] = readSensor (SENSOR_CLIFF_FRONT_RIGHT);
	cliffs[3] = readSensor (SENSOR_CLIFF_RIGHT);

	if (INT_MIN == cliffs[0]  || INT_MIN == cliffs[1] || INT_MIN == cliffs[2] || INT_MIN == cliffs[3])
	    return INT_MIN;

	return (cliffs[0]*8 + cliffs[1]*4 + cliffs[2]*2 + cliffs[3]);
}

/** \brief	Get analog sensor in millimeters
 *
 *	Returns the current analog sensor data interpreted as millimeters by using a standard
 *	conversion. This method works with standard sensors such as the Sharp IR Finder (GP2D12).
 *
 *	The value is determined using the following data:
 *
 *	Actual Distance (mm)	Analog Signal	Reverse Analog	Ratio
 *	75	542	458	0.163755459
 *	95	534	466	0.203862661
 *	115	466	534	0.215355805
 *	125	429	571	0.218914186
 *	135	396	604	0.223509934
 *	155	354	646	0.23993808
 *	175	320	680	0.257352941
 *	225	254	746	0.301608579
 *	275	212	788	0.348984772
 *	325	183	817	0.397796818
 *	375	162	838	0.447494033
 *	425	146	854	0.49765808
 *	475	126	874	0.543478261
 *	575	110	890	0.646067416
 *	675	98	902	0.748337029
 *	775	90	910	0.851648352
 *	875	82	918	0.953159041
 *	975	78	922	1.057483731
 *	107575	925	1.162162162
 *
 *	\return		Distance of sensed object in millimeters
 */
int COIL::getIRSensorDistanceFromAnalogSignal (int analogSignal)
{
	int reverseAnalog = 1000 - analogSignal;
	double ratio = 1.0;

	if		(reverseAnalog <= 458) 							ratio = 0.163755459;
	else if	(reverseAnalog > 458 && reverseAnalog <= 466)	ratio = 0.203862661;
	else if	(reverseAnalog > 466 && reverseAnalog < 534)	ratio = 0.215355805;
	else if	(reverseAnalog > 534 && reverseAnalog < 571)	ratio = 0.218914186;
	else if	(reverseAnalog > 571 && reverseAnalog < 604)	ratio = 0.223509934;
	else if	(reverseAnalog > 604 && reverseAnalog < 646)	ratio = 0.23993808;
	else if	(reverseAnalog > 646 && reverseAnalog < 680)	ratio = 0.257352941;
	else if	(reverseAnalog > 680 && reverseAnalog < 746)	ratio = 0.301608579;
	else if	(reverseAnalog > 746 && reverseAnalog < 788)	ratio = 0.348984772;
	else if	(reverseAnalog > 788 && reverseAnalog < 817)	ratio = 0.397796818;
	else if	(reverseAnalog > 817 && reverseAnalog < 838)	ratio = 0.447494033;
	else if	(reverseAnalog > 838 && reverseAnalog < 854)	ratio = 0.49765808;
	else if	(reverseAnalog > 854 && reverseAnalog < 874)	ratio = 0.543478261;
	else if	(reverseAnalog > 874 && reverseAnalog < 890)	ratio = 0.646067416;
	else if	(reverseAnalog > 890 && reverseAnalog < 902)	ratio = 0.748337029;
	else if	(reverseAnalog > 902 && reverseAnalog < 910)	ratio = 0.851648352;
	else if	(reverseAnalog > 910 && reverseAnalog < 918)	ratio = 0.953159041;
	else if	(reverseAnalog > 918 && reverseAnalog < 922)	ratio = 1.057483731;
	else if	(reverseAnalog >= 922)							ratio = 1.162162162;

	int distance = (int)(reverseAnalog * ratio); // millimeters
	return distance;
}

/** \brief	Get wall sensor in millimeters
 *
 *	Returns the current wall sensor data interpreted as millimeters by using a linear
 *	conversion at points x=0,x=60 where x is distance measured.
 *
 *	The value is determined using the following data where the mean material is the
 *	mean of several cardboards...
 *
 *	Material	Distance Measured (mm)	Analog Signal
 *	Mean	100	0
 *	Mean	90	1.85
 *	Mean	80	1.3
 *	Mean	70	2.62
 *	Mean	60	10
 *	Mean	50	29.1
 *	Mean	40	65.65
 *	Mean	30	116.15
 *	Mean	20	192
 *	Mean	10	240
 *	Mean	0	262
 *
 *	\return		Distance of sensed object in millimeters
 */
int COIL::getWallSensorDistanceFromSignal (int signal) {
	if(signal < 10) {
		return INT_MAX;
	} else if(signal > 262) {
		return 0;
	} else {
		int ret = (int)(((10.0-262.0)/(60.0))*(double)signal+262.0);
		if(ret > 0)	return ret;
		else		return 0;
	}
}


/** \brief	Get data from all sensors
 *
 *	Returns a pointer to the data from all sensors in the Create.  Sensors are in the order given
 *	in the CreateOI specification, starting with Bumps and Wheel Drops.
 *
 *	\return		Pointer to array of sensor data or a NULL pointer on error.
 */
bool COIL::getAllSensors(int* result)
{
	if(!result) return false;

	// Lock this to make sure that extractSensorData() doesn't collide from
	// another thread. Not entirely clean as the data source (*result) could
	// vary from different sources, but typically will never be the case.
	_mutex.lock(); {

		// Init
		byte buf[52];
		int i, numread;

		// Clear memory
		memset (buf, 0, 52*sizeof(byte));
		memset (result, 0, 36*sizeof(int));

		// Read all
		numread = readRawSensor (SENSOR_GROUP_ALL, buf, 52);
		if (numread < 52)
		{
			emit incompleteSensorDataRead();
			Debug::error ("[COIL] could not get all sensors: incomplete data (%1/52)", numread);
			_mutex.unlock();
			return false;
		}

		//Bumps And Wheel Drops to Buttons
		for (i = 0; i < 12; i++)
			result[i] = buf[i];

		result[12] = (short) ((buf[12] << 8) | buf[13]);		//Distance
		result[13] = (short) ((buf[14] << 8) | buf[15]);		//Angle
		result[14] = buf[16];									//Charging State
		result[15] = (buf[17] << 8) | buf[18];					//Voltage
		result[16] = (short) ((buf[19] << 8) | buf[20]);		//Current
		result[17] = (char) buf[21];							//Battery Temp

		//Battery Charge to Cliff Right Signal
		for (i = 0; i <= 6; i++)
			result[i + 18] = (buf[22 + 2*i] << 8) | buf[23 + 2*i];

		result[25] = buf[36];						//Cargo Bay DI
		result[26] = (buf[37] << 8) | buf[38];				//Cargo Bay Analog

		//Charging Sources to Number Of Stream Packets
		for (i = 0; i <= 4; i++)
			result[i + 27] = buf[39 + i];

		//Request sensors
		for (i = 0; i <= 3; i++)
			result [32 + i] = (short) ((buf[44 + 2*i] << 8) | buf[45 + 2*i]);

	} _mutex.unlock();

	return true;
}

/** \brief	Get raw data from multiple sensors
 *
 *  Requests the OI to return multiple sensor data packets.  The packets are returned in the
 * 	order you specify.  It is up to you to make sure you are reading the correct number of bytes
 * 	from the Create.
 *
 * 	\param[in]	packet_list	List of sensor packets to return
 * 	\param[in]	num_packets	Number of packets to get
 * 	\param[out]	buffer		Buffer to read data into
 * 	\param[in]	size		Number of bytes to read into buffer
 *
 * 	\return		number of bytes read or -1 on failure
 */
int COIL::readRawSensorList (oi_sensor* packet_list, byte num_packets, byte* buffer, int size)
{
	int numread, i;
	byte cmd[size + 2];
	cmd[0] = OPCODE_QUERY_LIST;
	cmd[1] = num_packets;

	for (i = 0; i < num_packets; i++)
		cmd[i+2] = packet_list[i];

	if (cwrite (port, cmd, size+2) < 0)
	{
		emit incompleteSensorDataRead();
		Debug::error ("[COIL] could not request sensor list");
		return -1;
	}

	numread = cread (port, buffer, size);
	if (numread < 0)
	{
		emit incompleteSensorDataRead();
		Debug::error ("[COIL] could not read sensor list");
		return -1;
	}

	return numread;
}

/**	\brief	Send script to Create
 *
 * 	Sends a script, consisting of opcodes and data bytes, to the Create's internal memory.  A
 * 	script can be up to 100  bytes long.
 *
 * 	\param	script	Script to send to Create
 * 	\param	size	Size, in bytes, of the script
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::writeScript (byte* script, byte size)
{
	int i;
	byte cmd[size+1];
	cmd[1] = OPCODE_SCRIPT;

	for (i = 0; i < size; i++)
		cmd[i+1] = script[i];

	if (cwrite (port, cmd, size+1) < 0)
	{
		Debug::error ("[COIL] could not write script");
		return -1;
	}
	return 0;
}

/** \brief	Plays currently stored script
 *
 * 	Loads the script that was written to the Create's internal memory and runs it.  The script
 * 	remains in memory after it has run, allowing you to play it multiple times.
 *
 * 	\return		0 if successful or -1 otherwise
 */
int COIL::playScript ()
{
	byte cmd[1];
	cmd[0] = OPCODE_PLAY_SCRIPT;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not play script");
		return -1;
	}
	return 0;
}

/**	\brief	Get script from Create
 *
 * 	Gets the currently stored script from the Create and returns a pointer to it.  The first data
 *	byte returned is the number of bytes in the script, followed by the script itself.
 *
 * 	\return		Pointer to script or NULL on failure
 */
byte* COIL::getScript ()
{
	byte* script;
	byte cmd[1];
	cmd[0] = OPCODE_SHOW_SCRIPT;
	byte size;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error ("[COIL] could not request script");
		return NULL;
	}
	if (cread (port, &size, 1) < 0)
	{
		 Debug::error ("[COIL] could not get script size");
		 return NULL;
	}
	script = (byte*) malloc ((size+1) * sizeof(byte));
	*script = size;

	if (cread (port, script+1, size) < 0)
	{
		Debug::error ("[COIL] could not get script data");
		return NULL;
	}

	return script;
}

/** \brief  Waits for the given amount of time
 *
 *  Waits the given amount of time, in seconds.  This timer has a resolution of 1 microsecond, or
 *  1e-6 seconds; however, there is no guarantee of the accuracy of this function.  It will probably
 *  be a little off and it is possible for an external signal to wake up your program's thread
 *  prematurely.  The function returns the time you told it to wait as a means of informing you it
 *  has woken.
 *
 *  \param  time  Time in seconds to wait
 *
 *  \return   The time passed in as a parameter
 */
void COIL::waitTime (unsigned long msecs)
{
	SleeperThread::msleep(msecs);
}

/** \brief Determines whether the create should exit from waitAngle or
 * waitDistance.
 *
 *		The purpose of this function is to determine whether the create
 *		is making satisfactory and safe progress toward whatever goal
 *		it is waiting to acheive.  Currently it checks bumper sensors,
 *		wheel drops, cliffs, and overcurrent sensors.
 *
 * 	\return		not 0 if Create should stop waiting, 0 otherwise.
 */
int COIL::stopWait()
{
  int shouldStop = 0;
  shouldStop = readSensor(SENSOR_BUMPS_AND_WHEEL_DROPS) + readSensor(SENSOR_OVERCURRENT) + getCliffs();
  return shouldStop;
}

/** \brief	Waits for Create to travel given distance
 *
 * 	Waits for Create to travel the given distance (in millimeters).  The distance is incremented
 *  when the Create travels forward and decremented when moving backward.  If the wheels are
 * 	passively rotated in either direction, the distance is incremented.  The distance sensor
 * 	will be reset after using this function.
 *
 * 	The distance travelled is updated once per 20ms or 50 times per second.
 *
 * 	\param	dist		Distance to travel in millimeters
 * 	\param	interrupt	0 if the waiting should be non-interruptible: ignore collisions
 *                              1 if the waiting should be interruptible: terminate on collision
 *
 * 	\return		Distance travelled or INT_MIN on error
 */
int COIL::waitDistance (int dist, int interrupt)
{
	int count = 0, current = 0;

	//reset sensor data
	if (INT_MIN == getDistance()) {
		return INT_MIN;
	}

	while (1)
	{
		//usleep (20000);
		SleeperThread::msleep(20);
		current = (int)getDistance();

		if (INT_MIN == current) {
			return INT_MIN;
		}
		count += current;

		if ((interrupt && stopWait()) || (dist >= 0 && count >= dist) || (dist < 0 && count <= dist))
			break;
	}
	return count;
}

/** \brief	Waits for Create to turn given angle
 *
 * 	Waits for Create to turn the given angle (in degrees).  The angle is incremented when the
 *  Create turns counterclockwise and decremented when turning clockwise.  The angle sensor will
 * 	be reset after using this function.
 *
 * 	The angle turned is updated once per 20ms or 50 times per second.
 *
 * 	\param	angle		Angle to travel in degrees.
 * 	\param	interrupt	0 if the waiting should be non-interruptible: ignore collisions
 *                              1 if the waiting should be interruptible: terminate on collision
 *
 * 	\return		Angle turned or INT_MIN on error
 */
int COIL::waitAngle (int angle, int interrupt)
{
	int count = 0, current = 0;

	//reset sensor data
	if (INT_MIN == getAngle()) {
		return INT_MIN;
	}

	while (1)
	{
		//usleep (20000);
		SleeperThread::msleep(20);
		current = (int)getAngle();

		if (INT_MIN == current) {
			return INT_MIN;
		}
		count += current;

		if ((interrupt && stopWait()) || (angle >= 0 && count >= angle) || (angle < 0 && count <= angle))
			break;
	}
	return count;
}

/**	\brief	Stops the Create and closes connection to it
 *
 * 	Stops the Create and closes the serial connection to it.  The connection can be restarted
 * 	by calling the startOI command.
 *
 *	\return		0 if successful or -1 otherwise
 */
int COIL::stopOI ()
{
	// Stop the movement of the robot
	if (directDrive (0, 0) < 0)
	{
		Debug::error ("[COIL] could not stop OI");
		return -1;
	}

	// Close port and return success
	port->close();
	return 0;
}


/** \brief	Write data to the Create
 *
 *	Writes data to the Create's serial port.  This function will continue to attempt writing to the serial
 *	port until the specified number of bytes has been written or until nothing is written 3 times in a row.
 *	At that point, the function will return the number of bytes written so far.  Use this function to write
 *	to the Create instead of OS-specific functions.
 *
 *	\param	fd			The file descriptor for the serial port
 *	\param	buf			The buffer to write from
 *	\param	numbytes	The number of bytes to write
 *
 *	\return		The number of bytes written to the port or -1 on error
 */
int COIL::cwrite (QextSerialPort *port, byte* buf, int numbytes)
{
	if(!port->isOpen()) return -1;

	int i, numwritten = 0, n = 0, numzeroes = 0;

	while (numwritten < numbytes)
	{
		n = port->write( (char*)(buf + numwritten), (numbytes - numwritten) );
		if (n < 0) {
			Debug::error("[COIL] could not write to serial device: closing connection...");
			port->close();
			emit disconnected();
			return -1;
		}
		if (n == 0)
		{
			numzeroes++;
			if (numzeroes > 3) break;
		}
		numwritten += n;
	}

	return numwritten;
}

/** \brief Read data from the Create
 *
 *	Reads data from the Create's serial port.  This function will continue reading from the port until
 *	the specified number of bytes has been read or until it reads nothing 3 times in a row (this can happen
 *	if you tell the function to read more data than the Create has available).  If this limit
 *	is reached, the buffer will contain what was read up to that point and the function will return the
 *	number of bytes read up to that point.  Use this instead of the OS-specific read function when reading
 *	from the Create.
 *
 *	\param	fd			The file descriptor for serial port
 *	\param	buf			The buffer to read the data into
 *	\param	numbytes	The number of bytes to read
 *
 *	\return		The number of bytes read from the serial port or -1 on error
 */
int COIL::cread (QextSerialPort *port, byte* buf, int numbytes)
{
	if(!port->isOpen()) return -1;

	int i, numread = 0, n = 0, numzeroes = 0;

	while (numread < numbytes)
	{
		n = port->read( (char*)(buf + numread), (numbytes - numread));
		if (n < 0) {
			Debug::error("[COIL] could not read from serial device: closing connection...");
			port->close();
			emit disconnected();
			return -1;
		}
		if (n == 0)
		{
			numzeroes++;
			if (numzeroes > 5) break;
		}
		numread += n;
	}

	port->flush();
	return numread;

}

/** \brief Helper function for extracting sensor data.
 *
 *	Extracts the specified sensor information from a complete set of sensor data.
 *
 *	\param	data			Pointer to sensor data (should be 35 bytes at least)
 *	\param	packet			The requested sensor packet
 *
 *	\return					The appropriate sensor data
 */
int COIL::extractSensorFromData(int* data, oi_sensor packet) {

	if(data == NULL) return 0;

	// Lock this to make sure that getAllSensors() doesn't collide from
	// another thread. Not entirely clean as the data source (*result) could
	// vary from different sources, but typically will never be the case.
	int result = 0;
	_mutex.lock(); {

		switch (packet)
		{
			//all one-byte unsigned sensors
			case SENSOR_BUMPS_AND_WHEEL_DROPS: result = data[0]; break;
			case SENSOR_WALL: result = data[1]; break;
			case SENSOR_CLIFF_LEFT: result = data[2]; break;
			case SENSOR_CLIFF_FRONT_LEFT: result = data[3]; break;
			case SENSOR_CLIFF_FRONT_RIGHT: result = data[4]; break;
			case SENSOR_CLIFF_RIGHT: result = data[5]; break;
			case SENSOR_VIRTUAL_WALL: result = data[6]; break;
			case SENSOR_OVERCURRENT: result = data[7]; break;
			//case SENSOR_UNUSED: result = data[8]; break;
			//case SENSOR_UNUSED: result = data[9]; break;
			case SENSOR_INFRARED: result = data[10]; break;
			case SENSOR_BUTTONS: result = data[11]; break;
			case SENSOR_DISTANCE: result = data[12]; break;
			case SENSOR_ANGLE: result = data[13]; break;
			case SENSOR_CHARGING_STATE: result = data[14]; break;
			case SENSOR_VOLTAGE: result = data[15]; break;
			case SENSOR_CURRENT: result = data[16]; break;
			case SENSOR_BATTERY_TEMP: result = data[17]; break;
			case SENSOR_BATTERY_CHARGE: result = data[18]; break;
			case SENSOR_BATTERY_CAPACITY: result = data[19]; break;
			case SENSOR_WALL_SIGNAL: result = data[20]; break;
			case SENSOR_CLIFF_LEFT_SIGNAL: result = data[21]; break;
			case SENSOR_CLIFF_FRONT_LEFT_SIGNAL: result = data[22]; break;
			case SENSOR_CLIFF_FRONT_RIGHT_SIGNAL: result = data[23]; break;
			case SENSOR_CLIFF_RIGHT_SIGNAL: result = data[24]; break;
			case SENSOR_DIGITAL_INPUTS: result = data[25]; break;
			case SENSOR_ANALOG_SIGNAL: result = data[26]; break;
			case SENSOR_CHARGING_SOURCES_AVAILABLE: result = data[27]; break;
			case SENSOR_OI_MODE: result = data[28]; break;
			case SENSOR_SONG_NUMBER: result = data[29]; break;
			case SENSOR_SONG_IS_PLAYING: result = data[30]; break;
			case SENSOR_NUM_STREAM_PACKETS: result = data[31]; break;
			case SENSOR_REQUESTED_VELOCITY: result = data[32]; break;
			case SENSOR_REQUESTED_RADIUS: result = data[33]; break;
			case SENSOR_REQUESTED_RIGHT_VEL: result = data[34]; break;
			case SENSOR_REQUESTED_LEFT_VEL: result = data[35]; break;

			//any other input is invalid (including packet groups)
			default: result = 0; break;
		}

	} _mutex.unlock();

	return result;
}

/**	\brief	Resets the Create.
 *
 * 	Resets the create using the undocumented OPCODE 7. This is useful if the iRobot has docked
 *  and will not initiate a charge. It is recommended to wait 3 seconds after entering this state.
 *
 *	\return		0 if successful or -1 otherwise
 */
int COIL::resetOI ()
{
	byte cmd[1];
	cmd[0] = OPCODE_RESET;

	if (cwrite (port, cmd, 1) < 0)
	{
		Debug::error("[COIL] could not reset OI: unable to send command");
		return -1;
	}

	return 0;
}

bool COIL::isConnected() {
	if(port == NULL) 	return false;
	else				return port->isOpen();
}

void COIL::disconnect() {
	port->close();
}
