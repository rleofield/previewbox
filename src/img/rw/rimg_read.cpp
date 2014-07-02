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
/*! \file rimg_rw_raw.cpp
\brief impl for read and write raw images

Lib: librimgrw
*/



#include <iostream>
#include <sstream>
#include <string>
#include <boost/shared_ptr.hpp>


#include "boost_cstdint.h"

#include "rimg_rw_exceptions.h"


#include "rimg_linear.h"
#include "rimg_planar.h"
#include "rimg_read_write_bin.h"
#include "rimg_read_write.h"
#include "rimg_fs.h"
#include "rBin.h"
#include "rimg_resolve_image_type.h"
#include "rimg_t_read.h"

using namespace std;


using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;

namespace rlf {

   namespace rimg_rw {

      shared_ptr<t_read> t_reader::create_reader(e_file_type type) {

         // reads binary data to buffer and looks for magic type marker
         //e_file_type type = findtype( _fn );


         // type not found, try with extension
         if( type == e_file_type::none ) {
            string ext = rlf::fs::to_lower( rlf::fs::extension( _fn ) );

            if( ext == rlf::rimg_rw::dot_tif || ext == rlf::rimg_rw::dot_tiff ) {
               type = e_file_type::tif;
            }

            if( ext == rlf::rimg_rw::dot_bmp ) {
               type = e_file_type::bmp;
            }

            if( ext == rlf::rimg_rw::dot_png ) {
               type = e_file_type::png;
            }

            if( ext == rlf::rimg_rw::dot_jpeg || ext == rlf::rimg_rw::dot_jpg ) {
               type = e_file_type::jpeg;
            }
         }

         if( type == e_file_type::tif ) {
            return shared_ptr<t_read>( new  t_read_tiff(_fn) );
         }

         if( type == e_file_type::bmp ) {
            return shared_ptr<t_read>( new  t_read_bmp(_fn) );
         }

         if( type == e_file_type::png ) {
            return shared_ptr<t_read>( new  t_read_png(_fn) );
         }

         if( type == e_file_type::jpeg ) {
            return shared_ptr<t_read>( new  t_read_jpeg(_fn) );
         }

         return nullptr;
      }


      void t_reader::read( string fn, tImgLinear& img )  {

         if( !fs::file_exists( fn ) ) {
            string s = bin_data::msg::file_not_exists( fn );
            throw tImgReadEx( s );
         }

         _fn = fn;
         e_file_type t = findtype( fn );
         shared_ptr<t_read> reader = create_reader( t );

         if( reader != nullptr ) {
            try{
               // read linear image and convert to planar
               reader->read( img );
               return;
            }
            catch( tImgReadEx &ex ){
               // nothing found, clear image
               img.reset();
            }
         }
         // nothing found, clear image
         img.reset();
      }

      void t_reader::read( string fn, tImgPlanar& img )  {

         if( !fs::file_exists( fn ) ) {
            string s = bin_data::msg::file_not_exists( fn );
            throw tImgReadEx( s );
         }
         _fn = fn;
         e_file_type t = findtype( fn );
         shared_ptr<t_read> reader = create_reader( t );

         if( reader != nullptr ) {
            try{
            // read linear image and convert to planar
            reader->read( img );
            return;
            }
            catch( tImgReadEx &ex ){
               // nothing found, clear image
               img.realloc( uint32_xy(), rlf::rimg_planar::tMask::empty );
            }
         }

         // nothing found, clear image
         img.realloc( uint32_xy(), rlf::rimg_planar::tMask::empty );
      }


void read( string fn, tImgLinear& img )  {
   rlf::rimg_rw::t_reader r;
   r.read(fn,img);
}
void read( string fn, tImgPlanar& img )  {
   rlf::rimg_rw::t_reader r;
   r.read(fn,img);
}


}
}
//EOF





