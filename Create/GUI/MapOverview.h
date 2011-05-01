/*
 *  MapOverview.h
 *
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

#ifndef MAPOVERVIEW_H_
#define MAPOVERVIEW_H_

#include <QWidget>
#include <QPainter>
#include <QList>
#include <QMap>
#include <QGridLayout>
#include <QScrollBar>
#include <QMenu>
#include <QSlider>
#include <QToolBar>
#include <QTimer>
#include <QPushButton>

#include "../Map/Map.h"
#include "../Map/MapMeta.h"


class MapOverview : public QWidget
{
	Q_OBJECT

private:
	QImage *bitmap;

public:

	QMenu *menuMain;
		QAction *actionAntiAlias;
		QAction *actionAutoUpdate;
		QAction *actionAutoFocus;
		QMenu *menuMaps;
	QPushButton *btnRefresh;
	QWidget *canvas;

	QRect				view;
	QList<Map*>			*maps;
	QList<MapMeta*>		*mapMetas;

	bool				antiAlias;
	QAction				*selectedAction;
	QTimer				*timerAutoUpdate;

	int					canvasWidth;
	int					canvasHeight;
	int					canvasScale;

public:
	MapOverview(int width, int height, int scale, int autoUpdateInterval, bool openGL, QWidget *parent = 0);
	virtual ~MapOverview();
	void registerMap(Map* map, bool enabled);
	void setAntiAlias(bool value);


protected:
	void contextMenuEvent(QContextMenuEvent *event);

public slots:
	void drawOverview(QPainter *painter);
	void compileOverview();
	void compileAndShow();
	void menuAction(QAction *action);

signals:

};

#endif /*MAPOVERVIEW_H_*/
