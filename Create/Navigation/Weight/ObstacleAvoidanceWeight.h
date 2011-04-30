/*
 * ObstacleAvoidance.h
 *
 *  Created on: Apr 29, 2011
 *      Author: seeyou
 */

#ifndef OBSTACLEAVOIDANCE_H_
#define OBSTACLEAVOIDANCE_H_

#include "Weight.h"

#include "../../MovementTracker/Tracker.h"

#include "../../Navigation/Navigation.h"

#include "../../Library/Debug.h"
#include "../../Library/Math.h"

#include "../../COIL/COIL.h"

#include "../../Controller/Controller.h"

class ObstacleAvoidanceWeight : public Weight {

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
	AvoidanceMode mode;
	AvoidanceDirection wantedAvoidDirection;		// The default or wanted avoid direction
	AvoidanceDirection actualAvoidDirection;		// The selected (actual) direction to turn when encountering a collision
	Trafo2D tr1;									// Transformation of robot at t0 (first encounter with obstacle)
	Trafo2D tr2;									// Transformation of robot at t (current position)
	Vector2D cp1;									// Collision point at t0 (first encounter with obstacle)
	Vector2D cp2;									// Collision point at t (newest sensed position of collision)
	Vector2D dropOrBumperCollisionPosition;			// The position of the original collision when trying to decide which direction to turn

public:
	ObstacleAvoidanceWeight(Create *create, AvoidanceDirection wantedAvoidDirection) : Weight("ObstacleAvoidanceWeight", create) {
		mode = NORMAL;
		actualAvoidDirection = AvoidanceDirectionLeft; // Default attempt
		this->wantedAvoidDirection = wantedAvoidDirection;
	};

	virtual ~ObstacleAvoidanceWeight() {};

	virtual void process(Vector2D &v) {

	};

};


#endif /* OBSTACLEAVOIDANCE_H_ */
