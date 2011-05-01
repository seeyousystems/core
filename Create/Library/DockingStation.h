/*
 * DockingStation.h
 *
 *
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

#ifndef DOCKINGSTATION_H_
#define DOCKINGSTATION_H_

#include "../CoreObject.h"

#include "../Library/Debug.h"
#include "../Library/Math.h"

#include "../MovementTracker/Tracker.h"

#include "../COIL/COIL.h"

class DockingStation : public CoreObject {

	Q_OBJECT

public:
	Trafo2D position;
	double baseSize; // mm
	double radiusForceField; // mm
	double widthRedGreenBeam; // mm
	double lengthRedGreenBeam; // mm
	double offsetRedGreenBeam; // mm
	double angleRedGreenBeam; // deg
	double minimumDockingPrecision; // mm
	bool enabled;
	bool redBeamEnabled;
	bool greenBeamEnabled;
	bool forceFieldEnabled;
	bool interferenceEnabled;

public:
	DockingStation(Create *create) : CoreObject("DockingStation", create) {
		baseSize = create->doubleSetting("Robot_DockingStation_Base_Size_mm");
		radiusForceField = create->doubleSetting("Robot_DockingStation_ForceField_Radius_mm");
		widthRedGreenBeam = create->doubleSetting("Robot_DockingStation_RedGreenBeam_Width_mm");
		lengthRedGreenBeam = create->doubleSetting("Robot_DockingStation_RedGreenBeam_Length_mm");
		angleRedGreenBeam = create->doubleSetting("Robot_DockingStation_RedGreenBeam_Angle");
		offsetRedGreenBeam = create->doubleSetting("Robot_DockingStation_RedGreenBeam_Offset_mm");
		minimumDockingPrecision  = create->doubleSetting("Robot_DockingStation_MinimumDockingPrecision_mm");
		enabled = create->boolSetting("Robot_DockingStation_Enabled");
		redBeamEnabled = create->boolSetting("Robot_DockingStation_RedBeamEnabled");;
		greenBeamEnabled = create->boolSetting("Robot_DockingStation_GreenBeamEnabled");;
		forceFieldEnabled = create->boolSetting("Robot_DockingStation_ForceFieldEnabled");;
		interferenceEnabled = create->boolSetting("Robot_DockingStation_InterferenceEnabled");;
	};

	~DockingStation() {};

	void positionDockingStationUnderneathRobot() {
		Trafo2D offset = Trafo2D::trans(create->doubleSetting("Robot_DockingStation_OffsetX_mm"), create->doubleSetting("Robot_DockingStation_OffsetY_mm"));
		Trafo2D robot = create->tracker->getTransformation();
		Trafo2D rot = Trafo2D::rot(Rad(create->doubleSetting("Robot_DockingStation_OffsetRotation_deg")));
		position = robot*(rot*offset);
	}

	void positionRobotOntopDockingStation() {
		Trafo2D offset = Trafo2D::trans(create->doubleSetting("Robot_DockingStation_OffsetX_mm"), create->doubleSetting("Robot_DockingStation_OffsetY_mm"));
		Trafo2D rot = Trafo2D::rot(Rad(create->doubleSetting("Robot_DockingStation_OffsetRotation_deg")));
		Trafo2D t = Trafo2D::trans(0,-create->doubleSetting("Robot_Diameter_mm"));
		create->tracker->setTransformation(position*(rot*offset*t));
	}

	int getEmulatedIRSignalForPoint(Vector2D point) {

		if(!enabled) return 0;

		// Init
		bool forcefield = false;
		bool redbeam = false;
		bool greenbeam = false;

		// Forcefield signal?
		// This is easy, just check if the point lies within radius distance
		if(enabled && forceFieldEnabled && dist(point, position.trans()) < radiusForceField) {
			if(interferenceEnabled) return COIL::IR_SIGNAL_HOMEBASE_RESERVED;
			forcefield = true;
		}

		// For the red and green beam, we have to check if the point lies within a
		// angled ellipse. To check if a point is within an ellipse, we use
		// the following reasoning:
		//
		// If ((x-x0)/a)^2+((y-y0)/b)^2 <= 1 then (x,y) point lies inside ellipse.
		// If ellipse is defined by bounding rectangle, then
		// x0:=(x1+x2)/2
		// y0:=(y1+y2)/2
		// a:=(x2-x1)/2
		// b:=(y2-y1)/2
		//
		// That is, if the ellipse is not angled (rotated). For the rotation
		// instead of rotating the ellipse, we just rotate the point in the counter
		// direction. We do this by first getting a transformation matrix which is just
		// the transformation of the point relative to the center of the docking station.
		// Then we create the ellipse relative to the center of the docking station and
		// check if the transformed point lies in that ellipse. :).

		// Green beam signal?
		if(enabled && greenBeamEnabled) {
			Trafo2D t = Trafo2D::rot(Rad(angleRedGreenBeam) - position.angle()) * Trafo2D::trans(point - position.trans() - Vector2D(-offsetRedGreenBeam,0));
			double x1 = -widthRedGreenBeam / 2;
			double x2 = +widthRedGreenBeam / 2;
			double y1 = -lengthRedGreenBeam;
			double y2 = 0;
			double x0 = (x1+x2)/2.0;
			double y0 = (y1+y2)/2.0;
			double a = (x2-x1)/2.0;
			double b = (y2-y1)/2.0;
			double aa = (t.trans().x()-x0)/a;
			double bb = (t.trans().y()-y0)/b;
			if ( aa*aa + bb*bb <= 1 ) {
				if(interferenceEnabled) return COIL::IR_SIGNAL_HOMEBASE_RESERVED;
				greenbeam = true;
			}
		}

		// Red beam signal?
		if(enabled && redBeamEnabled) {
			Trafo2D t = Trafo2D::rot(Rad(-angleRedGreenBeam) - position.angle()) * Trafo2D::trans(point - position.trans() - Vector2D(offsetRedGreenBeam,0));
			double x1 = -widthRedGreenBeam / 2;
			double x2 = +widthRedGreenBeam / 2;
			double y1 = -lengthRedGreenBeam;
			double y2 = 0;
			double x0 = (x1+x2)/2.0;
			double y0 = (y1+y2)/2.0;
			double a = (x2-x1)/2.0;
			double b = (y2-y1)/2.0;
			double aa = (t.trans().x()-x0)/a;
			double bb = (t.trans().y()-y0)/b;
			if ( aa*aa + bb*bb <= 1 ) {
				if(interferenceEnabled) return COIL::IR_SIGNAL_HOMEBASE_RESERVED;
				redbeam = true;
			}
		}

		// Convert to iRobot create IR byte
		if(forcefield && !redbeam && !greenbeam) 		return COIL::IR_SIGNAL_HOMEBASE_FORCEFIELD;
		else if(forcefield && redbeam && !greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_REDBUOY_FORCEFIELD;
		else if(forcefield && !redbeam && greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_GREENBUOY_FORCEFIELD;
		else if(!forcefield && redbeam && !greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_REDBUOY;
		else if(!forcefield && !redbeam && greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_GREENBUOY;
		else if(!forcefield && redbeam && greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY;
		else if(forcefield && redbeam && greenbeam) 	return COIL::IR_SIGNAL_HOMEBASE_REDBUOY_GREENBUOY_FORCEFIELD;
		else											return 0;
	}

};

#endif /* DOCKINGSTATION_H_ */
