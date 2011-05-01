/*
 *  MarkerObjectMap.cpp
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

#include "MarkerMapObject.h"

#include "../../Library/Debug.h"

void MarkerMapObject::paint(QPainter &painter, QRect view, int scale) {

	// Save the painter state for this paint session
	// so that it can be restored after without influencing other objects
	painter.save(); {

		// Translate, scale, and rotate...
		QTransform t;
		t.scale(1.0/scale, 1.0/scale);
		t.translate(x - view.x()*scale, view.height()*scale - y + view.y()*scale);
		painter.setTransform(t);

		for(int index = 0; index < transformationCount; index++) {
			long arrowLength = 1000*scale;
			long ballSize = 20*scale;
			Vector2D a = transformation[index].trans();
			Vector2D b = (transformation[index] * Trafo2D::trans(0, arrowLength)).trans();
			painter.setPen(transformationColors[index]);
			QPen pen = painter.pen();
			pen.setWidth(2*scale);
			painter.setPen(pen);
			painter.drawLine(a.x(), -a.y(), b.x(), -b.y());
			painter.setBrush(QBrush(transformationColors[index]));
			painter.drawEllipse(a.x()-ballSize/2, -(a.y()+ballSize/2), ballSize, ballSize);
		}

	} painter.restore();
}

void MarkerMapObject::setAsVector(int index, Vector2D vector) {

}

void MarkerMapObject::setAsPoint(int index, long x, long y) {

}

void MarkerMapObject::setAsTransformation(int index, Trafo2D transformation, QColor color) {
	this->transformation[index] = transformation;
	this->transformationColors[index] = color;
	if(index+1 > transformationCount) transformationCount = index+1;
}


