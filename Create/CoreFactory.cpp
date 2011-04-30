/*
 *  CoreFactory.cpp
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

#include "CoreFactory.h"

#include "create.h"

#include "COIL/COIL.h"
//#include "COIL/EmulatedCOIL.h"

#include "Library/Debug.h"
#include "Library/Util.h"

//#include "Map/GridMap.h"
//#include "Map/HeatMap.h"
//#include "Map/TerrainMap.h"
//#include "Map/ObjectMap.h"
//#include "Map/PhysicalMap.h"
//#include "Map/StructureMap.h"
//#include "Map/FadingCollisionMap.h"
//
//#include "Map/MapObject/RobotMapObject.h"
//#include "Map/MapObject/NavPathMapObject.h"
//#include "Map/MapObject/NavigationMapObject.h"
//#include "Map/MapObject/MarkerMapObject.h"
//#include "Map/MapObject/TrackerMapObject.h"

#include "MovementTracker/AveragedTracker.h"
#include "MovementTracker/SingleTracker.h"
#include "MovementTracker/SelfCorrectingTracker.h"
#include "MovementTracker/RawMovementTracker.h"
#include "MovementTracker/ExpectedMovementTracker.h"
#include "MovementTracker/FrenetMovementTracker.h"
#include "MovementTracker/CorrectedFrenetMovementTracker.h"

#include "Controller/EmssController.h"
//
//#include "Navigation/SplineNavigation.h"
#include "Navigation/SystemOfWeightsNavigation.h"

#include "Task/Task.h"
#include "Task/TaskManager.h"
//#include "Task/TestMoveTask.h"
//#include "Task/DiscoveryTask.h"
//#include "Task/Discovery2Task.h"
//#include "Task/NavigationTask.h"
//#include "Task/WallFollowerTask.h"
//#include "Task/RoamingTask.h"
//#include "Task/FingerprintNavigationTask.h"
//#include "Task/ChangeModeTask.h"
//#include "Task/DockAndChargeTask.h"
//#include "Task/UndockTask.h"
//#include "Task/PauseTask.h"
//#include "Task/ScriptedTask.h"
#include "Task/JoystickNavigationTask.h"
//
//#include "RemoteInterface/RemoteInterface.h"
//
//#include "Watchdog/Watchdog.h"
//#include "Watchdog/BeepWatchdogAction.h"
//#include "Watchdog/NetworkMonitorWatchdogAction.h"
//
//#include "GUI/Viewport.h"
//#include "GUI/OptionsDialog.h"
//#include "GUI/SettingsEditor.h"
//#include "GUI/CameraView.h"
//#include "GUI/TextToSpeechView.h"
//#include "GUI/MapOverview.h"
//#include "GUI/RemoteInterfaceView.h"
//#include "GUI/HeapLoggerView.h"
//#include "GUI/MapObjectsView.h"
//#include "GUI/JoystickView.h"
//#include "GUI/RobotControlPanel.h"
//#include "GUI/RobotSpeedGraph.h"
//#include "GUI/TaskEditor.h"
//#include "GUI/WeightEditor.h"
//#include "GUI/DockingStationView.h"



COIL* CoreFactory::createCOIL(Create *create, QString name) {
	if (name == "EmulatedCOIL"){
		return NULL;//return new EmulatedCOIL(create);
	} else if (name == "COIL") {
		return new COIL(create->connectionSerialPort, create);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid COIL!", name);
		return NULL;
	}
}

Tracker* CoreFactory::createTracker(Create *create, QString name) {

	// Create the correct implementation
	Tracker *tracker;
	if (name == "AveragedTracker"){
			tracker = new AveragedTracker(create);
	} else if (name == "SingleTracker"){
		tracker = new SingleTracker(create);
	} else if (name == "SelfCorrectingTracker"){
		tracker = new SelfCorrectingTracker(create);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Tracker!", name);
		return NULL;
	}

//	// Create and connect objects related to this tracker
//	RobotMapObject *robot = new RobotMapObject(core->objectMap, tracker->getTransformation().trans().x(), tracker->getTransformation().trans().y(), core->longSetting("Robot_Diameter_mm"), core->longSetting("Robot_Diameter_mm"), Qt::white, 1.0);
//	QObject::connect(tracker, SIGNAL(moved(long, long, double)), robot, SLOT(move(long,long,double)));
//	NavPathMapObject *navPath = new NavPathMapObject(core->objectMap, Qt::white);
//	QObject::connect(tracker, SIGNAL(moved(long, long, double)), navPath, SLOT(addNavPoint(long,long)));
//	new TrackerMapObject(core->objectMap, tracker);

	return tracker;
}

MovementTracker* CoreFactory::createMovementTracker(Create *create, QString name) {

	// Figure out the starting positiopn
	long rspX = create->longSetting("Robot_StartingPositionX_mm");
	if(create->doubleSetting("Robot_StartingPositionX_mm") < 1.0) rspX = (long)(create->doubleSetting("Robot_StartingPositionX_mm")*(double)create->mapWidth);
	if(rspX > create->mapWidth) rspX = create->mapWidth/2;
	long rspY = create->longSetting("Robot_StartingPositionY_mm");
	if(create->doubleSetting("Robot_StartingPositionY_mm") < 1.0) rspY = (long)(create->doubleSetting("Robot_StartingPositionY_mm")*(double)create->mapHeight);
	if(rspX > create->mapHeight) rspY = create->mapHeight/2;

	// Create the correct implementation
	MovementTracker *movementTracker;
	QColor color;
	if(name == "RawMovementTracker") {
		movementTracker = new RawMovementTracker(create, rspX, rspY, create->doubleSetting("Robot_StartingRotation_deg"));
		color = Qt::blue;
	} else if(name == "ExpectedMovementTracker") {
		movementTracker = new ExpectedMovementTracker(create, rspX, rspY, create->doubleSetting("Robot_StartingRotation_deg"));
		color = Qt::red;
	} else if(name == "FrenetMovementTracker") {
		movementTracker = new FrenetMovementTracker(create, rspX, rspY, create->doubleSetting("Robot_StartingRotation_deg"));
		color = Qt::yellow;
	} else if(name == "CorrectedFrenetMovementTracker") {
		movementTracker = new CorrectedFrenetMovementTracker(create, rspX, rspY, create->doubleSetting("Robot_StartingRotation_deg"));
		color = Qt::green;
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Movement Tracker!", name);
		return NULL;
	}
	assert(movementTracker);

	// Create and connect objects related to this movement tracker
//	RobotMapObject *robot = new RobotMapObject(create->objectMap, movementTracker->getTransformation().trans().x(), movementTracker->getTransformation().trans().y(), create->longSetting("Robot_Diameter_mm"), create->longSetting("Robot_Diameter_mm"), color, 0.5);
//	QObject::connect(movementTracker, SIGNAL(moved(long, long, double)), robot, SLOT(move(long,long,double)));
//	NavPathMapObject *navPath = new NavPathMapObject(create->objectMap, color);
//	QObject::connect(movementTracker, SIGNAL(moved(long, long, double)), navPath, SLOT(addNavPoint(long,long)));

	return movementTracker;
}


Navigation* CoreFactory::createNavigation(Create *create, QString name) {

	// Create the navigation module specified by name
	Navigation *navigation;
	if(name == "SplineNavigation") {
		return NULL;//navigation = new SplineNavigation(create);
	} else if (name == "SystemOfWeightsNavigation") {
		navigation = new SystemOfWeightsNavigation(create, true);
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Navigation!", name);
		return NULL;
	}

	// Attach a new navigation object to draw this navigation module...
//	new NavigationMapObject(create->objectMap, navigation);

	// Load all the initial nav points (if any)
	if(create->stringSetting("Navigation_InitialNavPointsListX_mm") != "" && create->stringSetting("Navigation_InitialNavPointsListY_mm") != "") {
		QStringList navPointsX = create->stringSetting("Navigation_InitialNavPointsListX_mm").split(' ');
		QStringList navPointsY = create->stringSetting("Navigation_InitialNavPointsListY_mm").split(' ');
		if(navPointsX.count() == navPointsY.count()) {
			for(int i = 0; i < navPointsX.count(); i++) {
				navigation->addNavPoint(navPointsX[i].toLong(NULL,10), navPointsY[i].toLong(NULL,10));
			}
		}
	}

	return navigation;
}

TaskManager* CoreFactory::createTaskManager(Create *create) {
	TaskManager *taskManager = new TaskManager(create);
	taskManager->setIdleTask(CoreFactory::createTask(create, create->stringSetting("TaskManager_IdleTask")));
	return taskManager;
}

Controller* CoreFactory::createController(Create *create, QString name) {
	if (name == "EmssController") {
		return new EmssController(create, create->intSetting("Controller_EmssController_Speed"), create->intSetting("Controller_EmssController_Interval"));
	} else {
		Debug::warning("[CoreFactory] %1 is not a valid Controller!", name);
		return NULL;
	}
}

////RemoteInterface* CoreFactory::createRemoteInterface(Core *core) {
////	return new RemoteInterface(core, core->stringSetting("RemoteInterface_IP"), core->intSetting("RemoteInterface_Port"));
////}
////
////Watchdog* CoreFactory::createWatchdog(Core *core) {
////	// Create a new watchdog and add default actions
////	Watchdog *watchdog = new Watchdog(core, core->intSetting("Watchdog_Interval"));
////	if(core->boolSetting("Watchdog_AddDefaultBeepAction")) watchdog->addAction(new BeepWatchdogAction(core));
////	if(core->boolSetting("Watchdog_AddDefaultNetworkMonitorAction")) watchdog->addAction(new NetworkMonitorWatchdogAction(core));
////	return watchdog;
////}
////
////// Some views might require some additional settings, typically not defined in the core settings.
////// Such views might be a Viewport, which lets the application specifiy what the refresh rate and
////// render mode is. For these views, provide a valid additionalSettings object from which the
////// settings are grabbed. The following views support additional settings:
//////   * name == "Viewport"
//////   * name == "MapOverview"
//////   * name == "AdditionalSettingsEditor"
////QWidget* CoreFactory::createGUIView(Core *core, QString name, QSettings *additionalSettings) {
////
////	if(name == "Viewport") {
////
////		// Figure out the additional settings needed
////		int updateInterval = 200;
////		bool openGL = false;
////		if(additionalSettings) {
////			updateInterval = additionalSettings->value("Viewport_AutoUpdateInterval", 0).toInt();
////			openGL = ( additionalSettings->value("Viewport_RenderMode", 0) == "opengl" );
////		}
////
////		// Create and return view
////		Viewport *viewport = new Viewport(core->intSetting("Map_Scale"), updateInterval, openGL);
////		viewport->setAntiAlias(false);
////		return viewport;
////
////	} else if(name == "MapOverview") {
////
////		// Figure out the additional settings needed
////		int updateInterval = 200;
////		bool openGL = false;
////		if(additionalSettings) {
////			updateInterval = additionalSettings->value("MapOverview_AutoUpdateInterval", 0).toInt();
////			openGL = ( additionalSettings->value("MapOverview_RenderMode", 0) == "opengl" );
////		}
////
////		MapOverview *mapOverview = new MapOverview(core->mapWidth, core->mapHeight, core->scale, updateInterval, openGL);
////		mapOverview->setAntiAlias(false);
////		return mapOverview;
////
////	} else if(name == "TaskEditor") {
////
////		// Get a valid task list from navigator and send pointer to TaskEditor widget...
////		TaskEditor *taskEditor = new TaskEditor(core);
////		return taskEditor;
////
////	} else if(name == "WeightEditor") {
////
////		// Show editor based on the current Navigation module. The editor will worry if it is not the correct type...
////		WeightEditor *weightEditor = new WeightEditor(core);
////		return weightEditor;
////
////	} else if(name == "CameraView") {
////
////		// Show a new camera view
////		CameraView *view = new CameraView(core->stringSetting("Camera_CaptureToolPath"),
////											core->stringSetting("Camera_CaptureToolCommand"),
////											QString("%1%2").arg(Util::getResourcesFilePath()).arg(core->stringSetting("Camera_CaptureToolOutFile")),
////											core->intSetting("Camera_AutoRefreshInterval"));
////		return view;
////
////	} else if(name == "TextToSpeechView") {
////
////		// Show a new text to speech view
////		TextToSpeechView *view = new TextToSpeechView(core->stringSetting("TextToSpeech_SpeakToolPath"),
////															core->stringSetting("TextToSpeech_SpeakToolCommand"));
////		return view;
////
////	} else if(name == "RemoteInterfaceView") {
////
////		// Show a new remote interface view
////		RemoteInterfaceView *view = new RemoteInterfaceView(core);
////		return view;
////
////	} else if(name == "HeapLoggerView") {
////
////		// Show a new heap logger view
////		HeapLoggerView *view = new HeapLoggerView();
////		return view;
////
////	} else if(name == "MapObjectsView") {
////
////		// Show a new map objects view
////		MapObjectsView *view = new MapObjectsView(core);
////		return view;
////
////	} else if(name == "DockingStationView") {
////
////		// Show a new docking station view
////		DockingStationView *view = new DockingStationView(core);
////		return view;
////
////	} else if (name == "CoreSettingsEditor"){
////
////		SettingsEditor *settingsEditor = new SettingsEditor(core->settings);
////		return settingsEditor;
////
////	} else if (name == "AdditionalSettingsEditor"){
////
////		SettingsEditor *settingsEditor = new SettingsEditor(additionalSettings);
////		return settingsEditor;
////
////	} else if (name == "RobotControlPanel"){
////
////		RobotControlPanel *controlPanel = new RobotControlPanel(core);
////		return controlPanel;
////
////	} else if (name == "RobotSpeedGraph"){
////
////		RobotSpeedGraph *speedGraph = new RobotSpeedGraph(core);
////		return speedGraph;
////
////	} else {
////		Debug::warning("[CoreFactory] %1 is not a valid view!", name);
////		return NULL;
////	}
////}

// Creates the specified task. If no name is specified, then a GUI
// dialog pops up where a task can be selected.
Task* CoreFactory::createTask(Create *create, QString name) {
	// Is no task specified? If so, open up a GUI dialog...
	Task *task = NULL;
	 if (name == "JoystickNavigationTask") {
		task = new JoystickNavigationTask(create);
	}  else if (name == "") {
		Debug::warning("[CoreFactory] %1 is not a valid task!", name);
		return NULL;
	}
	return task;
}

