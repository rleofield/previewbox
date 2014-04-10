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
/*! \file rimg_linear.cpp
\brief impl for linear images

Lib: librimgrw
*/

#include <iostream>
#include <sstream>
#include <string>


using namespace std;

#include "rimg_linear.h"
#include "rimg_alloc_raw_data.h"


namespace rlf {


   namespace rimg_linear {


      void  tImgLinear::alloc_data_buffer() {
         if( _data != nullptr ) {
            free_data_buffer();
         }

         size_t s = size().y() * pitch();
         uint8_t* p  = static_cast<uint8_t*>( rlf::rimg_alloc_raw_data::alloc_raw_data( s ) );
         //  t_Img_template<uint8_t>::set_data((uint8_t *) GlobalAlloc(GMEM_FIXED, s) );
         set_data( p );
         _is_allocated = true;
      }


      void tImgLinear::free_data_buffer() {
         if( _is_allocated ) {
            uint8_t* p = get_data();
            rlf::rimg_alloc_raw_data::free_raw_data( p );
            _data = nullptr;
            _is_allocated = false;
         }

         set_data( 0 );
      }


      uint32_t tImgLinear::pitch()const {
         if( _ImgFormat == PixelFormat::mono8() ) {
            return rlf::rimg_alloc_raw_data::align32( size().x() );
         }
         if( _ImgFormat == PixelFormat::palette() ) {
            return rlf::rimg_alloc_raw_data::align32( size().x() );
         }

         if( _ImgFormat == PixelFormat::rgb() ) {
            return sizeof( tRGB ) * size().x();
         }

         if( _ImgFormat == PixelFormat::rgba() ) {
            return sizeof( tRGBA ) * size().x();
         }

         //        if( _ImgFormat == PixelFormat::mono16() ) {
         //          return align32(sizeof( uint16_t ) * size().x());
         //        }

         throw rImgEx( "unknown pitch, format is: " + _ImgFormat.to_string() );
      }


      uint8_t    const* tImgLinear::get_data()const {
         return _data;
      }
      uint8_t*     tImgLinear::get_data() {
         return _data;
      }

      void tImgLinear::insert_data( vector<uint8_t> const& p, uint32_t offset ) {
         if( p.size() > 0 && p.size() + offset <= bytes() && _is_allocated ) {
            uint8_t* pdata = _data + offset;
            memcpy( pdata, &p[0], p.size() );
         }

      }


      void tImgLinear::insert_data( uint8_t const* p, uint32_t s, uint32_t offset ) {
         if( s > 0 && s + offset <= bytes() && _is_allocated ) {
            uint8_t* pdata = _data + offset;
            memcpy( pdata, p, s );
         }

      }

      tImgLinear::tImgLinear( tImgLinear const& imgIn ):
         _data( nullptr ),
         _description( imgIn._description ),
         _is_allocated( false ),
         _row_pointer(),
         _palette(imgIn._palette),
         _ImgFormat(),
         _size() {
         _size = imgIn._size;
         _ImgFormat = imgIn._ImgFormat;
         alloc_data_buffer();
         insert_data( imgIn.get_data(), bytes(), 0 );
      }

      tImgLinear& tImgLinear::operator=( tImgLinear const& imgIn ) {
         if( this != & imgIn ) {
            _description = imgIn._description;
            _palette = imgIn._palette;
            _size = imgIn._size;
            _ImgFormat = imgIn._ImgFormat;
            alloc_data_buffer();
            insert_data( imgIn.get_data(), bytes(), 0 );
         }

         return *this;

      }


      void RGB_LineTo_BGR( tImgViewLinear const& img, size_t y, vector<uint8_t>& v ) {
         tRGB const* source = ( tRGB const* )( img.row_ptr( y ) );
         v.resize( img.pitch() );
         tBGR* target = ( tBGR* )&v[0];
         size_t count = img.size().x();

         while( count-- ) {
            *target++ = *source++;
         }
      }


   } // end of ns img

}
//EOF
