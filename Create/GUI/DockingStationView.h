/*
 *  DockingStationView.h
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

#ifndef DOCKINGSTATIONVIEW_H_
#define DOCKINGSTATIONVIEW_H_

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

class DockingStation;
class Core;

class DockingStationView : public QWidget {

	Q_OBJECT

private:

	DockingStation	*dockingStation;

	QCheckBox *chkEnabled;
	QCheckBox *chkRedBeamEnabled;
	QCheckBox *chkGreenBeamEnabled;
	QCheckBox *chkForceFieldEnabled;
	QCheckBox *chkInterferenceEnabled;
	QPushButton *btnClose;

public:
	DockingStationView(Create *create, QWidget *parent = 0);
	virtual ~DockingStationView();

private slots:
	void registerCore(Create *create);
	void checkChanged();

};

#endif /* DOCKINGSTATIONVIEW_H_ */
