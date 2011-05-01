/*
 *  Util.h
 *
 *  Collection of various practical utility methods.
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

#ifndef UTIL_H_
#define UTIL_H_

#include <QPainter>
#include <QTextEdit>
#include <QSettings>
#include <QFileInfo>
#include <QInputDialog>
#include <QImage>

#include "../Library/Math.h"
#include "../Library/Debug.h"

class Create;
class Viewport;


class Util {

public:

	static void resetPainter(QPainter &painter, double scale = 1.0);
	static void setTextEditAsLogger(QTextEdit *textEdit);
	static void drawSpline(QPainter &painter, Spline1D *splineX, Spline1D *splineY, long movementScale, QColor color, int resolution);
	static void setSettingsFromTextData(QSettings *settings, QString textData, bool clearOldSettings = true);
	static QString getFilePath(QString defaultName, QString title, QString key, bool requiresCore, QString error = "");
	static QString getSettingsFilePath(QString name);
	static QString getResourcesFilePath();
	static QString getLogsFilePath();
	static void resetFilePaths();
	static GPSPosition gpsPositionFromString(QString latitudeLongitudeHeight);
	static bool isEqualQColor(QRgb reference, QRgb color, int tolerance);
	static QRgb convertStringToRGB(QString string);
	static void viewportAction(Create *create, Viewport *viewport, QString value, long x, long y, int speed);
	static void refreshSensors( Create *create, QTextEdit *txtSensorData );
	static void sendFingerprintMessage(Create *create, long fingerprintWaitTime);
	static void saveWorldState(Create *create);
	static QString getTextDataFromResource(QString name);

};

#endif /* UTIL_H_ */
