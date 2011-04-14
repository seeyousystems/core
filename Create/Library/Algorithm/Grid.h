/*
 * Grid.h
 *
 *  Created on: Jun 19, 2010
 *      Author: Abraham Gallardo
 */

#ifndef GRID_H_
#define GRID_H_

#include <iostream>
#include <vector>

using namespace std;

// PI constant
const double _PI = 3.141592653589793238462643383279f;
const double TO_DEGREES = _PI / 180.0;

// Constants for metric system
const double IN_TO_CM = 2.54f;
const double CM_TO_IN = 0.393700787f;

// Point struct to represent x and y
typedef struct
{
    int x;
    int y;
} POINT;

// iRobot Positions
const double iRobotDiameterIn = 13.00;
const double iRobotDiameterCm = 33.00;
const double iRobotRadiusIn = 6.5;
const double iRobotRadiusCm = 16.5;

// Maxbotix LV-EZ1 Field of View (FOV)
const int FOV = 36;
const int FOV_AXIS = FOV / 2;

// Certainty Values limits
const int CVMin = 0;
const int CVMax = 15;

// Certainty increment/decrement constants
const int CVIinc = 3;
const int CVIdec = -1;

// Force constant (repelling)
const int F_CR = 5000;

// Present coordinates of robot
const int X0 = 0;
const int Y0 = 0;

class Grid
{
public:
	 double sensorAngle;
	 double oMega;

    //---- Constructor
    Grid();
    //Grid( int width, double cellWidth);
    Grid( double widthOfCell, int widthOfGrid, double angleOfSensor );

    ~Grid();

    // Struct to represent each Cell in the Grid
    typedef struct
    {
        double distance;
        int position;
        int certainValue;
        int x, i, gx, centerX;
        int y, j, gy, centerY;
        double force;
    } CELL;

    void display( std::ostream & out );
    void buildBinaryHistogram();
    vector<int> getBinaryHistogram();

    template<typename DataType>
    DataType map( DataType x, DataType in_min, DataType in_max, DataType out_min, DataType out_max );

    POINT getOffSet( int x, int y );

    double getDistanceFromCenter( int x, int y );
    void updateCertainValues( double angle, double distanceFromObject );
    POINT getPointFromDistance( double angle, double distanceFromObject );
    vector<POINT> hashPointTable( double angle, double min, double max );
    void updateCV( vector<POINT> *p, double angle, double distanceFromObject );
    void updateCV( vector<POINT> *p, double angle, double distanceFromObject, int gx, int gy );


    // Force Field functions
    void updateForceField( vector<vector<POINT > > *p );
    // Attractive Force Field function
    void updateAttractiveForceField();
    // Update coordinates
    void updateCoordinates( vector<vector<POINT > > *p, int gx, int gy );

    POINT getCoor( int x, int y );


private:

    double cellWidth;
    int width;
    int middle;
//    double sensorAngle;

    int Fmag_i, Fmag_j;
    double DFmag_i, DFmag_j;

    // Angle of repulsive force
   // double oMega;

    double DAFmag_i, DAFmag_j;
    double Rmag_i, Rmag_j;

    vector<vector<CELL> > Histogram;

    vector<int> binaryHist;

    void pushCellToGrid( int x, int y, int index );

};

//----- Overloaded output operation
std::ostream & operator<<( std::ostream & out, Grid & t );

#endif /* GRID_H_ */
