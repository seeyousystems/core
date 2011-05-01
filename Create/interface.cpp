#include "interface.h"

#include <QLayout>

#include "Library/Debug.h"
#include "Library/Util.h"
#include "Library/Joystick2D.h"

#include "Map/GridMap.h"
#include "Map/HeatMap.h"
#include "Map/TerrainMap.h"
#include "Map/ObjectMap.h"
#include "Map/PhysicalMap.h"
#include "Map/StructureMap.h"
#include "Map/FadingCollisionMap.h"

#include "GUI/MapObjectsView.h"
#include "GUI/MapOverview.h"

#include "create.h"
#include "CoreFactory.h"

#include "COIL/COIL.h"

#include "MovementTracker/MovementTracker.h"
#include "MovementTracker/Tracker.h"


#include "Controller/Controller.h"
#include "Controller/ArduinoController.h"

#include "Task/Task.h"
#include "Task/TaskManager.h"
#include "Task/TestMoveTask.h"
//#include "Task/SeeYouTask.h"


Interface::Interface(QWidget *parent)
    : QWidget(parent)
{
	// Initialize
	create = NULL;
	settings = NULL;
	ignoreViewportClosedSignal = false;

	// PushButtons
	btnConnect = new QPushButton(QIcon(":connect"), "Connect");
	btnAbort = new QPushButton(QIcon(":cross"), "Abort");
	btnAbort->setEnabled(false);

	// GUI
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(createControlsExclusiveGroup(), 0, 0);
	//mainLayout->addWidget(createButtonsExclusiveGroup(), 0, 1);
	mainLayout->addWidget(createBlockControllerExclusiveGroup(), 0, 1);
	mainLayout->addWidget(createSensorExclusiveGroup(), 0, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("SeeYou Systems"));
	resize(640, 240);




	// Setup connections
	connect(btnConnect, SIGNAL(clicked()), this, SLOT(connectDisconnect()));
	connect(cbTask, SIGNAL(currentIndexChanged(int)), this, SLOT(currentTask(int)));

	// Slider connections
	connect(sliderDistance, SIGNAL(valueChanged(int)), lcdDistance, SLOT(display(int)));
	connect(sliderSpeed, SIGNAL(valueChanged(int)), lcdSpeed, SLOT(display(int)));
	connect(sliderAngle, SIGNAL(valueChanged(int)), lcdAngle, SLOT(display(int)));

	//Weight Editor
	connect(this, SIGNAL(createConnectionChanged(Create*)), weightEditor, SLOT(registerCore(Create*)));


	// Set default values
	sliderDistance->setRange(0, 1000);
	sliderDistance->setValue(100);
	sliderSpeed->setRange(0, 500);
	sliderSpeed->setValue(100);
	sliderAngle->setRange(0,360);
	sliderAngle->setValue(45);
	checkNegativeAngle->setChecked(false);

	// Enable/Disable Buttons
	blockBox->setEnabled(false);

	// Load settings and default values if needed
	settings = new QSettings(QString("%1emssInterface.config").arg(Util::getResourcesFilePath()), QSettings::IniFormat);

	if(settings->value("Window_X", -1) != -1 && settings->value("Window_Y", -1) != -1 && settings->value("Window_Width", -1) != -1 && settings->value("Window_Height", -1) != -1) this->setGeometry(settings->value("Window_X").toInt(), settings->value("Window_Y").toInt(), settings->value("Window_Width").toInt(), settings->value("Window_Height").toInt());

	// Setup debugging console
	//Debug::setOutput(txtHistory);
	if(settings->value("Log_LogToFile", "false").toBool()) {
		QString date = QDate::currentDate().toString("yyyyMMdd");
		QString time = QTime::currentTime().toString("hhmmss");
		Debug::logToFile(Util::getLogsFilePath().append(QString("%1_%2.log").arg(date).arg(time)));
	}



	// Set up viewport list
	viewports = new QList<Viewport*>();
}

Interface::~Interface()
{

	// Save settings
	settings->setValue("Connection_Port", cbPort->currentIndex());
//	settings->setValue("Connection_Controller", cbController->currentIndex());
//	settings->setValue("Connection_Tracker", cbTracker->currentIndex());
//	settings->setValue("Connection_Navigation", cbNavigation->currentIndex());
//	settings->setValue("Connection_EmulateHardware", checkEmulation->isChecked());
//	settings->setValue("Connection_SafeMode", checkSafeMode->isChecked());
	settings->setValue("Window_X", this->geometry().x());
	settings->setValue("Window_Y", this->geometry().y());
	settings->setValue("Window_Width", this->geometry().width());
	settings->setValue("Window_Height", this->geometry().height());
	settings->setValue("Controller_TargetSpeed", sliderSpeed->value());
	// Free heap memory
	if(viewports) delete viewports;
	if(settings) delete settings;
	if(create) delete create;
}

void Interface::connectDisconnect()
{
	if(create == NULL)
	{
		create = new Create();
		connect(btnAbort, SIGNAL(clicked()), create, SLOT(abort()));
		connect(create, SIGNAL(createConnected()), this, SLOT(createConnected()));
		connect(create, SIGNAL(createDisconnected()), this, SLOT(createDisconnected()));
	}
	// Connect or disconnect...
	if (create->isConnected() == false) {

		Debug::print("--------------------------------------------");
		btnConnect->setText("Connecting...");
		btnConnect->repaint();

		// Init the create
		if(create->connect(cbPort->currentText(), true)) {
			create->start();
			blockBox->setEnabled(true);
		}

		// Enable/Disable Buttons
//		btnConnect->setText("Disconnect");
//		blockBox->setEnabled(true);
		//groupBox->

	}
	else {

		Debug::print("[Interface] disconnecting...");

		// Call for disconnect
		create->stop();
		create->disconnect();

		// Enable/Disable Buttons
		btnConnect->setText("Disconnecting...");
		btnConnect->repaint();
		blockBox->setEnabled(false);


	}
}

void Interface::createConnected()
{
	// Connections...
	connect(joystick, SIGNAL(yokeChanged(double,double)), create->joystick, SLOT(setYoke(double,double)));
	connect(sliderSpeed, SIGNAL(valueChanged(int)), create->controller, SLOT(setTargetSpeed(int)));
	create->controller->setTargetSpeed(sliderSpeed->value());
	connect(create->tracker, SIGNAL(moved(long,long,double)), this, SLOT(focusOnPoint(long, long)));

	// GUI stuff...
	btnConnect->setText("Disconnect");
	btnConnect->setIcon(QIcon(":disconnect"));
//	menuView->setEnabled(true);
	//btnAbort->setEnabled();
	//btnRefreshSensors->setEnabled(true);
	if(viewports->count() == 0) menuAction(actionViewNewViewport); // open a new viewport

	emit createConnectionChanged(create);
	Debug::print("[Interface] connected");
}

void Interface::createDisconnected()
{
	// Gui stuff
	btnConnect->setText("Connect");
	btnConnect->setIcon(QIcon(":connect"));
	btnConnect->repaint();
	btnAbort->setEnabled(false);
//	menuView->setEnabled(false);
	//btnRefreshSensors->setEnabled(false);

	emit createConnectionChanged(create);
	Debug::print("[Interface] disconnected here");
}

void Interface::menuAction(QAction *action) {

	if(action == actionInterfaceExit) {

		if(create) create->disconnect();
		qApp->closeAllWindows();

	} else if(action == actionViewNewViewport) {

		Viewport *viewport = (Viewport*)CoreFactory::createGUIView(create, "Viewport", settings);
		assert(viewport);

		viewport->registerMap(create->physicalMap);
		viewport->registerMap(create->gridMap);
		viewport->registerMap(create->terrainMap);
		viewport->registerMap(create->heatMap);
		viewport->registerMap(create->fadingCollisionMap);
		viewport->registerMap(create->structureMap);
		viewport->registerMap(create->objectMap);

		viewport->addToolbarAction("Add Task","task[instant]", QIcon(":cog_add"));
		viewport->addToolbarAction("Navigate","navigationtask[instant]", QIcon(":cog_go"));
		viewport->addToolbarSeparator();
		viewport->addToolbarAction("Set NavPoint","navpoint", QIcon(":flag_pink"));
		viewport->addToolbarAction("Reset NavPoints","resetNavpoint[instant]", QIcon(":/flag_pink_delete"));
		viewport->addToolbarSeparator();
		viewport->addToolbarAction("Map Info","environmentinfo", QIcon(":information"));
		viewport->addToolbarAction("Focus on Point","focusviewport", QIcon(":cursor"));
		viewport->addToolbarSeparator();
		viewport->addToolbarAction("Find Robot","findrobot[instant]", QIcon(":find"));
		viewport->addToolbarAction("Move Robot","moverobot", QIcon(":arrow_in"));
		viewport->addToolbarAction("Rotate Robot","rotaterobot", QIcon(":arrow_rotate_anticlockwise"));
		viewport->addToolbarAction("Set Robot Docked","setrobotdocked[instant]", QIcon(":house"));
		viewport->addToolbarAction("Move Docking Station","movedockingstation", QIcon(":arrow_in_gray"));
		viewport->addToolbarAction("Rotate Docking Station","rotatedockingstation", QIcon(":arrow_rotate_anticlockwise_gray"));
		viewport->addToolbarSeparator();
		viewport->addToolbarAction("Mark as Collision","collision", QIcon(":cross"));
		viewport->addToolbarAction("Mark as Open Area","openarea", QIcon(":tick"));
		viewport->addToolbarAction("Raise Terrain","raiseterrain", QIcon(":world_add"));
		viewport->addToolbarAction("Lower Terrain","lowerterrain", QIcon(":world_delete"));

		viewports->append(viewport);

		connect(viewport, SIGNAL(toolbarAction(Viewport*, QString,long,long)), this, SLOT(viewportAction(Viewport*, QString,long,long)));
		connect(viewport, SIGNAL(closed(Viewport*)), this, SLOT(viewportClosed(Viewport*)));

		viewport->setGeometry(this->geometry().x()+this->width()+20,this->geometry().y(), (int)(this->width()*1.5),this->height());
		viewport->show();
		viewport->focusOnPoint(create->tracker->getX(), create->tracker->getY());

	} else if(action == actionViewTaskEditor) {

		QWidget *view = CoreFactory::createGUIView(create, "TaskEditor");
		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
		view->show();

	} else if(action == actionViewWeightEditor) {

		QWidget *view = CoreFactory::createGUIView(create, "WeightEditor");
		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
		view->show();

	} else if(action == actionViewRobotControlPanel) {

		QWidget *view = CoreFactory::createGUIView(create, "RobotControlPanel");
		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
		view->show();

	}

//	else if(action == actionViewRobotSpeedGraph) {
//
//		QWidget *view = CoreFactory::createGUIView(create, "RobotSpeedGraph");
//		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
//		view->show();
//
//	}

//	else if(action == actionViewCameraView) {
//
//		CoreFactory::createGUIView(create, "CameraView")->show();
//
//	}

//	else if(action == actionViewTextToSpeechView) {
//
//		CoreFactory::createGUIView(create, "TextToSpeechView")->show();
//
//	}
//
//	else if(action == actionViewRemoteInterface) {
//
//		QWidget *view = CoreFactory::createGUIView(create, "RemoteInterfaceView");
//		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
//		view->show();
//
//	}
//	else if(action == actionViewHeapLogger) {
//
//		CoreFactory::createGUIView(create, "HeapLoggerView")->show();
//
//	}
//	else if(action == actionHelpContents) {
//
//		InformationView *view = new InformationView("emss Interface Documentation", QIcon(":help"), "emssInterface_documentation");
//		view->show();
//
//	}
//		else if(action == actionHelpAbout) {
//
//		InformationView *view = new InformationView("About emss Interface", QIcon(":information"), "emssInterface_about");
//		view->show();

//	}
	else if(action == actionViewMapObjects) {

		QWidget *view = CoreFactory::createGUIView(create, "MapObjectsView");
		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
		view->show();

	} else if(action == actionViewDockingStation) {

		QWidget *view = CoreFactory::createGUIView(create, "DockingStationView");
		connect(this, SIGNAL(createConnectionChanged(Create*)), view, SLOT(registerCore(Create*)));
		view->show();

	} else if (action == actionInterfaceReset){

		// Close all viewports and reset
		ignoreViewportClosedSignal = true;
		for(int i = 0; i < viewports->count(); i++) {
			viewports->at(i)->close();
		}
		viewports->clear();
		ignoreViewportClosedSignal = false;
		if(create) {
			// Disconnect, reset, connect
			if (create->isConnected()) connectDisconnect();
			create->reset();
			connectDisconnect();
		}

	} else if (action == actionInterfaceSaveState){

		Util::saveWorldState(create);

	} else if (action == actionInterfaceSettingsCore){

		CoreFactory::createGUIView(create, "CoreSettingsEditor")->show();

	} else if (action == actionInterfaceSettingsController){

		CoreFactory::createGUIView(create, "AdditionalSettingsEditor", settings)->show();

	} else if (action == actionInterfaceResetFilePaths) {

		Util::resetFilePaths();

	} else {

		// This must be a dynamically-created action
		if(action->data().toString() == "newtask") {
			if(create && create->taskManager) create->taskManager->addTask(CoreFactory::createTask(create, action->text()));
		}
	}
}

void Interface::refreshSensors() {
}

void Interface::focusOnPoint(long x, long y){
	foreach(Viewport *viewport, *viewports){
		if(viewport) viewport->autoFocus(create->tracker->getX(), create->tracker->getY());
	}
}

void Interface::viewportAction(Viewport *viewport, QString value, long x, long y){
	Util::viewportAction(create, viewport, value, x, y, this->sliderSpeed->value());
}

void Interface::viewportClosed(Viewport *viewport) {
	if(!ignoreViewportClosedSignal) viewports->removeAll(viewport);
}

void Interface::currentTask(int index)
{
	Debug::print("[Interface] current index is: %1 name: %2", index, cbTask->itemText(index));
	QString task = cbTask->itemText(index);
	if(task == "Backwards") {
//		create->taskManager->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
	}
	else if(task == "Rotate 90") {
		create->addTask(new TestMoveTask(create, task));
	}
	else if(task == "Accuracy Test") {
		create->addTask(new TestMoveTask(create, task));
	}
//	else if(task == "Rotate -90")
//	{
//			create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
//	}
//	else if(task == "Rotate 360") {
//		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
//	}
//	else if(task == "Straight Path Move") {
//		//create->addTask(new StraightPathMoveTask(create,0,100 * create->scale, this->sliderSpeed->value()));
////		create->addTask(new StraightPathMoveTask(create,0,1000, this->sliderSpeed->value()));
////		if(create->vffAI->hist.sensorAngle > 60 && create->vffAI->hist.sensorAngle < 120)
////		{
////			//mode = SeeYouController::EmergencyStop;
////
////			create->boolSetting("SEEYOUCONTROLLER_EMERGENCY_STOP_ENABLED") == true;
////
////			create->controller->emergencyStop();
////		}
//	}
//	else if(task == "Wall Follower") {
//		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
//	}
	else if(task == "Straight")
	{
		;//create->taskManager->addTask(new TestMoveTask(create, task, this->sliderSpeed->value()));
		create->taskManager->addTask(new TestMoveTask(create, task));

	}
	else if(task == "Stop")
	{
//		//create->taskManager->stopRequested = true;
//		//Debug::print("[Interface] current status %l", create->taskManager->currentTask->status);
//		//create->taskManager->currentTask->status = Task::Interrupted;
//		if (create->taskManager->getIdleTask() != NULL)
//		{
//			Debug::print("[Interface] current status ");
//
//			create->taskManager->addTask(new TestMoveTask(create, task));
//		}

		if (create->taskManager->getIdleTask() != NULL)
		{

			create->taskManager->interruptTask();
			Debug::print("[SeeYouTask:NETCOMM] Stop Tasks");
		}

//		for(int i = 0; i < create->taskManager->tasks->count(); i++) {
//			create->taskManager->tasks->at(i)->status = Task::Interrupted;
//		}
		//create->stopTask();
		//create->taskManager->currentTask->interrupt();
		//create->taskManager->currentTask->interruptRequested();
		//create->coil->directDrive(0, 0);
		//create->controller->regularStop();
		//create->taskManager->currentTask = NULL;
	//	mode = SeeYouController::EmergencyStop;
//create->controller

		//Debug::print("[Interface] current status %l", create->taskManager->currentTask->status);
		//create->taskManager->currentTask = task

		//create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value(), Task::Immediate));


	}
}

QGroupBox *Interface::createSensorExclusiveGroup()
{
	sensorBox = new QGroupBox(tr("S&ensor Box"));

	sensorBox->setCheckable(false);
	sensorBox->setChecked(true);

	// Sensor Controller
	QGridLayout *layoutTopLeftBottomMaster = new QGridLayout();
	QFrame *frameTopLeftBottomNavigation = new QFrame();
	QGridLayout *layoutTopLeftBottomNavigation = new QGridLayout();
	frameTopLeftBottomNavigation->setLayout(layoutTopLeftBottomNavigation);


	// Set up lcd objects
	lcdLeft = new QLCDNumber();
	lcdLeft->setFrameStyle(QFrame::NoFrame);
	lcdLeft->setSegmentStyle(QLCDNumber::Flat);

	lcdUpperLeft = new QLCDNumber();
	lcdUpperLeft->setFrameStyle(QFrame::NoFrame);
	lcdUpperLeft->setSegmentStyle(QLCDNumber::Flat);

	lcdFront = new QLCDNumber();
	lcdFront->setFrameStyle(QFrame::NoFrame);
	lcdFront->setSegmentStyle(QLCDNumber::Flat);

	lcdUpperRight = new QLCDNumber();
	lcdUpperRight->setFrameStyle(QFrame::NoFrame);
	lcdUpperRight->setSegmentStyle(QLCDNumber::Flat);

	lcdRight = new QLCDNumber();
	lcdRight->setFrameStyle(QFrame::NoFrame);
	lcdRight->setSegmentStyle(QLCDNumber::Flat);

	layoutTopLeftBottomNavigation->addWidget(new QLabel("Left:"), 1, 0);
	layoutTopLeftBottomNavigation->addWidget(lcdLeft, 2, 0);
	layoutTopLeftBottomNavigation->addWidget(new QLabel("UpperLeft:"), 3, 0);
	layoutTopLeftBottomNavigation->addWidget(lcdUpperLeft, 4, 0);
	layoutTopLeftBottomNavigation->addWidget(new QLabel("Front:"), 5, 0);
	layoutTopLeftBottomNavigation->addWidget(lcdFront, 6, 0);
	layoutTopLeftBottomNavigation->addWidget(new QLabel("UpperRight:"), 7, 0);
	layoutTopLeftBottomNavigation->addWidget(lcdUpperRight, 8, 0);
	layoutTopLeftBottomNavigation->addWidget(new QLabel("Right:"), 9, 0);
	layoutTopLeftBottomNavigation->addWidget(lcdRight, 10, 0);
	layoutTopLeftBottomMaster->addWidget(frameTopLeftBottomNavigation, 11, 0);

	sensorBox->setLayout(layoutTopLeftBottomMaster);

	return sensorBox;
}

QGroupBox *Interface::createControlsExclusiveGroup()
{
	//QGroupBox *groupBox = new QGroupBox(tr("E&xclusive Controls"));
	exclusiveBox = new QGroupBox(tr("E&xclusive Controls"));

	exclusiveBox->setCheckable(false);
	exclusiveBox->setChecked(true);

	// Weight Editor
	weightEditor = new WeightEditor(NULL);


	// Joystick
	joystick = new JoystickView(JoystickView::JOYSTICK_MODE_FREE);
	joystick->acceleration = JoystickView::JOYSTICK_ACCELERATION_INSTANT;
	joystickFluidController = new JoystickView(JoystickView::JOYSTICK_MODE_FREE);
	joystickFluidController->acceleration = JoystickView::JOYSTICK_ACCELERATION_SLOWDOWN;

	// PushButtons
	btnConnect = new QPushButton(QIcon(":connect"), "Connect");
	btnAbort = new QPushButton(QIcon(":cross"), "Abort");
	btnAbort->setEnabled(false);

	// Kill switch Button
	killSwitchButton = new QPushButton(tr("Kill Switch"));
	killSwitchButton->setStyleSheet("* { background-color: rgb(255, 0, 0) }");
	killSwitchButton->setEnabled(true);


	// Port Connections
	cbPort = new QComboBox();
	cbPort->addItem("/dev/ttyUSB0");
	cbPort->addItem("/dev/ttyUSB1");
	cbPort->addItem("/dev/ttyUSB2");
	cbPort->addItem("/dev/ttyUSB3");

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(joystick);

	vbox->addWidget(btnConnect);
	vbox->addWidget(btnAbort);
	vbox->addWidget(cbPort);
	vbox->addWidget(killSwitchButton);
	vbox->addWidget(weightEditor);
	//vbox->addWidget(cbTask);
	vbox->addStretch(1);
	exclusiveBox->setLayout(vbox);

	return exclusiveBox;
}

QGroupBox *Interface::createBlockControllerExclusiveGroup()
{
	//QGroupBox *groupBox = new QGroupBox(tr("&Buttons"));
	blockBox = new QGroupBox(tr("&Buttons"));

	blockBox->setCheckable(false);
	blockBox->setChecked(true);

	// Block Drive Controller
	QGridLayout *layoutTopLeftBottomMaster = new QGridLayout();
	tabBlockDriveController = new QWidget();
	tabBlockDriveController->setEnabled(true);
	tabBlockDriveController->setLayout(layoutTopLeftBottomMaster);
	//tabNavigation->addTab(tabBlockDriveController, "Block Drive");
	QFrame *frameTopLeftBottomNavigation = new QFrame();
	QGridLayout *layoutTopLeftBottomNavigation = new QGridLayout();
	frameTopLeftBottomNavigation->setLayout(layoutTopLeftBottomNavigation);
	btnBlockDriveTurnLeft = new QPushButton("Left");
	layoutTopLeftBottomNavigation->addWidget(btnBlockDriveTurnLeft, 1, 0);
	btnBlockDriveTurnRight = new QPushButton("Right");
	layoutTopLeftBottomNavigation->addWidget(btnBlockDriveTurnRight, 1, 2);
	btnBlockDriveForward = new QPushButton("Forwards");
	layoutTopLeftBottomNavigation->addWidget(btnBlockDriveForward, 0, 1);
	btnBlockDriveBackward = new QPushButton("Backwards");
	layoutTopLeftBottomNavigation->addWidget(btnBlockDriveBackward, 2, 1);
	layoutTopLeftBottomMaster->addWidget(frameTopLeftBottomNavigation, 0, 0);
	sliderDistance = new QSlider(Qt::Horizontal);
	layoutTopLeftBottomMaster->addWidget(sliderDistance, 1, 0);
	sliderSpeed = new QSlider(Qt::Horizontal);
	//layoutTopLeftBottomMaster->addWidget(sliderSpeed, 1, 0);
	sliderAngle = new QSlider(Qt::Horizontal);
	layoutTopLeftBottomMaster->addWidget(sliderAngle, 3, 0);
	lcdAngle = new QLCDNumber();
	lcdAngle->setFrameStyle(QFrame::NoFrame);
	lcdAngle->setSegmentStyle(QLCDNumber::Flat);
	layoutTopLeftBottomMaster->addWidget(new QLabel("Angle:"), 4,0);
	layoutTopLeftBottomMaster->addWidget(lcdAngle, 4, 0);
	// CheckBox
	checkNegativeAngle = new QCheckBox("Negative Angle");
	layoutTopLeftBottomMaster->addWidget(checkNegativeAngle, 5, 0);

	lcdDistance = new QLCDNumber();
	lcdDistance->setFrameStyle(QFrame::NoFrame);
	lcdDistance->setSegmentStyle(QLCDNumber::Flat);
	layoutTopLeftBottomMaster->addWidget(new QLabel("Distance:"), 2, 0);
	layoutTopLeftBottomMaster->addWidget(lcdDistance, 2, 0);

	cbSlowStart = new QComboBox();
	cbSlowStart->addItem("Off");
	cbSlowStart->addItem("Linear");
	cbSlowStart->addItem("Square");
	layoutTopLeftBottomMaster->addWidget(cbSlowStart, 6, 0);

	lcdSpeed = new QLCDNumber();
	lcdSpeed->setFrameStyle(QFrame::NoFrame);
	lcdSpeed->setSegmentStyle(QLCDNumber::Flat);
	layoutTopLeftBottomMaster->addWidget(new QLabel("Speed:"), 7, 0);
	layoutTopLeftBottomMaster->addWidget(lcdSpeed, 7, 0);
	layoutTopLeftBottomMaster->addWidget(sliderSpeed, 8, 0);

	// Task list
	cbTask = new QComboBox();
	cbTask->addItem("Backwards");
	cbTask->addItem("Rotate 90");
	cbTask->addItem("Rotate -90");
	cbTask->addItem("Rotate 360");
	cbTask->addItem("Straight Path Move");
	cbTask->addItem("Accuracy Test");
	cbTask->addItem("Wall Follower");
	cbTask->addItem("Straight");
	cbTask->addItem("Stop");
	layoutTopLeftBottomMaster->addWidget(new QLabel("Tasks"), 9, 0);
	layoutTopLeftBottomMaster->addWidget(cbTask, 10, 0);

	blockBox->setLayout(layoutTopLeftBottomMaster);

	return blockBox;
}

QGroupBox *Interface::createButtonsExclusiveGroup()
{
	QGroupBox *groupBox = new QGroupBox(tr("&Buttons"));
	groupBox->setCheckable(true);
	groupBox->setChecked(true);
	return groupBox;
}
