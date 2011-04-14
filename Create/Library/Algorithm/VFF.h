/*
 * VFF.h
 *
 *  Created on: Jun 27, 2010
 *      Author: Abraham
 */

#ifndef VFF_H_
#define VFF_H_
//inline string convertInt( int number )
//{
//    stringstream ss;
//    ss << number;
//    return ss.str();
//}

#include <iostream>
#include <stdint.h>
#include <vector>


#include "Grid.h"

using namespace std;

// Minimum and Maximum distance sensors are limited to
const int RMin = 15;
const int RMax = 227;

class VFF
{
public:
	Grid hist;

    VFF();
    ~VFF();

    void run();
    void run( int ir0, int ir1, int ir2, int ir3, int ir4, int globalX, int globalY );
    void createHashTables();
    void createHashTables( int numberOfSensors, int fieldOfView, int offSet );

private:

    vector <double> s;
    //Grid hist;

    //TODO: Move all vectors to a single 2D vector
    vector<POINT> SensorList;

    vector<POINT> vectorOfSensors;
    typedef vector<POINT> newType;
    vector<newType> sArray;

    vector<POINT> sensor_0;
    vector<POINT> sensor_45;
    vector<POINT> sensor_90;
    vector<POINT> sensor_135;
    vector<POINT> sensor_180;
};

#endif /* VFF_H_ */
