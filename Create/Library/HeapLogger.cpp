/*
 *  HeapLogger.cpp
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


#include "HeapLogger.h"

#include <QReadLocker>

#include "../CoreObject.h"
#include "../CoreThread.h"

#include "Debug.h"



QHash<QString, int> HeapLogger::logger;
QReadWriteLock HeapLogger::lock;

void HeapLogger::registerEntity(CoreObject *coreObject){
#ifdef HEAPLOGGER_ENABLED
	registerEntity(coreObject->name);
#endif
}

void HeapLogger::unregisterEntity(CoreObject *coreObject){
#ifdef HEAPLOGGER_ENABLED
	unregisterEntity(coreObject->name);
#endif
}

void HeapLogger::registerEntity(CoreThread *coreThread){
#ifdef HEAPLOGGER_ENABLED
	registerEntity(coreThread->name);
#endif
}

void HeapLogger::unregisterEntity(CoreThread *coreThread){
#ifdef HEAPLOGGER_ENABLED
	unregisterEntity(coreThread->name);
#endif
}

void HeapLogger::registerEntity(QString name){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForWrite();
	{
		Debug::print("[HeapLogger] register %1", name);
		int i = logger.take(name);
		i++;
		logger.insert(name, i);
	}
	lock.unlock();
#endif
}

void HeapLogger::unregisterEntity(QString name){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForWrite();
	{
		Debug::print("[HeapLogger] free %1", name);
		int i = logger.take(name);
		if (i == 0){
			lock.unlock();
			return;
		}
		else {
			i--;
			logger.insert(name, i);
		}
	}
	lock.unlock();
#endif
}

void HeapLogger::printAllEntities(){
#ifdef HEAPLOGGER_ENABLED
	lock.lockForRead();
	{
		 Debug::print("Memory still in use\n------------------");
		 QHash<QString, int>::const_iterator i = logger.begin();
		 while (i != logger.constEnd()) {
			 Debug::print("%1 : %2", i.key(), i.value());
			 i++;
		 }
	}
	lock.unlock();
#endif
}

int HeapLogger::getRegisteredEntityCount() {
	int n = 0;
#ifdef HEAPLOGGER_ENABLED
	lock.lockForRead();
	{
		QHash<QString, int>::const_iterator i = logger.begin();
		while (i != logger.constEnd()) {
			n += i.value();
			i++;
		}
	}
	lock.unlock();
#endif
	return n;
}


QHash<QString, int> HeapLogger::getHeapSnapshot() {
	QReadLocker locker(&lock);
	return logger;
}

void HeapLogger::reset() {
	lock.lockForWrite();
	{
		logger.clear();
	}
	lock.unlock();
}
