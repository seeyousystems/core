/*
 *  SelfCorrectingTracker.cpp
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


#include "SelfCorrectingTracker.h"

//#include "../Map/PhysicalMap.h"
#include "../Controller/Controller.h"
#include "../Library/Math.h"

// TODO: Double check the magic jump maths to be sure the correction is good...

SelfCorrectingTracker::SelfCorrectingTracker(Create *create) : SingleTracker(create, "SelfCorrectingTracker") {
	// Init
	correlationLog = new QList<CorrelationTriangle>();
	suspectedWall.valid = false;
}

SelfCorrectingTracker::~SelfCorrectingTracker(){
	delete correlationLog;
}


void SelfCorrectingTracker::registerObjectDetected(double distance, double angle, double opacity, int size) {

	// Get current position and corresponding collision point
	CorrelationTriangle ct; // ct, correlation triangle
	ct.position = create->tracker->getTransformation();
	ct.collision = ct.position * Trafo2D::rot(Rad(angle)) * Trafo2D::trans(0, distance);

	// Make sure it is time to process the check, i.e. only
	// check every so often when moving through space..
	if(correlationLog->count() != 0 && dist(ct.position.trans(),correlationLog->last().position.trans()) < create->doubleSetting("Tracker_SelfCorrectingTracker_CorrelationLog_MinimumDistance_mm")) return;

	// Find closest 'wall' (ct.closestWall) to ct.collision.
	// This is done by 'spiraling' out from the center (ct.collision)...
	double r = 0;
	double rmax = 2000;
	double rstep = 10;
	double a = 0;
	double amax = 360;
	double astep = 10;
	bool found = false;
	while(!found && r < rmax) {
		a = 0;
		while(!found && a < amax) {
			Trafo2D pos = ct.collision * Trafo2D::rot(Rad(a)) * Trafo2D::trans(0, r);
//			if(create->physicalMap->getAreaType(pos.trans().x(), pos.trans().y()) != PhysicalMap::PhysicalMapAreaTypeOpen) {
//				ct.closestWall = pos;
//				found = true;
//			}
			a += astep;
		}
		r += rstep;
	}
	if(found) {

		// Add the ct to the log -- lock for write!
		lock1.lockForWrite(); {

			correlationLog->append(ct);

		} lock1.unlock();

		// Determine whether we have two straight lines in our log made
		// up of ct.collision and ct.closestWall. We need at least n ct's
		// to do this... -- Lock for read!
		lock1.lockForRead(); {

			// Init
			int n = 5;
			double maxLineDeviation = 40; //mm
			QList<CorrelationTriangle>::const_iterator i;
			int nn;

			if(correlationLog->count() >= n) {

				// Get start and end points for the two lines
				SuspectedWall wall;
				wall.valid = true;

				// Find the start and end point of the targeted line
				if(false) {

					// Just take the first and last ct!
					wall.start = correlationLog->at(correlationLog->count()-1);
					wall.end = correlationLog->at(correlationLog->count()-n);

				} else {

					// Detect the most distant two points in ct.collision.
					// This would be O(n^2) but we optimize by first caluclating the
					// bounding box, then taking the points which are on the bounds, and
					// finally finding the most distant of those. Best case is O(n+n+1),
					// worst case is O(n+n+n^2). The worst can only happen if all points
					// are on a straight vertical or horizontal line.

					// Get the bounding box of the ct points
					double xmin = INT_MAX;
					double xmax = INT_MIN;
					double ymin = INT_MAX;
					double ymax = INT_MIN;
					i = correlationLog->end();
					nn = n;
					while (i != correlationLog->constBegin() && nn > 0) {
						CorrelationTriangle ct = *--i; nn--;
						if(ct.collision.trans().x() > xmax) xmax = ct.collision.trans().x();
						else if(ct.collision.trans().x() < xmin) xmin = ct.collision.trans().x();
						if(ct.collision.trans().y() > ymax) ymax = ct.collision.trans().y();
						else if(ct.collision.trans().y() < ymin) ymin = ct.collision.trans().y();
					}

					// Get all the ct points which ly on the bounds
					i = correlationLog->end();
					nn = n;
					QList<CorrelationTriangle> candidates;
					while (i != correlationLog->constBegin() && nn > 0) {
						CorrelationTriangle ct = *--i; nn--;
						if(		ct.collision.trans().x() == xmin ||
								ct.collision.trans().x() == xmax ||
								ct.collision.trans().y() == ymin ||
								ct.collision.trans().y() == ymax	) {
							// Lies on the bounds, add to candidates
							candidates.append(ct);
						}
					}

					// Now we have our candidates - find the most distant ct points!
					i = candidates.constBegin();
					double maxdist = 0;
					while (i != candidates.constEnd()) {
						CorrelationTriangle cti = *i++;
						QList<CorrelationTriangle>::const_iterator j = candidates.constBegin();
						while (j != candidates.constEnd()) {
							CorrelationTriangle ctj = *j++;
							double distij = dist( cti.collision.trans(), ctj.collision.trans() );
							if( distij > maxdist ) {
								maxdist = distij;
								wall.start = cti;
								wall.end = ctj;
							}
						}
					}
				}

				// Here we iterate through all points between wall.start and wall.end
				// and try to determine if this is a wall. Basically, this is done by
				// just looking at the points and seeing if they all fall on a line
				// between wall.start and wall.end. We actually have two lines:
				// lineWall: the line along the detected closestWall (from physical map)
				// lineCollision: the line along the detected object or collision points (from sensor)
				//
				// Reference: http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
				nn = n;
				i = correlationLog->end();
				while (wall.valid && i != correlationLog->constBegin() && nn > 0) {

					// Grab a ct from the correlationLog...
					CorrelationTriangle ct = *--i; nn--;

					// Get the distance of this ct point from the line. If the
					// distance is too large, then the suspected wall is invalid, as
					// the points do not fall on a straight line (what we assume to be
					// a wall.
					// Formula for distance: d = ( | det(x_end - x_start, x_start - x_ct) | ) / ( | x_end - x_start | )
					double dw = ( det(wall.end.closestWall.trans() - wall.start.closestWall.trans(), wall.start.closestWall.trans() - ct.closestWall.trans()) ) / ( wall.end.closestWall.trans().x() - wall.start.closestWall.trans().x() );
					double dc = ( det(wall.end.collision.trans() - wall.start.collision.trans(), wall.start.collision.trans() - ct.collision.trans()) ) / ( wall.end.collision.trans().x() - wall.start.collision.trans().x() );
					if(dw > maxLineDeviation || dw < -maxLineDeviation || dc > maxLineDeviation || dc < -maxLineDeviation) {
						wall.valid = false;
						break;
					}

				}

				// Did we detect a wall?
				if(wall.valid) {

					// We have a valid wall - everything looks good for a magical 'jump'!
					// Lock lock for a change in the Trafo2D transfomration object
					//lock.lockForWrite(); {

						// Get the current transfomration
						MovementTracker *selectedMovementTracker = getSelectedMovementTracker();
						Trafo2D t = selectedMovementTracker->getTransformation();

						// Get the distance to go back and forwards again
						double d = dist (wall.end.collision.trans(), wall.start.collision.trans());

						// Get the angle to correct
						Vector2D lc(wall.end.collision.trans().x()-wall.start.collision.trans().x(), wall.end.collision.trans().y()-wall.start.collision.trans().y());
						Vector2D lw(wall.end.closestWall.trans().x()-wall.start.closestWall.trans().x(), wall.end.closestWall.trans().y()-wall.start.closestWall.trans().y());
						double alpha = atan2(lw.y(),lw.x()) - atan2(lc.y(),lc.x());

						// Perform the correction
						t = t * Trafo2D::trans( 0, -d ) * Trafo2D::rot(alpha) * Trafo2D::trans( 0, d );
						selectedMovementTracker->setTransformation(t);
						Debug::print("[SelfCorrectingTracker] corrected with d=%1, alpha=%2",d,Deg(alpha));

					//} lock.unlock();

				}

				// Update the suspected wall!
				lock2.lockForWrite(); {
					suspectedWall = wall;
				} lock2.unlock();

			}

		} lock1.unlock();

	}
}

// Returns a portion of the correlation log n items counting back from the end.
// That means the returned list contains items t to t-n starting at t (the newest move).
QList<CorrelationTriangle> SelfCorrectingTracker::getCorrelationLogSnapshot(int n) {

	QReadLocker locker(&lock1);

	QList<CorrelationTriangle>::const_iterator i = correlationLog->end();
	QList<CorrelationTriangle> snapshot;
	while (correlationLog->count() > 0 && i != correlationLog->constBegin() && n > 0) {
		snapshot.append(*--i);
		n--;
	}
	return snapshot;
}

SuspectedWall SelfCorrectingTracker::getCurrentlySuspectedWall() {

	QReadLocker locker(&lock2);

	return suspectedWall;
}

