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

/*! \file rimg_convert.cpp
\brief converts images, linear - planar, RGB - gray, ...

Lib: librimgrw
*/




// librimgrw

#include <memory.h>
#include <assert.h>
#include <algorithm>
#include <cstdint>

#include "rimg_planar.h"
#include "rimg_linear.h"
#include "rimg_convert.h"




using namespace std;
using namespace rlf::rimg_planar;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgViewLinear;

namespace rlf {
   namespace rimg_convert {

      namespace {
         string replace(
            string const& msg,
            string const& s0,
            string const& s1 = "",
            string const& s2 = "",
            string const& s3 = "" ) {

            string marker = "%s";
            string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }

               if( s1.size() > 0 ) {
                  pos = msg.find( marker );

                  if( pos != string::npos ) {
                     temp.erase( pos, marker.size() );
                     temp.insert( pos, s1 );
                  }

                  if( s2.size() > 0 ) {
                     pos = msg.find( marker );

                     if( pos != string::npos ) {
                        temp.erase( pos, marker.size() );
                        temp.insert( pos, s2 );
                     }

                     if( s3.size() > 0 ) {
                        pos = msg.find( marker );

                        if( pos != string::npos ) {
                           temp.erase( pos, marker.size() );
                           temp.insert( pos, s3 );
                        }
                     }
                  }
               }
            }

            return temp;
         }

         class incrementRGBA {
            tRGBA val;
         public:
            incrementRGBA(): val() {}
            incrementRGBA& operator++() {
               val.b()++;
               val.r()++;
               val.g()++;
               val.a() = 255;
               return *this;
            }
            tBGRA operator()() {
               return val;
            }
            operator tRGBA() {
               return val;
            }
            operator string() {
               return "incrementRGBA";
            }

         };

         vector<tRGBA> getLinearLut() {
            static vector<tRGBA> lut( 256 );
            boost::iota( lut, incrementRGBA() );
            return lut;
         }

         bool is_ramp( vector<tRGBA> const&   palette ) {
             vector<tRGBA> const& lut = getLinearLut();


            for( size_t i = 0; i < lut.size(); i++ ) {
               if( !(
                      lut[i].r() == palette[i].r()
                        &&
                      lut[i].g() == palette[i].g()
                        &&
                      lut[i].b() == palette[i].b() )

                 ) {
                  return false;
               }
            }

            return true;
         }

      }

      void convert( tImgViewLinear const& img, tImgPlanar& planar ) {
         if( img.is_mono8() ) {
            auto s = img.size();
            planar.realloc( s, tMask::mono );
            auto r_begin = planar[tMask::pos_mono].begin();
            auto r_start = planar[tMask::pos_mono].begin();
            auto r_end = planar[tMask::pos_mono].end();

            while( r_begin != r_end ) {
               auto ptr8 = img.row_ptr( r_begin - r_start );
               auto line_begin = r_begin->begin();
               auto line_end = r_begin++->end();

               while( line_begin < line_end ) {
                  *line_begin++ = *ptr8++;
               }
            }
            return;
         }

         if( img.is_palette() ) {

            if( is_ramp( img.palette()) ){
               auto s = img.size();
               planar.realloc( s, tMask::mono );
               auto r_begin = planar[tMask::pos_mono].begin();
               auto r_start = planar[tMask::pos_mono].begin();
               auto r_end = planar[tMask::pos_mono].end();

               while( r_begin != r_end ) {
                  auto ptr8 = img.row_ptr( r_begin - r_start );
                  auto line_begin = r_begin->begin();
                  auto line_end = r_begin++->end();

                  while( line_begin < line_end ) {
                     *line_begin++ = *ptr8++;
                  }
               }
               return;

            }
            auto s = img.size();
            planar.realloc( s, tMask::rgb );

            auto r_begin = planar[tMask::pos_r].begin();
            auto r_start = planar[tMask::pos_r].begin();
            auto r_end = planar[tMask::pos_r].end();
            auto g_begin = planar[tMask::pos_g].begin();
            auto b_begin = planar[tMask::pos_b].begin();
            auto palette = img.palette();

            while( r_begin != r_end ) {
               auto source = img.row_ptr( r_begin - r_start );
               auto vr_begin = r_begin->begin();
               auto vr_end   = r_begin++->end();
               auto vg_begin = g_begin++->begin();
               auto vb_begin = b_begin++->begin();
               while( vr_begin < vr_end ){
                  unsigned char r = *source;
                  *vr_begin++ = palette[ r ].r();
                  *vg_begin++ = palette[ r ].g();
                  *vb_begin++ = palette[ r ].b();
                  source++;
               }
            }
            return;
         }

         if( img.is_rgb() ) {
            auto s = img.size();
            planar.realloc( s, tMask::rgb );

            auto r_begin = planar[tMask::pos_r].begin();
            auto r_start = planar[tMask::pos_r].begin();
            auto r_end = planar[tMask::pos_r].end();
            auto g_begin = planar[tMask::pos_g].begin();
            auto b_begin = planar[tMask::pos_b].begin();
            while( r_begin != r_end ) {
               auto source = reinterpret_cast< tRGB const* >( img.row_ptr( r_begin - r_start ) );
               auto vr_begin = r_begin->begin();
               auto vr_end = r_begin++->end();
               auto vg_begin = g_begin++->begin();
               auto vb_begin = b_begin++->begin();
               while( vr_begin < vr_end ){
                  *vr_begin++ = source->r();
                  *vg_begin++ = source->g();
                  *vb_begin++ = source->b();
                  source++;
               }
            }
            return;
         }

         if( img.is_rgba() ) {
            tSize s = img.size();
            planar.realloc( img.size(), tMask::rgba );


            for( size_t y = 0; y < s.y(); y++ ) {
               auto& r = planar.red()[ y ];
               auto& g = planar.green()[ y ];
               auto& b = planar.blue()[ y ];
               auto& a = planar.alpha()[ y ];

               auto ptr8 = img.row_ptr( y );
               auto const* source = reinterpret_cast< tRGBA const* >( ptr8 );

               auto count = s.x();
               auto x = auto_null;

               while( count-- ) {
                  r[x] = source->r();
                  g[x] = source->g();
                  b[x] = source->b();
                  a[x] = source->a();
                  source++;
                  x++;
               }

            }

            return;
         }
      }

      void convert( tImgPlanar const& planar, tImgLinear& img ) {
         if( planar.is_mono8() ) {
            auto s = planar.size();
            img.size() = s;
            img.set_mono8();
            img.alloc_data_buffer();

            for( size_t i = 0; i < img.size().y(); i++ ) {
               memcpy( img.row_ptr( i ), &planar[0][ i ][0], img.pitch() );
            }

            return;
         }

         if( planar.is_rgb() ) {
            tSize s = planar.size();
            img.size() = s;
            img.set_rgb();
            img.alloc_data_buffer();



            for( size_t y = 0; y < s.y(); y++ ) {
               vector<uint8_t> const& r = planar.red()[ y ];
               vector<uint8_t> const& g = planar.green()[ y ];
               vector<uint8_t> const& b = planar.blue()[ y ];

               uint8_t* ptr8 = img.row_ptr( y );
               tRGB* target = reinterpret_cast< tRGB* >( ptr8 );

               size_t count = s.x();
               size_t x = 0;

               while( x < count ) {
                  *target++ = tRGB( r[x], g[x], b[x] );
                  x++;
               }

            }

            return;

         }

         if( planar.is_rgba() ) {


            tSize s = planar.size();
            img.size() = s;
            img.set_rgba();
            img.alloc_data_buffer();



            for( size_t y = 0; y < s.y(); y++ ) {
               auto const& r = planar.red()[ y ];
               auto const& g = planar.green()[ y ];
               auto const& b = planar.blue()[ y ];
               auto const& a = planar.alpha()[ y ];

               uint8_t* ptr8 = img.row_ptr( y );
               tRGBA* target = reinterpret_cast< tRGBA* >( ptr8 );


               size_t count = s.x();
               size_t x = 0;

               while( x < count ) {
                  *target++ = tRGBA( r[x], g[x], b[x], a[x] );
                  x++;
               }

            }

            return;

         }

      }

      // RGB to gray
      // http://en.wikipedia.org/wiki/Grayscale
      void convertRGB_Gray( tImgPlanar const& rgb, tImgPlanar& gray_img ) {

         static uint32_t red_weigth = static_cast<uint32_t>( floor( 0.299 * 4096.0 ) );
         static uint32_t green_weigth = static_cast<uint32_t>( floor( 0.587 * 4096.0 ) );
         static uint32_t blue_weigth = static_cast<uint32_t>( floor( 0.114 * 4096.0 ) );

         if( rgb.is_mono8() ) {
            gray_img = rgb;
            return;
         }

         if( rgb.is_rgb() || rgb.is_rgba() ) {

            tSize s = rgb.size();
            gray_img.realloc( s, tMask::mono );


            for( size_t y = 0; y < s.y(); y++ ) {
               vector<uint8_t> const& r = rgb.red()[ y ];
               vector<uint8_t> const& g = rgb.green()[ y ];
               vector<uint8_t> const& b = rgb.blue()[ y ];

               vector<uint8_t> & gray = gray_img.mono8()[ y ];

               size_t count = s.x();
               size_t x = 0;

               while( x < count ) {
                  uint32_t r0 = r[x];
                  uint32_t g0 = g[x];
                  uint32_t b0 = b[x];
                  uint32_t red   = red_weigth * r0;
                  uint32_t green = green_weigth * g0;
                  uint32_t blue  = blue_weigth * b0;
                  uint32_t val = static_cast<uint32_t>( red + green + blue );
                  val  = val >> 12;
                  gray[x] = val;
                  x++;
               }

            }
         } // RGB

      }
   }
}
//EOF


