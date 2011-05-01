/*
 *  KMLGroundOverlay.h
 *
 *  Parses KML files based on the following boilerplate:
 *
 *  <?xml version="1.0" encoding="UTF-8"?>
 *  <kml xmlns="http://earth.google.com/kml/2.2">
 *  <GroundOverlay>
 *  	<name>Gebäude 1 EG</name>
 *  	<color>e0ffffff</color>
 *  	<Metadata>
 *  		<floor>0</floor>
 *  		<background>0</background>
 *  	</Metadata>
 *  	<Icon>
 *  		<href>Gebaeude_1_EG.jpg</href>
 *  		<viewBoundScale>0.75</viewBoundScale>
 *  	</Icon>
 *  	<LatLonBox>
 *  		<north>47.22388264535114</north>
 *  		<south>47.22278337276146</south>
 *  		<east>8.818257171976143</east>
 *  		<west>8.816723630156409</west>
 *  	</LatLonBox>
 *  </GroundOverlay>
 *  </kml>
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

#ifndef KMLFILE_H_
#define KMLFILE_H_

#include <QObject>
#include <QString>
#include <QMap>
#include <QDomDocument>
#include <QFile>

#include "Math.h"

class KMLGroundOverlay : public QObject {

	Q_OBJECT

public:

	QString name;

	QMap<QString, QString> metaData;

	struct Icon {
		QString href;
		double viewBoundScale;
	} icon;

	struct LatLonBox {
		double north;
		double south;
		double east;
		double west;
	} latLonBox;


public:
	KMLGroundOverlay();
	virtual ~KMLGroundOverlay();
	bool loadFromFile(QString filepath);
	GPSPosition getGPSReferencePoint();

private:
	QDomElement findElement(QDomNode n, QString tagName);

};

#endif /* KMLFILE_H_ */
