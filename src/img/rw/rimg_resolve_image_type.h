
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

/*! \file rimg_resolve_type.h
\brief read write images, BMP,PNG,JPEG,TIFF, linear and planar

Lib: librimgrw
*/



#ifndef RIMG_rimg_resolve_type_H
#define RIMG_rimg_resolve_type_H

#include <string>

using namespace std;

namespace rlf {

   namespace rimg_rw {

enum class e_file_type: size_t {
         none,
         tif,
         bmp,
         png,
         jpeg
      };

      const string dot_tif = ".tif";
      const string dot_tiff = ".tiff";
      const string dot_jpeg = ".jpeg";
      const string dot_jpg = ".jpg";
      const string dot_png = ".png";
      const string dot_bmp = ".bmp";
      //      const string tif = "tif";
      //      const string tiff = "tiff";
      //      const string jpeg = "jpeg";
      //      const string jpg = "jpg";
      //      const string png = "png";
      //      const string bmp = "bmp";

      e_file_type findtype( string fn, vector<uint8_t> &v ) ;


   } // end of ns img_rw
} // end of ns rlf
#endif
//EOF



