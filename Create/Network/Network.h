/*
 * Network.h
 *
 *  Created on: Apr 14, 2011
 *      Author: abraham
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <QString>
#include <QMultiMap>
#include <QQueue>
#include <QThread>
#include <QList>
#include <QReadWriteLock>
#include "create.h"

//#include "RemoteInterfaceSender.h"
#include "RemoteInterfaceMessage.h"
#include "../Library/SleeperThread.h"
#include "../Library/Debug.h"

class RemoteInterfaceListener;
class RemoteInterfaceSender;

class Network : public QThread
{
	Q_OBJECT
public:
	QString ip;
	quint16 port;

	QMultiMap<QString,quint16> *subscribers;
	QQueue<RemoteInterfaceMessage*> *sendQueue;
	QList<RemoteInterfaceMessage*> *messages;

	typedef struct
	{
		int msgID;
		int userID;
		QString timeStamp;
		QTime time;
		int override;
		int x;
		int y;
		QString direction;
		QString room;
		int RFID;
		QString msg;
	}NetworkPacket;

	NetworkPacket netPacket;
	NetworkPacket prevNetPacket;

private:
	RemoteInterfaceListener *listener;
	RemoteInterfaceSender *sender;
	QReadWriteLock lock;

protected:
	bool stopRequested;

public:
	Create *create;
	QString name;
	int interval;

public:
	Network(Create *create, QString ip, quint16 port);
	Network(QString name, int interval);
	Network(Create *create, QString name, int interval);
	virtual ~Network();
	void stop();
	void start();
	void terminate();
	void start(QThread::Priority priority);
	void broadcastMessage(RemoteInterfaceMessage *message);
	bool sendMessage(QString ip, quint16 port, RemoteInterfaceMessage *message);
	void sendConfirm(QString ip, quint16 port, QString confirm);
	void sendError(QString ip, quint16 port, QString error);
	void subscribeToServer(QString ip, quint16 port);
	void unsubscribeFromServer(QString ip, quint16 port);
	void processMessage(RemoteInterfaceMessage *message);
	void addMessageToSent(RemoteInterfaceMessage *message);
	void deleteAllMessagesAndSendQueue();
	bool isSendQueueEmpty();
	RemoteInterfaceMessage *dequeueSendQueue();

signals:
	void messageReceived(RemoteInterfaceMessage *message);
	void messageSent(RemoteInterfaceMessage *message);
	void messageReceived();
	void hostUnsubscribed();
	void hostSubscribed();
};

#endif /* NETWORK_H_ */
