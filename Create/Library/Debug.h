/*
 *  Debug.h
 *
 *  The Debug class offers some debugging assistance, especially during runtime,
 *  by encapsulating common console commands. Printing color-formatted sets of
 *  values along with an automatic timestamp can be very easily achieved.
 *  Additionally, calling setOutput(widget) allows an application to re-route
 *  all debugging information to any given text widget.
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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <QTextEdit>
#include <QTime>
#include <QVariant>
#include <QMutex>
#include <QFile>

class Debug {

public:
	static QTextEdit *_outputTextBox;
	static QColor _colorNormal;
	static QMutex _mutex;
	static QFile _logFile;
	static bool _writeToLogFile;

public:
	static void setOutput(QTextEdit *outputTextBox);
	static void logToFile(QString filePath);

	static void print(QString text);
	static void print(QString text, QVariant value);
	static void print(QString text, QVariant value1, QVariant value2);
	static void print(QString text, QVariant value1, QVariant value2, QVariant value3);
	static void print(QString text, QVariant value1, QVariant value2, QVariant value3, QVariant value4);
	static void print(int value);
	static void print(int value1, int value2);
	static void print(QString text, int* data, int size);

	static void rnd(QString text = "%1");

	static void error(QString text);
	static void error(QString text, QVariant value);
	static void error(QString text, QVariant value1, QVariant value2);
	static void error(QString text, QVariant value1, QVariant value2, QVariant value3);

	static void warning(QString text);
	static void warning(QString text, QVariant value);
	static void warning(QString text, QVariant value1, QVariant value2);

};



#endif /*DEBUG_H_*/
