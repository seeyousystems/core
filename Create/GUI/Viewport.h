/*
 *  Viewport.h
 *
 *  The Viewport class is a very important part of the emss user interface.
 *  A Viewport has the ability to display any emss Map as well as the Maps
 *  Objects. In addition to displaying Maps, a Viewport can accept actions
 *  from the user and provides the necessary tools for navigating the Maps,
 *  such as scrolling. A Viewport allows multiple Maps to be layered on-top
 *  of each other, and automatically generates the proper GUI elements for
 *  hiding and showing specific Maps. Core Maps can have multiple viewports
 *  attached to it, allowing a user to create a set of specialized Viewports
 *  to his liking. Viewports also allow a set of actions to be added to its
 *  toolbar. When the user clicks an action, the viewport forwards this
 *  information back to the connected application which can in turn act on
 *  the given action. Other features such as auto focus and auto refresh
 *  make the Viewport a very flexible widget. The drawing routines have
 *  been specifically designed to run in a separate thread space,
 *  interfering with the emss Core as little as possible.
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

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include <QWidget>
#include <QGLWidget>
#include <QPainter>
#include <QList>
#include <QMap>
#include <QGridLayout>
#include <QScrollBar>
#include <QMenu>
#include <QToolBar>
#include <QTimer>
#include <QSlider>
#include <QReadWriteLock>
#include <QIcon>

#include "../Map/Map.h"
#include "../Map/MapMeta.h"

#include "../GUI/DrawCanvas.h"
#include "../GUI/DrawCanvasGL.h"

class Viewport : public QWidget
{
	Q_OBJECT

private:
	double maxMapWidth;		// pixel
	double maxMapHeight;	// pixel
	QReadWriteLock lock;

public:
	QGridLayout			*gridLayout;
	QScrollBar			*scrollHorizontal;
	QScrollBar			*scrollVertical;
	QToolBar			*toolbar;
	QWidget				*canvas;
	QSlider				*sliderZoom;

	QMenu *menuMain;
		QAction *actionAntiAlias;
		QAction *actionAutoUpdate;
		QAction *actionAutoFocus;
		QMenu *menuMaps;

	QRect				view;
	QList<Map*>			*maps;
	QList<MapMeta*>		*mapMetas;

	int					scale;
	bool				antiAlias;
	QAction				*selectedAction;
	QTimer				*timerAutoUpdate;
	int					previousAutoFocusX;
	int					previousAutoFocusY;

public:
	Viewport(int scale, int autoUpdateInterval, bool openGL, QWidget *parent = 0);
	virtual ~Viewport();
	void moveMapUp();
	void moveMapDown();
	void moveMapLeft();
	void moveMapRight();
	void registerMap(Map* map);
	void unregisterAllMaps();
	void setAntiAlias(bool value);
	void addToolbarAction(QString title, QString value, QIcon icon = QIcon());
	void addToolbarSeparator();
	void setScale(long scale);


protected:
	void keyPressEvent(QKeyEvent *event);
	void resizeEvent(QResizeEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void closeEvent(QCloseEvent *event);
	double getZoom();

public slots:
	void canvasClicked(QMouseEvent *event);
	void drawMaps(QPainter *painter);
	void paintAndShow();
	void resizeScrollbars();
	void menuAction(QAction *action);
	void toolbarAction(QAction *action);
	void focusOnPoint(long x, long y);
	void autoFocus(long x, long y);
	void resetToolbar();
	void zoomChanged();
	void setNaturalZoom();

signals:
	void leftClick();
	void statusUpdate(QString status);
	void toolbarAction(Viewport *viewport, QString value, long x, long y);
	void closed(Viewport *viewport);
};


#endif /*VIEWPORT_H_*/
