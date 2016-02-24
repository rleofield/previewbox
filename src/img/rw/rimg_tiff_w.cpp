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
/*! \file rimg_rw_tiff.cpp
\brief impl for read and write tif images

Lib: librimgrw
*/

#include <limits>


#include <iostream>
#include <sstream>
#include <string>




#include "rimg_read_write_bin.h"


#include "rimg_linear.h"
//#include "rimg_planar.h"
#include "rimg_convert.h"


#include "rimg_read_write.h"
#include "rimg_fs.h"
#include "rimg_rw_exceptions.h"

#include "stringhelper.h"
#include "tLog_Category_tiff.h"

#include "rimg_tiff.h"

using namespace std;
using std::vector;
using namespace rlf::rimg_convert;

using rlf::cTiffException;

using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;
using rlf::t_write_tiff;
using rlf::t_read_tiff;
using rlf::tTiff;
using rlf::cTiffEntryWrite;
using rlf::planar_cfg;
using rlf::phi;


namespace rlf {






   namespace {

      const string marker = "%s";
#ifdef USE_THIS
      string replace( string const& msg, string const& s0 ) {
         string temp = msg;

         if( s0.size() > 0 ) {
            size_t pos = msg.find( marker );

            if( pos != string::npos ) {
               temp.erase( pos, marker.size() );
               temp.insert( pos, s0 );
            }
         }

         return temp;
      }
#endif

#ifdef USE_THIS
      const string msg_write_file = "Couldn't write file: '%s'";
      string write_file( string const& s0 ) {
         return replace( msg_write_file, s0 );
      }
#endif


   }




   static void set_uint16_at( uint8_t* buf_, uint16_t val ) {
      memcpy( buf_, &val, 2 );
   }







   void write( string const& file, tImgViewLinear const& img )  {

      // Check for supported file types
      if( !img.is_mono8() &&  !img.is_rgb() ) {
         throw tImgWriteEx( msg_image_format_not_supported );
      }

      vector<uint8_t> b;
      tImgLinear l;
      tTiff tif( b, l );
      // defaults  == Gray images
      tif.bits_gray( 8 );
      tif.bits_red( 0 );
      tif.bits_green( 0 );
      tif.bits_blue( 0 );
      tif.samples( 1 );
      tif.planar_configuration( planar_cfg::none /* 0 */ );

      /* -- initialize parameters
      */

      if( img.is_mono8() ) {
         tif.photometric_interpretation( phi::GRAY_black_is_min );
      }

      if( img.is_rgb() != false ) {
         tif.photometric_interpretation( phi::RGB );
         tif.planar_configuration( planar_cfg::RGBRGBRGB /* 1 */ );   // RGBRGBRGB...
         tif.bits_gray( 0 );
         tif.bits_red( 8 );
         tif.bits_green( 8 );
         tif.bits_blue( 8 );
         tif.samples( 3 );
      }

      tif.size( img.size() );
      tif.description( img.description() );

      try {
         tif.write( file, img );
      } catch( cTiffException& ) {
         throw tImgWriteEx( msg_image_format_not_supported );
      }
   }

   void WriteToFile( string const& file, vector<uint8_t> const& b );

   void tTiff::write( string const& file, tImgViewLinear const& im ) {
      uint32_t string_offset;
      uint8_t* dir_ptr;
      uint32_t color_map_offset;
      //int l;

      uint32_t entry_count = 0;

      if( data.photometric_interpr == phi::GRAY_black_is_min ) { // 1
         // 1. new sub file
         // 2. width
         // 3. length
         // 4. bits per sample
         // 5. compression
         // 6. phi
         // 7. description
         // 8. stripe offsets
         // 9. samples per pixel
         // 10. rows per strip
         // 11. strip byte_t counts

         // planar configuration, nur RGB
         // colormap, nur Palette
         entry_count = 11;    /* 11 dir entries, 8,16 bit, gray    */
      }

      if( data.photometric_interpr == phi::RGB ) { // 2
         entry_count = 12;    /* 12 dir entries, 11 + einmal 24 bit      */
      }

      if( data.photometric_interpr == phi::PALETTE ) { // 3
         entry_count = 12;    /* 12 dir entries, 11 + einmal 8 bit colormap */
      }


      /* -- full size stripes
        */
      data.rows_per_strip =  data.get_size().y();
      //data._stripes      =  1;
      uint32_t strip_byte_counts =  data.pixels() * data.samples;

      if( data.bits_gray == 16 ) {
         strip_byte_counts *= 2;
      }


      /* -- buffer for the pointer and data
        */

      // -- calculate data size
      uint32_t data_size = 4 + Size::LONG;                     // -- first 4 + dir offset

      uint32_t dir_entry_offset = data_size;

      data_size += Size::SHORT;                       // -- entry count
      uint32_t dir_offset = data_size;

      data_size += ( entry_count + 1 ) * Size::DIR_ENTRY; // -- tiff dir
      data_size = tiff_align32( data_size );

      string_offset = data_size;




      /* -- tag 0x10E  -- Image Description
        */
      size_t namelength = data.description.length() + 1;

      if( namelength > 4 ) {
         data_size += static_cast<uint32_t>( namelength );
      }

      /* -- tag 0x102  -- Bits Per Sample
        */
      uint32_t bits_per_sample_offset = data_size;

      if( data.samples == 3 ) {
         data_size += 3 * Size::SHORT;
      }

      data_size = tiff_align32( data_size );


      /* -- tag 0x140  -- Colormap
        */
      int colormap_size = number_of_colors * 3 ;
      color_map_offset  = data_size;

      if( data.photometric_interpr == phi::PALETTE ) { // 3
         data_size  += static_cast<uint32_t>(colormap_size * sizeof( uint16_t ));
      }

      data_size = tiff_align32( data_size );

      /* -- img
        */
      uint32_t img_offset = data_size;

      if( data.bits_gray == 16 ) {
         data_size += 2 * data.pixels() * data.samples;
      } else {
         data_size += data.pixels() * data.samples;
      }


      vector< uint8_t > wbuf( data_size );
      uint8_t* ptrbuf = &wbuf[0];

      /* -- write first 4 Bytes
        */
      *( ptrbuf )   = 'I';
      *( ptrbuf + 1 ) = 'I';
      *( ptrbuf + 2 ) = '*';
      *( ptrbuf + 3 ) = '\0';

      /* -- write offset of directory
        */
      memcpy( ptrbuf + 4, &dir_entry_offset, Size::LONG );

      memcpy( ptrbuf + dir_entry_offset, &entry_count, Size::SHORT );


      uint32_t dir_count = 0;
      dir_ptr    = ptrbuf + dir_offset;


      cTiffEntryWrite entry;

      /* -- 0. tag 0xFE    -- new subfile type (0xFE, 254)"
        */
      entry.ifd_entry.tag    = tags::new_subfile_type; /*  0xFE */
      entry.ifd_entry.field_type   = field_type::long_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value = 0    ;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 1. tag 0x100    -- Image Width (0x100,256)
        */
      entry.ifd_entry.tag    = tags::image_width;
      entry.ifd_entry.field_type   = field_type::long_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value = data.get_size().x();
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 2. tag 0x101    -- Image Length (0x101,257)
        */
      entry.ifd_entry.tag    = tags::image_length;
      entry.ifd_entry.field_type   = field_type::long_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value = data.get_size().y();
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 3. tag 0x102  -- Bits Per Sample (0x102,258)
        */
      entry.ifd_entry.tag    = tags::bits_per_sample;
      entry.ifd_entry.field_type   = field_type::short_ ;

      if( data.samples == 3 ) {
         uint8_t* data1 = ( ptrbuf + bits_per_sample_offset );
         entry.ifd_entry.count = 3    ;
         entry.ifd_entry.offset_or_value = bits_per_sample_offset;
         set_uint16_at( data1, static_cast< uint16_t> (data.bits_red) );
         data1 += 2;
         set_uint16_at( data1, static_cast< uint16_t> (data.bits_green ) );
         data1 += 2;
         set_uint16_at( data1, static_cast< uint16_t> (data.bits_blue ) );
         data1 += 2;
      } else {
         entry.ifd_entry.count = 1    ;
         entry.ifd_entry.offset_or_value = static_cast< uint32_t> (data.bits_gray) & 0xffff;
      }

      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;


      /* -- 4. tag 0x103  -- Compression (0x103,259)
        */
      entry.ifd_entry.tag    = tags::compression;
      entry.ifd_entry.field_type   = field_type::short_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value =static_cast< uint32_t> (data.compression) & UINT16_MAX;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 5. tag 0x106  -- Photometric Interpretation (0x106,262)
        */
      entry.ifd_entry.tag    = tags::photometric_interpretation;
      entry.ifd_entry.field_type   = field_type::short_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value =static_cast< uint32_t> (data.photometric_interpr) & UINT16_MAX;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 6. tag 0x10E  -- Image Description (0x10E,270)
        */
      entry.ifd_entry.tag    = tags::description;
      entry.ifd_entry.field_type   = field_type::ascii;

      uint32_t descriptionlength = static_cast<uint32_t>( data.description.length() + 1 );

      if( descriptionlength > 4 ) {
         strcpy( reinterpret_cast<char*>( ptrbuf + string_offset ), data.description.c_str() );
         entry.ifd_entry.count = descriptionlength;
         entry.ifd_entry.offset_or_value = string_offset;
         string_offset += descriptionlength;
      } else {
         entry.ifd_entry.count = descriptionlength;
         strcpy( reinterpret_cast<char*>( &( entry.ifd_entry.offset_or_value ) ), data.description.c_str() );
      }

      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;


      /* -- 7. tag 0x111  -- Stripe Offsets (0x111,273)
        */
      if( img_offset < Size::MAX_SHORT ) {
         entry.ifd_entry.tag    = tags::strip_offsets;
         entry.ifd_entry.field_type   = field_type::short_;
         entry.ifd_entry.count = 1;
         entry.ifd_entry.offset_or_value = img_offset;
      } else {
         entry.ifd_entry.tag    = tags::strip_offsets;
         entry.ifd_entry.field_type   = field_type::long_;
         entry.ifd_entry.count = 1;
         entry.ifd_entry.offset_or_value = img_offset;
      }

      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 8. tag 0x115  -- Samples per Pixel (0x115,277)
        */
      entry.ifd_entry.tag    = tags::samples_per_pixel;
      entry.ifd_entry.field_type   = field_type::short_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value = data.samples;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 9. tag 0x116  -- Rows Per Strip (0x116,278)
        */
      entry.ifd_entry.tag    = tags::rows_per_strip;
      entry.ifd_entry.field_type   = field_type::long_;
      entry.ifd_entry.count = 1    ;
      entry.ifd_entry.offset_or_value = data.rows_per_strip;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;

      /* -- 10. tag 0x117  -- Strip byte_t Counts (0x117,279)
        */
      entry.ifd_entry.tag    = tags::strip_byte_counts;
      entry.ifd_entry.field_type   = field_type::long_ ;
      entry.ifd_entry.count = 1;
      entry.ifd_entry.offset_or_value = strip_byte_counts;
      entry.ToBuffer( dir_ptr );
      dir_ptr += Size::DIR_ENTRY;
      dir_count++;



      /* -- Planar Configuration   -- 24 bit images, (0x11C,273)
        //      tif.photometric_interpr = 2;
        //      tif.mPlanarConfiguration     = 1;   // RGBRGBRGB...
        */
      if( data.planar_configuration == planar_cfg::RGBRGBRGB    /* 1 */
            &&
            data.photometric_interpr == phi::RGB /* 2 */
        ) {

         entry.ifd_entry.tag    = tags::planar_configuration;
         entry.ifd_entry.field_type   = field_type::short_;
         entry.ifd_entry.count = 1    ;
         entry.ifd_entry.offset_or_value =
            static_cast< unsigned long >(data.planar_configuration) & UINT16_MAX;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY;
         dir_count++;
      }

      if( data.photometric_interpr == phi::PALETTE ) {

         /* -- 11. tag 0x140  -- Colormap (0x140,320)
          */
         entry.ifd_entry.tag    = tags::colormap;
         entry.ifd_entry.field_type   = field_type::short_;
         entry.ifd_entry.count = colormap_size;
         entry.ifd_entry.offset_or_value = color_map_offset;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY;
         dir_count++;
      }

      /* -- last entry == 0
        */
      entry.ifd_entry.tag    = 0;
      entry.ifd_entry.field_type   = 0;
      entry.ifd_entry.count = 0;
      entry.ifd_entry.offset_or_value = 0;
      entry.ToBuffer( dir_ptr );

      /* -- copy the colormap
        */
      if( data.photometric_interpr == phi::PALETTE ) { // 3
         uint16_t* plut = reinterpret_cast<uint16_t*> ( ptrbuf + color_map_offset );
         size_t count = 0;
         size_t nr = number_of_colors;

         vector<tRGBA> v( 256 );

         for( count = 0; count < nr; count++ ) {
            uint16_t temp = 0;
            temp = v[ count ].r();
            temp = static_cast<uint16_t>( temp << 8 );
            *( plut + 0 * nr + count )  = temp;
            temp = v[ count ].g();
            temp = static_cast<uint16_t>( temp << 8 );
            *( plut + 1 * nr + count )  = temp;
            temp = v[ count ].b();
            temp = static_cast<uint16_t>( temp << 8 );
            *( plut + 2 * nr + count )  = temp;
         }
      }

      /* -- write number of entries
        */
      entry_count = dir_count;

      /* -- copy the image
        */
      if( data.samples == 1 && data.bits_gray == 8 ) {
         uint8_t* p8 = ptrbuf + img_offset;
         size_t sx = im.pitch();

         for( size_t iy = 0; iy < im.size().y(); iy++ ) {
            uint8_t const* buf1 = im.row_ptr( iy );
            memcpy( p8, buf1, im.size().x() );
            p8 += sx;
         }
      }

      if( data.samples == 3 ) {
         uint8_t* ptr = ptrbuf + img_offset;

         for( size_t iy = 0; iy < im.size().y(); iy++ ) {
            // nnn
            tRGB const* source = reinterpret_cast< tRGB const*>( im.row_ptr( iy ) );
            tRGB* target = reinterpret_cast< tRGB* >(ptr);

            size_t count = im.size().x();
            size_t index = 0;

            while( index < count ) {
               *target = *source;
               target++;
               source++;
               index++;
            }

            ptr += data.get_size().x() * data.samples;
         }
      }

      WriteToFile( file,  wbuf );
   }


   void WriteToFile( string const& file, vector<uint8_t> const& b ) {

      try {
         bin_data::tWriteBin()( file, b );
      } catch( bin_data::tBinWriteEx& ) {
         throw  cTiffException( " write_file failed " , lfm_tif );
      }
   }


   void cTiffEntryWrite::ToBuffer( uint8_t* buf )const {
      memcpy( buf + Offset::TAG   , &ifd_entry.tag,    Size::SHORT );
      memcpy( buf + Offset::TYPE  , &ifd_entry.field_type,   Size::SHORT );
      memcpy( buf + Offset::LENGTH, &ifd_entry.count, Size::LONG );
      memcpy( buf + Offset::OFFSET, &ifd_entry.offset_or_value, Size::LONG );
   }


   void t_write_tiff::write( string const& fn, tImgViewLinear const& img )   {
      // Check for supported file types
      if( !img.is_mono8() &&  !img.is_rgb() ) {
         throw tImgWriteEx( rlf::msg_image_format_not_supported );
      }

      rlf::write( fn, img );
   }
   void t_write_tiff::write( string const& fn, tImgPlanar const& img )   {
      tImgLinear temp;
      convert( img, temp );
      tImgViewLinear v( temp );
      write( fn, v ) ;
   }



}


//EOF





