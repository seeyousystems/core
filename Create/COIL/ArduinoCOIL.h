/*
 * ArduinoCOIL.h
 *
 *  Created on: Mar 14, 2011
 *      Author: abraham
 */

#ifndef ARDUINOCOIL_H_
#define ARDUINOCOIL_H_

#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>

#include "qextserialport/qextserialport.h"

typedef unsigned char	byte;

class ArduinoCOIL : public QObject
{
	Q_OBJECT

protected:
	QMutex _mutex;				// Locking device for general purposes...


public:

		/** \brief Sensor trail for data verification
		 *
		 * Comments
		 */
		typedef enum
		{
			TRAILER									= 0x23,
			CARRIAGE_RETURN							= 0x0D
		}seeyou_verification;

	 	/** \brief Sensor packet constants
	     *
	     * Comments
	     */
	    typedef enum
	    {
	    	SENSOR_UNKNOWN                           = 0x30,
			SENSOR_IR_0,
			SENSOR_IR_1,
			SENSOR_IR_2,
			SENSOR_IR_3,
			SENSOR_LED_INDICATOR,
			SENSOR_COMPASS,
			SENSOR_LEFT_PINGER,
			SENSOR_RIGHT_PINGER,
			SENSOR_RFID,
		    SENSOR_ALL_SENSORS,
		    SENSOR_RESET_ALL
	    }seeyou_sensor;


public:
	ArduinoCOIL(QString portName);
	virtual ~ArduinoCOIL();

	int cwrite (QextSerialPort *port, byte* buf, int numbytes);
	int cread (QextSerialPort *port, byte* buf, int numbytes);
	int cread (QextSerialPort *port, byte* buf, int numbytes, seeyou_sensor sensors);
	void resetvariables();

private:
	QextSerialPort *port;
	static int debug;			//< debug mode status
	byte previousCompassReading[2];
	byte previousIR0Reading[2];
	byte previousIR1Reading[2];
	byte previousIR2Reading[2];
	byte previousIR3Reading[2];
	byte previousLeftPingReading[2];
	byte previousRightPingReading[2];
	byte previousRFID[2];
	byte previousAllSensors[16];



public:
    int setBaud (BaudRateType rate);
    int stopOI ();
    int setLEDState ();
    int readRawSensor (byte* buffer, int size);
    int readRawSensor (seeyou_sensor packet, byte* buffer, int size);
    int readSensor (seeyou_sensor packet);
    int readCompass();
    int readLeftPinger();
    int readRightPinger();
    int readInfraredFront();
    int readInfraredLeft();
    int readInfraredRight();
    int readRFID();
    void enableDebug();
    void disableDebug();
    int extractSensorFromData(int* data, seeyou_sensor packet);
    bool gellAllSensors(int* result);

};

#endif /* ARDUINOCOIL_H_ */
