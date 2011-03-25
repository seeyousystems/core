
// TODO: Prof. Wirth: can we GPLv3 this?

/*
 *  trafo2.h
 *
 *  Two-dimensional transformation matrices such as rotation matrices can be
 *  realized using the Trafo2T template class. Just as the Vector2T class,
 *  mathematical operations on transformation matrices have been implemented.
 *  There are three defined typedefs of Trafo2D (double), Trafo2F (float),
 *  and Trafo2L (long double). Multiple transformations can be appended by
 *  using the * operator.
 *
 *  ===========================================================================
 *
 *  Original implementation by Prof. Dr. Joachim Wirth.
 *  Modifications by Daniel Kruesi (Dan Krusi) and David Grob.
 *
 */

#ifndef TRAFO_2_H_INCLUDED
#define TRAFO_2_H_INCLUDED



#include "vector2.h"



template <class real>
class Trafo2T
{
        //  data

public:  real Data [3][3];


    public:

        //  constructor

            Trafo2T ();


        //  conversions

            Trafo2T (const Trafo2T <float> &);
            Trafo2T (const Trafo2T <double> &);
            Trafo2T (const Trafo2T <long double> &);


        //  binary operators

            Trafo2T operator * (const Trafo2T &) const;
            Vector2T <real> operator * (const Vector2T <real> &) const;


        //  rotation and translation

            static Trafo2T rot (real);
            static Trafo2T trans (real, real);
            static Trafo2T trans (const Vector2T <real> &);

            Trafo2T inv() const{
            	Trafo2T res;
            	res.Data[0][0] = Data[0][0]; 	res.Data[0][1] = Data[1][0]; 	res.Data[0][2] = - Data[0][0] * Data[0][2] - Data[1][0] * Data[1][2];
            	res.Data[1][0] = Data[0][1]; 	res.Data[1][1] = Data[1][1]; 	res.Data[1][2] = - Data[0][1] * Data[0][2] - Data[1][1] * Data[1][2];
            	res.Data[2][0] = 0; 			res.Data[2][1] = 0; 			res.Data[2][2] = 1;
            	return res;
            }

            Vector2T <real> trans () const
            {
            	return Vector2T <real> (Data[0][2],Data[1][2]);
            }

            double angle () const
            {
                return atan2 (Data[1][0], Data[0][0]);
            }

            void print() {
            	for (int j = 0; j < 3; j++){
					for (int k = 0; k < 3; k++){
						std::cout << " " << Data[j][k];
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
            }


};



////////////////////////////////////////////////////////////////////////
///   constructor                                                    ///
////////////////////////////////////////////////////////////////////////

template <class real>
Trafo2T <real> :: Trafo2T ()
{
    for (int i = 0; i < 3; i++)
    for (int k = 0; k < 3; k++)
        Data [i] [k] = i == k;
}



////////////////////////////////////////////////////////////////////////
///   conversions                                                    ///
////////////////////////////////////////////////////////////////////////

template <class real>
Trafo2T <real> :: Trafo2T (const Trafo2T <float> & m)
{
    for (int i = 0; i < 3; i++)
    for (int k = 0; k < 3; k++)
        Data [i] [k] = (real) m.Data [i] [k];
}



template <class real>
Trafo2T <real> :: Trafo2T (const Trafo2T <double> & m)
{
    for (int i = 0; i < 3; i++)
    for (int k = 0; k < 3; k++)
        Data [i] [k] = (real) m.Data [i] [k];
}



template <class real>
Trafo2T <real> :: Trafo2T (const Trafo2T <long double> & m)
{
    for (int i = 0; i < 3; i++)
    for (int k = 0; k < 3; k++)
        Data [i] [k] = (real) m.Data [i] [k];
}



////////////////////////////////////////////////////////////////////////
///   binary operators                                               ///
////////////////////////////////////////////////////////////////////////

template <class real>
Trafo2T <real> Trafo2T <real> :: operator * (const Trafo2T <real> & m) const
{
    Trafo2T res;

    for (int i = 0; i < 3; ++i)
    for (int k = 0; k < 3; ++k)
    {
        real s = 0;
        for (int j = 0; j < 3; ++j)
            s += Data [i] [j] * m.Data [j] [k];
        res.Data [i] [k] = s;
    }
	return res;
}



template <class real>
Vector2T <real> Trafo2T <real> :: operator * (const Vector2T <real> & v) const
{
    return Vector2T <real>
    (
        Data [0] [0] * v (0) + Data [0] [1] * v (1) + Data [0] [2],
        Data [1] [0] * v (0) + Data [1] [1] * v (1) + Data [1] [2]
    );
}



////////////////////////////////////////////////////////////////////////
///   rotation and translation                                       ///
////////////////////////////////////////////////////////////////////////

template <class real>
Trafo2T <real> Trafo2T <real> :: rot (real angle)
{
    real c = Cos (angle);
    real s = Sin (angle);

    Trafo2T t;

    t.Data [0] [0] = c;   t.Data [0] [1] = -s;   t.Data [0] [2] = 0;
    t.Data [1] [0] = s;   t.Data [1] [1] =  c;   t.Data [1] [2] = 0;
    t.Data [2] [0] = 0;   t.Data [2] [1] =  0;   t.Data [2] [2] = 1;

    return t;
}



template <class real>
Trafo2T <real> Trafo2T <real> :: trans (real x, real y)
{
    Trafo2T t;

    t.Data [0] [0] = 1;   t.Data [0] [1] = 0;   t.Data [0] [2] = x;
    t.Data [1] [0] = 0;   t.Data [1] [1] = 1;   t.Data [1] [2] = y;
    t.Data [2] [0] = 0;   t.Data [2] [1] = 0;   t.Data [2] [2] = 1;

    return t;
}



template <class real>
Trafo2T <real> Trafo2T <real> :: trans (const Vector2T <real> & v)
{
    return trans (v.x(), v.y());
}





////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef Trafo2T <float> Trafo2F;
typedef Trafo2T <double> Trafo2D;
typedef Trafo2T <long double> Trafo2L;



#endif
