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


#ifndef T_BIN_WRITE
#define T_BIN_WRITE


/*! \file wBin.h
*  \brief class t_bin_write, writes a binary file
*  \author Richard Albrecht
*/


#include <fstream>
#include <vector>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>



namespace bin_write {


   namespace whelper {
      const std::string marker = "%s";
      inline std::string FindAndReplace( const std::string& source,
                                         const std::string& find,
                                         const std::string& replace ) {
         size_t i;
         size_t start = 0;
         std::string ret = source;

         for( ; ( i = ret.find( find, start ) ) != std::string::npos; ) {
            ret.replace( i, find.length(), replace );
            start = i + replace.length();
         }

         return ret;
      }

      inline std::string replace( std::string const& msg, std::string const& s0 = "" ) {

         if( s0.size() > 0 ) {
            return FindAndReplace( msg, whelper::marker, s0 );
         }

         return msg;

      }


      inline char* toCharPtr( std::vector<uint8_t>& b ) {
         return reinterpret_cast<char* >( static_cast<uint8_t* >( &b[0] ) );
      }
      inline char const* toCharPtr( std::vector<uint8_t> const& b ) {
         return reinterpret_cast<char const* >( static_cast<uint8_t const* >( &b[0] ) );
      }

      inline bool file_exists_w( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }

         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::exists( s ) ) {
            return true;
         }

         return false;
      }


   }


   namespace err {
      const std::string msg_file_exists = "File exists: '" + whelper::marker + "'";
      const std::string msg_write_file = "Couldn't write file: '" + whelper::marker + "'";
      const std::string msg_open_file = "Couldn't open file for write: '" + whelper::marker + "'";
      const std::string msg_buffer_empty = "Buffer has zero size: '" + whelper::marker + "'";

      inline std::string write_file( std::string const& s0 ) {
         return whelper::replace( msg_write_file, s0 );
      }
      inline std::string file_exists( std::string const& s0 ) {
         return whelper::replace( msg_file_exists, s0 );
      }
      inline std::string file_open( std::string const& s0 ) {
         return whelper::replace( msg_open_file, s0 );
      }
      inline std::string buffer_empty( std::string const& s0 ) {
         return whelper::replace( msg_buffer_empty, s0 );
      }




   }





   /*! bad_bin_write,
   \param [in] msg  Message
   */
   class bad_bin_write: public std::runtime_error {
   public:
      bad_bin_write( const std::string& msg )
         : std::runtime_error( msg ) { }
   };


   /*! \class t_bin_write
   *  \brief writes a binary file
   */
   class t_bin_write  {
      t_bin_write( const t_bin_write& in );
      t_bin_write& operator= ( const t_bin_write& in );

   public:
      t_bin_write() {}
      ~t_bin_write() {}
      void operator()( const std::string& file, std::vector<uint8_t> const& buf ) {


         if( whelper::file_exists_w( file ) ) {
            throw bad_bin_write( err::file_exists( file ) );
         }

         if( buf.size() == 0 ) {
            throw bad_bin_write( err::buffer_empty( file ) );
         }

         std::ios_base::openmode mode = std::ios::out | std::ios::binary;
         std::ofstream fp( file.c_str(), mode );

         if( !fp.is_open() ) {
            throw bad_bin_write( err::file_open( file ) );
         }

         char const* p = whelper::toCharPtr( buf );
         fp.write( p, buf.size() );

         if( fp.bad() ) {
            throw bad_bin_write( err::write_file( file ) );
         }

      }

   };

}// end of namespace

#endif

//EOF

