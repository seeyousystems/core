#include "interface.h"

#include <QLayout>

#include "Library/Debug.h"
#include "Library/Util.h"

#include "create.h"

#include "COIL/COIL.h"

#include "MovementTracker/MovementTracker.h"

#include "Controller/Controller.h"

#include "Task/Task.h"
#include "Task/TaskManager.h"
#include "Task/TestMoveTask.h"
#include "Task/SeeYouTask.h"
#include "Task/StraightPathMoveTask.h"

//#include "TestMoveTask.h"


Interface::Interface(QWidget *parent)
    : QWidget(parent)
{
	// Initialize
	create = NULL;

	// PushButtons
	btnConnect = new QPushButton(QIcon(":connect"), "Connect");
	btnAbort = new QPushButton(QIcon(":cross"), "Abort");
	btnAbort->setEnabled(false);

	// GUI
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(createControlsExclusiveGroup(), 0, 0);
	//mainLayout->addWidget(createButtonsExclusiveGroup(), 0, 1);
	mainLayout->addWidget(createBlockControllerExclusiveGroup(), 0, 1);
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
}

Interface::~Interface()
{
	// Free heap memory
	if(create) delete create;
}

void Interface::connectDisconnect()
{
	if(create == NULL)
	{
		create = new Create();
		connect(btnAbort, SIGNAL(clicked()), create, SLOT(abort()));
		//connect(create, SIGNAL(createConnected()), this, SLOT(createConnected()));
		//connect(create, SIGNAL(createDisconnected()), this, SLOT(createDisconnected()));
	}
	// Connect or disconnect...
	if (create->isConnected() == false) {

		Debug::print("--------------------------------------------");
		btnConnect->setText("Connecting...");
		btnConnect->repaint();

		// Init the create
		if(create->connect(cbPort->currentText(), true)) {
			create->run();
		}

		if(create->controller->name == "emss") {
			connect(joystick, SIGNAL(yokeChanged(double,double)), create->controller, SLOT(setYoke(double,double)));
		}
		else if(create->controller->name == "SeeYou") {
			Debug::print("[Interface] SeeYou Armed");
			connect(joystick, SIGNAL(yokeChanged(double,double)), create->controller, SLOT(setYoke(double,double)));
			connect(sliderSpeed, SIGNAL(valueChanged(int)), create->controller, SLOT(setSpeed(int)));
			connect(sliderAngle, SIGNAL(valueChanged(int)), create->controller, SLOT(setAngle(int)));
		}
		else if(create->controller->name == "Block Drive") {

			connect(btnBlockDriveForward, SIGNAL(clicked()), create->controller, SLOT(moveForward()));
			connect(btnBlockDriveBackward, SIGNAL(clicked()), create->controller, SLOT(moveBackward()));
			connect(btnBlockDriveTurnLeft, SIGNAL(clicked()), create->controller, SLOT(turnLeft()));
			connect(btnBlockDriveTurnRight, SIGNAL(clicked()), create->controller, SLOT(turnRight()));
			connect(sliderDistance, SIGNAL(valueChanged(int)), create->controller, SLOT(setDistance(int)));
			connect(sliderSpeed, SIGNAL(valueChanged(int)), create->controller, SLOT(setSpeed(int)));
			connect(sliderAngle, SIGNAL(valueChanged(int)), create->controller, SLOT(setAngle(int)));
			connect(cbSlowStart, SIGNAL(currentIndexChanged(QString)), create->controller, SLOT(setSlowStartMode(QString)));
			//tabBlockDriveController->setEnabled(true);
		}

		// Enable/Disable Buttons
		btnConnect->setText("Disconnect");
		blockBox->setEnabled(true);
		//groupBox->

	}
	else {

		Debug::print("[Interface] disconnecting...");

		// Call for disconnect
		create->stop();
		create->disconnect();

		// Enable/Disable Buttons
		btnConnect->setText("Connect");
		btnConnect->repaint();
		blockBox->setEnabled(false);

	}
}

void Interface::createConnected()
{
	// Connections...
	//connect(joystick, SIGNAL(yokeChanged(double,double)), create->joystick, SLOT(setYoke(double,double)));
	//connect(sliderSpeed, SIGNAL(valueChanged(int)), create->controller, SLOT(setTargetSpeed(int)));
	//create->controller->setTargetSpeed(sliderSpeed->value());

	// GUI stuff...
	btnConnect->setText("Disconnect");
	btnConnect->setIcon(QIcon(":disconnect"));
	btnAbort->setEnabled(true);
	//btnRefreshSensors->setEnabled(true);

	//emit createConnectionChanged(create);
	Debug::print("[Interface] connected");
}

void Interface::createDisconnected()
{
	// Gui stuff
	btnConnect->setText("Connect");
	btnConnect->setIcon(QIcon(":connect"));
	btnAbort->setEnabled(false);
	//btnRefreshSensors->setEnabled(false);

	//emit createConnectionChanged(create);
	Debug::print("[Interface] disconnected");
}

void Interface::refreshSensors() {
	txtSensorData->clear();
	int* data = create->coil->getAllSensors();
	if(data != NULL) {

		txtSensorData->append(QString("BUMPS_AND_WHEEL_DROPS:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS)));
		txtSensorData->append(QString("WALL:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_WALL)));
		txtSensorData->append(QString("CLIFF_LEFT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT)));
		txtSensorData->append(QString("CLIFF_RIGHT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT)));
		txtSensorData->append(QString("VIRTUAL_WALL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_VIRTUAL_WALL)));
		txtSensorData->append(QString("OVERCURRENT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_OVERCURRENT)));
		txtSensorData->append(QString("INFRARED:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_INFRARED)));
		txtSensorData->append(QString("BUTTONS:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BUTTONS)));
		txtSensorData->append(QString("DISTANCE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_DISTANCE)));
		txtSensorData->append(QString("ANGLE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_ANGLE)));
		txtSensorData->append(QString("CHARGING_STATE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_STATE)));
		txtSensorData->append(QString("VOLTAGE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_VOLTAGE)));
		txtSensorData->append(QString("CURRENT:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CURRENT)));
		txtSensorData->append(QString("BATTERY_TEMP:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_TEMP)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));
		txtSensorData->append(QString("WALL_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL)));
		txtSensorData->append(QString("CLIFF_LEFT_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT_SIGNAL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT_SIGNAL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_RIGHT_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT_SIGNAL)));
		txtSensorData->append(QString("DIGITAL_INPUTS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_DIGITAL_INPUTS)));
		txtSensorData->append(QString("ANALOG_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL)));
		txtSensorData->append(QString("CHARGING_SOURCES_AVAILABLE:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_SOURCES_AVAILABLE)));
		txtSensorData->append(QString("OI_MODE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_OI_MODE)));
		txtSensorData->append(QString("SONG_NUMBER:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_SONG_NUMBER)));
		txtSensorData->append(QString("SONG_IS_PLAYING:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_SONG_IS_PLAYING)));
		txtSensorData->append(QString("NUM_STREAM_PACKETS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_NUM_STREAM_PACKETS)));
		txtSensorData->append(QString("REQUESTED_VELOCITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_VELOCITY)));
		txtSensorData->append(QString("REQUESTED_RADIUS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RADIUS)));
		txtSensorData->append(QString("REQUESTED_RIGHT_VEL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RIGHT_VEL)));
		txtSensorData->append(QString("REQUESTED_LEFT_VEL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_LEFT_VEL)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));

		delete data;

	}

}

void Interface::focusOnPoint(long x, long y){
	//foreach(Viewport *viewport, *viewports){
		//if(viewport) viewport->autoFocus(create->robotObject->x, create->robotObject->y);
	//}
}

void Interface::currentTask(int index)
{
	//Debug::print("[Interface] current index is: %1 name: %2", index, cbTask->itemText(index));
	QString task = cbTask->itemText(index);
	if(task == "Avoid Obstacles") {
		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
	}
	else if(task == "Rotate 90") {
		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
	}
	else if(task == "Rotate 360") {
		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
	}
	else if(task == "Straight Path Move") {
		//create->addTask(new StraightPathMoveTask(create,0,100 * create->scale, this->sliderSpeed->value()));
		create->addTask(new StraightPathMoveTask(create,50000,50100, this->sliderSpeed->value()));

	}
	else if(task == "Wall Follower") {
		create->addTask(new SeeYouTask(create, task, this->sliderSpeed->value()));
	}
}

QGroupBox *Interface::createControlsExclusiveGroup()
{
	//QGroupBox *groupBox = new QGroupBox(tr("E&xclusive Controls"));
	exclusiveBox = new QGroupBox(tr("E&xclusive Controls"));

	exclusiveBox->setCheckable(false);
	exclusiveBox->setChecked(true);

	// Joystick
	joystick = new JoystickView(JoystickView::JOYSTICK_MODE_FREE);
	joystick->acceleration = JoystickView::JOYSTICK_ACCELERATION_INSTANT;
	joystickFluidController = new JoystickView(JoystickView::JOYSTICK_MODE_FREE);
	joystickFluidController->acceleration = JoystickView::JOYSTICK_ACCELERATION_SLOWDOWN;

	// Sliders
//	sliderDistance = new QSlider();
//	sliderSpeed = new QSlider(Qt::Horizontal);
//	sliderAngle = new QSlider(Qt::Horizontal);

	// LCD
//	lcdAngle = new QLCDNumber();
//	lcdAngle->setFrameStyle(QFrame::NoFrame);
//	lcdAngle->setSegmentStyle(QLCDNumber::Flat);
//	lcdDistance = new QLCDNumber();
//	lcdDistance->setFrameStyle(QFrame::NoFrame);
//	lcdDistance->setSegmentStyle(QLCDNumber::Flat);
//	lcdSpeed = new QLCDNumber();
//	lcdSpeed->setFrameStyle(QFrame::NoFrame);
//	lcdSpeed->setSegmentStyle(QLCDNumber::Flat);

	// CheckBox
//	checkNegativeAngle = new QCheckBox("Negative Angle");

	// PushButtons
	btnConnect = new QPushButton(QIcon(":connect"), "Connect");
	btnAbort = new QPushButton(QIcon(":cross"), "Abort");
	btnAbort->setEnabled(false);

	// Port Connections
	cbPort = new QComboBox();
	cbPort->addItem("/dev/ttyUSB0");
	cbPort->addItem("/dev/ttyUSB1");
	cbPort->addItem("/dev/ttyUSB2");
	cbPort->addItem("/dev/ttyUSB3");

//	QHBoxLayout *hbox = new QHBoxLayout;
//	hbox->addWidget(new QLabel("Angle:"), 1,0);
//	hbox->addWidget(checkNegativeAngle);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(joystick);
//	vbox->addWidget(new QLabel("Speed (mm per second:)"), 4,0);
//	vbox->addWidget(lcdSpeed);
//	vbox->addWidget(sliderSpeed);
//	vbox->addLayout(hbox);
//	vbox->addWidget(lcdAngle);
//	vbox->addWidget(sliderAngle);
	vbox->addWidget(btnConnect);
	vbox->addWidget(btnAbort);
	vbox->addWidget(cbPort);
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
	sliderDistance = new QSlider();
	layoutTopLeftBottomMaster->addWidget(sliderDistance, 0, 1);
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
	layoutTopLeftBottomMaster->addWidget(new QLabel("Distance:"), 0, 2);
	layoutTopLeftBottomMaster->addWidget(lcdDistance, 0, 3);

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
	cbTask->addItem("Avoid Obstacles");
	cbTask->addItem("Rotate 90");
	cbTask->addItem("Rotate 360");
	cbTask->addItem("Straight Path Move");
	cbTask->addItem("Wall Follower");
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

