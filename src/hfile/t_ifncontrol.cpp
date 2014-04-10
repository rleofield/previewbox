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
#include "filetreewalk.h"
#include "t_fncontrol.h"
#include "timer.h"


using namespace std;
using rlf_hstring::index;

namespace rlf_hfile {




   bool tFnFunctions::create_folder( std::string const& path )const {
      bool temp = rlf_hfile_intern::create_directory( path );
      return temp;
   }
   bool tFnFunctions::create_folders( std::string const& path )const {
      rlf_hfile_intern::tSaveCurrentDirectory save;
      bool temp = rlf_hfile_intern::create_directory_recursive( path );
      return temp;
   }

   string tFnFunctions::working_folder()const {
      string temp = rlf_hfile_intern::working_directory();
      temp = rlf_hfile_intern::correct_slash_at_end( temp );
      return temp;
   }

   bool tFnFunctions::change_folder( string const& newPath )const {
      // rlf_hfile_intern::tSaveCurrentDirectory save;
      return rlf_hfile_intern::change_directory( newPath );
   }

   namespace {
      bool compareByString( string const& i, string const& j ) {
         return ( i < j );
      }
   }




   namespace {
      bool compareByFullname( t_filename const& i, t_filename const& j ) {
         return ( i.fullname() < j.fullname() );
      }
   }

   // get files in subfolder non recursive
   std::vector<rlf_filefn::t_filename> tFnFunctions::files_in_subfolders( std::string const& path, string const& include, string const& exclude ) const {

      string temp_include = include;
      string temp_exclude = exclude;


      rlf_ftw::ftw f( path );
      f.include_files( temp_include );
      f.exclude_files( temp_exclude );
      f.scan_folders();
      return f.files();
   }

   std::vector<rlf_filefn::t_filename> tFnFunctions::subfolders( std::string const& path, string const& include, string const& exclude ) const {
      if( !path_exists( path ) ) {
         return std::vector<rlf_filefn::t_filename>();
      }

      rlf_ftw::ftw f( path );
      f.include_folders( include );
      f.exclude_folders( exclude );
      f.scan_folders();
      return f.dirs();
   }



   std::vector<rlf_filefn::t_filename> tFnFunctions::files_in_folder( std::string const& path, string const& include, string const& exclude )const {
      string temp_include = include;
      string temp_exclude = exclude;


      rlf_ftw::ftw f( path );
      f.include_files( temp_include );
      f.exclude_files( temp_exclude );
      f.scan_folder();
      return f.files();

   }



   // wenn: an 2. Stelle am Anfang steht dann nicht ok
   bool tFnFunctions::isAbsolutePath( string const& path )const {
      size_t pos = index( path, ":" );

      if( pos == 1 ) {
         return true;
      }

      return false;
   }


   std::string tFnFunctions::correct_slash_at_end( std::string const& path ) {
      return rlf_hfile_intern::correct_slash_at_end( path );
   }

   std::string tFnFunctions::basename( std::string const& file )const {
      return rlf_hfile_intern::getbasename( file );
   }

   std::string tFnFunctions::extension( std::string const& file ) const {
      return rlf_hfile_intern::getextension( file );
   }


   bool tFnFunctions::file_exists( string const& fn )const {
      return rlf_hfile_intern::file_exists( fn );
   }

   bool tFnFunctions::path_exists( string const& path )const {
      return rlf_hfile_intern::path_exists( path );
   }

   boost::uintmax_t tFnFunctions::file_size( std::string const& file )const {
      return rlf_hfile_intern::file_size( file );
   }


   string tFnFunctions::int_to_string( int val, size_t width )const {
      string s = rlf_hstring::toString( val, width, ' ' );
      return s;
   }
   string tFnFunctions::uint_to_string( uint32_t val, size_t  width, char fill )const {
      string s = rlf_hstring::toString( val, width, fill );
      return s;
   }


   uint32_t tFnFunctions::get_folder_count( string const& path )const

   try {
      std::vector<rlf_filefn::t_filename> v = subfolders( path );
      return static_cast<uint32_t>( v.size() );
   } catch( rlf_ftw::bad_ftw& ex ) {
      return 0;
   }

   string tFnFunctions::date_time( string const& format )const {
      //return rlf_htime::tTimer().now( format );
      return rlf_htime::tTimer::now( format );
   }




} // end of namespace


//EOF

