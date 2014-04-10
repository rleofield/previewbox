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

/*! \file rimg_planar.cpp
\brief impl for planar images

Lib: librimgrw
*/


#include <memory.h>
#include <assert.h>
#include <algorithm>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>

#include "rimg_planar.h"
//#include "rimg_xy_types.h"
//#include "rimg_aoi.h"

using namespace std;
using namespace boost::assign;

namespace rlf {

   bool tMonoColor::operator==( const tMonoColor& m )const {
      return m._c == _c;
   }

   bool tMonoColor::operator!=( const tMonoColor& s )const {
      return !( s == *this );
   }
   tMonoColor& tMonoColor::operator=( const tMonoColor& m ) {
      if( this != &m ) {
         _c = m._c;
      }

      return *this;
   }
   // value of color
   uint8_t tMonoColor::val()const {
      return _c;
   }

   tMonoColor::operator vcolors()const {
      vcolors v( rimg_planar::tMask::size() );
      auto start = v.begin();
      auto running = v.begin();
      auto end   = v.end();

      while( running != end ) {
         size_t diff = running  - start;
         uint32_t m = 1 << diff;
         *running = 0;

         uint32_t v1 = rimg_planar::tMask::mono.val();
         uint32_t mm = m;
         uint32_t b = v1 & m;

         if( rimg_planar::tMask::mono.val() & m ) {
            *running  = _c;
         }

         ++running;
      }

      return v;
   }



   tRGB::operator vcolors()const {
      vcolors v( rimg_planar::tMask::size() );
      auto begin = v.begin();
      auto e = v.end();

      while( begin != e ) {
         size_t diff = e - begin;
         uint32_t m = 1 << diff;

         * begin = 0;

         if( rimg_planar::tMask::r.val() & m ) {
            * begin = _r;
         }

         if( rimg_planar::tMask::g.val() & m ) {
            * begin = _g;
         }

         if( rimg_planar::tMask::b.val() & m ) {
            * begin = _b;
         }

         ++begin;
      }

      return v;
   }

   tRGBA::operator vcolors()const {
      vcolors v( rimg_planar::tMask::size() );
      auto begin = v.begin();
      auto e = v.end();

      while( begin != e ) {
         size_t diff = e - begin;
         uint32_t m = 1 << diff;

         * begin = 0;

         if( rimg_planar::tMask::r.val() & m ) {
            * begin = _r;
         }

         if( rimg_planar::tMask::g.val() & m ) {
            * begin = _g;
         }

         if( rimg_planar::tMask::b.val() & m ) {
            * begin = _b;
         }

         if( rimg_planar::tMask::a.val() & m ) {
            * begin = _a;
         }

         ++begin;
      }

      return v;
   }

   namespace rimg_planar {

      tMask tMask::ch0 = tMask( 0x01 ); //0b00000001;
      tMask tMask::ch1 = tMask( 0x02 ); //0b00000010;
      tMask tMask::ch2 = tMask( 0x04 ); //0b00000100;
      tMask tMask::ch3 = tMask( 0x08 ); //0b00001000;
      tMask tMask::ch4 = tMask( 0x10 ); //0b00010000;
      tMask tMask::ch5 = tMask( 0x20 ); //b000100000;
      tMask tMask::ch6 = tMask( 0x40 ); //0b01000000;
      tMask tMask::ch7 = tMask( 0x80 ); //0b10000000;

      tMask tMask::empty = tMask();
      tMask tMask::mono = ch0;

      tMask tMask::r = ch1;
      tMask tMask::g = ch2;
      tMask tMask::b = ch3;
      tMask tMask::a = ch4;
      tMask tMask::rgb = tMask( r._mask | g._mask | b._mask );
      tMask tMask::rgba = tMask( r._mask | g._mask | b._mask | a._mask );

      tMask tMask::c = ch5;
      tMask tMask::m = ch6;
      tMask tMask::y = ch7;
      tMask tMask::k = ch1; // ==mono


      size_t tMask::pos_mono = 0;  // gray channel
      size_t tMask::pos_r = 1;  // red channel
      size_t tMask::pos_g = 2;  // green channel
      size_t tMask::pos_b = 3;  // blue channel
      size_t tMask::pos_a = 4;  // alpha channel



      uint32_t tMask::max = 8;

      namespace {
         // kernighan
         uint32_t countSetBits( uint32_t n ) {
            uint32_t count = 0;

            while( n ) {
               n &= ( n - 1 ) ;
               count++;
            }

            return count;
         }
      }

      size_t tMask::count()const {
         // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
         uint32_t v = _mask; // count bits set in this (32-bit value)
         uint32_t c = 0;
         c = v - ( ( v >> 1 ) & 0x55555555 );
         c = ( ( c >> 2 ) & 0x33333333 ) + ( c & 0x33333333 );
         c = ( ( c >> 4 ) + c ) & 0x0F0F0F0F;
         c = ( ( c >> 8 ) + c ) & 0x00FF00FF;
         c = ( ( c >> 16 ) + c ) & 0x0000FFFF;
         return c;

      }
      size_t tMask::pos_of_bit()const {
         assert( count() == 1 );

         if( _mask & ch0._mask ) {
            return 0;
         }

         if( _mask & ch1._mask ) {
            return 1;
         }

         if( _mask & ch2._mask ) {
            return 2;
         }

         if( _mask & ch3._mask ) {
            return 3;
         }

         if( _mask & ch4._mask ) {
            return 4;
         }

         if( _mask & ch5._mask ) {
            return 5;
         }

         if( _mask & ch6._mask ) {
            return 6;
         }

         if( _mask & ch7._mask ) {
            return 7;
         }

         // fallback is ch0
         return 0;
      }


      bool tMask::is_rgb()const {
         //uint32_t m = (_mask & rgb );
         return ( _mask & rgb._mask ) == rgb._mask;
      }
      bool tMask::is_mono()const {
         return ( _mask & mono._mask ) == mono._mask;
      }




      tImgPlanar tImgPlanar::emtptyImg;


      tPlane8& tImgPlanar::plane( tMask const& mask_ ) {
         size_t c = mask_.count();
         assert( c == 1 );
         assert( mask_ & _mask );
         size_t pos = mask_.pos_of_bit();
         return _m[pos].plane;
      }
      tPlane8 const& tImgPlanar::plane( tMask const& mask_ )const {
         size_t c = mask_.count();
         assert( c == 1 );
        // assert( mask_ & _mask );
         size_t pos = mask_.pos_of_bit();
         return _m[pos].plane;
      }

      tPlane8& tImgPlanar::mono8() {
         return plane( tMask::mono );
      }
      tPlane8& tImgPlanar::red() {
         return plane( tMask::r );
      }
      tPlane8& tImgPlanar::green() {
         return plane( tMask::g );
      }
      tPlane8& tImgPlanar::blue() {
         return plane( tMask::b );
      }
      tPlane8& tImgPlanar::alpha() {
         return plane( tMask::a );
      }


      tPlane8 const& tImgPlanar::mono8()const {
         return plane( tMask::mono );
      }
      tPlane8 const& tImgPlanar::red()const {
         return plane( tMask::r );
      }
      tPlane8 const& tImgPlanar::green()const {
         return plane( tMask::g );
      }
      tPlane8 const& tImgPlanar::blue()const {
         return plane( tMask::b );
      }
      tPlane8 const& tImgPlanar::alpha()const {
         return plane( tMask::a );
      }

      bool tImgPlanar::has_data()const {
         if( _mask.count() > 0 ) {
            return true;
         }

         return false;
      }

      size_t tImgPlanar::sx()      const {
         return size().x();
      }
      size_t tImgPlanar::aligned_sx()      const {
         return align32(size().x());
      }


      size_t tImgPlanar::sy()      const {
         return size().y();
      }


      size_t tImgPlanar::pixels()  const {
         return sx() * sy();
      }
      size_t tImgPlanar::bytes()  const {
         return align32( sx() ) * sy();
      }



      bool tImgPlanar::operator!=( const tImgPlanar& s )const {
         if( s == *this ) {
            return false;
         }

         return true;
      }

      bool tImgPlanar::operator==( const tImgPlanar& s )const {
         if( _size != s._size ) {
            return false;
         }

         if( _description != s._description ) {
            return false;
         }

         if( s._mask != _mask ) {
            return false;
         }

         if( _m != s._m ) {
            return false;
         }

         return true;
      }
      bool tImgPlanar::equals_size_mask( const tImgViewPlanar& s )const {
         if( size() == s.size() && mask() == s.mask() ) {
            return true;
         }

         return false;
      }

      tImgPlanar& tImgPlanar::operator=( const tImgPlanar& in ) {
         if( this != &in ) {
            _mask = in._mask;
            _size = in._size;
            _description = in._description;
            _m = in._m;
            //_m16 = in._m16;
         }

         return *this;
      }


      tImgPlanar::tImgPlanar( const tImgPlanar& in ):
         _mask( in._mask ),
         _size( in._size ),
         _m( in._m ),
         _description( in._description )
         //_m16 = in._m16;
      {}

      tImgPlanar::tImgPlanar():
         _mask( 0 ),
         _size(),
         //_m16(),
         _m( tMask::max ),
         _description() {
         _description = "";
      }


      tImgPlanar::tImgPlanar( tSize const& s, tMask const& mask_ ):
         //_m16(),
         _mask( mask_ ),
         _size( s ),
         _m( tMask::max ),
         _description()

      {
         uint32_t aligned_x   = s.x(); //align32( s.x() );

         auto mbegin = begin();
         auto mend = end();

         while( mbegin != mend ) {
            if( this->has_bit_at( mbegin ) ) {
               mbegin->plane.resize( s.y(), vLine8( aligned_x ) );
            }

            ++mbegin;
         }


      }


      tImgPlanar tImgPlanar::create( tSize const& s, tMask const& mask_ ) {
         return tImgPlanar( s, mask_ );
      }
      void tImgPlanar::realloc( uint32_xy const& s, tMask const& mask_ ) {
         if( ( _mask == mask_ ) ) {
            if( size() == s ) {
               return; // do nothing
            }
         }

         _mask = mask_;
         _size = s;

         uint32_t aligned_x   = s.x(); //align32( s.x() );
         auto mbegin = begin();
         auto mend = end();

         while( mbegin != mend ) {
            if( this->has_bit_at( mbegin ) ) {
               mbegin->plane.assign( s.y(), vLine8( aligned_x ) );
            } else {
               mbegin->plane.clear(); // plane.assign( s.y(), tLine8( aligned_x ) );
               mbegin->plane.shrink_to_fit();
            }

            ++mbegin;
         }
      }

      bool tImgPlanar::has_bit_at( tCube8::const_iterator const& i )const {
         size_t diff = i - begin();
         uint32_t m = 1 << diff;

         if( _mask._mask & m ) {
            return true;
         }

         return false;
      }


      tSize tImgPlanar::size()const {
         return _size;
      }
      //      bool tImgPlanar::voxelIsInCube( uint32_xyz xyz )  const {

      //         auto z_ = xyz.z();
      //         auto x_ = xyz.x();
      //         auto y_ = xyz.y();

      //         if( x_ >= sx() ||  y_ >= sy() || z_ >= begin()->size() ) {
      //            return false;
      //         }

      //         return true;
      //      }


      bool tImgPlanar::pixelIsInImage( uint32_xy xy )const {
         auto x_ = xy.x();
         auto y_ = xy.y();

         if( x_ >= sx() ||  y_ >= sy() ) {
            return false;
         }

         return true;
      }


      void tImgPlanar::pixel( uint32_xy xy_, uint32_t val ) {
         assert( is_mono8() );

         if( !pixelIsInImage( xy_ ) ) {
            return;
         }

         auto x = xy_.x();
         auto y = xy_.y();
         ( *this )[0][ y ][x] = ( uint8_t )val;
      }
      void tImgPlanar::pixel( int x, int y , int val ) {
         pixel( uint32_xy( x, y ), val );
      }



      uint32_t tImgPlanar::pixel( uint32_xy xy_ )const {
         assert( is_mono8() );
         assert( pixelIsInImage( xy_ ) );
         size_t x = xy_.x();
         size_t y = xy_.y();
         return ( *this )[tMask::pos_mono][ y ][x];
      }
      uint32_t tImgPlanar::pixel( int x, int y )const {
         return pixel( uint32_xy( x, y ) );
      }



      void tImgPlanar::pixel( uint32_xy xy_, tRGB val ) {
         assert( is_rgb() );
         assert( pixelIsInImage( xy_ ) );
         size_t x = xy_.x();
         size_t y = xy_.y();
         red()[ y ][x]   = val.r();
         green()[ y ][x] = val.g();
         blue()[ y ][x]  = val.b();
      }



      tRGB tImgPlanar::RGBpixel( uint32_xy xy_ )const {
         assert( pixelIsInImage( xy_ ) );
         assert( is_rgb() );
         tRGB ret;
         size_t x = xy_.x();
         size_t y = xy_.y();
         ret.r() = red()[ y ][x];
         ret.g() = green()[ y ][x];
         ret.b() = blue()[ y ][x];
         return ret;
      }

      void set_color( tPlane8& plane, uint8_t color ) {

         auto mbegin = plane.begin();
         auto mend = plane.end();
         size_t x = plane[0].size();

         while( mbegin != mend ) {
            mbegin->assign( x, color );
            mbegin++;
         }
      }

      void tImgPlanar::setColor( vcolors colors ) {

         assert( colors.size() == tMask::size() );
         auto mbegin = begin();
         auto mend = end();

         while( mbegin != mend ) {
            if( this->has_bit_at( mbegin ) ) {
               set_color( mbegin->plane, colors[ 0 ] );
            }

            ++mbegin;
         }

      }



      namespace {
         // test, if an integer is even
         bool  is_even( int x ) {
            return ( x % 2 == 0 ) ?  true : false ;
         }
      }


      size_t Get12( uint8_t const* b, int i ) {
         uint16_t a = 0;
         int index = 0;

         if( is_even( i ) ) {
            index = i;
            index *= 3;
            index /= 2;
            memcpy( &a, b + index, 2 );
            a &= 0xfff;
            return a;
         }

         index  = i - 1;
         index *= 3;
         index /= 2;
         index += 1;
         memcpy( &a, b + index, 2 );
         a >>= 4;
         a &= 0xfff;
         return a;
      }



      bool tImgPlanar::is_mono8()const {
         return mask() == tMask::mono;
         //&&
         //_m16.size() == 0;
      }


      bool tImgPlanar::is_rgb()  const {
         return mask() == tMask::rgb;
      }

      bool tImgPlanar::is_rgba()  const {
         return mask() == tMask::rgba;
      }
      //bool tImgPlanar::is_mono16()  const {
      //    return
      //            _m16.size() == channel::size_mono8
      //            &&
      //            _m.size() == 0;
      //}




      string tImgPlanar::TypeString()const {
         if( is_mono8() ) {
            return "8 bit gray image";
         }

         //    if( is_mono16() ) {
         //        return "16 bit gray image";
         //    }

         if( is_rgb() ) {
            return "24 bit RGB image";
         }

         if( is_rgba() ) {
            return "32 bit RGBA image";
         }

         return "unknown";
      }


      string tImgPlanar::description()const {
         return _description;
      }

      void tImgPlanar::description( string const& d ) {
         _description = d;
      }



   } // end of ns planar



} //end of nd rlf

//EOF

