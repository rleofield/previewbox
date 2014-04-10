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



#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <mutex>

#include <boost/filesystem.hpp>


#include "stringhelper.h"
#include "filehelper.h"
#include "t_filename.h"




static std::mutex mutex;

#define HAS_STD_PUT_TIME 0

using std::string;



using namespace std;
using rlf_hstring::trim;
using rlf_hstring::remove_first;
using rlf_hstring::last_char;
using rlf_filefn::t_filename;


namespace rlf_hfile_intern {

   namespace local {
      bool path_exists( std::string const& file ) {
         boost::filesystem::path p( file );
         boost::system::error_code err;
         boost::filesystem::file_status s = status( p, err );

         if( boost::filesystem::is_directory( s ) ) {
            if( boost::filesystem::exists( s ) ) {
               return true;
            }
         }

         return false;
      }

      bool create_directory_( boost::filesystem::path const& p ) {
         string msg ;

         try {
            return boost::filesystem::create_directory( p );

            if( boost::filesystem::is_directory( p ) ) {
               return true;
            }

            return false;
         } catch( boost::filesystem::filesystem_error& ex ) {
            msg = ex.what();
            return false;
         }

         return false;
      }
      string current_path_() {
         boost::filesystem::path p = boost::filesystem::current_path();
         string s = p.string();
         return s;
      }
      bool current_path_( string const& in ) {
         boost::filesystem::path p( in );

         try {
            boost::filesystem::current_path( p );
            return true;
         } catch( boost::filesystem::filesystem_error& err ) {
            return false;
         }
      }

      string slash() {
         static string slash = string();

         if( slash.size() == 0 ) {
            boost::filesystem::path p = boost::filesystem::current_path();
            slash = p.root_directory().string();
            assert( slash.size() == 1 );
         }

         return  slash;
      }
      char char_slash() {
         string sl = slash();
         assert( sl.size() == 1 );
         return  sl[0];
      }



   }

   string extension( string const& fn ) {
      boost::filesystem::path p( fn );
      string ext = p.extension().string();
      return ext;
   }

   bool create_directory( string const& fn ) {
      boost::filesystem::path p( fn );
      return local::create_directory_( p );
   }

   string current_path() {
      boost::filesystem::path p = local::current_path_();
      string s = p.string();
      return s;
   }

   bool file_exists( std::string const& file ) {
      boost::filesystem::path p( file );
      boost::system::error_code err;
      boost::filesystem::file_status s = status( p, err );

      if( boost::filesystem::is_regular_file( s ) ) {
         if( boost::filesystem::exists( s ) ) {
            return true;
         }
      }

      return false;
   }
   bool path_exists( std::string const& file ) {
      boost::filesystem::path p( file );
      boost::system::error_code err;
      boost::filesystem::file_status s = status( p, err );

      if( boost::filesystem::is_directory( s ) ) {
         if( boost::filesystem::exists( s ) ) {
            return true;
         }
      }

      return false;
   }

   boost::uintmax_t file_size( std::string const& file ) {
      boost::filesystem::path p( file );
      return boost::filesystem::file_size( p );
   }




   string fill( int val, char fill , size_t w ) {
      std::ostringstream o;
      o << std::setfill( fill ) << std::right << std::setw( w )  << val;
      return o.str();
   }

   void fill( std::ostream& o, size_t w, int val ) {
      char fill = '0';
      o << std::setfill( fill ) << std::setw( w )  << val;
   }



   string working_directory() {
      string pwd = current_path();
      return pwd;
   }

   bool change_directory( string const& in ) {

      string pwd = working_directory();

      if( pwd == in ) { // nothing to do
         return true;
      }

      return local::current_path_( in );
   }




   bool  create_directory_recursive( string const& in ) {

      string pwd = working_directory();

      if( pwd == in ) { // nothing to do
         return true;
      }

      boost::filesystem::path p( in );

      if( path_exists( in ) ) {
         return true;
      }

      boost::filesystem::create_directories( p );
      return false;
   }

   bool  dir_accessible( string const& in ) {
      return path_exists( in );
   }


   std::string correct_slash_at_end( std::string const& path ) {
      std::string temp = path;

      if( temp.size() == 0 ) {
         return string();
      }

      // correct slash at end
      string s = local::slash();

      if( last_char( temp ) != s[0] ) {
         temp +=    s;
      }

      return temp;
   }


   string  substring( string const& s, size_t pos, size_t n ) {
      if( n == 0 ) {
         return string();
      }

      size_t size = s.size();

      if( pos >= size ) {
         return string();
      }

      if( n > size - pos ) {
         n = size - pos;
      }

      return s.substr( pos, n );
   }



   std::string getbasename( std::string const& file ) {
      size_t pos =  file.rfind( "." );

      if( pos == string::npos ) {
         // file is basenaem
         return file;
      }

      if( pos == 0 ) {
         // is linux dot file
         return file;
      }

      // return first part
      return file.substr( 0, pos );
   }
   std::string getextension( std::string const& file ) {
      size_t pos =  file.rfind( "." );

      if( pos == string::npos ) {
         // file is basename
         return "";
      }

      if( pos == 0 ) {
         // is linux dot file
         return "";
      }

      // return first part
      return file.substr( pos + 1 );
   }







} // end of namespace


//EOF

