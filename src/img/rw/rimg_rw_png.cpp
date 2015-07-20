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

/*! \file rimg_rw_png.cpp
\brief impl for read and write png images

Lib: librimgrw
*/


#include <iostream>
#include <sstream>
#include <string>
#include <boost/filesystem.hpp>

#include "rimg_rw_exceptions.h"
#include "rimg_read_write.h"


#include "png.h"
#include "zlib.h"



using namespace std;


#include "rimg_read_write_bin.h"
#include "rimg_linear.h"
#include "rimg_convert.h"
#include "rimg_fs.h"
#include "rimg_rw_exceptions.h"
#include "rimg_t_read.h"
#include "rimg_t_write.h"


using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_linear::tImgViewLinear;

using namespace rlf::rimg_convert;
using rlf::t_read_png;
using rlf::t_write_png;

namespace rlf {

   namespace png {
      namespace msg {

         const string marker = "%s";
         string replace( string const& msg, string const& s0 = "" ) {
            string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( msg::marker );

               if( pos != ::string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }

         const string msg_file_not_exist = "File doesn't exist: '%s'";
         const string msg_open_file = "Couldn't open file: '%s'";

         const string msg_signature =  "Wrong PNG signature in file: '%s'";
         const string msg_init_png_struct = "Couldn't init PNG structure: '%s'";
         const string msg_allocate =  "Couldn't allocate image data: '%s'";
         const string msg_image_format_not_supported  = "Image format not supported (!8 and !24 bit)";

         inline string file_not_exist( string const& s0 ) {
            return replace( msg_file_not_exist, s0 );
         }
         inline string open_file( string const& s0 ) {
            return replace( msg_open_file, s0 );
         }
         inline string signature( string const& s0 ) {
            return replace( msg_signature, s0 );
         }
         inline string init_png_struct( string const& s0 ) {
            return replace( msg_init_png_struct, s0 );
         }
         inline string allocate( string const& s0 ) {
            return replace( msg_allocate, s0 );
         }

      }
   }

   namespace {

      // small helpers to provide a FILE* for old style code
      class filePtrReadBin {
         filePtrReadBin( filePtrReadBin const& );
         void operator=( filePtrReadBin const& );
      public:
         FILE* fp;
         filePtrReadBin( string const& f ): fp( nullptr ) {
            fp = fopen( f.c_str(), "rb" );

         }
         ~filePtrReadBin() {
            if( fp ) {
               fclose( fp );
            }
         }

      };
      class filePtrWriteBin {
         filePtrWriteBin( filePtrWriteBin const& );
         void operator= ( filePtrWriteBin const& );
      public:
         FILE* fp;
         filePtrWriteBin( string const& f ): fp( nullptr ) {
            fp = fopen( f.c_str(), "wb" );

         }
         ~filePtrWriteBin() {
            if( fp ) {

               fclose( fp );
            }
         }

      };





   }

   namespace png_local {

      const size_t SIGNATURE_BYTES_COUNT = 8;

      void write( string const& file_name, tImgViewLinear const& img, int compression ) {

         int color_type = PNG_COLOR_TYPE_GRAY;
         int bit_depth = 8;

         if( img.is_rgb() ) {
            // Image RGB24
            color_type = PNG_COLOR_TYPE_RGB;
            bit_depth = 8;
         }


         filePtrWriteBin f( file_name );

         if( !f.fp ) {
            throw tImgWriteEx( "Couldn't open file: '" + file_name + "'" );
         }

         png_structp png_ptr = ::png_create_write_struct(
                                  PNG_LIBPNG_VER_STRING,
                                  ( png_voidp )nullptr,
                                  ( png_error_ptr )nullptr,
                                  ( png_error_ptr )nullptr );

         if( !png_ptr ) {
            throw tImgWriteEx( "Couldn't create PNG write struct: '" + file_name + "'" );
         }

         png_infop info_ptr = ::png_create_info_struct( png_ptr );

         if( !info_ptr ) {
            ::png_destroy_write_struct( &png_ptr, ( png_infopp )NULL );
            throw tImgWriteEx( "Couldn't create PNG info struct: '" + file_name + "'" );
         }

         png_init_io( png_ptr, f.fp );

         // Store PNG compressed or uncompressed, as requested by user
         png_set_compression_level( png_ptr, compression );
         png_set_compression_strategy( png_ptr, Z_RLE );

         png_set_IHDR( png_ptr, info_ptr, img.size().w(), img.size().h(),
                       bit_depth, color_type, PNG_INTERLACE_NONE,
                       PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );

         png_write_info( png_ptr, info_ptr );

         if( bit_depth == 16 ) {
            ::png_set_swap( png_ptr );
         }

         if( color_type == PNG_COLOR_TYPE_RGB ) {
            //png_set_bgr( png_ptr );
         }

         for( size_t y = 0; y < img.size().y(); y++ ) {
            uint8_t const* p = img.row_ptr( y );
            ::png_write_row( png_ptr, p );
         }

         ::png_write_end( png_ptr, nullptr );


      }


      class tPNGReader {

         png_struct* png_ptr;
         png_info* info_ptr;
         png_info* end_info;
         FILE* fp;

         tPNGReader( tPNGReader const& );
         void operator=( tPNGReader const& );
      public:
         uint32_t width;
         uint32_t height;
         int bit_depth;
         int color_type;
         int interlace_type;
         int compression_type;
         int filter_method;

         tPNGReader( FILE* f ):
            png_ptr( nullptr ),
            info_ptr( nullptr ),
            end_info( nullptr ),
            fp( f ),
            width( 0 ),
            height( 0 ),
            bit_depth( 0 ),
            color_type( 0 ),
            interlace_type( 0 ),
            compression_type( 0 ),
            filter_method( 0 )

         {
         }
         bool init() {
            png_ptr  = ::png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr );

            if( png_ptr == nullptr ) {
               return false;
            }

            info_ptr = png_create_info_struct( png_ptr );

            if( info_ptr == nullptr ) {
               return false;
            }

            end_info  = png_create_info_struct( png_ptr );

            if( end_info == nullptr ) {
               return false;
            }

            return true;
         }

         ~tPNGReader() {
            if( png_ptr != nullptr && info_ptr != nullptr && end_info != nullptr ) {
               png_destroy_read_struct( &png_ptr, &info_ptr, &end_info );
               png_ptr = nullptr;
               info_ptr = nullptr;
               end_info = nullptr;
            }

            if( png_ptr != nullptr && info_ptr != nullptr ) {
               png_destroy_read_struct( &png_ptr, &info_ptr, nullptr );
               png_ptr = nullptr;
               info_ptr = nullptr;
               end_info = nullptr;
            }

            if( png_ptr != nullptr ) {
               png_destroy_read_struct( &png_ptr, nullptr, nullptr );
            }
         }


         void read() {
            int transforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_STRIP_ALPHA | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND;
            // Set IO and declare already used signature bytes
            png_init_io( png_ptr, fp );
            png_set_sig_bytes( png_ptr, SIGNATURE_BYTES_COUNT );



            png_voidp params = NULL;
            png_read_png( png_ptr, info_ptr, transforms, params );

            // Query image info
            png_get_IHDR( png_ptr, info_ptr, &width, &height,
                          &bit_depth, &color_type, &interlace_type,
                          &compression_type, &filter_method );
         }

         vector<uint8_t*> row_pointers() {
            uint8_t**  pp =  png_get_rows( png_ptr, info_ptr );
            vector<uint8_t*> rows( pp, pp  + height );
            return rows;
         }


      };


      void read( string const& file, tImgLinear& rimg ) {

         filePtrReadBin f( file ); //open and close fp

         if( !f.fp ) {
            throw tImgReadEx( png::msg::open_file( file ) );
         }

         // Check for PNG signature
         uint8_t header[SIGNATURE_BYTES_COUNT];
         fread( header, 1, SIGNATURE_BYTES_COUNT, f.fp );

         if( 0 != png_sig_cmp( header, 0, SIGNATURE_BYTES_COUNT ) ) {
            throw tImgReadEx( png::msg::signature( file ) );
         }

         tPNGReader reader( f.fp );

         if( reader.init() == false ) {
            throw tImgReadEx( png::msg::init_png_struct( file ) );
         }

         reader.read();

         rimg.size() = tSize( reader.width, reader.height );

         if( reader.color_type == PNG_COLOR_TYPE_GRAY && reader.bit_depth == 8 ) {
            rimg.set_mono8();
         } else {
            // 3 x 8 bit == rgb
            if( reader.color_type == PNG_COLOR_TYPE_RGB && reader.bit_depth == 8 ) {
               rimg.set_rgb();
            } else {
               throw tImgReadEx( " PNG Image is not 8 bit or 24 bit" );

            }
         }

         try {
            rimg.alloc_data_buffer();
         } catch( tImgAllocEx& ) {
            throw tImgReadEx( png::msg::allocate( file ) );
         }

         uint32_t img_x = rimg.size().x() * 3;

         if( rimg.is_mono8() ) {
            img_x = rimg.size().x();
         }

         for( uint32_t y = 0; y < rimg.size().y(); y++ ) {
            uint8_t* row = rimg.row_ptr( y );
            memcpy( row, reader.row_pointers()[y], img_x );
         }

      }

   } // end of ns png_local


   //namespace rimg_png {
   void t_write_png::write( string const& fn, tImgViewLinear const& img ) {
      if( !img.is_mono8() &&  !img.is_rgb() ) {
         throw tImgWriteEx( png::msg::msg_image_format_not_supported );
      }

      png_local::write( fn, img, c() );
   }

   void t_write_png::write( string const& fn, tImgPlanar const& img )   {
      tImgLinear temp;
      convert( img, temp );
      tImgViewLinear v( temp );
      write( fn, v ) ;
   }


   void t_read_png::read( tImgLinear& img )  {
      try {
         png_local::read( _fn, img );
         return;
      } catch( rImgEx& ex ) {

         throw ex;
      }
   }

   // read write planar, simple forward to tImgLinear
   void t_read_png::read( tImgPlanar& planar )  {

      tImgLinear img_read;
      read( img_read );
      tImgViewLinear vlin( img_read );
      convert( vlin, planar );
   }



}
//EOF





