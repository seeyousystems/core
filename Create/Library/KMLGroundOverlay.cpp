/*
 *  KMLGroundOverlay.h
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

#include "KMLGroundOverlay.h"

#include "../Library/Debug.h"

KMLGroundOverlay::KMLGroundOverlay() {

	// Just zero everything

	name = "";

	icon.href = "";
	icon.viewBoundScale = 0;

	latLonBox.north = 0;
	latLonBox.south = 0;
	latLonBox.east = 0;
	latLonBox.west = 0;

}

KMLGroundOverlay::~KMLGroundOverlay() {

}

bool KMLGroundOverlay::loadFromFile(QString filepath) {

	// Read file and dom tree...
	QDomDocument doc("kml");
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly)) {
		// Could not read file
		Debug::warning("[KMLGroundOverlay] could not open file '%1' for reading", filepath);
		return false;
	} else if (!doc.setContent(&file)) {
		// Could not parse
		file.close();
		Debug::warning("[KMLGroundOverlay] could not create Xml DOM from file '%1'", filepath);
		return false;
	}
	file.close();

	// Begin parse... get kml root
	QDomElement elemRoot = doc.documentElement();
	if(elemRoot.tagName() != "kml") {
		Debug::warning("[KMLGroundOverlay] the root element is not 'kml'! The file '%1' does not appear to be a valid kml file...", filepath);
		return false;
	}

	// Find GroundOverlay node...
	QDomNode n = elemRoot.firstChild();
	QDomElement elemGroundOverlay = findElement(elemRoot, "GroundOverlay");
	if(elemGroundOverlay.isNull()) {
		Debug::warning("[KMLGroundOverlay] could not find the GroundOverlay element! The file '%1' does not appear to be a valid kml GroundOverlay file...", filepath);
		return false;
	}

	// Load name
	QDomElement elemName = findElement(elemGroundOverlay, "name");
	if(!elemName.isNull()) name = elemName.text();

	// Load icon
	QDomElement elemIcon = findElement(elemGroundOverlay, "Icon");
	if(!elemIcon.isNull()) {
		if(!findElement(elemIcon, "href").isNull()) icon.href = findElement(elemIcon, "href").text();
		if(!findElement(elemIcon, "viewBoundScale").isNull()) icon.viewBoundScale = findElement(elemIcon, "viewBoundScale").text().toDouble();
	}

	// Load latlonbox
	QDomElement elemLatLonBox = findElement(elemGroundOverlay, "LatLonBox");
	if(!elemLatLonBox.isNull()) {
		if(!findElement(elemLatLonBox, "north").isNull()) latLonBox.north = findElement(elemLatLonBox, "north").text().toDouble();
		if(!findElement(elemLatLonBox, "south").isNull()) latLonBox.south = findElement(elemLatLonBox, "south").text().toDouble();
		if(!findElement(elemLatLonBox, "east").isNull()) latLonBox.east = findElement(elemLatLonBox, "east").text().toDouble();
		if(!findElement(elemLatLonBox, "west").isNull()) latLonBox.west = findElement(elemLatLonBox, "west").text().toDouble();
	}

	// Load meta data entries...
	QDomElement elemMetadata = findElement(elemGroundOverlay, "Metadata");
	if(!elemMetadata.isNull()) {
		QDomNode n = elemMetadata.firstChild();
		QDomElement elemRet;
		while(!n.isNull()) {
			QDomElement e = n.toElement(); // Try to convert the node to an element
			if(!e.isNull()) {
				metaData.insert(e.tagName(), e.text());
			}
			n = n.nextSibling();
		}
	}

	return true;
}

// Finds a given child element with the tag name tagName. Call .isNull() on
// the returned value to check if it was found!
QDomElement KMLGroundOverlay::findElement(QDomNode parent, QString tagName) {
	QDomNode n = parent.firstChild();
	QDomElement elemRet;
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // Try to convert the node to an element
		if(!e.isNull() && e.tagName() == tagName) {
			elemRet = e;
			break;
		}
		n = n.nextSibling();
	}
	return elemRet;
}

GPSPosition KMLGroundOverlay::getGPSReferencePoint(){
	return GPSPosition ((latLonBox.north + latLonBox.south) / 2, (latLonBox.west + latLonBox.east) / 2, 0);
}





