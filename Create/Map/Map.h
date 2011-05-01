/*
 *  Map.h
 *
 *  Maps hold information about the environment. In the emss framework there
 *  exist several types of maps, each serving a different purpose. emss Maps
 *  must define the abstract class Map, however they enjoy a significant amount
 *  of freedom. The only virtual Map method is paint(view,scale), which is used
 *  by the Viewports for displaying the map to the user. In addition, any map
 *  must provide a width and a height. If a Map does not know its own dimensions,
 *  it can consult the emss Core for the basic world boundaries. A certain set
 *  of functionality shared by both the Heat Map and Terrain Map have been
 *  combined into the Color Map.
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

#ifndef MAP_H_
#define MAP_H_

#include <QPainter>
#include <QString>
#include <QList>
#include <QReadWriteLock>

#include "../CoreObject.h"

#include "../Library/Debug.h"

class Map : public CoreObject
{
	Q_OBJECT

protected:
	QReadWriteLock lock;

public:
	Map(QString name, Create *create);
	virtual ~Map();

public:
	virtual void paint(QPainter &painter, QRect view) = 0;
	virtual long width() = 0; // Core mm
	virtual long height() = 0; // Core mm

signals:
	void sizeChanged();

};

#endif /*MAP_H_*/
