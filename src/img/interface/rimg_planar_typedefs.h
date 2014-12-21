/* --------------------------------------------------------------------------
Copyright 2013 by Richard Albrecht
richard.albrecht@rleofield.de

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

/*! \file rimg_planar_typedefs.h
\brief typdefs and forwards for planar images

*/



#ifndef RIMG_PLANAR_TYPEDEFS_H
#define RIMG_PLANAR_TYPEDEFS_H


#include <vector>

namespace rlf {

   namespace rimg_planar {

      typedef  std::vector< uint8_t > vLine8;
      typedef  std::vector< vLine8  > tPlane8;


      typedef  std::vector< uint16_t > tLine16;
      typedef  std::vector< tLine16  > tPlane16;
      typedef  std::vector< tPlane16 > tCube16;

      typedef  std::vector< uint32_t > tLine32;
      typedef  std::vector< tLine32  > tPlane32;
      typedef  std::vector< tPlane32 > tCube32;

      typedef  std::vector< int32_t > tIntLine32;
      typedef  std::vector< tIntLine32  > tIntPlane32;

      typedef  std::vector< uint16_t > tLine16;
      typedef  std::vector< tLine16  > tPlane16;

      typedef  std::vector< int16_t > tIntLine16;
      typedef  std::vector< tIntLine16  > tIntPlane16;

   }

}

#endif

//EOF
