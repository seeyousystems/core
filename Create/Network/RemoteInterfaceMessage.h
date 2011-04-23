/*
 *  RemoteInterfaceMessage.h
 *
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework  is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef REMOTEINTERFACEMESSAGE_H_
#define REMOTEINTERFACEMESSAGE_H_


#include <QMap>
#include <QByteArray>
#include <QDateTime>

#include "create.h"

//#include "../CoreObject.h"

class RemoteInterfaceMessage : public QObject {

	Q_OBJECT

public:
	Create *create;
	QString name;

public:
	QMap<QString,QString> *properties;
	QString type;
	QString sourceIp;
	quint16 sourcePort;
	QString destinationIp;
	quint16 destinationPort;
	QDateTime timeStamp;

public:
	RemoteInterfaceMessage(QString name, Create *create, QString type);
	RemoteInterfaceMessage(QString name, Create *create, QByteArray *data);
	RemoteInterfaceMessage(QString name, Create *create, RemoteInterfaceMessage &message);
	virtual ~RemoteInterfaceMessage();
	QString toString();
	QString property(QString key);
	QString replyIp();
	quint16 replyPort();
};

#endif /* REMOTEINTERFACEMESSAGE_H_ */
