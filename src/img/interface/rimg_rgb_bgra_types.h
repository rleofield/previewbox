/*
--------------------------------------------------------------------------
Copyright 2013 by Richard Albrecht
richard.albrecht@rleofield.de
www.rleofield.de

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


/*! \file rimg_rgb_bgra_types.h
\brief basic types for RGB Images

Lib: librimgrw
*/


#ifndef RIMG_T_RGB_RGBA_H
#define RIMG_T_RGB_RGBA_H

#include <boost/range/algorithm_ext/iota.hpp>
#include <vector>
#include <string>

#include "boost_cstdint.h"


using std::string;
using std::vector;
//using rlf::uint8_t;

namespace rlf {

   const uint8_t max_byte = 0xFF;



   typedef  std::vector<uint8_t>  vcolors;

   typedef std::vector<uint32_t> vminmax;

   class tMonoColor {
      uint8_t _c;

   public:

      tMonoColor(): _c( 0 ) {}
      tMonoColor( uint8_t color ): _c( color ) {}

      bool operator==( const tMonoColor& m )const ;
      bool operator!=( const tMonoColor& s )const ;
      tMonoColor& operator=( const tMonoColor& m ) ;
      // value of color
      uint8_t val() const;
      // get list of colors, according to mask
      operator vcolors()const;


   };



   class tRGB {
      uint8_t _r;
      uint8_t _g;
      uint8_t _b;
      tRGB& operator+=( const tRGB& s );
      tRGB& operator-=( const tRGB& s );
      tRGB& operator*=( int d );

   public:

      tRGB(): _r( 0 ), _g( 0 ), _b( 0 ) {}
      tRGB( uint8_t r_, uint8_t g_, uint8_t b_ ): _r( r_ ), _g( g_ ), _b( b_ ) {}
      ~tRGB() {}
      bool operator==( const tRGB& rgb )const {
         return rgb._r == _r && rgb._g == _g && rgb._b == _b;
      }

      bool operator!=( const tRGB& s )const {
         return !( s == *this );
      }
      //      tRGB& operator=( const tRGB& rgb ) {
      //         if( this != &rgb ) {
      //            this->_r = rgb._r;
      //            this->_g = rgb._g;
      //            this->_b = rgb._b;
      //         }

      //         return *this;
      //      }
      uint8_t& r() {
         return _r;
      }
      uint8_t& g() {
         return _g;
      }
      uint8_t& b() {
         return _b;
      }
      uint8_t r()const {
         return _r;
      }
      uint8_t g()const {
         return _g;
      }
      uint8_t b()const {
         return _b;
      }


      operator vcolors()const ;

   };


   class tRGBtxt: public tRGB {
      std::string _txt;
   public:
      tRGBtxt( uint8_t r, uint8_t g, uint8_t b, std::string const& t );
      ~tRGBtxt();
      std::string txt() const {
         return _txt;
      }
      static std::vector<tRGBtxt> rgb_txt;
      static std::vector<tRGBtxt> const& list() {
         return rgb_txt;
      }
      static std::vector<std::string> keys();
      static tRGB const& lookup( std::string const& k );
   };


   //   float k = MIN(255-red,MIN(255-green,255-blue));
   //   float c = 255*(255-red-k)/(255-k);
   //   float m = 255*(255-green-k)/(255-k);
   //   float y = 255*(255-blue-k)/(255-k);


   class tRGBA {
      uint8_t _r;
      uint8_t _g;
      uint8_t _b;
      uint8_t _a;

      tRGBA& operator+=( const tRGBA& s );
      tRGBA& operator-=( const tRGBA& s );
      tRGBA& operator*=( int d );

   public:

      tRGBA(): _r( 0 ), _g( 0 ), _b( 0 ), _a( 0 ) {}
      tRGBA( uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ ): _r( r_ ), _g( g_ ), _b( b_ ), _a( a_ ) {}
      tRGBA( uint8_t val ): _r( val ), _g( val ), _b( val ), _a( max_byte ) {}
      bool operator==( const tRGBA& rgb )const {
         return rgb._r == _r && rgb._g == _g && rgb._b == _b && _a == rgb._a ;
      }

      bool operator!=( const tRGBA& s )const {
         return !( s == *this );
      }
      tRGBA& operator=( const tRGBA& rgb ) {
         if( this != &rgb ) {
            _r = rgb._r;
            _g = rgb._g;
            _b = rgb._b;
            _a = rgb._a;
         }

         return *this;
      }
      //      operator uint32_t()const{
      //         return ((_a & 0xff) << 24) | ((_r & 0xff) << 16) | ((_g & 0xff) << 8) | (_b & 0xff ) ;
      //      }
      uint8_t& r() {
         return _r;
      }
      uint8_t& g() {
         return _g;
      }
      uint8_t& b() {
         return _b;
      }
      uint8_t& a() {
         return _a;
      }
      uint8_t r()const {
         return _r;
      }
      uint8_t g()const {
         return _g;
      }
      uint8_t b()const {
         return _b;
      }
      uint8_t a()const {
         return _a;
      }
      operator vcolors()const;

   };




   class tBGR {

      tBGR& operator+=( const tBGR& s );
      tBGR& operator-=( const tBGR& s );
      tBGR& operator*=( int d );

   public:
      uint8_t blue;
      uint8_t green;
      uint8_t red;
      tBGR(): blue(), green(), red() {}
      tBGR& operator=( size_t index ) {
         blue = static_cast<uint8_t>( index );
         green = static_cast<uint8_t>( index );
         red = static_cast<uint8_t>( index );
         return *this;
      }

      tBGR& operator=( tBGR const&  bgr ) {
         if( this != & bgr ) {
            blue = bgr.blue;
            green = bgr.green;
            red = bgr.red;
         }

         return *this;
      }
      // flip r and b
      // cast operator RGB to BGR
      tBGR( tRGB const&  rgb ): blue( rgb.b() ), green( rgb.g() ), red( rgb.r() ) {}
      tBGR& operator=( tRGB const&  rgb ) {
         blue = rgb.b();
         green = rgb.g();
         red = rgb.r();
         return *this;
      }
      operator tRGB()const {           // cast operator BGR to RGB
         return tRGB( red, green, blue );
      }
      operator string()const {
         return "tBGR";
      }


   };



   class tBGRA {
      tBGRA& operator+=( const tBGRA& s );
      tBGRA& operator-=( const tBGRA& s );
      tBGRA& operator*=( int d );

   public:
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
      tBGRA& operator=( size_t val ) {
         r = g = b = static_cast<uint8_t>( val );
         a = max_byte;
         return *this;
      }
      tBGRA(): b( 0 ), g( 0 ), r( 0 ), a( 0 ) { }
      tBGRA( tRGB const&  rgb ): b( rgb.r() ), g( rgb.g() ), r( rgb.b() ), a( max_byte ) { }
      tBGRA( tRGBA const&  rgba ): b( rgba.b() ), g( rgba.g() ), r( rgba.r() ), a( rgba.a() ) {}
      tBGRA( uint8_t val ): b( val ), g( val ), r( val ), a( max_byte ) {}
      tBGRA& operator=( tRGB const&  rgb ) {
         b = rgb.b();
         g = rgb.g();
         r = rgb.r();
         a = max_byte;
         return *this;
      }
      tBGRA& operator=( tBGRA const&  bgra ) {
         if( &bgra != this ) {
            b = bgra.b;
            g = bgra.g;
            r = bgra.r;
            a = bgra.a;
         }

         return *this;
      }
      bool operator==( tBGRA const&  v ) {
         if( b == v.b &&  g == v.g && r == v.r && a == v.a ) {
            return true;
         }

         return false;
      }
      bool operator!=( tBGRA const&  v ) {
         return !operator==( v );
      }
      operator tRGB()const {
         return tRGB( r, g, b );
      }
      operator tRGBA()const {
         return tRGBA( r, g, b, a );
      }
   };




}
#endif
//EOF
