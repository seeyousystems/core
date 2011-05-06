/*
 * RightWallFollowerWeight.h
 *
 *  Created on: May 2, 2011
 *      Author: seeyou
 */

#ifndef RIGHTWALLFOLLOWERWEIGHT_H_
#define RIGHTWALLFOLLOWERWEIGHT_H_

#include "Weight.h"

#include "../../COIL/COIL.h"
#include "../../Controller/Controller.h"
#include "../../Controller/ArduinoController.h"
#include "../../COIL/ArduinoCOIL.h"

#include "../Navigation.h"

#include "../../Task/WallFollowerTask.h"
#include "../../Task/TaskManager.h"
#include "../../MovementTracker/Tracker.h"

#include "../../Library/Math.h"

#include "../../Library/Debug.h"


class RightWallFollowerWeight : public Weight {

	Q_OBJECT;

private:
	double sensitivity;
	double stepSize;
	QTime noWallTime;
	QTime wallSearchTime;
	int noWallTimer;
	int wallSearchTimer;

public:
	RightWallFollowerWeight(Create *create) : Weight("RightWallFollowerWeight", create) {
		stepSize = create->doubleSetting("Weight_WallFollowerWeight_StepSize");
		noWallTimer = create->intSetting("Weight_WallFollowerWeight_NoWallTimer_Sec");
		wallSearchTimer = create->intSetting("Weight_WallFollowerWeight_SearchWallTimer_Sec");
		sensitivity = create->doubleSetting("Weight_WallFollowerWeight_Sensitivity");
		noWallTime = QTime(0,0,0,0);
		wallSearchTime = QTime(0,0,0,0);
	};

	virtual ~RightWallFollowerWeight() {};

	virtual void preProcess() {
		noWallTime = QTime::currentTime();
		wallSearchTime = QTime::currentTime();
	}

	virtual void process(Vector2D &v) {

		// Robot_SharpIRSensorCutoffValue=800
		double scollision = (double) create->navigation->getMaxOpenAreaDistanceFromMap(Navigation::CollisionSourceFadingCollisionMap, Navigation::CollisionDirectionAhead);
		double smax = create->doubleSetting("Robot_SharpIRSensorCutoffValue");

		// Is IR-Sensor triggered?
		if( scollision < smax )
		{
			// Yes

			Debug::print("[RightWallFollowerWeight] IR sensor triggered");

			// Reset sensitivity
			sensitivity = create->doubleSetting("Weight_WallFollowerWeight_Sensitivity");

			// Avoid wall ahead

		} else {

			// No

			// Robot_WallSensorRange_mm=210
			//double swall = (double) create->coil->getWallSensorDistanceFromSignal(create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_WALL_SIGNAL));
			double swall = (double) create->arduino->extractSensorFromData(create->arduinoController->sensorData, ArduinoCOIL::SENSOR_LEFT_PINGER);
			swall *= 10;
			double swallmax= create->doubleSetting("Robot_WallSensorRange_mm");

			// Is wall sensor triggered?
			//if(create->coil->extractSensorFromData(create->controller->sensorData, COIL::SENSOR_WALL))
			if(swall <= 220)
			{
				// Yes

				//Debug::print("[RightWallFollowerWeight] wall sensor triggered");

				// Reset timers
				noWallTime = QTime::currentTime();
				wallSearchTime = QTime::currentTime();

				// Decrease sensitivity
				if ( sensitivity > create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Min") ) sensitivity -= stepSize;

				// Peer left - Peer right
				//v = Vector2D(v.x() * (1-sensitivity) , v.y());
				v = Vector2D(v.x(), v.y() * (1-sensitivity));


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

						Debug::print("[RightWallFollowerWeight] search wall timeout");
						//Full speed ahead

					} else {

						// No
						Debug::print("[RightWallFollowerWeight] nowall timer timeout");

						// Increase sensitivity
						if ( sensitivity < create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Max") ) sensitivity += stepSize;

						// Peer right - peer left
						//v = Vector2D(v.x() , v.y()  * (1-sensitivity) );
						v = Vector2D(v.x() * (1-sensitivity) , v.y() );
					}

				} else {

					// No

					Debug::print("[RightWallFollowerWeight] no timer / sensor triggered");

					// Decrease sensitivity
					if ( sensitivity > create->doubleSetting("Weight_WallFollowerWeight_Sensitivity_Min") ) sensitivity -= stepSize;

					// Peer right - peer left
					// v = Vector2D(v.x() , v.y()  * (1-sensitivity) );
					v = Vector2D(v.x() * (1-sensitivity) , v.y() );

				}
			}
		}
	};
};

#endif /* RIGHTWALLFOLLOWERWEIGHT_H_ */
