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

/*! \file rimg_test_raw_rw.cpp
\brief impl for finding blobs in an image


*/


#include <assert.h>
#include <memory.h>

#include "t_2D_xy.h"

//#include "rimg_xy_types.h"


#include "rlf_blob.h"
#include "rimg_aoi.h"
#include "rimg_math.h"



using namespace rlf::rimg_math;

namespace rlf {

   namespace {
      int roundToInt( double d ) {
         double dfloor    = floor( d );
         double drem = d - dfloor;

         if( drem > 0.5 ) {
            return ( int )ceil( d );
         }

         return ( int )dfloor;
      }

   }

   tBlob::tBlob(): _integral( 0 ), _area( 0 ), _streaks() {}

   tBlob::~tBlob() {}

   tBlob::tBlob( const tBlob& s )
      :
      _pos( s._pos ),                               // position in image
      _midpoint( s._midpoint ),                     // midpoint ( arithmetic )
      _min( s._min ),                               // minimum position( arithmetic )
      _max( s._max ),                               // maximum position( arithmetic )
      _MaxIntensity( s._MaxIntensity ),             // position and value of maximum value
      _centerOfMass( s._centerOfMass ),             // centroid ( center of mass )
      _integral( s._integral ),                     // area*values
      _area( s._area ),                             // area
      _streaks( s._streaks ) {
   }


   /* -- OPERATORS --
   .                                                                 operators{
   */
   const tBlob& tBlob::operator=( const tBlob& s ) {
      if( this != &s ) {
         _streaks =           s._streaks;
         _pos =               s._pos;                // position in image
         _midpoint =          s._midpoint;           // midpoint ( arithmetic )
         _min =               s._min;                // minimum position( arithmetic )
         _max =               s._max;                // maximum position( arithmetic )
         _MaxIntensity =      s._MaxIntensity;       // position and value of maximum value
         _centerOfMass =      s._centerOfMass;      // centroid ( center of mass )
         _integral =          s._integral;           // area*values
         _area =              s._area;               // area
      }

      return *this;
   }


   tStreak& tBlob::operator[]( uint32_t i ) {
      return _streaks[ i ];
   }
   tStreak const& tBlob::operator[]( uint32_t i )const {
      return _streaks[ i ];
   }



   uint32_t tBlob::sx()const {
      return ( _max.x() + 1 ) - _min.x();
   }
   uint32_t tBlob::sy()const {
      return ( _max.y() + 1 ) - _min.y();
   }
   uint32_xy tBlob::size()const {
      return uint32_xy( sx(), sy() );
   }
   uint32_t tBlob::streak_count()  const {
		 return static_cast<uint32_t>(_streaks.size());
   }



   // valid only after calculation, see OPERATIONS

   uint32_t tBlob::integral( )const {
      return _integral;
   }
   uint32_t tBlob::area()     const {
      return _area;
   }
   int32_xy  tBlob::midpoint()    const {
      return _midpoint;
   }


   double_xy tBlob::MaximumIntensityPos()const {
      return _MaxIntensity.pos;
   }

   double tBlob::MaximumIntensityValue()const {
      return _MaxIntensity.value;
   }
   double_xy   tBlob::CenterOfMass()  const {
      return _centerOfMass;
   }
   tAoi   tBlob::BoundingBox()  const {
      return tAoi( _min, size() ) ;
   }



   void tBlob::calculateMidpnt() {
      calculateSize();
      double mx = _min.x() + _max.x();
      uint32_t xmid =  roundToInt( mx / 2.0 );
      double my = ( _min.y() + _max.y() );
      uint32_t ymid = roundToInt( my / 2.0 );
      _midpoint = int32_xy( xmid, ymid );


   }


   void tBlob::calculateSize() {
      struct csize {
         int32_t xmin;
         int32_t xmax;
         int32_t ymin;
         int32_t ymax;
         csize(): xmin( INT32_MAX ), xmax( 0 ), ymin( INT32_MAX ), ymax( 0 ) {}
         void operator()( tStreak const& s ) {
            int32_t lxmin = s.x();
            int32_t lxmax = s.last();

            if( lxmax > xmax ) {
               xmax = lxmax;
            }

            if( lxmin < xmin ) {
               xmin = lxmin;
            }

            // y
            int32_t y = s.y();

            if( y > ymax ) {
               ymax = y ;
            }

            if( y < ymin ) {
               ymin = y ;
            }
         }

      };
      csize const& s = for_each( _streaks.begin(), _streaks.end(), csize() );
      _min = int32_xy( s.xmin, s.ymin );
      _max = int32_xy( s.xmax, s.ymax );
   }



   void tBlob::calculateIntegral( int background ) {
      struct cintegral {
         uint32_t sum ;
         uint32_t _bgr;
         cintegral( uint32_t bgr ): sum( 0 ), _bgr( bgr ) {}
         void operator()( tStreak const& s ) {
            sum += s.calculateIntegral( _bgr );
         }
      };
      _integral = for_each( _streaks.begin(), _streaks.end(), cintegral( background ) ).sum;

   }

   void tBlob::calculateArea() {
      struct carea {
         uint32_t sum;
         void operator()( tStreak const& s ) {
            sum += s.length();
         }
      };
      _area = for_each( _streaks.begin(), _streaks.end(), carea() ).sum;

   }



   void tBlob::calculatePeakValue() {
      return;
      /*
      assert( p.is_mono8() );
      struct cpeak {
        uint32_t peak;
        cpeak():peak(0){}
        void operator()( tStreak const& s ) {
            if( peak, s.max() )
                peak = s.max();
        }
      };
      uint8_t max = for_each( _streaks.begin(), _streaks.end(), carea() ).peak;
      return max;
      tAoi area_( int32_xy( xmin, ymin ), uint32_xy( x, y ) );
      double_xyz val  = MaximumInArea( p, area_ );
      _MaxIntensity.pos = double_xy( val.x(), val.y() );
      _MaxIntensity.value = val.z();
      */

   }





   /* -- ACCESS --
   */
   void tBlob::push_back( tStreak const& s ) {
      _streaks.push_back( s );
   }
   void tBlob::clear() {
      uint32_t m = streak_count();

      if( m > 0 ) {
         _streaks.clear();
      }
   }


   bool tBlob::ConnectedWithImageBorder( uint32_xy size_ )const {
      if( this->streak_count() == 0 ) {
         return false;
      }

      auto iStreakStart = _streaks.begin();
      auto iStreakEnd = _streaks.end();
      auto bottom  = size_.x() - 1 ;
      auto right   = size_.y() - 1;
			uint32_t top = 0;
			uint32_t left = 0;

      while( iStreakStart != iStreakEnd ) {
         auto x = iStreakStart->x();
         auto y = iStreakStart->y();
         auto last_ = iStreakStart->last();

         if(
            // top
            y == top
            ||

            // bottom
            y == bottom
            ||

            // left
            x == left
            ||

            // right
            last_ == right
         ) {
            return true;
         }

         ++iStreakStart;
      }

      return false;
   }

   bool  tBlob::operator==( const tBlob& s )const {
      auto size1 = streak_count();
      auto size2 = s.streak_count();

      if( size1 != size2 ) {
         return false;
      }

      auto i1 = _streaks.begin();
      auto i2 = s._streaks.begin();

			for (uint32_t i = 0; i < size1; i++) {
         if( *i1 != *i2 ) {
            return false;
         }

         ++i1;
         ++i2;
      }

      return true;
   }
   bool  tBlob::operator!=( const tBlob& s )const {
      return !operator==( s );
   }

   bool tBlob::inside( uint32_xy const& xy )const {
      if( empty() == 0 ) {
         return false;
      }

      auto begin = _streaks.begin();
      auto end = _streaks.end();

      while( begin != end ) {
         if( begin->inside( xy ) ) {
            return true;
         }

         ++begin;
      }

      return false;
   }

   void WriteToImage( vector<tStreak> const& sl, tImgPlanar& p, int color, int border ) {
      assert( p.is_mono8() );
      auto iobj = sl.begin();
      auto iend = sl.end();

      if( border == -1 ) {
         while( iobj != iend ) {
            auto y = iobj->y();
            fill( p[0][ y ].begin() + iobj->x(), p[0][ y ].begin() + iobj->end(), color );
            ++iobj;
         }

         return;
      }

      while( iobj != iend ) {
         if( iobj->length() > 2 ) {

            int y = iobj->y();
            int x = iobj->x();
            fill( p[0][ y ].begin() + x, p[0][ y ].begin() + x + iobj->length(), color );
         }

         p.pixel( uint32_xy( iobj->x(), iobj->y() ), border );
         p.pixel( uint32_xy( iobj->last(), iobj->y() ), border );
         ++iobj;
      }

   }

   double_xy calculateCentroid( tBlob& bo ) {
      struct centroid {
         uint32_t icount;
         uint32_t xsum;
         uint32_t ysum;
         centroid(): icount( 0 ), xsum( 0 ), ysum( 0 ) {}
         void operator()( tStreak const& s ) {
            int l = s.length();

            // -- add y coordinates
            ysum += s.y() * l;

            // -- add x coordinates
            int ix    = s.x();
            icount += l;

            while( l-- ) {
               xsum +=  ix;
               ix++;
            }

         }

      };
      centroid const& s = for_each( bo.streaks().begin(), bo.streaks().end(), centroid() );
      double x = ( double )s.xsum / ( double )s.icount;
      double y = ( double )s.ysum / ( double )s.icount;
      bo._centerOfMass = double_xy( x, y );
      return bo._centerOfMass;

   }


}
//EOF

