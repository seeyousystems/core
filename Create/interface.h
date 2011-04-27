#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QTextEdit>
#include <QLCDNumber>
#include <QMenuBar>
#include <QGroupBox>
#include <QTabWidget>
#include <QSettings>
#include <QApplication>
#include <QList>
#include <QCloseEvent>
#include <QGridLayout>


#include "create.h"
#include "JoystickView.h"
#include "Library/Debug.h"

#include "GUI/WeightEditor.h"

class QPushButton;

class Interface : public QWidget
{
    Q_OBJECT

private:
	QTextEdit *txtSensorData;
    JoystickView *joystick;
    JoystickView *joystickFluidController;

	QPushButton *btnConnect;
	QPushButton *btnAbort;
	QPushButton *stopOI;

	// Control with QPushButton(s)
	QPushButton *forwardButton;
	QPushButton *stopButton;

	// Kill task button
	QPushButton *killSwitchButton;

	// Port
	QComboBox *cbPort;
	QComboBox *cbTask;
	QComboBox *cbSlowStart;

	// Slider
	QSlider *sliderSpeed;
	QSlider *sliderDistance;
	QSlider *sliderAngle;

	// LCD
	QLCDNumber *lcdDistance;
	QLCDNumber *lcdSpeed;
	QLCDNumber *lcdAngle;

	// LCD for Sensors
	QLCDNumber *lcdLeft;
	QLCDNumber *lcdUpperLeft;
	QLCDNumber *lcdFront;
	QLCDNumber *lcdUpperRight;
	QLCDNumber *lcdRight;

	// CheckBox
	QCheckBox *checkNegativeAngle;

	// Block Controller
	QGroupBox *frameTopLeftBottom;

	// Gui Global QGroupBox
	QGroupBox *exclusiveBox;
	QGroupBox *blockBox;
	QGroupBox *sensorBox;

	QWidget *tabBlockDriveController;
	QTabWidget *tabNavigation;
	QPushButton *btnBlockDriveForward;
	QPushButton *btnBlockDriveBackward;
	QPushButton *btnBlockDriveTurnLeft;
	QPushButton *btnBlockDriveTurnRight;

	QGroupBox *createControlsExclusiveGroup();
	QGroupBox *createButtonsExclusiveGroup();
	QGroupBox *createBlockControllerExclusiveGroup();
	QGroupBox *createSensorExclusiveGroup();

	WeightEditor *weightEditor;



public:
	Create *create;

public:
    Interface(QWidget *parent = 0);
    ~Interface();
    void initBlockController();


public slots:
	void connectDisconnect();
	void createConnected();
	void createDisconnected();

	void currentTask(int index);

	void refreshSensors();
	//void menuAction(QAction *action);
	//void viewportAction(Viewport *viewport, QString value, long x, long y);
	void focusOnPoint(long x, long y);

	void killSwitchFunction();

signals:
	void createConnectionChanged(Create *create); // Emitted when core connects/disconnects.

};

#endif // INTERFACE_H
