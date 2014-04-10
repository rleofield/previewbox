/*
--------------------------------------------------------------------------
Copyright 2013 by Richard Albrecht
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

/*! \file t_3D_xyz.h
\brief simple representation of a pair of values

Lib: librimgrw
*/

#ifndef t_3D_xyz_H
#define t_3D_xyz_H




#include <cfloat>
#include <cmath>

using namespace std;



namespace rlf_minit {
   template <class T>
   class xyz_t {
      T _x;
      T _y;
      T _z;
   public:

      xyz_t( ): _x( 0 ), _y( 0 ), _z( 0 ) {}
      xyz_t( const T& ix, const T& iy, const T& iz ): _x( ix ), _y( iy ), _z( iz ) {}
      xyz_t( const xyz_t& t ): _x( t._x ), _y( t._y ), _z( t._z ) {}
      virtual ~xyz_t() {}

      xyz_t& operator=( const xyz_t& s ) {
         if( this != &s ) {
            _x = s._x;
            _y = s._y;
            _z = s._z;
         }

         return *this;
      }

      bool operator==( const xyz_t& s )const {
         return s._x == _x && s._y == _y && s._z == _z;
      }

      bool operator!=( const xyz_t& s )const {
         return !( s == *this );
      }

      xyz_t& operator+=( const xyz_t& s ) {
         _x += s._x;
         _y += s._y;
         _z += s._z;
         return *this;
      }
      xyz_t& operator-=( const xyz_t& s ) {
         _x -= s.x;
         _y -= s.y;
         _z -= s.z;
         return *this;
      }

      xyz_t& operator*=( const xyz_t& s ) {
         _x *= s._x;
         _y *= s._y;
         _z *= s._z;
         return *this;
      }

      const T& x()const {
         return _x;
      }
      const T& y()const {
         return _y;
      }
      const T& z()const {
         return _z;
      }
      xyz_t& setx( const T& i ) {
         _x  = i;
         return *this;
      }
      xyz_t& sety( const T& i ) {
         _y  = i;
         return *this;
      }
      xyz_t& setz( const T& i ) {
         _z  = i;
         return *this;
      }
      xyz_t& AddX( const T& i ) {
         _x += i;
         return *this;
      }
      xyz_t& AddY( const T& i ) {
         _y += i;
         return *this;
      }
      xyz_t& AddZ( const T& i ) {
         _z += i;
         return *this;
      }
   };


   //      inline float ratio( float_xy const& p ) {
   //         return p.w() / p.h();
   //      }

   //      inline double_xy center( double_xy const& t ) {
   //         return double_xy( t.x() / 2, t.y() / 2 );
   //      }



   typedef xyz_t< int > tixyz;
   typedef xyz_t< uint32_t > tdwxyz;
   typedef xyz_t< double > double_xyz;
   typedef xyz_t< uint32_t > uint32_xyz;
   typedef xyz_t< int32_t > int32_xyz;


   /*
      inline uint32_xy to_tdwxy( int32_xy const& xy ) {
         return uint32_xy( xy.x(), xy.y() );
      }
      inline int32_xy to_tixy( uint32_xy const& xy ) {
         return tixy( xy.x(), xy.y() );
      }
      inline xy_size_t to( int32_xy xy ){
          return xy_size_t(xy.x(), xy.y() );
      }
   */

} // end of ns xy

#endif



//EOF
