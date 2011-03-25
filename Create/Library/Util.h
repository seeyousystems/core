/*
 *  Util.h
 *
 *  Collection of various practical utility methods.
 *
 *  ===========================================================================
 *
 *  Copyright 2008 Daniel Kruesi (Dan Krusi) and David Grob
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

#ifndef UTIL_H_
#define UTIL_H_

#include <QPainter>
#include <QTextEdit>
#include <QSettings>
#include <QFileInfo>
#include <QInputDialog>

#include "../Library/Math.h"
#include "../Library/Debug.h"

namespace Util {


static void inline resetPainter(QPainter &painter, double scale = 1.0) {
	painter.setOpacity(1.0);
	painter.resetTransform();
	painter.resetMatrix();
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.setFont(QFont());
	painter.scale(scale, scale);
}

static void setTextEditAsLogger(QTextEdit *textEdit) {
	textEdit->setReadOnly(true);
	textEdit->setFontFamily("Lucida Console");
	textEdit->setWordWrapMode(QTextOption::NoWrap);
	QFont fntConsole = textEdit->font();
	//fntConsole.setPointSize(fntConsole.pointSize()+2);
	textEdit->setFont(fntConsole);
	QPalette palette = textEdit->palette();
	palette.setColor(QPalette::Base, Qt::darkBlue);
	palette.setColor(QPalette::Text, Qt::white);
	textEdit->setPalette(palette);
}

static void drawSpline(QPainter &painter, Spline1D *splineX, Spline1D *splineY, long movementScale, QColor color, int resolution) {
	// Draw nav spline
	QPen pen = painter.pen();
	pen.setWidth(1);
	pen.setColor(color);
	painter.setPen(pen);
	for (int i = 0; i < splineX->getNodeCount() - 1; i++) {

		for (int t = 1; t < resolution+1; t++) {
			double tt = ((double)t) / (double)resolution;
			double x = splineX->getValue(i, tt) / movementScale;
			double y = splineY->getValue(i, tt) / movementScale;
			painter.drawPoint((int)x, (int)-y);
		}
	}
}

static void setSettingsFromTextData(QSettings *settings, QString textData) {
	settings->clear();
	QStringList data = textData.split('\n');
	for(int i = 0; i < data.count(); i++) {
		QStringList keyValue = data.at(i).split('=');
		if(keyValue[0] != "") {
			settings->setValue(keyValue[0], keyValue[1]);
		}
	}
}

static QString getSettingsFilePath(QString name) {
	QSettings settings("emss", "Code");
	QString filePath = settings.value(QString("SETTINGS_%1").arg(name)).toString();
	QFileInfo fileInfo(filePath);
	if(fileInfo.exists() == false) {
		filePath = QInputDialog::getText(NULL, QString("%1 Settings").arg(name), "Please enter the full settings file path:");
		settings.setValue(QString("SETTINGS_%1").arg(name), filePath);
	}
	return filePath;
}

};

#endif /* UTIL_H_ */
