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

#include "rimg_rw_exceptions.h"

#include "rimg_linear.h"
#include "rimg_planar.h"
#include "rimg_read_write_bin.h"
#include "rimg_read_write.h"
#include "rimg_fs.h"

using namespace std;


using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgViewLinear;
using rlf::rimg_planar::tImgPlanar;

namespace rlf {

   namespace rimg_raw {

      void read_raw_data( string const& fn, size_t w, size_t h, tImgLinear& rimg ) {

         vector<uint8_t> buf;

         try {
            bin_data::tReadBin()( fn, buf );
         }  catch( bin_data::tBinReadEx& ) {
            throw;
         }

         if( w * h == buf.size() ) {
            rimg.set_mono8();
         }

         if( w * h * 3  == buf.size() ) {
            rimg.set_rgb();
         }

         if( rimg.is_none() ) {
            string s = "Couldn't read raw file: '" + fn + "'";
            throw tImgReadEx( s );
         }

         rimg.size() = tSize( w, h );
         rimg.alloc_data_buffer();
         rimg.insert_data( buf, 0 );

      }


      void write_raw_data( string const& fn, tImgViewLinear const& img ) {
         try {
            bin_data::tWriteBin()( fn, img.data_ptr(), img.bytes() );
         }  catch( bin_data::tBinWriteEx& ) {
            throw;
         }
      }

   }
}
//EOF





