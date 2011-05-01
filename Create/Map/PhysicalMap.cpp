/*
 *  PhysicalMap.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms  framework.
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

#include "PhysicalMap.h"

#include "../Library/Math.h"
#include "../Library/Util.h"

// Note: the the filePath parameter requires a full path.
PhysicalMap::PhysicalMap(Create *create, QString filePath, double mapScale) : Map("PhysicalMap", create)
{

	// Init
	kml = NULL;
	QString imagePath;
	this->filePath = filePath;
	wall = Util::convertStringToRGB(create->stringSetting("Map_PhysicalMap_Color_Wall"));
	stairs = Util::convertStringToRGB(create->stringSetting("Map_PhysicalMap_Color_Stairs"));
	tolerance = create->intSetting("Map_PhysicalMap_Color_Tolerance");

	// Is it a KML File?
	if (this->filePath.endsWith(".kml")){
		kml = new KMLGroundOverlay();
		kml->loadFromFile(this->filePath);

		// Assemble code to image
		imagePath = QString(filePath).remove(filePath.lastIndexOf("/") + 1, filePath.length() - filePath.lastIndexOf("/") -1);
		imagePath += kml->icon.href;
	}
	else {
		imagePath = this->filePath;
	}


	// Load image
	if(imagePath != "") image = QImage(imagePath);

	// Determine mapScale
	if (kml != NULL){
		GPSPosition gps = kml->getGPSReferencePoint();
		double width_mm = (kml->latLonBox.east - kml->latLonBox.west) * gps.metersPerLongitude() * 1000; // mm of latlonbox
		double height_mm = (kml->latLonBox.north - kml->latLonBox.south) * gps.metersPerLatitude() * 1000; // mm of latlonbox
		double scaleX = create->pixelsTomm(image.width()) / width_mm; // map scale to represent image at create->scale
		double scaleY = create->pixelsTomm(image.height()) / height_mm;
		this->mapScale = (scaleX+scaleY)/2.0;
	}
	else {
		this->mapScale = mapScale;
	}
}

PhysicalMap::~PhysicalMap()
{
	delete kml;
}

void PhysicalMap::paint(QPainter &painter, QRect view) {

	// Draw map image
	QRectF target = QRect(0, 0, view.width(), view.height());
	int imageX = view.x()*mapScale;
	int imageY = image.height() - view.y()*mapScale - view.height()*mapScale;
	int imageWidth = view.width()*mapScale;
	int imageHeight = view.height()*mapScale;
	QRectF source = QRect(imageX, imageY, imageWidth, imageHeight);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
	painter.drawImage(target, image, source);

}

PhysicalMap::PhysicalMapAreaType PhysicalMap::getAreaType(long x, long y) {

	// Convert to pixel
	x = create->mmToPixels(x)*this->mapScale;
	y = create->mmToPixels(y)*this->mapScale;

	// Check bounds
	if(x > image.width() || image.height() - y > image.height()) return PhysicalMapAreaTypeOpen;

	// Get the area type based on alpha channel...
	QRgb pixel = image.pixel((int)(x), (int)(image.height() - y));

	// Check similarity to a reference color
	if ( Util::isEqualQColor(wall, pixel, tolerance)) return PhysicalMapAreaTypeWall;
	else if ( Util::isEqualQColor(stairs, pixel, tolerance)) return PhysicalMapAreaTypeStairs;
	else return PhysicalMapAreaTypeOpen;
}

QRgb PhysicalMap::getPixelValue(long x, long y) {

	// Convert to pixel mapScale
	x = create->mmToPixels(x)*this->mapScale;
	y = create->mmToPixels(y)*this->mapScale;

	// Check bounds
	if(x > image.width() || image.height() - y > image.height()) return qRgb(0,0,0);

	// See what the physical map states this area is...
	return image.pixel((int)(x), (int)(image.height() - y));

}


long PhysicalMap::width() {
	return create->pixelsTomm(this->image.width()/mapScale);
}

long PhysicalMap::height() {
	return create->pixelsTomm(this->image.height()/mapScale);
}

GPSPosition PhysicalMap::mmToGPS(long x, long y){
	GPSPosition gps;

	if( kml == NULL ){

		gps = Util::gpsPositionFromString(create->stringSetting("Map_PhysicalMap_GPSOffset"));
		gps.longitude -= gps.longitudesPerMeters(x / 1000.0); // convert mm to m
		gps.latitude += gps.latitudesPerMeters(y / 1000.0); // convert mm to m

	} else {

		// difference in mm to reference point
		double refx_mm = width() / 2;
		double refy_mm = height() / 2;

		double dx_mm = x - refx_mm;
		double dy_mm = y - refy_mm;

		// Calculate GPS-Coordinate using the GPS-ReferencepPoint
		gps.longitude = kml->getGPSReferencePoint().longitude + (kml->getGPSReferencePoint().longitudesPerMeters(dx_mm / 1000));
		gps.latitude = kml->getGPSReferencePoint().latitude + (kml->getGPSReferencePoint().latitudesPerMeters(dy_mm / 1000));

	}

	return gps;
}


QString PhysicalMap::getFloor(){
	if ( kml == NULL) return create->stringSetting("Map_PhysicalMap_Floor");
	return kml->metaData.value("floor");
}

QString PhysicalMap::getMapDescription(){
	if ( kml == NULL) return create->stringSetting("Map_PhysicalMap_Description");
	return kml->name;
}

