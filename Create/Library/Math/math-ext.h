
// TODO: Prof. Wirth: can we GPLv3 this?

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
 */

#ifndef MATH_EXT_H_INCLUDED
#define MATH_EXT_H_INCLUDED



#include <cmath>
#include <iostream>

#define PI 3.1415926535897932384626433832795029

// TODO: replace templates because of int problems



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

#endif
