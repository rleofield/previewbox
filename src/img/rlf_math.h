/* --------------------------------------------------------------------------
    Copyright 2013 by Richard Albrecht
    richard.albrecht@rleofield.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ------------------------------------------------------------------------------
*/

/*
Workfile: tMath.h
*/
/*! \file rlf_math.h
\brief header for all math functions


*/


#ifndef CMATH_LIBSTD_H
#define CMATH_LIBSTD_H

#include <vector>
#include <math.h>

#include "t_2D_xy.h"
#include "t_3D_xyz.h"

#include "rimg_planar.h"
//#include "rimg_xy_types.h"
#include "rimg_aoi.h"

using rlf_minit::double_xy;
using rlf_minit::double_xyz;


namespace rlf {
   namespace math {

      const double PI = 3.141592653589793238462643383279502884197169399;
      const double inch = 2.54;

      double Gauss( double stddev, double x );
      double atan2( double x, double y );
      double length2d( const double_xy& xy );
      double length3d( const double_xyz& xy );

      double distance( double_xy const& xy0, double_xy const& xy1 );
      double distance( double_xyz const& xy0, double_xyz const& xy1 );
      double distance( const int32_xy& xy0, const int32_xy& xy1 );

      double square_distance( double_xy const& xy0, double_xy const& xy1 );
      double square_distance( const int32_xy& xy0, const int32_xy& xy1 );
      double square_distance( double_xyz const& xy0, double_xyz const& xy1 );

      double Distance( const double_xyz& d );
      double Distance( const double_xy& d );
      double Distance( const int32_xy& d );
      unsigned long Area( const int32_xy& t );
      unsigned long Pixels( const int32_xy& t );
      double Area( const double_xy& t );
      double Pixels( const double_xy& t );

      double VectorLength( const double_xyz& d );

      double VectorLength( const double_xy& d );
      double VectorLength( const int32_xy& d );

      double interpolate( double x, double_xy xy0, double_xy xy1 );
      double std_dev( const double* buf, double mean, int size );
      double std_dev( vector<double> const& v, double mean );
      double std_dev( vector<double> const& v );
      double mean( const double* buf, int size );
      double mean( vector<double> const& v );
      long rand_long( void );
      double rand_double( void );
      void rand_long_seed( unsigned long seed );
      void rand_double_seed( double dseed );

      double inch_to_cm( double x );
      double cm_to_inch( double x );
      double inch_to_m( double x );
      double m_to_inch( double x );
      double inch_to_mm( double x );
      double mm_to_inch( double x );

      /* -- RG_RAND_.C    -- from SNIPPETS
      */

      double gaussfunction( double stddev0, double dx0, double stddev1, double dx1 );
      double gaussfunction( double stddev, double dx );

      void gauss_convolution( vector< uint32_t > arr );
      void gauss_convolution( uint32_t* arr, int size );
      void gauss_convolution( uint8_t*  arr, int size );
      double rand_gauss();


      void quick_sort( int lo, int hi, uint8_t* base );

      void heap_sort( int* arr, int n );
      void heap_sort( uint16_t* arr, int n );
      void shell_sort( int* arr, size_t n );
      void shell_sort( vector<size_t>& base, size_t n ) ;

      void linear_zoom( const vector<uint8_t>& source, vector<uint8_t>& target );

      int roundToInt( double d );
      int roundToInt( double d );

      int Min( int x, int y );
      int Max( int x, int y );

   } // end of namespace
}

#endif

//EOF
