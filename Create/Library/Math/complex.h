/*
 *  complex.h
 *
 *  Represents a complex number with all operators implemented (*+-/) as well
 *  as related functions such as sin, exp, conjugate, sqrt, pow, et cetera...
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

#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>

#include "math-ext.h"

template <class real>
class ComplexT
{
        //  data

		real _re;
		real _im;


    public:

        //  constructors

            ComplexT (void);
            ComplexT (real, real);


        //  conversions

            ComplexT (const ComplexT <float> &);
            ComplexT (const ComplexT <double> &);
            ComplexT (const ComplexT <long double> &);


        //  components

            real re (void) const;
            real im (void) const;
            real r (void) const; // radius
            real abs (void) const; // absolute value, same as r
            real arg (void) const; // argument, angle sigma

            real & re (void);
            real & im (void);



        //  unary operators

            const ComplexT <real> & operator + (void) const;
            const ComplexT <real> operator - (void) const;


        //  binary operators

            ComplexT operator + (const ComplexT &) const;
            ComplexT operator - (const ComplexT &) const;
            ComplexT operator * (const ComplexT &) const;
            ComplexT operator / (const ComplexT &) const;
            ComplexT operator * (real) const;
            ComplexT operator / (real) const;
            bool operator == (const ComplexT &) const;


        //  assignment operators

            ComplexT & operator =  (const ComplexT &);
            ComplexT & operator += (const ComplexT &);
            ComplexT & operator -= (const ComplexT &);
            ComplexT & operator *= (real);
            ComplexT & operator /= (real);



};



////////////////////////////////////////////////////////////////////////
///   constructors                                                   ///
////////////////////////////////////////////////////////////////////////

template <class real>
ComplexT <real> :: ComplexT (void)
{
}



template <class real>
ComplexT <real> :: ComplexT (real rea, real ima)
{
    re() = rea;
    im() = ima;
}



////////////////////////////////////////////////////////////////////////
///   conversions                                                    ///
////////////////////////////////////////////////////////////////////////

template <class real>
ComplexT <real> :: ComplexT (const ComplexT <float> & v)
{
    re() = (real) v.re();
    im() = (real) v.im();
}



template <class real>
ComplexT <real> :: ComplexT (const ComplexT <double> & v)
{
    re() = (real) v.re();
    im() = (real) v.im();
}



template <class real>
ComplexT <real> :: ComplexT (const ComplexT <long double> & v)
{
    re() = (real) v.re();
    im() = (real) v.im();
}



////////////////////////////////////////////////////////////////////////
///   components                                                     ///
////////////////////////////////////////////////////////////////////////

template <class real>
real ComplexT <real> :: re (void) const
{
    return _re;
}



template <class real>
real ComplexT <real> :: im (void) const
{
    return _im;
}


template <class real>
real ComplexT <real> :: r (void) const
{
	// r = sqrt(a1^2 + a2^2)
    return Sqrt(re()*re() + im()*im());
}


template <class real>
real ComplexT <real> :: abs (void) const
{
    return r();
}


template <class real>
real ComplexT <real> :: arg (void) const
{
	// arg = sign(a2) * arccos(a1/r)
    return Sign( im() ) * Acos( re() / r() );
}





template <class real>
real & ComplexT <real> :: re (void)
{
    return _re;
}



template <class real>
real & ComplexT <real> :: im (void)
{
    return _im;
}






////////////////////////////////////////////////////////////////////////
///   unary operators                                                ///
////////////////////////////////////////////////////////////////////////

template <class real>
const ComplexT <real> & ComplexT <real> :: operator + (void) const
{
    return *this;
}



template <class real>
const ComplexT <real> ComplexT <real> :: operator - (void) const
{
    return ComplexT <real> (-re(), -im());
}



////////////////////////////////////////////////////////////////////////
///   binary operators                                               ///
////////////////////////////////////////////////////////////////////////

template <class real>
ComplexT <real> ComplexT <real> :: operator + (const ComplexT <real> & v) const
{
    return ComplexT <real> (re() + v.re(), im() + v.im());
}



template <class real>
ComplexT <real> ComplexT <real> :: operator - (const ComplexT <real> & v) const
{
    return ComplexT <real> (re() - v.re(), im() - v.im());
}



template <class real>
ComplexT <real> ComplexT <real> :: operator * (const ComplexT <real> & v) const
{
	// a,b in C
	// a * b = (a1 + ja2) * (b1 + jb2)
	//       = a1b1 - a2b2 + i(a1b2 + a2b1)
    return ComplexT <real> (re()*v.re() - im()*v.im(), re()*v.im() + im()*v.re());
}



template <class real>
ComplexT <real> ComplexT <real> :: operator / (const ComplexT <real> & v) const
{
	// a,b in C
	// a : b = (a1 + ja2) : (b1 + jb2)
	//       = ( a1b1 + a2b2 + i(a2b1 - a1b2) ) / ( b1^2 + b2^2 )
	real denom = v.re()*v.re() + v.im()*v.im();
    return ComplexT <real> ( ( re()*v.re() + im()*v.im() ) / denom, ( im()*v.re() - re()*v.im() ) / denom  );
}



template <class real>
ComplexT <real> ComplexT <real> :: operator * (real s) const
{
    return ComplexT <real> (re() * s, im() * s);
}



template <class real>
ComplexT <real> ComplexT <real> :: operator / (real s) const
{
    return ComplexT <real> (re() / s, im() / s);
}



template <class real>
ComplexT <real> operator * (real s, const ComplexT <real> & v)
{
    return ComplexT <real> (v.re() * s, v.im() * s);
}

template <class real>
ComplexT <real> operator / (real s, const ComplexT <real> & v)
{
	// a in C
	// s in R
	// s / a = s*a1/(a1^2+a2^2) + i s*a2/(a1^2+a2^2)
	real denom = v.re()*v.re() + v.im()*v.im();
    return ComplexT <real> (s*v.re()/denom, s*v.im()/denom);
}




template <class real>
ComplexT <real> & ComplexT <real> :: operator = (const ComplexT <real> & v)
{
    re() = v.re(); im() = v.im(); return *this;
}



template <class real>
ComplexT <real> & ComplexT <real> :: operator += (const ComplexT <real> & v)
{
    re() += v.re(); im() += v.im(); return *this;
}



template <class real>
ComplexT <real> & ComplexT <real> :: operator -= (const ComplexT <real> & v)
{
    re() -= v.re(); im() -= v.im(); return *this;
}



template <class real>
ComplexT <real> & ComplexT <real> :: operator *= (real s)
{
    re() *= s; im() *= s; return *this;
}



template <class real>
ComplexT <real> & ComplexT <real> :: operator /= (real s)
{
    re() /= s; im() /= s; return *this;
}

template <class real>
bool ComplexT <real> :: operator == (const ComplexT <real> & v) const
{
    return re()==v.re() && im()==v.im();
}



////////////////////////////////////////////////////////////////////////
///   related functions                                              ///
////////////////////////////////////////////////////////////////////////

template <class real>
ComplexT <real> pow (const ComplexT <real> & v, const real n) {
	// [cos(sigma) + i sin(sigma)]^n = abs(v) * (cos(n sigma) + i sin(n sigma)) for n in N
	real sigma = v.arg();
	return v.abs() * ComplexT<real>( Cos( n * sigma ) , Sin( n * sigma) );
}

template <class real>
ComplexT <real> sqrt (const ComplexT <real> & v) {
	// c1 = root(r,n) * cis( sigma/n )
	// c2 = root(r,n) * cis( sigma/n +         (2*PI/n) )
	// c3 = root(r,n) * cis( sigma/n +     2 * (2*PI/n) )
	// .
	// .
	// .
	// cn = root(r,n) * cis( sigma/n + (n-1) * (2*PI/n) )
	//
	// I will just calculate c1 for n = 2:
	// c1 = sqrt(r) * cis ( sigma/n )
	real sigma = v.arg();
	return Sqrt(v.r()) * ComplexT<real>( Cos( sigma / 2.0 ) , Sin( sigma / 2.0 ) );
}

template <class real>
ComplexT <real> conjugate (const ComplexT <real> & v) {
	return ComplexT<real>( v.re() , -v.im() );
}

template <class real>
ComplexT <real> exp (const ComplexT <real> & v) {
	// exp(a) = exp(a1) * cis(a2) for a in C
	return exp(v.re()) * ComplexT<real>( Cos( v.im() ) , Sin( v.im() ) );
}

template <class real>
ComplexT <real> sin (const ComplexT <real> & v) {
	// sin(a) = ( exp(ia) - exp(-ia) ) / 2i for a in C
	ComplexT<real> i(0,1);
	return (exp(i*v) - exp(-i*v) ) / 2*i;
}





//
//////////////////////////////////////////////////////////////////////////
/////   text input and output                                          ///
//////////////////////////////////////////////////////////////////////////

//template <class real>
//std :: istream & operator >> (std :: istream & file, ComplexT <real> & v)
//{
//    return file
//        >> v.re()
//        >> v.im();
//}



template <class real>
std :: ostream & operator << (std :: ostream & file, const ComplexT <real> & v)
{
    int w = file.width();

    if(v.im() < 0) {
    	return file
				<< "("
    	        << std :: setw (w) << v.re()
    	        << "-"
    	        << std :: setw (w) << v.im()*-1
    	        << "i)";
    } else {
    	return file
				<< "("
    	        << std :: setw (w) << v.re()
    	        << "+"
    	        << std :: setw (w) << v.im()
    	        << "i)";
    }
}



////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef ComplexT <float> ComplexF;
typedef ComplexT <double> ComplexD;
typedef ComplexT <long double> ComplexL;

#endif /* COMPLEX_H_ */
