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


#ifndef rlf_tFnFunctions_H
#define rlf_tFnFunctions_H

#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <cstdint>


#include "filehelper.h"
#include "t_filename.h"

//#include "filetreewalk.h"

using std::string;

namespace rlf_hfile {

   //   class NonCopyable {
   //         NonCopyable( const NonCopyable& );
   //         const NonCopyable&  operator= ( const NonCopyable& );
   //      protected:
   //         NonCopyable( ) { }
   //         virtual ~NonCopyable( ) { }
   //   };

   namespace fn_control {






      std::string working_folder();
      bool change_folder( std::string const& newPath );

      // return true, if exists
      bool create_folder( std::string const& path );
      bool create_folders( std::string const& path );


      bool delete_recursive( std::string const& path );


      bool isAbsolutePath( string const& path );

      string correct_slash_at_end( string const& path );
      string basename( string const& file );
      string extension( string const& file );


      bool file_exists( string const& fn ) ;
      bool path_exists( string const& path ) ;

      boost::uintmax_t file_size( string const& file );

      // int to string, fill char is ' '
      // an other fillchar is ugly for negative values
      string int_to_string( int val, int width = 3 );

      // uint to string, default fillchar is '0'
      string uint_to_string( uint32_t val, int  width = 3, char fill = '0' );

      //uint32_t get_folder_count( string const& path );

      string date_time( string const& format = rlf_time::date_time_hyphenated ); // as 2012-07-25_17-35-10

   } // end of namespace fn_control


} // end of namespace rlf_hfile

#endif
//EOF

