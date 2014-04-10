
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

/*! \file rimg_rotate.cpp
\brief impl for linear images


*/


//#include <algorithm>
//#include <functional>
//#include <limits.h>
//#include <assert.h>
//#include <iostream>
//#include <math.h>

//#include "t_2D_xy.h"
//#include "rimg_imgsize_type.h"

////#include "rimg_xy_types.h"
//#include "rimg_math.h"
////#include "rimg_read_write.h"
//#include "rimg_timer.h"
//#include "str_util.h"

//using namespace std;
//using namespace rlf_xy;



namespace rlf {


   namespace rimg_math {
#ifdef ggggggg
      void Rotate_90( cImageBuffer& p ) {
         assert( p.IsBit8() );
         cImageBuffer tmp( p );

         int sSx = p.Sx();
         int sSy = p.Sy();

         p.Realloc( p.Size() );


         for( int y = 0; y < sSy; y++ ) {
            for( int x = 0; x < sSx; x++ ) {
               p.Pixel( tixy( y, x ), tmp.Pixel( tixy( x, y ) ) );
            }
         }
      }


      int RotateBinObject( cImageBuffer& target, double angle, tixy point ) {
         int xc = point.X();
         int yc = point.Y();

         cImageBuffer s( target );
         //   int x = s.Sx();
         //   int y = s.Sy();
         target.Realloc( s.Size(), cImageBuffer::TypeBit8() );

         // get color in bin img
         //int count = miX*mY;
         int c     = 0;
         Dword iy = 0;

         for( iy = 0; iy < s.Sy(); iy++ ) {
            for( Dword ix = 0; ix < s.Sx(); ix++ ) {
               if( s.Image()[iy][ix] ) {
                  c = s.Image()[iy][ix];
                  break;
               }
            }

            if( c ) {
               break;
            }
         }

         if( c == 0 ) { // no binary object in img
            return 0;
         }


         angle *=  PI / 180.0; // to radians
         double sina = sin( angle );
         double cosa = cos( angle );

         /* scan vert */
         for( iy = 0; iy < s.Sy(); iy++ ) {
            /* scan horiz */
            for( Dword ix = 0; ix < s.Sx(); ix++ ) {
               if( s.Image()[iy][ix] ) {
                  int x_rot = ( int )( ( xc + ( ix - xc ) * cosa + ( iy - yc ) * sina ) );
                  int y_rot = ( int )( ( yc + ( iy - yc ) * cosa - ( ix - xc ) * sina ) );
                  target.Image()[y_rot][x_rot] = ( Byte )c;
               }
            }
         }

         return 0;
      }
#endif

   }

} // end of namespace rlf

//EOF






