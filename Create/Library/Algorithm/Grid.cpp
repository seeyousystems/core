/*
 * Grid.cpp
 *
 *  Created on: Jun 19, 2010
 *      Author: Abraham Gallardo
 */

#include "Grid.h"
#include <cmath>
#include <iomanip>
#include <stdio.h>

Grid::Grid() :
    cellWidth( 15 ), width( 11 ), sensorAngle( 36 )
{
}

Grid::~Grid()
{
    if ( !Histogram.empty() )
        Histogram.clear();
    if ( !binaryHist.empty() )
        binaryHist.clear();
}

Grid::Grid( double widthOfCell, int widthOfGrid, double angleOfSensor ) :
    cellWidth( widthOfCell ), width( widthOfGrid ), sensorAngle( angleOfSensor )
{
	Fmag_i = 0;
	Fmag_j = 0;
	DFmag_i = 0.0;
	DFmag_j = 0.0;
	DAFmag_i = 0.0;
	DAFmag_j = 0.0;
	Rmag_i = 0.0;
	Rmag_j = 0.0;
	oMega = 0.0;

    int index = 0;
    middle = (width - 1) / 2;
    printf("mmm: %d  ww: %d", middle, widthOfGrid);
    CELL cell;

    // Dynamically build binary Histogram
    vector<int> binaryTemp( width * width, 0 );

    // Dynamically build a Histogram grid
    vector<vector<CELL> > temp( width, vector<CELL> ( width, cell ) );

    // Copy binaryTemp 2dvector to Global vector binaryHist
    binaryHist = binaryTemp;

    // Copy temp 2Dvector to Global variable Histogram
    Histogram = temp;

    // Erase temporary "temp" and "binaryTemp" vectors
    temp.clear();
    binaryTemp.clear();

    // Initialize each cell in the Histogram grid
    for ( unsigned int j = 0; j < Histogram.size(); j++ )
    {
        for ( unsigned int i = 0; i < Histogram[j].size(); i++ )
        {
            pushCellToGrid( i, j, index );
        }
    }
}

void Grid::pushCellToGrid( int x, int y, int index )
{
	POINT p;
    CELL cell;
    cell.certainValue = 0;
    cell.distance = 0;
    cell.position = index;
    // Array space
    cell.x = x;
    cell.y = y;
    // Physical space
    p = getCoor(x,y);
    cell.i = p.x;
    cell.j = p.y;
    // Global space
    cell.gx = 0;
    cell.gy = 0;
    // TODO: Midpoint of global space
    cell.centerX = 0;
    cell.centerY = 0;
    cell.force = 0.0;
    Histogram[x][y] = cell;
}

void Grid::display( std::ostream & out )
{
    POINT ptx;

    //double crap = (double)(Fmag_j / Fmag_i);
    double m = (double)Fmag_i;
    double n = (double)Fmag_j;
    double sss = 0.0;
    if( n == 0.0 )
    	sss = 0.0;
    else
    	sss = n / m;

    out<<  ( atan( sss ) * 180 / _PI ) << "\t"  << DAFmag_i << ", " << DAFmag_j << endl;
    for ( unsigned int j = 0; j < Histogram.size(); j++ )
    {
        for ( unsigned int i = 0; i < Histogram[j].size(); i++ )
        {
			//out << "(" << i << "," << j << ")|";
			//out << "(" << Histogram[i][j].i << "," << Histogram[i][j].j << "):";
			//ptx = getCoor(i,j);
			//out << "(" << ptx.x << "," << ptx.y << "),";
            out << setprecision(2) << "(" << Histogram[i][j].certainValue << ":" << Histogram[i][j].force << "), ";
        }
        out << endl;
    }

    Fmag_i = 0;
    Fmag_j = 0;
}

template<typename DataType>
DataType Grid::map( DataType x, DataType in_min, DataType in_max, DataType out_min, DataType out_max )
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

POINT Grid::getOffSet( int x, int y )
{
    // getOffSet(int,int) converts a Cartesian coordinate
    // to an array index.

    POINT pt;

    // Quadrant I
    if ( x >= 0 && y >= 0 )
    {
        pt.x = map( x, 0, middle, middle, width - 1 );
        pt.y = map( y, 0, middle, middle, 0 );
        //cout << tempX << "," << tempY;
    }
    // Quadrant II
    else if ( x <= -1 && y >= 0 )
    {
        pt.x = map( x, -middle, -1, 0, middle - 1 );
        pt.y = map( y, 0, middle, middle, 0 );
        //cout << tempX << "," << tempY;
    }
    // Quadrant III
    else if ( x <= 0 && y <= -1 )
    {
        pt.x = map( x, -middle, 0, 0, middle );
        pt.y = map( y, -middle, -1, width - 1, middle + 1 );
        //cout << tempX << "," << tempY;
    }
    // Quadrant IV
    else if ( x >= 1 && y <= -1 )
    {
        pt.x = map( x, 1, middle, middle + 1, width - 1 );
        pt.y = map( y, -middle, -1, width - 1, middle + 1 );
        //cout << tempX << "," << tempY;
    }
    return pt;
}

double Grid::getDistanceFromCenter( int x, int y )
{
    return sqrt( ((x * x) + (y * y)) ) * (cellWidth);// - iRobotRadiusCm;
}

void Grid::updateCertainValues( double angle, double distanceFromObject )
{
    POINT pt;
    double x = 0;
    double y = 0;
    if ( angle != 180.0 )
    {
        x = distanceFromObject * cos( angle * TO_DEGREES );
        y = distanceFromObject * sin( angle * TO_DEGREES );
    }
    else // if angle = 180
    {
        x = distanceFromObject * cos( 0 );
        y = distanceFromObject * sin( 0 );
        x = -x;
    }

    //cout << x << "," << y << endl;

    // Normalize coordinates
    pt.x = (int) ((x + (cellWidth / 2.0)) / cellWidth);
    pt.y = (int) ((y + (cellWidth / 2.0)) / cellWidth);

    // Get coordinates offset
    pt = getOffSet( pt.x, pt.y );

    // Check we are inside grid boundaries
    if ( pt.x > (width - 1) || pt.y > (width - 1) || pt.x < 0 || pt.y < 0 )
    {
        return;
    }

    // Update corresponding cell
    Histogram[pt.x][pt.y].certainValue += 1;
    if ( Histogram[pt.x][pt.y].certainValue >= 15 )
        Histogram[pt.x][pt.y].certainValue = 15;

    //cout << pt.x << "," << pt.y << endl;

    //Histogram[x0][y0].certainValue += CVIinc;
}

vector<POINT> Grid::hashPointTable( double angle, double min, double max )
{
    //TODO: Update min and max variables to provide better measurements
    //      when different robots shapes are used.
    POINT currentPoint, previousPoint;

    // Initialize to default values
    previousPoint.x = -1;
    previousPoint.y = -1;
    vector<POINT> points;

    for ( int i = min; i < max; i++ )
    {
        // Get point with offset
        currentPoint = getPointFromDistance( angle, i );

        // Iterate until we hit the boundaries
        if ( currentPoint.x > (width - 1) || currentPoint.y > (width - 1) || currentPoint.x < 0 || currentPoint.y < 0 )
        {
            break;
        }

        // If we have a repeated point skip it
        if ( currentPoint.x == previousPoint.x && currentPoint.y == previousPoint.y )
        {
            continue;
        }
        if( (currentPoint.x == (width -1) / 2) && (currentPoint.y == (width -1) / 2))
        	continue;
        // Save first point
        points.push_back( currentPoint );

        // Keep previous for comparison
        previousPoint.x = currentPoint.x;
        previousPoint.y = currentPoint.y;
    }
    return points;
}

POINT Grid::getPointFromDistance( double angle, double distanceFromObject )
{
    POINT pt;
    double x = 0;
    double y = 0;
    if ( 1 )
    {
        x = distanceFromObject * cos( angle * TO_DEGREES );
        y = distanceFromObject * sin( angle * TO_DEGREES );
    }
//    else // if angle = 180
//    {
//        x = distanceFromObject * cos( 0 );
//        y = distanceFromObject * sin( 0 );
//        x = -x;
//    }

    //cout << x << "," << y << endl;

    // Normalize coordinates
    pt.x = (int) ((x + (cellWidth / 2.0)) / cellWidth);
    pt.y = (int) ((y + (cellWidth / 2.0)) / cellWidth);

    if( pt.x == 1)
    	pt.x = 2;
    else if( pt.x == -1 )
    	pt.x = -2;
    else if( pt.y == 1)
    	pt.y = 2;
    else if( pt.y == -1 )
    	pt.y = -2;

    // Get coordinates offset
    pt = getOffSet( pt.x, pt.y );

    return pt;
}

void Grid::updateCV( vector<POINT> *p, double angle, double distanceFromObject )
{
    POINT pt;
    // Check whether the point is valid or invalid
    // TODO: Need to re-evaluate this condition because if there is an active cell
    //       that has value x!=0 at a valid distance, then if the distance is out
    //	     of range then should the active cell be affected? I believe so, because
    //       if the sensor is not picking any objects, previous readings should be clear
    //       from the Histogram grid.
    //

    // Get Cell point that corresponds to a given distance
    pt = getPointFromDistance( angle, distanceFromObject );

    // If a Point is outside our grid, the grid has to be clear!!!
    if ( pt.x > (width - 1) || pt.y > (width - 1) || pt.x < 0 || pt.y < 0 )
    {
        for ( int i = 0; i < p->size(); i++ )
        {
            // Get values to speed up process
            int x = p->at( i ).x;
            int y = p->at( i ).y;
            if ( Histogram[x][y].certainValue != CVMin )
            {
                Histogram[x][y].certainValue += CVIdec;
            }
        }
        return;
    }

    // Iterate through points (Hash Table) of given angle
    for ( int i = 0; i < p->size(); i++ )
    {
        // Get values to speed up process
        int x = p->at( i ).x;
        int y = p->at( i ).y;

        // If given point equals the vector then increment CV
        if ( x == pt.x && y == pt.y )
        {
            // Update corresponding cell
            Histogram[x][y].certainValue += CVIinc;

            // Update distance from origin to this Cell
            Histogram[x][y].distance = distanceFromObject;

            if ( Histogram[x][y].certainValue >= CVMax )
                Histogram[x][y].certainValue = CVMax;
        }
        // If given point has a value greater than 0 then decrement CV
        else if ( Histogram[x][y].certainValue != CVMin )
        {
            Histogram[x][y].certainValue += CVIdec;
        }
    }
}

void Grid::buildBinaryHistogram()
{
    int x = 0;
    // Build a binary Histogram for debugging purposes
    for ( unsigned int j = 0; j < Histogram.size(); j++ )
    {
        for ( unsigned int i = 0; i < Histogram[j].size(); i++ )
        {
            binaryHist[x++ ] = Histogram[i][j].certainValue;
        }
    }
}

vector<int> Grid::getBinaryHistogram()
{
    return binaryHist;
}

/** \brief Updates certainty value of corresponding cell
 *
 * 	This command updates cells with a certainty value and
 * 	updates local coordinates to global coordinates.
 *
 * 	\param
 *
 *  \return	void
 */
void Grid::updateCV( vector<POINT> *p, double angle, double distanceFromObject, int gx, int gy )
{
    POINT pt;

    // Get Cell point that corresponds to a given distance
    pt = getPointFromDistance( angle, distanceFromObject );

    // If a Point is outside our grid, the grid has to be clear!!!
    if ( pt.x > (width - 1) || pt.y > (width - 1) || pt.x < 0 || pt.y < 0 )
    {
        for ( int i = 0; i < p->size(); i++ )
        {
            // Get values to speed up process
            int x = p->at( i ).x;
            int y = p->at( i ).y;
            if ( Histogram[x][y].certainValue != CVMin )
            {
                Histogram[x][y].certainValue += CVIdec;
            }
        }
        return;
    }

    // Iterate through points (Hash Table) of given angle
    for ( int i = 0; i < p->size(); i++ )
    {
        // Get values to speed up process
        int x = p->at( i ).x;
        int y = p->at( i ).y;

        // If given point equals the vector then increment CV
        if ( x == pt.x && y == pt.y )
        {
            // Update corresponding cell
            Histogram[x][y].certainValue += CVIinc;

            // Update distance from origin to this Cell
            Histogram[x][y].distance = distanceFromObject;

            if ( Histogram[x][y].certainValue >= CVMax )
                Histogram[x][y].certainValue = CVMax;
        }
        // If given point has a value greater than 0 then decrement CV
        else if ( Histogram[x][y].certainValue != CVMin )
        {
            Histogram[x][y].certainValue += CVIdec;
        }
    }
}

/** \brief Updates force field
 *
 * 	This command updates cells the force field value
 * 	calculated from the VFF algorithm.
 *
 * 	\param 2D vector
 *
 *  \return	void
 */
void Grid::updateForceField( vector<vector<POINT > > *p )
{
	//TODO: I need to send hash table with all vectors included instead
	// of individual vectors.

	// Iterate through points (Hash Table) of given angle
	for( int sensor = 0; sensor < p->size(); sensor++)
	{
		for (int i = 0; i < p->at(sensor).size(); i++)
		{
			// Get values to speed up process
			int x = p->at(sensor).at(i).x;
			int y = p->at(sensor).at(i).y;

			if( Histogram[x][y].certainValue == 0 )
			{
				Histogram[x][y].force = 0.0;
			}
			else if( Histogram[x][y].certainValue != 0 )
			{
				Histogram[x][y].force = ((F_CR * Histogram[x][y].certainValue) / (Histogram[x][y].distance * Histogram[x][y].distance));
				//int F_i = ((F_CR * Histogram[x][y].certainValue) / (Histogram[x][y].distance * Histogram[x][y].distance));
				//int F_j = ((F_CR * Histogram[x][y].certainValue) / (Histogram[x][y].distance * Histogram[x][y].distance));

				Fmag_i += Histogram[x][y].i;
				Fmag_j += Histogram[x][y].j;
				DFmag_j += (Histogram[x][y].force * (Histogram[x][y].i / (Histogram[x][y].distance)));
				DFmag_i += (Histogram[x][y].force * (Histogram[x][y].j / (Histogram[x][y].distance)));

			}
		}
	}

//	double dx, dy;
//	dx = -8.0;
//	dx = 10.0;
//
//	dx = dx + DFmag_i;
//	dy = dy + DFmag_j;
//
//
//	//printf("Resultant: [%.2f,%.2f]", DFmag_i, DFmag_j);
//	DFmag_i = 0.0;
//	DFmag_j = 0.0;
//	dx = 0.0;
//	dy = 0.0;

	/*// Calculate resultant repulsive force (vectorial sum from all individual cells)
	for (int i = 0; i < p->size(); i++)
	{
		// Get values to speed up process
		int x = p->at(i).x;
		int y = p->at(i).y;

		// Repulsive force vector
		int F_i = 0;
		int F_j = 0;

		if( Histogram[x][y].force >= 1.0 )
		{
			F_i += Histogram[x][y].i;
			F_j += Histogram[x][y].j;
		}
	}*/
}

/** \brief Updates force field
 *
 * 	This command updates cells the force field value
 * 	calculated from the VFF algorithm.
 *
 * 	\param 2D vector
 *
 *  \return	void
 */
void Grid::updateAttractiveForceField()
{
	//TODO: to be continue

	// Force variable must be set to a specific number
	double Force = 5000.0;

	// POINT is a struct of two integers (x and y)
	POINT target;

	// Let's simulate a desire target
	target.x = 50;
	target.y = 210;

	// Calculation for distance
	double x = ( double )( target.x - Histogram[11][8].centerX );
	double y = ( double )( target.y - Histogram[11][8].centerY );

//	printf("%f, %f\n" , _x, _y);
//	printf("%d, %d\n" , Histogram[11][8].centerX, Histogram[11][8].centerY);

	// Calculate distance between target coor. and robot coor.

	double distance = sqrt( ((x * x) + (y * y)) );

	printf("Point:(%.2f, %.2f)  Distance: %.2f\n", x, y, distance);

	// Attractive force vector
	DAFmag_i = Force * ( x  / distance );
	DAFmag_j = Force * ( y  / distance );

	Rmag_i = DAFmag_i + DFmag_i;
	Rmag_j = DAFmag_j + DFmag_j;

//	printf("Atractive: %.2f, %.2f\n", DAFmag_i, DAFmag_j);

	//printf( "Delta: %.2f,  %.2f \n", Fmag_j, Fmag_i );

	// Reset Forces
	DFmag_i = 0.0;
	DFmag_j = 0.0;

	DAFmag_i = 0.0;
	DAFmag_j = 0.0;

//	Fmag_i = 0;
//	Fmag_j = 0;

	//printf("Point:(%.2f, %.2f)  Distance: %.2f", x, y, distance);
	//printf("\tAttractive: [%.2f,%.2f]\n", DAFmag_i, DAFmag_j);

}

/** \brief Updates coordinates
 *
 * 	This command updates global coordinates
 *
 * 	\param 2D vector
 *
 *  \return	void
 */
void Grid::updateCoordinates( vector<vector<POINT > > *p, int gx, int gy )
{
	for( int sensor = 0; sensor < p->size(); sensor++)
	{
		for (int i = 0; i < p->at(sensor).size(); i++)
		{
			// Get values to speed up process
			int x = p->at(sensor).at(i).x;
			int y = p->at(sensor).at(i).y;

			// Update global coordinates
			Histogram[x][y].gx = Histogram[x][y].i + gx;
			Histogram[x][y].gy = Histogram[x][y].j + gy;

			// Update global Vehicle Center Point (VCP) coordinates
			Histogram[x][y].centerX = gx;
			Histogram[x][y].centerY = gy;

			///printf( "%d, %d\n", x, y );
		}
		//printf("\n");
	}
}

POINT Grid::getCoor( int x, int y )
{
    // getOffSet(int,int) converts a Cartesian coordinate
    // to an array index.

    POINT pt;

    // Quadrant I
    if ( x >= middle && y <= middle )
    {
        pt.x = map( x, middle, width - 1, 0, middle );
        pt.y = map( y, 0, middle, middle, 0 );
        //cout << tempX << "," << tempY;
    }
    // Quadrant II
    else if ( x <=  middle - 1 && y <= middle )
    {
        pt.x = map( x,0, middle - 1, -middle, -1 );
        pt.y = map( y, 0, middle, middle, 0 );
        //cout << tempX << "," << tempY;
    }
    // Quadrant III
    else if ( x <= middle - 1 && y >= middle + 1 )
    {
        pt.x = map( x, 0, middle, -middle, -1 );
        pt.y = map( y, middle + 1, width - 1, -1, -middle );
        //cout << tempX << "," << tempY;
    }
    // Quadrant IV
    else if ( x >= middle && y >= middle + 1 )
    {
        pt.x = map( x, middle, width - 1, 0, middle );
        pt.y = map( y, middle + 1, width - 1, -1, -middle );
        //cout << tempX << "," << tempY;
    }
    return pt;
}

//----- Definition of operator<<()
std::ostream & operator<<( std::ostream & out, Grid & t )
{
    t.display( out );
    return out;
}

/*
 Grid::Grid( const Grid & origList )
 :width(origList.width), pos_limit(origList.pos_limit), neg_limit(origList.neg_limit), position(origList.position)
 {

 }

 const Grid & Grid::operator=( const Grid & rightHandSide )
 {
 if( this != &rightHandSide )
 {
 Histogram_Grid.clear();
 vector<Cell> newVect(rightHandSide.Histogram_Grid);
 }
 return *this;
 }


 */
