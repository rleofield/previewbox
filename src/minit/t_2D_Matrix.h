/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
richard.albrecht@rleofield.de
www.rleofield.de

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

#ifndef t_2D_Matrix_h
#define t_2D_Matrix_h

#include <assert.h>
#include <string>
#include <float.h>
#include <vector>
#include <cmath>


#include "t_2D_xy.h"



namespace rlf_minit {

   const size_t msize = 2;

   template < typename T >
   class tVector2 {
      std::vector< T > _v;
   public:
      tVector2(): _v( msize ) {}
      tVector2( tVector2 const& v0 ): _v( v0._v ) {}
      tVector2( T const& v0, T const& v1 ): _v( 2 ) {
         _v[0] = v0;
         _v[1] = v1;
      }
      tVector2& operator=( tVector2<T> const& v0 ) {
         if( this != & v0 ) {
            _v = v0._v;
         }

         return *this;
      }
      bool operator==( tVector2<T> const& v0 )const {
         if( _v == v0._v ) {
            return true;
         }

         return false;
      }
      bool operator!=( tVector2<T> const& v0 )const {
         return !operator==( v0 );
      }

      T const& operator[]( size_t x )const {
         assert( x < 2 );
         return _v[x];
      }
      T& operator[]( size_t x ) {
         assert( x < 2 );
         return _v[x];
      }

   };

   template < typename T >
   class tMatrix2 {
      std::vector< tVector2<T> > _v;
   public:
      tMatrix2(): _v( 2 ) {}
      tMatrix2( T const& v0, T const& v1, T const& v2, T const& v3 ): _v( 2 ) {
         assert( msize == 2 );
         _v[0] = tVector2<T>( v0, v1 );
         _v[1] = tVector2<T>( v2, v3 );
      }

      tMatrix2( tVector2<T> const& v0, tVector2<T> const& v1 ): _v( 2 ) {
         _v[0] = v0;
         _v[1] = v1;
      }
      tMatrix2( tMatrix2<T> const& m0 ): _v( 2 ) {
         _v[0] = m0[0];
         _v[1] = m0[1];
      }
      tMatrix2<T>& operator=( tMatrix2<T> const& m0 ) {
         if( this != & m0 ) {
            _v[0] = m0[0];
            _v[1] = m0[1];
         }

         return *this;
      }

      bool operator==( tMatrix2<T> const& m0 )const {
         if( _v[0] == m0._v[0] && _v[1] == m0._v[1] ) {
            return true;
         }

         return false;
      }
      bool operator!=( tMatrix2<T> const& m0 )const {
         return !operator==( m0 );
      }

      T rowcolvalue( size_t x, size_t y )const {
         assert( x < 2 );
         assert( y < 2 );
         return _v[y][x];
      }
      tVector2<T> const& operator[]( size_t x )const {
         assert( x < 2 );
         return _v[x];
      }
      tVector2<T>& operator[]( size_t x ) {
         assert( x < 2 );
         return _v[x];
      }


   };

   template < typename T >
   inline tMatrix2<T> getScaleMatrix( tVector2<T> s ) {
      tMatrix2< T > _v;
      _v[0][0] = s[0];
      _v[0][1] = T();
      _v[1][0] = T();
      _v[1][1] = s[1];
      return _v;
   }

   template < typename T >
   inline tMatrix2<T> getScaleMatrix( T v00, T v11 ) {
      tMatrix2< T > _v;
      _v[0][0] = v00;
      _v[0][1] = T();
      _v[1][0] = T();
      _v[1][1] = v11;
      return _v;
   }




   template < typename T >
   inline xy_t<T> operator*( xy_t<T> const& xy, tMatrix2< T > const& m2 ) {
      size_t row = 0; // col 0..1
      T x = xy.x() * m2[row][0] + xy.x() * m2[row][1];
      row = 1; // col 0..1
      T y = xy.y() * m2[row][0] + xy.y() * m2[row][1];
      return xy_t<T>( x, y );
   }
   template < typename T >
   inline xy_t<T> operator*( tMatrix2< T > const& m2, xy_t<T> const& xy ) {
      size_t row = 0; // col 0..1
      T x = xy.x() * m2[row][0] + xy.x() * m2[row][1];
      row = 1; // col 0..1
      T y = xy.y() * m2[row][0] + xy.y() * m2[row][1];
      return xy_t<T>( x, y );
   }

   // http://ncalculators.com/matrix/2x2-matrix-multiplication-calculator.htm
   template < typename T >
   inline tMatrix2< T > operator*( tMatrix2< T > const& m1, tMatrix2< T > const& m2 ) {
      // row0         * col0
      T x0 = m1[0][0] * m2[0][0]
             + m1[0][1] * m2[1][0];

      // row0         * col1
      T x1 = m1[0][0] * m2[0][1]
             + m1[0][1] * m2[1][1];

      // row1         * col0
      T x2 = m1[1][0] * m2[0][0]
             + m1[1][1] * m2[1][0];

      // row1         * col1
      T x3 = m1[1][0] *
             m2[0][1]
             + m1[1][1] * m2[1][1];

      return tMatrix2< T >( x0, x1, x2, x3 );
   }


   typedef  tVector2<uint32_t> uint32_v2;
   typedef  tVector2<int32_t> int32_v2;
   typedef  tVector2<float> float_v2;

   typedef  tMatrix2< uint32_t > uint32_m2;
   typedef  tMatrix2< int32_t > int32_m2;
   typedef  tMatrix2< float > float_m2;

   inline float_m2 getInverseIdentityMatrix() {
      float_m2 _v;

      _v[0][0] = -1.0f;
      _v[0][1] = 0.0f;
      _v[1][0] = 0.0f;
      _v[1][1] = -1.0f;

      return _v;
   }
   inline float_m2 getIdentityMatrix() {
      float_m2 _v;

      _v[0][0] = 1.0f;
      _v[0][1] = 0.0f;
      _v[1][0] = 0.0f;
      _v[1][1] = 1.0f;
      return _v;
   }

   // shp sb abs transform in  shp rendering
   inline float_m2 getBaseFlipMatrix( bool_xy flip ) {
      float_m2 _v;
      float x = 1.0f;

      if( flip.hor() ) {
         x = -1.0;
      }

      float y = 1.0f;

      if( flip.ver() ) {
         y = -1.0;
      }

      _v[0][0] = x;
      _v[0][1] = 0.0f;
      _v[1][0] = 0.0f;
      _v[1][1] = y;
      return _v;
   }
   inline float_m2 getBaseFlipMatrix( bool hor, bool ver ) {
      return getBaseFlipMatrix( bool_xy( hor, ver ) );
   }

   // draw on img:
   // -1, if hor
   // -1, if !ver   (ver is reversed)
   inline float_m2 getFlipMatrix( bool_xy flip ) {
      // contruct with getFlipMatrix
      return getBaseFlipMatrix( flip );
   }
   inline float_m2 getFlipMatrix( bool hor, bool ver ) {
      // invert ver
      return getBaseFlipMatrix( bool_xy( hor, !ver ) );
   }

   // draw on canvas, if hor is negative, if ver is negative
   // -1, if hor
   // -1, if ver
   inline float_m2 getFlipMatrixCnv( bool_xy flip ) {
      return getBaseFlipMatrix( flip );
   }
   inline float_m2 getFlipMatrixCnv( bool hor, bool ver ) {
      return getBaseFlipMatrix( bool_xy( hor, ver ) );
   }





}




#endif



//EOF
