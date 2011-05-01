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

#include "TrackerMapObject.h"

#include "../../MovementTracker/Tracker.h"
#include "../../MovementTracker/SelfCorrectingTracker.h"
#include "../../Map/Map.h"
#include "../../Library/Debug.h"
#include "../../Library/Util.h"

TrackerMapObject::TrackerMapObject(ObjectMap *map, Tracker *tracker) : MapObject(map, "TrackerMapObject", 0, 0, 0, 0)
{
	this->tracker = tracker;
}

void TrackerMapObject::paint(QPainter &painter, QRect view, int scale)
{
	// Save the painter state for this paint session
	// so that it can be restored after without influencing other objects
	painter.save(); {

		// Translate, scale, and rotate...
		QTransform t;
		t.scale(1.0/scale, 1.0/scale);
		t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
		painter.setTransform(t, true);


		if(tracker) {

			// Init painter
			painter.setPen(Qt::transparent);
			painter.setOpacity(0.5);
			painter.setBrush(QBrush(Qt::yellow));
			int size = 100;
			int ctsize = 60;
			int historyMovement = 100;
			int historyCorrelation = 5;

			// Draw all points in tracker movement log using the snapshot feature
			QList<Trafo2D> log = tracker->getMovementLogSnapshot(historyMovement);
			QList<Trafo2D>::const_iterator i = log.begin();
			while (i != log.constEnd()) {
				Trafo2D t = *i++;
				painter.drawEllipse(QRect(t.trans().x() - (size / 2), (-t.trans().y()) - (size / 2), size, size));
			}

			// Do we have a SelfCorrectingTracker? If, so draw the corresponding points
			if(tracker->name == "SelfCorrectingTracker") {

				// Draw all ct's
				QList<CorrelationTriangle> logct = ((SelfCorrectingTracker*)tracker)->getCorrelationLogSnapshot(historyCorrelation);
				QList<CorrelationTriangle>::const_iterator ict = logct.begin();
				while (ict != logct.constEnd()) {
					// Draw the ct points and connecting line
					CorrelationTriangle ct = *ict++;
					painter.setOpacity(1.0);
					int ctcx = ct.collision.trans().x();
					int ctcy = (-ct.collision.trans().y());
					int ctwx = ct.closestWall.trans().x();
					int ctwy = (-ct.closestWall.trans().y());
					QPen pen = QPen(Qt::yellow);
					pen.setWidth(10);
					painter.setPen(pen);
					painter.drawLine(ctcx, ctcy, ct.position.trans().x(), -ct.position.trans().y());
					pen = QPen(Qt::green);
					pen.setWidth(20);
					painter.setPen(pen);
					painter.drawLine(ctcx, ctcy, ctwx, ctwy);
					painter.setPen(Qt::transparent);
					painter.setBrush(QBrush(Qt::green));
					painter.drawEllipse(QRect(ctcx - (ctsize / 2), ctcy - (ctsize / 2), ctsize, ctsize));
					painter.setBrush(QBrush(Qt::blue));
					painter.drawEllipse(QRect(ctwx - (ctsize / 2), ctwy - (ctsize / 2), ctsize, ctsize));
				}

				// Draw the currently suspected wall, if valid
				SuspectedWall wall = ((SelfCorrectingTracker*)tracker)->getCurrentlySuspectedWall();
				if(wall.valid) {
					QPen pen = QPen(Qt::darkGreen);
					pen.setWidth(20);
					painter.setPen(pen);
					painter.drawLine(	wall.start.collision.trans().x(),
										-wall.start.collision.trans().y(),
										wall.end.collision.trans().x(),
										-wall.end.collision.trans().y()	);
					pen = QPen(Qt::darkBlue);
					pen.setWidth(20);
					painter.setPen(pen);
					painter.drawLine(	wall.start.closestWall.trans().x(),
										-wall.start.closestWall.trans().y(),
										wall.end.closestWall.trans().x(),
										-wall.end.closestWall.trans().y()	);
				}

			}

		}

	} painter.restore();
}

void TrackerMapObject::setTracker(Tracker *tracker) {
	this->tracker = tracker;
}

