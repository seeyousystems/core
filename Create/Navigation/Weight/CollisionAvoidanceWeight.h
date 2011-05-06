
/*
 *  CollisionAvoidanceWeight.h
 *
 *  This Weight is responsible for avoiding all kinds of collisions. It must avoid
 *  collisions in the distance (via the IR-sensor), contact collisions (via the bumper),
 *  and drop collisions (via cliff-sensors). In addition to attempting to avoid these
 *  collisions, it also must be able to resolve a situation if a collision was
 *  encountered.
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

#ifndef COLLISIONAVOIDANCEWEIGHT_H_
#define COLLISIONAVOIDANCEWEIGHT_H_

#include "Weight.h"

#include "../../MovementTracker/Tracker.h"

#include "../../Navigation/Navigation.h"

#include "../../Library/Debug.h"
#include "../../Library/Math.h"

#include "../../COIL/COIL.h"

#include "../../Controller/Controller.h"

class CollisionAvoidanceWeight : public Weight {

	Q_OBJECT;

public:
	enum AvoidanceDirection {AvoidanceDirectionLeft, AvoidanceDirectionRight, AvoidanceDirectionAuto};
	enum AvoidanceMode {
							NORMAL,
							OBSTACLE_ENCOUNTER,		// A collision has been encountered, mark where it is and try to decide which way to turn
							OBSTACLE_AVOIDANCE,		// Everything is good, normal drive ahead avoiding any wall if detected
							DROP_AVOIDANCE,			// A drop has been detected, avoid it (reverse)
							BUMPER_AVOIDANCE,		// The bumper was pushed, avoid the obstact (reverse)
						};

private:
	AvoidanceMode mode;								// The current mode
	AvoidanceDirection wantedAvoidDirection;		// The default or wanted avoid direction
	AvoidanceDirection actualAvoidDirection;		// The selected (actual) direction to turn when encountering a collision
	Trafo2D tr1;									// Transformation of robot at t0 (first encounter with obstacle)
	Trafo2D tr2;									// Transformation of robot at t (current position)
	Vector2D cp1;									// Collision point at t0 (first encounter with obstacle)
	Vector2D cp2;									// Collision point at t (newest sensed position of collision)
	Vector2D dropOrBumperCollisionPosition;			// The position of the original collision when trying to decide which direction to turn

public:
	CollisionAvoidanceWeight(Create *create, AvoidanceDirection wantedAvoidDirection) : Weight("CollisionAvoidanceWeight", create) {
		mode = NORMAL;
		actualAvoidDirection = AvoidanceDirectionLeft; // Default attempt
		this->wantedAvoidDirection = wantedAvoidDirection;
	};

	virtual ~CollisionAvoidanceWeight() {};

	virtual void process(Vector2D &v) {

		// Check cliff sensors and bumpers and set mode
		bool cliffFrontLeft = create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_LEFT);
		bool cliffFrontRight = create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_CLIFF_FRONT_RIGHT);
		bool cliffLeft = create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_CLIFF_LEFT);
		bool cliffRight = create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_CLIFF_RIGHT);
		bool bumperLeft = (create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_LEFT) == COIL::BUMPWHEELDROP_BUMP_LEFT;
		bool bumperRight = (create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_BUMPS_AND_WHEEL_DROPS) & COIL::BUMPWHEELDROP_BUMP_RIGHT) == COIL::BUMPWHEELDROP_BUMP_RIGHT;

		// Check if we need to enter a avoidance mode!
		if ( mode != DROP_AVOIDANCE && (cliffFrontLeft || cliffLeft || cliffRight || cliffFrontRight) ){
			mode = DROP_AVOIDANCE;
			dropOrBumperCollisionPosition = create->tracker->getTranslation();
			//Debug::print("[CollisionAvoidanceWeight] mode: DROP_AVOIDANCE");
		} else if ( mode != BUMPER_AVOIDANCE && (bumperLeft || bumperRight) ){
			mode = BUMPER_AVOIDANCE;
			dropOrBumperCollisionPosition = create->tracker->getTranslation();
			Debug::print("[CollisionAvoidanceWeight] mode: OBSTACLE_AVOIDANCE");
		}

		double av = (v.x() + v.y()) / 2.0; // Set our avoidance speed based on the input speed average. So if we are still anyways, we don't have to avoid anything. If we are rotating around our axis, we don't do anything.
		double af = create->doubleSetting("Weight_CollisionAvoidanceWeight_AvoidanceFactor"); // Avoidance factor, determines the threshold for the urgency to avoid an obstacle. 1.0 means you will start to avoid the obstacle as soon as you detect it. 2.0 will avoid the obstacle before you detect it (virtually). 0.5 will only avoid the obstacle once you are half-way to hitting it since detection.
		double reverseDistance = create->doubleSetting("Weight_CollisionAvoidanceWeight_ReverseDistance_mm"); // The distance to reverse, in mm
		double decisionBuffer = create->doubleSetting("Weight_CollisionAvoidanceWeight_DecisionBuffer_mm"); // The buffer distnace in mm which we wait before making a turn direction decision

		double scollision = (double) create->navigation->getMaxOpenAreaDistanceFromMap(Navigation::CollisionSourceFadingCollisionMap, Navigation::CollisionDirectionAhead);
		double smax = create->doubleSetting("Robot_SharpIRSensorCutoffValue");

		// Check status
		if (mode == DROP_AVOIDANCE || mode == BUMPER_AVOIDANCE){

			// Reverse with the averaged speed until we are reverseDistance millimeters away from the dropOrBumperCollisionPosition
			v = Vector2D(-Abs(av),-Abs(av));
			if( dist(dropOrBumperCollisionPosition, create->tracker->getTranslation()) > reverseDistance) mode = OBSTACLE_AVOIDANCE;

		} else if(av > 0) { // Only try to avoid obstacle if moving forward

			// OBSTACLE_AVOIDANCE or NORMAL

			// Check if a collision is detected and our avoidance factor allows us to already avoid it
			if(scollision < smax*af) {

				// We see a collision to avoid!

				// Here we are usually either OBSTACLE_ENCOUNTER or OBSTACLE_AVOIDANCE mode.
				// The NORMAL mode just jumps us into the OBSTACLE_ENCOUNTER mode while registering
				// the initial tr1 and cp1 values...
				// Note that the if statements are all seperate, compared with using if-elseif's.
				// This is so that the next mode "in line" will be processed much faster, rather than
				// having to wait for the next process().

				if(mode == NORMAL) { // was mode == OBSTACLE_AVOIDANCE

					// This must mean it is our first encounter!

					// Do we just have a set direction to turn, or do we need to make a decision?
					if(wantedAvoidDirection == AvoidanceDirectionAuto) {

						// Here we register our tr1 and cp1 values and set the mode to OBSTACLE_ENCOUNTER
						tr1 = create->tracker->getTransformation();
						cp1 = (create->tracker->getTransformation() * Trafo2D::trans(0, scollision)).trans();

						// Jump to OBSTACLE_ENCOUNTER mode as we have encountered the wall now but
						// are not sure yet how to avoid it...
						mode = OBSTACLE_ENCOUNTER;
						//Debug::print("[CollisionAvoidanceWeight] mode: OBSTACLE_ENCOUNTER");

					} else {

						// Just set the
						actualAvoidDirection = wantedAvoidDirection;

						// Jump to OBSTACLE_AVOIDANCE mode as we have encountered the wall now
						// and also know how to avoid it
						mode = OBSTACLE_AVOIDANCE;
						//Debug::print("[CollisionAvoidanceWeight] mode: OBSTACLE_AVOIDANCE");
					}


				}

				if(mode == OBSTACLE_ENCOUNTER) {

					// OBSTACLE_ENCOUNTER mode!

					// Get the newest tr2 and cp2 value
					tr2 = create->tracker->getTransformation();
					cp2 = (create->tracker->getTransformation() * Trafo2D::trans(0, scollision)).trans();


					// Only make a decision once we have driven at least x millimeters
					if(dist(tr1.trans(), tr2.trans()) > decisionBuffer) {

						//
						Vector2D cp1cp2 = cp2 - cp1;
						Vector2D cp1tr1 = tr1.trans() - cp1;
						double beta = Deg(acosf(dot(normalize(cp1tr1), normalize(cp1cp2))));
						if(beta < 90) {
							actualAvoidDirection = (actualAvoidDirection == AvoidanceDirectionLeft ? AvoidanceDirectionRight : AvoidanceDirectionLeft);
						}
						//Debug::print("[CollisionAvoidanceWeight]: change avoidance dir to %1", actualAvoidDirection == AvoidanceDirectionLeft ? "left" : "right");

						mode = OBSTACLE_AVOIDANCE;
						//Debug::print("[CollisionAvoidanceWeight] mode: OBSTACLE_AVOIDANCE");
					}

				}

				// Always avoid the wall. We must do this even if there is no
				// decision yet on where to turn, otherwise we will drive towards
				// the same point on the wall and will never be able to make
				// a good decision. If the AvoidanceDirectionAuto is set, then
				// we drive here by default the last direction we chose. This is
				// good because very possible we are driving against the last wall
				// we saw so the decision will still be valid.
				if(actualAvoidDirection == AvoidanceDirectionLeft) {
					v = Vector2D(av * (scollision/(smax*af)), av); // left
				} else {
					v = Vector2D(av, av * (scollision/(smax*af))); // right
				}

			} else {

				// There is no collision to avoid!

				// Reset the back to normal if there is no WALL sensed...
				//if(mode != NORMAL) Debug::print("[CollisionAvoidanceWeight] mode: NORMAL");
				mode = NORMAL;

			}

		}



	};
};

#endif /* COLLISIONAVOIDANCEWEIGHT_H_ */
