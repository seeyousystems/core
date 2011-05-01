/*
 *  Util.h
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

#include "Util.h"

#include "../create.h"
#include "../CoreFactory.h"

#include "../GUI/Viewport.h"
#include "../GUI/OptionsDialog.h"

#include "../Navigation/Navigation.h"

#include "../Task/NavigationTask.h"
#include "../Task/TaskManager.h"

#include "../Map/HeatMap.h"
#include "../Map/ColorMap.h"
#include "../Map/TerrainMap.h"
#include "../Map/PhysicalMap.h"
#include "../Map/StructureMap.h"

#include "../Library/DockingStation.h"

#include "../COIL/COIL.h"

//#include "../RemoteInterface/RemoteInterface.h"


#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QApplication>

void Util::resetPainter(QPainter &painter, double scale) {
	painter.setOpacity(1.0);
	painter.resetTransform();
	painter.resetMatrix();
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.setFont(QFont());
	painter.scale(scale, scale);
}

void Util::setTextEditAsLogger(QTextEdit *textEdit) {
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

void Util::drawSpline(QPainter &painter, Spline1D *splineX, Spline1D *splineY, long movementScale, QColor color, int resolution) {
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

void Util::setSettingsFromTextData(QSettings *settings, QString textData, bool clearOldSettings) {
	if(clearOldSettings) settings->clear();
	if(textData == "") return; // don't do anything if empty string...
	QStringList data = textData.split('\n');
	for(int i = 0; i < data.count(); i++) {
		QString line = data.at(i);
		line = line.replace("\\=", "{EQUAL_SIGN}");
		QStringList keyValue = line.split('=');
		if(keyValue[0] != "") {
			settings->setValue(keyValue[0], keyValue[1].replace("{EQUAL_SIGN}","\\="));
		}
	}
}

QString Util::getSettingsFilePath(QString name) {

	Debug::warning("[Util] getSettingsFilePath() is deprecated!");

	QSettings settings("emss", "Core");
	QString filePath = settings.value(QString("SETTINGS_%1").arg(name)).toString();
	QFileInfo fileInfo(filePath);
	if(fileInfo.exists() == false) {
		filePath = QInputDialog::getText(NULL, QString("%1 Settings").arg(name), "Please enter the full settings file path:");
		settings.setValue(QString("SETTINGS_%1").arg(name), filePath);
	}
	return filePath;
}

// Gets a specific file path by showing a small dialog box. If the path does not exist,
// it is tried to be created. The slashes are added accordingly automatically.
// This will call recursively until a good path has been entered or cancel was pressed.
QString Util::getFilePath(QString defaultName, QString title, QString key, bool requiresCore, QString error) {

	// Init
	QSettings settings("emss", title);
	if(error != "") error = error.prepend("\n\nError: ");

	// Get current settings
	QString filePath = settings.value(key).toString();

	// Valid?
	if(!filePath.endsWith(QDir::separator()) || QFileInfo(filePath).isDir() == false) {

		// Not valid - get a new path
		QDir cd = QDir(QFileInfo(qApp->argv()[0]).absoluteDir());
		cd.cdUp();
		QString defaultPath = cd.path().append(QDir::separator()).append(defaultName);
		bool ok;
		filePath = QInputDialog::getText(NULL, QString("emss %1 Path").arg(title), QString("Please enter the full emss $Title path.\nOn Windows, such a path might look like 'C:\\emss\\$DefaultName',\nwhile on Unix such a path will look like '/opt/emss/$DefaultName'.$Error").replace("$Title", title).replace("$DefaultName", defaultName).replace("$Error", error), QLineEdit::Normal, defaultPath, &ok);

		if(ok) {

			// Fix trailing slash
			if(!filePath.endsWith(QDir::separator())) filePath.append(QDir::separator());

			// File path entered - check it!
			if(QFileInfo(filePath).isDir()) {

				// Make sure it contains the core
				if(requiresCore && !QFileInfo(QString(filePath).append("emssCore.config")).exists()) {
					return getFilePath(defaultName, title, key, requiresCore, "The emssCore.config file could not be found in the specified path!");
				}

				// All good... set the new value
				settings.setValue(QString("RESOURCE_PATH"), filePath);
				return filePath;

			} else {

				// Retry recursively...
				return getFilePath(defaultName, title, key, requiresCore, "The specified path does not exist or is not valid!");

			}

		} else {

			// Cancel pressed - break out
			return "";

		}


	} else {

		// Just return it
		return filePath;

	}
}

QString Util::getResourcesFilePath() {
	return getFilePath("resources", "Resources", "RESOURCE_PATH", true);
}

QString Util::getLogsFilePath() {
	return getFilePath("logs", "Logs", "LOGS_PATH", false);
}

void Util::resetFilePaths() {
	QSettings settings("emss", "Resources");
	settings.setValue(QString("RESOURCE_PATH"), "");
	settings.setValue(QString("LOGS_PATH"), "");
	Util::getResourcesFilePath();
	Util::getLogsFilePath();
}

// Returns a GPSPosition structure based on a space seperated GPS Latitude-Longitude-Height coords...
// Note: The height is optional.
// Note: this function is not included in gps.h in order to decrease gps.h's dependency on Qt (QString)
// Examples: "47.227736 8.816255"-->returns GPS 47.227736-8.816255-0, "47.227736 8.816255 33.00", ""-->returns GPS 0-0-0
GPSPosition Util::gpsPositionFromString(QString latitudeLongitudeHeight) {
	QStringList llh = latitudeLongitudeHeight.split(" ");
	if(llh.count() == 2) {
		return GPSPosition(llh[0].toDouble(),llh[1].toDouble(),0);
	} else if(llh.count() == 3) {
		return GPSPosition(llh[0].toDouble(),llh[1].toDouble(),llh[2].toDouble());
	} else {
		return GPSPosition(0,0,0);
	}
}

bool Util::isEqualQColor(QRgb reference, QRgb color, int tolerance){
	if(
			qRed(color) >= qRed(reference)-tolerance && qRed(color) <= qRed(reference)+tolerance &&
			qGreen(color) >= qGreen(reference)-tolerance && qGreen(color) <= qGreen(reference)+tolerance &&
			qBlue(color) >= qBlue(reference)-tolerance && qBlue(color) <= qBlue(reference)+tolerance &&
			qAlpha(color) >= qAlpha(reference)-tolerance && qAlpha(color) <= qAlpha(reference)+tolerance
	) 		return true;
	else 	return false;
}

QRgb Util::convertStringToRGB(QString string){
	QStringList l = string.split(" ");
	if(l.count() == 3){
		return qRgb(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt());
	}
	else if(l.count() == 4){
		return qRgba(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt(), l.at(3).toInt());
	}
	return qRgba(0,0,0,0);
}

void Util::viewportAction(Create *create, Viewport *viewport, QString value, long x, long y, int speed) {

	// Convert to mm
	create->pixelsTomm(x);
	x = create->pixelsTomm(x);
	y = create->pixelsTomm(y);

	if(value == "navpoint") {

		create->navigation->addNavPoint(x, y);

	} else if(value == "task") {

		Task *task = CoreFactory::createTask(create); // Don't specify a name, a GUI will be popped up...
		create->taskManager->addTask(task);
		viewport->resetToolbar();

	} else if(value == "collision") {

		create->heatMap->registerHeat(ColorMap::CollisionAreaChannel, x, y);

	} else if(value == "openarea") {

		create->heatMap->registerHeat(ColorMap::OpenAreaChannel, x, y);

	} else if(value == "environmentinfo") {

		// Get physical map area type as string
		QString physicalMapAreaType = "unknown";
		QRgb physicalMapColor = create->physicalMap->getPixelValue(x,y);
		if(create->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeWall) physicalMapAreaType = "wall";
		else if(create->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeOpen) physicalMapAreaType = "open";
		else if(create->physicalMap->getAreaType(x, y) == PhysicalMap::PhysicalMapAreaTypeStairs) physicalMapAreaType = "stairs";

		// Get GPS coordinates
		GPSPosition gps = create->physicalMap->mmToGPS(x, y);

		// Get docking station infos
		QString dockSignal = "unknown";
		int ir = create->dockingStation->getEmulatedIRSignalForPoint(Vector2D(x,y));
		if(ir == COIL::IR_SIGNAL_HOMEBASE_RESERVED) 							dockSignal = "reserved";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_REDBUOY) 						dockSignal = "red";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_GREENBUOY) 						dockSignal = "green";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_FORCEFIELD) 						dockSignal = "force";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY) 				dockSignal = "red,green";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_FORCEFIELD) 				dockSignal = "force,red";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_GREENBUOY_FORCEFIELD) 			dockSignal = "force,green";
		else if(ir == COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY_FORCEFIELD) 	dockSignal = "force,red,green";

		// Dump map info's
		Debug::print("[Interface] Local position: (%1,%2) mm", (int)x, (int)y);
		Debug::print("[Interface] Global position: (%1,%2)", QString("%1").arg(gps.latitude, 0 , 'f', 14 ),QString("%1").arg(gps.longitude, 0 , 'f', 14 ) );
		Debug::print("[Interface] Collision Area Heat: %1", create->heatMap->getChannelValue(HeatMap::CollisionAreaChannel, x, y));
		Debug::print("[Interface] Open Area Heat: %1", create->heatMap->getChannelValue(HeatMap::OpenAreaChannel, x, y));
		Debug::print("[Interface] Physical Map: %1 (%2)", physicalMapAreaType, QString("%1,%2,%3,%4").arg(qRed(physicalMapColor)).arg(qGreen(physicalMapColor)).arg(qBlue(physicalMapColor)).arg(qAlpha(physicalMapColor)));
		Debug::print("[Interface] Terrain Map: %1", create->terrainMap->getHeight(x,y));
		Debug::print("[Interface] Dock Signal: %1", dockSignal);

	} else if(value == "findrobot") {

		viewport->focusOnPoint(create->tracker->getX(), create->tracker->getY());
		viewport->resetToolbar();

	} else if(value == "raiseterrain") {

		create->terrainMap->raise(x, y);

	} else if(value == "lowerterrain") {

		create->terrainMap->lower(x, y);

	} else if(value == "moverobot") {

		create->tracker->setTransformation(Trafo2D::trans(x,y) * Trafo2D::rot(create->tracker->getTransformation().angle()));
		viewport->resetToolbar();

	} else if(value == "rotaterobot") {

		// Calculate rotating angle to the clicked point. Due to the fact that the robots position is between -180 and 180 it is necessary to subtract Rad(180) to get the exact angle
		double rotate = -angle(Vector2D(x,y), create->tracker->getTranslation()) - create->tracker->getTransformation().angle() - Rad(180);
		create->tracker->setTransformation(create->tracker->getTransformation() * Trafo2D::rot(rotate));
		viewport->resetToolbar();

	} else if(value == "movedockingstation") {
		QList<Vector2D> wallPoints;
		Vector2D closestWall;
		double beta;


		// Is snap to wall enabled?
		if(create->boolSetting("Robot_DockingStation_SnapToWall_Enabled")){

			// Find closest point to the wall for each degree. If one exists,
			// push into a list. Save also the closest point overall points
			double closestDist = INT_MAX;
			double amin = 0;
			double amax = 360;
			double astep = 1;
			double rmin = 0;
			double rmax = 500;
			double rstep = 1;
			for(double a = amin; a < amax; a = a + astep) {
				for(double r = rmin; r < rmax; r = r + rstep) {
					Trafo2D pos = Trafo2D::trans(x,y) * Trafo2D::rot(Rad(a)) * Trafo2D::trans(0, r);
					if(create->physicalMap->getAreaType(pos.trans().x(), pos.trans().y()) != PhysicalMap::PhysicalMapAreaTypeOpen) {
						wallPoints.append(pos.trans());
						if(dist(pos.trans(),Vector2D(x,y)) < closestDist) {
							closestDist = dist(pos.trans(),Vector2D(x,y));
							closestWall = pos.trans();
						}
						break;
					}
				}
			}


			// Detect the most distant two points in wallPoints.
			// This would be O(n^2) but we optimize by first calculating the
			// bounding box, then taking the points which are on the bounds, and
			// finally finding the most distant of those. Best case is O(n+n+1),
			// worst case is O(n+n+n^2). The worst can only happen if all points
			// are on a straight vertical or horizontal line.
			QList<Vector2D>::const_iterator i;

			// Get the bounding box of the ct points
			Vector2D wallStart, wallEnd;
			double xmin = INT_MAX;
			double xmax = INT_MIN;
			double ymin = INT_MAX;
			double ymax = INT_MIN;
			i = wallPoints.end();
			while (i != wallPoints.constBegin()) {
				Vector2D ct = *--i;
				if(ct.x() > xmax) xmax = ct.x();
				else if(ct.x() < xmin) xmin = ct.x();
				if(ct.y() > ymax) ymax = ct.y();
				else if(ct.y() < ymin) ymin = ct.y();
			}

			// Get all the ct points which ly on the bounds
			i = wallPoints.end();
			QList<Vector2D> candidates;
			while (i != wallPoints.constBegin()) {
				Vector2D ct = *--i;
				if(		ct.x() == xmin ||
						ct.x() == xmax ||
						ct.y() == ymin ||
						ct.y() == ymax	) {
					// Lies on the bounds, add to candidates
					candidates.append(ct);
				}
			}

			// Now we have our candidates - find the most distant ct points!
			i = candidates.constBegin();
			double maxdist = 0;
			while (i != candidates.constEnd()) {
				Vector2D cti = *i++;
				QList<Vector2D>::const_iterator j = candidates.constBegin();
				while (j != candidates.constEnd()) {
					Vector2D ctj = *j++;
					double distij = dist( cti, ctj );
					if( distij > maxdist ) {
						maxdist = distij;
						wallStart = cti;
						wallEnd = ctj;
					}
				}
			}

			// Calculate angle to turn the docking station
			double alpha = angle(wallStart, wallEnd);
			beta = Rad(90) - alpha;

			// If the dockingstation looks to the wall, we have to turn
			// it at 180 degrees

			Trafo2D tt = Trafo2D::trans(closestWall) * Trafo2D::rot(beta);
			tt = tt * Trafo2D::trans(0,50);
			if(create->physicalMap->getAreaType(tt.trans().x(), tt.trans().y()) == PhysicalMap::PhysicalMapAreaTypeOpen) {
				beta += Rad(180);
			}
		}

		int xx = closestWall.x();
		int yy = closestWall.y();

		// Is there no wall found in the rmax radius, we will use
		// the clicked x,y coordinates
		if (xx == 0 && yy == 0) {
			xx = x;
			yy = y;
			beta = create->dockingStation->position.angle();
		}

		// Apply the coordinates and angle to the docking station object
		create->dockingStation->position = Trafo2D::trans(xx,yy) * Trafo2D::rot(beta);
		viewport->resetToolbar();

	} else if(value == "rotatedockingstation") {

		double rotate = -angle(Vector2D(x,y), create->dockingStation->position.trans()) - create->dockingStation->position.angle() - Rad(0);
		create->dockingStation->position = create->dockingStation->position * Trafo2D::rot(rotate);
		viewport->resetToolbar();

	} else if(value == "navigationtask") {

		create->addTask(new NavigationTask(create, speed));
		viewport->resetToolbar();

	} else if(value == "resetNavpoint") {

		create->navigation->clearNavPoints();
		viewport->resetToolbar();

	} else if(value == "border") {

		create->structureMap->addStructureMapPoint(Vector2D(x,y));

		if(create->structureMap->isFinish()){
			viewport->resetToolbar();
		}

	} else if(value == "resetBorder") {

		create->structureMap->clearStructureMapPoints();
		viewport->resetToolbar();

	} else if(value == "setrobotdocked") {

		create->dockingStation->positionRobotOntopDockingStation();
		viewport->resetToolbar();

	}

}

void Util::refreshSensors( Create *create, QTextEdit *txtSensorData ) {
	txtSensorData->clear();
	int *data = (int*)malloc (36*sizeof(int));
	create->coil->getAllSensors(data);
	if(data != NULL) {

		txtSensorData->append(QString("BUMPS_AND_WHEEL_DROPS:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS)));
		txtSensorData->append(QString("WALL:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_WALL)));
		txtSensorData->append(QString("CLIFF_LEFT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT)));
		txtSensorData->append(QString("CLIFF_RIGHT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT)));
		txtSensorData->append(QString("VIRTUAL_WALL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_VIRTUAL_WALL)));
		txtSensorData->append(QString("OVERCURRENT:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_OVERCURRENT)));
		txtSensorData->append(QString("INFRARED:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_INFRARED)));
		txtSensorData->append(QString("BUTTONS:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BUTTONS)));
		txtSensorData->append(QString("DISTANCE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_DISTANCE)));
		txtSensorData->append(QString("ANGLE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_ANGLE)));
		txtSensorData->append(QString("CHARGING_STATE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_STATE)));
		txtSensorData->append(QString("VOLTAGE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_VOLTAGE)));
		txtSensorData->append(QString("CURRENT:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CURRENT)));
		txtSensorData->append(QString("BATTERY_TEMP:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_TEMP)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));
		txtSensorData->append(QString("WALL_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_WALL_SIGNAL)));
		txtSensorData->append(QString("CLIFF_LEFT_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_LEFT_SIGNAL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_LEFT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_FRONT_RIGHT_SIGNAL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_FRONT_RIGHT_SIGNAL)));
		txtSensorData->append(QString("CLIFF_RIGHT_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CLIFF_RIGHT_SIGNAL)));
		txtSensorData->append(QString("DIGITAL_INPUTS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_DIGITAL_INPUTS)));
		txtSensorData->append(QString("ANALOG_SIGNAL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_ANALOG_SIGNAL)));
		txtSensorData->append(QString("CHARGING_SOURCES_AVAILABLE:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_CHARGING_SOURCES_AVAILABLE)));
		txtSensorData->append(QString("OI_MODE:\t\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_OI_MODE)));
		txtSensorData->append(QString("SONG_NUMBER:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_SONG_NUMBER)));
		txtSensorData->append(QString("SONG_IS_PLAYING:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_SONG_IS_PLAYING)));
		txtSensorData->append(QString("NUM_STREAM_PACKETS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_NUM_STREAM_PACKETS)));
		txtSensorData->append(QString("REQUESTED_VELOCITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_VELOCITY)));
		txtSensorData->append(QString("REQUESTED_RADIUS:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RADIUS)));
		txtSensorData->append(QString("REQUESTED_RIGHT_VEL:\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_RIGHT_VEL)));
		txtSensorData->append(QString("REQUESTED_LEFT_VEL:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_REQUESTED_LEFT_VEL)));
		txtSensorData->append(QString("BATTERY_CHARGE:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CHARGE)));
		txtSensorData->append(QString("BATTERY_CAPACITY:\t\t%1").arg(create->coil->extractSensorFromData(data, COIL::SENSOR_BATTERY_CAPACITY)));

		free(data);

	}
}

void Util::sendFingerprintMessage(Create *create, long fingerprintWaitTime){
//	if (create->remoteInterface){
//		// Send a fingerprint message...
//		RemoteInterfaceMessage *message = new RemoteInterfaceMessage("fingerprint");
//
//		// Local position
//		message->properties->insert("local-position-x", QString("%1").arg(create->tracker->getX()));
//		message->properties->insert("local-position-y", QString("%1").arg(create->tracker->getY()));
//
//		// Get GPS coordinates based on an offset added with the degrees based on x,y meters at that offset
//		GPSPosition gps = create->physicalMap->mmToGPS(create->tracker->getX(), create->tracker->getY());
//		message->properties->insert("global-position-latitude", QString("%1").arg(gps.latitude, 0 , 'f', create->intSetting("Task_FingerprintNavigationTask_GPSPrecision") ));
//		message->properties->insert("global-position-longitude", QString("%1").arg(gps.longitude, 0 , 'f', create->intSetting("Task_FingerprintNavigationTask_GPSPrecision") ));
//
//		Debug::print("[Util] GPS Coordinates: %1 / %2", gps.latitude, gps.longitude);
//
//		// Add additional map information
//
//		message->properties->insert("floor",create->physicalMap->getFloor());
//		message->properties->insert("map-description", create->physicalMap->getMapDescription());
//		message->properties->insert("map-file", QString(create->physicalMap->filePath).remove(0, create->physicalMap->filePath.lastIndexOf("/") + 1 ));
//		message->properties->insert("wait-time", QString("%1").arg(fingerprintWaitTime));
//
//		// Send off the message as a broadcast to all connected
//		create->remoteInterface->broadcastMessage(message);
//	}
//	else {
//		Debug::warning("[Util] no RemoteInterface available for sending fingerprint message!");
//	}

}

void Util::saveWorldState(Create *create) {
	// Save robot state
	create->settings->setValue("Robot_StartingPositionX_mm", QString("%1").arg((long)create->tracker->getTranslation().x()));
	create->settings->setValue("Robot_StartingPositionY_mm", QString("%1").arg((long)create->tracker->getTranslation().y()));
	create->settings->setValue("Robot_StartingRotation_deg", QString("%1").arg(create->tracker->getRotation()));

	// Save docking station state
	create->settings->setValue("Robot_DockingStation_StartingPositionX_mm", QString("%1").arg((long)create->dockingStation->position.trans().x()));
	create->settings->setValue("Robot_DockingStation_StartingPositionY_mm", QString("%1").arg((long)create->dockingStation->position.trans().y()));
	create->settings->setValue("Robot_DockingStation_StartingRotation_deg", QString("%1").arg(Deg(create->dockingStation->position.angle())));
	create->settings->setValue("Robot_DockingStation_Enabled", QString("%1").arg(create->dockingStation->enabled ? "true" : "false"));
	create->settings->setValue("Robot_DockingStation_RedBeamEnabled", QString("%1").arg(create->dockingStation->redBeamEnabled ? "true" : "false"));
	create->settings->setValue("Robot_DockingStation_GreenBeamEnabled", QString("%1").arg(create->dockingStation->greenBeamEnabled ? "true" : "false"));
	create->settings->setValue("Robot_DockingStation_ForceFieldEnabled", QString("%1").arg(create->dockingStation->forceFieldEnabled ? "true" : "false"));
	create->settings->setValue("Robot_DockingStation_InterferenceEnabled", QString("%1").arg(create->dockingStation->interferenceEnabled ? "true" : "false"));

	// Save navigation nav points as a string list seperated by ' '
	QString navPointsListX;
	QString navPointsListY;
	bool first = true;
	foreach(Vector2D p, create->navigation->getNavPoints()) {
		navPointsListX.append(QString(first ? "%1":" %1").arg(p.x()));
		navPointsListY.append(QString(first ? "%1":" %1").arg(p.y()));
		first = false;
	}
	create->settings->setValue("Navigation_InitialNavPointsListX_mm", navPointsListX);
	create->settings->setValue("Navigation_InitialNavPointsListY_mm", navPointsListY);

	Debug::print("[Util] saved world state");
}

QString Util::getTextDataFromResource(QString name) {
	QFile file(QString(":%1").arg(name));
	if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream txt(&file);
		return txt.readAll().remove(-1,1);
	} else {
		return "";
	}
}
