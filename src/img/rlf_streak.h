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

/*! \file rlf_streak.h
\brief finds streaks in image, used in blobs

*/

#ifndef CSTREAK_H
#define CSTREAK_H

#include <vector>
#include <cstdint>

#include "t_2D_xy.h"
//#include "rimg_typedefs.h"
//#include "rimg_xy_types.h"

using std::vector;
using rlf_minit::uint32_xy;



/* -- BINSTRK_.C
*/

namespace rlf {




   class tStreak {
      uint32_t  _x;     // -- starting coordinate of streak
      uint32_t  _y;     // -- starting coordinate of streak
      vector<uint32_t> _pix;
      uint32_t _sum;

      explicit tStreak( uint32_xy const& xy ): _x( xy.x() ), _y( xy.y() ), _pix(), _sum( 0 ) {}

   public:
      static tStreak Create( uint32_xy const& xy );
      tStreak(): _x( 0 ), _y( 0 ), _pix(), _sum( 0 ) {}
      tStreak( const tStreak& s ):
         _x( s._x ), _y( s._y ), _pix( s._pix ), _sum( s._sum ) {}

      ~tStreak() {}

      bool operator==( const tStreak& s )const {
         if( _x == s._x && _y == s._y && length() == s.length() ) {
            return true;
         }

         return false;
      }
      bool operator!=( const tStreak& s )const {
         return !operator==( s );
      }

      tStreak& operator=( const tStreak& in ) {
         if( this != &in ) {
            _x = in._x;
            _y = in._y;
            _pix = in._pix;
            _sum = in._sum;
         }

         return *this;
      }

      uint32_t x()const {
         return _x;
      }
      uint32_t y()const {
         return _y;
      }

      void add( uint8_t val ) {
         _pix.push_back( val );
         _sum += val;
      }
      vector<uint32_t> const& pix()const {
         return _pix;
      }

      uint32_t length()const {
         return static_cast<uint32_t>( _pix.size() );
      }
      uint32_t end() const {
         return x() + length();
      }
      uint32_t last() const {
         if( empty() ) {
            return x();
         }

         return _x + length() - 1;
      }
      bool empty()const {
         return length() == 0 ;
      }

      bool inside( uint32_xy const& test ) const {
         if( empty() ) {
            return false;
         }

         if( y() == test.y() && x() <= test.x() && ( x() + length() ) > test.x() ) {
            return true;
         }

         return false;
      }
      uint32_t calculateIntegral( int background )const {
         uint32_t s0 = _sum - length() * background;
         return s0;

      }

   };


}
#endif
//EOF

