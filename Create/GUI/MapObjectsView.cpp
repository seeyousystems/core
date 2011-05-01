/*
 *  MapObjectsView.cpp
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

#include "MapObjectsView.h"

#include "../create.h"
#include "../Library/Util.h"
#include "../Library/HeapLogger.h"
#include "../Map/ObjectMap.h"
#include "../Map/MapObject/MapObject.h"

#include <QVBoxLayout>
#include <QIcon>

MapObjectsView::MapObjectsView(Create *create, QWidget *parent) : QWidget(parent) {

	// Init
	this->objectMap = NULL;
	this->setAttribute(Qt::WA_DeleteOnClose, true);

	// Gui stuff...
	this->setWindowTitle("emss Map Objects");
	this->setLayout(new QVBoxLayout());
	this->setWindowIcon(QIcon(":map"));

	// MapObject items
	this->layout()->addWidget(new QLabel("Map Objects:"));
	lstMapObjects = new QListWidget();
	this->layout()->addWidget(lstMapObjects);

	registerCore(create);

	// Buttons
	btnClose = new QPushButton(QIcon(":cancel"), "Close");
	this->layout()->addWidget(btnClose);

	// Connections
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(lstMapObjects, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
}

MapObjectsView::~MapObjectsView() {
}


void MapObjectsView::refreshMapObjects() {
	lstMapObjects->clear();
	if(!objectMap) return;

	QList<MapObject*>::const_iterator i = objectMap->objects->begin();
	while (i != objectMap->objects->constEnd()) {
		MapObject *obj = *i;
		QListWidgetItem *item = new QListWidgetItem(QIcon(":map_magnify"), obj->name);
		lstMapObjects->addItem(item);
		item->setCheckState(Qt::Checked);
		item->setData(Qt::UserRole, (int)obj); // Why can a listitem not be connected with a QObject? Fine! I will just cast the pointer to int (yes, a QVariant) and then backcast it later...
		i++;
	}
}


void MapObjectsView::itemChanged(QListWidgetItem *item) {
	if(!objectMap) return;

	// Extract the object pointer and update it's visibility
	MapObject *obj = (MapObject*)(item->data(Qt::UserRole).toInt());
	if(obj) obj->visible = ( item->checkState() == Qt::Checked ? true : false );
}

void MapObjectsView::registerCore(Create *create) {
	if(create)	objectMap = create->objectMap;
	else		objectMap = NULL;

	refreshMapObjects();

	this->setEnabled(create && create->isConnected());
}







