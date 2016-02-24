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

/*! \file rimg_fs.cpp
\brief filessystem functions, based on boost

Lib: librimgrw
*/

#ifndef RIMG_fs_H
#define RIMG_fs_H

#include <boost/filesystem.hpp>
#include "t_filename.h"


using namespace std;


namespace rlf {

   namespace fs {
      namespace {
         bool file_exists( boost::filesystem::path const& p ) {
            if( !boost::filesystem::is_regular_file( p ) ) {
               return false;
            }

            boost::filesystem::file_status s = status( p );

            if( boost::filesystem::exists( s ) ) {
               return true;
            }

            return false;
         }
//         bool path_exists( boost::filesystem::path const& p ) {
//            boost::filesystem::file_status s = status( p );

//            if( boost::filesystem::is_directory( s ) ) {
//               return true;
//            }

//            return false;
//         }
      }

      bool file_exists( string const& fn ) {
         boost::filesystem::path p( fn );
         return file_exists( p );
      }
      bool path_exists( string const& path ) {
         boost::filesystem::path p( path );
         return file_exists( p );
      }
      string extension( string const& fn ) {
         boost::filesystem::path p( fn );
         string ext = p.extension().string();
         return ext;
      }

      void to_upper_in_place( string& s ) {
         std::string::iterator start = s.begin();

         while( start != s.end() ) {
            *start = static_cast< char >( ::toupper( *start ) );
            ++start;
         }
      }

      string to_upper( string const& s ) {
         string temp = s;
         to_upper_in_place( temp );
         return temp;
      }

      void to_lower_in_place( string& s ) {
         std::string::iterator start = s.begin();

         while( start != s.end() ) {
            *start = static_cast<char>( ::tolower( *start ) );
            ++start;
         }
      }

      string to_lower( string const& s ) {
         string temp = s;
         to_lower_in_place( temp );
         return temp;
      }

   }

}



#endif // RIMNG_FS_H

