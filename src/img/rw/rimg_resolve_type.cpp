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
#include "rimg_t_read.h"

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




      const string IntelLogo = "II";
      const string MacLogo = "MM";
      vector<uint8_t> vjpeg = {0xff,0xd8,0xff};

      inline uint8_t const* toUint8Ptr( std::vector<uint8_t> &b ) {
         return &b[0];
      }



      e_file_type findtype( string fn ) {

         vector<uint8_t> v;
         bin_read::t_bin_read reader;
         reader( fn, v, 10 );

         if( v.size() < 10 ) {
            return e_file_type::none;
         }


         // TIF
         char const* logo = bin_read::rhelper::toCharPtr( v );
         string l2(v.begin(), v.begin() + 2);
         if( l2 == MacLogo  || l2 == IntelLogo ) {
            return e_file_type::tif;
         }

         // FF D8 FF
         // jpeg
         vector<uint8_t> l3(v.begin(), v.begin() + 3);
         if( vjpeg ==  l3 ) {
            return e_file_type::jpeg;
         }

         // PNG
         // 89 50 4E 47
         string sl3(l3.begin(),l3.end());
         if( sl3 == "PNG" ) {
            return e_file_type::png;
         }

         // 42 4D == BM, 0x4d42
         // BMP
         string sl2 (l2.begin(),l2.end());
         if( sl2 == "BM" ) {
            return e_file_type::bmp;
         }

         return e_file_type::none;
      }







   }
}
//EOF






