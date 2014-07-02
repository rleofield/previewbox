/*! \file rimg_planar.h
\brief planar images

Lib: librimgrw
*/

/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
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




#ifndef RIMG_tImgPlanarH
#define RIMG_tImgPlanarH


#include <cstdint>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "rimg_planar_typedefs.h"
#include "rimg_rgb_bgra_types.h"
#include "rimg_rw_exceptions.h"
#include "rimg_imgsize_type.h"
#include "boost_cstdint.h"

using std::vector;
using std::string;



namespace rlf {


   namespace rimg_planar {





      //   template<typename T>
      //     class tLineT: public std::vector<T> {
      //     public:
      //        typedef typename vector<T>::const_iterator T_const_iterator;
      //        typedef typename vector<T>::iterator T_iterator;

      //        tLineT() {}
      //       // tLineT( tSize const& s ): std::vector<T>( s.y(), T( s.x() ) ) {}
      //        tLineT( T const& s ): std::vector<T>( s ) {}
      ////        uint32_t size()const {
      ////           return std::vector<T>::size();
      ////        }
      ////        T const& operator[]( uint32_t i )const {
      ////           return std::vector<T>::operator[](i);
      ////        }
      ////        T& operator[]( uint32_t i ) {
      ////           return std::vector<T>::operator[](i);
      ////        }

      ////        T_const_iterator begin()const {
      ////           return std::vector<T>::begin();
      ////        }
      ////        T_const_iterator end()const {
      ////           return std::vector<T>::end();
      ////        }
      ////        T_iterator begin() {
      ////           return std::vector<T>::begin();
      ////        }
      ////        T_iterator end() {
      ////           return std::vector<T>::end();
      ////        }



      ////        bool operator==( tLineT const& p )const {
      ////            return  std::vector<T>::operator==( p );
      ////        }

      //        static size_t pos_(tLineT<T> l, T_iterator i){
      //            size_t p = i - l.begin();
      //            return p;
      //        }



      //     };





      template<typename T>
      class tPlaneT {
      public:
         typedef typename vector<T>::const_iterator T_const_iterator;
         typedef typename vector<T>::iterator T_iterator;

         tPlaneT(): plane() {}
         tPlaneT( tSize const& s ): plane( s.y(), T( s.x() ) ) {}
         vector< T > plane;
         uint32_t size()const {
            return plane.size();
         }
         T const& operator[]( uint32_t i )const {
            return plane[i];
         }
         T& operator[]( uint32_t i ) {
            return plane[i];
         }

         T_const_iterator begin()const {
            return plane.begin();
         }
         T_const_iterator end()const {
            return plane.end();
         }
         T_iterator begin() {
            return plane.begin();
         }
         T_iterator end() {
            return plane.end();
         }



         bool operator==( tPlaneT const& p )const {
            if( plane == p.plane ) {
               return true;
            }

            return false;
         }

         size_t pos( T_iterator i ) {
            size_t p = i - plane.begin();
            return p;
         }


      };




      typedef tPlaneT<vLine8> tPlane8m;
      typedef tPlaneT<uint16_t> tPlane16m;
      typedef tPlaneT<uint32_t> tPlane32m;

      typedef  vector< tPlane8m > tCube8;



      // see http://de.wikipedia.org/wiki/Bitmaske
      class tMask {
         uint32_t _mask;

         // count must be 1
				 uint32_t pos_of_bit()const;


         tMask(): _mask( 0 ) {}

         bool operator==( const tMask& m )const {
            return m._mask == _mask;
         }

         bool operator!=( const tMask& s )const {
            return !( s == *this );
         }
         tMask& operator=( const tMask& m ) {
            if( this != &m ) {
               _mask = m._mask;
            }

            return *this;
         }
         bool operator&( const tMask& m )const {
            if( ( m.val() & _mask ) > 0 ) {
               return true;
            }

            return false;
         }


         static tMask ch0;
         static tMask ch1;
         static tMask ch2;
         static tMask ch3;
         static tMask ch4;
         static tMask ch5;
         static tMask ch6;
         static tMask ch7;

         static tMask c;
         static tMask m;
         static tMask y;
         static tMask k;

         static uint32_t max;
         bool is_rgb()const;
         bool is_mono()const ;

      public:

         static tMask empty;  // gray channel
         static tMask mono;  // gray channel
         static tMask r;  // red channel
         static tMask g;  // green channel
         static tMask b;  // blue channel
         static tMask a;  // alpha channel
         static tMask rgb;  // red & green & blue  channels
         static tMask rgba; // red & green && blue && alpha channels

         explicit tMask( uint32_t mask_ ): _mask( mask_ ) {}

         uint32_t val()const {
            return _mask;
         }
				 uint32_t count()const;
         static uint32_t size() {
            return max;
         }

				 static uint32_t pos_mono;  // gray channel
				 static uint32_t pos_r;  // red channel
				 static uint32_t pos_g;  // green channel
				 static uint32_t pos_b;  // blue channel
				 static uint32_t pos_a;  // alpha channel

         friend class tImgPlanar;

      };




      class tImgViewPlanar;



      class tImgPlanar {


      private:

         rlf::rimg_planar::tMask  _mask;
         uint32_xy _size;
         tCube8 _m;
         //tCube16 _m16;
         string _description;


         void operator=( uint32_t val );

         tPlane8& plane( tMask const& mask );
         tPlane8 const& plane( tMask const& mask )const;

      public:
         tImgPlanar();

         tImgPlanar( tSize const& s, tMask const& mask = tMask::mono );


         tImgPlanar( tImgPlanar const& in_ );
         ~tImgPlanar() {}

         tImgPlanar& operator=( tImgPlanar const& in ) ;


         string name;
         string fullname;

         // compares size, type, and pixel content
         bool operator!=( const tImgPlanar& s )const ;
         bool operator==( const tImgPlanar& s )const ;

         // compares only size and type, not pixel content
         bool equals_size_mask( const tImgViewPlanar& s ) const;

         // Size of image in x,y pixels
         tSize size()const;
         bool has_data()const;
				 uint32_t sx()      const;
				 uint32_t aligned_sx()      const;
				 uint32_t sy()      const;
				 uint32_t pixels()  const; // sx * sy
				 uint32_t bytes()  const;  // pitch * sy
         tMask mask()  const {
            return _mask;
         }
         uint32_t planes()const {
            return static_cast<uint32_t>( _m.size() );
         }
         bool has_bit_at( tCube8::const_iterator const& i )const;



         // "8 bit gray image";
         // "24 bit RGB image";
         // "16 bit gray image", not used
         string TypeString()const;


         // creates a new image, copies content
         static tImgPlanar create( tSize const& s, tMask const& mask );

         // reallocs, lose content
         void realloc( uint32_xy const& s, tMask const& mask );


         tCube8::const_iterator begin()const {
            return _m.begin();
         }
         tCube8::const_iterator end()const {
            return _m.end();
         }
         tCube8::iterator begin() {
            return _m.begin();
         }
         tCube8::iterator end() {
            return _m.end();
         }
         tCube8 const& data()const {
            return _m;
         }
         tCube8& data() {
            return _m;
         }
         tPlane8 const& operator[]( uint32_t i )const {
            return _m[i].plane;
         }
         tPlane8& operator[]( uint32_t i ) {
            return _m[i].plane;
         }


         // get the plane of certain channels
         tPlane8& mono8();
         tPlane8& red();
         tPlane8& green();
         tPlane8& blue();
         tPlane8& alpha();

         tPlane8 const& mono8()const;
         tPlane8 const& red()const;
         tPlane8 const& green()const;
         tPlane8 const& blue()const;
         tPlane8 const& alpha()const;

         //      vector< vector<uint16_t> > & mono16() {
         //        return  _m16[channel_offset::mono16];
         //      }


         //      vector< vector<uint16_t> > const& mono16()const {
         //        return _m16[channel_offset::mono16];
         //      }


         // tests
         //bool voxelIsInCube( uint32_xyz xyz )  const;
         bool pixelIsInImage( uint32_xy xy )  const;

         // one pixel, mono only, from first plane
         void pixel( uint32_xy xy, uint32_t val );

				 void pixel(uint32_t x, uint32_t y, uint32_t val);
         void pixel( uint32_xy xy, tRGB val );

         uint32_t pixel( uint32_xy xy )const;
				 uint32_t pixel(uint32_t x, uint32_t y)const;

         // pixel RGB,
         tRGB RGBpixel( uint32_xy xy )const;



         bool is_mono8()const ; // 1 plane,  0x01
         bool is_rgb()const ;   // 3 planes, 0x0e
         bool is_rgba()const ;  // 4 planes, 0x1e
         //bool is_mono16()const;

         void setColor( vcolors colors );

         string description()const ;
         void description( string const& d ) ;

         static tImgPlanar emtptyImg;
      };


      class tImgViewPlanar {
         tImgViewPlanar( tImgViewPlanar const& img ) ;
         void operator=( tImgViewPlanar const& img ) ;
         tImgPlanar const& _source;
      public:
         tImgViewPlanar( tImgPlanar const& source ): _source( source ) {}
         ~tImgViewPlanar() {}
         string description()const {
            return _source.description();
         }

         // Size of image in x,y pixels
         tSize size()const {
            return _source.size();
         }
         bool has_data()const {
            return _source.has_data();
         }
				 uint32_t sx()     const {
            return _source.sx();
         }
				 uint32_t sy()       const {
            return _source.sy();
         }

         tMask mask()  const {
            return _source.mask();
         }
         bool has_bit_at( tCube8::const_iterator const& i )const {
            return _source.has_bit_at( i );
         }

         tCube8::const_iterator begin()const {
            return _source.begin();
         }
         tCube8::const_iterator end()const {
            return _source.end();
         }

         bool is_mono8()const {
            return _source.is_mono8();   // 1 plane 0x01
         }
         bool is_rgb()const {
            return _source.is_rgb();   // 3 planes, 0x0e
         }
         bool is_rgba()const {
            return _source.is_rgba();  // 4 planes, 0x1e
         }
      };



   } // end of ns img

}



#endif
//EOF

