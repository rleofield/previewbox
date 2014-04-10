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

#ifndef tArea_H
#define tArea_H

#include <assert.h>
#include <string>
#include <float.h>
#include <vector>
#include <cmath>


#include "t_2D_xy.h"




namespace rlf_minit {




   template<typename T >
   class tArea {
   public:

      tArea() {}
      tArea( xy_t<T> pos_, xy_t<T> size_ ): _pos( pos_ ), _size( size_ ) {}

      tArea( const tArea& in ): _pos( in._pos ), _size( in._size )  {}

      virtual ~tArea() {}

      T x() const {
         return _pos.x();
      }
      void setx( T const& x_ ) {
         return _pos.setx( x_ );
      }
      T y() const {
         return _pos.y();
      }
      void sety( T const& y_ ) {
         return _pos.sety( y_ );
      }
      T left()const {
         return x();
      }
      T top()const {
         return y();
      }
      T width()const {
         return dx();
      }
      T heigth()const {
         return dy();
      }
      T w()const {
         return dx();
      }
      T h()const {
         return dy();
      }
      xy_t<T> center()const {
         return _pos + _size / static_cast<T>( 2 );
      }
      T dx()const {
         return _size.x();
      }
      void setdx( T const& dx_ ) {
         return _size.setx( dx_ );
      }
      T dy()const {
         return _size.y();
      }
      void setdy( T const& dy_ ) {
         return _size.sety( dy_ );
      }
      xy_t<T> pos()const {
         return _pos;
      }
      xy_t<T> size()const {
         return _size;
      }
      xy_t<T> start()const {
         return _pos;
      }
      xy_t<T> end()const {
         return _pos + _size;
      }
      xy_t<T> last()const {
         return _pos + _size - xy_t<T>( static_cast<T>( 1 ) );
      }

      // true, if xy is at pos or above, or below pos + size
      bool inside( xy_t<T> const& p )const {
         bool ret = false;
         int x0 = p.x();
         int y0 = p.y();

         if(
            x0 >=  _pos.x()          &&
            x0 < ( _pos.x() + _size.x() )  &&
            y0 >=  _pos.y()          &&
            y0 < ( _pos.y() + _size.y() ) ) {
            ret = true;
         }

         return ret;
      }


      xy_t<T> clamp( xy_t<T> const& xy )const {
         xy_t<T> temp = xy;

         if( temp.x() < _pos.x() ) {
            temp.setx( _pos.x() );
         }

         if( temp.x() >= end().x() ) {
            temp.setx( end().x() - 1 );
         }

         if( temp.y() < _pos.y() ) {
            temp.sety( _pos.y() );
         }

         if( temp.y() >= end().y() ) {
            temp.sety( end().y() - 1 );
         }

         return temp;

      }

   private:
      xy_t<T> _pos;
      xy_t<T> _size;
   };




   // contrains a xy pos to an area, 0, 0, sizex-1, sizey-1
   template<typename T >
   inline xy_t<T> constrain_to_area( xy_t<T> const& constrain, xy_t<T> const& pos, xy_t<T> const& border ) {
      xy_t<T> temp = constrain;

      if( temp.x() <= pos.x() ) {
         temp.setx( pos.x() );
      }

      if( temp.x() >= border.x() ) {
         temp.setx( ( T )border.x() - 1 );
      }

      if( temp.y() <= pos.y() ) {
         temp.sety( pos.y() );
      }

      if( temp.y() >= border.y() ) {
         temp.sety( border.y() - 1 );
      }

      return temp;
   }
   template<typename T >
   inline xy_t<T> clamp_to( xy_t<T> const& constrain, xy_t<T> const& border ) {
      return constrain_to_area( constrain, xy_t<T>(), border );
   }



   typedef tArea<double> double_rect;







} // end of ns pnt





#endif



//EOF
