/*
 *  Debug.cpp
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

#include "Debug.h"

#include <stdio.h>

#include <QMetaObject>

QTextEdit *Debug::_outputTextBox = NULL;
QColor Debug::_colorNormal;
QMutex Debug::_mutex;
QFile Debug::_logFile;
bool Debug::_writeToLogFile = false;

void Debug::setOutput(QTextEdit *outputTextBox) {
	Debug::_outputTextBox = outputTextBox;
	Debug::_colorNormal = outputTextBox->palette().color(QPalette::Text);
}

void Debug::logToFile(QString filePath) {
	Debug::_logFile.setFileName(filePath);
	_writeToLogFile = Debug::_logFile.open(QIODevice::Append);
}

void Debug::print(QString text) {
	_mutex.lock(); {
		text.prepend(QTime::currentTime().toString("hh:mm:ss:zzz "));
		if(Debug::_writeToLogFile) {
			Debug::_logFile.write(text.toAscii());
			Debug::_logFile.write(QString("\n").toAscii());
			Debug::_logFile.flush();
		}
		if(Debug::_outputTextBox == NULL) {
			text.append("\n");
			printf(text.toAscii());
		} else {
			// Invoke the GUI methods on the textedit in the queued events pipeline...
			QMetaObject::invokeMethod(Debug::_outputTextBox, "setTextColor", Qt::QueuedConnection, Q_ARG(QColor, Debug::_colorNormal));
			QMetaObject::invokeMethod(Debug::_outputTextBox, "append", Qt::QueuedConnection, Q_ARG(QString, text));
		}
	} _mutex.unlock();
}

void Debug::print(QString text, QVariant value) {
	Debug::print(text.arg(value.toString()));
}

void Debug::print(QString text, QVariant value1, QVariant value2) {
	Debug::print(text.arg(value1.toString()).arg(value2.toString()));
}

void Debug::print(QString text, QVariant value1, QVariant value2, QVariant value3) {
	Debug::print(text.arg(value1.toString()).arg(value2.toString()).arg(value3.toString()));
}

void Debug::print(QString text, QVariant value1, QVariant value2, QVariant value3, QVariant value4) {
	Debug::print(text.arg(value1.toString()).arg(value2.toString()).arg(value3.toString()).arg(value4.toString()));
}

void Debug::print(int value) {
	Debug::print(QString("%1").arg(value));
}

void Debug::print(int value1, int value2) {
	Debug::print(QString("%1,%2").arg(value1).arg(value2));
}

void Debug::print(QString text, int* data, int size) {
	for(int i = 0; i < size; i++) {
		Debug::print(text.arg(data[i]).arg(i));
	}
}

void Debug::rnd(QString text) {
	Debug::print(text.arg(text).arg(rand()%35000));
}

void Debug::error(QString text) {
	_mutex.lock(); {
		text.prepend(QTime::currentTime().toString("hh:mm:ss:zzz "));
		if(Debug::_writeToLogFile) {
			Debug::_logFile.write(text.toAscii());
			Debug::_logFile.write(QString("\n").toAscii());
			Debug::_logFile.flush();
		}
		if(Debug::_outputTextBox == NULL) {
			perror(text.toAscii()); // TODO: change to correct error out method. perror is wrong!
		} else {
			// Invoke the GUI methods on the textedit in the queued events pipeline...
			QMetaObject::invokeMethod(Debug::_outputTextBox, "setTextColor", Qt::QueuedConnection, Q_ARG(QColor, Qt::red));
			QMetaObject::invokeMethod(Debug::_outputTextBox, "append", Qt::QueuedConnection, Q_ARG(QString, text));
		}
	} _mutex.unlock();
}

void Debug::error(QString text, QVariant value) {
	Debug::error(text.arg(value.toString()));
}

void Debug::error(QString text, QVariant value1, QVariant value2) {
	Debug::error(text.arg(value1.toString()).arg(value2.toString()));
}

void Debug::error(QString text, QVariant value1, QVariant value2, QVariant value3) {
	Debug::error(text.arg(value1.toString()).arg(value2.toString()).arg(value3.toString()));
}

void Debug::warning(QString text) {
	_mutex.lock(); {
		text.prepend(QTime::currentTime().toString("hh:mm:ss:zzz "));
		if(Debug::_writeToLogFile) {
			Debug::_logFile.write(text.toAscii());
			Debug::_logFile.write(QString("\n").toAscii());
			Debug::_logFile.flush();
		}
		if(Debug::_outputTextBox == NULL) {
			perror(text.toAscii());
		} else {
			// Invoke the GUI methods on the textedit in the queued events pipeline...
			QMetaObject::invokeMethod(Debug::_outputTextBox, "setTextColor", Qt::QueuedConnection, Q_ARG(QColor, Qt::yellow));
			QMetaObject::invokeMethod(Debug::_outputTextBox, "append", Qt::QueuedConnection, Q_ARG(QString, text));
		}
	} _mutex.unlock();
}

void Debug::warning(QString text, QVariant value) {
	Debug::warning(text.arg(value.toString()));
}

void Debug::warning(QString text, QVariant value1, QVariant value2) {
	Debug::warning(text.arg(value1.toString()).arg(value2.toString()));
}

