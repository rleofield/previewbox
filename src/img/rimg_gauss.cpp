
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

/*
Workfile: rimg_gauss.cpp
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

   namespace rimg_math {




      /*  --  gaussian 3x3 convolution
            */
      void rimginterface::Gauss3( tImgPlanar& img ) {
         assert( img.is_mono8() );

         uint32_t sx = img.sx();
         uint32_t sy = img.sy();
         uint32_t nsx = sx + 2;
         uint32_t nsy = sy + 2;

         tPlane16 newf( nsy, tLine16( nsx ) );
         tPlane16 news( nsy, tLine16( nsx ) );

         vector<uint16_t> newfirst( nsx * nsy );
         vector<uint16_t> newsecond( nsx * nsy );

         uint16_t* first_im  = &newfirst[0];
         uint16_t* second_im = &newsecond[0];


         /* -- copy image to extended first im
              */
         uint16_t* bword = first_im + sx + 3;
         uint32_t ycount = sy;
         uint32_t ix, iy;



         for( iy = 0; iy < sy; iy++ ) {
            for( ix = 0; ix < sx; ix++ ) {
               *bword++ = img[0][iy][ix];
            }

            bword += 2;
         }


         /* -- the image is in first
                 -- copy second column to first column
              */
         uint16_t* ptr_source = first_im + nsx + 1;
         uint16_t* ptr_target = first_im + nsx;
         ycount = sy;

         while( ycount-- ) {
            *ptr_target =   *ptr_source;
            ptr_target += nsx;
            ptr_source += nsx;
         }

         /* -- copy last-1 column to last column
              */
         ptr_source = first_im + 2 * nsx - 2;
         ptr_target = first_im + 2 * nsx - 1;
         ycount = sy;

         while( ycount-- ) {
            *ptr_target =   *ptr_source;
            ptr_target += nsx;
            ptr_source += nsx;
         }

         /* -- convolve horizontal
              */
         ptr_source = first_im  + nsx + 1;
         ptr_target = second_im + nsx + 1;

         /* -- the rows    */
         ycount = sy;

         while( ycount-- ) {
            uint32_t xcount = sx;

            while( xcount-- ) {
               *ptr_target++ = static_cast<uint16_t>( *( ptr_source - 1 )            // 1
                                             + ( ( *ptr_source ) << 1 )       // 2
                                             +    * ( ptr_source + 1 ) );       // 1
               ptr_source++;
            }

            ptr_target += 2;
            ptr_source += 2;
         }




         /* -- now is the image in second   */
         /* -- copy second row to first row in second  */
         ptr_target = second_im       + 1;
         ptr_source = second_im + nsx + 1;
         memcpy( ptr_target, ptr_source, sx * sizeof( uint16_t ) );

         /* -- copy last-1 row to last row in second     */
         ptr_target = second_im + ( sy + 1 ) * ( nsx ) + 1;
         ptr_source = second_im + ( sy ) * ( nsx ) + 1;
         memcpy( ptr_target, ptr_source, sx * sizeof( uint16_t ) );


         /* --  convolve vertikal
              */
         ptr_target = first_im  + nsx + 1;
         ptr_source = second_im + nsx + 1;

         /* -- the rows   */
         ycount = sy;

         while( ycount-- ) {
            /* -- the cols         */
            auto xcount = sx;

            while( xcount-- ) {
               *ptr_target = static_cast<uint16_t>( (
                                *( ptr_source - nsx )  // 1
                                + ( ( *ptr_source ) << 1 ) // 2
                                +   * ( ptr_source + nsx ) // 1
                             ));
               ptr_target++;
               ptr_source++;
            }

            ptr_target += 2;
            ptr_source += 2;
         }

         /* -- now is the image again in first  */
         /* -- end convolve 3 ker gauss         */


         /* -- copy back to image and shift     */

         int shift = 4;        /* -- 16 bit  */
         //b     = Mutable(Buf());
         bword  = first_im + nsx + 1;
         ycount = sy;

         auto ybegin = img.begin()->begin();

         for( iy = 0; iy < sy; iy++ ) {
            auto xbegin = ybegin->begin();

            for( ix = 0; ix < sx; ix++ ) {
               *xbegin = static_cast<uint8_t>(( ( *bword ) >> shift ));
               bword++;
               ++xbegin;
            }

            bword += 2;
            ++ybegin;
         }

         /* -- result is in image  */

      }

      void rimginterface::Gauss31( tImgPlanar& img ) {
         assert( img.is_mono8() );

         auto sx = img.sx();
         size_t sy = img.sy();
         size_t nsx = sx + 2;
         size_t nsy = sy + 2;


         vector<uint16_t> newfirst( nsx * nsy );
         vector<uint16_t> newsecond( nsx * nsy );

         uint16_t* first_im  = &newfirst[0];
         uint16_t* second_im = &newsecond[0];


         /* -- copy image to extended first im
              */
         uint16_t* bword = first_im + sx + 3;
         size_t ycount = sy;
         size_t ix, iy;

         auto ybegin = img.begin()->begin();

         for( iy = 0; iy < sy; iy++ ) {
            auto xbegin = ybegin->begin();

            for( ix = 0; ix < sx; ix++ ) {
               *bword++ = *xbegin;
               ++xbegin;
            }

            bword += 2;
            ++ybegin;
         }


         /* -- the image is in first
                 -- copy second column to first column
              */
         uint16_t* ptr_source = first_im + nsx + 1;
         uint16_t* ptr_target = first_im + nsx;
         ycount = sy;

         while( ycount-- ) {
            *ptr_target =   *ptr_source;
            ptr_target += nsx;
            ptr_source += nsx;
         }

         /* -- copy last-1 column to last column
              */
         ptr_source = first_im + 2 * nsx - 2;
         ptr_target = first_im + 2 * nsx - 1;
         ycount = sy;

         while( ycount-- ) {
            *ptr_target =   *ptr_source;
            ptr_target += nsx;
            ptr_source += nsx;
         }

         /* -- convolve horizontal
              */
         ptr_source = first_im  + nsx + 1;
         ptr_target = second_im + nsx + 1;

         /* -- the rows    */
         ycount = sy;

         while( ycount-- ) {
            int xcount = sx;

            while( xcount-- ) {
               *ptr_target++ = static_cast<uint16_t>(( *( ptr_source - 1 )            // 1
                                             + ( ( *ptr_source ) << 1 )       // 2
                                             +    * ( ptr_source + 1 ) ));       // 1
               ptr_source++;
            }

            ptr_target += 2;
            ptr_source += 2;
         }




         /* -- now is the image in second   */
         /* -- copy second row to first row in second  */
         ptr_target = second_im       + 1;
         ptr_source = second_im + nsx + 1;
         memcpy( ptr_target, ptr_source, sx * sizeof( uint16_t ) );

         /* -- copy last-1 row to last row in second     */
         ptr_target = second_im + ( sy + 1 ) * ( nsx ) + 1;
         ptr_source = second_im + ( sy ) * ( nsx ) + 1;
         memcpy( ptr_target, ptr_source, sx * sizeof( uint16_t ) );


         /* --  convolve vertikal
              */
         ptr_target = first_im  + nsx + 1;
         ptr_source = second_im + nsx + 1;

         /* -- the rows   */
         ycount = sy;

         while( ycount-- ) {
            /* -- the cols         */
            int xcount = sx;

            while( xcount-- ) {
               *ptr_target = static_cast<uint16_t>((
                                *( ptr_source - nsx )  // 1
                                + ( ( *ptr_source ) << 1 ) // 2
                                +   * ( ptr_source + nsx ) // 1
                             ) );
               ptr_target++;
               ptr_source++;
            }

            ptr_target += 2;
            ptr_source += 2;
         }

         /* -- now is the image again in first  */
         /* -- end convolve 3 ker gauss         */


         /* -- copy back to image and shift     */

         int shift = 4;        /* -- 16 bit  */
         //b     = Mutable(Buf());
         bword  = first_im + nsx + 1;
         ycount = sy;

         for( iy = 0; iy < sy; iy++ ) {
            for( ix = 0; ix < sx; ix++ ) {
               img[0][ iy ][ix] = static_cast<uint8_t>(( ( *bword ) >> shift ));
               bword++;
            }

            bword += 2;
         }

         /* -- result is in image  */

      }

      /*  --  gaussian convolution any odd kernel

                    Notes:   the larger kernels are done by repeating the 3*3 conv
                             conv is done with 32 bit precision
            */
      void rimginterface::Gauss( tImgPlanar& img, int ker_size ) {
         assert( img.is_mono8() );

         /* -- test for odd kernel_size
              */
         ker_size = ToOddKernelSize( ker_size );

         if( ker_size == 3 ) {
            Gauss3( img );
            return;
         }

         int sx  = img.sx();
         int sy  = img.sy();
         int nsx = sx + 2;
         int nsy = sy + 2;

         vector<uint32_t> newfirst( nsx * nsy );
         vector<uint32_t> newsecond( nsx * nsy );

         uint32_t* first_im  = &newfirst[0];
         uint32_t* second_im = &newsecond[0];

         /*
              32 bit
              kernel   times  sum half kernel         sum kernel        shift after convolution
                                               bits               bits
              3*3       1        4 = 4*1       2             16    4      4
              5*5       2       16 = 4*4       4            256    8      8
              7*7       3       64 = 4*16      6           4096   12     12
              9*9       4      256 = 4*64      8          65536   16     16
              11*11     5     1024 = 4*256    10        1048576   20     20   internal shift
              13*13     6     4096 = 4*1024   12       16777216   24     20 + 1*4 = 24
              15*15     7    16384 = 4*4048   14                         20 + 2*4 = 28
              17*17     8    65536 = 4*16384  16                         20 + 3*4 = 32
              19*19     9   262144 = 4*65536  18                         20 + 4*4 = 36
              21*21    10  1048576 = 4*262144 20                         20 + 5*4 = 40

                 max_value after conv = 32 bit = sum kernel * 256 = sum kernel << 8

              */

         int times = ( ker_size - 1 ) / 2; /* -- times repeating the 3*3     */

         uint32_t shift = times * 4;      /* -- 32 bit  */

         if( shift > 20 ) {
            shift = 20;
         }



         /* -- copy image to extended first im  */
         uint32_t* bdword = first_im + nsx + 1;
         int ix, iy;

         for( iy = 0; iy < sy; iy++ ) {
            for( ix = 0; ix < sx; ix++ ) {
               *bdword = static_cast<uint32_t>(img[0][ iy ][ix]);
               bdword++;
            }

            bdword += 2;
         }

         // convolve image in 32 bit space 'times' times

         for( int i = 0; i < times; i++ ) {
            int shift_internal = 0;

            if( i > 4 )    /* -- 32 bit  */  // from times = 6
            {
               shift_internal = 4;
            } else {
               shift_internal = 0;
            }


            /* -- the image is in first
                   -- copy second column to first column
                */
            uint32_t* ptr_source = first_im + nsx + 1;
            uint32_t* ptr_target = first_im + nsx;
            uint32_t ycount = sy;

            while( ycount-- ) {
               *ptr_target = *ptr_source;
               ptr_target += nsx;
               ptr_source += nsx;
            }

            /* -- copy last-1 column to last column
                */
            ptr_source = first_im + 2 * nsx - 2;
            ptr_target = first_im + 2 * nsx - 1;
            ycount = sy;

            while( ycount-- ) {
               *ptr_target =   *ptr_source;
               ptr_target += nsx;
               ptr_source += nsx;
            }


            /* -- convolve horizontal
                */
            ptr_source = first_im  + nsx + 1;
            ptr_target = second_im + nsx + 1;

            /* -- the rows    */
            ycount = sy;

            while( ycount-- ) {
               int xcount = sx;

               while( xcount-- ) {

                  *ptr_target++ = static_cast<uint32_t>(( *( ptr_source - 1 )    // 1
                                                + ( ( unsigned long ) * ptr_source << 1 ) // 2
                                                +   * ( ptr_source + 1 ) )); // 1
                  ptr_source++;
               }

               ptr_target += 2;
               ptr_source += 2;
            }


            /* -- now is the image in second  */


            /* -- copy second row to first row in second  */
            ptr_target = second_im       + 1;
            ptr_source = second_im + nsx + 1;
            memcpy( ptr_target, ptr_source, sx * sizeof( uint32_t ) );

            /* -- copy last-1 row to last row in second     */
            ptr_target = second_im + ( sy + 1 ) * ( nsx ) + 1;
            ptr_source = second_im + ( sy ) * ( nsx ) + 1;
            memcpy( ptr_target, ptr_source, sx * sizeof( uint32_t ) );


            /* --  convolve vertikal
                */
            ptr_target = first_im  + nsx + 1;
            ptr_source = second_im + nsx + 1;

            /* -- the rows   */
            ycount = sy;

            while( ycount-- ) {
               /* -- the cols         */
               int xcount = sx;

               while( xcount-- ) {
                  *ptr_target = static_cast<uint32_t>((
                                   *( ptr_source - nsx )   // 1
                                   + ( ( unsigned long )( *ptr_source ) << 1 )  // 2
                                   +  * ( ptr_source + nsx )  // 1
                                ));

                  // shift intern to avoid overflow
                  if( shift_internal != 0 ) {
                     *ptr_target = ( unsigned long ) * ptr_target >> shift_internal;
                  }

                  ptr_target++;
                  ptr_source++;
               }

               ptr_target += 2;
               ptr_source += 2;
            }

            /* -- now is the image again in first  */
         }

         /* --  end convolve 3 ker gauss  */


         /* -- copy back to image and shift last time */

         bdword = first_im + nsx + 1;

         for( iy = 0; iy < sy; iy++ ) {
            for( ix = 0; ix < sx; ix++ ) {
               img[0][ iy ][ix] = static_cast<uint8_t>(( ( unsigned long )( ( *bdword ) >> shift ) ));
               bdword++;
            }

            bdword += 2;
         }


         /* -- result is in image  */

      }






   } // end of namespace

}






//EOF



