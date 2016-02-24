
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



#ifndef RIMG_t_read_H
#define RIMG_t_read_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "e_file_type.h"

using namespace std;
using rlf::rimg_rw::e_file_type;


namespace rlf {
   class t_read;
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

namespace rlf {



   namespace rimg_rw {
      class t_reader {
      protected:
         string _fn;
      public:
         t_reader():_fn(){}
         shared_ptr<t_read> create_reader( e_file_type t ) ;
         void read( string fn, tImgLinear& img );
         void read( string fn, tImgPlanar& img );

      };

   }

}

using rlf::rimg_linear::tImgViewLinear;



namespace rlf {



   class t_read {

   protected:

      string _fn;


      t_read( string fn ): _fn( fn ) {}
      virtual ~t_read() {}
   public:
      virtual void read( tImgLinear& img )  = 0;
      virtual void read( tImgPlanar& planar )  = 0;

      //      friend shared_ptr<t_read> rlf::rimg_rw::t_reader::create_reader();

   };



   class t_read_tiff: public t_read {
      void _read( tImgLinear& img ) ;


      t_read_tiff( string fn ): t_read( fn ) {}
      void read( tImgLinear& img );
      void read( tImgPlanar& planar );

   public:
      ~t_read_tiff() {}


      friend shared_ptr<t_read> rlf::rimg_rw::t_reader::create_reader( e_file_type t );
   };


   class t_read_bmp: public t_read {

      void _read( tImgLinear& img ) ;

      t_read_bmp( string fn ): t_read( fn ) {}
      void read( tImgLinear& img );
      void read( tImgPlanar& planar );

   public:
      ~t_read_bmp() {}

      friend shared_ptr<t_read> rlf::rimg_rw::t_reader::create_reader( e_file_type t );
   };


   class t_read_png: public t_read {

      t_read_png( string fn ): t_read( fn ) {}
      void read( tImgLinear& img );
      void read( tImgPlanar& planar );

   public:
      ~t_read_png() {}

      friend shared_ptr<t_read> rlf::rimg_rw::t_reader::create_reader( e_file_type t );
   };


   class t_read_jpeg: public t_read {

      t_read_jpeg( string fn ): t_read( fn ) {}
      void read( tImgLinear& img );
      void read( tImgPlanar& planar );

   public:
      ~t_read_jpeg() {}

      friend shared_ptr<t_read> rlf::rimg_rw::t_reader::create_reader( e_file_type t );
   };





} // end of ns rlf
#endif
//EOF
