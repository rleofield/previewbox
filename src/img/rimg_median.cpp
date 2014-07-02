
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

/*! \file rimg_median.cpp
\brief median and rank


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
#include "rlf_math.h"
#include "rimg_plane_linear.h"

#include "timer.h"
#include "str_util.h"

#ifdef _WIN32
#pragma warning( disable:4267) // possible loss of data ( size_t nach int )
#endif
using namespace std;




namespace rlf {


   namespace rimg_math {


      void getkernel( vector<size_t> & arr, tPlaneLinear8 const& extended, uint32_xy xy, int kernel ) {

         int hk = kernel / 2;
				 uint32_t xcount = xy.x() - hk;
				 uint32_t xcountmax = xy.x() + hk;

         vector<size_t>::iterator parr = arr.begin();

         while( xcount <= xcountmax ) {


            int ycount  = xy.y() - hk;
            int ycountmax  = xy.y() + hk;

						int y = ycount * extended.pitch() + xcount;

            while( ycount <= ycountmax ) {
               *parr++ = extended[ y ];
               y++;
            }

            xcount++;
         }
      }

      void kernel3( vector<size_t> & arr, vector<uint8_t> const& bv, int sx ) {
         uint8_t const* b = &( bv[0] );
         b -= sx;
         int count( 3 );
         size_t* parr = &( arr[0] );

         while( count-- ) {
            *parr++ =   *( b - 1 );
            *parr++ =   *( b );
            *parr++ =   *( b + 1 );
            b += sx;
         }
      }

      void kernel3_( int* parr, uint8_t const* b, int sx ) {
         b -= sx;
         int count( 3 );

         while( count-- ) {
            *parr++ =   *( b - 1 );
            *parr++ =   *( b );
            *parr++ =   *( b + 1 );
            b += sx;
         }
      }

      void kernel5_( int* parr, uint8_t* b, int nx ) {
         *parr++ =   *( b - 2 * nx - 2 );
         *parr++ =   *( b - 2 * nx - 1 );
         *parr++ =   *( b - 2 * nx );
         *parr++ =   *( b - 2 * nx + 1 );
         *parr++ =   *( b - 2 * nx + 2 );
         *parr++ =   *( b -   nx - 2 );
         *parr++ =   *( b -   nx - 1 );
         *parr++ =   *( b -   nx );
         *parr++ =   *( b -   nx + 1 );
         *parr++ =   *( b -   nx + 2 );
         *parr++ =   *( b        - 2 );
         *parr++ =   *( b        - 1 );
         *parr++ =   *( b );
         *parr++ =   *( b      + 1 );
         *parr++ =   *( b      + 2 );
         *parr++ =   *( b +   nx - 2 );
         *parr++ =   *( b +   nx - 1 );
         *parr++ =   *( b +   nx );
         *parr++ =   *( b +   nx + 1 );
         *parr++ =   *( b +   nx + 2 );
         *parr++ =   *( b + 2 * nx - 2 );
         *parr++ =   *( b + 2 * nx - 1 );
         *parr++ =   *( b + 2 * nx );
         *parr++ =   *( b + 2 * nx + 1 );
         *parr++ =   *( b + 2 * nx + 2 );
      }


      void kernel7_( int* parr, uint8_t* b, int nx ) {
         uint8_t* pb = b - 3 * nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
         pb += nx;
         *parr++ =   *( pb - 3 );
         *parr++ =   *( pb - 2 );
         *parr++ =   *( pb - 1 );
         *parr++ =   *( pb );
         *parr++ =   *( pb + 1 );
         *parr++ =   *( pb + 2 );
         *parr++ =   *( pb + 3 );
      }



      // kernel is already odd                                        // pos in extimg
      inline void getkernel_( tPlane8 const& im, vector<size_t> &arr, uint32_xy xy, int kernel ) {
         //int s = arr.size();
         assert( arr.size() == ( uint32_t )( kernel * kernel ) );

         int x = xy.x();
         int y = xy.y();

         int ycount  = kernel;
         int iy = y - kernel / 2;
         int index = 0;

         while( ycount-- ) {
            int xcount = kernel;
            int ix = x - kernel / 2;

            while( xcount-- ) {
               arr[index] = im[iy][ix];
               index++;
               ix++;
            }

            iy++;
         }
      }



      inline void kernel3( tPlane8 const& im, vector<size_t> &arr, uint32_xy xy ) {
         getkernel_( im, arr, xy, 3 );
      }
      inline void kernel5( tPlane8 const& im, vector<size_t> &arr, uint32_xy xy ) {
         getkernel_( im, arr, xy, 5 );
      }
      inline void kernel7( tPlane8 const& im, vector<size_t> &arr, uint32_xy xy ) {
         getkernel_( im, arr, xy, 7 );
      }


      void Median3( tPlane8& im ) {

         uint32_t halfkernel = 1;

         int y = im.size();
         int x = im[0].size();
         int x2 = x + 2;
         int y2 = y + 2;

         tPlane8 extended( x2, vLine8( y2 ) );
         ToExtendedImageArea( im, extended, halfkernel );

         size_t nr_kernel_pixels     = 3 * 3;
         size_t nr_kernel_pixels_half     = nr_kernel_pixels / 2;

         vector<size_t> kernel_pixels( nr_kernel_pixels );

         tPlane8& target = im;


         auto ybegin_m1 = extended.begin(); // [iy-1]
         auto ybegin    = ybegin_m1 + 1; // [iy]
         auto ybegin_p1 = ybegin_m1 + 2; // [iy+1]

         auto yend = extended.end() - 1;


         auto target_ybegin    = target.begin();


         while( ybegin != yend ) {

            auto xbegin_ym1 = ybegin_m1->begin() + 1; // [iy-1][ix]
            auto xbegin_y   = ybegin->begin() + 1;    // [iy][ix]
            auto xbegin_yp1 = ybegin_p1->begin() + 1; // [iy+1][ix]
            auto xend       = ybegin->end() - 1;

            auto target_xbegin    = target_ybegin->begin();

            while( xbegin_y != xend ) {

               // [y-1][x-1]
               int  ym1_xm1 = *( xbegin_ym1 - 1 );
               // [y-1][x]
               int  ym1_xm0   = *( xbegin_ym1 );
               // [y-1][x+1]
               int  ym1_xp1 = *( xbegin_ym1 + 1 );

               // [y-0][x-1]
               int  ym0_xm1 = *( xbegin_y   - 1 );
               // [y-0][x]
               int  ym0_xm0   = *( xbegin_y );
               // [y-0][x+1]
               int  ym0_xp1 = *( xbegin_y + 1 );

               // [y+1][x-1]
               int  yp1_xm1 = *( xbegin_yp1 - 1 );
               // [y+1][x]
               int  yp1_xm0   = *( xbegin_yp1 );
               // [y+1][x+1]
               int  yp1_xp1 = *( xbegin_yp1 + 1 );

               kernel_pixels[0] = ym1_xm1;
               kernel_pixels[1] = ym0_xm1;
               kernel_pixels[2] = yp1_xm1;
               kernel_pixels[3] = ym1_xm0;
               kernel_pixels[4] = ym0_xm0;
               kernel_pixels[5] = yp1_xm0;
               kernel_pixels[6] = ym1_xp1;
               kernel_pixels[7] = ym0_xp1;
               kernel_pixels[8] = yp1_xp1;


               math::shell_sort( kernel_pixels, nr_kernel_pixels );
               *target_xbegin = ( uint8_t )kernel_pixels[ nr_kernel_pixels_half ];

               ++xbegin_ym1;
               ++xbegin_y;
               ++xbegin_yp1;
               ++target_xbegin;
            }

            ++ybegin_m1;
            ++ybegin;
            ++ybegin_p1;
            ++target_ybegin;
         }




      }

      void Median5( tPlane8& im ) {



         uint32_t halfkernel = 2;


         int y = im.size();
         int x = im[0].size();
         int x2 = x + halfkernel * 2;
         int y2 = y + halfkernel * 2;

         tPlane8 extended( x2, vLine8( y2 ) );
         ToExtendedImageArea( im, extended, halfkernel );

         size_t nr_kernel_pixels     = 5 * 5;
         size_t nr_kernel_pixels_half     = nr_kernel_pixels / 2;

         vector<size_t> kernel_pixels( nr_kernel_pixels );

         tPlane8& target = im;



         auto ybegin_m2 = extended.begin(); // [iy-2]
         auto ybegin_m1 = ybegin_m2 + 1; // [iy-1]
         auto ybegin_m0 = ybegin_m1 + 1; // [iy]
         auto ybegin_p1 = ybegin_m1 + 2; // [iy+1]
         auto ybegin_p2 = ybegin_m1 + 3; // [iy+2]

         auto yend = extended.end() - 1;


         auto target_ybegin    = target.begin();

         while( ybegin_m0 != yend ) {
            auto xbegin_ym2 = ybegin_m2->begin() + 2; // [iy-2][ix]
            auto xbegin_ym1 = ybegin_m1->begin() + 2; // [iy-1][ix]
            auto xbegin_ym0 = ybegin_m0->begin() + 2; // [iy]  [ix]
            auto xbegin_yp1 = ybegin_p1->begin() + 2; // [iy+1][ix]
            auto xbegin_yp2 = ybegin_p2->begin() + 2; // [iy+2][ix]
            auto xend       = ybegin_m0->end() - 2;

            auto target_xbegin    = target_ybegin->begin();

            while( xbegin_ym0 != xend ) {


               // [y-2][x-2]
               kernel_pixels[ 0] = *( xbegin_ym2 - 2 );
               // [y-2][x-1]
               kernel_pixels[ 1] = *( xbegin_ym2 - 1 );
               // [y-2][x]
               int  ym2_xm0   = *( xbegin_ym2 );
               // [y-2][x+1]
               int  ym2_xp1 = *( xbegin_ym2 + 1 );
               // [y-2][x+1]
               int  ym2_xp2 = *( xbegin_ym2 + 2 );


               // [y-1][x-2]
               int  ym1_xm2 = *( xbegin_ym1 - 2 );
               // [y-1][x-1]
               int  ym1_xm1 = *( xbegin_ym1 - 1 );
               // [y-1][x]
               int  ym1_xm0   = *( xbegin_ym1 );
               // [y-1][x+1]
               int  ym1_xp1 = *( xbegin_ym1 + 1 );
               // [y-1][x+1]
               int  ym1_xp2 = *( xbegin_ym1 + 2 );


               // [y][x-2]
               int  ym0_xm2 = *( xbegin_ym0 - 2 );
               // [y][x-1]
               int  ym0_xm1 = *( xbegin_ym0 - 1 );
               // [y][x]
               int  ym0_xm0 = *( xbegin_ym0 );
               // [y][x+1]
               int  ym0_xp1 = *( xbegin_ym0 + 1 );
               // [y][x+1]
               int  ym0_xp2 = *( xbegin_ym0 + 2 );


               // [y+1][x-2]
               int  yp1_xm2 = *( xbegin_yp1 - 2 );
               // [y+1][x-1]
               int  yp1_xm1 = *( xbegin_yp1 - 1 );
               // [y+1][x]
               int  yp1_xm0   = *( xbegin_yp1 );
               // [y+1][x+1]
               int  yp1_xp1 = *( xbegin_yp1 + 1 );
               // [y+1][x+1]
               int  yp1_xp2 = *( xbegin_yp1 + 2 );


               // [y+2][x-2]
               int  yp2_xm2 = *( xbegin_yp2 - 2 );
               // [y+2][x-1]
               int  yp2_xm1 = *( xbegin_yp2 - 1 );
               // [y+2][x]
               int  yp2_xm0   = *( xbegin_yp2 );
               // [y+2][x+1]
               int  yp2_xp1 = *( xbegin_yp2 + 1 );
               // [y+2][x+1]
               int  yp2_xp2 = *( xbegin_yp2 + 2 );




               kernel_pixels[ 2] = ym2_xm0;
               kernel_pixels[ 3] = ym2_xp1;
               kernel_pixels[ 4] = ym2_xp2;

               kernel_pixels[ 5] = ym1_xm2;
               kernel_pixels[ 6] = ym1_xm1;
               kernel_pixels[ 7] = ym1_xm0;
               kernel_pixels[ 8] = ym1_xp1;
               kernel_pixels[ 9] = ym1_xp2;

               kernel_pixels[10] = ym0_xm2;
               kernel_pixels[11] = ym0_xm1;
               kernel_pixels[12] = ym0_xm0;
               kernel_pixels[13] = ym0_xp1;
               kernel_pixels[14] = ym0_xp2;

               kernel_pixels[15] = yp1_xm2;
               kernel_pixels[16] = yp1_xm1;
               kernel_pixels[17] = yp1_xm0;
               kernel_pixels[18] = yp1_xp1;
               kernel_pixels[19] = yp1_xp2;

               kernel_pixels[20] = yp2_xm2;
               kernel_pixels[21] = yp2_xm1;
               kernel_pixels[22] = yp2_xm0;
               kernel_pixels[23] = yp2_xp1;
               kernel_pixels[24] = yp2_xp2;


               math::shell_sort( kernel_pixels, nr_kernel_pixels );
               *target_xbegin = ( uint8_t )kernel_pixels[ nr_kernel_pixels_half ];

               ++xbegin_ym2;
               ++xbegin_ym1;
               ++xbegin_ym0;
               ++xbegin_yp1;
               ++xbegin_yp2;
               ++target_xbegin;

            }

            ++ybegin_m2;
            ++ybegin_m1;
            ++ybegin_m0;
            ++ybegin_p1;
            ++ybegin_p2;
            ++target_ybegin;

         }
      }


      void rimginterface::Median( tImgPlanar& source, uint32_t kernel ) {

         auto mbegin = source.begin();
         auto mend = source.end();
         kernel = ToOddKernelSize( kernel );

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {
               switch( kernel ) {
               case 3:
                  Median3( mbegin->plane ) ;
                  break;
               case 5:
                  Median5( mbegin->plane ) ;
                  break;
               default:
                  break;
               }
            }

            ++mbegin;
         }

      }


      void rimginterface::Rank( tPlane8 const& im, tPlane8& min_img,
                                tPlane8& max_img, int kernel ) {

         kernel = ToOddKernelSize( kernel );

         uint32_t halfkernel = kernel >> 1;

         size_t x = im.size();
         size_t y = im[0].size();
         //size_t x2 = x + halfkernel * 2;
         //size_t y2 = y + halfkernel * 2;



         tImgPlanar temp;
         uint32_xy s( x, y );
         s += ( 2 * halfkernel );

         temp.realloc( s, tMask::mono );

         tPlaneLinear8 linear;

         rimg_math::ToExtendedLinearArea( im, linear, halfkernel );

         int area     = kernel * kernel;
         vector<size_t> arr( area );



         tPlane8 const& source = im;

         auto ybegin = source.begin();
         auto yend = source.end();

         auto min_ybegin = min_img.begin();
         auto max_ybegin = max_img.begin();

         int iy = 0;

         while( ybegin != yend ) {
            auto xbegin = ybegin->begin();
            auto xend = ybegin->end();

            auto max_xbegin = max_ybegin->begin();
            auto min_xbegin = min_ybegin->begin();

            int ix = 0;

            while( xbegin != xend ) {

               uint32_xy xy( ix, iy );
               getkernel( arr,  linear, xy, kernel );


               math::shell_sort( arr, area );
               min_img[iy][ix] = ( uint8_t )arr[ 0 ];
               max_img[iy][ix] = ( uint8_t )arr[ arr.size() - 1 ];

               ix++;
               ++xbegin;
               ++min_xbegin;
               ++max_xbegin;
            }

            iy++;
            ++ybegin;
            ++min_ybegin;
            ++max_ybegin;
         }

      }


      void rimginterface::Rank( tImgViewPlanar const& source,  tImgPlanar& min_img,  tImgPlanar& max_img, uint32_t kernel ) {

         assert( min_img.equals_size_mask( source ) );
         assert( max_img.equals_size_mask( source ) );

         auto mbegin = source.begin();
         auto mend = source.end();

         auto min_mbegin = min_img.begin();
         auto max_mbegin = max_img.begin();

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {
               Rank( mbegin->plane, min_mbegin->plane, max_mbegin->plane, kernel ) ;
            }

            ++mbegin;
            ++min_mbegin;
            ++max_mbegin;
         }

      }




   }

} // end of namespace rlf

//EOF






