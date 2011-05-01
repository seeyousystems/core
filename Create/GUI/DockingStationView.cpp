/*
 *  DockingStationView.cpp
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

#include "DockingStationView.h"

#include "../Library/Util.h"
#include "../Library/HeapLogger.h"
#include "../Library/DockingStation.h"

#include "../create.h"

#include "../Map/ObjectMap.h"
#include "../Map/MapObject/MapObject.h"

#include <QVBoxLayout>
#include <QIcon>

DockingStationView::DockingStationView(Create *create, QWidget *parent) : QWidget(parent) {

	// Init
	this->dockingStation = NULL;

	// Gui stuff...
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowTitle("emss Docking Station");
	this->setWindowIcon(QIcon(":house"));
	this->setLayout(new QVBoxLayout());

	// Check boxes
	this->layout()->addWidget(new QLabel("Docking Station:"));
	chkEnabled = new QCheckBox("Enabled");
	this->layout()->addWidget(chkEnabled);
	this->layout()->addWidget(new QLabel("Signals:"));
	chkRedBeamEnabled = new QCheckBox("Red Beam");
	this->layout()->addWidget(chkRedBeamEnabled);
	chkGreenBeamEnabled = new QCheckBox("Green Beam");
	this->layout()->addWidget(chkGreenBeamEnabled);
	chkForceFieldEnabled = new QCheckBox("Force Field");
	this->layout()->addWidget(chkForceFieldEnabled);
	chkInterferenceEnabled = new QCheckBox("Interference");
	this->layout()->addWidget(chkInterferenceEnabled);

	// Buttons
	btnClose = new QPushButton(QIcon(":cancel"), "Close");
	this->layout()->addWidget(btnClose);

	registerCore(create);

	// Connections
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(chkEnabled, SIGNAL(stateChanged(int)), this, SLOT(checkChanged()));
	connect(chkRedBeamEnabled, SIGNAL(stateChanged(int)), this, SLOT(checkChanged()));
	connect(chkGreenBeamEnabled, SIGNAL(stateChanged(int)), this, SLOT(checkChanged()));
	connect(chkForceFieldEnabled, SIGNAL(stateChanged(int)), this, SLOT(checkChanged()));
	connect(chkInterferenceEnabled, SIGNAL(stateChanged(int)), this, SLOT(checkChanged()));
}

DockingStationView::~DockingStationView() {

}


void DockingStationView::checkChanged() {
	if(dockingStation) {
		dockingStation->enabled = chkEnabled->isChecked();
		chkRedBeamEnabled->setEnabled(dockingStation->enabled);
		chkGreenBeamEnabled->setEnabled(dockingStation->enabled);
		chkForceFieldEnabled->setEnabled(dockingStation->enabled);
		chkInterferenceEnabled->setEnabled(dockingStation->enabled);
		dockingStation->redBeamEnabled = chkRedBeamEnabled->isChecked();
		dockingStation->greenBeamEnabled = chkGreenBeamEnabled->isChecked();
		dockingStation->forceFieldEnabled = chkForceFieldEnabled->isChecked();
		dockingStation->interferenceEnabled = chkInterferenceEnabled->isChecked();
	}
}

void DockingStationView::registerCore(Create *create) {

	// Register component
	if(create) 	dockingStation = create->dockingStation;
	else		dockingStation = NULL;

	// Refresh checks et cetera
	if(dockingStation) {
		chkEnabled->setChecked(dockingStation->enabled);
		chkRedBeamEnabled->setChecked(dockingStation->redBeamEnabled);
		chkGreenBeamEnabled->setChecked(dockingStation->greenBeamEnabled);
		chkForceFieldEnabled->setChecked(dockingStation->forceFieldEnabled);
		chkInterferenceEnabled->setChecked(dockingStation->interferenceEnabled);
	}

	this->setEnabled(create && create->isConnected());
}
