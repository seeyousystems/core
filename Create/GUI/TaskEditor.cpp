/*
 *  TaskEditor.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#include "TaskEditor.h"

#include "../Library/Util.h"

#include <QVBoxLayout>
#include <QModelIndexList>

#include <iostream>

TaskEditor::TaskEditor(TaskList *tasks, QWidget *parent) : QWidget(parent) {

	// Init
	this->tasks = tasks;
	timer = new QTimer();

	// Gui stuff...
	this->setWindowTitle("emss Task Editor");
	this->setLayout(new QVBoxLayout());

	lstTasks = new QListWidget();
	this->layout()->addWidget(lstTasks);

	txtTaskInfo = new QTextEdit();
	Util::setTextEditAsLogger(txtTaskInfo);
	this->layout()->addWidget(txtTaskInfo);

	btnStopTask = new QPushButton("Stop Task");
	btnStopTask->setEnabled(false);
	this->layout()->addWidget(btnStopTask);

	btnClose = new QPushButton("Close");
	this->layout()->addWidget(btnClose);

	// Load tasks
	refreshList();

	// Connections
	connect(timer, SIGNAL(timeout()), this, SLOT(refreshList()));
	connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(btnStopTask, SIGNAL(clicked()), this, SLOT(stopTask()));
	connect(lstTasks, SIGNAL(clicked(QModelIndex)), this, SLOT(showTask(QModelIndex)));

	// Start timer
	timer->setInterval(500); // 500 ms
	timer->start();
}

TaskEditor::~TaskEditor() {
	timer->stop();
	delete timer;
}

void TaskEditor::refreshList() {

	// Load tasks
	lstTasks->clear();
	for(int i = 0; i < tasks->count(); i++) {
		QString status;
		if     (tasks->at(i)->status == Task::Waiting) status = "Waiting";
		else if(tasks->at(i)->status == Task::Running) status = "Running";
		else if(tasks->at(i)->status == Task::Finished) status = "Finished";
		else if(tasks->at(i)->status == Task::Interrupted) status = "Interrupted";
		QListWidgetItem *item = new QListWidgetItem(QString("%1: %2 - %3").arg(i).arg(tasks->at(i)->name).arg(status));
		item->setData(Qt::UserRole, i);
		if(tasks->at(i)->status == Task::Running) item->setData(Qt::TextColorRole, Qt::blue);
		else if(tasks->at(i)->status == Task::Finished) item->setData(Qt::TextColorRole, Qt::gray);
		else if(tasks->at(i)->status == Task::Interrupted) item->setData(Qt::TextColorRole, Qt::red);
		lstTasks->addItem(item);
	}
}

void TaskEditor::showTask(QModelIndex index) {
	selectedIndex = index.row();
	// disable stop task button
	btnStopTask->setEnabled(false);

	// view task information
	Task *task = tasks->at(index.row());
	QString status;
	if     (task->status == Task::Waiting) status = "Waiting";
	else if(task->status == Task::Running) {
		status = "Running";
		btnStopTask->setEnabled(true);
	}
	else if(task->status == Task::Finished) status = "Finished";
	else if(task->status == Task::Interrupted) status = "Interrupted";
	txtTaskInfo->setText(QString("ID:\t%1\nTask:\t%2\nStatus:\t%3\nInterval:\t%4\n%5").arg(index.row()).arg(task->name).arg(status).arg(task->interval).arg(task->description()));
}

void TaskEditor::stopTask(){
	Task *task = tasks->at(selectedIndex);
	task->status = Task::Interrupted;

}


