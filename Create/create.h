//Third edit by Hanam via github.com interface
#ifndef CREATE_H
#define CREATE_H

#include <QObject>
#include <QString>
#include <QSettings>

#include "math.h"
#include "Library/Algorithm/VFF.h"
#include <QTimer>

// Forwards declarations
class COIL;
class ArduinoCOIL;

class GridMap;
class HeatMap;
class TerrainMap;
class ObjectMap;
class PhysicalMap;
class StructureMap;
class FadingCollisionMap;

class Tracker;
class Controller;
class ArduinoController;

class Navigation;

class Joystick2D;

class Task;
class TaskManager;

class Network;

class DockingStation;

class Create : public QObject
{
    Q_OBJECT

public:
	// Enums
	enum AreaType { AreaTypeCollision, AreaTypeOpen, AreaTypeUnknown};

public:
	// Core Instance Settings
	bool	connected;
	long 	scale;
	long 	robotStartingPositionX;
	long 	robotStartingPositionY;
	long 	mapWidth;
	long 	mapHeight;
	long 	robotSize;
	long 	gridSize;
	long 	heatSpotSize;

	// Connection settings
	QString connectionController;
	QString connectionMovementTracker;
	QString connectionSerialPort;
	bool	connectionForceConnect;

public:
	// Core Modules and components

	COIL *coil;
	ArduinoCOIL *arduino;
	VFF *vffAI;
	Network *network;

	GridMap *gridMap;
	HeatMap *heatMap;
	TerrainMap *terrainMap;
	ObjectMap *objectMap;
	PhysicalMap *physicalMap;
	StructureMap *structureMap;
	FadingCollisionMap *fadingCollisionMap;

	Tracker *tracker;
	Controller *controller;
	ArduinoController *arduinoController;

	Navigation *navigation;

	Joystick2D *joystick;

	TaskManager *taskManager;

	QSettings *settings;

	DockingStation *dockingStation;


public:
	Create(QSettings *settings);
	Create();
	virtual ~Create();
	bool connect(QString strSerialPort, bool safeMode);
	bool isConnected();
	long mmToPixels(long mm);
	long pixelsTomm(long pixels);
	void addTask(Task *task);
	void initMapsAndObjects();
	int stopTask();
	void interruptTask();

	// Global Settings
	int intSetting(QString key);
	long longSetting(QString key);
	double doubleSetting(QString key);
	QString stringSetting(QString key);
	bool boolSetting(QString key);
	QVariant getSetting(QString key);


private:
	static int arduino_active;			//TODO: fail safe debug mode status
	static int network_active;			//TODO: fail safe debug mode status
	void initCreate(QSettings *settings);

public slots:
	bool disconnect();
	void start();
	void stop();
	void abort();
	void reset();

signals:
	void focusOnPoint(long x, long y);
	void coreReset();
	void createConnected();
	void createDisconnected();

};

#endif // CREATE_H
