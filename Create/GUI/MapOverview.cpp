/*
 *  MapOverview.cpp
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

#include "MapOverview.h"

#include <QPainter>
#include <QRect>
#include <QPen>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolBar>
#include <QList>

#include "../Library/Debug.h"
#include "../Library/Util.h"
#include "../GUI/DrawCanvas.h"
#include "../GUI/DrawCanvasGL.h"


MapOverview::MapOverview(int width, int height, int scale, int autoUpdateInterval, bool openGL, QWidget *parent) : QWidget(parent)
{
	// Init
	this->bitmap = NULL;
	canvasWidth = width;
	canvasHeight = height;
	canvasScale = scale;
	maps = new QList<Map*>();
	mapMetas = new QList<MapMeta*>();

	antiAlias = false;
	selectedAction = NULL;
	timerAutoUpdate = new QTimer();
	timerAutoUpdate->setInterval(autoUpdateInterval);
	this->setLayout(new QVBoxLayout());
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowTitle("emss Map Overview");

	// Build menu
	menuMain = new QMenu(this);
	actionAntiAlias = new QAction("Anti Alias", menuMain);
	actionAntiAlias->setCheckable(true);
	actionAntiAlias->setChecked(antiAlias);
	menuMain->addAction(actionAntiAlias);
	actionAutoUpdate = new QAction("Auto Update", menuMain);
	actionAutoUpdate->setCheckable(true);
	actionAutoUpdate->setChecked(false);
	menuMain->addAction(actionAutoUpdate);
	menuMaps = new QMenu("Maps");
	menuMain->addMenu(menuMaps);

	// Canvas
	//TODO: OpenGL doesn't work...?
	if(openGL) 	Debug::warning("[MapOverview] openGL is not supported!");
	if(openGL)	canvas = new DrawCanvasGL();
	else		canvas = new DrawCanvas();
	canvas->setMinimumSize(300,200);
	this->layout()->addWidget(canvas);

	// Other GUI elements
	btnRefresh = new QPushButton(QIcon(":arrow_refresh"), "Re-compile Overview");
	this->layout()->addWidget(btnRefresh);

	// Register events
	this->setFocusPolicy(Qt::StrongFocus);
	this->setMouseTracking(true);

	// Connections
	connect(menuMain, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(menuMaps, SIGNAL(triggered(QAction*)), this, SLOT(menuAction(QAction*)));
	connect(timerAutoUpdate, SIGNAL(timeout()), this, SLOT(compileAndShow()));
	connect(btnRefresh, SIGNAL(clicked()), this, SLOT(compileAndShow()));
	connect(canvas,SIGNAL(draw(QPainter*)), this, SLOT(drawOverview(QPainter*)));

	compileAndShow();
}

MapOverview::~MapOverview()
{
	timerAutoUpdate->stop();
	delete timerAutoUpdate;
	delete maps;
	delete mapMetas;
	if(bitmap) delete bitmap;
}

void MapOverview::registerMap(Map* map, bool enabled) {

	maps->append(map);
	mapMetas->append(new MapMeta(enabled));

	QAction *actionShowHideMap = new QAction(map->name, menuMaps);
	actionShowHideMap->setData("map");
	actionShowHideMap->setCheckable(true);
	actionShowHideMap->setChecked(enabled);
	menuMaps->addAction(actionShowHideMap);
}



void MapOverview::drawOverview(QPainter *painter) {
	painter->fillRect(0, 0, this->width(), this->height(), Qt::black);
	painter->setRenderHint(QPainter::Antialiasing, antiAlias);
	if(bitmap) painter->drawImage(QRect(0,0,this->width(),this->height()), *bitmap, bitmap->rect(), Qt::ThresholdDither);
}

void MapOverview::compileOverview() {


	// Create a new canvas to save results...
	if(!bitmap) bitmap = new QImage(canvasWidth/canvasScale, canvasHeight/canvasScale, QImage::Format_ARGB32);

	// Init painting region
	this->view.setX(0);
	this->view.setY(0);
	this->view.setWidth(bitmap->width());
	this->view.setHeight(bitmap->height());

	// Init painter and background
	QPainter painter(bitmap);
	painter.fillRect(bitmap->rect(), Qt::black);
	painter.setRenderHint(QPainter::Antialiasing, antiAlias);

	// Draw all maps :)
	for(int i = 0; i < this->maps->count(); i++) {
		// Paint the map. Note: Map is responsible for painting it's objects
		if(mapMetas->at(i)->show == true) {
			Util::resetPainter(painter, 1.0);
			maps->at(i)->paint(painter, this->view);
		}
	}

	// Make disabled effect
	if(this->isEnabled() == false) {
		painter.setOpacity(0.5);
		painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
	}
}

void MapOverview::compileAndShow() {
	compileOverview();
	canvas->repaint();
}

void MapOverview::contextMenuEvent(QContextMenuEvent *event)
{
	menuMain->exec(event->globalPos());
}

void MapOverview::menuAction(QAction *action) {
	if(action == actionAntiAlias) {

		antiAlias = actionAntiAlias->isChecked();

	} else if(action == actionAutoUpdate) {

		if(actionAutoUpdate->isChecked() == true) 	timerAutoUpdate->start();
		else										timerAutoUpdate->stop();

	} else if(action->data() == "map") {

		// This is a map show/hide action, find the correct one and show/hide it...
		for(int i = 0; i < this->maps->count(); i++) {
			if(maps->at(i)->name == action->text()) {
				mapMetas->at(i)->show = action->isChecked();
			}
		}

	}

}

void MapOverview::setAntiAlias(bool value) {
	antiAlias = value;
	actionAntiAlias->setChecked(antiAlias);
}


