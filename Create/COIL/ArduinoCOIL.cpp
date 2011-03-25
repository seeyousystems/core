/*
 * ArduinoCOIL.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: abraham
 */

#include "ArduinoCOIL.h"

#include "../Library/Debug.h"
#include "../Library/SleeperThread.h"

#define MAX(a,b)	(a > b? a : b)
#define MIN(a,b)	(a < b? a : b)

int ArduinoCOIL::debug = 0;

ArduinoCOIL::ArduinoCOIL(QString portName) {
	// TODO Auto-generated constructor stub
	port = new QextSerialPort(portName);
	port->setBaudRate(BAUD57600);
	port->setFlowControl(FLOW_OFF);
	port->setParity(PAR_NONE);
	port->setDataBits(DATA_8);
	port->setStopBits(STOP_1);


	if(!port->open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
		Debug::warning("[COIL] Could not open serial port on %1", portName);
	}

}

ArduinoCOIL::~ArduinoCOIL() {
	// TODO Auto-generated destructor stub
	if(port) delete port;
}

int ArduinoCOIL::stopOI ()
{
	port->close();
	return 0;
}

int ArduinoCOIL::cwrite (QextSerialPort *port, byte* buf, int numbytes)
{


	int i, numwritten = 0, n = 0, numzeroes = 0;

	//write (fd, (buf + numwritten), (numbytes - numwritten));

	while (numwritten < numbytes)
	{
		//n = write (fd, (buf + numwritten), (numbytes - numwritten));
		n = port->write( (char*)(buf + numwritten), (numbytes - numwritten) );
		if (n < 0) return -1;
		if (n == 0)
		{
			numzeroes++;
			if (numzeroes > 3) break;
		}
		numwritten += n;
	}

	if (debug)
	{
		printf ("Write: ");
		for (i = 0; i < numwritten; i++) printf ("%d ", buf[i]);
		printf (" (%d of %d bytes with %d tries)\n", numwritten, numbytes, numzeroes);
	}

	return numwritten;
}

int ArduinoCOIL::cread (QextSerialPort *port, byte* buf, int numbytes)
{
	int i, numread = 0, n = 0, numzeroes = 0;

	while (numread < numbytes)
	{
		//n = read (fd, (buf + numread), (numbytes - numread));
		n = port->read( (char*)(buf + numread), (numbytes - numread));
		if (n < 0) return -1;
		if (n == 0)
		{
			numzeroes++;
			if (numzeroes > 5) break;
		}
		numread += n;
	}

	if (debug)
	{
		printf ("Read:   ");
		for (i = 0; i < numread; i++) printf ("%d ", buf[i]);
		printf (" (%d of %d bytes with %d tries)\n", numread, numbytes, numzeroes);
	}

	port->flush();
	return numread;

}

int ArduinoCOIL::readRawSensor (byte* buffer, int size)
{
	int numread = 0;
	byte cmd[3];
	cmd[0] = SENSOR_RIGHT_PINGER;
	cmd[1] = TRAILER;
	cmd[2] = CARRIAGE_RETURN;;

	if (cwrite (port, cmd, 3) < 0)
	{
		Debug::error ("[COIL] Could not request sensor");
		return -1;
	}

	SleeperThread::msleep(40); // BUG: Why must we sleep? RTFM...

	numread = cread (port, buffer, size);
	if (numread < 0)
	{
		Debug::error ("[COIL] Could not read sensor");
		return -1;
	}

	return numread;
}

int ArduinoCOIL::readRawSensor (seeyou_sensor packet, byte* buffer, int size)
{
	int numread = 0;
	byte cmd[3];
	cmd[0] = packet;
	cmd[1] = TRAILER;
	cmd[2] = CARRIAGE_RETURN;

	if (cwrite (port, cmd, 3) < 0)
	{
		Debug::error ("[COIL] Could not request sensor");
		return -1;
	}

	SleeperThread::msleep(40); // BUG: Why must we sleep? RTFM...

	numread = cread (port, buffer, size);
	if (numread < 0)
	{
		Debug::error ("[COIL] Could not read sensor");
		return -1;
	}

	return numread;
}

int ArduinoCOIL::setLEDState ()
{
	byte cmd[3];
	cmd[0] = SENSOR_LED_INDICATOR;
	cmd[1] = TRAILER;
	cmd[2] = CARRIAGE_RETURN;

	if (cwrite (port, cmd, 3) < 0)
	{
		Debug::error ("[ArduinoCOIL] Could not set LED status");
		return -1;
	}
	return 0;
}

int ArduinoCOIL::readSensor (seeyou_sensor packet)
{
	int result = 0;
	byte* buffer;

	switch (packet)
	{
		//two-byte signed sensors
		case SENSOR_IR_0:
		case SENSOR_IR_1:
		case SENSOR_IR_2:
		case SENSOR_IR_3:
		case SENSOR_LED_INDICATOR:
		case SENSOR_COMPASS:
		case SENSOR_LEFT_PINGER:
		case SENSOR_RIGHT_PINGER:
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

int ArduinoCOIL::readCompass()
{
	return readSensor(SENSOR_COMPASS);
}

int ArduinoCOIL::readLeftPinger()
{
	return readSensor(SENSOR_LEFT_PINGER);
}

int ArduinoCOIL::readRightPinger()
{
	return readSensor(SENSOR_RIGHT_PINGER);
}

int ArduinoCOIL::readInfraredFront()
{
	return readSensor(SENSOR_IR_2);
}

void ArduinoCOIL::enableDebug()
{
	debug = 1;
}

void ArduinoCOIL::disableDebug()
{
	debug = 0;
}
