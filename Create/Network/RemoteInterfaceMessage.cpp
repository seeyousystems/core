/*
 *  RemoteInterfaceMessage.cpp
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

#include "RemoteInterfaceMessage.h"

#include <QStringList>

RemoteInterfaceMessage::RemoteInterfaceMessage(QString name, Create *create, RemoteInterfaceMessage &message) {
	// Copy all contents
	this->name = name;
	this->create = create;
	this->type = message.type;
	this->properties = new QMap<QString,QString>();
	this->sourceIp = message.sourceIp;
	this->sourcePort = message.sourcePort;
	this->destinationIp = message.destinationIp;
	this->destinationPort = message.destinationPort;
	this->timeStamp = message.timeStamp;
	QMap<QString,QString>::iterator k = message.properties->begin();
	while(k != message.properties->end()){
		this->properties->insert(k.key(),k.value());
		k++;
	}
}

RemoteInterfaceMessage::RemoteInterfaceMessage(QString name, Create *create, QString type) /*CoreObject("RemoteInterfaceMessage") */{
	// Init
	this->name = name;
	this->create = create;
	this->type = type;
	this->properties = new QMap<QString,QString>();
	this->sourceIp = "";
	this->sourcePort = 0;
	this->destinationIp = "";
	this->destinationPort = 0;
	timeStamp = QDateTime::currentDateTime();
}

RemoteInterfaceMessage::RemoteInterfaceMessage(QString name, Create *create, QByteArray *data) {
	// Init
	this->name = name;
	this->create = create;
	this->type = "unknown";
	this->properties = new QMap<QString,QString>();
	this->sourceIp = "";
	this->sourcePort = 0;
	this->destinationIp = "";
	this->destinationPort = 0;
	timeStamp = QDateTime::currentDateTime();

	// Unpack message
	QStringList keyvalues = QString(*data).split('\n');
	for(int i = 0; i < keyvalues.size(); i++) {
		if(keyvalues.value(i) != "") {
			QStringList keyvalue = keyvalues.value(i).split('=');
			if(keyvalue.count() > 0 && keyvalue.value(0) == "message") this->type = keyvalue.value(1);
			else if(keyvalue.count() == 2) properties->insert(keyvalue.value(0),keyvalue.value(1));
		}
	}
}

RemoteInterfaceMessage::~RemoteInterfaceMessage() {
	delete properties;
}

QString RemoteInterfaceMessage::toString() {
	QString data;
	data += "message=";
	data += this->type;
	QMap<QString,QString>::iterator k = this->properties->begin();
	while(k != this->properties->end()){
		data += "\n";
		data += k.key();
		data += "=";
		data += k.value();
		k++;
	}
	return data;
}

QString RemoteInterfaceMessage::property(QString key) {
	return properties->value(key);
}

QString RemoteInterfaceMessage::replyIp() {
	if(properties->value("reply-ip") != "") 	return properties->value("reply-ip");
	else										return sourceIp;
}

quint16 RemoteInterfaceMessage::replyPort() {
	if(properties->value("reply-port") != "") 	return properties->value("reply-port").toInt(NULL,10);
	else										return sourcePort;
}
