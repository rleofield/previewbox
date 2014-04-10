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

/*! \file rlf_blob.h
\brief blob analyse


*/




#ifndef CBINARYOBJECT_H
#define CBINARYOBJECT_H

#include "rimg_planar.h"
#include "rimg_aoi.h"
#include "rlf_streak.h"

using namespace rlf::rimg_planar;
using rlf_minit::double_xy;

namespace rlf {

   struct tMaxIntensity {
      double_xy pos;       // position and value of maximum value
      double value;        // position and value of maximum value

   };

   class tBlob {
      enum { RESERVE_SIZE = 200 };

      // calculated
      int32_xy  _pos;                // position in image
      int32_xy  _midpoint;           // midpoint ( arithmetic )
      int32_xy  _min;                // minimum position( arithmetic ), upper left
      int32_xy  _max;                // maximum position( arithmetic ), lower right
      tMaxIntensity _MaxIntensity;
      double_xy  _centerOfMass;      // centroid ( center of mass )
      uint32_t   _integral;            // area * values in img
      uint32_t   _area;                // area

      vector<tStreak> _streaks;

   public:

      tBlob();
      ~tBlob();

      tBlob( const tBlob& s );

      /* -- OPERATORS --
      */
      const tBlob& operator=( const tBlob& s );
      bool  operator==( const tBlob& s )const;
      bool  operator!=( const tBlob& s )const;

      /* -- ITERATORS --
      */
      tStreak& operator[]( uint32_t i );
      tStreak const& operator[]( uint32_t i )const;

      void push_back( tStreak const& s );
      void clear();


      void calculateIntegral( int background = 0 );
      void calculateArea();
      void calculatePeakValue();
      void calculateMidpnt();
      void calculateSize() ;



      vector<tStreak> const& streaks()const {
         return _streaks;
      }
      bool empty()const {
         return _streaks.size() == 0;
      }



      uint32_t sx()const;
      uint32_t sy()const;
      uint32_xy size()  const;
      uint32_t streak_count()  const;


      // valid only after calculation, see OPERATIONS

      uint32_t integral( )const ;
      uint32_t area()     const ;
      int32_xy  midpoint()     const ;

      int32_xy  UpperLeft()    const ;
      int32_xy  LowerRight()    const ;

      double_xy MaximumIntensityPos()const;
      double MaximumIntensityValue()const;

      double_xy   CenterOfMass()  const;
      tAoi   BoundingBox()  const;

      bool ConnectedWithImageBorder( uint32_xy size )const;
      bool inside( uint32_xy const& xy )const;

      friend double_xy calculateCentroid( tBlob& b );

   };
   double_xy calculateCentroid( tBlob& b );

   void WriteToImage( vector<tStreak> const& sl, tImgPlanar& p, int color, int bordercolor = -1 );

}
#endif
//EOF
