/*
 * Network.cpp
 *
 *  Created on: Apr 14, 2011
 *      Author: abraham
 */

#include "Network.h"

Network::Network(Create *create, QString ip, quint16 port) {
	// TODO Auto-generated constructor stub
	this->create = create;
	this->ip = ip;
	this->port = port;

	subscribers = new QMultiMap<QString, quint16>();
	sendQueue = new QQueue<RemoteInterfaceMessage*>();
	messages = new QList<RemoteInterfaceMessage*>();

	//listener = new NetworkListener("RemoteInterfaceMessage", create, this);
//	sender = new RemoteInterfaceSender(create, this);
}

Network::Network(QString name, int interval)
{
	this->name = name;
	this->interval = interval;

	stopRequested = false;
}

Network::Network(Create *create, QString name, int interval)
{
	this->create = create;
	this->name = name;
	this->interval = interval;

	stopRequested = false;
}

Network::~Network() {
	// TODO Auto-generated destructor stub
//	deleteAllMessagesAndSendQueue();
	delete subscribers;
	delete sendQueue;
	delete messages;
//	delete listener;
//	delete sender;
}

void Network::stop() {

//	listener->stop();
//	sender->stop();
	stopRequested = true;
	while(this->isRunning() == true) SleeperThread::msleep(100);
	Debug::print("[Network] stopped");

}
//
//void Network::terminate() {
//	listener->terminate();
//	sender->terminate();
//}
//
void Network::start() {
	this->start(QThread::NormalPriority);
}

void Network::start(QThread::Priority priority) {

//	listener->start(priority);
//	sender->start(priority);
	QThread::start(priority);
	Debug::print("[Network] started on %1:%2", ip, port);

}

//void Network::broadcastMessage(RemoteInterfaceMessage *message) {
//	// Enqueue a  message to all our subscribers
//	lock.lockForWrite(); {
//		QMap<QString,quint16>::iterator i = subscribers->begin();
//		while(i != subscribers->end()){
//			// Create a copy of this message based on the given message
//			// and change the destination
//			RemoteInterfaceMessage *msg = new RemoteInterfaceMessage(*message);
//			msg->destinationIp = i.key();
//			msg->destinationPort = i.value();
//			sendQueue->enqueue(msg);
//			i++;
//		}
//	} lock.unlock();
//
//	// Delete the original message
//	delete message;
//}
//
//bool Network::sendMessage(QString ip, quint16 port, RemoteInterfaceMessage *message) {
//	// Update destination and enqueue
//	message->destinationIp = ip;
//	message->destinationPort = port;
//	message->sourceIp = this->ip;
//	message->sourcePort = this->port;
//	lock.lockForWrite(); {
//		sendQueue->enqueue(message);
//	} lock.unlock();
//	return true; // TODO: because of the send queue, this information here is inaccurate...
//}
//
//void Network::subscribeToServer(QString ip, quint16 port) {
//	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("subscribe");
//	sendMessage(ip, port, message);
//}
//
//void Network::unsubscribeFromServer(QString ip, quint16 port) {
//	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("unsubscribe");
//	sendMessage(ip, port, message);
//}
//
//void Network::processMessage(RemoteInterfaceMessage *message) {
//
//	// Emit that we received a message
//	emit messageReceived(message);
//
//	// Is it a connection message? If so do the necessary bookkeeping...
//	if(message->type == "unknown") {
//
//		// This message is not in the correct format, it has no message property
//		Debug::warning("[RemoteInterface] received invalid message from %1:%2", ip, message->property("port"));
//
//	} else if(message->type == "error") {
//
//		// Just show warning
//		Debug::warning("[RemoteInterface] received error message from %1:%2", ip, message->property("port"));
//
//	} else if(message->type == "confirm") {
//
//		// Do nothing
//
//	} else if(message->type == "subscribe") {
//
//		// Add to subscribers!
//		QString ip = message->replyIp();
//		quint16 port = message->replyPort();
//		if(!subscribers->contains(ip,port)) {
//			subscribers->insert(ip,port);
//			sendConfirm(ip, port, "subscribe");
//			emit hostSubscribed();
//			Debug::print("[RemoteInterface] %1:%2 subscribed", ip, message->property("port"));
//		} else {
//			sendError(ip, port, "already-subscribed");
//			Debug::warning("[RemoteInterface] %1:%2 is already subscribed", ip, message->property("port"));
//		}
//
//	} else if(message->type == "unsubscribe") {
//
//		// Remove from subscribers list
//		QString ip = message->replyIp();
//		quint16 port = message->replyPort();
//		if(subscribers->contains(ip,port)) {
//			subscribers->remove(ip,port);
//			sendConfirm(ip, port, "unsubscribe");
//			emit hostUnsubscribed();
//			Debug::print("[RemoteInterface] %1:%2 unsubscribed", ip, message->property("port"));
//		} else {
//			sendError(ip, port, "not-subscribed");
//			Debug::warning("[RemoteInterface] %1:%2 is not subscribed", ip, message->property("port"));
//		}
//
//	} else if(message->type == "start-task") {
//
//		// Start a task!
//		QString error = "";
//		QString task = message->property("task");
//
//		if (task == "fingerprintnavigation"){
//			if(message->property("interval") != "" && message->property("wait-time") != "" ) {
//				// Extract properties
//				int interval = message->property("interval").toInt();
//				int waittime = message->property("wait-time").toInt();
//
//				// Pre-tasks?
//				QString preTasks = core->stringSetting("Task_FingerprintNavigationTask_PreTasks");
//				if(preTasks != "") {
//					// Just add all the tasks listed in tasks to the task manager...
//					foreach(QString taskName, preTasks.split(' ')) {
//						core->taskManager->addTask(CoreFactory::createTask(core, taskName));
//					}
//				}
//
//				// Finally add fingerprint task
//				core->addTask(new FingerprintNavigationTask(core, interval, waittime));
//			} else {
//				error = "missing-keys";
//			}
//		} else {
//			Task *t = CoreFactory::createTask(core, task);
//			if(t)	core->addTask(t);
//			else	error = "task-undefined";
//		}
//
//		if(error == ""){
//			sendConfirm(message->replyIp(), message->replyPort(), "start-task");
//			Debug::print("[RemoteInterface] started task %1", task);
//		} else {
//			sendError(message->replyIp(), message->replyPort(), error);
//			Debug::warning("[RemoteInterface] error starting %1 task: %2", task, error);
//		}
//
//	} else {
//
//		// Reply with an error stating that we don't recognize this message
//		sendError(message->replyIp(), message->replyPort(), "invalid-message-type");
//		Debug::warning("[RemoteInterface] invalid message type: %1", message->type);
//
//	}
//
//
//}
//
//void Network::sendConfirm(QString ip, quint16 port, QString confirm){
//	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("confirm");
//	message->properties->insert("confirm", confirm);
//	sendMessage(ip, port, message);
//}
//
//void Network::sendError(QString ip, quint16 port, QString error){
//	RemoteInterfaceMessage *message = new RemoteInterfaceMessage("error");
//	message->properties->insert("error", error);
//	sendMessage(ip, port, message);
//}

void Network::addMessageToSent(RemoteInterfaceMessage *message) {
	lock.lockForWrite(); {
		messages->append(message); // Add to sent list
	} lock.unlock();
	emit messageSent(message); // Let others know that a new message was sent
}

void Network::deleteAllMessagesAndSendQueue() {
	lock.lockForWrite(); {
		foreach(RemoteInterfaceMessage *message, *messages) {
			delete message;
		}
		messages->clear();
		foreach(RemoteInterfaceMessage *message, *sendQueue) {
			delete message;
		}
		sendQueue->clear();
	} lock.unlock();
}

bool Network::isSendQueueEmpty() {
	QReadLocker locker(&lock);
	return sendQueue->isEmpty();
}

RemoteInterfaceMessage* Network::dequeueSendQueue() {
	QWriteLocker locker(&lock);
	return sendQueue->dequeue();
}
