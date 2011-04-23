#include "networkcommunication.h"
#include <QDebug>
#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"
#include "Task/TaskManager.h"
#include "Task/SeeYouTask.h"

#include "create.h"
#include "Controller/SeeYouController.h"

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

    ir_packet.x = 4;
    ir_packet.y = 5;

    direction_packet.rfid_tag = 30;
    direction_packet.speed = 400;
    direction_packet.direction = "\"west\"";

    info_packet.bumper = "\"off\"";
    info_packet.bitrate = 54;
    info_packet.batteryLife = 49;
    info_packet.voltage = 5.999;
    info_packet.message = "\"some\"";

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
}

NetworkCommunication::~NetworkCommunication()
{

}

void NetworkCommunication::run()
{
    //qDebug() << system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1");
    //qDebug() << system("iwconfig wlan0 | grep Bit | cut -d\"=\" -f2 | cut -d\" \" -f1");

	int taskFlag = 1;
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


				//tODO: memory leak
				//TODO: memory leak
				//TODO: memory leak



				/*
				 * Compare the time between the messages to ignore delayed packets
				 */
				int delay = netPacket.time.secsTo(prevNetPacket.time);
				if(netPacket.msgID != prevNetPacket.msgID)
				{
					if(delay >= -5)
					{
						qDebug() << netPacket.msg;
						//IF OVERRIDE IS OFF
						if(netPacket.override == 0){
							/*
							 * Prevent tag to be overwritten with zero
							 */
							if (netPacket.RFID != 0)
							{
								create->controller->tags(netPacket.RFID);
								Debug::print("[NetworkCommunication] RFID Tag %1", netPacket.RFID);
								taskFlag = 0;
							}
						}
						//IF OVERRIDE IS ON
						else if(netPacket.override == 1) {
							if(netPacket.msg == "left") {
								create->addTask(new SeeYouTask(this->create, "NetComm_left", 100));
							}
							else if(netPacket.msg == "right") {
								create->addTask(new SeeYouTask(this->create, "NetComm_right", 100));
							}
							else if(netPacket.msg == "forward") {
								create->addTask(new SeeYouTask(this->create, "NetComm_forward", 100));
							}
							else if(netPacket.msg == "backward") {
								create->addTask(new SeeYouTask(this->create, "NetComm_backward", 100));
							}
							else if(netPacket.msg == "stop") {
								//create->addTask(new SeeYouTask(this->create, "NetComm_stop", 100));
								if (create->taskManager->getTask() != NULL)
								{
									create->taskManager->setCurrentTask(Task::Interrupted);
								}
							}
						}
					}
				}


				/*
				 * Store to previous packet
				 */
				prevNetPacket = netPacket;


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
       n = sprintf(buffer, "%d-50-%d-400-\"north\"-\"off\"-%d-49-5.999-\"Hanam breaks computers world\"-\n", 29+counter, 250+counter, 54-counter);
//       n = sprintf(buffer, "%d-%d-%d-%d-%c-%c-%d-%d-%f-%c-\n",4,4,4,2,3,4,4,4,4.0,4 );

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





//        qDebug() << "BitRate";
//        //system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1");
//        system("iwconfig wlan0 | grep Bit | cut -d\"=\" -f2 | cut -d\" \" -f1");

//        qDebug() << "Signal Level: ";
//        system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1");

       // int
        //std::cout << "Signal Level: " << system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1") << "Mbs";
        //qDebug() << system("iwconfig wlan0 | grep Signal | cut -d\"=\" -f3 | cut -d\" \" -f1");
       // qDebug() << "Bit Rate: " << system("iwconfig wlan0 | grep Bit | cut -d\"=\" -f2 | cut -d\" \" -f1") <<  "Mbs";

        this->msleep(interval);
    }
}
