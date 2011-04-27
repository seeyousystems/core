/*
 *  Spline1.h
 *
 *  This template class implements different one-dimensional spline approaches.
 *  Splines are smooth and efficient curve interpolations of a finite set of
 *  nodes. The different kernels we implemented include SplineKernalBruenner,
 *  SplineKernelNRNatural, and SplineKernelStoer. Nodes can be freely added and
 *  removed from the spline. When a call to getValue(node,t) is made, Spline1T
 *  automatically decides whether or not an internal recalculation of the
 *  spline coefficients is needed, maximizing the ease of use to the user.
 *  Each kernel has its own implantation of getValue(node,t),
 *  getFirstDerivative(node,t), and getSecondDerivative(node,t).
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

#ifndef SPLINE1_H_
#define SPLINE1_H_

#include <vector>
#include <iterator>

#include <stdio.h>
#include <assert.h>


//	Typedefs and defines
typedef enum SplineKernel { SplineKernelBruenner, SplineKernelNRNatural, SplineKernelStoer, SplineKernelWirth };


template <class real>
class Spline1T
{

        //  data

	private:

		std::vector<real> data;

		// Kernel data...
		std::vector<real> a;
		std::vector<real> b;
		std::vector<real> c;
		//std::vector<real> d;

		// Additional NR kernel data...
		std::vector<real> y2;

		// Additional start/end restrictions
		real dy0;
		real dyn;
		bool constraintsSet;

		SplineKernel kernel;
		bool needsCalculation;

    public:

        //  constructor and kernel settings

    	Spline1T ();
    	Spline1T (SplineKernel kernel);
    	void setKernel (SplineKernel kernel);
    	SplineKernel getKernel ();
    	void setConstraints (real dy0, real dyn);
    	void unsetConstraints ();

    	//  values

    	real getValue(int i, real t);
		real getFirstDerivative(int i, real t);
		real getSecondDerivative(int i, real t);

		//  nodes
		void addNode(real value);
		std::vector<real> getNodes();
		void removeFirstNode();
		void clearNodes();
		void dropNodes(int amount);
		void changeNode(int index, real value);
		int getNodeCount();

    private:

    	//  calculations

    	void calculate();

};



////////////////////////////////////////////////////////////////////////
///   constructor and kernel settings                                ///
////////////////////////////////////////////////////////////////////////

template <class real>
Spline1T <real> :: Spline1T ()
{
	// Init
	needsCalculation = true;
	kernel = SplineKernelBruenner; // Default kernel
	dy0 = 0;
	dyn = 0;
	constraintsSet = false;
}

template <class real>
Spline1T <real> :: Spline1T (SplineKernel kernel)
{
	// Init
	needsCalculation = true;
	this->kernel = kernel;
	dy0 = 0;
	dyn = 0;
	constraintsSet = false;
}

template <class real>
void Spline1T <real> :: setKernel (SplineKernel kernel) {
	this->kernel = kernel;
	needsCalculation = true; // Set dirty
}

template <class real>
SplineKernel Spline1T <real> :: getKernel () {
	return kernel;
}

template <class real>
void Spline1T <real> :: setConstraints (real dy0, real dyn) {
	this->dy0 = dy0;
	this->dyn = dyn;
	constraintsSet = true;
	needsCalculation = true; // Set dirty
}

template <class real>
void Spline1T <real> :: unsetConstraints () {
	this->dy0 = 0;
	this->dyn = 0;
	constraintsSet = false;
	needsCalculation = true; // Set dirty
}


////////////////////////////////////////////////////////////////////////
///   calculations                                                   ///
////////////////////////////////////////////////////////////////////////

template <class real>
void Spline1T <real> :: calculate ()
{
	if(kernel == SplineKernelBruenner) {

		// Implementation based on the writings of Arndt Bruenner

		// Init
		int n = data.size() - 1;
		a = std::vector<real>(data.size());
		b = std::vector<real>(data.size());
		c = std::vector<real>(data.size());
		std::vector<std::vector<real> > coefficientMatrix = std::vector<std::vector<double> >(n + 1, std::vector<double>(n + 1));
		std::vector<real> coefficientMatrixBValues = std::vector<real>(data.size());

		// Calculate coefficient matrix
		//  _       _
		// |*       |
		// |*\*     |
		// | \\\    |
		// |  \\\   |
		// |   \\\  |
		// |    *\* |
		// |_     *_|
		//
		for(int i = 0; i <= n; i++) {
			for(int bi = 0; bi <= n; bi++) {
				double value = 0;
				if( (i > 0 && i < n) && (bi > 0 && bi < n) ) {
					if(bi == i-1) {
						value = i - (i-1);
					}else if(bi == i) {
						value = 2 * (i+1 - (i-1));
					}else if(bi == i+1) {
						value = (i+1) - i;
					}
				}
				coefficientMatrix[i][bi] = value;
			}

		}

		// Calculate coefficient matrix B Values
		for (int i = 0; i <= n; i++) {
			for (int bi = 0; bi < n; bi++) {
				coefficientMatrixBValues[i] += coefficientMatrix[i][bi];
			}
		}

		// Calculate b[i]
		for (int i = 1; i < n; i++) {
			// 3((di+1-di)/(xi+1-xi) - (di-di-1)/(xi-xi-1))
			double xi = i;
			double xip1 = i+1;
			double xim1 = i-1;
			double di = data[i];
			double dip1 = data[i+1];
			double dim1 = data[i-1];
			b[i] = ( 3*((dip1-di)/(xip1-xi) - (di-dim1)/(xi-xim1)) ) / coefficientMatrixBValues[i];
		}

		// Calculate c[i]
		for (int i = 0; i < n; i++) {
			// ci = (di+1-di)/(xi+1-xi) - (bi+1-bi)(xi+1-xi)/3 - bi(xi+1-xi)
			double xi = i;
			double xip1 = i+1;
			double di = data[i];
			double dip1 = data[i+1];
			double bi = b[i];
			double bip1 = b[i+1];
			c[i] = ((dip1-di)/(xip1-xi)) - ((bip1-bi)*(xip1-xi)/3.0) - (bi*(xip1-xi));
		}

		// Calculate a[i]
		for (int i = 0; i < n; i++) {
			a[i] = (b[i + 1] - b[i]) / (3 * ((i+1) - i));
		}

	} else if (kernel == SplineKernelNRNatural) {

		// Calculates the second derivatives of the interpolating function
		// at the tabulated points xi, with xi = (i, y[i]).
		// Based on algorithm extracted from the Book "Numerical Recipes"

		// Init
		int n = data.size();
		y2 = std::vector<real>(n);
		std::vector<real> u = std::vector<real>(n);

		// Calculate u
		for(int i = 1; i < n - 1; i++) {
			y2[i] = -1.0 / (4.0 + y2[i - 1]);
			u[i] = (6.0 * (data[i + 1] - 2.0 * data[i] + data[i - 1]) - u[i - 1]) / (4.0 + y2[i - 1]);
		}

		// Calculate y2
		for(int i = n - 2; i >= 0; i--) {
			y2[i] = y2[i] * y2[i + 1] + u[i];
		}

	} else if (kernel == SplineKernelStoer) {

		// Calculations based on Joseph Stoer's "Numerische Mathematik 1"

		// Init
		int n = data.size() - 1;
		a = std::vector<real>(data.size());
		b = std::vector<real>(data.size());
		c = std::vector<real>(data.size());
		std::vector<real> d = std::vector<real>(data.size());
		std::vector<real> p = std::vector<real>(data.size());
		std::vector<real> q = std::vector<real>(data.size());
		std::vector<real> u = std::vector<real>(data.size());
		std::vector<real> M = std::vector<real>(data.size());
		std::vector<real> lambda = std::vector<real>(data.size());
		std::vector<real> mu = std::vector<real>(data.size());

		// Init default lambda and mu values
		// TODO: optimize this once it works correctly!
		for (int i = 1; i < n; i++) {
			lambda[i] = 0.5;
			mu[i] = 1.0 - lambda[i];
		}
		lambda[0] = 1.0;
		mu[n] = 1.0;

		// Calculate d values
		d[0] = 6 * (data[1] - data[0] - dy0);
		for (int i = 1; i < n; i++) {
			d[i] = 3.0 * (data[i+1] - data[i]);
		}
		d[n] = 6.0 * (dyn - data[n] + data[n-1]);

		// Calculate p, q, and u values
		q[0] = -1.0 * lambda[0] / 2;
		u[0] = d[0] / 2.0;
		lambda[n] = 0.0;
		for(int k = 1; k <= n; k++) {
			p[k] = mu[k] * q[k-1] + 2.0;
			q[k] = -1.0 * lambda[k] / p[k];
			u[k] = (d[k] - mu[k] * u[k-1]) / p[k];
		}

		// Back-calculate M values
		M[n] = u[n];
		for(int k = n-1; k >= 0; k--) {
			M[k] = q[k] * M[k+1] + u[k];
		}

		// Calculate rest coefficients
		for(int i = 0; i < n; i++) {
			a[i] = (M[i+1] - M[i]) / 6.0;
			b[i] = M[i] / 2.0;
			c[i] = (data[i+1] - data[i]) - (2.0 * M[i] + M[i+1]) / 6.0;
		}

	} else if (kernel == SplineKernelWirth) {

		// Calculations based on Joseph Stoer's "Numerische Mathematik 1"
		// and Dr. Joachim Wirth's input


		//  number of intervals
		int i;
		int np = data.size();
		int ni = np - 1;

		//  coeffs
		a = std::vector<real>(np);
		b = std::vector<real>(np);
		c = std::vector<real>(np);
		std::vector<real> dx = std::vector<real>(ni);
		std::vector<real> dy = std::vector<real>(ni);

		//  calculate x values, x differences, and y differences
		for (i = 0; i < ni; ++i)
		{
			dx [i] = i+1 - i;
			dy [i] = (data [i + 1] - data [i]) / dx [i];

			assert(dx[i] != 0);
			assert(dy[i] != 0);
		}


		//  solve system of linear equations
		std::vector<real> q (np + 1);
		std::vector<real> u (np + 1);

		// Calculate d0
		real lambda0;
		if (constraintsSet) 	lambda0 = 0;
		else					lambda0 = 1;
		q [0] = - lambda0 / 2;
		u [0] = 3 / (1 - 0) * (dy [0] - dy0);

		// Calculate d
		for (i = 1; i < ni; ++i)
		{
			real dd = (i+1) - (i-1); //x [i + 1] - x [i - 1];
			real lambda = dx [i] / dd;
			real mu = 1 - lambda;
			real d = 6.0 * (dy [i] - dy [i - 1]) / dd;

			real p = mu * q [i - 1] + 2.0;
			q [i] = - lambda / p;
			u [i] = (d - mu * u [i - 1]) / p;
		}

		// Calculate dn
		real ddn = (ni + 1) - (ni - 1);
		real lambdan = dx [ni] / ddn;
		real mun;
		real dn;
		if (constraintsSet) {
			mun = 0;
			dn = 6.0 * (dy [ni] - dy [ni - 1]) / ddn;
		} else {
			mun = 1;
			dn = 6.0 / (ni - (ni - 1)) * (dyn - dy [ni]);
		}

		real pn = mun * q [ni - 1] + 2.0;
		q [ni] = - lambdan / pn;
		u [ni] = (dn - mun * u [i - 1]) / pn;


		// Back calculate...
		std::vector<real> m (np + 1);
		m [np] = u [np];
		for (i = ni; i >= 0; --i)
			m [i] = q [i] * m [i + 1] + u [i];

		//  calculate coefficients of polynomials
		for (i = 0; i < ni; ++i) {
			a[i] = (m [i + 1] - m [i]) / (6.0 * dx [i]);
			b[i] = m [i] / 2.0;
			c[i] = dy [i] - dx [i] * (2.0 * m [i] + m [i + 1]) / 6.0;
		}

//		// Calculate rest coefficients
//		for(int i = 0; i < ni; i++) {
//			a[i] = (m[i+1] - m[i]) / 6.0;
//			b[i] = m[i] / 2.0;
//			c[i] = (data[i+1] - data[i]) - (2.0 * m[i] + m[i+1]) / 6.0;
//		}


	}

	needsCalculation = false;
}

template <class real>
real Spline1T <real> :: getValue(int i, real t) {

	if(needsCalculation) calculate();

	if(kernel == SplineKernelBruenner) {

		return a[i]*powf(t,3) + b[i]*powf(t,2) + c[i]*(t) + data[i]; //TODO: optimize calculations

	} else if (kernel == SplineKernelNRNatural) {

		// Returns a cubic-spline interpolated value y for the point between
		// point (n, y[n]) and (n+1, y[n+1), with t ranging from 0 for (n, y[n])
		// to 1 for (n+1, y[n+1]).

		return t * data[i + 1] - ((t - 1.0) * t * ((t - 2.0) * y2[i] - (t + 1.0) * y2[i + 1])) / 6.0 + data[i] - t * data[i];

	} else if (kernel == SplineKernelStoer) {

		return a[i]*powf(t,3) + b[i]*powf(t,2) + c[i]*(t) + data[i]; //TODO: optimize calculations

	} else if (kernel == SplineKernelWirth) {

		//S dx = x - X;
		//return ((A * dx + B) * dx + C) * dx + D;
		return a[i]*powf(t,3) + b[i]*powf(t,2) + c[i]*(t) + data[i]; // unoptimized :)

//		real dx = t;
//		return ((a[i] * dx + b[i]) * dx + c[i]) * dx + d[i];

	}
}

template <class real>
real Spline1T <real> :: getFirstDerivative(int i, real t){

	if(needsCalculation) calculate();

	if(kernel == SplineKernelBruenner) {

		return 3 * a[i]*powf(t,2) + 2 * b[i]*(t) + c[i];

	} else if (kernel == SplineKernelNRNatural) {

		return ((1.0/2.0) * (t*t) - (1.0/6.0))*y2[i + 1] + (-1.0*(1.0/2.0) * (t*t) + t - (1.0/3.0))*y2[i] + data[i + 1] - data[i];

	} else if (kernel == SplineKernelStoer) {

		return 3 * a[i]*powf(t,2) + 2 * b[i]*(t) + c[i];

	} else if (kernel == SplineKernelWirth) {

		return 3 * a[i]*powf(t,2) + 2 * b[i]*(t) + c[i];

	}
}

template <class real>
real Spline1T <real> :: getSecondDerivative(int i, real t){

	if(needsCalculation) calculate();

	if(kernel == SplineKernelBruenner) {

		return 6 * a[i]*(t) + 2 * b[i];

	} else if (kernel == SplineKernelNRNatural) {

		return t*y2[i + 1] + (-1.0*t + 1.0)*y2[i];

	} else if (kernel == SplineKernelStoer) {

		return 6 * a[i]*(t) + 2 * b[i];

	} else if (kernel == SplineKernelWirth) {

		return 6 * a[i]*(t) + 2 * b[i];

	}
}

////////////////////////////////////////////////////////////////////////
///   nodes                                                          ///
////////////////////////////////////////////////////////////////////////

template <class real>
void Spline1T <real> :: addNode(real value) {
	data.push_back(value);
	needsCalculation = true; // Set dirty
}

template <class real>
std::vector<real> Spline1T <real> :: getNodes() {
	return data;
}


template <class real>
void Spline1T <real> :: removeFirstNode() {
	data.erase(data.begin());
	needsCalculation = true; // Set dirty
}


template <class real>
void Spline1T <real> :: clearNodes() {
	data.clear();
	needsCalculation = true; // Set dirty
}

template <class real>
void Spline1T <real> :: dropNodes(int amount) {
	for(int i = 0; i < amount; i++) {
		data.pop_back();
	}
	needsCalculation = true; // Set dirty
}


template <class real>
void Spline1T <real> :: changeNode(int index, real value) {
	data.at(index) = value;
	needsCalculation = true; // Set dirty
}

template <class real>
int Spline1T <real> :: getNodeCount() {
	return (int)data.size();
}


////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef Spline1T <float> Spline1F;
typedef Spline1T <double> Spline1D;
typedef Spline1T <long double> Spline1L;



#endif /* SPLINE1_H_ */
