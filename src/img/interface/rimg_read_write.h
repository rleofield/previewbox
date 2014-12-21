
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

/*! \file rimg_read_write.h
\brief read write images, BMP,PNG,JPEG,TIFF, linear and planar

Lib: librimgrw
*/



#ifndef RIMG_read_write_H
#define RIMG_read_write_H

#include <string>
#include <boost/shared_ptr.hpp>


namespace rlf {
   namespace rimg_planar {
      class tImgPlanar;
   }
   namespace rimg_linear {
      class tImgLinear;
      class tImgViewLinear;
   }
}

using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgViewLinear;

namespace rlf {

   namespace png_compression {
      const size_t MIN_COMPRESSION_LEVEL        = 0;
      const size_t MAX_COMPRESSION_LEVEL        = 9;
      const size_t FILE_COMPRESSION_DEFAULT     = 3;
   }

   namespace jpeg_compression {
      const size_t JPEG_QUALITY_DEFAULT = 60; // 0..100
   }

   namespace rimg_rw {


      void read( std::string fn, tImgLinear& img )  ;
      void read( std::string fn, tImgPlanar& planar )  ;

      void write( std::string const& fn,
                  tImgViewLinear const& img,
                  bool override_,
                  size_t c = -1 );
      void write( std::string const& fn,
                  tImgPlanar const& img,
                  bool override_,
                  size_t c = -1 );


      void write_png( string const& fn,
                      tImgViewLinear const& img,
                      bool override_,
                      size_t c = png_compression::FILE_COMPRESSION_DEFAULT );
      void write_jpeg( string const& fn,
                       tImgViewLinear const& img,
                       bool override,
                       size_t c = jpeg_compression::JPEG_QUALITY_DEFAULT );

      void write_png( string const& fn,
                      rlf::rimg_planar::tImgPlanar const& img,
                      bool override_,
                      size_t c = png_compression::FILE_COMPRESSION_DEFAULT )   ;
      void write_jpeg( string const& fn,
                       rlf::rimg_planar::tImgPlanar const& img,
                       bool override_,
                       size_t c = jpeg_compression::JPEG_QUALITY_DEFAULT )   ;


   }





   namespace rimg_raw {

      // raw data, linear only
      //      linear, RGBRGBRGB...
      void read_raw_data( string const& fn,
                          size_t w,
                          size_t h,
                          rlf::rimg_linear::tImgLinear& rimg ) ;

      void write_raw_data( string const& fn,
                           rlf::rimg_linear::tImgViewLinear const& wimg ) ;
   }


} // end of ns rlf
#endif
//EOF
