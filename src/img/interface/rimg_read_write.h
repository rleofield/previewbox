
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


using namespace std;
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


   class t_read {

   public:
      t_read() {}
      virtual ~t_read() {}
      virtual void read( string const& fn, tImgLinear& img )  = 0;
      virtual void read( string const& fn, tImgPlanar& planar )  = 0;

   };

   class t_write {

      size_t _c;
   protected:
      size_t c()const {
         return _c;
      }
   public:
      t_write( size_t c_ ): _c( c_ ) {}
      virtual ~t_write() {}
      virtual void write( string const& fn, tImgViewLinear const& img )  = 0;
      virtual void write( string const& fn, rlf::rimg_planar::tImgPlanar const& img )  = 0 ;

   };


   class t_read_tiff: public t_read {

   public:
      t_read_tiff() {}
      ~t_read_tiff() {}
      void read( string const& fn, tImgLinear& img ) ;
      void read( string const& fn, tImgPlanar& planar )  ;

   };

   class t_write_tiff: public t_write {

   public:
      t_write_tiff(): t_write( 0 ) {}
      ~t_write_tiff() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };

   class t_read_bmp: public t_read {

   public:
      t_read_bmp() {}
      ~t_read_bmp() {}
      void read( string const& fn, tImgLinear& img ) ;
      void read( string const& fn, tImgPlanar& planar )  ;

   };

   class t_write_bmp: public t_write {

   public:
      t_write_bmp(): t_write( 0 ) {}
      ~t_write_bmp() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };

   class t_read_png: public t_read {
   public:
      //rw_png(size_t c = png_compression::FILE_COMPRESSION_DEFAULT):_c(c) {}
      t_read_png() {}
      ~t_read_png() {}
      void read( string const& fn, tImgLinear& img ) ;
      void read( string const& fn, tImgPlanar& planar )  ;
   };

   class t_write_png: public t_write {
   public:
      t_write_png( size_t c_ ): t_write( c_ ) {}
      ~t_write_png() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;
   };

   class t_read_jpeg: public t_read {
   public:
      t_read_jpeg() {}
      ~t_read_jpeg() {}
      void read( string const& fn, tImgLinear& img ) ;
      void read( string const& fn, tImgPlanar& planar )  ;

   };

   class t_write_jpeg: public t_write {

   public:
      t_write_jpeg( size_t c_ ):  t_write( c_ ) {}
      ~t_write_jpeg() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };


   namespace rimg_rw {


      void read( string fn, tImgLinear& img )  ;
      void write( string const& fn, tImgViewLinear const& img, bool override_ )   ;
      void write_png( string const& fn, tImgViewLinear const& img, bool override_, size_t c = png_compression::FILE_COMPRESSION_DEFAULT );
      void write_jpeg( string const& fn, tImgViewLinear const& img, bool override, size_t c = jpeg_compression::JPEG_QUALITY_DEFAULT );

      void read( string fn, rlf::rimg_planar::tImgPlanar& planar )  ;
      void write( string const& fn, rlf::rimg_planar::tImgPlanar const& img, bool override_ )   ;
      void write_png( string const& fn, rlf::rimg_planar::tImgPlanar const& img, bool override_, size_t c = png_compression::FILE_COMPRESSION_DEFAULT )   ;
      void write_jpeg( string const& fn, rlf::rimg_planar::tImgPlanar const& img, bool override_, size_t c = jpeg_compression::JPEG_QUALITY_DEFAULT )   ;


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
