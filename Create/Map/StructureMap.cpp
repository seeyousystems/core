/*
 * StructureMap.cpp
 *
 *  Created on: 22.03.2009
 *      Author: dgrob
 */

#include "StructureMap.h"

#include "../Library/Math.h"
#include "../MovementTracker/Tracker.h"
#include "../create.h"
#include "../Controller/Controller.h"

StructureMap::StructureMap(Create *create) : Map("StructureMap", create)
{
	finish = false;
}

StructureMap::~StructureMap()
{
}

void StructureMap::registerObjectDetected(double distance, double angle, double opacity, int size) {
	if (finish) return;
	Trafo2D objectLocation = create->tracker->getTransformation() * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance);

	Vector2D p = Vector2D(objectLocation.trans().x(),objectLocation.trans().y());

	appendPoint(p);

}

void StructureMap::paint(QPainter &painter, QRect view) {

	// Translate to our drawing system
	painter.translate(-view.x(), view.height() + view.y());
	QPen pen(Qt::yellow);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(Qt::transparent);

	lock.lockForRead(); {
		for(int i = 0; i < collisions.count()-1; i++){
			int x1 = (int) collisions.at(i).x();
			int y1 = (int) collisions.at(i).y();
			int x2 = (int) collisions.at(i+1).x();
			int y2 = (int) collisions.at(i+1).y();
			painter.drawLine(
					create->mmToPixels(x1),
					-create->mmToPixels(y1),
					create->mmToPixels(x2),
					-create->mmToPixels(y2)
					);
		}
		if(finish){
			painter.drawLine(
					create->mmToPixels(collisions.first().x()),
					-create->mmToPixels(collisions.first().y()),
					create->mmToPixels(collisions.last().x()),
					-create->mmToPixels(collisions.last().y())
					);
		}
	} lock.unlock();

}
void  StructureMap::connectController(QObject *target) {
	connect(target, SIGNAL(signalObjectDetected(double,double,double,int)), this, SLOT(registerObjectDetected(double,double,double,int)));
}

bool StructureMap::isFinish(){
	return finish;
}

bool StructureMap::isPointOutside(Vector2D p){

	// If the StructureMap isn't finish, no Point can be calculated
	if (!finish) return false;

	// Init variables
	double x1, x2;
	int crossings = 0;
	double eps = 0.000001;

	// Check each line
	for(int i = 0; i < collisions.count(); i++){

		// Make sure that it doesn't depend on the direction left to right or right to left.
		if ( collisions.at(i).x() < collisions.at( (i+1) % collisions.count() ).x() ){
			x1 = collisions.at(i).x();
			x2 = collisions.at( (i+1) % collisions.count() ).x();
		}
		else {
			x1 = collisions.at( (i+1) % collisions.count() ).x();
			x2 = collisions.at(i).x();
		}

		// Check if a line is in the range of the checking Point
		if (p.x() > x1 && p.x() <= x2 && ( p.y() < collisions.at(i).y() || p.y() < collisions.at( (i+1)%collisions.count() ).y() )){
			double dx = collisions.at( (i+1)%collisions.count() ).x() - collisions.at(i).x();
			double dy = collisions.at( (i+1)%collisions.count() ).y() - collisions.at(i).y();
			double k = 0;

			if ( std::abs(dx) < eps ){
				k = INT_MAX;
			}
			else {
				k = dy/dx;
			}

			double m = collisions.at(i).y() - (k * collisions.at(i).x());

			double y2 = k * p.x() + m;


			if (p.y() <= y2){
				crossings++;
			}
		}
	}
	if (crossings %2 == 1){
		// Point p is inside the polygon
		return false;
	}
	return true;
}

long StructureMap::width() {
	return 0;
}

long StructureMap::height() {
	return 0;
}

void StructureMap::addStructureMapPoint(Vector2D p){
	appendPoint(p);
}

void StructureMap::clearStructureMapPoints(){
	lock.lockForWrite();
	{
		this->collisions.clear();
	}
	lock.unlock();
}

void StructureMap::setFinish(){
	this->finish = true;
}

int StructureMap::count(){
	int retVal = 0;
	lock.lockForRead();
	{
		retVal = this->collisions.count();
	}
	lock.unlock();

	return retVal;
}

void StructureMap::appendPoint(Vector2D p){
	double distanceBetweenStructurePoints = create->doubleSetting("Map_StructureMap_MinimumDistanceBetweenCollision");
	double finishTolerance = create->doubleSetting("Map_StructureMap_FinishTolerance_mm");

	lock.lockForWrite();
		{
			if (collisions.count() <= 0){
				this->collisions.append(p);
			}
			// Check if the point is the defined distanceBetweenStructerPoints away from the last and the second last
			else if(dist(p, collisions.last()) > distanceBetweenStructurePoints && collisions.count() > 0){
				this->collisions.append(p);
			}
			if (collisions.count() > 1 && dist(collisions.first(), collisions.last()) < finishTolerance){

				// Sort the coordinates
				for (int i = 0; i < collisions.count(); i++){
					int distance = INT_MAX;
					int nearestPoint = 0;
					for(int j = i+1; j < collisions.count(); j++){
						int temp = dist(collisions[i], collisions[j]);
						if (temp < distance){
							distance = temp;
							nearestPoint = j;
						}
					}
					if (i != collisions.count() - 1) {
						Vector2D temp = collisions[i + 1];
						collisions[i + 1] = collisions[nearestPoint];
						collisions[nearestPoint] = temp;
					}

				}

				finish = true;
			}
		}
	lock.unlock();
}
