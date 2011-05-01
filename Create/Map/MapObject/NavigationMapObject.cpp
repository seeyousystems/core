/*
 *  NavigationMapObject.cpp
 *
 *  Draws a visual represention of the Navigation component, especially
 *  the nav points...
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

#include "NavigationMapObject.h"

#include "../../Navigation/Navigation.h"
//#include "../../Navigation/SplineNavigation.h"
#include "../../Map/Map.h"
#include "../../Library/Debug.h"
#include "../../Library/Util.h"

NavigationMapObject::NavigationMapObject(ObjectMap *map, Navigation *navigation) : MapObject(map, "NavigationMapObject", 0, 0, 0, 0)
{
	this->navigation = navigation;
}

void NavigationMapObject::paint(QPainter &painter, QRect view, int scale)
{
	// Save the painter state for this paint session
	// so that it can be restored after without influencing other objects
	painter.save(); {

		// Translate, scale, and rotate...
		QTransform t;
		t.scale(1.0/scale, 1.0/scale);
		t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
		painter.setTransform(t, true);

		if(navigation != NULL) {

			// Init
			QColor pink(255,68,224);
			width = 0;
			height = 0;
			painter.setPen(Qt::transparent);
			painter.setOpacity(0.5);
			painter.setBrush(QBrush(pink));
			QList<Vector2D> navPoints = navigation->getNavPoints();

			// Draw all nav points
			for(int i = 0; i < navPoints.count(); i++) {

				// Calc size and coord
				int size = 100;
				if(navigation->getCurrentNavPoint()+1 == i) size = 200;
				int x = (int)navPoints.at(i).x();
				int y = (int)navPoints.at(i).y();

				// Set object dimensions based on most distant points...
				if(x > width) width = x;
				if(y > height) height = y;

				painter.setPen(Qt::transparent);
				painter.setOpacity(0.5);
				painter.setBrush(QBrush(pink));
				painter.drawEllipse(QRect(x - (size / 2), (-y) - (size / 2), size, size));

				painter.setFont(QFont("Arial Black", 120, QFont::Bold));
				painter.setPen(pink);
				painter.setOpacity(0.5);
				painter.drawText(x + 150, (-y) - 150, QString("%1").arg(i));

				if(i > 0) {

					QPen pen = painter.pen();
					pen.setWidth(20);
					painter.setPen(pen);
					painter.drawLine((int)navPoints.at(i-1).x(), -((int)navPoints.at(i-1).y()), x, -y);
				}
			}

			// Draw splines
			if(navigation->name == "SplineNavigation") {
//				drawSpline(painter, ((SplineNavigation*)navigation)->getNavSplineX(), ((SplineNavigation*)navigation)->getNavSplineY(), scale, Qt::yellow, 20);
//				drawSpline(painter, ((SplineNavigation*)navigation)->getWheelLeftSplineX(), ((SplineNavigation*)navigation)->getWheelLeftSplineY(), scale, Qt::red, 1);
//				drawSpline(painter, ((SplineNavigation*)navigation)->getWheelRightSplineX(), ((SplineNavigation*)navigation)->getWheelRightSplineY(), scale, Qt::green, 1);
			}

		}

	} painter.restore();
}

void NavigationMapObject::drawSpline(QPainter &painter, Spline1D splineX, Spline1D splineY, int scale, QColor color, int resolution) {
	// Draw nav spline
	QPen pen = painter.pen();
	pen.setWidth(10);
	pen.setColor(color);
	painter.setPen(pen);
	for (int i = 0; i < splineX.getNodeCount() - 1; i++) {

		for (int t = 1; t < resolution+1; t++) {
			double tt = ((double)t) / (double)resolution;
			double x = splineX.getValue(i, tt);
			double y = splineY.getValue(i, tt);
			painter.drawPoint((int)x, (int)-y);
		}
	}
}

void NavigationMapObject::setNavigation(Navigation *navigation) {
	this->navigation = navigation;
}

