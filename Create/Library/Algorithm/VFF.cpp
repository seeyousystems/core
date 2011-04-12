#include "VFF.h"
#include <stdio.h>

VFF::VFF()
{
	// Width of grid (number of cells)
	int width = 19;
	// Instantiate a Grid object
	hist = Grid( 10.0, width, 36.0 );
	createHashTables( 5, 36, 45 );
//	createHashTables();
}

VFF::~VFF()
{
}

void VFF::run( int ir0, int ir1, int ir2, int ir3, int ir4, int globalX, int globalY )
{
//	hist.updateCV( &sensor_0, 0.0, ir0, globalX, globalY );
//	hist.updateCV( &sensor_45, 45.0, ir1*2.54, globalX, globalY );
//	hist.updateCV( &sensor_90, 90.0, ir2*2.54, globalX, globalY );
//	hist.updateCV( &sensor_135, 135.0, ir3*2.54, globalX, globalY );
//	hist.updateCV( &sensor_180, 180.0, ir4, globalX, globalY );

	hist.updateCoordinates( &sArray, globalX, globalY );

	hist.updateCV( &sArray.at(0), 0, ir0, globalX, globalY );
	hist.updateCV( &sArray.at(1), 45.0, ir1*2.54, globalX, globalY );
	hist.updateCV( &sArray.at(2), 90.0, ir2*2.54, globalX, globalY );
	hist.updateCV( &sArray.at(3), 135.0, ir3*2.54, globalX, globalY );
	hist.updateCV( &sArray.at(4), 180.0, ir4, globalX, globalY );



//	printf("%d,%d\n", globalX, globalY);
	hist.updateForceField( &sArray );

	hist.updateAttractiveForceField();
}

void VFF::run()
{

    //TODO: I need to restrict the outer limits of the histogram to avoid
    //      errors.
    //		Clean all classes.

//    // Width of grid
//    int width = 21;
//
//    // Counter for general use
//    int programCounter = 0;
//
//    // Instantiate a Grid object
//    hist = Grid( 15.0, width, 36.0 );
//
//    // Create hash table of each sensor to speed up search
//    // This will determine exactly which cells correspond to a given sensor
//    // TODO: Document this function
//    //createHashTables();
//    createHashTables( 5, 36, 0 );
//
//    // 1D Vector used to save a snapshot of a grid to a file
//    vector<int> oneDHistogram;

    ///////////////////////////////// HASH TABLE /////////////////////////////////////////
    //	vector< POINT > pp;
    //
    //	pp = hist.hashPointTable( 36.0, RMin, 325 );
    //
    //	for( unsigned int j = 0; j < pp.size(); j++ )
    //	{
    //		cout << "(" << pp.at(j).x << "," << pp.at(j).y << ")," << endl;
    //	}
    //	cout << "size: " << pp.size() << endl;
    //////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////// TEST 360 SCAN /////////////////////////////////////////////
//    	cout << hist << endl;
//    	for(int ang = 18; ang < 343; ang += 36 )
//    	{
//    		for( int i = RMin; i < 400; i++ )
//    		{
//    			hist.updateCertainValues( ang, (double)i );
//    		}
//    	}
//
//
    	cout << hist << endl;
//
//    	hist.buildBinaryHistogram();

    //////////////////////////////////////////////////////////////////////////////////////

    /*
     *    a. Read sonar information (from the sensor-control computer).
     *    b. Update the histogram grid.
     *    c. Calculate and add (up to) 33×33=1089 individual force vectors
     *    	 inside the active window (Eqs. 1 and 2).
     *    d. Compute the resultant R and the steering direction (Eqs. 3 and 4).
     *    e. Calculate the speed command.
     *    f. Communicate with the low-level motion controller (send speed and
     *    steer commands and receive position update).s
     *
     */

    /*
     * A. Part
     */

//    int ss = 15;
//    for(int m = 0; m < 10;m++)
//    //while ( true )
//    {
//        // Get new values from the Arduino board
//        //Arduino.updateSerialData();
//
//        // Save values to an array
//        //s = Arduino.getSensorArray();
////	cout << s.size() << endl;
//    ss = 15*(m+1);
//	if(1)   //( s.size() == 10 )
//	{
//		int ang = 18;
//		for(int i = 0; i < 10; i++)
//		{
//			if( i == 0 || i == 90 )
//				hist.updateCV( &sArray.at(i), ang, ss );
//			ang += 36;
//		}
//
//		//for(int i = 0; i < 10; i++)
//		//{
//			hist.updateForceField( &sArray );
//		//}
//
//        //TODO: (Check 's' size) Update Histogram counterclockwise
//		/*
//        hist.updateCV( &sensor_0, 18.0, ss );
//        hist.updateCV( &sensor_36, 54.0, ss );
//        hist.updateCV( &sensor_72, 90.0, ss );
//        hist.updateCV( &sensor_108, 126.0, ss );
//        hist.updateCV( &sensor_144, 162.0, ss );
//        hist.updateCV( &sensor_180, 198.0, ss );
//        hist.updateCV( &sensor_216, 234.0, ss );
//        hist.updateCV( &sensor_252, 270.0, ss );
//        hist.updateCV( &sensor_288, 306.0, ss );
//        hist.updateCV( &sensor_324, 342.0, ss );
//
//        hist.updateForceField( &sensor_0 );
//		hist.updateForceField( &sensor_36 );
//		hist.updateForceField( &sensor_72 );
//		hist.updateForceField(&sensor_108);
//		hist.updateForceField(&sensor_144);
//		hist.updateForceField(&sensor_180);
//		hist.updateForceField(&sensor_216);
//		hist.updateForceField(&sensor_252);
//		hist.updateForceField(&sensor_288);
//		hist.updateForceField( &sensor_324);
//		*/
//
//	}
//	s.clear();
//        // Print a histogram image to a file
//        try
//        {
//        	//cout << hist << endl;
//            // Prepare a snapshot of a histogram
//            hist.buildBinaryHistogram();
//
//            // Request a copy
//            oneDHistogram = hist.getBinaryHistogram();
//
//            // Clear the vector for next reading
//            oneDHistogram.clear();
//
//            cout << hist << endl;
//
//        }
//        catch ( ... )
//        {
//            cerr << "*** Error while printing Histogram ***" << endl;
//        }
//
//        //boost::this_thread::sleep( boost::posix_time::milliseconds( 200 ) );
//        programCounter++;
//
//    }
}

/** \brief Creates points that correspond to a cell in respect to a sensor readng
 *
 * 	This command is used to initialized the point that are within every cell.
 * 	For example, to be continue...
 *
 * 	\param none
 *
 *  \return	void
 */
void VFF::createHashTables()
{
    sensor_0 = hist.hashPointTable( 0.0, RMin, 325 );
    sensor_45 = hist.hashPointTable( 45.0, RMin, 325 );
    sensor_90 = hist.hashPointTable( 90.0, RMin, 325 );
    sensor_135 = hist.hashPointTable( 135.0, RMin, 325 );
    sensor_180 = hist.hashPointTable( 180.0, RMin, 325 );
}

void VFF::createHashTables( int numberOfSensors, int fieldOfView, int offSet )
{
	int ang = 0;

	for( int i = 0; i < numberOfSensors; i++ )
	{
		vectorOfSensors = hist.hashPointTable( (float)ang, RMin, 325 );
		sArray.push_back( vectorOfSensors );
		ang = ang + offSet;
	}
}
