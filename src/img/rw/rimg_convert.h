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
/*! \file rimg_convert.h
\brief convert images

Lib: librimgrw
*/




#ifndef RIMG_convert_H
#define RIMG_convert_H

#include <string>

#include "rimg_linear.h"
#include "rimg_planar.h"

namespace rlf {

   string replace( string const& msg,
                   string const& s0,
                   string const& s1 = "",
                   string const& s2 = "",
                   string const& s3 = "" ) ;
   namespace rimg_convert {



      using rlf::rimg_linear::tImgViewLinear;
      using rlf::rimg_linear::tImgLinear;
      using rlf::rimg_planar::tImgPlanar;

      // convert an gray/rgb/rgba image to an planar image, alpha channel in rgba is preserved
      void convert( tImgViewLinear const& img, tImgPlanar& planar );
      void convert( tImgPlanar const& planar, tImgLinear& img );

      // RGB to gray with weights: red = 0.299, green = 0.587, blue = 0.114;
      void convertRGB_Gray( tImgPlanar const& rgb, tImgPlanar& gray_img ) ;
   }

}
#endif
//EOF
