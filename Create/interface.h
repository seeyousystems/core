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

	// CheckBox
	QCheckBox *checkNegativeAngle;

	// Block Controller
	QGroupBox *frameTopLeftBottom;

	// Gui Global QGroupBox
	QGroupBox *exclusiveBox;
	QGroupBox *blockBox;

	QWidget *tabBlockDriveController;
	QTabWidget *tabNavigation;
	QPushButton *btnBlockDriveForward;
	QPushButton *btnBlockDriveBackward;
	QPushButton *btnBlockDriveTurnLeft;
	QPushButton *btnBlockDriveTurnRight;

	QGroupBox *createControlsExclusiveGroup();
	QGroupBox *createButtonsExclusiveGroup();
	QGroupBox *createBlockControllerExclusiveGroup();


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

};

#endif // INTERFACE_H
