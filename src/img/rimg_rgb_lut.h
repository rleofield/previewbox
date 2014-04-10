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

      C++, GCC
File: cRGBLut.h
*/
#ifndef C_RGB_LUT_H
#define C_RGB_LUT_H


#include <vector>


#include "cbasetyp.h"
#include <cRGBAlpha.h>
#include "tstring.h"

using namespace rlf::tstr;

namespace rlf {
   namespace nImg {


      class c_RGBLut {

      public:


         static bool Write( string const& file, std::vector<cRGBAlpha> const& l );    // write to disk
         static std::vector<cRGBAlpha> Read( string const& file );          // read from disk
         static std::vector<cRGBAlpha> ReadHex( string const& file );       // read photoshop format
         static bool WriteHex( string const& file, std::vector<cRGBAlpha> const& l ); // write photoshop format

         static std::vector<cRGBAlpha> GlowScale();
         static std::vector<cRGBAlpha> Green();
         static std::vector<cRGBAlpha> Spectrum();
         static std::vector<cRGBAlpha> Gray();
         static std::vector<cRGBAlpha> Linearize();


         //   0..< low = blue, high..< 256 = red
         static std::vector<cRGBAlpha> Range( word32 low, word32 high );
         static std::vector<cRGBAlpha> Invert();



      };
   } // end of namespace nImg
} // end of namespace rlf


#endif // C_RGB_LUT_H
