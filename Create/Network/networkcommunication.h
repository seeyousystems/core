#ifndef NETWORKCOMMUNICATION_H
#define NETWORKCOMMUNICATION_H

#include "Network.h"

class NetworkCommunication : public Network
{
    Q_OBJECT

public:
    int messageId;
    int localId;
    QString message;
    Create *create;

    struct irobot_data_IR
    {
    	int x;
    	int y;
    };
    struct irobot_data_IR ir_packet;

    struct irobot_data_Direction
    {
    	int rfid_tag;
    	int speed;
    	QString direction;
    };
    struct irobot_data_Direction direction_packet;

    struct irobot_data_info
    {
    	QString bumper;
    	int bitrate;
    	int batteryLife;
    	float voltage;
    	QString message;
    };
    struct irobot_data_info info_packet;


public:
    NetworkCommunication(int interval);
    NetworkCommunication(Create *create, int interval);

    virtual ~NetworkCommunication();
    virtual void run();

private:
    static int network_debug;
};

#endif // NETWORKCOMMUNICATION_H
