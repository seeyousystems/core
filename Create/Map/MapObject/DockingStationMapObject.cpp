/*
 *  DockingStationMapObject.cpp
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

#include "DockingStationMapObject.h"


DockingStationMapObject::DockingStationMapObject(ObjectMap *map, DockingStation *dockingStation) : MapObject(map, "DockingStationObject", 0, 0, 0, 0) {
	this->dockingStation = dockingStation;
}

void DockingStationMapObject::paint(QPainter &painter, QRect view, int scale) {

	if(!dockingStation) return;

	// Save the painter state for this paint session
	// so that it can be restored after without influencing other objects
	painter.save(); {

		// Translate, scale, and rotate...
		QTransform t;
		t.scale(1.0/scale, 1.0/scale);
		t.translate(dockingStation->position.trans().x() - view.x()*scale, view.height()*scale - dockingStation->position.trans().y() + view.y()*scale);
		t.rotate(-Deg(dockingStation->position.angle()));
		painter.setTransform(t, true);

		// Draw body

		// Set clipping for wall edge
		painter.setClipRect(-dockingStation->baseSize, -dockingStation->baseSize*0.35/2, 2*dockingStation->baseSize, 2*dockingStation->baseSize);
		painter.setOpacity(1.0);

		// Draw Base
		painter.setBrush(QBrush(Qt::gray));
		painter.drawEllipse(QRect(-dockingStation->baseSize/2, -dockingStation->baseSize*0.8/2, dockingStation->baseSize, dockingStation->baseSize*0.8));

		// Draw backsupport
		painter.setBrush(QBrush(Qt::darkGray));
		painter.drawEllipse(QRect(-dockingStation->baseSize/2, -dockingStation->baseSize*0.5/2, dockingStation->baseSize, dockingStation->baseSize*0.3));
		painter.setClipping(false);

		// Translate to IR ob
		painter.translate(0, -dockingStation->baseSize*0.35/2);

		// Draw IR nob
		painter.drawEllipse(QRect(-dockingStation->baseSize*0.18/2, 0, dockingStation->baseSize*0.18, dockingStation->baseSize*0.18));

		// Force field
		painter.setOpacity(0.2);
		painter.setPen(Qt::transparent);
		if(dockingStation->enabled && dockingStation->forceFieldEnabled) {
			painter.setBrush(dockingStation->interferenceEnabled ? Qt::white : Qt::blue);
			painter.drawEllipse(QRect(-dockingStation->radiusForceField, -dockingStation->radiusForceField, dockingStation->radiusForceField*2, dockingStation->radiusForceField*2));
		}

		// Green beam
		painter.rotate(dockingStation->angleRedGreenBeam);
		if(dockingStation->enabled && dockingStation->greenBeamEnabled) {
			painter.setBrush(dockingStation->interferenceEnabled ? Qt::white : Qt::green);
			painter.drawEllipse(QRect(-dockingStation->widthRedGreenBeam/2-dockingStation->offsetRedGreenBeam, 0, dockingStation->widthRedGreenBeam, dockingStation->lengthRedGreenBeam));
		}

		// Red beam
		painter.rotate(-dockingStation->angleRedGreenBeam*2);
		if(dockingStation->enabled && dockingStation->redBeamEnabled) {
			painter.setBrush(dockingStation->interferenceEnabled ? Qt::white : Qt::red);
			painter.drawEllipse(QRect(-dockingStation->widthRedGreenBeam/2+dockingStation->offsetRedGreenBeam, 0, dockingStation->widthRedGreenBeam, dockingStation->lengthRedGreenBeam));
		}

	} painter.restore();



}
