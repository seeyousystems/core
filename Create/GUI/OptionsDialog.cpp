/*
 *  OptionsDialog.cpp
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

#include "OptionsDialog.h"

#include <QVBoxLayout>
#include <QIcon>

OptionsDialog::OptionsDialog(QString options, QString description, QString title, QWidget *parent) : QDialog() {

	// Gui stuff...
	this->setWindowTitle(title);
	this->setLayout(new QVBoxLayout());
	this->setWindowIcon(QIcon(":emss"));

	label = new QLabel(description);
	this->layout()->addWidget(label);

	lstOptions = new QListWidget();
	lstOptions->addItems(options.split('|'));
	this->layout()->addWidget(lstOptions);

	// Connections
	connect(lstOptions, SIGNAL(itemClicked (QListWidgetItem*)), this, SLOT(acceptAndClose()));
}

OptionsDialog::~OptionsDialog() {

}

void OptionsDialog::acceptAndClose() {
	setResult(QDialog::Accepted);
	result = lstOptions->currentItem()->text();
	close();
}

QString OptionsDialog::choose(QString options, QString description, QString title) {
	OptionsDialog *dialog = new OptionsDialog(options, description, title);
	dialog->exec();
	QString result = dialog->result;
	delete dialog;
	return result;
}



