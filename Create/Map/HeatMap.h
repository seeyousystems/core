/*
 *  HeatMap.h
 *
 *  A Heat Map is built to visualize the discovered environment of the robot.
 *  When the robot moves through its environment, the Movement Tracker is
 *  responsible for translating the sensory data into physical movements
 *  through space. In turn, the Movement Tracker sends signals to the
 *  appropriate Maps, such as the Heat Map, registering various kinds of
 *  �heat�. The emss Heat Map support two kinds of heat: Open Area and
 *  Collision Area. Open Area is displayed as green, and portrays the robots
 *  path through the environment, while Collision Area is displayed as red,
 *  and represents any sort of collision or obstacle, forming the environments
 *  boundary. The Heat Map can be consulted by different emss components in
 *  order to make decisions, such as navigational choices. In addition, other
 *  influences, such as a user, can also �color� the Heat Map for diagnostic
 *  reasons such as staking out a territory.
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

#ifndef HEATMAP_H_
#define HEATMAP_H_

#include <QImage>

#include "ColorMap.h"

#include "../Library/Math.h"

class HeatMap : public ColorMap
{
	Q_OBJECT

private:
	QMutex _mutex;
	double distance;
	Vector2D lastCollisionAreaPosition;
	Vector2D lastOpenAreaPosition;


public:
	HeatMap(Create *create, long width, long height);
	virtual ~HeatMap();
	void connectController(QObject *target);

public slots:
	void registerHeat(ColorMap::MapChannel channel, long x, long y);
	void registerHeat(ColorMap::MapChannel channel, long x, long y, double opacity, long size);
	void registerHeat(ColorMap::MapChannel channel, Trafo2D position, double distance, double angle, double opacity, int size);
	void registerCollisionAtTrackerLocation(double distance, double angle, double opacity, int size);

};

#endif /*HEATMAP_H_*/
