
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

/*! \file rimg_sobel.cpp
\brief impl for sobel operator in images


*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

//#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rimg_math.h"
//#include "rimg_read_write.h"
#include "rimg_planar.h"
#include "timer.h"
#include "str_util.h"

using namespace std;

using rlf_htime::tTimer;


namespace rlf {

   // test copy cron

   namespace rimg_math {


      typedef  vector< vector<int32_t> > tMatrix32;







      // --  horizontal edges for sobel operator


      int32_xy SobelHorizontal( tPlane8& extended, tIntPlane32& hor ) {
         /*
         horizontal values ( result is signed !!)
         int ker_ver[]={
         1,2,1,
         0,0,0,
         -1,-2,-1
         */


         int32_t min = INT32_MAX;
         int32_t max = INT32_MIN;


         auto ybegin_m1 = extended.begin(); // [iy-1]
         auto ybegin    = ybegin_m1 + 1; // [iy]
         auto ybegin_p1 = ybegin_m1 + 2; // [iy+1]

         auto yend = extended.end() - 1;

         auto hor_ybegin = hor.begin();

         while( ybegin != yend ) {
            auto xbegin = ybegin->begin() + 1; // [iy][ix]
            auto xend   = ybegin->end() - 1;

            vLine8::const_iterator xbegin_m1 = ybegin_m1->begin() + 1; // [iy-1][ix]
            vLine8::const_iterator xbegin_p1 = ybegin_p1->begin() + 1; // [iy+1][ix]

            tIntLine32::iterator hor_xbegin = hor_ybegin->begin();


            size_t ix = 1;

            while( xbegin != xend ) {


               int32_t val =
                  // 1,2,1,  row - 1, from -1 - +1
                  ( *( xbegin_m1 - 1 )       +
                    ( *( xbegin_m1 ) << 1 )    +
                    * ( xbegin_m1 + 1 ) )
                  -
                  (
                     // -1,-2,-1,  row + 1, from -1 - +1
                     * ( xbegin_p1 - 1 )     +
                     ( *xbegin_p1 << 1 ) +
                     * ( xbegin_p1 + 1 )
                  );

               if( val > max ) {
                  max = val;
               }

               if( val < min ) {
                  min = val;
               }

               *hor_xbegin = val;

               ++xbegin;
               ++xbegin_m1;
               ++xbegin_p1;
               ++hor_xbegin;
               ix++;


            }

            ++ybegin;
            ++ybegin_m1;
            ++ybegin_p1;
            ++hor_ybegin;
            //iy++;
         }

         return int32_xy( min, max );

      }

      int32_xy SobelHorizontal_xy( tPlane8& extended, tIntPlane32& hor ) {
         /*
         horizontal values ( result is signed !!)
         int ker_ver[]={
         1,2,1,
         0,0,0,
         -1,-2,-1
         */

         int32_t min = INT32_MAX;
         int32_t max = INT32_MIN;

         size_t iy = 1;

         size_t y = extended.size() - 1;

         while( iy  < y ) {


            size_t ix = 1;
            size_t xend = extended[1].size() - 1;

            while( ix < xend ) {

               int32_t val =
                  // 1,2,1,  row - 1, from -1 - +1
                  extended [  iy - 1][ix - 1]        +
                  ( extended [iy - 1][ix]  << 1 )    +
                  extended [  iy - 1][ix + 1]
                  -
                  (
                     // -1,-2,-1,  row + 1, from -1 - +1
                     extended [  iy + 1 ][ ix - 1 ]      +
                     ( extended [  iy + 1 ][ ix   ] << 1 ) +
                     extended [  iy + 1 ][ ix + 1]
                  );

               if( val > max ) {
                  max = val;
               }

               if( val < min ) {
                  min = val;
               }

               hor[iy - 1][ix - 1] = val;

               ix++;

            }

            iy++;
         }

         return int32_xy( min, max );
      }


      int32_xy SobelVertical( tPlane8& extended, tIntPlane32& ver ) {
         /*
         vertikal values ( result is signed !!)
         int ker_ver[]={
         1,0,-1,
         2,0,-2,
         1,0,-1
         */


         int32_t min = INT32_MAX;
         int32_t max = INT32_MIN;

         // row iterator starts at +1, ends at end -1


         //size_t y = 1;
         //size_t ycount = extended.size() - 1;

         auto ybegin_m1 = extended.begin(); // [iy-1]
         auto ybegin    = ybegin_m1 + 1; // [iy]
         auto ybegin_p1 = ybegin_m1 + 2; // [iy+1]

         auto yend = extended.end() - 1;

         auto ver_ybegin = ver.begin();

         while( ybegin != yend ) {

            auto xbegin_ym1 = ybegin_m1->begin() + 1; // [iy-1][ix]
            auto xbegin     = ybegin->begin() + 1;    // [iy][ix]
            auto xbegin_yp1 = ybegin_p1->begin() + 1; // [iy+1][ix]
            auto xend   = ybegin->end() - 1;

            auto ver_xbegin = ver_ybegin->begin();

            while( xbegin != xend ) {


               // vertical at pos (x-1) 1,2,1
               // [y-1][x-1]

               int  ym1_xm1 = *( xbegin_ym1 - 1 );
               // [y-0][x-1]
               int  ym0_xm1 = *( xbegin - 1 );
               // [y+1][x-1]
               int  yp1_xm1 = *( xbegin_yp1 - 1 );


               // vertical at pos (x+1) 1,2,1
               // [y-1][x+1]
               int  ym1_xp1 = *( xbegin_ym1 + 1 );
               // [y-0][x+1]
               int  ym0_xp1 = *( xbegin + 1 );
               // [y+1][x+1]
               int  yp1_xp1 = *( xbegin_yp1 + 1 );


               // first line, 1,0,-1
               int first = ym1_xm1 - ym1_xp1;
               // middle line 2,0,-2,
               int middle = ( ym0_xm1 << 1 ) - ( ym0_xp1 << 1 );
               // last line, 1,0,-1
               int last = yp1_xm1 - yp1_xp1;

               int32_t val = first + middle + last;

               if( val > max ) {
                  max = val;
               }

               if( val < min ) {
                  min = val;
               }

               //if( val != val2 ){
               //    *ver_xbegin = val;
               //}
               *ver_xbegin = val;


               // target
               ++ver_xbegin;
               //               ++ver_xbegin1;
               // increment linepointer
               ++xbegin_ym1;
               ++xbegin;
               ++xbegin_yp1;




            }

            ++ybegin;
            ++ybegin_m1;
            ++ybegin_p1;
            ++ver_ybegin;
            //y++;

         }

         return int32_xy( min, max );

      }


      int32_xy SobelVertical_xy( tPlane8& extended, tIntPlane32& ver ) {
         /*
         vertikal values ( result is signed !!)
         int ker_ver[]={
         1,0,-1,
         2,0,-2,
         1,0,-1
         */



         int32_t min = INT32_MAX;
         int32_t max = INT32_MIN;


         size_t iy = 1;


         size_t y = extended.size() - 1;

         while( iy  < y ) {


            size_t ix = 1;
            size_t xend = extended[1].size() - 1;

            while( ix < xend ) {


               int32_t val =
                  // vertical at pos (x-1) 1,2,1
                  extended [  iy - 1][ix - 1]       +
                  ( extended [iy][ix - 1]  << 1 )    +
                  extended [  iy + 1][ix - 11]
                  -
                  (
                     // -1,-2,-1,  row + 1, from -1 - +1
                     extended [  iy - 1][ix + 1]     +
                     ( extended [iy][ix + 1] << 1 ) +
                     extended [  iy + 1][ix + 1]
                  );

               if( val > max ) {
                  max = val;
               }

               if( val < min ) {
                  min = val;
               }

               ver[iy - 1][ix - 1] = val;

               ix++;

            }

            iy++;
         }


         return int32_xy( min, max );

      }

      inline uint32_t square( uint32_t x ) {
         return x * x;
      }
      inline int32_t int_abs( int32_t x ) {
         if( x < 0 ) {
            return -x;
         }

         return x;
      }


      int32_xy SobelOrientation( tIntPlane32 const& hor ,
                                 tIntPlane32 const& ver ,
                                 tIntPlane32& orientation_img
                               ) {



         int32_t orientmin = INT32_MAX;
         int32_t orientmax = INT32_MIN;

         tIntPlane32::const_iterator hor_ybegin = hor.begin();
         tIntPlane32::const_iterator hor_yend   = hor.end();
         tIntPlane32::const_iterator ver_ybegin = ver.begin();
         tIntPlane32::iterator orient_ybegin       = orientation_img.begin();



         while( hor_ybegin != hor_yend ) {

            tIntLine32::const_iterator hor_xbegin = hor_ybegin->begin();
            tIntLine32::const_iterator hor_xend   = hor_ybegin->end();
            tIntLine32::const_iterator ver_xbegin = ver_ybegin->begin();

            tIntLine32::iterator orient_xbegin = orient_ybegin->begin();

            while( hor_xbegin != hor_xend ) {
               int32_t valhor = *hor_xbegin;
               int32_t valver = *ver_xbegin;



               double d_orient = M_PI / 2.0;

               //if( valhor != 0 ) {
               if( valhor != 0 ) {
                  d_orient = atan( ( double ) valver / ( double ) valhor );
               }

               //}
               int32_t v = static_cast<int32_t>( ( d_orient ) * 256.0 );

               if( valhor == 0 ) {
                  v = v;
               }

               if( valhor > 0 ) {
                  v = v;
               }


               *orient_xbegin = v;

               if( *orient_xbegin < orientmin ) {
                  orientmin = *orient_xbegin;
               }

               if( *orient_xbegin > orientmax ) {
                  orientmax = *orient_xbegin;
               }


               ++hor_xbegin;
               ++ver_xbegin;
               ++orient_xbegin;

            }

            ++hor_ybegin;
            ++ver_ybegin;
            ++orient_ybegin;

         }

         return int32_xy( orientmin, orientmax );
      }

      int32_xy SobelAbs( tIntPlane32 const& hor ,
                         tIntPlane32 const& ver,
                         tIntPlane32& abs_value_img ) {

         int32_t absmin = INT32_MAX;
         int32_t absmax = INT32_MIN;

         auto hor_ybegin = hor.begin();
         auto hor_yend   = hor.end();
         auto ver_ybegin = ver.begin();
         auto abs_ybegin = abs_value_img.begin();


         while( hor_ybegin != hor_yend ) {

            auto hor_xbegin = hor_ybegin->begin();
            auto hor_xend   = hor_ybegin->end();
            auto ver_xbegin = ver_ybegin->begin();

            auto abs_xbegin = abs_ybegin->begin();

            while( hor_xbegin != hor_xend ) {
               //int32_t valhor = *hor_xbegin;
               //int32_t valver = *ver_xbegin;;

               //not exact
               *abs_xbegin = int_abs( *hor_xbegin ) + int_abs( *ver_xbegin );

               // exact
               //static_cast<uint32_t>( sqrt( ( double )( d ) ) );

               if( *abs_xbegin < absmin ) {
                  absmin = *abs_xbegin;
               }

               if( *abs_xbegin > absmax ) {
                  absmax = *abs_xbegin;
               }

               ++hor_xbegin;
               ++ver_xbegin;
               ++abs_xbegin;

            }

            ++hor_ybegin;
            ++ver_ybegin;
            ++abs_ybegin;
         }

         return int32_xy( absmin, absmax );
      }

      //  --  sobel operator
      void rimginterface::Sobel( tImgViewPlanar const& source, tImgPlanar& abs_value_img, tImgPlanar& orientation_img ) {
         assert( source.is_mono8() );



         tImgPlanar temp = tImgPlanar::create( source.size(), source.mask() );

         ExtendImage( source, temp, 3 );


         tTimer t;
         tIntPlane32 mver( source.size().y(),  tIntLine32( source.size().x() ) );
         tIntPlane32 mhor( source.size().y(),  tIntLine32( source.size().x() ) );
         int32_xy xy;

         auto mbegin = temp.begin();
         auto mend = temp.end();

         auto abs_mbegin = abs_value_img.begin();




         while( mbegin  != mend ) {
            if( temp.has_bit_at( mbegin ) ) {

               // 0.30971699999999996 mit ctor only
               // 0.142899  ohne abs und orient
               //double seconds = t.seconds(); //  0.265113, 0.298705 mit resize, 0.294815 mit assign
               // .224331 sec mit iterators
               // .253702 sec mit xy index
               // 0.224831
               // 0.806218 sec xy home PC

               // 0.162067 mit sse ivy i7 und xy
               // 0.138780 mit sse ivy i7 und iterators
               // 10x = 1.476295 sec
               xy = SobelHorizontal( mbegin->plane, mhor );
               xy = SobelVertical( mbegin->plane, mver );

               /* -- calculate the sobel values  */

            }

            ++mbegin;
            ++abs_mbegin;
         }

         /* -- calculate the sobel values  */
         if( abs_value_img.has_data() ) {
            tIntPlane32 abs_value( source.size().y(),  tIntLine32( source.size().x() ) );
            int32_xy absminmax = SobelAbs( mhor, mver, abs_value );
            FitToImgPlanar( abs_value, abs_value_img, absminmax );
         }

         if( orientation_img.has_data() ) {
            tIntPlane32 orientation( source.size().y(),  tIntLine32( source.size().x() ) );
            int32_xy orientationminmax = SobelOrientation( mhor, mver, orientation );
            FitToImgPlanar( orientation, orientation_img, orientationminmax );
         }

      }

   } // end of namespace
}






//EOF



