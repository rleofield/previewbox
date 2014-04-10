
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

/*! \file rimg_rw_exceptions.h
\brief exceptions in this framework

Lib: librimgrw
*/


#ifndef RIMG_rw_exceptions_H
#define RIMG_rw_exceptions_H


#include <cstdint>
#include <string>
#include <stdexcept>

//#include "rimg_exceptions.h"

using namespace std;

namespace rlf {


   class rImgEx: public runtime_error {
   protected:

   public:
      rImgEx( const string& msg ):  runtime_error( msg )
      { }
      ~rImgEx()throw() {}
   };

   class tImgAllocEx: public rImgEx {
      string _what;
   public:
      tImgAllocEx( const string& msg ): rImgEx( msg ), _what()
      { }
      ~tImgAllocEx()throw() {}

   };



   class tImgIndexEx: public rImgEx {
      string _what;
   public:
      tImgIndexEx( const string& msg ): rImgEx( msg ), _what()
      { }
      ~tImgIndexEx()throw() {}
   };


   class tImgReadEx: public rImgEx {

   public:
      tImgReadEx( const string& msg ): rImgEx( msg )
      { }
      ~tImgReadEx()throw() {}
   };
   class tImgWriteEx: public rImgEx {

   public:
      tImgWriteEx( const string& msg ): rImgEx( msg )
      { }
      ~tImgWriteEx()throw() {}
   };




}

#endif
//EOF
