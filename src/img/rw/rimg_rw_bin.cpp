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

/*! \file rimg_rw_bin.cpp
\brief read write bin data

Lib: librimgrw
*/


#include <iostream>
#include <sstream>
#include <string>

#include "rimg_read_write_bin.h"


namespace rlf {

   namespace bin_data {


      namespace msg {

         string replace( string const& msg, string const& s0 ) {
            string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != ::string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }



         string read_file_( string const& s0 ) {
            return replace( msg_read_file, s0 );
         }
         string file_not_exists( string const& s0 ) {
            return replace( msg_file_not_exists, s0 );
         }


         string write_file( string const& s0 ) {
            return replace( msg_write_file, s0 );
         }
         string file_exists( string const& s0 ) {
            return replace( msg_file_exists, s0 );
         }
         string file_open( string const& s0 ) {
            return replace( msg_open_file, s0 );
         }
         string buffer_empty( string const& s0 ) {
            return replace( msg_buffer_empty, s0 );
         }

      } // end of ns msg





      tReadBin::tReadBin() {}
      tReadBin::~tReadBin() {}
      void tReadBin::operator()( const string& file, vector<uint8_t>& buf )  {
         boost::filesystem::path p( file );

         uintmax_t size = boost::filesystem::file_size( p );

         ios_base::openmode mode = ios::in | ios::binary;
         ifstream fp( p.c_str(), mode );

         if( fp.bad() ) {
            string s = msg::read_file_( file );
            throw tBinReadEx( s );
         }

         buf.resize( static_cast<size_t>( size ), 0 );
         char* const buffer = to_char_ptr<uint8_t>( buf );
         fp.read( buffer, size );

         if( fp.eof() ) {
            string s = msg::read_file_( file );
            throw tBinReadEx( s );
         }

      }
      \
      tWriteBin::tWriteBin() {}
      tWriteBin::~tWriteBin() {}
      void tWriteBin::operator()( const string& fn, vector<uint8_t> const& buf ) {

         ios_base::openmode mode = ios::out | ios::binary;
         ofstream fp( fn.c_str(), mode );

         if( fp.bad() ) {
            throw tBinWriteEx( msg::file_open( fn ) );
         }

         char const* p = rlf::to_char_ptr<uint8_t>( buf );
         fp.write( p, buf.size() );

         if( fp.bad() ) {
            throw tBinWriteEx( msg::write_file( fn ) );
         }

      }
      void tWriteBin::operator()( const string& fn, uint8_t const* buf, size_t size ) {
         boost::filesystem::path pa( fn );

         if( size == 0 ) {
            throw tBinWriteEx( msg::buffer_empty( fn ) );
         }

         ofstream fp( fn.c_str(), ios::out | ios::binary );

         if( fp.bad() ) {
            throw tBinWriteEx( msg::file_open( fn ) );
         }

         char const* p =  reinterpret_cast<char const*>( buf );
         fp.write( p, size );

         if( fp.bad() ) {
            throw tBinWriteEx( msg::write_file( fn ) );
         }
      }


   }// end of namespace bin_data

} // end of ns rlf


