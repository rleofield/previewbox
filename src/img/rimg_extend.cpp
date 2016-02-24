
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
/*! \file rimg_extend.cpp
\brief extend an image at border by n  pixels


*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>
#include <iostream>
#include <math.h>

#include "t_2D_xy.h"
#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rimg_math.h"
//#include "rimg_read_write.h"
#include "rimg_planar.h"
#include "timer.h"
#include "str_util.h"

using namespace std;




namespace rlf {

   // test copy cron

   namespace rimg_math {



      void ToExtendedImageArea( const tPlane8& in,
                                tPlane8& out, uint32_t border ) {


         auto yinbegin = in.begin();
         auto yinend = in.end();
         auto youtbegin = out.begin() + border;

         while( yinbegin != yinend ) {
            copy( yinbegin->begin(), yinbegin->end(), youtbegin->begin() + border );
            ++yinbegin;
            ++youtbegin;
         }
      }

      void ToExtendedLinearArea( const tPlane8& in, tPlaneLinear8& out, uint32_t border ) {

         if( in.size() == 0 ) {
            return;
         }

         uint32_t sx = static_cast<uint32_t>( in[0].size() );
         uint32_t sy = static_cast<uint32_t>( in.size() );

         uint32_xy xy = uint32_xy( sx, sy );
         xy += ( 2 * border );
         out.size() = xy;
         out.resize();

         auto ybegin = in.begin();
         auto yend = in.end();

         uint32_t offset = out.pitch() + 1;

         while( ybegin != yend ) {
            auto xbegin = ybegin->begin();
            auto xend   = ybegin->end();

            out.insert_data( *ybegin, offset );
            offset += out.pitch();
            ++ybegin;
         }
      }

      /////////////////////////////////////////////////////////////////

      void extend_lines_at_all_sides( tPlane8& image, uint32_t half ) {
         uint32_t s = half;

         uint32_t xend = static_cast<uint32_t>( image[0].size() );
         uint32_t x_intern_end = xend - s;
         uint32_t x_intern_last = x_intern_end - 1;

         uint32_t yend = static_cast<uint32_t>( image.size() );
         uint32_t y_intern_end = yend - s;
         uint32_t y_intern_last = y_intern_end - 1;


         // first s lines
         for( uint32_t yborder = 0; yborder < s; yborder++ ) {
            image[ yborder ] = image[ s ];
         }

         // last s lines
         for( uint32_t yborder = 0; yborder < s; yborder++ ) {
            image[ y_intern_end + yborder ] = image[ y_intern_last ];
         }

         // corners
         uint8_t pix1 = image[s][s];
         uint8_t pix2 = image[s][x_intern_last];
         uint8_t pix3 = image[y_intern_last][s];
         uint8_t pix4 = image[y_intern_last][x_intern_last];

         for( uint32_t xborder = 0; xborder < s; xborder++ ) {

            for( uint32_t yborder = 0; yborder < s; yborder++ ) {
               //  1. corner
               image[yborder][xborder] = pix1 ;
               //  2. corner
               image[yborder][x_intern_end + xborder] = pix2 ;
               //  3. corner
               image[y_intern_end + yborder][xborder] = pix3;
               //  4. corner
               image[y_intern_end + yborder][x_intern_end + xborder] = pix4;
            }
         }

         // first and last columns
         for( uint32_t y = s; y < y_intern_end; y++ ) {

            for( uint32_t x = 0; x < s; x++ ) {
               image[y][x] = image[y][s];
            }
         }

         for( uint32_t y = s; y < y_intern_end; y++ ) {

            for( uint32_t x = 0; x < s; x++ ) {
               image[y][x_intern_end + x] = image[y][x_intern_last];
            }
         }
      }

      void extend_lines_at_all_sides( tImgPlanar& image, uint32_t half ) {

         auto mbegin = image.begin();
         auto mend   = image.end();

         while( mbegin != mend ) {
            if( image.has_bit_at( mbegin ) ) {
               extend_lines_at_all_sides( mbegin->plane, half );
            }

            ++mbegin;
         }
      }



      void rimginterface::ToExtendedImageArea( const tImgViewPlanar& in,
            tImgPlanar& extended, uint32_t border ) {

         assert( in.mask().val() == extended.mask().val() ) ;

         tSize ts = in.size();
         uint32_xy s = ts;
         s = s + uint32_xy( ( 2 * border ) );
         extended.realloc( s, in.mask() );

         auto mbegin = in.begin();
         auto mend = in.end();

         auto moutbegin = extended.begin();

         while( mbegin != mend ) {
            if( in.has_bit_at( mbegin ) ) {
               rimg_math::ToExtendedImageArea( mbegin->plane, moutbegin->plane, border );
            }

            ++mbegin;
         }
      }

      // kernel is already odd
      void rimginterface::ExtendImage( const tImgViewPlanar& in, tImgPlanar& extended, uint32_t kernel ) {

         assert( !( kernel < 3 || kernel > 7 ) );
         assert( is_odd( kernel ) );

         auto half = kernel / 2;

         uint32_xy s = in.size();
         s += ( 2 * half );
         extended.realloc( s, in.mask() );

         ToExtendedImageArea( in,   extended, half );
         extend_lines_at_all_sides( extended, half );

      }

      void rimginterface::FitToImgPlanar( tIntPlane32 const& source, tPlane8& target, int32_xy minmax ) {

         int32_t min = minmax.x();
         int32_t max = minmax.y();
         int32_t delta = max - min;

         if( delta == 0 ) {
            return;
         }





         auto ybegin = source.begin();
         auto yend   = source.end();
         auto target_ybegin   = target.begin();

         while( ybegin != yend ) {

            auto xbegin = ybegin->begin();
            auto xend   = ybegin->end();
            auto target_xbegin   = target_ybegin->begin();


            while( xbegin < xend ) {
               *target_xbegin = static_cast<uint8_t>( ( max_byte * ( *xbegin - min ) ) / ( delta ) );
               ++xbegin;
               ++target_xbegin;
            }

            ++ybegin;
            ++target_ybegin;
         }

      }

      void rimginterface::FitToImgPlanar( tIntPlane32 const& source, tImgPlanar& target, int32_xy minmax ) {

         int32_t min = minmax.x();
         int32_t max = minmax.y();
         int32_t delta = max - min;

         if( delta == 0 ) {
            return;
         }


         uint32_t sx = static_cast<uint32_t>( source[0].size() );
         uint32_t sy = static_cast<uint32_t>( source.size() );
         target.realloc( tSize( sx, sy ), tMask::mono );


         auto ybegin = source.begin();
         auto yend   = source.end();
         auto target_mbegin   = target.begin();
         auto target_ybegin   = target_mbegin->begin();

         while( ybegin != yend ) {

            auto xbegin = ybegin->begin();
            auto xend   = ybegin->end();
            auto target_xbegin   = target_ybegin->begin();


            while( xbegin < xend ) {
               *target_xbegin =  static_cast<uint8_t>(   ( max_byte * ( *xbegin - min ) ) / ( delta ) );
               ++xbegin;
               ++target_xbegin;
            }

            ++ybegin;
            ++target_ybegin;
         }

      }

   }

} // end of namespace rlf

//EOF




