/*
 *  WallFollowerWeight.h
 *
 *  This Weight is used to follow walls. The first step is to find the wall.
 *  Because the robot may not have any idea about the current structure of the
 *  room, it begins by driving straight ahead until it detects an obstacle. At
 *  this point, if the obstacle was detected using the forward-looking IR-sensor,
 *  the obstacle must be avoided. If, however, the obstacle was detected using
 *  the wall-sensor, this obstacle (presumed to be a wall) must be followed. When
 *  such a wall has been found, the WallFollowerWeight will continuously go back
 *  and forth between seeing and not-seeing the wall, trying to navigate along
 *  the wall at the edge of the wall-sensors range. If the wall is lost or unseen
 *  for a longer period of time, the WallFollowerWeight will restart, searching for
 *  a new wall.
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

#ifndef WALLFOLLOWERWEIGHT_H_
#define WALLFOLLOWERWEIGHT_H_

#include "Weight.h"

#include "../../COIL/COIL.h"
#include "../../Controller/Controller.h"
#include "../Navigation.h"

#include "../../Task/WallFollowerTask.h"
#include "../../Task/TaskManager.h"
#include "../../MovementTracker/Tracker.h"

#include "../../Library/Math.h"

#include "../../Library/Debug.h"


class WallFollowerWeight : public Weight {

	Q_OBJECT;

private:
	double sensitivity;
	double stepSize;
	QTime noWallTime;
	QTime wallSearchTime;
	int noWallTimer;
	int wallSearchTimer;

public:
	WallFollowerWeight(Create *create) : Weight("WallFollowerWeight", create) {
		stepSize = create->doubleSetting("Weight_WallFollowerWeight_StepSize");
		noWallTimer = create->intSetting("Weight_WallFollowerWeight_NoWallTimer_Sec");
		wallSearchTimer = create->intSetting("Weight_WallFollowerWeight_SearchWallTimer_Sec");
		sensitivity = create->doubleSetting("Weight_WallFollowerWeight_Sensitivity");
		noWallTime = QTime(0,0,0,0);
		wallSearchTime = QTime(0,0,0,0);
	};

	virtual ~WallFollowerWeight() {};

	virtual void preProcess() {
		noWallTime = QTime::currentTime();
		wallSearchTime = QTime::currentTime();
	}

	virtual void process(Vector2D &v) {

		double scollision = (double) create->navigation->getMaxOpenAreaDistanceFromMap(Navigation::CollisionSourceFadingCollisionMap, Navigation::CollisionDirectionAhead);
		double smax = create->doubleSetting("Robot_SharpIRSensorCutoffValue");

		// Is IR-Sensor triggered?
		if( scollision < smax )
		{
			// Yes

			Debug::print("[WallFollowerWeight] IR sensor triggered");

			// Reset sensitivity
			sensitivity = create->doubleSetting("Weight_WallFollowerWeight_Sensitivity");

			// Avoid wall ahead

		} else {

			// No

			double swall = (double) create->coil->getWallSensorDistanceFromSignal(create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_WALL_SIGNAL));
			double swallmax= create->doubleSetting("Robot_WallSensorRange_mm");

			// Is wall sensor triggered?
			if(create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_WALL))
			{
				// Yes

				//Debug::print("[WallFollowerWeight] wall sensor triggered");

				// Reset timers
				noWallTime = QTime::currentTime();
				wallSearchTime = QTime::currentTime();

				// Decrease sensitivity
				if ( sensitivity > create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Min") ) sensitivity -= stepSize;

				// Peer left
				v = Vector2D(v.x() * (1-sensitivity) , v.y());

			} else {

				// No

				// NoWall timeout?
				if (noWallTime.secsTo(QTime::currentTime()) > noWallTimer)
				{
					// Yes

					// WallSearch timeout?
					if ( wallSearchTime.secsTo(QTime::currentTime()) > wallSearchTimer)
					{
						// Yes

						Debug::print("[WallFollowerWeight] search wall timeout");
						//Full speed ahead

					} else {

						// No
						Debug::print("[WallFollowerWeight] nowall timer timeout");

						// Increase sensitivity
						if ( sensitivity < create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Max") ) sensitivity += stepSize;

						// Peer right
						v = Vector2D(v.x() , v.y()  * (1-sensitivity) );
					}

				} else {

					// No

					Debug::print("[WallFollowerWeight] no timer / sensor triggered");

					// Decrease sensitivity
					if ( sensitivity > create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Min") ) sensitivity -= stepSize;

					// Peer right
					v = Vector2D(v.x() , v.y()  * (1-sensitivity) );

				}
			}
		}
	};
};


#endif /* WALLFOLLOWERWEIGHT_H_ */
