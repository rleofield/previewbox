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

C++, GCC
File: rimg_rw_jpeg.cpp
// librimgrw
*/
/*! \file rimg_rw_jpeg.cpp
\brief impl for read and write jpeg images

Lib: librimgrw
*/


#include <iostream>
#include <sstream>
#include <string>
#include "rimg_rw_exceptions.h"


/*
* <setjmp.h> is used for the optional error recovery mechanism shown in
* the second part of the example.
*/
#include <setjmp.h>
#include "rimg_fs.h"
#include "jpeglib.h"



using namespace std;


#include "rimg_linear.h"
#include "rimg_convert.h"
#include "rimg_read_write_bin.h"
#include "rimg_read_write.h"
#include "rimg_t_read.h"
#include "rimg_t_write.h"

using namespace rlf::rimg_convert;
using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;
using rlf::t_read_jpeg;
using rlf::t_write_jpeg;

namespace rlf {

   namespace jpeg {
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

         const string msg_init_png_struct = "Couldn't init PNG structure: '%s'";
         const string msg_write =  "Couldn't write image data: '%s'";
         const string msg_read =  "Couldn't read image file: '%s'";
         const string msg_image_format_not_supported  = "Image format not supported (!8 and !24 bit)";
         inline string write( string const& s0 ) {
            return rlf::replace( msg_write, s0 );
         }
         inline string write_file( string const& s0 ) {
            return replace( msg_write, s0 );
         }
         inline string read_file( string const& s0 ) {
            return replace( msg_read, s0 );
         }
      }
   }



   namespace jpeg_glue {

      jpeg_error_mgr* std_error( jpeg_error_mgr* err ) {
         return ::jpeg_std_error( err );

      }

      void create_decompress( jpeg_decompress_struct* cinfo ) {

         ::jpeg_create_decompress( cinfo );
      }

      int jpegWriteScanlines( jpeg_compress_struct& cinfo, vector<uint8_t*> const& rows ) {
         if( rows.size() == 0 ) {
            return 0;
         }

         uint8_t** bp =  const_cast<uint8_t**>( &rows[0] );
         JDIMENSION dim = static_cast<JDIMENSION>( rows.size() );
         return jpeg_write_scanlines( &cinfo, bp, dim );
      }

   }

   namespace jpeg_local {


      void write( tImgViewLinear const& img, string fn, size_t quality ) {

         J_COLOR_SPACE color_space = JCS_UNKNOWN;

         if( img.is_mono8() ) {
            color_space = JCS_GRAYSCALE;
         }

         if( img.is_rgb() ) {
            color_space = JCS_RGB;
         }

         vector<uint8_t*> const& rows = img.rows();
         uint32_t h = img.size().h();
         uint32_t w = img.size().w();

         struct jpeg_compress_struct cinfo;
         struct jpeg_error_mgr jerr;

         cinfo.err = jpeg_glue::std_error( &jerr );
         jpeg_create_compress( &cinfo );

         unsigned long outbuffer_size;
         uint8_t* jpeg_buffer_raw = nullptr;
         ::jpeg_mem_dest( &cinfo, &jpeg_buffer_raw, &outbuffer_size );


         cinfo.in_color_space = color_space;
         jpeg_set_defaults( &cinfo );

         cinfo.image_width = w;
         cinfo.image_height = h;
         cinfo.input_components = 1;

         if( color_space == JCS_RGB ) {
            cinfo.input_components = 3;
         }

         //cinfo.data_precision = 32;

         /* Convert user 0-100 rating to percentage scaling */
         if( quality > 100 ) {
            quality = 100;
         }

         //cinfo.dct_method = JDCT_FASTEST;

         jpeg_set_quality( &cinfo, 90, TRUE );
         jpeg_start_compress( &cinfo, TRUE );

         // wrte image in one stroke to mem
         jpeg_glue::jpegWriteScanlines( cinfo, rows );

         jpeg_finish_compress( &cinfo );
         jpeg_destroy_compress( &cinfo );

         // write bin
         if( outbuffer_size == 0 ) {
            throw tImgWriteEx( bin_data::msg::buffer_empty( fn ) );
         }

         try {
            bin_data::tWriteBin()( fn, jpeg_buffer_raw, outbuffer_size );
         } catch( bin_data::tBinWriteEx& ) {
            if( jpeg_buffer_raw != nullptr ) {
               delete [] jpeg_buffer_raw;
               jpeg_buffer_raw = nullptr;
            }

            throw; //img_ex::t_ImgWriteEx( error_messages::bmp::write_file( path ) );
         }

         if( jpeg_buffer_raw != nullptr ) {
            delete [] jpeg_buffer_raw;
            jpeg_buffer_raw = nullptr;
         }

      }

      struct my_error_mgr {
         struct jpeg_error_mgr pub;
         jmp_buf setjmp_buffer;
      };


      /*
      * Here's the routine that will replace the standard error_exit method:
      */

      void
      my_error_exit( j_common_ptr cinfo ) {
         my_error_mgr* myerr = ( my_error_mgr* ) cinfo->err;
         ( *cinfo->err->output_message )( cinfo );

         /* Return control to the setjmp point */
         longjmp( myerr->setjmp_buffer, 1 );
      }

      void read( string const& fn, tImgLinear& img1 ) {


         vector<uint8_t> buf;

         try {
            bin_data::tReadBin()( fn, buf );
         }  catch( bin_data::tBinReadEx& ex ) {
            throw tImgReadEx( ex.what() );
         }


         jpeg_decompress_struct cinfo;
         struct my_error_mgr jerr;

         cinfo.err = jpeg_glue::std_error( &jerr.pub );
         jerr.pub.error_exit = my_error_exit;

         if( setjmp( jerr.setjmp_buffer ) ) {
            /* If we get here, the JPEG code has signaled an error.
            * We need to clean up the JPEG object, close the input file, and return.
            */
            jpeg_destroy_decompress( &cinfo );
            //fclose( infile );
            throw  tImgReadEx( jpeg::msg::read_file( fn ) );
         }

         jpeg_glue::create_decompress( &cinfo );


         uint8_t* pbuf = &buf[0];
         jpeg_mem_src( &cinfo, pbuf, buf.size() );

         jpeg_read_header( &cinfo, TRUE );

         img1.set_mono8();

         if( cinfo.num_components == 3 ) {
            img1.set_rgb();
         }

         img1.size() = tSize( cinfo.image_width, cinfo.image_height );
         img1.alloc_data_buffer();


         ( void ) jpeg_start_decompress( &cinfo );

         int row_stride;     /* physical row width in output buffer */
         row_stride = cinfo.output_width * cinfo.output_components;
         JSAMPARRAY buffer = ( *cinfo.mem->alloc_sarray )
                             ( ( j_common_ptr ) &cinfo, JPOOL_IMAGE, row_stride, 1 );


         /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
         uint32_t offset = 0;

         while( cinfo.output_scanline < cinfo.output_height ) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
            * Here the array is only one element long, but you could ask for
            * more than one scanline at a time if that's more convenient.
            */
            ( void ) jpeg_read_scanlines( &cinfo, buffer, 1 );
            //vector<uint8_t> v( buffer[0], buffer[0] + row_stride );
            img1.insert_data( *buffer, row_stride, offset );
            offset += row_stride ;
         }

         ( void ) jpeg_finish_decompress( &cinfo );

         jpeg_destroy_decompress( &cinfo );


      }

   }



   void t_write_jpeg::write( string const& fn, tImgViewLinear const& img ) {
      if( !img.is_mono8() &&  !img.is_rgb() ) {
         throw tImgWriteEx( jpeg::msg::msg_image_format_not_supported );
      }

      try {
         jpeg_local::write( img, fn, c() );
      } catch( tImgWriteEx& ex ) {
         throw ex;
      }
   }
   void t_write_jpeg::write( string const& fn, tImgPlanar const& img )   {
      tImgLinear temp;
      convert( img, temp );
      tImgViewLinear v( temp );
      write( fn, v ) ;
   }


   void t_read_jpeg::read( tImgLinear& img ) {


      try {
         jpeg_local::read( _fn, img );
      } catch( rImgEx& ) {
         throw;
      }
   }

   // read write planar, simple forward to tImgLinear
   void t_read_jpeg::read( tImgPlanar& planar )  {

      tImgLinear img_read;
      read( img_read );
      tImgViewLinear vlin( img_read );
      convert( vlin, planar );
   }



}
//EOF






