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

/*! \file rimg_aoi.cpp
\brief area of interest

Lib: librimgrw
*/

//cc


#include "rimg_aoi.h"


namespace rlf {


   tAoi::tAoi( int32_t ix, int32_t iy, uint32_t sx_, uint32_t sy_ ):_pos(), _size( )  {
      _pos =  int32_xy( ix, iy );
      _size =   uint32_xy( sx_, sy_ );
   }
   tAoi::tAoi( const tAoi& in ):_pos( in._pos ), _size( in._size ){
   }
   tAoi::tAoi( int32_xy const& pos0, uint32_xy const& size0 ):_pos( pos0.x(),  pos0.y()  ), _size( size0.x(), size0.y() ) {}

   int32_xy tAoi::pos()const {
      return _pos;
   }
   uint32_xy tAoi::size() const {
      return _size;
   }
   //   int32_xy to_tixy( uint32_xy const& xy ) {
   //      return int32_xy( xy.x(), xy.y() );
   //   }

   uint32_t  tAoi::sx()const {
      return _size.x();
   }
   uint32_t  tAoi::sy()const {
      return _size.y();
   }
   void tAoi::sx( uint32_t x ) {
      return _size.setx( x );
   }
   void tAoi::sy( uint32_t y ) {
      return _size.sety( y );
   }
   int32_t    tAoi::px()const {
      return _pos.x();
   }
   int32_t    tAoi::py()const {
      return _pos.y();
   }
   void tAoi::px( uint32_t x ) {
      return _pos.setx( x );
   }
   void tAoi::py( uint32_t y ) {
      return _pos.sety( y );
   }


   bool tAoi::overlap( tAoi  const& area_ )const {
      int s0 = area();
      int s1 = area_.area();

      if( s0 > s1 ) {

         int xend = area_.px() + area_.sx();
         int yend = area_.py() + area_.sy();

         for( int y = area_.py(); y < yend; y++ ) {
            for( int x = area_.px(); x < xend; x++ ) {
               if( inside( int32_xy( x, y ) ) != false ) {
                  return true;
               }
            }
         }
      } else {
         int xend = px() + sx();
         int yend = py() + sy();

         for( int y = py(); y < yend; y++ ) {
            for( int x = px(); x < xend; x++ ) {
               if( area_.inside( int32_xy( x, y ) ) != false ) {
                  return true;
               }
            }
         }
      }

      return false;
   }

   tAoi& tAoi::operator=( const tAoi& s ) {
      if( this != &s ) {
         _pos  = s._pos;
         _size = s._size;
      }

      return *this;
   }

   // including  0 <= n
   int32_xy tAoi::upper_left()  const {
      return _pos;
   }
   int32_xy tAoi::lower_right() const {
      return int32_xy( _pos.x() + _size.x() - 1, _pos.y() + _size.y() - 1 );
   }

   int32_xy tAoi::upper_right()  const {
      return int32_xy( _pos.x() + _size.x() - 1, _pos.y() );
   }
   int32_xy tAoi::lower_left() const {
      return int32_xy( _pos.x(), _pos.y() + _size.y() - 1 );
   }


   int32_xy tAoi::upperleft()  const {
      return upper_left();
   }
   int32_xy tAoi::upperright()  const {
      return upper_right();
   }

   int32_xy tAoi::lowerright() const {
      return lower_right();
   }
   int32_xy tAoi::lowerleft() const {
      return lower_right();
   }


   int32_xy tAoi::center()const {
      int32_xy xy( _pos.x() + _size.x() / 2, _pos.y() + _size.y() / 2 );
      return xy;
   }

   uint32_t tAoi::area()const {
      return sx() * sy();
   }


   bool tAoi::inside( int32_xy const& p )const {
      bool ret = false;
      int x = p.x();
      int y = p.y();

      if( x >=  px()          &&
            x < static_cast<int>( ( px() + sx() ) )  &&
            y >=  py()          &&
            y < static_cast<int>( ( py() + sy() ) ) ) {
         ret = true;
      }

      return ret;
   }







}

//EOF

