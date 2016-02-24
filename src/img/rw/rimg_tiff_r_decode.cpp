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

using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;
using rlf::t_write_tiff;
using rlf::t_read_tiff;
using rlf::bin_data::tReadBin;

using rlf::cTiffException;

using rlf::tTiffData;
using rlf::tTiff;
using rlf::cTiffEntryBase;
using rlf::cTiffEntry;
using rlf::cTiffEntryWrite;
using rlf::planar_cfg;
using rlf::phi;


namespace rlf {





   namespace {
      //      const string marker = "%s";
      //      string replace( string const& msg, string const& s0 ) {
      //         string temp = msg;

      //         if( s0.size() > 0 ) {
      //            size_t pos = msg.find( marker );

      //            if( pos != string::npos ) {
      //               temp.erase( pos, marker.size() );
      //               temp.insert( pos, s0 );
      //            }
      //         }

      //         return temp;
      //      }

      //      const string msg_write_file = "Couldn't write file: '%s'";
      //      string write_file( string const& s0 ) {
      //         return replace( msg_write_file, s0 );
      //      }


      const string msg_image_format_not_supported  = "Image write failed";

   }



   bool tTiff::decode_entry( cTiffEntry& e ) {

      tTiffTag t_tag    = e.tiff_tag();
      uint16_t tag    = e.ftag();
      uint16_t t_type   = e.ifd_entry.field_type;
      uint32_t t_count   = e.ifd_entry.count;
      uint32_t offset_or_value   = e.ifd_entry.offset_or_value;
      uint16_t const* uint16Ptr = reinterpret_cast<uint16_t const*> ( & ( e.ifd_entry.offset_or_value ) );
      uint16_t short_array[2];
      short_array[0] = *uint16Ptr;
      uint16Ptr++;
      short_array[1] = *uint16Ptr;

      if( _do_swap ) {
         swap( short_array[0], short_array[1] );
      }



      string s;
      string tt ;
      s =  " tag     " +  t_tag.to_string();
      tt = s;
      s += ", type   " + type_to_string( t_type );
      s += ", length " + stringify( t_count );
      s += ", offset " + stringify( offset_or_value );
      s += ", soffset0 " + stringify( short_array[0] );
      s += ", soffset1 " + stringify( short_array[1] );
      LOGT_TIFF_DEBUG( s );
      tagloglines.push_back( s );



      /* -- new subfile type (0xFE, 254)"
              */
      if( t_tag == tags::new_subfile_type ) {
         if( t_type   != field_type::long_ ) {
            return  false;
         }

         if( t_count != 1 )    {
            return false;
         }

         return true;
      }


      /* -- subfile type (0xFF,255)
              */
      if( t_tag  == tags::subfile_type ) {
         return true;
      }

      /* -- Image Width (0x100,256)
              */
      if( t_tag == tags::image_width ) {
         if( t_type == field_type::short_ ) {
            data.sx( static_cast< uint32_t > (short_array[0] ) );
            return true;
         }

         if( t_type == field_type::long_ ) {
            data.sx( offset_or_value );
            return true;
         }

         return false;
      }

      /* -- Image Length (0x101,257)
              */
      if( t_tag == tags::image_length ) {
         if( t_type == field_type::short_ ) {
            data.sy( short_array[0] );
            return true;
         }

         if( t_type == field_type::long_ ) {
            data.sy( offset_or_value );
            return true;
         }

         return false;
      }


      /* -- Bits Per Sample (0x102,258)
              */
      if( t_tag == tags::bits_per_sample ) {
         if( t_type == field_type::short_ ) {
            data.samples = 0;
            data.bits_gray = 0;
            data.bits_red = 0;
            data.bits_green = 0;
            data.bits_blue = 0;
            data.bits_alpha = 0;

            if( t_count == 1 ) {
               data.samples = 1;
               data.bits_gray = short_array[0];
               return true;
            }

            if( t_count == 2 ) {
               data.samples = 1;
               data.bits_gray = short_array[0];
               return true;
            }

            if( t_count >= 3 ) {
               data.samples = 3;

               data.bits_red   = uint16_at( offset_or_value + 0 ) ;
               data.bits_green = uint16_at( offset_or_value + 2 ) ;
               data.bits_blue  = uint16_at( offset_or_value + 4 ) ;

               if( t_count == 4 ) {       // for 32 bit image, not supported
                  data.samples = 4;
                  data.bits_alpha = uint16_at( offset_or_value + 6 );
               }

               return true;
            }
         }

         return false;
      }


      /* -- Compression (0x103,259)
              */
      if( t_tag  == tags::compression ) {
         if( t_type == field_type::short_ ) {
            data.compression = short_array[0];

            if( data.compression != 1 ) {
               string s1 = "Compression is not 1, others are not supported.";
               throw cTiffException( s1, lfm_tif );
            }

            return true;
         }

         return false;
      }


      /* -- Photometric Interpretation (0x106,262)
              */
      if( t_tag == tags::photometric_interpretation ) {
         if( t_type == field_type::short_ ) {
            data.photometric_interpr = short_array[0];
            return true;
         }

         return false;
      }


      /* -- Description (0x10E,270)
              */
      if( t_tag == tags::description ) { /* 270 */
         if( t_type == field_type::ascii ) {
            uint8_t const* p = nullptr;

            if( t_count > 4 ) {
               p = const_ptr_at( offset_or_value );
            } else {
               p = reinterpret_cast< uint8_t const*> (&offset_or_value);
            }

            data.description = string( reinterpret_cast<char const*>( p), t_count );

            return true;
         }

         return false;
      }

      /* -- Document name (0x10D,269)
              */
      if( t_tag == tags::document_name ) { /* 270 */
         if( t_type == field_type::ascii ) {
            uint8_t const* p = nullptr;

            if( t_count > 4 ) {
               p = const_ptr_at( offset_or_value );
            } else {
               p = reinterpret_cast< uint8_t const* >(&offset_or_value);
            }

            data.document_name = string( reinterpret_cast<char const*>(p), t_count );

            return true;
         }

         return false;
      }


      /* -- Stripe Offsets (0x111,273)
        */
      if( t_tag == tags::strip_offsets ) {

         data.strip_offsets.resize( t_count );

         if( t_type == field_type::long_ ) {
            if( t_count == 1 ) {
               uint32_t value = offset_or_value;
               data.strip_offsets[0] = value;
               return true;
            }

            // is offset to array of offsets
            uint32_t offset = offset_or_value;

            for( uint32_t i = 0; i < t_count; i++ ) {
               data.strip_offsets[i] = uint32_at( offset );
               offset += Size::LONG;
            }

            return true;
         }

         if( t_type == field_type::short_ ) {


            if( t_count == 1 ) {
               uint32_t value = short_array[0];
               data.strip_offsets[0] = value;
               return true;
            }

            if( t_count == 2 ) {
               data.strip_offsets[0] = short_array[0];
               data.strip_offsets[1] = short_array[1];
               return true;
            }

            // is offset to short array of offsets
            uint32_t off = short_array[0];

            for( uint32_t i = 0; i < t_count; i++ ) {
               data.strip_offsets[i] = uint16_at( off );
               off += Size::SHORT;
            }

            return true;
         }

         return false;
      }


      /* -- Samples per Pixel (0x115,277)
              */
      if( t_tag == tags::samples_per_pixel ) {
         if( t_type == field_type::short_ ) {
            data.samples = short_array[0];

            if( data.samples == 2 ) {
               data.samples = 1;
            }

            return true;
         }

         return false;
      }

      /* -- Rows Per Strip (0x116,278)
              */
      if( t_tag == tags::rows_per_strip ) {
         if( t_type == field_type::long_ ) {
            data.rows_per_strip = offset_or_value;
            return true;
         }

         if( t_type == field_type::short_ ) {
            data.rows_per_strip = short_array[0];
            return true;
         }

         return false;
      }

      /* -- Strip byte_t Counts (0x117,279)
              */
      if( t_tag == tags::strip_byte_counts ) {

         data.strip_byte_counts.resize( t_count );

         if( t_type == field_type::long_ ) {

            if( t_count == 1 ) {
               uint32_t value = offset_or_value;
               data.strip_byte_counts[0] = value;
               return true;
            }

            uint32_t offset = offset_or_value;

            for( uint32_t i = 0; i < t_count; i++ ) {
               data.strip_byte_counts[i] = uint32_at( offset );
               offset += Size::LONG;
            }

            return true;
         }

         if( t_type == field_type::short_ ) {


            if( t_count == 1 ) {
               uint32_t value = short_array[0];
               data.strip_byte_counts[0] = value;
               return true;
            }

            if( t_count == 2 ) {
               data.strip_byte_counts[0] = short_array[0];
               data.strip_byte_counts[1] = short_array[1];
               return true;
            }

            uint32_t offset = short_array[0];

            for( uint32_t i = 0; i < t_count; i++ ) {
               data.strip_byte_counts[i] = uint16_at( offset );
               offset += Size::SHORT;
            }

            return true;
         }

         return false;
      }


      // -- Colormap  (0x140,320)
      if( t_tag == tags::colormap ) {
         if( t_type == field_type::short_ ) {
            data.offset_colormap = offset_or_value;
            uint8_t* p8 = ptr_at( data.offset_colormap );
            cmap& map = data.c_map;
            // use memcpy, works at BIG and LITTLE endian
            uint32_t sz = static_cast<uint32_t>( map.r.size() ) * Size::SHORT;
            memcpy( &map.r[0], p8,       sz );
            memcpy( &map.g[0], p8 + sz,   sz );
            memcpy( &map.b[0], p8 + 2 * sz, sz );

            if( _do_swap ) {
               uint16_array_swap( &map.r[0], sz );
               uint16_array_swap( &map.g[0], sz );
               uint16_array_swap( &map.b[0], sz );
            }


            for( tRGBA & ic : data.palette ) {
               tRGBA a;
               a.a() = 255;
               ptrdiff_t df = &ic - &data.palette[0];
               a.r() = static_cast<uint8_t>(map.r[ df ] >> 8 );
               a.g() = static_cast<uint8_t>(map.g[ df ] >> 8 );
               a.b() = static_cast<uint8_t>(map.b[ df ] >> 8 );
               data.palette[ df ] = a;
            }

            return true;
         }

         return false;
      }

      /* -- Planar Configuration   (0x11C,273)
      */
      if( t_tag == tags::planar_configuration ) {
         if( t_type == field_type::short_ ) {
            data.planar_configuration = short_array[0];
            return true;
         }

         return false;
      }

      /* -- orientation  (0x112,274)
      */
      if( t_tag == tags::orientation ) {
         if( t_type == field_type::short_ ) {
            // nix
            return true;
         }

         return false;
      }

      return true;

   }




   // return: offset of next tiff directory
   // return 0, if no more dirs exist
   uint32_t tTiff::DecodeTiff( uint32_t offset ) {

      uint16_t  entry_count = uint16_at( offset );

      if( _do_swap ) {
         entry_count = uint16_swap( entry_count );
      }

      data.rows_per_strip = 0;
      data.samples      = 1;

      // skip to start of directory entry list
      offset += Size::SHORT;

      uint8_t const* p = ptr_at( offset ) ;


      for( int i = 0; i < entry_count; i++ ) {
         cTiffEntry entry;

         // use memcpy, works at BIG and LITTLE endian
         memcpy( &entry,        p ,   Size::DIR_ENTRY );
         uint32_t tt = entry.ftag();

         if( _do_swap ) {
            entry.swap( buf );
         }

         tTiffTag t_tag    = entry.tiff_tag();

         if( !decode_entry( entry ) ) {
            char str[20];
            sprintf( str, "%0X", i );
            //LOGT_ERROR( ", Error in Tiff tag entry: " + tString( str ) );
            return uint32_t( -1 );
         }

         p += Size::DIR_ENTRY;

      }

      // last entry, offset of next IFD
      uint32_t* p32 = const_cast< uint32_t*> ( reinterpret_cast< uint32_t const* >(p) );
      uint32_t offset_to_next_IFD = *p32;
      assert( data.strip_byte_counts.size() == data.strip_offsets.size() );

      if( data.rows_per_strip <= 0  && data.strip_offsets.size() == 1 ) {
         // all rows in one stripe, tag
         data.rows_per_strip = data.get_size().y();
      }



      int ret = data_to_image();

      if( ret == -1 ) {
         return uint32_t( -1 );
      }

      return offset_to_next_IFD;
   }

}

/*
List of TIFF Tags


New Subfile Type     Tag  = 254 (FE)
                     Type = LONG
                     N    = 1

Subfile Type         Tag  = 255 (FF)
                     Type = SHORT
                     N    = 1

Image Width          Tag  = 256 (100)
                     Type = SHORT or LONG
                     N    = 1

Image Length         Tag  = 257 (101)
                     Type = SHORT or LONG
                     N    = 1

Bits per Sample      Tag  = 258 (102)
                     Type = SHORT
                     N    = Bits per Sample

Compression          Tag  = 259 (103)
                     Type = SHORT
                     N    = 1

Photometric
   Interpretation    Tag  = 262 (106)
                     Type = SHORT
                     N    = 1

Thresholding         Tag  = 263 (107)
                     Type = SHORT
                     N    = 1

Cell Width           Tag  = 264 (108)
                     Type = SHORT
                     N    = 1

Cell Length          Tag  = 265 (109)
                     Type = SHORT
                     N    = 1

Fill Order           Tag  = 266 (10A)
                     Type = SHORT
                     N    = 1

Document Name        Tag  = 269 (10D)
                     Type = ASCII
                     N    = strlen

Image Description    Tag  = 270 (10E)
                     Type = ASCII
                     N    = strlen

Make
                     Tag  = 271 (10F)
                     Type = ASCII
                     N    = strlen

Model                Tag  = 272 (110)
                     Type = ASCII
                     N    =

Strip Offsets        Tag  = 273 (111)
                     Type = SHORT or LONG
                     N    = StripsPerImage
                              for PlanarConfiguration == 1
                            SamplesPerPixel*StripsPerImage
                              for PlanarConfiguration == 2

Orientation          Tag  = 274 (112)
                     Type = SHORT
                     N    = 1

SamplesPerPixel      Tag  = 277 (115)
                     Type = SHORT
                     N    = 1

RowsPerStrip         Tag  = 278 (116)
                     Type = SHORT
                     N    = 1

Rows Per Strip       Tag  = 278 (116)
                     Type = SHORT or LONG
                     N    = 1

Strip byte_t Counts    Tag  = 279 (117)
                     Type = LONG or SHORT
                     N    = 1


Min Sample Value     Tag  = 280 (118)
                     Type = SHORT
                     N    = Samples Per Pixel

Max Sample Value     Tag  = 281 (119)
                     Type = SHORT
                     N    = Samples Per Pixel

X Resolution         Tag  = 282 (11A)
                     Type = RATIONAL
                     N    = 1

Y Resolution         Tag  = 283 (11B)
                     Type = RATIONAL
                     N    = 1

Planar Configuration Tag  = 284 (11C)
                     Type = SHORT
                     N    = 1

Page Name            Tag  = 285 (11D)
                     Type = ASCII
                     N    =

X Position           Tag  = 286 (11E)
                     Type = RATIONAL
                     N    =

Y Position           Tag  = 287 (11F)
                     Type = RATIONAL
                     N    =

Free Offsets         Tag  = 288 (120)
                     Type = LONG
                     N    =

Free byte_t Counts     Tag  = 289 (121)
                     Type = LONG
                     N    =

Gray Response Unit   Tag  = 290 (122)
                     Type = SHORT
                     N    = 1

Gray Response Curve  Tag  = 291 (123)
                     Type = SHORT
                     N    = 2**Bits Per sample

Group 3 Options      Tag  = 292 (124)
                     Type = LONG
                     N    = 1

Group 4 Options      Tag  = 293 (125)
                     Type = LONG
                     N    = 1

Resolution Unit      Tag  = 296 (128)
                     Type = SHORT
                     N    = 1

Page Number
   Tag  = 297 (129)
   Type = SHORT
   N    = 1

Color Response Curves
   Tag  = 301 (12D)
   Type = SHORT
   N    = 3*(2**Bits Per Sample)

Software
   Tag  = 305 (131)
   Type = ASCII
   N    =

Date Time
   Tag  = 306 (132)
   Type = ASCII
   N    = 20

Artist
   Tag  = 315 (13B)
   Type = ASCII
   N    =

Host Computer
   Tag  = 316 (13C)
   Type = ASCII
   N    =

Predictor
   Tag  = 317 (13D)
   Type = SHORT
   N    = 1

White Point
   Tag  = 318 (13E)
   Type = RATIONAL
   N    = 2

Primary Chromaticities
   Tag  = 319 (13F)
   Type = RATIONAL
   N    = 6

Color Map
   Tag  = 320 (140)
   Type = SHORT
   N    = 3*(2**Bits Per Sample)


*/

//EOF





