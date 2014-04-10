
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
/*! \file rimg_linear.cpp
\brief impl for linear images

Lib: librimgrw
*/

#include <iostream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>


#include "rimg_alloc_raw_data.h"
#include "rimg_rw_exceptions.h"

using namespace std;

namespace rlf {


   namespace rimg_alloc_raw_data {


      uint8_t*  alloc_raw_data( size_t s ) {

         //  t_Img_template<uint8_t>::set_data((uint8_t *) GlobalAlloc(GMEM_FIXED, s) );
         uint8_t* p = new uint8_t [ s ];

         // goto new handler, if fails
         if( p == 0 ) { // only GlobalAlloc can be NULL, or (nothrow)
            string msg = "GlobalAlloc(GMEM_FIXED, s) fails, size: " + boost::lexical_cast<string>( s ) ;
            throw tImgAllocEx( msg );
         }

         return p;
      }



      void free_raw_data( uint8_t* p ) {
         //GlobalFree((HGLOBAL)getROImgData());
         if( p != 0 ) {
            delete [] p;
            p = nullptr;
         }
      }



   } // end of ns rimg_alloc_raw_data

}
//EOF
