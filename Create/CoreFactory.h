/*
 *  CoreFactory.h
 *
 *  The CoreFactory class serves as the factory for the entire Core. It is
 *  responsible for creating the different modules and their various
 *  implementations.  For modules which have multiple implementations a
 *  name must be specified which corresponds to the human-readable type name
 *  given in CoreObject or CoreThread. All factory methods return pointers
 *  to a created object on the heap when successful, and NULL otherwise.
 *  Objects returned by the CoreFactory remain under the responsibility of
 *  the callee, meaning that they must also be cleaned up by the one who
 *  called the factory method. However, in most cases this responsibility can
 *  easily be delegated.
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

#ifndef COREFACTORY_H_
#define COREFACTORY_H_

#include <QObject>
#include <QString>
#include <QSettings>

class Create;
class COIL;
class Tracker;
class MovementTracker;
class Navigation;
class TaskManager;
class Controller;
//class RemoteInterface;
//class Watchdog;
class Task;

class CoreFactory {

public:
	static COIL* createCOIL(Create *create, QString name);
	static Tracker* createTracker(Create *create, QString name);
	static MovementTracker* createMovementTracker(Create *create, QString name);
	static Navigation* createNavigation(Create *create, QString name);
	static TaskManager* createTaskManager(Create *create);
	static Controller* createController(Create *create, QString name);
//	static RemoteInterface* createRemoteInterface(Core *core);
//	static Watchdog* createWatchdog(Core *c);
//	static QWidget* createGUIView(Core *core, QString name, QSettings *additionalSettings = NULL);
	static Task* createTask(Create *create, QString name = "");
};

#endif /* COREFACTORY_H_ */
