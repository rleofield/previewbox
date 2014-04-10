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

/*! \file rimg_imgsize_type.h
\brief type to represent the size of an image

Lib: librimgrw
*/


#ifndef imgrw_size_H
#define imgrw_size_H


#include <cstdint>
#include <string>


#include "t_2D_xy.h"





using rlf_minit::uint32_xy;


namespace rlf {

   inline uint32_t align32( uint32_t s ) {
      if( s & 0x3 ) {  // 0011,  1,2, oder 3
         s |= 0x3;     // set to 0x11, (3)
         ++s;          // add 1 to align
      }

      return s;
   }


   class tSize {
      uint32_xy  _size;
   public:
      tSize( uint32_t x_, uint32_t y_ ): _size( uint32_xy( x_, y_ ) ) {}
      tSize( uint32_xy const& xy ): _size( xy ) {}
      tSize(): _size( uint32_xy( 0, 0 ) ) {}
      void setx( uint32_t i ) {
         _size.setx( i );
      }
      void  sety( uint32_t i ) {
         _size.sety( i );
      }
      uint32_t x()const {
         return _size.x();
      }
      uint32_t y()const {
         return _size.y();
      }
      uint32_t scan_line_size()const {
         return align32( x() );
      }

      // as w/h
      void setw( uint32_t i ) {
         _size.setx( i );
      }
      void seth( uint32_t i ) {
         _size.sety( i );
      }
      uint32_t w()const {
         return _size.x();
      }
      uint32_t h()const {
         return _size.y();
      }

      bool operator==( tSize const& source )const {
         return  _size == source._size;
      }
      bool operator!=( tSize const& source )const {
         return !operator==( source );
      }
      uint32_t area()const {
         return x() * y();
      }
      uint32_t pixels()const {
         return x() * y();
      }
      uint32_t bytes()const {
         return align32( x() ) * y();
      }
      operator uint32_xy()const {
         return _size;
      }

   };

}
#endif
//EOF
