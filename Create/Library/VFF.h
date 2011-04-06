/*
 * VFF.h
 *
 *  Created on: Apr 4, 2011
 *      Author: abraham
 */

#ifndef VFF_H_
#define VFF_H_

double _x = 0;
double _y = 0;

// Force constant (repelling)
const double F_CR = -85000.0;
const double F_CT = 5000.0;

int Fmag_i = 0, Fmag_j = 0;
double DFmag_i = 0, DFmag_j = 0;
double TFmag_i = 0, TFmag_j = 0;
double RFmag_i = 0, RFmag_j = 0;

// PI constant
#define ToRad(x) (x*0.01745329252)  // *pi/180
#define ToDeg(x) (x*57.2957795131)  // *180/pi

// PI constant
const double TO_DEGREES = 0.0174532925199433f;

// Minimum and Maximum distance sensors are limited to
const int RMin = 15;
const int RMax = 227;

const int leftAngle = 0;
const int rightAngle = 180;

const int F_cr = 5000;                  // Constant Force (repelling)

int cell_size = 15;                     // cell width = height = 15 cm
const int num_cells = 14;               // Number of cells
const int width = 31;
const int middle = 15;

int certainty_left[14];                 // cell length = 15 cm * 14 = 210 cm
int certainty_right[14];                // cell length = 15 cm * 14 = 210 cm

const int robot_x = 0;
const int robot_y = 0;

int count = 0;

class VFF
{
public:
	VFF();
	virtual ~VFF();
	void initVFF();

	typedef struct
	{
		double distance;
		int cv;
		int x, y;
		double force;
		double targetForce;
	}Cell;

	Cell cell_vff_left[num_cells];
	Cell cell_vff_left_middle[num_cells];
	Cell cell_vff_right_middle[num_cells];
	Cell cell_vff_right[num_cells];
	Cell cell_vff_front[num_cells];

	void printVFF()
	{
		for(int i = num_cells-1; i >=0; i--)
		{
			std::cout << cell_vff_left[i].cv << ",";
		}
		std::cout << "Center,";
		for(int i = 0; i < num_cells; i++)
		{
			std::cout << cell_vff_front[i].cv << ",";
		}
		std::cout << "Center,";
	    for(int i = 0; i < num_cells; i++)
	    {
	    	std::cout << cell_vff_right[i].cv << ",";
	    }
	    std::cout << endl;
	}

private:

};

/////////////////////////////
/// constructor
/////////////////////////////

VFF::VFF()
{
	initVFF();
}

/////////////////////////////
/// destructor
/////////////////////////////

VFF::~VFF()
{

}

/////////////////////////////
/// init
/////////////////////////////
void VFF::initVFF()
{
	for(int i = 0; i < num_cells; i++)
	{
		cell_vff_left[i].distance = 0;
		cell_vff_left[i].cv = 0;
		cell_vff_left[i].x = -i;
		cell_vff_left[i].y = 0;
		cell_vff_left[i].force = 0;
		cell_vff_left[i].targetForce = 0;

		cell_vff_right[i].distance = 0;
		cell_vff_right[i].cv = 0;
		cell_vff_right[i].x = i;
		cell_vff_right[i].y = 0;
		cell_vff_right[i].force = 0;
		cell_vff_right[i].targetForce = 0;

		cell_vff_front[i].distance = 0;
		cell_vff_front[i].cv = 0;
		cell_vff_front[i].x = 0;
		cell_vff_front[i].y = i;
		cell_vff_front[i].force = 0;
		cell_vff_front[i].targetForce = 0;
	}
}

#endif /* VFF_H_ */
