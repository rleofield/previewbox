
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
Workfile: rimg_graph.cpp
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
#include "rimg_planar.h"
#include "timer.h"
#include "str_util.h"

using namespace std;




namespace rlf {


   namespace rimg_math {



      inline void setInPlane( tPlane8& img, int32_xy const& xy, int c ) {
         if( xy.x() < 0 || xy.y() < 0 ) {
            return ;
         }

         if( img.size() == 0 ) {
            return ;
         }

         if( xy.x() >= static_cast<int>( img[0].size() ) ||  xy.y() >=  static_cast<int>( img.size() ) ) {
            return ;
         }

         img[xy.y()][xy.x()] = static_cast<uint8_t>(c);

      }


      inline bool isInLine( vLine8 const& line, int32_t x ) {
         if( x < 0 ) {
            return false;
         }

         if( line.size() == 0 ) {
            return false;
         }

         if( x >= static_cast<int>( line.size() ) ) {
            return false;
         }

         return true;
      }

      extern uint8_t const char_set00[];

      namespace {
         void DrawChar( tPlane8& img, uint32_xy const& xy, char ch, int c ) {

            uint32_t index = ch;
            int charindex = ( index << 4 );


            auto y = xy.y();
            const uint8_t* ptr = char_set00 + charindex;

            for( int i = 0; i < 16; i++ ) {
               int b = *ptr++;
               auto x = xy.x();

               for( int i8 = 0; i8 < 8; i8++ ) {
                  int is = 1 << i8;

                  if( b & is ) {
                     img[y][x] = static_cast<uint8_t>(c);
                  }

                  x++;
               }

               y++;
            }

         }


         void DrawChar( tImgPlanar& img, uint32_xy const& xy, char ch, vcolors colors ) {

            auto mbegin = img.begin();
            auto mend   = img.end();
            auto c = colors.begin();

            while( mbegin != mend ) {
               DrawChar( mbegin->plane, xy, ch, *c );
               ++mbegin;
               ++c;
            }

         }
      } // end of local ns

      void rimginterface::DrawText( tImgPlanar& img, uint32_xy xy, const string& text, vcolors const& colors ) {
         assert( img.has_data() );
         assert( img.mask().count() == colors.size() );

         uint32_t x = xy.x();
         uint32_t y = xy.y();

         auto maxx = img.sx();
         auto tbegin = text.begin();
         auto tend = text.end();

         while( tbegin != tend ) {
            if( x < maxx ) {
               DrawChar( img, uint32_xy( x, y ), *tbegin, colors );
            }

            x += 8;
            ++tbegin;
         }
      }


      namespace circle {
         void plot_circle( tPlane8& img,
                           int x,
                           int y,
                           int x_center,
                           int y_center,
                           int c,
                           bool filled,
                           double asp_ratio
                         ) {

            int startx, endx, x1, starty, endy, y1;

            starty = static_cast<int>( y    * asp_ratio );
            endy   = static_cast<int>( ( y + 1 ) * asp_ratio );
            startx = static_cast<int>( x    * asp_ratio );
            endx   = static_cast<int>( ( x + 1 ) * asp_ratio );

            for( x1 = startx; x1 <= endx; ++x1 ) {
               if( filled ) {
                  int i;

                  for( i = x_center - x1; i <= x_center + x1; i++ ) {
                     setInPlane( img, int32_xy( i, y_center + y ), c );
                     setInPlane( img, int32_xy( i, y_center - y ), c );
                  }
               } else {
                  setInPlane( img, int32_xy( x_center + x1, y_center + y ) , c );
                  setInPlane( img, int32_xy( x_center + x1, y_center - y ) , c );
                  setInPlane( img, int32_xy( x_center - x1, y_center - y ) , c );
                  setInPlane( img, int32_xy( x_center - x1, y_center + y ) , c );
               }
            }

            for( y1 = starty; y1 <= endy; ++y1 ) {
               if( filled ) {
                  int i;

                  for( i = x_center - y1; i <= x_center + y1; i++ ) {
                     setInPlane( img, int32_xy( i, y_center + x ), c );
                     setInPlane( img, int32_xy( i, y_center - x ), c );
                  }
               } else {
                  setInPlane( img, int32_xy( y1 + x_center, x + y_center ), c );
                  setInPlane( img, int32_xy( y1 + x_center, y_center - x ), c );
                  setInPlane( img, int32_xy( x_center - y1, y_center - x ), c );
                  setInPlane( img, int32_xy( x_center - y1, x + y_center ), c );
               }
            }
         }




         void Circle(
            tPlane8& img,
            int x_center,
            int y_center,
            int radius,
            int c,
            bool filled,
            double asp_ratio
         ) {

            int x, y, delta;
            y     = radius;
            delta = 3 - 2 * radius;

            for( x = 0; x < y; x++ ) {
               plot_circle( img, x, y, x_center, y_center, c, filled, asp_ratio );

               if( delta < 0 ) {
                  delta += 4 * x     + 6;
               } else {
                  delta += 4 * ( x - y ) + 10;
                  y--;
               }
            }

            x = y;

            if( y ) {
               plot_circle( img, x, y, x_center, y_center, c, filled, asp_ratio );
            }
         }

      } // end of local ns

      void rimginterface::Circle( tImgPlanar& img, int32_xy xy, uint32_t r, vcolors const& colors ) {
         assert( img.has_data() );
         assert( img.planes() == colors.size() );
         auto mbegin = img.begin();
         auto mend = img.end();
         auto c = colors.begin();

         while( mbegin  != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               circle::Circle( mbegin->plane, xy.x(), xy.y(), r, *c, 0, 1.0 );
            }

            ++mbegin;
            ++c;
         }
      }

      void rimginterface::FilledCircle( tImgPlanar& img, int32_xy xy, uint32_t r, vcolors const& colors ) {
         //assert( img.has_data() );
         //assert( img.planes() == colors.size() );
         auto mbegin = img.begin();
         auto mend = img.end();
         auto c = colors.begin();

         while( mbegin  != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               circle::Circle( mbegin->plane, xy.x(), xy.y(), r, *c, 1, 1.0 );
            }

            ++mbegin;
            ++c;
         }
      }


      namespace vline {
         void VerticalLine( tPlane8& img,  uint32_t xpos, int c ) {
            uint32_t y = static_cast<uint32_t>( img.size() );

            while( y-- ) {
               setInPlane( img, int32_xy( y, xpos ), c );
            }
         }
      } // end of local ns



      void rimginterface::VerticalLine( tImgPlanar& img, uint32_t xpos, vcolors const& colors ) {
         assert( img.has_data() );
         assert( img.planes() == colors.size() );

         auto mbegin = img.begin();
         auto mend   = img.end();
         auto c = colors.begin();

         while( mbegin != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               vline::VerticalLine( mbegin->plane,  xpos, *c );
            }

            ++mbegin;
            ++c;
         }
      }

      namespace hline {
         void HorizontalLine( tPlane8& img,  uint32_t ypos, int c ) {
            int32_t x = static_cast<int32_t>( img[0].size() ); // changed win

            while( x-- ) {
               setInPlane( img, int32_xy( ypos, x ), c );
            }
         }
      } // end of local ns



      void rimginterface::HorizontalLine( tImgPlanar& img, uint32_t xpos, vcolors const& colors ) {
         assert( img.has_data() );
         assert( img.planes() == colors.size() );

         auto mbegin = img.begin();
         auto mend   = img.end();
         auto c = colors.begin();

         while( mbegin != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               hline::HorizontalLine( mbegin->plane,  xpos, *c );
            }

            ++mbegin;
            ++c;
         }
      }

      void rimginterface::WedgeHorizontal( tImgPlanar& img, uint32_t sy ) {
         img.realloc( uint32_xy( 256, sy ), tMask::mono );

         for( int x = 0; x < 256; x++ ) {
            vcolors color( img.planes() );
            color[0] = static_cast<uint8_t>(x);
            VerticalLine( img, sy, color );
         }
      }

      void rimginterface::WedgeVertical( tImgPlanar& img, uint32_t sx ) {
         img.realloc( uint32_xy( sx, 256 ), tMask::mono );

         for( int y = 0; y < 256; y++ ) {
            vcolors color;
            color.push_back( static_cast<uint8_t>(y) );
            HorizontalLine( img, y, color );
         }
      }

      namespace line {

         void Line( tPlane8& img,  int32_xy from, int32_xy to, uint32_t c ) {

            int vx = from.x();
            int vy = from.y();
            int nx = to.x();
            int ny = to.y();

            int xinc, yinc, x, y, dx, dy, start, help1, help2, last;
            bool wiha;
            xinc = yinc = 1;
            x = vx, y = vy;
            last = nx;
            dx = nx - vx;
            dy = ny - vy;
            wiha = false;

            if( dx < 0 ) {
               xinc = -1;
               dx = -dx;
            }

            if( dy < 0 ) {
               yinc = -1;
               dy = -dy;
            }

            /* -- swap x,y, if dx < dy
            */
            if( dx < dy ) {
               int help;
               help = x;
               x    = y;
               y    = help;
               help = dx;
               dx   = dy;
               dy   = help;
               help = xinc;
               xinc = yinc;
               yinc = help;
               last = ny;
               wiha = true;
            }

            start = dy << 1;
            help1 = ( dx << 1 ) - start;
            help2 = start - dx;

            if( wiha == true ) {
               setInPlane( img, int32_xy( x, y ), c );

               while( x != last ) {
                  x = x + xinc ;

                  if( help2 > 0 ) {
                     y = y + yinc;
                     help2 = help2 - help1;
                  } else {
                     help2 = help2 + start;
                  }

                  setInPlane( img, int32_xy( x, y ), c );
               }
            } else {
               setInPlane( img, int32_xy( x, y ), c );

               while( x != last ) {
                  x = x + xinc ;

                  if( help2 > 0 ) {
                     y = y + yinc;
                     help2 = help2 - help1;
                  } else {
                     help2 = help2 + start;
                  }

                  setInPlane( img, int32_xy( x, y ), c );
               }
            }

         }
      } // end of ns line


      void rimginterface::OrthogonalCross( tImgPlanar& img, int32_xy pos, uint32_t size, vcolors const& colors ) {

         assert( img.has_data() );
         assert( img.planes() == colors.size() );

         auto mbegin = img.begin();
         auto mend   = img.end();
         auto c = colors.begin();

         while( mbegin != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               int x = pos.x() - size / 2;
               int y = pos.y();
               int32_xy from( x, y );
               int32_xy to( from );
               to += int32_xy( size, 0 );
               line::Line( mbegin->plane, from, to, *c );
               y = pos.y() - size / 2;
               x = pos.x();
               from = int32_xy( x, y );
               to = from;
               to += int32_xy( 0, size );
               line::Line( mbegin->plane, from, to, *c );
            }

            ++mbegin;
            ++c;
         }



      }




      void rimginterface::DiagonalCross( tImgPlanar& img, int32_xy xy, uint32_t const& c ) {
         assert( img.is_mono8() );

         int32_t xp = xy.x();
         int32_t yp = xy.y();

         int32_t x = xy.x();
         int32_t y = xy.y();

         uint32_t sx = img.sx();
         uint32_t sy = img.sy();
         int32_t signed_sx = img.sx();
         int32_t signed_sy = img.sy();

         uint8_t color = static_cast<uint8_t>(c);

         tPlane8 &plane = img[0];

         assert( xp >= 0 );
         assert( yp >= 0 );
         assert( xp < signed_sx);
         assert( yp < signed_sy );

         plane[yp][xp] = color;

         xp = x + 1;

         if( xp >= signed_sx ) {
            xp--;
         }

         yp = y + 1;

         if( yp >= signed_sy ) {
            yp--;
         }

         img[0][yp][xp] = color;

         xp = x - 1;

         if( xp < 0 ) {
            xp++;
         }

         img[0][yp][xp] = color ;

         yp = y - 1;

         if( yp < 0 ) {
            yp++;
         }

         img[0][yp][xp] = color ;

         xp = x + 1;

         if( xp >= static_cast<int>(img.sx()) ) {
            xp--;
         }

         img[0][yp][xp] = color ;
      }



      /* -- draws a line line, clips at border
      Bresenhams line algorithm
      see: http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
      */
      void rimginterface::Line( tImgPlanar& img,  int32_xy from, int32_xy to, vcolors const& colors ) {
         assert( img.planes() == colors.size() );


         auto mbegin = img.begin();
         auto mend = img.end();
         auto c = colors.begin();

         while( mbegin  != mend ) {
            if( img.has_bit_at( mbegin ) ) {
               line::Line( mbegin->plane, from, to, *c );
            }

            ++mbegin;
            ++c;
         }
      }


      // The function object to determine the average
      class DrawPixelListFunc {
      private:
         uint32_t num;
         tImgPlanar* pImageBuffer;
      public:
         // Constructor initializes the value to multiply by
         DrawPixelListFunc( uint32_t c, tImgPlanar* bu ) : num( c ), pImageBuffer( bu ) {
         }

         // The function call to process the next elment
         void operator( )( uint32_xy xy ) {
            pImageBuffer->pixel( xy, num );
         }

      };

      void rimginterface::DrawPixelList( tImgPlanar& img, std::vector<int32_xy> v,  uint32_t val ) {
         for( size_t i = 0; i < v.size(); i++ ) {

            img.pixel( uint32_xy( v[i].x(), v[i].y() ), val );
         }

         //std::for_each( v.begin(), v.end(), DrawPixelListFunc(val,&img) );
      }

      std::vector<uint32_t> rimginterface::GetLinePixels( tImgPlanar const& img, uint32_xy from, uint32_xy to ) {
         int vx = from.x();
         int vy = from.y();
         int nx = to.x();
         int ny = to.y();

         std::vector<uint32_t> v;

         int xinc, yinc, x, y, dx, dy, start, help1, help2, last;
         bool wiha;
         xinc = yinc = 1;
         x = vx, y = vy;
         last = nx;
         dx = nx - vx;
         dy = ny - vy;
         wiha = false;

         if( dx < 0 ) {
            xinc = -1;
            dx = -dx;
         }

         if( dy < 0 ) {
            yinc = -1;
            dy = -dy;
         }

         /* -- swap x,y, if dx < dy
         */
         if( dx < dy ) {
            int help;
            help = x;
            x    = y;
            y    = help;
            help = dx;
            dx   = dy;
            dy   = help;
            help = xinc;
            xinc = yinc;
            yinc = help;
            last = ny;
            wiha = true;
         }

         start = dy << 1;
         help1 = ( dx << 1 ) - start;
         help2 = start - dx;

         if( wiha == true ) {
            //Pixel( tixy(y, x), c );
            v.push_back( img.pixel( y, x ) );

            while( x != last ) {
               x = x + xinc ;

               if( help2 > 0 ) {
                  y = y + yinc;
                  help2 = help2 - help1;
               } else {
                  help2 = help2 + start;
               }

               //Pixel( tixy(y, x), c );
               v.push_back( img.pixel( y, x ) );
            }
         } else {
            //Pixel( tixy(x, y), c );
            v.push_back( img.pixel( x, y ) );

            while( x != last ) {
               x = x + xinc ;

               if( help2 > 0 ) {
                  y = y + yinc;
                  help2 = help2 - help1;
               } else {
                  help2 = help2 + start;
               }

               //Pixel( tixy(x, y), c );
               v.push_back( img.pixel( x, y ) );
            }
         }

         return v;
      }

      int rimginterface::GetLinePixels( tImgPlanar const& img, uint32_xy from, uint32_xy to, std::vector<uint32_t>& v ) {
         int vx = from.x();
         int vy = from.y();
         int nx = to.x();
         int ny = to.y();


         int xinc, yinc, x, y, dx, dy, start, help1, help2, last;
         bool wiha;
         xinc = yinc = 1;
         x = vx, y = vy;
         last = nx;
         dx = nx - vx;
         dy = ny - vy;
         wiha = false;

         if( dx < 0 ) {
            xinc = -1;
            dx = -dx;
         }

         if( dy < 0 ) {
            yinc = -1;
            dy = -dy;
         }

         /* -- swap x,y, if dx < dy
         */
         if( dx < dy ) {
            int help;
            help = x;
            x    = y;
            y    = help;
            help = dx;
            dx   = dy;
            dy   = help;
            help = xinc;
            xinc = yinc;
            yinc = help;
            last = ny;
            wiha = true;
         }

         start = dy << 1;
         help1 = ( dx << 1 ) - start;
         help2 = start - dx;

         if( wiha == true ) {
            //Pixel( tixy(y, x), c );
            v.push_back( img.pixel( x, y ) );

            while( x != last ) {
               x = x + xinc ;

               if( help2 > 0 ) {
                  y = y + yinc;
                  help2 = help2 - help1;
               } else {
                  help2 = help2 + start;
               }

               //Pixel( tixy(y, x), c );
               v.push_back( img.pixel( x, y ) );
            }
         } else {
            //Pixel( tixy(x, y), c );
            v.push_back( img.pixel( x, y ) );

            while( x != last ) {
               x = x + xinc ;

               if( help2 > 0 ) {
                  y = y + yinc;
                  help2 = help2 - help1;
               } else {
                  help2 = help2 + start;
               }

               //Pixel( tixy(x, y), c );
               v.push_back( img.pixel( x, y ) );
            }
         }

         return static_cast<int>(v.size());
      }

      // http://de.wikipedia.org/wiki/Codepage_437

      uint8_t const char_set00[4096] = {

         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7e, 0x81, 0xa5, 0x81, 0x81, 0xbd, 0x99, 0x81, 0x81, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 0x7f, 0x7f, 0x6b, 0x7f, 0x7f, 0x5d, 99, 0x7f, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 54, 119, 0x7f, 0x7f, 0x7f, 0x7f, 0x3e, 28, 0x08, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x08, 28, 0x3e, 0x7f, 0x3e, 28, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x08, 28, 28, 0x08, 54, 119, 54, 0x08, 28, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x08, 28, 0x3e, 0x3e, 0x7f, 0x7f, 0x7f, 54, 0x08, 28, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 120, 112, 120, 108, 30, 51, 51, 51, 51, 30, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 60, 102, 102, 102, 60, 24, 0x7e, 24, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 124, 108, 124, 108, 108, 110, 111, 102, 112, 120, 48, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 219, 0x7e, 60, 102, 102, 60, 0x7e, 219, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 1, 7, 15, 0x3f, 0x7f, 0x3f, 15, 7, 1, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 64, 112, 124, 0x7e, 0x7f, 0x7e, 124, 112, 64, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 60, 0x7e, 24, 24, 24, 24, 0x7e, 60, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 102, 102, 102, 102, 102, 102, 102, 0x00, 102, 102, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0xfe, 219, 219, 219, 219, 222, 216, 216, 216, 216, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 6, 0x3e, 111, 123, 0x3e, 48, 99, 99, 0x3e, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 60, 0x7e, 24, 24, 24, 0x7e, 60, 24, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 60, 0x7e, 24, 24, 24, 24, 24, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 24, 24, 24, 24, 24, 24, 0x7e, 60, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' '
         0x00, 0x00, 24, 60, 60, 60, 60, 24, 24, 0x00, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 108, 108, 108, 108, 40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 54, 54, 54, 0x7f, 54, 54, 0x7f, 54, 54, 54, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 24, 0x3e, 99, 3, 30, 60, 96, 99, 0x3e, 24, 24, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 70, 102, 48, 24, 12, 102, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 28, 54, 28, 12, 110, 0x7e, 51, 51, 51, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 48, 48, 48, 24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 48, 24, 12, 12, 12, 12, 12, 12, 24, 48, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 12, 24, 48, 48, 48, 48, 48, 48, 24, 12, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 54, 28, 0x7f, 28, 54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 24, 24, 0x7e, 24, 24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 48, 48, 48, 24, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 64, 96, 48, 24, 12, 6, 3, 1, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 0x6b, 0x6b, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 30, 24, 24, 24, 24, 24, 24, 24, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 96, 48, 24, 12, 6, 99, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 96, 96, 60, 96, 96, 96, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 48, 56, 60, 54, 51, 51, 0x7f, 48, 48, 120, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 3, 3, 3, 0x3f, 96, 96, 96, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 3, 3, 0x3f, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 99, 96, 48, 24, 12, 12, 12, 12, 12, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 0x3e, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 99, 0x7e, 96, 96, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 48, 48, 0x00, 0x00, 48, 48, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 48, 48, 0x00, 0x00, 48, 48, 48, 24, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 48, 24, 12, 6, 3, 6, 12, 24, 48, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 6, 12, 24, 48, 96, 48, 24, 12, 6, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 48, 24, 24, 24, 0x00, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 123, 123, 123, 59, 3, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 28, 54, 99, 99, 99, 0x7f, 99, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3f, 102, 102, 102, 0x3e, 102, 102, 102, 102, 0x3f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 60, 102, 67, 3, 3, 3, 3, 67, 102, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 31, 54, 102, 102, 102, 102, 102, 102, 54, 31, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 102, 6, 6, 0x3e, 6, 6, 6, 102, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 102, 6, 6, 0x3e, 6, 6, 6, 6, 15, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 3, 3, 3, 115, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 99, 99, 0x7f, 99, 99, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 60, 24, 24, 24, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 60, 24, 24, 24, 24, 24, 24, 27, 27, 14, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 51, 27, 15, 15, 27, 51, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 15, 6, 6, 6, 6, 6, 6, 70, 102, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 119, 119, 0x7f, 0x6b, 0x6b, 0x6b, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 103, 103, 111, 123, 115, 115, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3f, 102, 102, 102, 102, 0x3e, 6, 6, 6, 15, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 99, 99, 99, 0x6b, 0x6b, 0x3e, 96, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3f, 102, 102, 102, 0x3e, 30, 54, 102, 102, 103, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 3, 3, 14, 56, 96, 96, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7e, 90, 24, 24, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 99, 99, 99, 99, 99, 54, 28, 0x08, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 99, 0x6b, 0x6b, 0x6b, 0x7f, 119, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 99, 54, 28, 28, 54, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 102, 102, 102, 102, 102, 60, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 99, 97, 48, 24, 12, 6, 67, 99, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 6, 6, 6, 6, 6, 6, 6, 6, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 1, 3, 6, 12, 24, 48, 96, 64, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 48, 48, 48, 48, 48, 48, 48, 48, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x08, 28, 54, 99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
         0x00, 24, 24, 24, 48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 7, 6, 6, 0x3e, 102, 102, 102, 102, 102, 0x3f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 99, 3, 3, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 56, 48, 48, 0x3e, 51, 51, 51, 51, 51, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 99, 99, 0x7f, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 56, 108, 12, 12, 0x3f, 12, 12, 12, 12, 30, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 110, 115, 99, 99, 115, 110, 96, 99, 0x3e, 0x00, 0x00,
         0x00, 0x00, 7, 6, 6, 0x3e, 102, 102, 102, 102, 102, 103, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 24, 0x00, 28, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 48, 48, 0x00, 56, 48, 48, 48, 48, 48, 51, 51, 30, 0x00, 0x00,
         0x00, 0x00, 7, 6, 6, 102, 102, 54, 30, 54, 102, 103, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 28, 24, 24, 24, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 54, 0x7f, 0x6b, 0x6b, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 59, 102, 102, 102, 102, 102, 102, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 59, 102, 102, 102, 102, 0x3e, 6, 6, 15, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 110, 51, 51, 51, 51, 0x3e, 48, 48, 120, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 59, 102, 6, 6, 6, 6, 15, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 99, 3, 0x3e, 96, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 12, 12, 12, 0x3f, 12, 12, 12, 12, 108, 56, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 51, 51, 51, 51, 51, 51, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 99, 99, 99, 99, 54, 28, 0x08, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 99, 99, 0x6b, 0x6b, 0x6b, 0x7f, 54, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 99, 99, 54, 28, 54, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 99, 99, 99, 99, 115, 110, 96, 99, 0x3e, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 97, 48, 24, 12, 70, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 112, 24, 24, 24, 14, 24, 24, 24, 24, 112, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 24, 24, 24, 0x00, 24, 24, 24, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 14, 24, 24, 24, 112, 24, 24, 24, 24, 14, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 110, 59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 28, 28, 54, 54, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 60, 102, 3, 3, 3, 99, 102, 60, 24, 48, 51, 28, 0x00, 0x00,
         0x00, 0x00, 99, 99, 0x00, 99, 99, 99, 99, 99, 115, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 48, 24, 12, 0x00, 0x3e, 99, 99, 0x7f, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 12, 30, 51, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 51, 51, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 6, 12, 24, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 28, 54, 28, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x3e, 99, 3, 3, 99, 0x3e, 24, 48, 54, 28, 0x00, 0x00,
         0x00, 12, 30, 51, 0x00, 0x3e, 99, 99, 0x7f, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 51, 51, 0x00, 0x3e, 99, 99, 0x7f, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 12, 24, 48, 0x00, 0x3e, 99, 99, 0x7f, 3, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 102, 102, 0x00, 28, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 24, 60, 102, 0x00, 28, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 6, 12, 24, 0x00, 28, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 99, 99, 0x00, 28, 54, 99, 99, 0x7f, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         28, 54, 28, 0x00, 28, 54, 99, 99, 0x7f, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         48, 24, 12, 0x00, 0x7f, 6, 6, 0x3e, 6, 6, 6, 0x7f, 0x00, 0x00, 0x00, 0x00,

         0x00, 0x00, 0x00, 0x00, 102, 219, 216, 0xfe, 27, 27, 0xfb, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7e, 27, 27, 27, 27, 0x7f, 27, 27, 27, 123, 0x00, 0x00, 0x00, 0x00,
         0x00, 12, 30, 51, 0x00, 0x3e, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 99, 99, 0x00, 0x3e, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 12, 24, 48, 0x00, 0x3e, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 12, 30, 51, 0x00, 99, 99, 99, 99, 99, 115, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 6, 12, 24, 0x00, 99, 99, 99, 99, 99, 115, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 99, 99, 0x00, 99, 99, 99, 99, 115, 110, 96, 96, 99, 0x3e, 0x00, 0x00,
         0x00, 99, 99, 0x00, 0x3e, 99, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 99, 99, 0x00, 99, 99, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 24, 24, 0x3e, 99, 3, 3, 99, 0x3e, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 28, 54, 6, 6, 15, 6, 6, 6, 102, 111, 54, 0x00, 0x00, 0x00, 0x00,
         0x00, 102, 102, 102, 102, 60, 24, 0x7e, 24, 60, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x3f, 99, 99, 0x3f, 3, 51, 123, 51, 51, 51, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 112, 216, 24, 24, 24, 0x7e, 24, 24, 24, 24, 24, 27, 14, 0x00, 0x00,
         0x00, 48, 24, 12, 0x00, 30, 48, 0x3e, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 48, 24, 12, 0x00, 28, 24, 24, 24, 24, 24, 60, 0x00, 0x00, 0x00, 0x00,
         0x00, 48, 24, 12, 0x00, 0x3e, 99, 99, 99, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 24, 12, 6, 0x00, 51, 51, 51, 51, 51, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 110, 59, 0x00, 61, 102, 102, 102, 102, 102, 103, 0x00, 0x00, 0x00, 0x00,
         0x00, 110, 59, 0x00, 99, 99, 103, 111, 123, 115, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 60, 54, 54, 124, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 28, 54, 54, 28, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 12, 12, 0x00, 12, 12, 12, 6, 99, 99, 0x3e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 6, 6, 6, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 96, 96, 96, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 6, 6, 70, 102, 54, 24, 12, 6, 59, 108, 48, 24, 124, 0x00, 0x00,
         0x00, 6, 6, 70, 102, 54, 24, 108, 118, 123, 108, 0x7e, 96, 96, 0x00, 0x00,
         0x00, 0x00, 24, 24, 0x00, 24, 24, 60, 60, 60, 60, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 108, 54, 27, 54, 108, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 27, 54, 108, 54, 27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         136, 34, 136, 34, 136, 34, 136, 34, 136, 34, 136, 34, 136, 34, 136, 34,
         85, 170, 85, 170, 85, 170, 85, 170, 85, 170, 85, 170, 85, 170, 85, 170,
         187, 0xee, 187, 0xee, 187, 0xee, 187, 0xee, 187, 0xee, 187, 0xee, 187, 0xee, 187, 0xee,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 31, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 31, 24, 31, 24, 24, 24, 24, 24, 24, 24,
         108, 108, 108, 108, 108, 108, 108, 108, 111, 108, 108, 108, 108, 108, 108, 108,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 108, 108, 108, 108, 108, 108, 108,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 31, 24, 31, 24, 24, 24, 24, 24, 24, 24,
         108, 108, 108, 108, 108, 108, 111, 96, 111, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 96, 111, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 111, 96, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         108, 108, 108, 108, 108, 108, 108, 108, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         24, 24, 24, 24, 24, 24, 31, 24, 31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 31, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         24, 24, 24, 24, 24, 24, 24, 24, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 0xf8, 24, 24, 24, 24, 24, 24, 24,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         24, 24, 24, 24, 24, 24, 24, 24, 0xff, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 0xf8, 24, 0xf8, 24, 24, 24, 24, 24, 24, 24,
         108, 108, 108, 108, 108, 108, 108, 108, 0xec, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 0xec, 12, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 12, 0xec, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 0xef, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xef, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 0xec, 12, 0xec, 108, 108, 108, 108, 108, 108, 108,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         108, 108, 108, 108, 108, 108, 0xef, 0x00, 0xef, 108, 108, 108, 108, 108, 108, 108,
         24, 24, 24, 24, 24, 24, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         108, 108, 108, 108, 108, 108, 108, 108, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0xff, 24, 24, 24, 24, 24, 24, 24,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 108, 108, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         24, 24, 24, 24, 24, 24, 0xf8, 24, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 24, 0xf8, 24, 24, 24, 24, 24, 24, 24,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 108, 108, 108, 108, 108, 108, 108,
         108, 108, 108, 108, 108, 108, 108, 108, 0xff, 108, 108, 108, 108, 108, 108, 108,
         24, 24, 24, 24, 24, 24, 0xff, 24, 0xff, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 24, 24, 24, 24, 24, 24, 24,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
         15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
         0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
         0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 110, 59, 27, 27, 27, 27, 59, 110, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 30, 51, 27, 0x3f, 99, 99, 103, 59, 3, 3, 0x00, 0x00,
         0x00, 0x00, 0x7f, 102, 70, 6, 6, 6, 6, 6, 6, 6, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 54, 54, 54, 54, 54, 54, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 99, 70, 12, 24, 24, 12, 70, 99, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 27, 51, 51, 51, 27, 14, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 102, 102, 102, 102, 102, 0x3e, 6, 3, 1, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 110, 59, 24, 24, 24, 24, 24, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x7f, 28, 28, 54, 99, 99, 54, 28, 28, 0x7f, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 28, 54, 99, 99, 0x7f, 99, 99, 54, 28, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 28, 54, 99, 99, 99, 99, 54, 54, 54, 119, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 124, 6, 6, 60, 102, 99, 99, 99, 51, 30, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 219, 219, 219, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 64, 96, 0x3e, 115, 123, 111, 111, 0x3e, 6, 3, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 56, 12, 6, 6, 0x3e, 6, 6, 12, 56, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x3e, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 24, 24, 0x7e, 24, 24, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 12, 24, 48, 96, 48, 24, 12, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 48, 24, 12, 6, 12, 24, 48, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 48, 120, 88, 24, 24, 24, 24, 24, 24, 24, 24, 24,
         24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 26, 30, 12, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 24, 24, 0x00, 0x7e, 0x00, 24, 24, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 110, 59, 0x00, 110, 59, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 30, 51, 51, 30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 24, 24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0xf8, 24, 24, 24, 24, 24, 27, 27, 30, 28, 24, 0x00, 0x00, 0x00,
         0x00, 0x00, 27, 54, 54, 54, 54, 54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 14, 27, 24, 12, 6, 31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };


   } // end of ns rimg_math

} // end of namespace rlf

//EOF

