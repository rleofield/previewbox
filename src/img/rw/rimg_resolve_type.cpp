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
/*! \file rimg_resolve_type.cpp
\brief find the type of an image by nmaguc numbers

Lib: librimgrw
*/


#include <iostream>
#include <sstream>
#include <string>


#include "boost_cstdint.h"

#include "rBin.h"
#include "rimg_resolve_image_type.h"

using namespace std;



namespace rlf {

   /*
    *https://en.wikipedia.org/wiki/List_of_file_signatures
   49 49 2A 00 (little endian format) or
   4D 4D 00 2A (big endian format)
         II*. or
         MM.*
         tif, tiff
         Tagged Image File Format

   FF D8 FF
         ÿØÿà
         jpg, jpeg
         JPEG

   89 50 4E 47
         .PNG
         png
         Image encoded in the Portable Network Graphics format
   42 4D
         BM
         bmp,
         BMP file, a bitmap format used mostly in the Windows world

   47 49 46 38 37 61
   47 49 46 38 39 61
         GIF87a
         GIF89a
         gif
         Image file encoded in the Graphics Interchange Format (GIF)
    */
   namespace rimg_rw {




      char const* IntelLogo = "II";
      char const* MacLogo = "MM";
      uint8_t jpeg0 = 0xFF;
      uint8_t jpeg1 = 0xD8;
      uint8_t jpeg2 = 0xFF;

      inline uint8_t const* toUint8Ptr( std::vector<uint8_t> &b ) {
         return &b[0];
      }



      e_file_type findtype( string fn, vector<uint8_t> &v ) {

         bin_read::t_bin_read reader;
         reader( fn, v );

         if( v.size() < 10 ) {
            return e_file_type::none;
         }


         // TIF
         char const* logo = bin_read::rhelper::toCharPtr( v );

         if( strncmp( static_cast<char const*>( logo ), MacLogo, 2 ) == 0
               || strncmp( static_cast<char const*>( logo ), IntelLogo, 2 ) == 0 ) {
            return e_file_type::tif;
         }

         // FF D8 FF
         // jpeg
         uint8_t j0 = *logo;
         uint8_t j1 = *( logo + 1 );
         uint8_t j2 = *( logo + 2 );

         if( j0 ==  jpeg0 && j1 ==  jpeg1 && j2 ==  jpeg2 ) {
            return e_file_type::jpeg;
         }

         // PNG
         // 89 50 4E 47
         if( strncmp( static_cast<char const*>( logo + 1 ), "PNG", 3 ) == 0 ) {
            return e_file_type::png;
         }

         // 42 4D == BM, 0x4d42
         // BMP
         if( strncmp( static_cast<char const*>( logo ), "BM", 2 ) == 0 ) {
            return e_file_type::bmp;
         }

         return e_file_type::none;
      }







   }
}
//EOF






