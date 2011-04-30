/*
 *  WeightEditor.h
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

#ifndef WEIGHTEDITOR_H_
#define WEIGHTEDITOR_H_

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>


class Create;
class Navigation;
class SystemOfWeightsNavigation;

class WeightEditor : public QWidget {

	Q_OBJECT

private:
	SystemOfWeightsNavigation *navigation;

public:
	QListWidget *lstWeights;
	QTextEdit *txtWeightInfo;
	QPushButton *btnMoveUp;
	QPushButton *btnMoveDown;
	QPushButton *btnAdd;
	QPushButton *btnDelete;
	QPushButton *btnActive;
	QPushButton *btnClose;
	int selectedIndex;

public:
	WeightEditor(Create *create, QWidget *parent = 0);
	virtual ~WeightEditor();

public slots:
	void registerCore(Create *create);
	void showWeight(QModelIndex index);
	void addWeight();
	void deleteWeight();
	void activateWeight();
	void moveWeightUp();
	void moveWeightDown();
	void refreshList();
	void setNavigation(Navigation *navigation);

};

#endif /* WEIGHTEDITOR_H_ */
