
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



#ifndef RIMG_t_write_H
#define RIMG_t_write_H

#include <string>
#include <boost/shared_ptr.hpp>

using namespace std;

namespace rlf {
   class t_read;
   namespace rimg_planar {
      class tImgPlanar;
   }

   namespace rimg_linear {
      class tImgLinear;
      class tImgViewLinear;
   }

   namespace rimg_rw {
      shared_ptr<t_read> create_reader( string const& fn, vector<uint8_t>& v );

   }

}

using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgViewLinear;

namespace rlf {

   class t_write {

      size_t _c;
   protected:
      int c()const {
         return static_cast<int>(_c );
      }
   public:
      t_write( size_t c_ ): _c( c_ ) {}
      virtual ~t_write() {}
      virtual void write( string const& fn, tImgViewLinear const& img )  = 0;
      virtual void write( string const& fn, tImgPlanar const& img )  = 0 ;

   };



   class t_write_tiff: public t_write {

   public:
      t_write_tiff(): t_write( 0 ) {}
      ~t_write_tiff() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };

   class t_write_bmp: public t_write {

   public:
      t_write_bmp(): t_write( 0 ) {}
      ~t_write_bmp() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };


   class t_write_png: public t_write {
   public:
      t_write_png( size_t c_ ): t_write( c_ ) {}
      ~t_write_png() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;
   };

   class t_write_jpeg: public t_write {

   public:
      t_write_jpeg( size_t c_ ):  t_write( c_ ) {}
      ~t_write_jpeg() {}
      void write( string const& fn, tImgViewLinear const& img )  ;
      void write( string const& fn, tImgPlanar const& img )   ;

   };



} // end of ns rlf
#endif
//EOF
