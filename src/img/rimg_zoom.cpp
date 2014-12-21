
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

/*! \file rimg_zoom.cpp
\brief impl for zoom in out images

*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>
#include <iostream>
#include <math.h>

//#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rimg_math.h"
//#include "rimg_read_write.h"
#include "rimg_planar.h"
#include "timer.h"
#include "str_util.h"

using namespace std;


#define srand48(s) srand(s)
#define drand48() (((double)rand())/((double)RAND_MAX))
#define lrand48() rand()

using rlf_minit::double_xy;

namespace rlf {


   namespace rimg_math {


      inline double interpolate( double x, double_xy const& xy0, double_xy const& xy1 ) {

         double x0 = xy0.x();
         double y0 = xy0.y();

         double dy = xy0.y() - xy1.y();
         double dx = xy0.x() - xy1.x();
         // -- and interpolate
         return  y0 + ( ( x - x0 ) / dx ) * dy ;
      }

      void linear_zoom( vLine8 const& s, vLine8&  t ) {

         double ds = 1.0 / s.size();
         double dt = 1.0 / t.size();

         for( size_t i = 0; i < t.size(); i++ ) {
            double x = i * dt;
            int x1   = ( int )( x / ds );
            int x2   = x1 + 1;

            if( x2 == ( int )s.size() ) {
               t[i] = s[x1];
            } else {
               //double_xy xy0( x1 * ds, s[x1] );
               //double_xy xy1( x2 * ds, s[x2] );

               t[i] = ( uint8_t )interpolate( x, double_xy( x1 * ds, s[x1] ) /* xy0 */, double_xy( x2 * ds, s[x2] ) /* xy1 */ );
            }
         }
      }


      /*
      void zoom_byte_float( Byte const* const*s, int sx, int sy,
                            Byte **t,            int tx, int ty,
                            std::vector<Byte> &help ){

         int ix;
         int iy;



         BYTE const* const*sbuf = s;
         BYTE **tbuf = buf;
         for( iy = 0; iy < sy; iy++ ){
            zoom_buf_float( sbuf[iy], tbuf[iy], sx, tx );
            }

         std::vector<Byte> newsbuf( sy );
         std::vector<Byte> newtbuf( ty );

         for( ix = 0; ix < tx; ix++ ){

            BYTE *pbuf = vsbuf;
            BYTE *pim  = *(sbuf + iy);
            for( iy = 0; iy < sy; iy++ ){
               newsbuf[iy] = sbuf[iy][ix];
               pim += t.x;
               }

            Byte *vsbuf = newsbuf.begin();
            Byte *vtbuf = newtbuf.begin();


            zoom_buf_float( vsbuf, vtbuf, sy, ty );

            pbuf = tbuf;
            pim  = t + ix;
            for( iy = 0; iy < t.y; iy++ ){
               *pim = *pbuf++;
               pim += tx;
               }
            }
         }
      */

      // http://www.scratchapixel.com/lessons/3d-advanced-lessons/interpolation/bilinear-interpolation/

      // c01 - c11   topleft - topright
      //  |     |       |         |
      // c00 - c10   lowerleft - lowerright

      double bilinear( double_xy xy, size_t lowerleft, size_t lowerright, size_t topleft, size_t topright ) {
         double tx = xy.x();
         double ty = xy.y();
#if 1
         double a = lowerleft * ( 1 - tx ) + lowerright * tx; // interpolate hor lower
         double b = topleft * ( 1 - tx ) + topright * tx;   // interpolate hor upper
         return a * ( 1 - ty ) + b * ty;                     // interpolate ver a - b
#else
         return ( 1 - tx ) * ( 1 - ty ) * c00
                + tx * ( 1 - ty )       * c10
                + ( 1 - tx ) * ty       * c01
                + tx * ty             * c11;
#endif
      }
      void testBilinearInterpolation() {
         // testing bilinear interpolation
         int imageWidth = 512;
         int gridSizeX = 9;
         int gridSizeY = 9;
         vector<size_t> source( ( gridSizeX + 1 ) * ( gridSizeY + 1 ) );

         // lattices
         // fill grid with random colors
         for( int j = 0, k = 0; j <= gridSizeY; ++j ) {
            for( int i = 0; i <= gridSizeX; ++i, ++k ) {
               source[j * ( gridSizeX + 1 ) + i] = ( size_t )drand48();
            }
         }

         // now compute our final image using bilinear interpolation
         vector<size_t> target( imageWidth * imageWidth );

         for( int j = 0; j < imageWidth; ++j ) {
            for( int i = 0; i < imageWidth; ++i ) {
               // convert i,j to grid coordinates
               double gx = ( ( double )i ) / imageWidth * gridSizeX;
               // be careful to interpolate boundaries
               double gy = ( ( double )j ) / imageWidth * gridSizeY;
               // be careful to interpolate boundaries
               int gxi = static_cast<int>( gx );
               int gyi = static_cast<int>( gy );
               size_t lowerleft = source[gyi * ( gridSizeX + 1 ) + gxi];
               size_t lowerright = source[gyi * ( gridSizeX + 1 ) + ( gxi + 1 )];
               size_t topleft = source[( gyi + 1 ) * ( gridSizeX + 1 ) + gxi];
               size_t topright = source[( gyi + 1 ) * ( gridSizeX + 1 ) + ( gxi + 1 )];
               target[ j * imageWidth + i ] = static_cast<int>( bilinear( double_xy( gx - gxi, gy - gyi ), lowerleft, lowerright, topleft, topright ) );
            }
         }
      }
      void zoom_byte_float( tPlane8 const& source, tPlane8& target ) {

         size_t ix;
         size_t iy;
         //size_t sx = source[0].size();
         size_t sy = source.size();

         size_t tx = target[0].size();
         size_t ty = target.size();


         for( iy = 0; iy < sy; iy++ ) {
            vLine8 const& sbuf = source[iy];
            vLine8& t = target[iy];

            linear_zoom( sbuf, t );
         }

         vLine8 newsbuf( sy );
         vLine8 newtbuf( ty );



         auto ybegin = source.begin();
         //auto yend = source.end();
         auto tbegin = target.begin();
         iy = 0;


         for( ix = 0; ix < tx; ix++ ) {
            // vertical
            vLine8::const_iterator  pbuf = ybegin->begin() + ix;
            tPlane8::const_iterator  ybuf = ybegin;

            for( iy = 0; iy < sy; iy++ ) {
               newsbuf[iy] = ybuf[iy][ix];
            }

            linear_zoom( newsbuf, newtbuf );

            for( iy = 0; iy < sy; iy++ ) {
               tbegin[iy][ix] = newtbuf[iy];

            }
         }
      }




      // simple repeat pixels, ver and hor
      namespace ns_zoomin {
         void zoom_in_buf( tPlane8 const& source, tPlane8& target,  int scale ) {

            size_t sx = source[0].size();
            size_t sy = source.size();

            size_t tx = target[0].size();
            size_t ty = target.size();

            if( ! tx == sx * scale ) {
               return;
            }

            if( ! ty == sy * scale ) {
               return;
            }

            auto ybegin = source.begin();
            auto yend = source.end();

            //auto target_ybegin = target.begin();

            int iy = 0;

            while( ybegin < yend ) {

               // repeat in im scale times to targetx

               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               //auto target_xbegin = target_ybegin->begin();

               int ix = 0;

               while( xbegin < xend ) {

                  //auto target_y = target_ybegin + cnt * tx;
                  //auto target_x = target_ybegin->begin() + sx + cnt;

                  int cy = 0;

                  while( cy < scale ) {
                     int cx = 0;

                     while( cx < scale ) {
                        target[iy * scale + cy][ix * scale + cx] = *xbegin; // repeat value in line scale times
                        cx++;
                     }

                     cy++;
                  }

                  ++xbegin;
                  //target_xbegin += scale;
                  ix++;
               }

               ++ybegin;
               //target_ybegin += scale;
               iy++;
            }

         }
      }

      void rimginterface::ZoomIn( tImgPlanar const& source, tImgPlanar& target, int scale ) {


         if( scale == 1 ) {
            target = source;
            return ;
         }



         uint32_t sx = source.sx();
         uint32_t sy = source.sy();
         uint32_t tsx = sx * scale;
         uint32_t tsy = sy * scale;

         target.realloc( uint32_xy( tsx, tsy ), source.mask() );



         auto mbegin = source.begin();
         auto mend = source.end();

         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {
               ns_zoomin::zoom_in_buf( mbegin->plane, target_mbegin->plane, scale );
            }

            ++mbegin;
            ++target_mbegin;
         }


      }


      // int scale
      namespace ns_zoomout {
         void zoom_out_buf( tPlane8 const& source, tPlane8& target, uint32_t scale ) {



            uint32_t sqrscale = scale * scale;

            size_t sx = source[0].size();
            size_t sy = source.size();

            size_t tx = target[0].size();
            size_t ty = target.size();

            if( ! tx == sx * scale ) {
               return;
            }

            if( ! ty == sy * scale ) {
               return;
            }

            //auto ybegin = target.begin();
            //auto yend = target.end();



            size_t iy = 0;

            while( iy < ty ) {
               // repeat in im scale times to targetx

               size_t ix = 0;

               while( ix < tx ) {


                  // repeat value in line scale times
                  size_t cy = 0;
                  uint32_t sum = 0;

                  while( cy < scale ) {
                     size_t cx = 0;

                     while( cx < scale ) {
                        sum += source[iy * scale + cy][ ix * scale + cx ];
                        cx++;
                     }

                     cy++;
                  }

                  target[iy][ix] = ( uint8_t )( sum / sqrscale );

                  ix++;
               }

               iy++;
            }
         }
      }

      void rimginterface::ZoomOut( tImgPlanar const& source, tImgPlanar& target, int scale ) {



         if( scale == 1 ) {
            target = source;
            return ;
         }


         uint32_t sx = source.sx();
         //   Dword isx = miX;
         uint32_t sy = source.sy();
         uint32_t tsx = sx / scale;
         uint32_t tsy = sy / scale;

         sx = tsx * scale;
         sy = tsy * scale;




         target.realloc( uint32_xy( tsx, tsy ), source.mask() );



         auto mbegin = source.begin();
         auto mend = source.end();

         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {
               ns_zoomout::zoom_out_buf( mbegin->plane, target_mbegin->plane, scale );
            }


            ++mbegin;
         }





      }

      void rimginterface::Zoom( tImgPlanar const& source, tImgPlanar& target, double scale ) {



         double e = 1e-3;
         assert( !( scale < 0 ) );
         assert( !( fabs( scale ) < e ) );

         // zoom == 1
         if( fabs( scale - 1 ) < e ) {
            target = source;
            return ;
         }

         if( scale > 1 ) {
            return ZoomIn( source, target, ( int )( scale + e ) );
         }

         if( scale < 1 ) {
            return ZoomOut( source, target, ( int )( ( 1 / scale ) + e ) );
         }


      }

      void rimginterface::ZoomToSize( tImgPlanar const& source, tImgPlanar& target,  uint32_t sizex, uint32_t sizey ) {


         uint32_t current_sx = target.sx();
         uint32_t current_sy = target.sy();

         double fx = ( ( double )sizex ) / current_sx;
         double fy = ( ( double )sizey ) / current_sy;
         int zfx = ( int )fx;
         int zfy = ( int )fy;

         if( fx > 1 && fy > 1 ) {
            if( zfx < zfy ) {
               ZoomIn( source, target, zfx );
            } else {
               ZoomIn( source, target, zfy );
            }

            return ;
         }

         zfx = 1;
         zfy = 1;

         if( fx <= 1 && fy > 1 ) {
            zfx = ( int )( 1 / fx );
         }

         if( fy <= 1 && fx > 1 ) {
            zfy = ( int )( 1 / fy );
         }

         if( fy <= 1 && fx <= 1 ) {
            //   print_str( fx );
            zfx = ( int )( 1 / fx );
            zfy = ( int )( 1 / fy );
            //   print_str( zfx );
         }

         if( zfx < zfy ) {
            ZoomOut( source, target, zfy );
         } else {
            ZoomOut( source, target, zfx );
         }


      }





      // void zoom_byte_float( tPlane8 const& source, tPlane8& target ) {


      void rimginterface::zoom( tImgPlanar const& source, tImgPlanar& target, double xscale, double yscale ) {


         int inewx = static_cast<int>( source.sx() * xscale + 0.5 );
         int inewy = static_cast<int>( source.sy() * yscale + 0.5 );

         target.realloc( uint32_xy( inewx, inewy ), source.mask() );

         auto mbegin = source.begin();
         auto mend = source.end();

         auto mtarget = target.begin();

         while( mbegin != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               zoom_byte_float( mbegin->plane, mtarget->plane );
            }

            ++mbegin;
            ++mtarget;
         }

      }

   }

} // end of namespace rlf

//EOF






