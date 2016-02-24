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
/*! \file rimg_tiff_r.cpp
\brief impl for read and write tif images

Lib: librimgrw
*/

#include <limits>


#include <iostream>
#include <sstream>
#include <string>

#include "rBin.h"
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


   vector<uint8_t> to_v16_scale_to8( vector<uint16_t> const& v16, uint32_t shift ) {
      assert( shift < 16 );
      vector<uint8_t> v8( v16.size() );
      auto b0 = v8.begin();

      for( auto p : v16 ) {
         *( b0++ ) = static_cast<uint8_t>(p >> shift);
      }

      return std::move( v8 );
   }


//   namespace {
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
//      const string msg_image_format_not_supported  = "Image write failed";
//   }

   string type_to_string( uint16_t val ) {

      if( val == field_type::byte_t ) {
         return "byte";
      }

      if( val == field_type::ascii ) {
         return "ascii";
      }

      if( val == field_type::short_ ) {
         return "short";
      }

      if( val == field_type::long_ ) {
         return "long";
      }

      if( val == field_type::rational ) {
         return "rational";
      }

      if( val == field_type::sbyte ) {
         return "sbyte";
      }

      if( val == field_type::undefined ) {
         return "undefined";
      }

      if( val == field_type::sshort ) {
         return "sshort";
      }

      if( val == field_type::slong ) {
         return "slong";
      }

      if( val == field_type::srational ) {
         return "srational";
      }

      if( val == field_type::float_ ) {
         return "float_";
      }

      if( val == field_type::double_ ) {
         return "double_";
      }

      string s = "Unknown field type: " + stringify( val );
      throw cTiffException( s , lfm_tif );

   }


   tTiffTag tTiffTag::uint16_to_tiff_tag( uint16_t val ) {
      for( auto t : tags::vtags ) {
         if( t == val ) {
            return t;
         }
      }

      return tags::invalid;
   }


   //  -- byte_t-reverse WORD or uint32_t in a buffer, inplace
   void uint16_array_swap( uint16_t* b, size_t l ) {
      while( l-- ) {
         uint16_t v = *b;
         *b = uint16_swap( *b );
         v = *b;
         b++;
         v = 0;
      }
   }

   void uint32_array_swap( uint32_t* b, size_t l ) {
      while( l-- ) {
         uint32_t d = *b;
         d = uint32_swap( d );
         *b = d;
         b++;
      }
   }



   tTiffData::tTiffData():
      size(),
      rows_per_strip( 0 ),
      strip_offsets(),
      strip_byte_counts(),
      photometric_interpr( phi::GRAY_white_is_min ),
      description(),
      document_name(),
      compression( 1 ),
      offset_colormap( 0 ),
      planar_configuration( 0 ),
      bits_gray( 8 ),
      bits_red( 0 ),
      bits_green( 0 ),
      bits_blue( 0 ),
      bits_alpha( 0 ),
      samples( 1 ),
      c_map(),
      palette( 0x100 ) {
   }

   void  tTiffData::sx( uint32_t i ) {
      size.setx( i );
   }
   void  tTiffData::sy( uint32_t i ) {
      size.sety( i );
   }
   void  tTiffData::set_size( uint32_xy s ) {
      size = s;
   }
   uint32_xy  tTiffData::get_size()const {
      return size;
   }

   uint32_t tTiffData::pixels()const {
      return size.size();
   }


   string tTiffData::log( ) {
      string s;
      string str;
      str = "width,length:        " + stringify( size.x() ) + ", " + stringify( size.y() );
      s += str + "\n";

      if( samples == 3 ) {
         str = "bits per sample:     " + stringify( bits_red ) + " " + stringify( bits_green ) + " " + stringify( bits_blue );
      } else {
         str = "bits per sample:  " + stringify( bits_gray );
      }

      s += str + "\n";
      str = "rows per strip:      " + stringify( rows_per_strip );
      s += str + "\n";
      str = "compression:         " + stringify( compression );
      s += str + "\n";
      str = "photometric_interpr: " + stringify( photometric_interpr );
      s += str + "\n";
      str = "strips per image     " + stringify( strip_offsets.size() );
      s += str + "\n";
      str = "samples per pixel    " + stringify( samples );
      s += str + "\n";
      //str = "offset stripes       " + stringify( strip_offsets[0] );
      //s += str + "\n";
      str = "offset colormap      " + stringify( offset_colormap );
      s += str + "\n";
      str = "planar configuration " + stringify( planar_configuration );
      s += str + "\n";
      LOGT_TIFF_INFO( s );
      return s;
   }




   void cTiffEntry::swap( vector<uint8_t> const& b ) {


      if( ifd_entry.tag == 513 ) {
         LOGT_TIFF_DEBUG( "" );
      }

      ifd_entry.tag = uint16_swap( ifd_entry.tag );
      ifd_entry.field_type = uint16_swap( ifd_entry.field_type );
      ifd_entry.count = uint32_swap( ifd_entry.count );



      // byte_t, ascii, no swap
      if( ifd_entry.field_type == field_type::byte_t ) {
         return;
      }

      if( ifd_entry.field_type == field_type::ascii ) {
         return;
      }

      ifd_entry.offset_or_value = uint32_swap( ifd_entry.offset_or_value );

      if( ifd_entry.field_type == field_type::short_ ) {

         uint16_t* entryPtr = reinterpret_cast<uint16_t*> (  &( ifd_entry.offset_or_value )  );

         if( ifd_entry.count == 1 ) {
            *( entryPtr ) = uint16_swap( *( entryPtr ) );
            return;
         }

         if( ifd_entry.count == 2 ) {
            uint16_array_swap( reinterpret_cast<uint16_t*> ( &( ifd_entry.offset_or_value ) ), 2 );
            return;
         }

         uint16_t *p16 = const_cast<uint16_t*>(  reinterpret_cast<uint16_t const*>   ( &b[ifd_entry.offset_or_value] ) );
         uint16_array_swap( const_cast<uint16_t*>(   reinterpret_cast<uint16_t const*> (  ( &b[ifd_entry.offset_or_value] ) ) ), ifd_entry.count ) ;

         return;
      }

      if( ifd_entry.field_type == field_type::long_ ) {

         if( ifd_entry.count == 1 ) {
            uint32_swap( ifd_entry.offset_or_value );
            return;
         }

         uint32_array_swap( const_cast<uint32_t*> (  reinterpret_cast<uint32_t const*>  ( ( &b[ifd_entry.offset_or_value] ) ) ), ifd_entry.count );

         return;
      }


      if( ifd_entry.field_type == field_type::rational ) {

         uint32_array_swap( const_cast< uint32_t* > (  reinterpret_cast<uint32_t const*>  ( &b[ifd_entry.offset_or_value] ) ), 2  );
      }
   }


   void tTiff::to_gray_image( tImgLinear& im ) {

      if( data.bits_gray == 16 ) {
         vector<uint16_t> v16;

         if( data.strip_offsets.size() == 1 ) {
            auto offset = data.strip_offsets[0];
            auto bytes = data.strip_byte_counts[0];
            uint8_t* p8 = ptr_at( offset );
            uint16_t* p16 = reinterpret_cast<uint16_t*>( p8 );
            bytes >>= 1;

            if( _do_swap ) {
               uint16_array_swap( p16, bytes );
            }

            v16.assign( p16, p16 + bytes );
         } else {
            for( size_t i = 0; i < data.strip_offsets.size(); i++ ) {
               uint32_t offset = data.strip_offsets[i];
               uint32_t bytes = data.strip_byte_counts[i];

               uint8_t* p8 = ptr_at( offset );
               uint16_t* p16 = reinterpret_cast<uint16_t*>( p8 );
               bytes >>= 1;

               if( _do_swap ) {
                  uint16_array_swap( p16, bytes );
               }

               v16.insert( v16.end(), p16, p16 + bytes );
            }
         }

         uint32_t shift = 4;  // for 12 bit images
         vector<uint8_t> v8 = to_v16_scale_to8( v16, shift );
         im.insert_data( &v8[0], static_cast<uint32_t>(v8.size()), 0 );

         return;
      }

      if( data.strip_offsets.size() == 1 ) {
         auto offset = data.strip_offsets[0];
         auto bytes = data.strip_byte_counts[0];

         if( data.photometric_interpr == phi::GRAY_white_is_min ) { // 0
            uint8_t* p = const_cast<uint8_t*>( ptr_at( offset ) );
            inverse( p, bytes );
         }

         im.insert_data( ptr_at( offset ), bytes, 0 );
         return;
      }

      uint32_t im_offset = 0;


      for( size_t i = 0; i < data.strip_offsets.size(); i++ ) {
         uint32_t offset = data.strip_offsets[i];
         auto bytes = data.strip_byte_counts[i];

         if( data.photometric_interpr == phi::GRAY_white_is_min ) { // 0
            uint8_t* p = const_cast<uint8_t*>( ptr_at( offset ) );
            inverse( p, bytes );
         }

         uint8_t const* p = ptr_at( offset );
         im.insert_data( p, bytes, im_offset );
         im_offset += bytes;
      }

   }




   void tTiff::check_II_MM()  {

      int lLogo = static_cast<int>(II_IntelLogo.size());
      uint8_t const* p0 = ptr_at( 0 );
      uint8_t const* p1 = ptr_at( lLogo );

      string magic( ptr_at( 0 ), ptr_at( lLogo ) );

      if( magic != II_IntelLogo && magic != MM_MacLogo ) {
         string e;
         e = "Tifflogo is not 'II' or 'MM'";
         throw cTiffException( e, lfm_tif );
      }

      /*
            *  big endian
            *  MIPS-, SPARC-, PowerPC-, Motorola 6800/68k-, Atmel AVR32[2]- und TMS9900-Prozessoren
            *
            * little endian
            * PC-Systeme (x86-kompatible)
            * Alpha, Altera Nios, Atmel AVR, manche SH3/SH4-Systeme oder VAX.
            */

      bool big_endian = IsBigEndian();

      if( magic == II_IntelLogo ) {
         _do_swap = big_endian;
         /* -- II and little (intel)      = no swap */
         /* -- II and big (mac)    endian = swap    */
      }

      if( magic == MM_MacLogo ) {
         _do_swap = !big_endian;
         /* -- MM and little endian = swap  */
         /* -- MM and big endian = no swap  */
         /* -- MM and Intel CPU  = swap  */
         /* -- MM and MAC CPU    = no swap  */
      }


   }




   void tTiff::read() {



      try {
         check_II_MM();
      } catch( cTiffException const& ex ) {
         throw ex;
      }

      /* -- offset of first dir, 4 bytes after file start
             */
      int count = 0;

      try {
         uint32_t offset1 = uint32_at( 4 );

         if( _do_swap ) {
            offset1 = uint32_swap( offset1 );
         }

         // offset == 0, end of file
         while( offset1 ) {
            offset1 = DecodeTiff( offset1 );

            // error
            if( offset1 == uint32_t( -1 ) ) {
               break;
            }

            count++;
         }
      } catch( cTiffException const& ex ) {
         throw ex;
      }

      if( count > 1 ) {  // multiple tiff files
         string s = " File contains multiple tiff files. ";
         throw cTiffException( s, lfm_tif );
      }
   }


   // ret == -1 error
   // ret ==  0 ok
   int tTiff::data_to_image() {

      // size and description are used in all image formats
      img.size() = data.get_size();
      img.description( data.description );

      //
      // -- 8, 12, 16  bit gray images
      //
      if( data.photometric_interpr == phi::GRAY_white_is_min      /* 0 */
            || data.photometric_interpr == phi::GRAY_black_is_min /* 1 */ ) {
         if( data.bits_gray == 8 && data.samples == 1 ) {
            img.set_mono8();
            img.alloc_data_buffer();
            to_gray_image( img );
         }

         if( data.bits_gray == 16 && data.samples == 1 ) {
            img.set_mono8();
            img.alloc_data_buffer();
            to_gray_image( img );
         }

      }

      //
      // color map tiff
      //
      if( data.bits_gray == 8
            && data.samples == 1
            && data.photometric_interpr == phi::PALETTE   /* 3 */ ) {
         img.set_palette();
         img.alloc_data_buffer();
         to_gray_image( img );
         img.palette( data.palette );
      }

      // check for impossible combination of parameters
      if( data.photometric_interpr == phi::GRAY_white_is_min /* 0 */
            ||
            data.photometric_interpr == phi::GRAY_black_is_min /* 1 */ ) {
         if( (
                  data.bits_gray != 8
                  &&
                  data.bits_gray != 16
                  &&
                  data.bits_gray != 12 )

               || data.samples != 1 ) {
            //LOGT_ERROR( "photometric_interpr == 1 || photometric_interpr == 3" );
            //LOGT_ERROR( " is valid only for 8 bit files. " );
            //LOGT_ERROR( "(bits_per_sample == 8  && samples_per_pixel == 1 ) != TRUE" );
            //LOGT_ERROR( "unknown tiff file type 'tiff_read'" );
            return -1;
         }
      }

      //
      // -- 24 bit images, RGB
      //
      if( data.photometric_interpr ==  phi::RGB /* 2 */
            && data.bits_red == 8 ) { // 8 bit RGB
         img.set_rgb();
         img.alloc_data_buffer();
         img.description( data.description );

         switch( data.planar_configuration ) {
         case planar_cfg::RGBRGBRGB: {                                 // RGBRGBRGB...
            size_t offset_img = 0;
            uint32_t count = 0;

            for( uint32_t offset : data.strip_offsets ) {
               uint8_t const* p = ptr_at( offset );
               uint32_t bytes = data.strip_byte_counts[count++];
               img.insert_data( p, bytes, static_cast<uint32_t>(offset_img) );
               offset_img += bytes;
            }
         }
         break;

         case planar_cfg::RRRGGGBBB://RRRR GGGGG BBBB

            if( data.strip_offsets.size() == 3 ) {
               uint32_t const* p = &data.strip_byte_counts[0];
               assert( p[0] == p[1] );
               assert( p[1] == p[2] );

               uint32_t ib = img.bytes();
               vector<uint8_t> v( img.bytes() );

               uint8_t* vptr = &v[0];
               tRGB* target = reinterpret_cast< tRGB* >( vptr ) ;

               uint8_t const* red = ptr_at( data.strip_offsets[0] );
               uint8_t const* red_end = red + data.strip_byte_counts[0];
               uint8_t const* green = ptr_at( data.strip_offsets[1] );
               uint8_t const* blue = ptr_at( data.strip_offsets[2] );

               while( red < red_end ) {
                  *target++ = tRGB( *red++, *green++, *blue++ );
               }

               img.insert_data( v, 0 );
               break;
            } else {
               string e;
               e =  "unknown tiff file type 'TiffRead', ";
               //e += file;
               e += "24 bit image, strips per image != 3";
               throw cTiffException( e, lfm_tif );
            }

         default:
            string e = "planar configuration is wrong: '";
            e += stringify( data.planar_configuration );
            e += "' in 24 bit image: ";
            //e +=        im.fullname() ;
            throw cTiffException( e, lfm_tif );
         }
      } // end rgb 8 bit

      if(
         data.photometric_interpr ==  phi::RGB  /* 2 */
         &&
         data.bits_red == 16 ) { // 16 bit RGB
         string e = "planar configuration RGB and 16 bit is not supported: '";
         e += "' in 24 bit RGB image: ";
         //throw cTiffException( e );

         img.set_rgb();
         img.alloc_data_buffer();
         img.description( data.description );

         switch( data.planar_configuration ) {
         case 1:  {                                // RGBRGBRGB...


            // N = StripsPerImage for PlanarConfiguration equal to 1; N = SamplesPerPixel * StripsPerImage for PlanarConfiguration equal to 2
            vector<uint16_t> v16;

            if( data.strip_offsets.size() == 1 ) {
               uint32_t offset = data.strip_offsets[0];
               uint8_t* b8 = ptr_at( offset );
               uint16_t* b16 = reinterpret_cast< uint16_t*>( b8 );
               uint32_t bytes = data.strip_byte_counts[0];

               bytes >>= 1;

               if( _do_swap ) {
                  uint16_array_swap( b16, bytes );
               }

               v16.assign( b16, b16 + bytes );
            } else {

               uint32_t bytes = 0;
               uint32_t count = 0;

               for( size_t i = 0; i <  data.strip_offsets.size() ; i++ ) {
                  uint32_t offset = data.strip_offsets[i];
                  uint8_t*  p8 = ptr_at( offset );
                  uint16_t* p16 = reinterpret_cast< uint16_t* >( p8 );
                  bytes = data.strip_byte_counts[i];

                  bytes >>= 1;
                  count += bytes;

                  if( _do_swap ) {
                     uint16_array_swap( p16, bytes );
                  }

                  v16.insert( v16.end(), p16, p16 + bytes );
               }

            }

            uint32_t shift = 8;
            vector<uint8_t> v8 = to_v16_scale_to8( v16, shift ) ;
            img.insert_data( v8, 0 );

         }
         break;

         case 2://RRRR GGGGG BBBB

            if( data.strip_offsets.size() == 3 ) {
               uint32_t roffset;
               uint32_t goffset;
               uint32_t boffset;

               uint32_t stripe_offset = data.strip_offsets[0];
               uint8_t const* p = ptr_at( stripe_offset );
               bool islong = true;

               if( vector<uint32_t>::difference_type(
                        &data.strip_offsets[0] - &data.strip_offsets[1] ) == 2 ) {
                  islong = false;
               }

               if( islong ) {
                  memcpy( &roffset, p + 0, 4 );
                  memcpy( &goffset, p + 4, 4 );
                  memcpy( &boffset, p + 8, 4 );
               } else {
                  uint16_t r, g, b;
                  memcpy( &r, p + 0, 2 );
                  roffset = static_cast< unsigned long >(r);
                  memcpy( &g, p + 2 , 2 );
                  goffset = static_cast< unsigned long >(g);
                  memcpy( &b, p + 4, 2 );
                  boffset = static_cast< unsigned long >(b);
               }

               vector<uint8_t> v( img.bytes() );
               size_t pixels = data.pixels();
               size_t index = 0;
               uint8_t* vptr = &v[0];
               tRGB* target = reinterpret_cast< tRGB* >( vptr );
               uint8_t const* red = ptr_at( roffset );
               uint8_t const* green = ptr_at( goffset );
               uint8_t const* blue = ptr_at( boffset );

               while( index < pixels ) {
                  tRGB rgb;
                  rgb.r() = *red;
                  rgb.g() = *green;
                  rgb.b() = *blue;
                  *target = rgb;
                  target++;
                  red++;
                  green++;
                  blue++;
               }

               img.insert_data( v, 0 );
               break;
            } else {
               string e1;
               e1 =  "unknown tiff file type 'TiffRead', ";
               //e += file;
               e1 += "24 bit image, strips per image != 3";
               throw cTiffException( e1, lfm_tif );
            }

         default:
            string e2 = "planar configuration is wrong: '";
            e2 += stringify( data.planar_configuration );
            e2 += "' in 24 bit image: ";
            //e +=        im.fullname() ;
            throw cTiffException( e2, lfm_tif );
         }
      }

      return 0;
   }


   string tTiff::description()const {
      return data.description;
   }

   void tTiff::description( string  const& n ) {
      data.description = n;
   }

   tTiff ::tTiff( vector<uint8_t> const& buf_, tImgLinear& im_ ):
      img( im_ ),
      data(),
      buf( buf_ ),
      _do_swap( false ),
      tagloglines(),
      errors()
   {}

   tTiff::~tTiff() {
      // delete &t;
   }





   void read_local( string fn, tImgLinear& img ) {
      uint32_t se = sizeof( cTiffEntry );

      img.reset();
      uintmax_t size = boost::filesystem::file_size( fn );

      std::vector<uint8_t> buf( size );

      bin_read::t_bin_read reader;
      reader( fn, buf, size );

      tTiff tr( buf, img );
      tr.read();
   }


   void tTiff::photometric_interpretation( int i ) {
      data.photometric_interpr = i;
   }
   void tTiff::x( int x_ ) {
      data.sx( x_ );
   }
   void tTiff::y( int y_ ) {
      data.sy( y_ );
   }
   void tTiff::size( uint32_xy s ) {
      data.set_size( s );
   }
   void tTiff::planar_configuration( int p ) {
      data.planar_configuration = p;
   }
   void tTiff::bits_gray( int i ) {
      data.bits_gray = i;
   }
   void tTiff::bits_red( int i ) {
      data.bits_red = i;
   }
   void tTiff::bits_green( int i ) {
      data.bits_green = i;
   }

   void tTiff::bits_blue( int i ) {
      data.bits_blue = i;
   }

   void tTiff::samples( int i ) {
      data.samples = i;
   }


   void t_read_tiff::_read( tImgLinear& img )  {

      try {
         read_local( _fn, img );
      } catch( cTiffException& ex ) {
         throw tImgReadEx( ex.what() );
      }

   }

   // read  tImgLinear
   void t_read_tiff::read( tImgLinear& img )  {
      _read( img );
   }

   // read  planar, simple forward to tImgLinear
   void t_read_tiff::read( tImgPlanar& planar )  {

      tImgLinear rimg;
      read( rimg );
      tImgViewLinear vlin( rimg );
      convert( vlin, planar );
   }



}


//EOF





