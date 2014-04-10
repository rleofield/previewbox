

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <iostream>


//#include "t_fncontrol.h"
#include "filetreewalk.h"
#include "stringhelper.h"


using std::string;
using std::vector;
using namespace boost::filesystem;
using rlf_filefn::t_filename;
using rlf_hstring::split;

namespace rlf_ftw {

   const string delimiters = ";, ";

   std::vector <t_filename > const& ftw::files()const {
      return _files;
   }
   std::vector <t_filename > const& ftw::dirs()const {
      return _dirs;
   }

   void ftw::path( std::string const& p ) {
      if( _path.path() != p ) {
         _path = p;
         _files.clear();
         _dirs.clear();
      }
   }
   void ftw::include_files( string const& i ) {
      _include_files = split( i, delimiters ) ;
   }
   void ftw::exclude_files( string const& i ) {
      _exclude_files = rlf_hstring::split( i, delimiters ) ;

   }
   void ftw::include_folders( string const& i ) {
      _include_folders = rlf_hstring::split( i, delimiters ) ;
   }
   void ftw::exclude_folders( string const& i ) {
      _exclude_folders = rlf_hstring::split( i, delimiters ) ;
   }

   bool ftw::filter_folder( boost::filesystem::path const& p ) {

      bool b = true;

      string n = p.string();

for( auto s: _exclude_folders ) {
         if( s == "*" ) {
            b = false;
            break;
         }

         size_t pos = rlf_hstring::index_right( n, s );

         if( pos != string::npos ) {
            b = false;
            break;
         }
      }

      // b == false, wenn file weggelassen wird

for( auto s: _include_folders ) {
         if( s == "*" ) {
            b = true;
            break;
         }

         size_t pos = rlf_hstring::index_right( n, s );

         if( pos != string::npos && pos >= n.size() - s.size() ) {
            b = true;
            break;
         }

      }

      // b == true, wenn file verwendet wird
      return b;
   }

   bool ftw::filter_file( boost::filesystem::path const& p ) {

      bool b = true;

      string n = p.string();

for( auto s: _exclude_files ) {
         if( s == "*" ) {
            b = false;
            break;
         }

         size_t pos = rlf_hstring::index_right( n, s );

         if( pos != string::npos ) {
            b = false;
            break;
         }
      }

      // b == false, wenn file weggelassen wird

for( auto s: _include_files ) {
         if( s == "*" ) {
            b = true;
            break;
         }

         size_t pos = rlf_hstring::index_right( n, s );

         if( pos != string::npos && pos >= n.size() - s.size() ) {
            b = true;
            break;
         }
      }

      // b == true, wenn file verwendet wird
      return b;
   }

   void ftw::scan_folders() throw( bad_ftw ) {

      if( ! _path.path_exists() ) {
         throw bad_ftw( "folder doesn't exist: " + _path.path() );
      }

      if( ! _path.is_folder() ) {
         throw bad_ftw( "input is not a folder: " + _path.path() );
      }

      //boost::regex pattern("*"); // list all files starting with a
      boost::filesystem::path  current_dir( _path.path() ); //

      for( recursive_directory_iterator iter( current_dir ), end;
            iter != end;
            ++iter ) {
         boost::filesystem::path p = iter->path();

         if( boost::filesystem::is_directory( p ) ) {
            std::string dname = p.string();
            bool b = filter_folder( p );

            if( b ) {
               t_filename fn;
               fn.path( dname );
               _dirs.push_back( fn );
            }
         }

         if( boost::filesystem::is_regular_file( p ) ) {
            std::string name = p.string();
            bool b = filter_file( p );

            if( b ) {
               t_filename fn( name );
               _files.push_back( fn );
            }
         }

         //    if (regex_match(name, pattern))
         //      std::cout << iter->path() << "\n";
      }
   }


   void ftw::scan_folder() throw( bad_ftw ) {
      if( ! _path.path_exists() ) {
         throw bad_ftw( "folder doesn't exist: " + _path.path() );
      }

      if( ! _path.is_folder() ) {
         throw bad_ftw( "input is not a folder: " + _path.path() );
      }

      //boost::regex pattern("*"); // list all files starting with a
      boost::filesystem::path  current_dir( _path.path() ); //

      for( directory_iterator iter( current_dir ), end;
            iter != end;
            ++iter ) {
         boost::filesystem::path p = iter->path();

         if( boost::filesystem::is_directory( p ) ) {
            std::string dname = p.string();
            bool b = filter_folder( p );

            if( b ) {
               t_filename fn;
               fn.path( dname );
               _dirs.push_back( fn );
            }
         }

         if( boost::filesystem::is_regular_file( p ) ) {
            std::string name = p.string();
            bool b = filter_file( p );

            if( b ) {
               t_filename fn( name );
               _files.push_back( fn );
            }
         }

         //    if (regex_match(name, pattern))
         //      std::cout << iter->path() << "\n";
      }
   }




}














// EOF


