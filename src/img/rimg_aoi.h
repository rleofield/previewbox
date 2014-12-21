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


/*! \file rimg_aoi.h
\brief coordinates of an area of interest in an image.

Lib: librimgrw
*/


#ifndef RLF_IMGT_AOI_H
#define RLF_IMGT_AOI_H

#include "t_2D_xy.h"

using rlf_minit::int32_xy;
using rlf_minit::uint32_xy;


namespace rlf {

   class tAoi {
   public:

      tAoi(): _pos(), _size() {}
      tAoi( int32_t ix, int32_t iy, uint32_t sx, uint32_t sy );
      tAoi( int32_xy const& pos0, uint32_xy const& size0 );
      tAoi( const tAoi& in );
      ~tAoi() {}

      tAoi& operator=( const tAoi& s );

      int32_xy pos()const;
      uint32_xy size() const;

      int32_xy upper_left()  const;
      int32_xy lower_right() const;

      int32_xy upper_right()  const;
      int32_xy lower_left() const;


      int32_xy upperleft()  const;
      int32_xy upperright()  const;

      int32_xy lowerright() const;
      int32_xy lowerleft() const;


      int32_xy center()const;

      uint32_t area()const;

      uint32_t  sx()const;
      uint32_t  sy()const;
      void sx( uint32_t );
      void sy( uint32_t );

      int32_t px()const;
      int32_t py()const;
      void px( uint32_t );
      void py( uint32_t );


      int32_t left() {
         return px();
      }
      int32_t top() {
         return py();
      }
      int32_t right() {
         return px() + sx();
      }
      int32_t bottom() {
         return py() + sy();
      }

      bool inside( int32_xy  const& p )const;
      bool overlap( tAoi const& area )const;

   private:
      int32_xy _pos;
      uint32_xy _size;
   };





} // end of namespace nMath


#endif
//EOF

