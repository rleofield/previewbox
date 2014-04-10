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

/*! \file rimg_linear.h
\brief linear arrays for pixel

Lib: librimgrw
*/



#ifndef imgrw_tImgH
#define imgrw_tImgH



#include <cstdint>
#include <exception>
#include <string>
#include <memory.h>
#include <vector>
#include <iterator>

#include <boost/lexical_cast.hpp>


#include "rimg_rgb_bgra_types.h"
#include "rimg_rw_exceptions.h"
#include "rimg_imgsize_type.h"




namespace rlf {


   namespace rimg_linear {

      // currently not used,
      class tImgRefCount {
         size_t _count;
      public:
         size_t hasReferences()const {
            return _count > 0;
         }
         size_t add() {
            _count++;
            return _count;
         }
         size_t release() {
            if( _count > 0 ) {
               _count--;
            }

            return _count;
         }
      };

      class PixelFormat {
         size_t _val;
      public:
         PixelFormat(): _val( 0 ) {}
         size_t& val() {
            return _val;
         }

         size_t const& val()const {
            return _val;
         }

         PixelFormat( size_t v ): _val( v ) {}

         PixelFormat& operator=( PixelFormat const& f ) {
            _val = f._val;
            return *this;
         }
         PixelFormat& operator=( size_t v ) {
            _val = v;
            return *this;
         }
         bool operator==( PixelFormat const& f )const {
            if( _val == f._val ) {
               return true;
            }

            return false;
         }
         bool operator!=( PixelFormat const& f )const {
            if( _val != f._val ) {
               return true;
            }

            return false;
         }
         bool operator==( size_t v )const {
            if( _val == v ) {
               return true;
            }

            return false;
         }
         string to_string()const {
            if( _val == none().val() ) {
               return "none";
            }

            if( _val == mono8().val() ) {
               return "mono8";
            }
            if( _val == palette().val() ) {
               return "palette";
            }

            if( _val == rgb().val() ) {
               return "rgb";
            }

            if( _val == rgba().val() ) {
               return "rgba";
            }

            return"unknown";
         }

         static PixelFormat none() {
            return 0;
         }
         static PixelFormat mono8() { // 1
            return sizeof( uint8_t );
         }
         static PixelFormat rgb() {
            return sizeof( tRGB ); // 3 = 24 bit
         }
         static PixelFormat rgba() {
            return sizeof( uint32_t );  // 4
         }
         static PixelFormat palette() { // 768
            return 3 * 256;
         }
         //      static PixelFormat mono16() {
         //        return sizeof( uint16_t );
         //      }
      };





      class tImgLinear {
      private:
         uint8_t*    _data;
         string _description; // TIFF only
         bool  _is_allocated;
         vector<uint8_t*> _row_pointer;
         vector<tRGBA> _palette;

         PixelFormat  _ImgFormat;

         tSize  _size;
      public:
         tImgLinear():
            _data( nullptr ),
            _description(),
            _is_allocated( false ),
            _row_pointer(),
            _palette( 0x100 ),
            _ImgFormat(),
            _size()
         {}
         ~tImgLinear() {
            free_data_buffer() ;
         }
         tSize size()const {
            return _size;
         }
         tSize& size() {
            return _size;
         }
         tImgLinear( tImgLinear const& imgIn );
         tImgLinear& operator=( tImgLinear const& imgIn ) ;
         vector<tRGBA> const& palette()const{ return    _palette; }
         void palette(vector<tRGBA> const& p){ _palette = p; }


         uint8_t    const* row_ptr( size_t y )const {
            if( y >= _row_pointer.size() ) {
               throw tImgIndexEx( "index out of range, is: "
                                  + boost::lexical_cast<string>( y )
                                  + "' max: "
                                  + boost::lexical_cast<string>( size().y() ) );
            }

            return _row_pointer[ y ];
         }
         uint8_t* row_ptr( size_t y ) {
            return const_cast<uint8_t*>( static_cast<tImgLinear const&>( *this ).row_ptr( y ) );
         }
         uint32_t pitch()const;

         uint32_t bytes()const {
            return size().y() * pitch();
         }

         string description()const {
            return _description;
         }
         void description( string const& d ) {
            _description = d;
         }
         void clear( uint8_t val = 0 ) {
            if( _data != 0 ) {
               memset( _data, val , bytes() );
            }
         }
         uint8_t const* data_ptr()const {
            return  get_data();
         }
         uint8_t* data_ptr() {
            return  const_cast<uint8_t*>( static_cast<tImgLinear const&>( *this ).data_ptr() );
         }
         void insert_data( vector<uint8_t> const& p, uint32_t offset );
         void insert_data( uint8_t const* p, uint32_t s, uint32_t offset );

         bool is_none()const {
            return ( _ImgFormat == PixelFormat::none() );
         }
         bool is_mono8()const {
            return ( _ImgFormat == PixelFormat::mono8() );
         }
         bool is_palette()const {
            return ( _ImgFormat == PixelFormat::palette() );
         }
         //      bool is_mono16()const {
         //        return ( _ImgFormat == PixelFormat::mono16() );
         //      }
         bool is_rgb()const {
            return ( _ImgFormat == PixelFormat::rgb() );
         }
         bool is_rgba()const {
            return ( _ImgFormat == PixelFormat::rgba() );
         }
         void set_mono8() {
            _ImgFormat = PixelFormat::mono8();
         }
         void set_palette() {
            _ImgFormat = PixelFormat::palette();
         }
         void set_rgb() {
            _ImgFormat = PixelFormat::rgb();
         }
         void set_rgba() {
            _ImgFormat = PixelFormat::rgba();
         }
         //      void set_mono16() {
         //        _ImgFormat = PixelFormat::mono16();
         //      }
         vector<uint8_t*> const& rows()const {
            return _row_pointer;
         }

         void  set_extern_data_pointer( uint8_t* p ) {
            free_data_buffer();
            _is_allocated = false;
            set_data( p );
         }

         void alloc_data_buffer();
      protected:
         uint8_t    const* get_data()const;
         uint8_t*     get_data();
         void    set_data( uint8_t* p ) {
            _data = p;

            if( _data != 0 ) {
               set_row_pointer();
            } else {
               _row_pointer.clear();
            }
         }
      private:
         void free_data_buffer() ;
         void set_row_pointer() {
            size_t p = pitch();
            uint8_t* ptr = _data;

            size_t count = size().h();
            _row_pointer.resize( count );
            size_t i = 0;

            while( i < count ) {
               _row_pointer[i] = ptr;
               ptr += p;
               i++;
            }
         }


      };

      class tImgViewLinear {
         tImgViewLinear( tImgViewLinear const& img ) ;
         void operator=( tImgViewLinear const& img ) ;
         tImgLinear const& _source;
      public:
         tImgViewLinear( tImgLinear const& source ): _source( source ) {}
         ~tImgViewLinear() {}
         string description()const {
            return _source.description();
         }

         uint8_t const* data_ptr()const {
            return  _source.data_ptr();
         }
         tSize  size()const {
            return _source.size();
         }
         uint32_t pitch()const {
            return _source.pitch();
         }
         uint32_t bytes()const {
            return _source.bytes();
         }
         uint8_t const* row_ptr( size_t y )const {
            return _source.row_ptr( y );
         }
         vector<tRGBA> const& palette()const{ return    _source.palette(); }
         bool is_mono8()const {
            return _source.is_mono8();
         }
         bool is_palette()const {
            return _source.is_palette();
         }
         bool is_rgb()const {
            return _source.is_rgb();
         }
         bool is_rgba()const {
            return _source.is_rgba();
         }
         vector<uint8_t*> const& rows()const {
            return _source.rows();
         }

      };




      void RGB_LineTo_BGR( tImgViewLinear const& img, size_t y, vector<uint8_t>& v );

   } // end of ns img

}


#endif
//EOF
