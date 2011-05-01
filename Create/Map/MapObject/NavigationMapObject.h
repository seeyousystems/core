/*
 *  NavigationMapObject.h
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

#ifndef NAVIGATIONMAPOBJECT_H_
#define NAVIGATIONMAPOBJECT_H_

#include "MapObject.h"

#include "../../Navigation/Navigation.h"
#include "../../Library/Math.h"

class NavigationMapObject: public MapObject
{

	Q_OBJECT

private:
	Navigation *navigation;

public:
	NavigationMapObject(ObjectMap *map, Navigation *navigation);
	virtual ~NavigationMapObject() {};
	virtual void paint(QPainter &painter, QRect view, int scale);
	void drawSpline(QPainter &painter, Spline1D splineX, Spline1D splineY, int scale, QColor color, int resolution);

public slots:
	void setNavigation(Navigation *navigation);

};

#endif /* NAVIGATIONMAPOBJECT_H_ */