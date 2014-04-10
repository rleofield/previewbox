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

using std::string;

namespace rlf_hfile {

   class NonCopyable {
      NonCopyable( const NonCopyable& );
      const NonCopyable&  operator= ( const NonCopyable& );
   protected:
      NonCopyable( ) { }
      virtual ~NonCopyable( ) { }
   };

   class tFnFunctions: public NonCopyable {

      bool operator==( const tFnFunctions& s )const;

   public:
      tFnFunctions() {}
      ~tFnFunctions() {}


      string const& Argv( uint32_t pos )const;
      void SetArgv( char* iargv[] );

      std::string working_folder()const;
      bool change_folder( std::string const& newPath )const;

      // return true, if exists
      bool create_folder( std::string const& path )const;
      bool create_folders( std::string const& path )const;


      bool delete_recursive( std::string const& path )const;


      std::vector<rlf_filefn::t_filename> files_in_folder( std::string const& path, string const& include = "", string const& exclude = "" ) const;
      std::vector<rlf_filefn::t_filename> files_in_subfolders( std::string const& path, string const& include = "", string const& exclude = "" )const;
      std::vector<rlf_filefn::t_filename> subfolders( std::string const& path,  string const& include = "", string const& exclude = "" )const;

      bool isAbsolutePath( std::string const& path )const;

      std::string correct_slash_at_end( std::string const& path );
      std::string basename( std::string const& file )const;
      std::string extension( std::string const& file )const;


      bool file_exists( string const& fn )const ;
      bool path_exists( string const& path )const ;

      boost::uintmax_t file_size( std::string const& file )const ;

      // int to string, fill char is ' '
      // an other fillchar is ugly for negative values
      string int_to_string( int val, size_t width = 3 )const;

      // uint to string, default fillchar is '0'
      string uint_to_string( uint32_t val, size_t  width = 3, char fill = '0' )const;

      uint32_t get_folder_count( string const& path )const;

      string date_time( string const& format = rlf_time::date_time_hyphenated )const; // as 2012-07-25_17-35-10

   };


} // end of namespace rlf_hfile

#endif
//EOF

