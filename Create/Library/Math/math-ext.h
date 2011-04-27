/*
 *  math-ext.h
 *
 *  Provides common math routines used by Spline1T, Trafo2T, and Vector2T.
 *
 *  ===========================================================================
 *
 *  Original implementation by Prof. Dr. Joachim Wirth.
 *  Modifications by Daniel Kruesi (Dan Krusi) and David Grob.
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

#ifndef MATH_EXT_H_INCLUDED
#define MATH_EXT_H_INCLUDED



#include <cmath>


////////////////////////////////////////////////////////////////////////
///   Globals                                                        ///
////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433832795029



////////////////////////////////////////////////////////////////////////
///   Sqrt                                                           ///
////////////////////////////////////////////////////////////////////////

template <class real>
inline real Sqrt (real x)
{
    return sqrt (x);
}



template <>
inline float Sqrt (float x)
{
    return sqrtf (x);
}



template <>
inline long double Sqrt (long double x)
{
    return sqrtl (x);
}



////////////////////////////////////////////////////////////////////////
///   radian and degrees                                             ///
////////////////////////////////////////////////////////////////////////

inline double Rad (int deg)
{
    return deg * 3.1415926535897932384626433832795029 / 180;
}



inline float Rad (float deg)
{
    return deg * 3.1415926535897932384626433832795029F / 180;
}



inline double Rad (double deg)
{
    return deg * 3.1415926535897932384626433832795029 / 180;
}



inline long double Rad (long double deg)
{
    return deg * 3.1415926535897932384626433832795029 / 180;
}



inline float Deg (float rad)
{
    return 180 * rad / 3.1415926535897932384626433832795029F;
}



inline double Deg (double rad)
{
    return 180 * rad / 3.1415926535897932384626433832795029F;
}



inline long double Deg (long double rad)
{
    return 180 * rad / 3.1415926535897932384626433832795029F;
}



////////////////////////////////////////////////////////////////////////
///   Cos                                                            ///
////////////////////////////////////////////////////////////////////////

template <class real>
inline real Cos (real x)
{
    return cos (x);
}



template <>
inline float Cos (float x)
{
    return cosf (x);
}



template <>
inline long double Cos (long double x)
{
    return cosl (x);
}



////////////////////////////////////////////////////////////////////////
///   Sin                                                            ///
////////////////////////////////////////////////////////////////////////

template <class real>
inline real Sin (real x)
{
    return sin (x);
}



template <>
inline float Sin (float x)
{
    return sinf (x);
}



template <>
inline long double Sin (long double x)
{
    return sinl (x);
}


////////////////////////////////////////////////////////////////////////
///   ATan2                                                          ///
////////////////////////////////////////////////////////////////////////

template <class real>
inline real ATan2 (real x, real y)
{
    return atan2 (x, y);
}



template <>
inline float ATan2 (float x, float y)
{
    return atan2f (x, y);
}



template <>
inline long double ATan2 (long double x, long double y)
{
    return atan2l (x, y);
}

template <class T>
inline T Abs (T arg)
{
    return (arg < 0) ? -arg : arg;
}

#endif
