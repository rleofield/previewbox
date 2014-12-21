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

/*! \file rimg_testimages.cpp
\brief impl for generating test raw images

*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>

//#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rimg_math.h"
//#include "rimg_read_write.h"
#include "rimg_planar.h"

using namespace std;




namespace rlf {


   namespace rimg_math {


      void rimginterface::fill( tImgPlanar& img, uint32_t val ) {
         auto mbegin = img.begin();

         while( mbegin < img.end() ) {
            auto ybegin = mbegin->begin();

            while( ybegin < mbegin->end() ) {
               std::fill( ybegin->begin(), ybegin->end(), val );
               ++ybegin;
            }

            ++mbegin;
         }
      }
      void rimginterface::rampx( tImgPlanar& img, uint32_t start, uint32_t end_ ) {
         auto mbegin = img.begin();

         while( mbegin < img.end() ) {
            auto ybegin = mbegin->begin();

            while( ybegin < mbegin->end() ) {
               auto xbegin = ybegin->begin();
               uint8_t v = start;

               while( xbegin != ybegin->end() ) {
                  *xbegin = v;
                  ++xbegin;

                  if( v == end_ ) {
                     v = start;
                  } else {
                     v++;
                  }
               }

               ++ybegin;
            }

            ++mbegin;
         }
      }
      void rimginterface::rampy( tImgPlanar& img, uint32_t start, uint32_t end_ ) {
         auto mbegin = img.begin();

         while( mbegin < img.end() ) {
            auto ybegin = mbegin->begin();
            uint8_t v = start;

            while( ybegin < mbegin->end() ) {
               std::fill( ybegin->begin(), ybegin->end(), v );
               ++ybegin;

               if( v == end_ ) {
                  v = start;
               } else {
                  v++;
               }
            }

            ++mbegin;
         }
      }

      void rimginterface::rampxy( tImgPlanar& img, uint32_t start, uint32_t end_ ) {
         auto mbegin = img.begin();

         while( mbegin < img.end() ) {
            auto ybegin = mbegin->begin();
            uint8_t yv = 1;

            while( ybegin < mbegin->end() ) {
               auto xbegin = ybegin->begin();
               uint8_t xv = start;

               while( xbegin != ybegin->end() ) {
                  *xbegin = yv + xv;
                  ++xbegin;

                  if( xv == end_ ) {
                     xv = start;
                  } else {
                     xv++;
                  }
               }

               ++ybegin;

               if( yv == end_ ) {
                  yv = start;
               } else {
                  yv++;
               }
            }

            ++mbegin;
         }
      }
      namespace nsrectangle {
         void rectangle( tPlane8& img,  uint32_t val, uint32_xy pos, uint32_xy size ) {

            auto ybegin = img.begin() + pos.y();
            auto yend   = img.begin() + size.y();

            while( ybegin < yend ) {
               std::fill( ybegin->begin() + pos.x(), ybegin->begin() + size.x(), val );
               ++ybegin;
            }

         }
      }

      bool rimginterface::rectangle( tImgPlanar& img,  uint32_t val, uint32_xy pos, uint32_xy size ) {

         // test if target position is in img
         if( pos.x() > img.sx() - 1 || pos.y() > img.sy() - 1 ) {
            return false;
         }

         uint32_xy aoimax = pos + size;

         if( aoimax.x()  > img.sx() && aoimax.y() > img.sy() ) {
            return false;
         }


         auto mbegin = img.begin();

         while( mbegin < img.end() ) {
            nsrectangle::rectangle( mbegin->plane, val, pos, size );
            ++mbegin;
         }

         return true;
      }

   } // end of namespace
}






//EOF



