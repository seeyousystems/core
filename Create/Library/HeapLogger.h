/*
 *  HeapLogger.h
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

#ifndef HEAPLOGGER_H_
#define HEAPLOGGER_H_

class CoreObject;
class CoreThread;

#include <QHash>
#include <QReadWriteLock>

// Remove the HEAPLOGGER_ENABLED definition to disable all heap logging calls at compile
// time. GNU G++ should optimize away the empty functions even without the -O flag.
//#define HEAPLOGGER_ENABLED

class HeapLogger{

private:
	static QReadWriteLock lock;
	static QHash<QString, int> logger;
public:
	static void registerEntity(CoreObject *coreObject);
	static void unregisterEntity(CoreObject *coreObject);
	static void registerEntity(CoreThread *coreThread);
	static void unregisterEntity(CoreThread *coreThread);
	static void registerEntity(QString name);
	static void unregisterEntity(QString name);
	static void printAllEntities();
	static int getRegisteredEntityCount();
	static QHash<QString, int> getHeapSnapshot();
	static void reset();

};

#endif /* HEAPLOGGER_H_ */
