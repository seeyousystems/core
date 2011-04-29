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

public:
    NetworkCommunication(int interval);
    NetworkCommunication(Create *create, int interval);

    virtual ~NetworkCommunication();
    virtual void run();

private:
    static int network_debug;

signals:
	void yokeChanged(double yokeX, double yokeY);
};

#endif // NETWORKCOMMUNICATION_H
