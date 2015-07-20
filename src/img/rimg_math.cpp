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

/*! \file rimg_math.cpp
\brief impl for linear images


*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>

#include "t_2D_xy.h"
#include "t_3D_xyz.h"
#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rimg_math.h"
//#include "rimg_read_write.h"
#include "rimg_planar.h"

using namespace std;



namespace rlf {


   namespace rimg_math {

      double_xyz rimginterface::MaximumInArea( tImgPlanar const& im, tAoi area ) {
         assert( im.is_mono8() );
         int iy = area.sy();
         const int x =  area.px();
         const int y =  area.py();
         int max = 0;

         while( iy-- ) {
            int ix = area.sx();

            while( ix-- ) {

               int val = im[0][ y + iy ][ x + ix];

               if( val > max ) {
                  max = val;
               }
            }

         }

         vector<int32_xy> newxy( area.sx()*area.sy() + 1 );
         // tixy *xy = newxy();

         uint32_t maxcount = 0;
         iy = area.sy();

         while( iy-- ) {
            int ix = area.sx();

            while( ix-- ) {
               int val = im.pixel( uint32_xy( x + ix, y + iy ) );

               if( val == max ) {
                  newxy[ maxcount ].setx( x + ix );
                  newxy[ maxcount ].sety( y + iy );
                  maxcount++;
               }
            }
         }

         if( maxcount == 0 ) {
            assert( false );
         }


         double dx = 0.0;
         double dy = 0.0;
         uint32_t count = maxcount;

         while( count ) {
            count--;
            dx += static_cast<double>(newxy[ count ].x());
            dy += static_cast<double>(newxy[ count ].y());
         }

         dx /= static_cast<double>(maxcount);
         dy /= static_cast<double>(maxcount);
         return double_xyz( dx, dy, max );
      }

      size_t rimginterface::max_x( tImgPlanar const& im ) {
         assert( im.is_mono8() );
         size_t maxx = 0;

         auto begin = im.mono8().begin();
         auto end = im.mono8().end();

         while( begin != end ) {
            auto xbegin = begin->begin();
            auto xend = begin->end();
            size_t ix = 0;

            while( xbegin != xend ) {
               if( *xbegin > maxx ) {
                  maxx = ix;
               }

               ix++;
               ++xbegin;
            }

            ++begin;
         }

         return maxx;
      }

      // min x of all objeckts in bin img
      size_t rimginterface::min_x( tImgPlanar const& im ) {
         size_t x = im.sx();
         size_t y = im.sy();
         size_t minx = uint8_t( -1 );

         if( im.is_mono8() ) {
            size_t iy = 0;

            while( y-- ) {
               for( size_t ix = 0; ix < x; ix++ ) {
                  if( im[0][ iy ][ix] )
                     if( ix < minx ) {
                        minx = ix;
                     }
               }

               iy++;
            }
         }

         return minx;
      }

      // max y of all objeckts in bin img
      size_t rimginterface::max_y( tImgPlanar const& im )  {
         size_t x = im.sx();
         size_t y = im.sy();
         size_t maxy = 0;

         if( im.is_mono8() ) {
            for( size_t iy = 0; iy < y; iy++ ) {
               for( size_t ix = 0; ix < x; ix++ ) {
                  if( im[0][ iy ][ix] )
                     if( iy > maxy ) {
                        maxy = iy;
                     }
               }
            }
         }

         return maxy;
      }


      size_t rimginterface::min_y( tImgPlanar const& im ) {
         size_t x = im.sx();
         size_t y = im.sy();
         size_t miny = y;

         if( im.is_mono8() ) {
            for( size_t iy = 0; iy < y; iy++ ) {
               for( size_t ix = 0; ix < x; ix++ ) {
                  if( im[0][ iy ][ix] ) {
                     miny = iy;
                     ix = x;
                     iy = y;
                  }
               }
            }
         }

         return miny;
      }


      namespace insert {
         void Insert( const tPlane8& in, tPlane8& out, int32_xy pos, uint32_xy size ) {

            auto yinbegin = in.begin();
            auto yinend = in.begin() + size.y();
            auto youtbegin = out.begin() + pos.y();

            while( yinbegin != yinend ) {
               copy( yinbegin->begin(), yinbegin->begin() + size.x(), youtbegin->begin() + pos.x() );
               ++yinbegin;
               ++youtbegin;
            }

         }
      }

      bool rimginterface::Insert( const tImgViewPlanar& aoi, tImgPlanar& target, int32_xy xy ) {
         assert( target.equals_size_mask( aoi ) );

         // test if target position is in img
         if( xy.x() > static_cast<int>(target.sx()) - 1 || xy.y() > static_cast<int>(target.sy()) - 1 ) {
            return false;
         }

         uint32_xy xxyy( xy.x(), xy.y() );
         uint32_xy aoi_ = aoi.size();
         uint32_xy result =  aoi_ + xxyy;
         int32_xy aoimax = int32_xy( result.x(), result.y() );

         if( aoimax.x()  <= 0 && aoimax.y() <= 0 ) {
            return false;
         }

         if( aoimax.x()  > static_cast<int>(target.sx()) && aoimax.y() > static_cast<int>(target.sy()) ) {
            return false;
         }

         uint32_xy saoi = aoi.size();
         uint32_xy so = target.size();

         auto maoibegin = aoi.begin();
         auto maoiend = aoi.end();
         auto out = target.begin();

         uint32_xy size( aoi.size() );

         while( maoibegin != maoiend ) {
            insert::Insert( maoibegin->plane, out->plane, xy, size );
            ++maoibegin;
            ++out;
         }

         return true;
      }

      bool rimginterface::ExtractAoi( tImgViewPlanar const& source, tImgPlanar& target, tAoi& aoi ) {
         if( target.equals_size_mask( source ) ) {
            //target = source.to_ImgViewPlanar();
            return true;
         }

         // test if aoi is in img
         if( aoi.px() >= static_cast<int>(source.sx()) || aoi.py() >= static_cast<int>(source.sy()) ) {
            return false;
         }

         // -- calculate the true size of copied image
         if( aoi.px() + aoi.sx()  > source.sx() ) {
            aoi.sx( source.sx() - aoi.px() );
         }

         if( aoi.py() + aoi.sy()  > source.sy() ) {
            aoi.sy( source.sy() - aoi.py() );
         }

         if( aoi.sx() <= 1 || aoi.sy() <= 1 ) {
            return false;
         }

         tSize size = tSize( aoi.sx(), aoi.sy() ); // img.size();
         target.realloc( size, source.mask() );

         auto mbegin =  source.begin();
         auto mend =  source.end();
         auto maoibegin = target.begin();

         while( mbegin < mend ) {

            vector< vector<uint8_t> >::const_iterator ybegin = mbegin->begin() +  aoi.py();

            vector< vector<uint8_t> >::iterator yaoibegin = maoibegin->begin();
            vector< vector<uint8_t> >::const_iterator yaoiend = maoibegin->end();

            int c = 0;

            while( yaoibegin != yaoiend ) {
               yaoibegin->assign( ybegin->begin() + aoi.px(),  ybegin->begin() + aoi.px() + aoi.sx() );
               ++yaoibegin;
               ++ybegin;
               c++;

            }

            ++mbegin;
            ++maoibegin;


         }

         return true;
      }


      namespace subtract {
         void Subtract( tPlane8& target, const tPlane8& second, int const1 ) {

            auto ybegin = second.begin();
            auto yend = second.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto target_xbegin = target_ybegin->begin();

               while( xbegin != xend ) {

                  int s = *target_xbegin - *xbegin + const1;

                  if( s > 0xff ) {
                     s = 0xff;
                  }

                  if( s < 0 ) {
                     s = 0;
                  }

                  *target_xbegin = static_cast<uint8_t>(s);


                  ++xbegin;
                  ++target_xbegin;
               }

               ++ybegin;
               ++target_ybegin;
            }

         }
      }

      void rimginterface::Subtract( tImgPlanar& target, const tImgViewPlanar& second, int const1 ) {
         tImgViewPlanar const& source = second;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty

         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            subtract::Subtract( target_mbegin->plane, mbegin->plane, const1 );
            ++mbegin;
            ++target_mbegin;
         }

      }



      namespace nsadd {

         void Add( tPlane8& source, int const1 ) {


            auto ybegin = source.begin();
            auto yend = source.end();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();

               while( xbegin != xend ) {

                  int s = *xbegin + const1;

                  if( s > 0xff ) {
                     s = 0xff;
                  }

                  if( s < 0 ) {
                     s = 0;
                  }

                  *xbegin = static_cast<uint8_t>(s);


                  ++xbegin;
               }

               ++ybegin;
            }

         }

         void Add( tPlane8& sum,   const tPlane8& summand,  int constant ) {
            auto ybegin = summand.begin();
            auto yend = summand.end();
            auto target_ybegin = sum.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto target_xbegin = target_ybegin->begin();

               while( xbegin != xend ) {

                  int s = *xbegin  + *target_xbegin + constant;

                  if( s > 255 ) {
                     s = 0xFF;
                  }

                  if( s < 0 ) {
                     s = 0;
                  }

                  *target_xbegin  = static_cast<uint8_t>(s);

                  ++xbegin;
                  ++target_xbegin;
               }

               ++ybegin;
               ++target_ybegin;
            }

         }
      }
      // add a am img and a constant
      void rimginterface::Add( tImgPlanar& sum,   const tImgViewPlanar& summand,  int constant ) {
         tImgViewPlanar const& source = summand;
         tImgPlanar& target = sum;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty

         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            nsadd::Add( target_mbegin->plane, mbegin->plane, constant );
            ++mbegin;
            ++target_mbegin;
         }


      }

      // add a constant
      void Add( tImgPlanar& source, int const1 ) {

         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            nsadd::Add( mbegin->plane, const1 );
            ++mbegin;
         }

      }


   } // end of ns math
} //end of ns rlf






//EOF



