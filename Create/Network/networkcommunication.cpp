#include "networkcommunication.h"
#include <QDebug>
#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"
#include "Task/TaskManager.h"
#include "Task/SeeYouTask.h"
#include "Task/TestMoveTask.h"
#include "../MovementTracker/Tracker.h"

#include "Library/Joystick2D.h"
#include "../COIL/COIL.h"
#include "create.h"
#include "Controller/SeeYouController.h"
#include "Controller/ArduinoController.h"
#include "Controller/Controller.h"

#include <stdlib.h>
#include <iostream>
#include <QTcpSocket>

int NetworkCommunication::network_debug = 0;

NetworkCommunication::NetworkCommunication(int interval) :
    Network("Network Communication", interval)
{
    this->interval = interval;
    this->messageId = -1;
    this->localId = -1;
    this->message = "";
}

NetworkCommunication::NetworkCommunication(Create *create, int interval) :
    Network("Network Communication", interval)
{
	this->create = create;
    this->interval = interval;
    this->messageId = -1;
    this->localId = -1;
    this->message = "";

    serverPacket.x = 4;
    serverPacket.y = 5;
    serverPacket.rfid_tag = 30;
    serverPacket.speed = 400;
    serverPacket.direction = "\"west\"";
    serverPacket.bumper = "\"off\"";
    serverPacket.bitrate = 54;
    serverPacket.batteryLife = 49;
    serverPacket.voltage = 7.8;
    serverPacket.message = "\"some\"";

    netPacket.msgID = 0;
    netPacket.userID = 0;
    netPacket.timeStamp = "";
    netPacket.override = -1;
    netPacket.x = 0;
    netPacket.y = 0;
    netPacket.direction = "";
    netPacket.room = "";
    netPacket.RFID = -1;
    netPacket.msg = "";

    prevNetPacket = netPacket;
    prevNetPacket.time = QTime::fromString("00:00:00", "hh:mm:ss");

	connect(this, SIGNAL(yokeChanged(double,double)), this->create->joystick, SLOT(setYoke(double,double)));

}

NetworkCommunication::~NetworkCommunication()
{

}

void NetworkCommunication::run()
{
    //qDebug() << system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1");
    //qDebug() << system("iwconfig wlan0 | grep Bit | cut -d\"=\" -f2 | cut -d\" \" -f1");

    static int counter = 0;
	QTcpSocket socket;
	char buf[512];
	char buf_in[512];
	memset(buf, 0, 512);
	memset(buf_in, 0, 512);
	quint16 read_ret = 0;

	int n = 0;
	char buffer[1024]={};
	char *x = NULL;
	qint64 s;

	socket.connectToHost("130.191.166.4", 58080);
	if(socket.waitForConnected(5000) == false)
	{
		qDebug() << "Failed to connect";
		stopRequested = true;
	}

/*===============================================
 * Processing Loops:  The robot will perform these in a loop:
 *	 1. Read from the socket
 *	 2. Perform tasks from the string
 *	 3. Write debug information to socket.
 * TODO:
 * 		Create timeouts for buffered messages
===================================================*/

    stopRequested = false;
    while(stopRequested == false)
    {
        if(socket.state() == QAbstractSocket::UnconnectedState)
       {
           stopRequested = true;
       }
        // Read data
        if(socket.waitForReadyRead(0))
        {
            read_ret = socket.read(buf, 512);
            if(network_debug == 1)
            {
            	qDebug() << "received: " << buf;
            }
    /* ==============================
     * 1. Read from the socket
     *  (Message is written as:
     *  +------------+------------------+------+-----+-------------------+----------------+
		| Field      | Type             | Null | Key | Default           | Extra          |
		+------------+------------------+------+-----+-------------------+----------------+
		| 0 MessageID| int(10) unsigned | NO   | PRI | NULL              | auto_increment |
		| 1 UserID   | int(10) unsigned | NO   |     | 0                 |                |
		| 2 Posted   | timestamp        | NO   |     | CURRENT_TIMESTAMP |                |
		| 3 Override | tinyint(1)       | YES  |     | 0                 |                |
		| 4 X        | int(3)           | YES  |     | 0                 |                |
		| 5 Y        | int(3)           | YES  |     | 0                 |                |
		| 6 Direction| varchar(10)      | NO   |     |                   |                |
		| 7 Room     | varchar(2)       | NO   |     |                   |                |
		| 8 RFID     | int(3)           | YES  |     | 0                 |                |
		| 9 Message  | varchar(255)     | NO   |     |                   |                |
		+------------+------------------+------+-----+-------------------+----------------+
     * ==============================*/
            try
            {
				QString s = buf;
				QStringList tokens = s.split(",");

				/*
				 * Debugging messages can be activated by setting 'network_debug' to 1
				 */
				if(network_debug == 1)
				{
					bool ok;
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(0).toInt(&ok, 10));//, Time: %d, OR: %d, RFID: %d, Message: %s", tokens.at(1),tokens.at(3),tokens.at(4),tokens.at(9),tokens.at(10).toLatin1());
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(1).toInt(&ok, 10));
					printf("[MYSQL User_commands] At 10: \t\t%s\n",  tokens.at(2).toAscii().constData());
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(3).toInt(&ok, 10));
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(4).toInt(&ok, 10));
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(5).toInt(&ok, 10));
					printf("[MYSQL User_commands] At 10: \t\t%s\n",  tokens.at(6).toAscii().constData());
					printf("[MYSQL User_commands] At 10: \t\t%s\n",  tokens.at(7).toAscii().constData());
					printf("[MYSQL User_commands] At 10: \t\t%d\n",  tokens.at(8).toInt(&ok, 10));
					printf("[MYSQL User_commands] At 10: \t\t%s\n",  tokens.at(9).toAscii().constData());
				}

				/*
				 * Read most recent data from server
				 */
				bool ok;
				if(tokens.count() == 10)
				{
					netPacket.msgID = tokens.at(0).toInt(&ok, 10);
					netPacket.userID = tokens.at(1).toInt(&ok, 10);
					netPacket.timeStamp = tokens.at(2);
					netPacket.override = tokens.at(3).toInt(&ok, 10);
					netPacket.x = tokens.at(4).toInt(&ok, 10);
					netPacket.y = tokens.at(5).toInt(&ok, 10);
					netPacket.direction = tokens.at(6);
					netPacket.room = tokens.at(7);
					netPacket.RFID = tokens.at(8).toInt(&ok, 10);
					netPacket.msg = tokens.at(9);


					/*
					 * Extract the time which is the second token '2011-04-22 00:31:20'
					 */
					QStringList piece = netPacket.timeStamp.split(" ");
					netPacket.time = QTime::fromString(piece.at(1), "hh:mm:ss");
					//qDebug() << netPacket.time;



					/* ==============================
					 * 2. Perform Tasks
					 * ==============================*/
						/*===============================================
						 * Override Controls:  This allows messages with "forward/backwards/left/right"
						 * to execute a task.
						 * TODO: Prioritize the override commands with the autonomous controls.
						===================================================*/

					/*
					 * Compare the time between the messages to ignore delayed packets
					 */
					int delay = netPacket.time.secsTo(prevNetPacket.time);
					if(netPacket.msgID != prevNetPacket.msgID)
					{
						if(delay >= -60)
						{
							qDebug() << netPacket.msg;
							//IF OVERRIDE IS OFF
							if (netPacket.RFID != 0)
							{
	//							create->controller->tags(netPacket.RFID);
	//							Debug::print("[NetworkCommunication] RFID Tag %1", netPacket.RFID);
							}
							if(netPacket.override == 0)
							{
								/*
								 * Prevent tag to be overwritten with zero
								 */
								//if (netPacket.RFID != 0)
								//{
									//create->controller->tags(netPacket.RFID);
									Debug::print("[NetworkCommunication] RFID Tag %1", netPacket.RFID);
								//}
								if (netPacket.RFID == 52)
								{
									create->addTask(new TestMoveTask(create, "Room201"));
								}
								else if (netPacket.RFID == 57)
								{
									//create->addTask(new TestMoveTask(create, "Offices"));
								}
								else if (netPacket.RFID == 1)
								{
									//create->addTask(new TestMoveTask(create, "SeniorLab"));
								}
								else if (netPacket.RFID == 47)
								{
									//create->addTask(new TestMoveTask(create, "Room220A"));
								}
								else if (netPacket.RFID == 84)
								{
									//create->addTask(new TestMoveTask(create, "Room220"));
								}

							}
							//IF OVERRIDE IS ON
							else if(netPacket.override == 1)
							{
								if(netPacket.msg == "left")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "left"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(0.75, -0.013);
								}
								else if(netPacket.msg == "forwardleft")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "left"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(0.70, 0.70);
								}
								else if(netPacket.msg == "backwardleft")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "left"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(0.70, -0.70);
								}
								else if(netPacket.msg == "right")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "right"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(-0.75, -0.013);
								}
								else if(netPacket.msg == "forwardright")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "right"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(-0.60, 0.69);
								}
								else if(netPacket.msg == "backwardright")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "right"));
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
									emit yokeChanged(-0.81, -0.773);
								}
								else if(netPacket.msg == "forward")
								{
	//								Debug::print("[NetworkCommunication] GlobalTag for Vernon: %1", create->controller->getTags());
									//create->taskManager->addTask(new TestMoveTask(create, "forward"));
									emit yokeChanged(0.0, .90);
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
								}
								else if(netPacket.msg == "backward")
								{
									//create->taskManager->addTask(new TestMoveTask(create, "backward"));
									emit yokeChanged(0.0, -0.90);
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
								}
	//							else if(netPacket.msg == "tag")
	//							{
	//								create->addTask(new SeeYouTask(this->create, "NetComm_tag", 100));
	//							}
								else if(netPacket.msg == "stop")
								{
									//create->taskManager->addTask(new TestMoveTask(this->create, "stop"));
//									if (create->taskManager->getTask() != NULL)
//									{
//										create->taskManager->setCurrentTask(Task::Interrupted);
//									}
									emit yokeChanged(0.0, 0.0);
									Debug::print("[NetworkCommunication] left: %1   %2", netPacket.x, netPacket.x);
								}
							}
						}
					}

				/*
				 * Store to previous packet
				 */
				prevNetPacket = netPacket;

				} // end if(count == 10)

            } catch(...)
            {
            	Debug::print("[NetworkCommunication] Something is wrong...");
            }
            memset(buf, 0, 512);
        }

	/* ==============================
	 * 3. Write debug information to socket.
	 *    Message written as:
	 *      +-------------+-----------------+------+-----+-------------------+----------------+
			| Field       | Type            | Null | Key | Default           | Extra          |
			+-------------+-----------------+------+-----+-------------------+----------------+
			| 1 X         | int(10)         | YES  |     | 0                 |                |
			| 2 Y         | int(10)         | YES  |     | 0                 |                |
			| 3 RFID      | int(4) unsigned | NO   |     | 0                 |                |
			| 4 Speed     | int(4) unsigned | NO   |     | 0                 |                |
			| 5 Direction | varchar(10)     | NO   |     |                   |                |
			| 6 Bumper    | varchar(10)     | NO   |     |                   |                |
			| 7 Bitrate   | int(3) unsigned | NO   |     | 0                 |                |
			| 8 Battery   | int(3) unsigned | NO   |     | 0                 |                |
			| 9 Voltage   | float unsigned  | NO   |     | 0                 |                |
			| 10 Message  | varchar(140)    | NO   |     |                   |                |
			+-------------+-----------------+------+-----+-------------------+----------------+
	 * ==============================*/
       // int, int, int, int,  string,   string,   int,    int,    float,  string
       // X,   Y,   RFID,Speed,Direction,Bumper,Bitrate,Battery,Voltage,Message
        /*
         * Delimiter '-' to split the string
         * \"  \"
         *
         */

        /*
         * Update all variables that will be sent to the server.
         */

        serverPacket.x = create->tracker->getX();
		serverPacket.y = create->tracker->getY();

		///Debug::print("[NetworkCommunication] x: %1, y: %2", serverPacket.x, serverPacket.y);

		serverPacket.rfid_tag = create->arduinoController->getRFID();
		serverPacket.speed = 100;

		QString st;
		st.setNum(create->arduinoController->getHeading(), 10);
		serverPacket.direction = "\"" + st + "\"";
		bool bumperLeft = (create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_LEFT) == COIL::BUMPWHEELDROP_BUMP_LEFT;
		bool bumperRight = (create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_RIGHT) == COIL::BUMPWHEELDROP_BUMP_RIGHT;

		QString bump;
		bump.setNum((bumperLeft || bumperRight), 10);
		serverPacket.bumper = "\"" + bump + "\"";
		serverPacket.bitrate = 54;
		serverPacket.batteryLife = create->coil->extractSensorFromData(create->controller->sensorData,COIL::SENSOR_BATTERY_CHARGE);
		serverPacket.voltage = create->coil->extractSensorFromData(create->controller->sensorData,COIL::SENSOR_VOLTAGE);
		serverPacket.message = "\"some\"";
		//serverPacket.speed = create->coil->extractSensorFromData(create->controller->targetSpeed, create->intSetting(COIL::SENSOR_REQUESTED_VELOCITY)); //create->intSetting("EMSSCONTROLLER_SPEED"); //
		//serverPacket.speed = create->coil->extractSensorFromData(create->navigation->getWheelSpeed(0, 0);

//       n = sprintf(buffer, "%d,50,%d,400,\"north\",\"off\",%d,49,5.999,\"Hanam breaks computers world\",\n", 29+counter, 250+counter, counter);
		n = sprintf(buffer, "%d,%d,%d,%d,%s,%s,%d,%d,%f,%s,\n",
    		   serverPacket.x,
    		   serverPacket.y,
    		   serverPacket.rfid_tag,
    		   serverPacket.speed,
    		   serverPacket.direction.toAscii().constData(),
    		   serverPacket.bumper.toAscii().constData(),
    		   serverPacket.bitrate,
    		   serverPacket.batteryLife,
    		   serverPacket.voltage,
    		   serverPacket.message.toAscii().constData());

       x = new char[n];
       x = buffer;
       s = socket.write(x, n);
       if(socket.waitForBytesWritten(1000))
       {
//           qDebug() << "Bytes written" << s;
       }
       x = NULL;
       memset(buffer, 0, 1024);
       n = 0;
       s = 0;
       counter++;

       this->msleep(interval);
    }
}
