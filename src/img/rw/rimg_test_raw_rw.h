
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

/*! \file rimg_test_raw_rw.h
\brief reads and writes fast data to a file, for fats test

Lib: librimgrw
*/


#ifndef RIMG_test_raw_read_write_H
#define RIMG_test_raw_read_write_H

#include <string>
#include <vector>

#include <stdexcept>

#include "rimg_planar.h"

using namespace std;

namespace rlf {


   namespace rimg_planar {
      class tImgPlanar;
   }



   namespace rimg_test_rw {




      // raw data, planer, own format, no standard
      //      planar = mask, y, x, data
      void read_raw_data( string const& fn, rlf::rimg_planar::tImgPlanar& rimg ) ;

      void write_raw_data( string const& fn, rlf::rimg_planar::tImgPlanar const& wimg, bool override_ = true ) ;







   } // end of ns img_rw
} // end of ns rlf
#endif
//EOF
