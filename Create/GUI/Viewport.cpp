/*
 *  Viewport.cpp
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

// BUG: When zooming, the focus changes (traverses). The scroll bars need to be adjusted at
// the same time...

#include "Viewport.h"

#include <QPainter>
#include <QRect>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolBar>
#include <QList>
#include <QIcon>

#include "../Library/Debug.h"
#include "../Library/Util.h"


Viewport::Viewport(int scale, int autoUpdateInterval, bool openGL, QWidget *parent) : QWidget(parent)
{
	// Init
	this->scale = scale;
	sliderZoom = NULL;
	maps = new QList<Map*>();
	mapMetas = new QList<MapMeta*>();
	antiAlias = true;
	selectedAction = NULL;
	timerAutoUpdate = new QTimer();
	timerAutoUpdate->setInterval(autoUpdateInterval);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowIcon(QIcon(":eye"));
	this->setWindowTitle("emss Viewport");
	maxMapWidth = 0.0;
	maxMapHeight = 0.0;

	// Toolbar
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0,0,0,0);
	toolbar = new QToolBar();
	toolbar->setAutoFillBackground(true);
	this->layout()->addWidget(toolbar);

	// Add scrollbars...
	gridLayout = new QGridLayout();
	gridLayout->setContentsMargins(0,0,0,0);
	QWidget *gridHolder = new QWidget();
	gridHolder->setLayout(gridLayout);
	this->layout()->addWidget(gridHolder);
	scrollHorizontal = new QScrollBar(Qt::Horizontal);
	scrollHorizontal->setMinimum(0);
	scrollHorizontal->setMaximum(1000);
	scrollHorizontal->setSingleStep(10);
	scrollVertical = new QScrollBar(Qt::Vertical);
	scrollVertical->setMinimum(0);
	scrollVertical->setMaximum(1000);
	scrollVertical->setSingleStep(10);
	gridLayout->addWidget(scrollHorizontal,3,1);
	gridLayout->addWidget(scrollVertical,0,2);

	// Canvas
	if(openGL)	canvas = new DrawCanvasGL();
	else		canvas = new DrawCanvas();
	gridLayout->addWidget(canvas, 0,1);

	// Zoom
	if(openGL) {
		// Only allow zooming if open gl. Software render is just not built for this...
		sliderZoom = new QSlider(Qt::Horizontal);
		setNaturalZoom();
		gridLayout->addWidget(sliderZoom,2,1);
	}

	// Build menu
	menuMain = new QMenu(this);
	QAction *actionLabelNativeOrGL = new QAction( (openGL ? "openGL Render" : "Native Render"), menuMain);
	actionLabelNativeOrGL->setEnabled(false);
	menuMain->addAction(actionLabelNativeOrGL);
	actionAntiAlias = new QAction("Anti Alias", menuMain);
	actionAntiAlias->setCheckable(true);
	actionAntiAlias->setChecked(antiAlias);
	menuMain->addAction(actionAntiAlias);
	actionAutoUpdate = new QAction("Auto Update", menuMain);
	actionAutoUpdate->setCheckable(true);
	actionAutoUpdate->setChecked(true);
	menuMain->addAction(actionAutoUpdate);
	actionAutoFocus = new QAction("Auto Focus", menuMain);
	actionAutoFocus->setCheckable(true);
	actionAutoFocus->setChecked(true);
	menuMain->addAction(actionAutoFocus);
	menuMaps = new QMenu("Maps");
	menuMain->addMenu(menuMaps);

	// Register events
	this->setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);

	// Connections
	connect(menuMain, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(menuMaps, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(toolbar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolbarAction(QAction*)));
	connect(timerAutoUpdate, SIGNAL(timeout()), this, SLOT(paintAndShow()));
	connect(canvas,SIGNAL(draw(QPainter*)), this, SLOT(drawMaps(QPainter*)));
	connect(canvas,SIGNAL(mouseClick(QMouseEvent*)), this, SLOT(canvasClicked(QMouseEvent*)));
	if(sliderZoom) connect(sliderZoom, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged()));

	// Start timers...
	resizeScrollbars();
	timerAutoUpdate->start();
}

Viewport::~Viewport()
{
	timerAutoUpdate->stop();
	delete timerAutoUpdate;
	delete maps;
	delete mapMetas;
}

void Viewport::registerMap(Map* map) {

	lock.lockForWrite(); {

		maps->append(map);
		mapMetas->append(new MapMeta(true));

		QAction *actionShowHideMap = new QAction(map->name, menuMaps);
		actionShowHideMap->setData("map");
		actionShowHideMap->setCheckable(true);
		actionShowHideMap->setChecked(true);
		menuMaps->addAction(actionShowHideMap);

		// Reload max sizes
		maxMapWidth = 0.0;
		maxMapHeight = 0.0;
		for(int i = 0; i < this->maps->count(); i++) {
			if(maps->at(i)->width() > maxMapWidth) 		maxMapWidth = maps->at(i)->width();
			if(maps->at(i)->height() > maxMapHeight) 	maxMapHeight = maps->at(i)->height();
		}
		maxMapWidth = maxMapWidth / scale; // convert back to pixel
		maxMapHeight = maxMapHeight / scale; // convert back to pixel
		scrollHorizontal->setMaximum((int)(maxMapWidth));
		scrollVertical->setMaximum((int)(maxMapHeight));

	} lock.unlock();
}

void Viewport::unregisterAllMaps() {

	lock.lockForWrite(); {

		maps->clear();
		mapMetas->clear();
		menuMaps->clear();

	} lock.unlock();
}

void Viewport::resizeScrollbars()
{
	// Deprecated, currently no longer used
}



void Viewport::paintAndShow() {
	canvas->repaint();
}

void Viewport::drawMaps(QPainter *painter) {

	if(!this->isVisible() || !this->isEnabled()) return; // Don't bother drawing if not visible...

	// Calculate zoom and other commonly used values
	double zoom = getZoom();
	double rzoom = 1.0 / zoom; // the reversed zoom
	double scrollX = (double)scrollHorizontal->value() / (double)(scrollHorizontal->maximum());
	double scrollY = (double)scrollVertical->value() / (double)(scrollVertical->maximum());

	// Init painting region
	this->view.setX((int)( (maxMapWidth-view.width()) * scrollX ));
	this->view.setY((int)( maxMapHeight-view.height() - ((maxMapHeight-view.height()) * scrollY) ));
	this->view.setWidth((int)( canvas->width() * rzoom ));
	this->view.setHeight((int)( canvas->height() * rzoom));

	// Init painter and background
	painter->setRenderHint(QPainter::Antialiasing, antiAlias);
	painter->fillRect(0, 0, canvas->width(), canvas->height(), Qt::black);

	// Set the clipping region
	painter->setClipRect(0,0,canvas->width(),canvas->height());

	// Draw all maps :)

	lock.lockForRead(); {

		for(int i = 0; i < this->maps->count(); i++) {
			// Paint the map. Note: Map is responsible for painting it's objects
			if(mapMetas->at(i)->show == true) {

				// Reset the painter, so that we have a clean one for each map
				Util::resetPainter(*painter);

				// Do transform for zoom, if not 1.0
				if(zoom != 1.0) {
					QTransform t;
					t.scale(zoom,zoom);
					painter->setWorldTransform(t);
				}

				// Paint the actual map
				maps->at(i)->paint(*painter, this->view);

			}
		}

	} lock.unlock();

	// Make disabled effect
	if(this->isEnabled() == false) {
		painter->setOpacity(0.5);
		painter->fillRect(0, 0, canvas->width(), canvas->height(), Qt::white);
	}
}

void Viewport::canvasClicked(QMouseEvent *event)
{
	// Get the actual point clicked
	double rzoom = 1.0/getZoom();
	long x = (long)((double)(event->x()*rzoom)) + view.x();
	long y = view.height() - (long)((double)(event->y()*rzoom)) + view.y();

	// Go through all actions, checking if we have some pressed so we can
	// process it on the point clicked...
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true) {
			// Check if we need to emit a signal or process it now
			if(actions.at(i)->data().toString().replace("[instant]","") == "focusviewport") {
				focusOnPoint(x*scale,y*scale);
			} else {
				emit toolbarAction(this, actions.at(i)->data().toString().replace("[instant]",""), x, y);
			}
		}
	}
}



void Viewport::keyPressEvent(QKeyEvent *event)
{
	switch(event->key()) {
		case Qt::Key_Up: 		moveMapUp(); break;
		case Qt::Key_Down: 		moveMapDown(); break;
		case Qt::Key_Left: 		moveMapLeft(); break;
		case Qt::Key_Right: 	moveMapRight(); break;
		default:				QWidget::keyPressEvent(event);
	}
}

void Viewport::resizeEvent(QResizeEvent *event) {
	this->resizeScrollbars();
}

void Viewport::moveMapUp() {
	scrollVertical->setValue(scrollVertical->value() - 20);
}

void Viewport::moveMapDown() {
	scrollVertical->setValue(scrollVertical->value() + 20);
}

void Viewport::moveMapLeft() {
	scrollHorizontal->setValue(scrollHorizontal->value() - 20);
}

void Viewport::moveMapRight() {
	scrollHorizontal->setValue(scrollHorizontal->value() + 20);
}

void Viewport::contextMenuEvent(QContextMenuEvent *event)
{
	menuMain->exec(event->globalPos());
}

void Viewport::menuAction(QAction *action) {
	if(action == actionAntiAlias) {

		antiAlias = actionAntiAlias->isChecked();

	} else if(action == actionAutoUpdate) {

		if(actionAutoUpdate->isChecked() == true) 	timerAutoUpdate->start();
		else										timerAutoUpdate->stop();

	} else if(action->data() == "map") {

		// This is a map show/hide action, find the correct one and show/hide it...
		lock.lockForRead(); {
			for(int i = 0; i < this->maps->count(); i++) {
				if(maps->at(i)->name == action->text()) {
					mapMetas->at(i)->show = action->isChecked();
				}
			}
		} lock.unlock();
	}

}

void Viewport::setAntiAlias(bool value) {
	antiAlias = value;
	actionAntiAlias->setChecked(antiAlias);
}

void Viewport::addToolbarAction(QString title, QString value, QIcon icon) {
	QAction *action;

	if ( icon.isNull() ){
		action = toolbar->addAction(title);
	}
	else {
		action = toolbar->addAction(icon, title);
	}

	action->setCheckable(true);
	action->setData(value);
}

void Viewport::addToolbarSeparator(){
	toolbar->addSeparator();
}

void Viewport::toolbarAction(QAction *action) {
	// Set the correct canvas cursor
	bool somethingChecked = false;
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true && !action->data().toString().endsWith("[instant]")) {
			somethingChecked = true;
			break;
		}
	}
	if(somethingChecked == true) 	canvas->setCursor(Qt::CrossCursor);
	else 							canvas->setCursor(Qt::ArrowCursor);

	if(action->data().toString().endsWith("[instant]")) {
		emit toolbarAction(this, action->data().toString().replace("[instant]",""), 0, 0);
	}
}

void Viewport::focusOnPoint(long x, long y) {
	double px = (x/scale) / maxMapWidth;
	double py = (y/scale) / maxMapHeight;
	scrollHorizontal->setValue((scrollHorizontal->maximum()+view.width())*px - view.width()/2);
	scrollVertical->setValue(scrollVertical->maximum() - (scrollVertical->maximum()+view.height())*py + view.height()/2);
}

void Viewport::resetToolbar() {
	QList<QAction*> actions = toolbar->actions();
	for(int i = 0; i < actions.count(); i++) {
		if(actions.at(i)->isChecked() == true) {
			actions.at(i)->setChecked(false);
		}
	}
	this->setCursor(Qt::ArrowCursor);
}

void Viewport::autoFocus(long x, long y) {
	if(actionAutoFocus->isChecked() == true && (previousAutoFocusX != x || previousAutoFocusY != y)) {
		previousAutoFocusX = x;
		previousAutoFocusY = y;
		focusOnPoint(x, y);
	}
}

void Viewport::closeEvent(QCloseEvent *event) {
	// Signal the close and accept the event
	emit closed(this);
	event->accept();
}

double Viewport::getZoom() {
	if(!sliderZoom) return 1.0;

	// Target range: 0.3 - 3.0
	double p = (double)sliderZoom->value() / (double)sliderZoom->maximum();
	double min = 0.3;
	double max = 3.0;
	return min + p * (max - min);
}

void Viewport::setNaturalZoom() {
	// Target zoom: 1.0
	double min = 0.3;
	double max = 3.0;
	double p = (1.0 - min) / (max - min);
	sliderZoom->setValue(p*sliderZoom->maximum());
}

void Viewport::zoomChanged() {

}

void Viewport::setScale(long scale){
	this->scale = scale;
}





