/*
 *  SystemOfWeightsNavigation.h
 *
 *  We consider the SplineNavigation module to be a mechanical method for
 *  controlling a differential steering system, as the entire path for navigation
 *  is pre-determined and then executed. The start and end are given, the execution
 *  is calculated, and then faithfully realized. When the state of the environment
 *  changes, such as an unforeseen obstacle appears, the spline must be altered by
 *  changing or appending navigation points � an undesired effect in the design. An
 *  alternative to the SplineNavigation approach, is the navigation using a system
 *  of weights � the SystemOfWeightsNavigation module. Unlike the spline approach,
 *  the SystemOfWeightsNavigation is completely dynamic and reacts (almost) solely
 *  on the current perception on the state of the environment at that moment of time.
 *  Thus we call it organic, because it constantly adapts to its surrounding
 *  environment. In a sense, it is not interested how it will exactly reach its goal,
 *  but rather how it will at that moment of time in that current state strive towards
 *  its goal.
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

#ifndef SYSTEMOFWEIGHTSNAVIGATION_H_
#define SYSTEMOFWEIGHTSNAVIGATION_H_

#include <QList>

#include "Navigation.h"

class Weight;

class SystemOfWeightsNavigation : public Navigation {

	Q_OBJECT;

private:
	QList<Weight*> *weights;	// Ordered list of weights which build up the virtual formula for the left and right wheel channel
	bool stopAtLastNavPoint;

public:
	SystemOfWeightsNavigation(Create *create, bool createDefaultWeights);
	virtual ~SystemOfWeightsNavigation();
	virtual Vector2D getWheelSpeed(int tick, int interval);
	const QList<Weight*> *getWeights();
	void deleteAllWeights();
	void addWeight(Weight* weight);
	void addWeightFront(Weight* weight);
	bool deleteWeight(QString name);
	bool deleteWeight(Weight* weight);
	void setWeightActive(QString name, bool active = true);
	void setWeightActive(Weight* weight, bool active = true);
	void moveWeightDown(Weight* weight);
	void moveWeightUp(Weight* weight);

signals:
	void weightsChanged();

};

#endif /* SYSTEMOFWEIGHTSNAVIGATION_H_ */
