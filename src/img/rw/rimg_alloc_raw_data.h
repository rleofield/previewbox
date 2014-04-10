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

/*! \file rimg_alloc_raw_data.h
\brief linear arrays for pixel

Lib: librimgrw
*/


#ifndef RIMG_ALLOC_RAW_DATA
#define RIMG_ALLOC_RAW_DATA



#include <exception>
#include <string>
#include <memory.h>

#include "t_2D_xy.h"

//#include "rimg_typedefs.h"





namespace rlf {





   namespace rimg_alloc_raw_data {

      /*! align to multiple of 4 == 32 bit boundary
      \return converted value
      \param s value to align
      */
      inline uint32_t align32( uint32_t s ) {
         if( s & 0x3 ) {  // 0011,  1,2, oder 3
            s |= 0x3;     // set to 0x11, (3)
            ++s;          // add 1 to align
         }

         return s;
      }

      uint8_t*  alloc_raw_data( size_t s ) ;
      void free_raw_data( uint8_t* p ) ;


   } // end of ns rimg_alloc_raw_data

}


#endif
//EOF
