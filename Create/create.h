#ifndef CREATE_H
#define CREATE_H

#include <QObject>
#include <QString>
#include <QSettings>

#include "math.h"
#include <QTimer>

// Forwards declarations
class COIL;
class ArduinoCOIL;
//class GridMap;
//class HeatMap;
//class TerrainMap;
//class ObjectMap;
//class PhysicalMap;

//class RobotObject;
//class NavPathObject;
//class NavPointsObject;
//class NavigationObject;

class MovementTracker;
class Controller;
class ArduinoController;

//class Navigation;

class Task;
class TaskManager;

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

//	GridMap *gridMap;
//	HeatMap *heatMap;
//	TerrainMap *terrainMap;
//	ObjectMap *objectMap;
//	PhysicalMap *physicalMap;
//
//	RobotObject *robotObject;
//	NavPathObject *navPathObject;
//	NavPointsObject *navPointsObject;
//	NavigationObject *navigationObject;

	MovementTracker *movementTracker;
	Controller *controller;
	ArduinoController *arduinoController;

	//Navigation *navigation;

	TaskManager *taskManager;

	QSettings *settings;;


public:
	Create();
	virtual ~Create();
	bool connect(QString strSerialPort, bool safeMode);
	bool isConnected();
	inline long mmToPixels(long mm);
	inline long pixelsTomm(long pixels);
	void addTask(Task *task);
	void initMapsAndObjects();

	// Global Settings
	int intSetting(QString key);
	long longSetting(QString key);
	double doubleSetting(QString key);
	QString stringSetting(QString key);
	bool boolSetting(QString key);
	QVariant getSetting(QString key);


private:
	static int arduino_active;			//TODO: fail safe debug mode status
	//void initCreate(QSettings *settings);

public slots:
	void robotMoved(long x,long y, double rotation);
	void robotCollided(long x, long y);
	void objectDetected(long x, long y);
	bool disconnect();
	void run();
	void stop();
	void abort();
	void reset();

signals:
	void focusOnPoint(long x, long y);
	void coreReset();

};

#endif // CREATE_H
