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

/*! \file rimg_read_write_bin.h
\brief read write binary data in one stroke with a vector

Lib: librimgrw
*/


#ifndef bin_data_tReadBin_H
#define bin_data_tReadBin_H






#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <boost/filesystem.hpp>

using namespace std;

namespace rlf {

   template <typename T>
   inline char* to_char_ptr( std::vector<T>& b ) {
      return reinterpret_cast<char* >( b.data()  );
   }
   template <typename T>
   inline char const* to_char_ptr( std::vector<T> const& b ) {
      return reinterpret_cast<char const* >( b.data()  );
   }


   namespace bin_data {

      namespace msg {
         const string marker = "%s";
         string replace( string const& msg, string const& s0 = "" ) ;


         const string msg_file_not_exists = "File doesn't exist: '%s'";
         const string msg_read_file = " Couldn't read file '%s'";
         const string msg_file_exists = "File exists: '%s'";
         const string msg_write_file = "Couldn't write file: '%s'";
         const string msg_open_file = "Couldn't open file for write: '%s'";
         const string msg_buffer_empty = "Buffer has zero size: '%s'";

         string read_file_( string const& s0 ) ;
         string file_not_exists( string const& s0 ) ;


         string write_file( string const& s0 ) ;
         string file_exists( string const& s0 ) ;
         string file_open( string const& s0 ) ;
         string buffer_empty( string const& s0 ) ;

      } // end of ns msg





      /*! tBinReadEx
      \param [in] msg  error message
      */
      class tBinReadEx: public runtime_error {
      public:
         tBinReadEx( const string& msg )
            : runtime_error( msg ) { }
      };

      /*! \class tReadBin
      *  \brief reads a binary file
      *
      */
      class tReadBin  {
         tReadBin& operator= ( const tReadBin& in );
         tReadBin( const tReadBin& in );
      public:

         /*! \brief default ctor
         */
         tReadBin();
         ~tReadBin();
         /*! \brief read a file
         */
         void operator()( const string& file, vector<uint8_t>& buf )  ;



      };





      /*! tBinWriteEx,
      \param [in] msg  Message
      */
      class tBinWriteEx: public runtime_error {
      public:
         tBinWriteEx( const string& msg )
            : runtime_error( msg ) { }
      };


      /*! \class tWriteBin
      *  \brief writes bin files
      */
      class tWriteBin  {
         tWriteBin( const tWriteBin& in );
         tWriteBin& operator= ( const tWriteBin& in );

      public:
         tWriteBin();
         ~tWriteBin();
         void operator()( const string& fn, vector<uint8_t> const& buf ) ;
         void operator()( const string& fn, uint8_t const* buf, size_t size ) ;
      };

   }// end of namespace bin_data

} // end of ns rlf

/*
  flags for ios::outmode

  ios::app,    to seek to the end of a stream before each insertion.
  ios::ate,    to seek to the end of a stream when its controlling object is first created.
  ios::binary, to read a file as a binary stream, rather than as a text stream.
  ios::in,     to permit extraction from a stream.
  ios::out,    to permit insertion to a stream.
  ios::trunc,  to delete contents of an existing file when its controlling object is created.

  */


#endif

//EOF

