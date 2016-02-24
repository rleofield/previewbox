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
/*! \file rimg_tiff.h
\brief impl for read and write tif images

Lib: librimgrw
*/

#ifndef RIMG_TIFF_H
#define RIMG_TIFF_H

#include <limits>


#include <iostream>
#include <sstream>
#include <string>
//#include <netinet/in.h>



#include "rimg_read_write_bin.h"


#include "rimg_linear.h"

#include "rimg_convert.h"


#include "rimg_read_write.h"
#include "rimg_fs.h"
#include "rimg_rw_exceptions.h"
#include "rimg_t_read.h"
#include "rimg_t_write.h"
#include "stringhelper.h"



using namespace std;
using namespace rlf::rimg_convert;

using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;
using rlf::t_write_tiff;
using rlf::t_read_tiff;
using rlf_hstring::stringify;
//#include <limits>

namespace rlf {

   inline string stringify( size_t const& val ) {
      return rlf_hstring::stringify( val );
   }


   namespace lfm {

      class tLfm_tif {
         uint32_t _line;
         string _file;
         string _method;

      public:
         tLfm_tif(): _line( 0 ), _file(), _method() {}

         tLfm_tif( uint32_t line_, std::string const& file_, std::string const& method_ )
            : _line( line_ ), _file( file_ ), _method( method_ ) {
         }
         size_t line()const {
            return _line;
         }
         std::string file()const {
            return _file;
         }

         std::string method()const {
            return _method;
         }
         operator string()const {
            return "l: " + stringify( line() ) + ", f: " + file() + ". m: " + method();
         }
         ~tLfm_tif() {}
      };

      // use of parameterobject for line, file, method
      inline tLfm_tif Lfm_tif( uint32_t line_, string const& file_, string const& method_ ) {
         return tLfm_tif( line_, file_, method_ );
      }


   }

#define lfm_tif lfm::Lfm_tif( __LINE__,__FILE__,__FUNCTION__)




   vector<uint8_t> to_v8( vector<uint16_t> const& v16 ) ;




   const string msg_image_format_not_supported  = "Image write failed";


   class cTiffException: public rImgEx {
   protected:
      string _what;
   public:
      cTiffException( const string& msg, lfm::tLfm_tif const& l ):
         rImgEx( msg + " - " + static_cast<string>( l ) )
         , _what( msg + " - " + static_cast<string>( l ) )

      { }
      ~cTiffException()throw() {}
      virtual const char* what() const throw() {
         return _what.c_str();
      }
   };

   void read( vector<uint8_t>  const& buf, rlf::rimg_linear::tImgLinear& img );
   void write( string const& file, tImgViewLinear const& img ) ;

   const string II_IntelLogo = "II";
   const string MM_MacLogo = "MM";

   struct tTiffTag {

      tTiffTag( uint32_t val, string info_ ): info( info_ ), value( val ) {}
      bool operator==( tTiffTag tt )const {
         if( value == tt.value ) {
            return true;
         }

         return false;
      }
      bool operator==( uint16_t val ) const {
         if( value == val ) {
            return true;
         }

         return false;
      }
      operator uint16_t()const {
         return static_cast<uint16_t>( value );
      }
      string info;
      int32_t value;
      string to_string() {
         return info;
      }
      static tTiffTag uint16_to_tiff_tag( uint16_t val );

   };

   namespace tags {
      const tTiffTag new_subfile_type = { 0xFE,  "new_subfile_type (0xFE, 254)" };
      const tTiffTag subfile_type( 0xFF, "subfile_type (0xFF,255)" );
      const tTiffTag image_width( 0x100, "image_width (0x100,256)" );
      const tTiffTag image_length( 0x101 , "image_length (0x101,257)" );
      const tTiffTag bits_per_sample( 0x102, "bits_per_sample (0x102,258)" );
      const tTiffTag compression( 0x103, "compression (0x103,259)" );
      const tTiffTag photometric_interpretation( 0x106, "photometric_interpretation (0x106,262)" );
      const tTiffTag description( 0x10E , "description (0x10E,270)" );
      const tTiffTag document_name( 0x10D , "document name (0x10D,269)" );
      const tTiffTag strip_offsets( 0x111, "strip_offsets (0x111,273)" );
      const tTiffTag samples_per_pixel( 0x115, "samples_per_pixel (0x115,277)" );
      const tTiffTag rows_per_strip( 0x116, "rows_per_strip (0x116,278)" );
      const tTiffTag strip_byte_counts( 0x117, "stripByte_counts (0x117,279)" );
      const tTiffTag colormap( 0x140, "colormap (0x140,320)" );
      const tTiffTag planar_configuration( 0x11C, "planar_configuration (0x11C,273)" );
      const tTiffTag orientation( 0x112, "orentation (0x112,274)" );


      const tTiffTag invalid( 0, "unknown tag" );



      const vector<tTiffTag> vtags = {
         new_subfile_type,
         subfile_type,
         image_width,
         image_length,
         bits_per_sample,
         compression,
         photometric_interpretation,
         description,
         document_name,
         strip_offsets,
         samples_per_pixel,
         rows_per_strip,
         strip_byte_counts,
         colormap,
         planar_configuration
      };
   }

   struct _IFD {
      uint16_t tag;
      uint16_t field_type;
      uint32_t count;
      uint32_t offset_or_value;
      _IFD():
         tag( 0 ),
         field_type( 0 ),
         count( 0 ),
         offset_or_value( 0 ) {}

   };



   struct Offset {
      static const uint32_t TAG    = 0;
      static const uint32_t TYPE   = 2;
      static const uint32_t LENGTH = 4;
      static const uint32_t OFFSET = 8;
   };

   struct Size {
      static const uint32_t LONG  =  sizeof( uint32_t );
      static const uint32_t SHORT =  sizeof( uint16_t );
      static const uint32_t BYTE  =  sizeof( uint8_t );
      static const uint32_t DIR_ENTRY = 12;

      static const uint32_t MAX_LONG = uint32_t( -1 ); // std::numeric_limits<uint32_t>::max();
      //0xffffffff; //uint32_t(-1);
      static const uint32_t MAX_SHORT =  uint16_t( -1 ); // std::numeric_limits<uint16_t>::max ();//0xffff;     //uint16_t(-1);
      static const uint32_t MAX_BYTE  =  uint8_t( -1 ); //std::numeric_limits<uint8_t>::max ();//0xff;       //uint8_t(-1);

   };

   /*  --  Field type
    1 - byte_t
    2 - ASCII string, counted in length.
        Most often an ASCIIZ string, the
        trailing zero is counted with the
        data length.
    3 - uint16_t
    4 - uint32_t / uword
    5 - rational (2 dwords, numerator and denominator)
   */
   struct field_type {
      static const uint32_t byte_t = 1;
      static const uint32_t ascii  = 2;
      static const uint32_t short_ = 3;
      static const uint32_t long_  = 4;
      static const uint32_t rational = 5 ;
      static const uint32_t sbyte = 6 ;
      static const uint32_t undefined = 7 ;
      static const uint32_t sshort = 8 ;
      static const uint32_t slong = 9 ;
      static const uint32_t srational = 10 ;
      static const uint32_t float_ = 11 ;
      static const uint32_t double_ = 12 ;

   };
   const vector<uint32_t> types = {
      field_type::byte_t,
      field_type::ascii,
      field_type::short_,
      field_type::long_,
      field_type::rational,
      field_type::sbyte,
      field_type::undefined,
      field_type::sshort,
      field_type::slong,
      field_type::srational,
      field_type::float_,
      field_type::double_
   };

   string type_to_string( uint16_t val ) ;



   struct PlanarConfiguration {
      // 284 (11C)
      // SHORT
      static const uint32_t none = 0;
      static const uint32_t RGBRGBRGB  = 1;
      static const uint32_t RRRGGGBBB  = 2;

   };
   typedef PlanarConfiguration planar_cfg;


   struct PhotometricInterpretation {
      static const size_t GRAY_white_is_min = 0;
      static const size_t GRAY_black_is_min = 1;
      static const size_t PALETTE           = 3;
      static const size_t RGB               = 2;
   };
   typedef PhotometricInterpretation phi;


   /* -- byte_t reverse WORD and uint32_t
   */
   inline uint16_t uint16_swap( uint16_t x ) {
      int temp = ( ( ( ( x ) >> 8 ) & 0xff ) | ( ( ( x ) & 0xff ) << 8 ) ) ;
      return static_cast<uint16_t>(temp);

   }
   inline  uint32_t uint32_swap( uint32_t x ) {
      return ( ( ( ( x ) & 0xff000000 ) >> 24 ) | ( ( ( x ) & 0x00ff0000 ) >>  8 ) |            \
               ( ( ( x ) & 0x0000ff00 ) <<  8 ) | ( ( ( x ) & 0x000000ff ) << 24 ) ) ;

   }

   //  -- byte_t-reverse WORD or uint32_t in a buffer, inplace
   void uint16_array_swap( uint16_t* b, size_t l ) ;
   void uint32_array_swap( uint32_t* b, size_t l ) ;


   inline uint32_t tiff_align32( uint32_t s ) {
      if( s & 0x0003 ) {  // 0x11,  1,2, oder 3
         s |= 0x0003; // set to 0x11, (3)
         ++s;         // add 1 to align
      }

      return s;
   }

   template<typename T>
   inline void inverse( T* pimg, size_t count ) {
      while( count-- ) {
         auto temp = ( T( -1 ) - *pimg );
         *pimg = static_cast<T> ( temp );
         pimg++;
      }
   }

   inline bool IsBigEndian() {
      int x = 1;


      if( *( reinterpret_cast<char*>(&x) ) == 1 ) {
         return false;
      }

      return true;
   }


   const uint32_t size_of_color_map = 0x100 * 3;

   class nc {
      nc( const nc& );
      const nc&  operator= ( const nc& );
   protected:
      nc( ) { }
      virtual ~nc( ) { }
   };


   class cmap {

   public:
      cmap(): r( 0x100 ), g( 0x100 ), b( 0x100 ) {}
      vector<uint16_t> r;
      vector<uint16_t> g;
      vector<uint16_t> b;
   };


   class cTiffEntry;

   class tTiffData: private nc {
      //            void operator=( tTiffData const& );
      //            tTiffData( tTiffData const& );
   public:

      tTiffData();

      void  sx( uint32_t i );
      void  sy( uint32_t i );
      void  set_size( uint32_xy s );
      uint32_xy  get_size()const;

      uint32_t pixels()const ;
      uint32_t offset_value_at( const uint8_t* poff, cTiffEntry const& e )const;

      string log();


   private:
      uint32_xy size;
      uint32_t rows_per_strip;
      vector<uint32_t> strip_offsets;
      vector<uint32_t> strip_byte_counts;
      uint32_t photometric_interpr;
      string description;
      string document_name;
      uint32_t compression;
      uint32_t offset_colormap;
      uint32_t planar_configuration;
      uint32_t bits_gray;
      uint32_t bits_red;
      uint32_t bits_green;
      uint32_t bits_blue;
      uint32_t bits_alpha;
      uint32_t samples;
      cmap     c_map;
      vector<tRGBA> palette;


      friend class tTiff;
   };


   class cTiffEntryBase {
      void operator=( cTiffEntryBase const& );
      cTiffEntryBase( cTiffEntryBase const& );
   public:

      cTiffEntryBase():
         ifd_entry() {}
      _IFD ifd_entry;
      // public: see memcpy in DecodeEntry, works with BIG and LITTLE endian
   };

   class cTiffEntry: public cTiffEntryBase {
      void operator=( cTiffEntry const& );
      cTiffEntry( cTiffEntry const& );
   public:
      cTiffEntry():
         cTiffEntryBase() {
      }

      tTiffTag tiff_tag()const {
         return tTiffTag::uint16_to_tiff_tag( ifd_entry.tag );
      }
      uint16_t ftag()const {
         return ifd_entry.tag;
      }
      void swap( std::vector<uint8_t> const& b );
      uint32_t size()const {
         return sizeof( cTiffEntry );
      }
   };

   class cTiffEntryWrite: public cTiffEntry {
   public:

      cTiffEntryWrite(): cTiffEntry() {}
      void ToBuffer( uint8_t* buf )const;
   };


   // impl in rimg_tiff_r.cpp
   class tTiff {

      // read
      tImgLinear& img;

      // prevent copy
      tTiff( tTiff const& );
      tTiff& operator=( tTiff const& );


      bool  decode_entry( cTiffEntry& e );
      uint32_t  DecodeTiff( uint32_t offset );
      int  data_to_image();
      tTiffData   data;

      // external tiff buffer
      std::vector<uint8_t> const& buf;


      bool  _do_swap;
      void check_II_MM();

      vector<string> tagloglines;
      vector<string> errors;

      void to_gray_image( tImgLinear& im ) ;


      void y( int y );
      void x( int x );


      uint8_t const* ptr_at( size_t offset )const {
         if( offset >= buf.size() ) {
            string e = "offset in buf is out of range";
            throw cTiffException( e, lfm_tif );
         }

         return buf.data() + offset;
      }
      uint8_t* ptr_at( size_t offset ) {
         if( offset >= buf.size() ) {
            string e = "offset in buf is out of range";
            throw cTiffException( e, lfm_tif );
         }

         uint8_t const* p =  buf.data() + offset;
         return const_cast<uint8_t*>( p );
      }
      uint8_t const* const_ptr_at( size_t offset ) {
         if( offset >= buf.size() ) {
            string e = "offset in buf is out of range";
            throw cTiffException( e, lfm_tif );
         }

         uint8_t const* p =  buf.data() + offset;
         return p;
      }
      int8_t const* signed_const_ptr_at( size_t offset ) {
         if( offset >= buf.size() ) {
            string e = "offset in buf is out of range";
            throw cTiffException( e, lfm_tif );
         }

         uint8_t const* p =  buf.data() + offset;
         return reinterpret_cast<int8_t const*>( p);
      }

      uint16_t uint16_at( size_t off ) {
         if( off >= buf.size() ) {
            return 0;
         }

         // portable copy
         uint16_t var;
         memcpy( &var, ptr_at( off ), Size::SHORT );
         return var;
      }

      uint32_t uint32_at( size_t off )const {
         if( off >= buf.size() ) {
            return 0;
         }

         // portable copy
         uint32_t var = 0;
         memcpy( &var, ptr_at( off ), Size::LONG );
         return var;
      }


   public:

      tTiff( vector<uint8_t> const& buf, tImgLinear& im );
      ~tTiff();

      void description( string  const& n );
      string description()const;
      void bits_gray( int i );
      void bits_red( int i );
      void bits_green( int i );
      void bits_blue( int i );

      void size( uint32_xy s );

      void photometric_interpretation( int i );
      void samples( int i );
      void planar_configuration( int p );

      void write( string const& file, tImgViewLinear const& im );
      void read();


   };

   const uint32_t number_of_colors = 256;

   void WriteToFile( string const& file, vector<uint8_t> const& b );

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

#endif

//EOF





