/*
 *  JoystickView.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "JoystickView.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <stdio.h>

JoystickView::JoystickView(JoystickView::JoystickMode mode, QWidget *parent) : QWidget(parent) {

	yokeX = 0.0;
	yokeY = 0.0;
	mouseDown = false;
	this->mode = mode;
	acceleration = JoystickView::JOYSTICK_ACCELERATION_INSTANT;
	timer = new QTimer();
	timer->setInterval(10);
	this->setMinimumSize(150,150);

	connect(timer, SIGNAL(timeout()), this, SLOT(accelerate()));
}

JoystickView::~JoystickView() {
	delete timer;
}

void JoystickView::paintEvent(QPaintEvent *event)
{
	this->paint();
}

void JoystickView::paint() {
	// Init painter and background
	QPainter painter(this);
	//painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::white);

	// Draw crosshair
	QPen pen;
	pen.setWidth(2);
	pen.setColor(Qt::blue);
	painter.setPen(pen);
	if(mode == JoystickView::JOYSTICK_MODE_FREE) {
		painter.drawEllipse(10, 10, this->width()-20, this->height()-20);
		painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
		painter.drawLine(this->width()/2, 0, this->width()/2, this->height());
	} else if(mode == JoystickView::JOYSTICK_MODE_LOCKED) {
		painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
		painter.drawLine(this->width()/2, 0, this->width()/2, this->height());
	}


	// Draw stick
	painter.drawLine(
			this->width()/2,
			this->height()/2,
			(int)((this->width()/2) + (yokeX*(this->width()/2))),
			(int)((this->height()/2) - (yokeY*(this->height()/2))));

	// Draw ball
	pen.setWidth(3);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	painter.setBrush(QBrush(Qt::white));
	painter.drawEllipse(
			(int)((this->width()/2) + (yokeX*(this->width()/2)) - 20),
			(int)((this->height()/2) - (yokeY*(this->height()/2)) - 20),
			40,
			40 );

	// Make disabled effect
	if(this->isEnabled() == false) {
		painter.setOpacity(0.5);
		painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
	}

}

void JoystickView::accelerate() {

	double step = 0.01;

	// Fix
	if (yokeX > -step && yokeX < +step) yokeX = 0.0;
	if (yokeY > -step && yokeY < +step) yokeY = 0.0;

	// Are we done with decceleration?
	if (yokeX == 0.0 && yokeY == 0.0) {

		timer->stop();
		emit yokeChanged(yokeX, yokeY);

	} else {

		if		(yokeX > 0.0) yokeX -= step;
		else if	(yokeX < 0.0) yokeX += step;
		if		(yokeY > 0.0) yokeY -= step;
		else if	(yokeY < 0.0) yokeY += step;

		emit yokeChanged(yokeX, yokeY);

		//this->paint();
		this->repaint();

	}
}

void JoystickView::mousePressEvent(QMouseEvent *event)
{
	timer->stop();
	mouseDown = true;
	mouseMoveEvent(event);
}

void JoystickView::mouseReleaseEvent(QMouseEvent *event)
{
	mouseDown = false;

	if(acceleration == JoystickView::JOYSTICK_ACCELERATION_INSTANT) {

		yokeX = 0.0;
		yokeY = 0.0;
		emit yokeChanged(yokeX, yokeY);

		//this->paint();
		this->repaint();

	} else {

		timer->start();

	}
}

void JoystickView::mouseMoveEvent(QMouseEvent *event) {
	if(mouseDown) {

		// Solve for positionX: (this->width()/2) + (positionX*(this->width()/2)) = pos().x()
		yokeX = ( (double)event->pos().x() - ((double)this->width()/2.0) ) / ((double)this->width()/2.0);
		yokeY = -( (double)event->pos().y() - ((double)this->height()/2.0) ) / ((double)this->height()/2.0);

		printf("%f, %f\t\t%f, %f\n", (double)event->pos().x(), (double)event->pos().y(), (double)this->width()/2.0, (double)this->height()/2.0);

		// Fix
		if(mode == JoystickView::JOYSTICK_MODE_FREE) {
			if(yokeX < -1.0) yokeX = -1.0;
			if(yokeX > +1.0) yokeX = 1.0;
			if(yokeY < -1.0) yokeY = -1.0;
			if(yokeY > +1.0) yokeY = 1.0;
		} else if(mode == JoystickView::JOYSTICK_MODE_LOCKED) {
			if(yokeX >= 0 && yokeY >= 0) {
				if(yokeX > yokeY) yokeY = 0.0;
				else yokeX = 0.0;
			} else if(yokeX >= 0 && yokeY < 0) {
				if(yokeX < -yokeY) yokeX = 0.0;
				else yokeY = 0.0;
			} else if(yokeX < 0 && yokeY >= 0) {
				if(-yokeX > yokeY) yokeY = 0.0;
				else yokeX = 0.0;
			} else if(yokeX < 0 && yokeY < 0) {
				if(-yokeX < -yokeY) yokeX = 0.0;
				else yokeY = 0.0;
			}
		}

		emit yokeChanged(yokeX, yokeY);

		//this->paint();
		this->repaint();
	}
}

void JoystickView::keyPressEvent(QKeyEvent *event)
{

}

void JoystickView::resizeEvent(QResizeEvent *event) {

}

double JoystickView::getX() {
	return yokeX;
}

double JoystickView::getY() {
	return yokeY;
}

