/*
 *  CoreObject.h
 *
 *  A module in the Core which either does not require any continuous processing
 *  or is processed by another module inherits from the CoreObject class. The
 *  CoreObject is a simple interface which only requires a human-readable type
 *  name and a pointer to its Core in order to be constructed. In addition, when
 *  activated, HeapLogger routines are called in order to keep track of which
 *  CoreObjects are on the heap.
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

#ifndef COREOBJECT_H_
#define COREOBJECT_H_

#include <QObject>

#include "create.h"

#include "Library/HeapLogger.h"

class CoreObject : public QObject
{
	Q_OBJECT


public:
	QString name;
	Create *create;

public:
	CoreObject(QString name, Create *create = NULL) {
		this->name = name;
		this->create = create;
		HeapLogger::registerEntity(this);
	}


	virtual ~CoreObject() {
		HeapLogger::unregisterEntity(this);
	}


};

#endif /* COREOBJECT_H_ */
