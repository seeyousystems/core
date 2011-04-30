/*
 *  CoreThread.h
 *
 *  A module in the Core which requires continuous processing in the fashion
 *  of a thread must inherit from the CoreThread class. Similar to a CoreObject,
 *  CoreThread is a simple interface which only requires a human-readable type
 *  name and a pointer to its Core in order to be constructed. In addition a
 *  thread priority may be specified in the constructor. When activated,
 *  HeapLogger routines are called in order to keep track of which CoreThreads
 *  are on the heap.
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

#ifndef CORETHREAD_H_
#define CORETHREAD_H_

#include <QThread>

#include "create.h"

#include "Library/HeapLogger.h"

class CoreThread : public QThread
{
	Q_OBJECT


public:
	QString name;
	Create *create;

public:
	CoreThread(QString name, Create *create) {
		this->name = name;
		this->create = create;
		HeapLogger::registerEntity(this);
	}

	virtual ~CoreThread() {
		HeapLogger::unregisterEntity(this);
	}


};

#endif /* CORETHREAD_H_ */
