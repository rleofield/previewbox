
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

/*! \file rimg_test_raw_rw.cpp
\brief impl for read and write test raw images

Lib: librimgrw
*/

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <boost/filesystem.hpp>

//#include "rimg_typedefs.h"
#include "rimg_test_raw_rw.h"

#include "rimg_planar.h"
#include "rimg_fs.h"
#include "rimg_rw_exceptions.h"


using namespace std;

using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_planar::tMask;

namespace rlf {

   namespace rimg_test_rw {


      namespace msg {
         const string marker = "%s";
         inline string replace( string const& msg, string const& s0 = "" ) {
            ::string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != ::string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }

         const string msg_file_not_exists = "File doesn't exist: '%s'";
         const string msg_read_file = " Couldn't read file '%s'";

         inline string read_file_( string const& s0 ) {
            return replace( msg_read_file, s0 );
         }
         inline string file_not_exists( string const& s0 ) {
            return replace( msg_file_not_exists, s0 );
         }

         const string msg_file_exists = "File exists: '%s'";
         const string msg_write_file = "Couldn't write file: '%s'";
         const string msg_open_file = "Couldn't open file for write: '%s'";
         const string msg_buffer_empty = "Buffer has zero size: '%s'";

         inline string write_file( string const& s0 ) {
            return replace( msg_write_file, s0 );
         }
         inline string file_exists( string const& s0 ) {
            return replace( msg_file_exists, s0 );
         }
         inline string file_open( string const& s0 ) {
            return replace( msg_open_file, s0 );
         }
         inline string buffer_empty( string const& s0 ) {
            return replace( msg_buffer_empty, s0 );
         }

      } // end of ns msg

      class rawData {
      public:
            rawData():mask(0),x(0),y(0),data(), len(0),txt(){}
         uint32_t mask;
         uint32_t x;
         uint32_t y;
         std::vector<uint8_t> data;
         uint32_t len;
         vector<uint8_t> txt;


      };

      inline char const* toCharPtr( vector<uint8_t> const& b ) {
         return reinterpret_cast<char const* >( static_cast<uint8_t const* >( &b[0] ) );
      }
      inline char const* toCharPtr( rawData const& b ) {
         return reinterpret_cast<char const* >( static_cast<rawData const* >( &b ) );
      }
      inline char const* toCharPtr( uint32_t const& b ) {
         return reinterpret_cast<char const* >( static_cast<uint32_t const* >( &b ) );
      }


      /*! tWriteRawEx,
      \param [in] msg  Message
      */
      class tWriteRawEx: public runtime_error {
      public:
         tWriteRawEx( const string& msg )
            : runtime_error( msg ) { }
      };


      class tWriteRaw  {
         tWriteRaw( const tWriteRaw& in );
         tWriteRaw& operator= ( const tWriteRaw& in );

      public:
         tWriteRaw() {}
         ~tWriteRaw() {}
         void operator()( const string& file, rawData const& data );
      };



      void tWriteRaw::operator()( const string& file, rawData const& data ) {

         ios_base::openmode mode = ios::out | ios::binary;
         ofstream fp( file.c_str(), mode );

         if( fp.bad() ) {
            throw tWriteRawEx( msg::file_open( file ) );
         }

         char const* p =  toCharPtr( data );
         fp.write( p, 12 );


         p = toCharPtr( data.data );
         fp.write( p, data.data.size() );

         // the desription
         p = toCharPtr( data.len );
         fp.write( p, 4 );

         p = toCharPtr( data.txt );
         fp.write( p, data.txt.size() );


         if( fp.bad() ) {
            throw tWriteRawEx( msg::write_file( file ) );
         }

      }

      // raw data, planer, own format, no standard
      //      planar = mask, y, x, data
      void write_raw_data( string const& fn, rlf::rimg_planar::tImgPlanar const& wimg, bool override_ ) {
         if( !override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         rawData data;
         data.mask  = wimg.mask().val();
         data.x = wimg.sx();
         data.y = wimg.sy();
         std::string s = wimg.description();
         data.len = static_cast<uint32_t>( s.length() );
         data.txt.assign( s.begin(), s.end() );
         data.data.reserve( data.x * data.y * wimg.mask().count() );
         auto mbegin = wimg.begin();
         auto mend = wimg.end();

         while( mbegin != mend ) {
            bool b = wimg.has_bit_at( mbegin );

            if( b ) {
               auto ybegin = mbegin->begin();
               auto yend = mbegin->end();

               while( ybegin != yend ) {
                  auto xbegin = ybegin->begin();
                  auto xend = ybegin->end();

                  while( xbegin != xend ) {
                     data.data.push_back( *xbegin );
                     ++xbegin;
                  }

                  ++ybegin;
               }
            }

            ++mbegin;
         }

         try {
            tWriteRaw()( fn, data );

         } catch( tWriteRawEx& ex ) {
            throw tImgWriteEx( ex.what() );
         }


      }

      inline char* toCharPtr( vector<uint8_t>& b ) {
         return reinterpret_cast<char* >( static_cast<uint8_t* >( &b[0] ) );
      }
      inline char* toCharPtr( rawData& b ) {
         return reinterpret_cast<char* >( static_cast<rawData* >( &b ) );
      }
      inline char* toCharPtr( uint32_t& b ) {
         return reinterpret_cast<char* >( static_cast<uint32_t* >( &b ) );
      }


      /*! tBinReadEx
      \param [in] msg  error message
      */
      class tRawReadEx: public runtime_error {
      public:
         tRawReadEx( const string& msg )
            : runtime_error( msg ) { }
      };

      /*! \class tReadBin
      *  \brief reads a binary file
      *
      */
      class tReadRaw  {
         tReadRaw& operator= ( const tReadRaw& in );
         tReadRaw( const tReadRaw& in );
      public:

         /*! \brief default ctor
         */
         tReadRaw() {}
         ~tReadRaw() {}
         /*! \brief read a file
         */
         void operator()( const string& file, rawData& data )  {
            boost::filesystem::path pfn( file );
            //uintmax_t size = boost::filesystem::file_size( pfn );

            ios_base::openmode mode = ios::in | ios::binary;
            ifstream fp( file.c_str(), mode );

            if( fp.bad() ) {
               string s = msg::read_file_( file );
               throw tRawReadEx( s );
            }


            // read mask, x, y
            char*   p =  toCharPtr( data );
            fp.read( p, 12 );

            tMask mask( data.mask );
            uint32_t x = data.x;
            uint32_t y = data.y;
            uint32_t size_ = x * y * mask.count();

            // read img
            data.data.resize( static_cast<size_t>( size_ ), 0 );
            p = toCharPtr( data.data );
            fp.read( p, size_ );

            if( fp.eof() ) {
               string s0 = msg::read_file_( file );
               throw tRawReadEx( s0 );
            }

            // the desription
            p = toCharPtr( data.len );
            fp.read( p, 4 );
            uint32_t len = data.len;

            if( fp.eof() ) {
               string s1 = msg::read_file_( file );
               throw tRawReadEx( s1 );
            }


            data.txt.resize( static_cast<size_t>( len ), 0 );
            p = toCharPtr( data.txt );
            fp.read( p, len );



            if( fp.eof() ) {
               string s = msg::read_file_( file );
               throw tRawReadEx( s );
            }

         }
      };

      void read_raw_data( string const& fn, rlf::rimg_planar::tImgPlanar& rimg ) {

         rawData data;

         try {
            tReadRaw()( fn, data );
         }  catch( tRawReadEx& ) {
            throw;
         }

         rimg.realloc( uint32_xy( data.x, data.y ), tMask( data.mask ) );

         auto mbegin = rimg.begin();
         auto mend = rimg.end();

         uint32_t offset = 0;

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {

               auto ybegin = mbegin->begin();
               auto yend = mbegin->end();

               while( ybegin != yend ) {
                  auto xbegin = ybegin->begin();
                  auto bdata = data.data.begin() + offset;
                  auto edata = data.data.begin() + offset + data.x;

                  copy( bdata, edata, xbegin );

                  ++ybegin;
                  offset += data.x;
               }


            }

            ++mbegin;
         }


         string s = string( data.txt.begin(), data.txt.end() );
         rimg.description( s );


      }

   }
}
//EOF





