/*
 *  DrawCanvasGL.h
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

#ifndef DRAWCANVASGL_H_
#define DRAWCANVASGL_H_

#include <QGLWidget>
#include <QPainter>

class DrawCanvasGL : public QGLWidget
{
	Q_OBJECT

public:
	DrawCanvasGL(QWidget *parent = 0) : QGLWidget(parent) {};
	virtual ~DrawCanvasGL() {};

protected:
	void paintEvent(QPaintEvent *event) {
		 QPainter painter;
		 painter.begin(this);
		 emit draw(&painter);
		 painter.end();
	};

	void mousePressEvent(QMouseEvent *event) {
		emit mouseClick(event);
	};

signals:
	void draw(QPainter *painter);
	void mouseClick(QMouseEvent *event);

};

#endif /* DRAWCANVASGL_H_ */
