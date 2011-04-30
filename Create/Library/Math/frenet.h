/*
 *  frenet.h
 *
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

#ifndef FRENET_H_
#define FRENET_H_

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <vector>




template <class real>
class DiffEqVector
{

public:

	//  data

	real coord[6];

public:

	//  constructors

	DiffEqVector();
	DiffEqVector(real x0, real y0, real x1, real y1, real x2, real y2);

	//  components

	real x0();
	real y0();
	real x1();
	real y1();
	real x2();
	real y2();
};







template <class real>
class FrenetT
{

public:

	//  enums

	enum ApproximationMethod { ApproximationMethodEuler, ApproximationMethodRungeKutta };

private:

	//  data

	real (*_curveFunction)(real t);
	DiffEqVector<real> _y0;

public:

	//  constructors

	FrenetT (real (*curveFunction)(real t), DiffEqVector<real> y0);

private:

	//  math functions for DiffEqVector

	DiffEqVector<real> ode (real t, DiffEqVector<real> y);
	DiffEqVector<real> add (DiffEqVector<real> v, DiffEqVector<real> w);
	DiffEqVector<real> sub (DiffEqVector<real> v, DiffEqVector<real> w);
	DiffEqVector<real> mul (real v, DiffEqVector<real> w);
	DiffEqVector<real> div (DiffEqVector<real> v, real w);
	DiffEqVector<real> Euler (real x0, DiffEqVector<real> y0, real h);
	DiffEqVector<real> RungeKutta (real x0, DiffEqVector<real> y0, real h);

public:

	//  approximations

	DiffEqVector<real> ApproximateDiffEqVector(ApproximationMethod method, real t, real h);
	std::vector< DiffEqVector<real> > ApproximateCurve(ApproximationMethod method, real tmax, real h);


};




////////////////////////////////////////////////////////////////////////
///   constructors                                                   ///
////////////////////////////////////////////////////////////////////////


template <class real>
FrenetT<real> :: FrenetT (real (*curveFunction)(real t), DiffEqVector<real> y0) {
	this->_y0 = y0;
	this->_curveFunction = curveFunction;
}




////////////////////////////////////////////////////////////////////////
///   math functions on DiffEqVectorT                                ///
////////////////////////////////////////////////////////////////////////


template <class real>
DiffEqVector<real> FrenetT<real> :: ode (real t, DiffEqVector<real> y)
{
	DiffEqVector<real> result;


	real mult = sqrt(y.coord[2]*y.coord[2] + y.coord[3]*y.coord[3]) * _curveFunction(t); // sqrt(x1^2+y1^2) * k

	result.coord [0] = y.coord [2]; // x0' = x1
	result.coord [1] = y.coord [3]; // y0' = y1
	result.coord [2] = mult * y.coord [4]; // x1' = sqrt(x1^2+y1^2) * k * x2
	result.coord [3] = mult * y.coord [5]; // y1' = sqrt(x1^2+y1^2) * k * y2
	result.coord [4] = - mult * y.coord [2]; // x2' = - sqrt(x1^2+y1^2) * k * x1
	result.coord [5] = - mult * y.coord [3]; // y2' = - sqrt(x1^2+y1^2) * k * y1

	return result;
}



template <class real>
DiffEqVector<real> FrenetT<real> :: add (DiffEqVector<real> v, DiffEqVector<real> w)
{
	DiffEqVector<real> u;
	int i;

	for (i = 0; i < 6; ++i)
		u.coord [i] = v.coord [i] + w.coord [i];

	return u;
}



template <class real>
DiffEqVector<real> FrenetT<real> :: sub (DiffEqVector<real> v, DiffEqVector<real> w)
{
	DiffEqVector<real> u;
	int i;

	for (i = 0; i < 6; ++i)
		u.coord [i] = v.coord [i] - w.coord [i];

	return u;
}



template <class real>
DiffEqVector<real> FrenetT<real> :: mul (real v, DiffEqVector<real> w)
{
	DiffEqVector<real> u;
	int i;

	for (i = 0; i < 6; ++i)
		u.coord [i] = v * w.coord [i];

	return u;
}



template <class real>
DiffEqVector<real> FrenetT<real> :: div (DiffEqVector<real> v, real w)
{
	return mul (1 / w, v);
}




////////////////////////////////////////////////////////////////////////
///   approximation methods                                          ///
////////////////////////////////////////////////////////////////////////


template <class real>
DiffEqVector<real> FrenetT<real> :: Euler (real x0, DiffEqVector<real> y0, real h)
{
	return add (y0, mul (h, ode (x0, y0)));
}



template <class real>
DiffEqVector<real> FrenetT<real> :: RungeKutta (real x0, DiffEqVector<real> y0, real h)
{
	DiffEqVector<real> k1 = mul (h, ode (x0, y0));
	DiffEqVector<real> k2 = mul (h, ode (x0 + h / 2, add (y0, div (k1, 2))));
	DiffEqVector<real> k3 = mul (h, ode (x0 + h / 2, add (y0, div (k2, 2))));
	DiffEqVector<real> k4 = mul (h, ode (x0 + h, add (y0, k3)));
	return add (y0, div (add (add (k1, k4), mul (2, add (k2, k3))), 6));
}


////////////////////////////////////////////////////////////////////////
///   approximations                                                 ///
////////////////////////////////////////////////////////////////////////


template <class real>
DiffEqVector<real> FrenetT<real> :: ApproximateDiffEqVector(ApproximationMethod method, real t, real h) {
	real t0 = 0;
	real tmax = t;
	DiffEqVector<real> y0 = _y0; // use copy
	for (;;)
	{

		if (t0 >= tmax) break;

		if(method == ApproximationMethodEuler)	y0 = Euler (t0, y0, h);
		else									y0 = RungeKutta (t0, y0, h);
		t0 += h;
	}

	return y0;
}

template <class real>
std::vector< DiffEqVector<real> > FrenetT<real> :: ApproximateCurve(ApproximationMethod method, real tmax, real h) {
	std::vector< DiffEqVector<real> > data;
	real t0 = 0;
	DiffEqVector<real> y0 = _y0; // use copy
	for (;;)
	{
		data.push_back(y0);

		if (t0 >= tmax) break;

		if(method == ApproximationMethodEuler)	y0 = Euler (t0, y0, h);
		else									y0 = RungeKutta (t0, y0, h);
		t0 += h;
	}

	return data;
}






////////////////////////////////////////////////////////////////////////
///   DiffEqVector class definition                                  ///
////////////////////////////////////////////////////////////////////////

template <class real>
DiffEqVector<real> :: DiffEqVector() {
	for(int i = 0; i < 6; i++) coord[i] = 0;
}

template <class real>
DiffEqVector<real> :: DiffEqVector(real x0, real y0, real x1, real y1, real x2, real y2) {
	coord[0] = x0;
	coord[1] = y0;
	coord[2] = x1;
	coord[3] = y1;
	coord[4] = x2;
	coord[5] = y2;
}

template <class real>
real DiffEqVector<real> :: x0() {
	return coord[0];
}

template <class real>
real DiffEqVector<real> :: y0() {
	return coord[1];
}

template <class real>
real DiffEqVector<real> :: x1() {
	return coord[2];
}

template <class real>
real DiffEqVector<real> :: y1() {
	return coord[3];
}

template <class real>
real DiffEqVector<real> :: x2() {
	return coord[4];
}

template <class real>
real DiffEqVector<real> :: y2() {
	return coord[5];
}





////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef DiffEqVector <float> DiffEqVectorF;
typedef DiffEqVector <double> DiffEqVectorD;
typedef DiffEqVector <long double> DiffEqVectorL;

typedef FrenetT <float> FrenetF;
typedef FrenetT <double> FrenetD;
typedef FrenetT <long double> FrenetL;




#endif /* FRENET_H_ */
