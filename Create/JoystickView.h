/*
 *  JoystickView.h
 *
 *  The JoystickView class can be used to control the various Controllers. It
 *  provides a widget which can be controlled by the mouse and provides all
 *  the appropriate slots and signals. The joystick has two yokes: x and y,
 *  which in turn can be used for control. Furthermore, it can be customized
 *  to force exclusive yokes � a mode where only one of the yokes can be moved
 *  at a time. In addition, the JoystickView class also provides control over its
 *  acceleration and deceleration, a useful feature for gently controlling a robot.
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

#ifndef JOYSTICKVIEW_H_
#define JOYSTICKVIEW_H_

#include <QWidget>
#include <QPainter>
#include <QTimer>

class JoystickView : public QWidget {

	Q_OBJECT

private:
	double yokeX;
	double yokeY;
	bool mouseDown;
	QTimer *timer;

public:
	enum JoystickMode { JOYSTICK_MODE_FREE, JOYSTICK_MODE_LOCKED } mode;
	enum JoystickAcceleration { JOYSTICK_ACCELERATION_INSTANT, JOYSTICK_ACCELERATION_SLOWDOWN } acceleration;

public:
	JoystickView(JoystickMode mode, QWidget *parent = 0);
	virtual ~JoystickView();
	double getX();
	double getY();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event);

public slots:
	void paint();
	void accelerate();

signals:
	void yokeChanged(double yokeX, double yokeY);
};

#endif /* JOYSTICKVIEW_H_ */
