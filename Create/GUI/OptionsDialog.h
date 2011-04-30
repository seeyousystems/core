/*
 *  OptionsDialog.h
 *
 *  Often a user interface requires some sort of choice from the user.
 *  The Options Dialog class provides an easy to call, blocking static method
 *  choose(options,description,title) which creates a modal dialog of choices
 *  based on the options parameter. The choice in options must separated by
 *  the | character.
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
#ifndef OPTIONSDIALOG_H_
#define OPTIONSDIALOG_H_

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>

class OptionsDialog : public QDialog {

	Q_OBJECT

private:
	QString value;

public:
	QListWidget *lstOptions;
	QLabel		*label;
	QString		result;

public:
	OptionsDialog(QString options, QString description, QString title, QWidget *parent = 0);
	virtual ~OptionsDialog();

public:
	static QString choose(QString options, QString description = "", QString title = "");

public slots:
	void acceptAndClose();

};

#endif /* OPTIONSDIALOG_H_ */
