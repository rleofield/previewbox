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


#include <algorithm>

#include "stringhelper.h"
#include "filehelper.h"
//#include "filetreewalk.h"
#include "fn_control.h"
#include "timer.h"




using std::string;
using rlf_hstring::index;
using rlf_filefn::t_filename;
using rlf_hfile_intern::tSaveCurrentDirectory ;


namespace rlf_hfile {

   namespace fn_control {

      bool create_folder( std::string const& path ) {
         bool temp = rlf_hfile_intern::create_directory( path );
         return temp;
      }
      bool create_folders( std::string const& path ) {
         tSaveCurrentDirectory save;
         bool temp = rlf_hfile_intern::create_directory_recursive( path );
         return temp;
      }

      string working_folder() {
         string temp = rlf_hfile_intern::working_directory();
         temp = rlf_hfile_intern::correct_slash_at_end( temp );
         return temp;
      }

      bool change_folder( string const& newPath ) {
         return rlf_hfile_intern::change_directory( newPath );
      }

      namespace {
//         bool compareByString( string const& i, string const& j ) {
//            return ( i < j );
//         }
      }



      namespace {
//         bool compareByFullname( t_filename const& i, t_filename const& j ) {
//            return ( i.fullname() < j.fullname() );
//         }
      }

      std::string correct_slash_at_end( std::string const& path ) {
         return rlf_hfile_intern::correct_slash_at_end( path );
      }

      std::string basename( std::string const& file ) {
         return rlf_hfile_intern::getbasename( file );
      }

      std::string extension( std::string const& file ) {
         return rlf_hfile_intern::getextension( file );
      }


      bool file_exists( string const& fn ) {
         return rlf_hfile_intern::file_exists( fn );
      }

      bool path_exists( string const& path ) {
         return rlf_hfile_intern::path_exists( path );
      }

      boost::uintmax_t file_size( std::string const& file ) {
         return rlf_hfile_intern::file_size( file );
      }


      string int_to_string( int val, int width ) {
         int prec = 2;
         string s = rlf_hstring::toString( val, width, prec );
         return s;
      }
      string uint_to_string( uint32_t val, int  width, char fill ) {
         int prec = 2;
         string s = rlf_hstring::toString( val, width, prec, fill );
         return s;
      }

      //      uint32_t get_folder_count( string const& path ) {
      //         try {
      //            ftwReturn ret = subfolders( path );
      //            std::vector<t_filename> v = ret.folders();
      //            return static_cast<uint32_t>( v.size() );
      //         } catch( rlf_ftw::bad_ftw& ex ) {
      //            return 0;
      //         }
      //      }


      string date_time( string const& format ) {
         return rlf_htime::tTimer::now( format );
      }

   } // end of ns fn_control


} // end of ns rlf_hfile


//EOF

