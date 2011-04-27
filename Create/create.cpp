#include "create.h"
#include "CoreFactory.h"

#include "COIL/COIL.h"
#include "COIL/ArduinoCOIL.h"

#include "Library/Debug.h"
#include "Library/Util.h"
#include "Library/SleeperThread.h"
#include "Library/Algorithm/VFF.h"

#include "Library/Joystick2D.h"
#include "Library/HeapLogger.h"

#include "MovementTracker/Tracker.h"
#include "MovementTracker/AveragedTracker.h"
#include "MovementTracker/SingleTracker.h"
#include "MovementTracker/RawMovementTracker.h"
#include "MovementTracker/ExpectedMovementTracker.h"

#include "Controller/EmssController.h"
#include "Controller/FluidDriveController.h"
#include "Controller/SeeYouController.h"
#include "Controller/SensorController.h"
#include "Controller/BlockDriveController.h"

#include "Network/networkcommunication.h"

#include "Navigation/SystemOfWeightsNavigation.h"

#include "Task/Task.h"
#include "Task/JoystickNavigationTask.h"

#include "Task/TaskManager.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <assert.h>

// TODO: If Arduino is not connected, set this variable to 0
int Create::arduino_active = 1;
int Create::network_active = 0;
Create::Create()
{
	// NULL everything for safety of deletion
	coil = NULL;
	arduino = NULL;

	tracker = NULL;
	controller = NULL;
	arduinoController = NULL;

	navigation = NULL;

	taskManager = NULL;

	settings = NULL;

	vffAI = NULL;

	network = NULL;

	joystick = new Joystick2D();

	// Load settings and default values if needed
	settings = new QSettings(QString("Resources/emssCore.config"), QSettings::IniFormat);

	// Init	QLCDNumber *lcdLeft;

	this->scale = this->longSetting("MOVEMENT_SCALE");
	this->robotStartingPositionX = this->longSetting("ROBOT_STARTING_POSITION_X_MM");
	this->robotStartingPositionY = this->longSetting("ROBOT_STARTING_POSITION_Y_MM");
	this->mapWidth = this->longSetting("MAP_HEIGHT_MM");
	this->mapHeight = this->longSetting("MAP_WIDTH_MM");
	this->robotSize = this->longSetting("ROBOT_SIZE_MM");
	this->gridSize = this->longSetting("GRID_SIZE_MM");
	this->heatSpotSize = this->longSetting("HEAT_SPOT_SIZE_MM");
	connected = false;

	// Init maps and objects
	//initMapsAndObjects();
}

Create::~Create()
{
	if(coil) delete coil;

	if(arduino) delete arduino;

	if(controller) delete controller;

	if(arduinoController) delete arduinoController;

	if(navigation) delete navigation;

	if(tracker) delete tracker;

	if(taskManager) delete taskManager;

	if(settings) delete settings;

	if(vffAI) delete vffAI;

	if(network) delete network;

	if (joystick) delete joystick;
}

bool Create::connect(QString strSerialPort, bool safeMode)
{
	Debug::error("[Create] opening port");

	// Variables
	connectionSerialPort = strSerialPort;
	QString strController = "EmssController";
	QString strNavigation = "SystemOfWeightsNavigation";
	QString strTracker = "SelfCorrectingTracker";

	// Init Virtual Force Field VFF
	if(vffAI) { delete vffAI; vffAI = NULL; }
	vffAI = new VFF();
	assert(vffAI);

	// Init Communication with iRobot
	if(coil) { delete coil; coil = NULL; }
	///coil = new COIL(strSerialPort); // no Core
	coil = CoreFactory::createCOIL(this, "COIL");
	assert(coil);

	// Start OI
	if(coil->startOI() == -1 ) {
		Debug::error("[Create] could not start OI");
		delete coil;
		coil = NULL;
		return false;
	}
	Debug::print("[Create] started OI");

	// Enter the desired op mode on the controller...
	if(safeMode) {
		// Enter safe mode
		if(coil->enterSafeMode() == -1 ) {
			Debug::error("[Create] could not enter safe mode");
			delete coil;
			coil = NULL;
			return false;
		}
		Debug::print("[Create] entered safe mode");
	}
	else {
		// Enter full mode
		if(coil->enterFullMode() == -1 ) {
			Debug::error("[Create] could not enter full mode");
			delete coil;
			coil = NULL;
			return false;
		}
		Debug::print("[Create] entered full mode");
	}

	// Reset LED's
	coil->setLEDState(COIL::LED_ADVANCE | COIL::LED_PLAY, 0, 255);

	// Init tracker module
	if(tracker) { delete tracker; tracker = NULL; }
	tracker = CoreFactory::createTracker(this, strTracker);
	assert(tracker);

	// Init movement movementTrackers based on a semicolon seperated list from settings...
	if(stringSetting("Tracker_DefaultMovementTrackers") != "") {
		foreach(QString trackerToAdd, stringSetting("Tracker_DefaultMovementTrackers").split(" ")) {
			MovementTracker *newTracker = CoreFactory::createMovementTracker(this, trackerToAdd);
			assert(newTracker);
			tracker->addMovementTracker(newTracker);
		}
	}

	// Do we need to set a selected Movement Tracker for the Single Tracker?
	if(tracker->name == "SingleTracker" || tracker->name == "SelfCorrectingTracker") {
		((SingleTracker*)tracker)->setSelectedMovementTracker(stringSetting("Tracker_SingleTracker_SelectedMovementTracker"));
	}


	//	// Init movement tracker
	//	/*if(strMovementTracker == "Raw Movement Tracker")*/ {
	//		movementTracker = new RawMovementTracker(this, robotStartingPositionX, robotStartingPositionY, 0);
	//	}

		// Init controller
	//	if(controller) { delete controller; controller = NULL; }
	//	controller = new SeeYouController(this, intSetting("EMSSCONTROLLER_SPEED"), intSetting("EMSSCONTROLLER_INTERVAL"));
	//	//controller = new BlockDriveController(this, intSetting("BLOCKDRIVECONTROLLER_INTERVAL"), intSetting("BLOCKDRIVECONTROLLER_SPEED"), intSetting("BLOCKDRIVECONTROLLER_ANGLE"), intSetting("BLOCKDRIVECONTROLLER_DISTANCE"), BlockDriveController::Off);
	//	assert(controller);

	// Init Communication with Arduino
	if(arduino_active) {
		if(arduino) { delete arduino; arduino = NULL; }
		arduino = new ArduinoCOIL("/dev/ttyUSB1"); // no Core
		assert(arduino);
		//	// Establish Communication
		//	if(arduino.startCOM() == -1)
		//	{
		//		Debug::error("[Create] could not connect to uController");
		//		delete arduino;
		//		arduino = NULL;
		//		return false; // TODO:
		//	}

		// Wait for 2 seconds for Arduino to be stable
		SleeperThread::msleep(2000);

		// Set LED to indicate initialization
		arduino->setLEDState();
		arduino->resetvariables();
	}

	// Init movement tracker
//	/*if(strMovementTracker == "Raw Movement Tracker")*/ {
//		movementTracker = new RawMovementTracker(this, robotStartingPositionX, robotStartingPositionY, 0);
//	}

	// Init controller
	if(controller) { delete controller; controller = NULL; }
	controller = CoreFactory::createController(this, strController);
	assert(controller);

//	// Init controller
//	if(controller) { delete controller; controller = NULL; }
//	controller = new SeeYouController(this, intSetting("EMSSCONTROLLER_SPEED"), intSetting("EMSSCONTROLLER_INTERVAL"));
//	//controller = new BlockDriveController(this, intSetting("BLOCKDRIVECONTROLLER_INTERVAL"), intSetting("BLOCKDRIVECONTROLLER_SPEED"), intSetting("BLOCKDRIVECONTROLLER_ANGLE"), intSetting("BLOCKDRIVECONTROLLER_DISTANCE"), BlockDriveController::Off);
//	assert(controller);

	// Init Arduino controller
	if(arduino_active == 1) {
		if(arduinoController) { delete arduinoController; arduinoController = NULL; }
		arduinoController = new SensorController(this, intSetting("SENSORCONTROLLER_SPEED"), intSetting("SENSORCONTROLLER_INTERVAL"));
		assert(arduinoController);
	}

	// Network
	if(network) { delete network; network = NULL; }
	network = new NetworkCommunication(this, 200);
	assert(network);

	// Create new navigation set
	if(navigation) { delete navigation; navigation = NULL; }
	navigation = CoreFactory::createNavigation(this, strNavigation);
	assert(navigation);

	// Init the task manager
	//taskManager = new TaskManager(this);
	// Init the task manager
	if(taskManager) { delete taskManager; taskManager = NULL; }
	taskManager = CoreFactory::createTaskManager(this);
	assert(taskManager);

	// Register misc. stuff with objects
	tracker->connectController(controller);

	// Success!
	connected = true;
	emit createConnected();
	Debug::print("[Create] connected");
	return true;
}

void Create::start() {

	// Kickoff the controller and navigator :)
	if(controller) controller->start(QThread::NormalPriority);
	if(arduino_active == 1) {
		if(arduinoController) arduinoController->start(QThread::NormalPriority);
	}
	if(taskManager) taskManager->start(QThread::NormalPriority);
	if(network_active == 1)	{
		if(network) network->start(QThread::NormalPriority);
	}
	Debug::print("[Core] Running...");
}

void Create::stop() {

	// Shutdown controller and movement tracker
	if(taskManager) taskManager->stop();
	if(controller) controller->stop();
	if(arduino_active == 1) {
		if(arduinoController) arduinoController->stop();
	}
	if(network_active == 1) {
		if(network) network->stop();
	}
	Debug::print("[Core] Stopped");
}

void Create::abort() {
	//TODO: double check this abort... does it make sense?

	// Shutdown services...
	Debug::print("[Core] Aborting, shutting down services...");
	controller->emergencyStop();
	this->stop();

	// Interrupt all tasks
	if(isConnected() == true) {
		for(int i = 0; i < taskManager->tasks->count(); i++) {
			taskManager->tasks->at(i)->status = Task::Interrupted;
		}
	}

	// Restart services
	Debug::print("[Core] Aborted, restarting services...");
	this->start();
}

bool Create::disconnect() {

	// Shutdown coil
	if(coil) {
		Debug::print("[Create] shutdown coil");
		coil->setLEDState(0, 255, 255);
		coil->enterPassiveMode();
		coil->stopOI();
		delete coil;
		coil = NULL;
	}
	// Shutdown arduino
	if(arduino) {
		if(arduino_active)
			arduino->stopOI();
		delete coil;
		coil = NULL;
	}

	// Return success
	Debug::print("[Core] Disconnected");
	connected = false;
	emit createConnected();
	return true;
}

void Create::robotMoved(long x, long y, double rotation) {

	// Update robot location
//	robotObject->x = mmToPixels(x);
//	robotObject->y = mmToPixels(y);
//	robotObject->rotation = -rotation; // Core/COIL has a anti-clockwise positive rotation

	// Update nav path and heat map
	//navPathObject->addNavPoint(x / scale,y / scale);
	//heatMap->registerHeat(ColorMap::OpenAreaChannel, x / scale, y / scale, doubleSetting("REGISTERHEAT_OPACITY_OPENAREA"), mmToPixels(heatSpotSize));

	emit focusOnPoint(x, y); // Signal to focus onto this point
}

void Create::robotCollided(long x, long y){

	//heatMap->registerHeat(ColorMap::CollisionAreaChannel, x / scale, y / scale, doubleSetting("REGISTERHEAT_OPACITY_COLLISION"), mmToPixels(heatSpotSize));
}

void Create::objectDetected(long x, long y) {

	//heatMap->registerHeat(ColorMap::CollisionAreaChannel, x / scale, y / scale, doubleSetting("REGISTERHEAT_OPACITY_OBJECT"), mmToPixels(heatSpotSize));
}

inline long Create::mmToPixels(long mm) {
	return mm / scale;
}

inline long Create::pixelsTomm(long pixels) {
	return pixels * scale;
}

void Create::addTask(Task *task) {
	taskManager->addTask(task);
}

int Create::stopTask() {
	//taskManager->tasks->currentTask->
	//return ();
//	if (taskManager->getCurrentTask() == Task::Interrupted)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}

}

void Create::interruptTask()
{
//	taskManager->setCurrentTask(Task::Interrupted);
}

void Create::initMapsAndObjects(){
//	gridMap = new GridMap(this, mmToPixels(gridSize));
//	heatMap = new HeatMap(this, mmToPixels(mapWidth), mmToPixels(mapHeight));
//	terrainMap = new TerrainMap(this, mmToPixels(mapWidth), mmToPixels(mapHeight));
//	objectMap = new ObjectMap(this, mmToPixels(mapWidth), mmToPixels(mapHeight));
//	physicalMap = new PhysicalMap(this, this->stringSetting("PHYSICALMAP_FILENAME"), this->longSetting("PHYSICALMAP_SCALE"));
//	robotObject = new RobotObject(objectMap, mmToPixels(robotStartingPositionX), mmToPixels(robotStartingPositionY), mmToPixels(robotSize), mmToPixels(robotSize));
//	navPathObject = new NavPathObject(objectMap);
//	navPointsObject = new NavPointsObject(objectMap, NULL, scale);
//	navigationObject = new NavigationObject(objectMap, NULL, scale);
}

void Create::reset(){
	if(connected == true) {
		stop();
		disconnect();
		//initMapsAndObjects();
		//connect(connectionController, connectionMovementTracker, connectionSerialPort, connectionForceConnect);
		start();
	} else {
		//initMapsAndObjects();
	}
	emit coreReset();
}

bool Create::isConnected() {
	return connected;
}

int Create::intSetting(QString key)
{
	return getSetting(key).toInt(NULL);
}

long Create::longSetting(QString key) {
	return getSetting(key).toInt(NULL);
}

double Create::doubleSetting(QString key) {
	return getSetting(key).toDouble(NULL);
}

QString Create::stringSetting(QString key) {
	return getSetting(key).toString().replace("\\=","=");
}

bool Create::boolSetting(QString key) {
	return getSetting(key).toBool();
}

QVariant Create::getSetting(QString key) {
	QVariant value = settings->value(key, "UNDEFINED");
	if(value.toString() == "UNDEFINED") {
		Debug::error("[Cretea] The Core Setting %1 is not defined!", key);
		return "UNDEFINED";
	} else {
		return value;
	}
}

//void Create::initCreate(QSettings *settings)
//{
//	assert(settings!=NULL);
//	this->settings = settings;
//
//	// Object to communicate to the iRobot Create
//	coil = NULL;
//	controller = NULL;
//	movementTracker = NULL;
//
//	this->robotStartingPositionX = 0;
//	this->robotStartingPositionY = 0;
//
//	// Flag to indicate if a connection has been established
//	activated = false;
//}
