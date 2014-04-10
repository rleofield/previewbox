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


#include <iostream>
#include <sstream>
#include <string>




#include "rimg_read_write_bin.h"


#include "rimg_linear.h"
#include "rimg_planar.h"
//#include "rimg_read_write.h"
#include "rimg_convert.h"


#include "rimg_read_write.h"
#include "rimg_fs.h"
#include "rimg_rw_exceptions.h"

using namespace std;
using namespace rlf::rimg_convert;

using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;
using rlf::t_write_tiff;
using rlf::t_read_tiff;

namespace rlf {


   vector<uint8_t> to_v8( vector<uint16_t> const& v16 ){
      vector<uint16_t>::const_iterator begin = v16.begin();
      vector<uint16_t>::const_iterator end = v16.end();
      vector<uint8_t> v8;
      size_t count = 0;
      while( begin != end ){
         uint16_t val16 = *begin++;
         uint8_t v = val16 >> 8 ;
         v8.push_back(v);
         count++;
         if( count % 1000 == 0 ){
            count = 0;
      }
      }
      return v8;
   }
   namespace tiff_local {

      namespace {
         const string marker = "%s";
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

         const string msg_write_file = "Couldn't write file: '%s'";
         string write_file( string const& s0 ) {
            return replace( msg_write_file, s0 );
         }

         string stringify( size_t const& val ) {
            return boost::lexical_cast<string>( val );
         }


      }

      const string msg_image_format_not_supported  = "Image write failed";


      class cTiffException: public exception {
      protected:
         string _what;
      public:
         cTiffException( const string& msg ): _what( msg )
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
            return value;
         }
         string info;
         int32_t value;
         string to_string() {
            return info;
         }
         static tTiffTag value_to_tiff_tag( uint16_t val );

      };

      struct tags {
         tags() {}
         const static tTiffTag new_subfile_type_0xFE;
         const static tTiffTag subfile_type_0xFF;
         const static tTiffTag image_width_0x100;
         const static tTiffTag image_length_0x101;
         const static tTiffTag bits_per_sample_0x102 ;
         const static tTiffTag compression_0x103 ;
         const static tTiffTag photometric_interpretation_0x106;
         const static tTiffTag description_0x10E;
         const static tTiffTag strip_offsets_0x111;
         const static tTiffTag samples_per_pixel_0x115;
         const static tTiffTag rows_per_strip_0x116 ;
         const static tTiffTag stripByte_counts_0x117;
         const static tTiffTag colormap_0x140;
         const static tTiffTag planar_configuration_0x11C;
         const static tTiffTag invalid;
      };
      const tTiffTag tags::new_subfile_type_0xFE( 0xFE,  "new_subfile_type (0xFE, 254)" );
      const tTiffTag tags::subfile_type_0xFF( 0xFF, "subfile_type (0xFF,255)" );
      const tTiffTag tags::image_width_0x100( 0x100, "image_width (0x100,256)" );
      const tTiffTag tags::image_length_0x101( 0x101 , "image_length (0x101,257)" );
      const tTiffTag tags::bits_per_sample_0x102( 0x102, "bits_per_sample (0x102,258)" );
      const tTiffTag tags::compression_0x103( 0x103, "compression (0x103,259)" );
      const tTiffTag tags::photometric_interpretation_0x106( 0x106, "photometric_interpretation (0x106,262)" );
      const tTiffTag tags::description_0x10E( 0x10E , "description (0x10E,270)" );
      const tTiffTag tags::strip_offsets_0x111( 0x111, "strip_offsets (0x111,273)" );
      const tTiffTag tags::samples_per_pixel_0x115( 0x115, "samples_per_pixel (0x115,277)" );
      const tTiffTag tags::rows_per_strip_0x116( 0x116, "rows_per_strip (0x116,278)" );
      const tTiffTag tags::stripByte_counts_0x117( 0x117, "stripByte_counts (0x117,279)" );
      const tTiffTag tags::planar_configuration_0x11C( 0x11C, "planar_configuration (0x11C,273)" );
      const tTiffTag tags::colormap_0x140( 0x140, "colormap (0x140,320)" );
      const tTiffTag tags::invalid( 0, "unknown Tag" );


      tTiffTag tTiffTag::value_to_tiff_tag( uint16_t val ) {

         if( tags::new_subfile_type_0xFE == val ) {
            return tags::new_subfile_type_0xFE;
         }

         if( tags::subfile_type_0xFF == val ) {
            return tags::subfile_type_0xFF;
         }

         if( tags::image_width_0x100 == val ) {
            return tags::image_width_0x100;
         }

         if( tags::image_length_0x101 == val ) {
            return tags::image_length_0x101;
         }

         if( tags::bits_per_sample_0x102 == val ) {
            return tags::bits_per_sample_0x102;
         }

         if( tags::compression_0x103 == val ) {
            return tags::compression_0x103;
         }

         if( tags::photometric_interpretation_0x106 == val ) {
            return tags::photometric_interpretation_0x106;
         }

         if( tags::description_0x10E == val ) {
            return tags::description_0x10E;
         }

         if( tags::strip_offsets_0x111 == val ) {
            return tags::strip_offsets_0x111;
         }

         if( tags::samples_per_pixel_0x115 == val ) {
            return tags::samples_per_pixel_0x115;
         }

         if( tags::rows_per_strip_0x116 == val ) {
            return tags::rows_per_strip_0x116;
         }

         if( tags::stripByte_counts_0x117 == val ) {
            return tags::stripByte_counts_0x117;
         }

         if( tags::colormap_0x140 == val ) {
            return tags::colormap_0x140;
         }

         if( tags::planar_configuration_0x11C == val ) {
            return tags::planar_configuration_0x11C;
         }

         return tags::invalid;

      }





      namespace {

         struct Offset {
            static uint32_t TAG() {
               return  0;
            }
            static uint32_t TYPE() {
               return  2;
            }
            static uint32_t LENGTH() {
               return  4;
            }
            static uint32_t OFFSET() {
               return  8;
            }
         };

         struct Size {
            static uint32_t LONG() {
               return  4;
            }
            static uint32_t SHORT() {
               return  2;
            }
            static uint32_t BYTE() {
               return  1;
            }
            static uint32_t DIR_ENTRY() {
               return  12;
            }
         };

         /*        1 uint16_t   Field type
                            1 - byte_t
                            2 - ASCII string, counted in length.
                                Most often an ASCIIZ string, the
                                trailing zero is counted with the
                                data length.
                            3 - uint16_t
                            4 - uint32_t / uword
                            5 - rational (2 dwords, numerator and denominator)
                            */

         struct TiffFieldType {
            static uint32_t byte_t() {
               return  1;
            }
            static uint32_t ascii() {
               return  2;
            }
            static uint32_t short_() {
               return  3;
            }
            static uint32_t long_() {
               return  4;
            }
            static uint32_t rational() {
               return 5 ;
            }
         };

         typedef TiffFieldType t_field;

         struct PlanarConfiguration {
            // 284 (11C)
            // SHORT
            // if Samples == 1, not used
            static uint32_t none()  {
               return 0;
            }
            static uint32_t RGBRGBRGB()  {
               return 1;
            }
            static uint32_t RRRGGGBBB()  {
               return 2;
            }
         };
         typedef PlanarConfiguration planar_cfg;


         struct PhotometricInterpretation {
            static size_t GRAY_white_is_min()  {
               return 0;
            }
            static size_t GRAY_black_is_min()  {
               return 1;
            }
            static size_t PALETTE() {
               return 3;
            }
            static size_t RGB() {
               return 2;
            }
         };
         typedef PhotometricInterpretation phi;
      }

      class cTiffInterface {

      public:

         cTiffInterface();


         int ReadFile( vector<uint8_t> const& buf );

         /* -- check endian
               return: 1 -->  big    endian   ( MAC, MIPS-SGI )
                       0 -->  little endian   ( PC, DECalpha )
            bool check_big_endian(){
               if(*(char *)&x == (char)1 )  //  byte_t Order: 1000  Intel, 0001 Mac
                  return false;       // intel
               return true;           // Motorola
               }
         */

         const uint8_t* buf( size_t offset )const {
            if( offset >= mBuf.size() ) {
               string e = "offset in buf is out of range";
               throw cTiffException( e );
            }

            return &mBuf[0] + offset;
         }
         uint8_t* buf( size_t offset ) {
            if( offset >= mBuf.size() ) {
               string e = "offset in buf is out of range";
               throw cTiffException( e );
            }

            return &mBuf[0] + offset;
         }

         uint32_t GetLongVariable( size_t off );
         uint16_t GetShortVariable( size_t off );


         static void PutShort( uint8_t* buf_, uint16_t val ) {
            memcpy( buf_, &val, 2 );
         }
         static void PutLong( uint8_t* buf_,  uint32_t val ) {
            memcpy( buf_, &val, 4 );
         }

         void  CopyToGrayImage( tImgLinear& im );
         void  CopyToPaletteImage( tImgLinear& im );

         //         word32 sx()const {
         //            return _size.x();
         //         }
         //         word32 sy()const {
         //            return _size.y();
         //         }
         void  sx( uint32_t i ) {
            _size.setx( i );
         }
         void  sy( uint32_t i ) {
            _size.sety( i );
         }
         void  size( uint32_xy s ) {
            _size = s;
         }
         uint32_xy  size()const {
            return _size;
         }

         uint32_t Pixels()const {
            return _size.x() * _size.y();
         }
         uint32_t GetOffset( const uint8_t* poff )const;


         string log();

         vector<uint8_t> mBuf;
         vector<uint16_t> mBuf16;
         void add_to_v16( uint16_t const* begin, uint16_t const* end ){
            assert( ((ptrdiff_t)(end-begin)) > 0  );
            while ( begin != end ) {
               mBuf16.push_back(*begin++);
            }
         }

         bool  mSwap ;

         uint32_t mRowsPerStrip;
         uint32_xy _size;

         uint32_t mStripes;
         uint32_t mStripeOffset;
         bool  mIsDwordOffset;
         uint32_t   photometric_interpr;
         string description;
         int   mCompression;
         uint32_t mOffsetColorMap;
         uint32_t   mPlanarConfiguration;
         uint32_t mBitsGray;
         uint32_t mBitsRed;
         uint32_t mBitsGreen;
         uint32_t mBitsBlue;
         uint32_t mBitsAlpha;
         uint32_t mSamples;


      };


      cTiffInterface::cTiffInterface(): mBuf( 1 ),
         mSwap( false ),
         mRowsPerStrip( 0 ),
         _size(),
         mStripes( 0 ),
         mStripeOffset( 0 ),
         mIsDwordOffset( false ),
         photometric_interpr( phi::GRAY_white_is_min() ),
         description(),
         mCompression( 1 ),
         mOffsetColorMap( 0 ),
         mPlanarConfiguration( 0 ),
         mBitsGray( 8 ),
         mBitsRed( 0 ),
         mBitsGreen( 0 ),
         mBitsBlue( 0 ),
         mBitsAlpha( 0 ),
         mSamples( 1 ) {

      }



      uint32_t cTiffInterface::GetLongVariable( size_t off ) {
         if( off >= mBuf.size() ) {
            return 0;
         }

         uint32_t var = 0;
         memcpy( &var, buf( off ), Size::LONG() );
         return var;
      }

      uint16_t cTiffInterface::GetShortVariable( size_t off ) {
         if( off >= mBuf.size() ) {
            return 0;
         }

         uint16_t var;
         memcpy( &var, buf( off ), Size::SHORT() );
         return var;
      }



      class cTiffEntry {


         void operator=( cTiffEntry const& );
      public:

         cTiffEntry( cTiffInterface& ti
                   ): t( ti ), mTag( 0 ), mType( 0 ), mLength( 0 ), mOffset( 0 ) {
         }

         // public: see memcpy in DecodeEntry, works at BIG and LITTLE endian
         cTiffInterface& t;
         uint16_t  mTag;
         uint16_t  mType;
         uint32_t mLength;
         uint32_t mOffset;
         void ToBuffer( uint8_t* buf )const;
         void SwapEntry();
      };




      string cTiffInterface::log( ) {
         string s;
         string str;
         str = "width,length:        " + stringify( _size.x() ) + ", " + stringify( _size.y() );
         s += str + "\n";

         if( mSamples == 3 ) {
            str = "bits per sample:     " + stringify( mBitsRed ) + " " + stringify( mBitsGreen ) + " " + stringify( mBitsBlue );
         } else {
            str = "bits per sample:  " + stringify( mBitsGray );
         }

         s += str + "\n";
         str = "rows per strip:      " + stringify( mRowsPerStrip );
         s += str + "\n";
         str = "compression:         " + stringify( mCompression );
         s += str + "\n";
         str = "photometric_interpr: " + stringify( photometric_interpr );
         s += str + "\n";
         str = "strips per image     " + stringify( mStripes );
         s += str + "\n";
         str = "samples per pixel    " + stringify( mSamples );
         s += str + "\n";
         str = "offset stripes       " + stringify( mStripeOffset );
         s += str + "\n";
         str = "offset colormap      " + stringify( mOffsetColorMap );
         s += str + "\n";
         str = "planar configuration " + stringify( mPlanarConfiguration );
         s += str + "\n";
         return s;
      }


      namespace {

         /* -- byte_t reverse WORD and uint32_t
         */
         inline void WORD_swap( uint16_t& buf ) {
            uint16_t b = buf;
            buf = ( uint16_t )( ( ( b << 8 ) & 0xff00 ) + ( ( b >> 8 ) & 0x00ff ) );
         }
         inline void DWORD_swap( uint32_t& buf ) {
            uint32_t b = buf;
            buf = ( ( ( b & 0x000000ff ) << 24 ) & 0xff000000 )
                  + ( ( ( b & 0x0000ff00 ) <<  8 ) & 0x00ff0000 )
                  + ( ( ( b & 0x00ff0000 ) >>  8 ) & 0x0000ff00 )
                  + ( ( ( b & 0xff000000 ) >> 24 ) & 0x000000ff );
         }

         //  -- byte_t-reverse WORD or uint32_t in a buffer, inplace
         void WORD_arr_swap( uint16_t* b, size_t l ) {
            while( l-- ) {
               WORD_swap( *b++ );
            }
         }


         void DWORD_arr_swap( uint32_t* b, size_t l ) {
            while( l-- ) {
               uint32_t d = *b;
               DWORD_swap( d );
               *b = d;
               b++;
            }
         }

         /*        1 uint16_t   Field type
                            1 - byte_t,          l = 1
                            2 - ASCII string, counted in length.
                                Most often an ASCIIZ string, the
                                trailing zero is counted with the
                                data length.,  l = 1
                            3 - uint16_t,          l = 2
                            4 - uint32_t / uword, l = 4
                            5 - rational (2 dwords, numerator and denominator)
                                l = 8
                            */
         size_t type_to_data_size( uint16_t type ) {
            if( type == t_field::byte_t() ) {
               return sizeof( uint8_t );
            }

            if( type == t_field::ascii() ) {
               return sizeof( uint8_t );
            }

            if( type == t_field::short_() ) {
               return sizeof( uint16_t );
            }

            if( type == t_field::long_() ) {
               return sizeof( uint32_t );
            }

            if( type == t_field::rational() ) {
               return 2 * sizeof( uint32_t );
            }

            return 0;
         }


         inline uint32_t align32( uint32_t s ) {
            if( s & 0x0003 ) {  // 0x11,  1,2, oder 3
               s |= 0x0003; // set to 0x11, (3)
               ++s;         // add 1 to align
            }

            return s;
         }

         void inverse( uint8_t* pimg, size_t count ) {
            while( count-- ) {
               *pimg = ( uint8_t )( UINT8_MAX - *pimg );
               pimg++;
            }
         }
         void inverse( uint16_t* pimg, size_t count ) {
            while( count-- ) {
               *pimg = ( uint16_t )( UINT16_MAX - *pimg );
               pimg++;
            }
         }

         bool IsBigEndian() {
            int x = 1;

            if( *( char* )&x == 1 ) {
               return false;
            }

            return true;
         }

      }


      void cTiffEntry::SwapEntry() {

         uint8_t const* b = t.buf( 0 );
         WORD_swap( mTag );
         WORD_swap( mType );
         DWORD_swap( mLength );

         int l = mLength;

         size_t data_size = type_to_data_size( mType ) *  mLength;

         // offset is an adress to data
         if( data_size > 4 ) {
            DWORD_swap( mOffset );
         }

         // byte_t, ascii, no swap
         if( mType == t_field::byte_t() ) {
            return;
         }

         if( mType == t_field::ascii() ) {
            return;
         }


         if( mType == t_field::short_() ) {

            uint16_t* wp = ( uint16_t* )( &( mOffset ) );

            if( l == 1 ) {
               WORD_swap( *( wp ) );
            }

            if( l == 2 ) {
               WORD_arr_swap( ( uint16_t* )( &( mOffset ) ), 2 );
            }

            if( l > 2 ) {
               WORD_arr_swap( ( uint16_t* )( b + mOffset ), l );
            }

            return;
         }

         if( mType == t_field::long_() ) {

            if( l == 1 ) {
               DWORD_swap( mOffset );
            } else {
               DWORD_arr_swap( ( uint32_t* )( b + mOffset ), l );
            }

            return;
         }


         if( mType == t_field::rational() ) {

            DWORD_arr_swap( ( uint32_t* )( b + mOffset ), 2 );
         }
      }

      uint32_t cTiffInterface::GetOffset( const uint8_t* poff )const {
         uint32_t offset4 = 0;
         uint16_t offset2 = 0;

         if( mIsDwordOffset != false ) {
            memcpy( &offset4, poff, sizeof( uint32_t ) );
         } else {
            memcpy( &offset2, poff, sizeof( uint16_t ) );
            offset4 = offset2;
         }

         return offset4;
      }

      void cTiffInterface::CopyToGrayImage( tImgLinear& im ) {
         size_t bytes = mRowsPerStrip * _size.x();

         if( mStripes == 1 ) {
            auto offset = mStripeOffset;

            if( photometric_interpr == phi::GRAY_white_is_min() ) { // 0
               inverse( buf( offset ), bytes );
            }

            im.insert_data( buf( offset ), bytes, 0 );
            return;
         }

         uint32_t im_offset = 0;
         uint32_t step   = mIsDwordOffset ? 4 : 2;

         uint32_t i;
         uint32_t offset = 0;

         const uint8_t* poff = buf( mStripeOffset );

         for( i = 0; i < ( mStripes - 1 ); i++ ) {
            offset = GetOffset( poff );

            if( photometric_interpr == phi::GRAY_white_is_min() ) { // 0
               inverse( buf( offset ), bytes );
            }

            im.insert_data( buf( offset ), bytes, im_offset );
            poff   += step;
            im_offset += bytes;
         }

         uint32_t last = Pixels() - im_offset;
         offset     = GetOffset( poff );

         if( photometric_interpr == 0 ) {
            inverse( buf( offset ), last );
         }

         im.insert_data( buf( offset ), last, im_offset );
      }

      void cTiffInterface::CopyToPaletteImage( tImgLinear& im ) {
         uint32_t bytes = mRowsPerStrip * _size.x();

         if( mStripes == 1 ) {
            uint32_t offset = mStripeOffset;

            if( photometric_interpr == phi::GRAY_white_is_min() ) { // 0
               inverse( buf( offset ), bytes );
            }

            im.insert_data( buf( offset ), bytes, 0 );
            return;
         }

         uint32_t im_offset = 0;
         uint32_t step   = mIsDwordOffset ? 4 : 2;

         uint32_t i;
         uint32_t offset = 0;

         const uint8_t* poff = buf( mStripeOffset );

         for( i = 0; i < ( mStripes - 1 ); i++ ) {
            offset = GetOffset( poff );

            if( photometric_interpr == phi::GRAY_white_is_min() ) { // 0
               inverse( ( uint8_t* )buf( offset ), bytes );
            }

            im.insert_data( buf( offset ), bytes, im_offset );
            poff   += step;
            im_offset += bytes;
         }

         uint32_t last = Pixels() - im_offset;
         offset     = GetOffset( poff );

         if( photometric_interpr == phi::GRAY_white_is_min() ) { // 0
            inverse( buf( offset ), last );
         }

         vector<uint8_t>v2( buf( offset ), buf( offset ) + last );
         im.insert_data( buf( offset ), last, im_offset );

      }

      class cTiffEntry;
      class cTiffInterface;

      class cTiff {

         // prevent use
         cTiff( cTiff const& );
         cTiff& operator=( cTiff const& );

      protected:
         int  DecodeTiffEntry( cTiffEntry* e );
         int  DecodeTiff( uint32_t offset, tImgLinear& img );
         int  CopyTiffStructureToImage( tImgLinear& im );
         cTiffInterface   t;


      public:
         cTiff();
         ~cTiff();

         void Description( string  const& n );
         string Description()const;
         void BitsGray( int i );
         void BitsBlue( int i );
         void BitsGreen( int i );
         void BitsRed( int i );

         void y( int y );
         void x( int x );
         void size( uint32_xy s );

         void PhotometricInterpretation( int i );
         void Samples( int i );
         void PlanarConfiguration( int p );

         void read( vector<uint8_t> const& buf, tImgLinear& im );
         void write( string const& file, tImgViewLinear const& im );



      };


#define TIFF_DEBUG

      int cTiff::DecodeTiffEntry( cTiffEntry* e ) {

         if( t.mSwap != false ) {
            e->SwapEntry();
         }

         tTiffTag  tag   = tTiffTag::value_to_tiff_tag( e->mTag );
         uint16_t  type      = e->mType;
         uint32_t length   = e->mLength;
         uint32_t offset   = e->mOffset;
         uint16_t const* wp = ( uint16_t const* ) & ( e->mOffset ) ;
         uint16_t  s_offset = *wp;

#ifdef TIFF_DEBUG
         string s;
         s =  " tag    " +  tag.to_string();
         s += " type   " + stringify( type );
         s += " length " + stringify( length );
         s += " offset " + stringify( offset );
         // LOGT_DEBUG( s );
#endif




         /* -- new subfile type
              */
         if( tag == tags::new_subfile_type_0xFE ) {
            if( type   != t_field::long_() ) {
               return  -tag.value;
            }

            if( length != 1 )    {
               return -tag.value;
            }

            return 0;
         }


         /* -- subfile type
              */
         if( tag  == tags::subfile_type_0xFF ) {
            return 0;
         }

         /* -- Image Width
              */
         if( tag == tags::image_width_0x100 ) {
            if( type == t_field::short_() ) {
               t.sx( ( unsigned long )s_offset );
               return 0;
            }

            if( type == t_field::long_() ) {
               t.sx( offset );
               return 0;
            }

            return -tag.value;
         }

         /* -- Image Length
              */
         if( tag == tags::image_length_0x101 ) {
            if( type == t_field::short_() ) {
               t.sy( ( unsigned long )s_offset );
               return 0;
            }

            if( type == t_field::long_() ) {
               t.sy( offset );
               return 0;
            }

            return -tag.value;
         }


         /* -- Bits Per Sample
              */
         if( tag == tags::bits_per_sample_0x102 ) {
            if( type == t_field::short_() ) {
               if( length == 1 ) {
                  t.mBitsGray = ( unsigned long )s_offset;
                  t.mBitsRed = 0;
                  t.mBitsGreen = 0;
                  t.mBitsBlue = 0;
                  t.mBitsAlpha = 0;
                  return 0;
               }

               if( length >= 3 ) {
                  t.mSamples = 3;
                  t.mBitsRed   = t.GetShortVariable( offset + 0 );
                  t.mBitsGreen = t.GetShortVariable( offset + 2 );
                  t.mBitsBlue  = t.GetShortVariable( offset + 4 );
                  t.mBitsGray = 0;

                  if( length == 4 ) {       // for 32 bit image, not supported
                     t.mSamples = 4;
                     t.mBitsAlpha = t.GetShortVariable( offset + 6 );
                  }

                  return 0;
               }
            }

            return -tag.value;
         }


         /* -- Compression
              */
         if( tag  == tags::compression_0x103 ) {
            if( type == t_field::short_() ) {
               t.mCompression = s_offset;
               return 0;
            }

            return -tag.value;
         }


         /* -- Photometric Interpretation
              */
         if( tag == tags::photometric_interpretation_0x106 ) {
            if( type == t_field::short_() ) {
               t.photometric_interpr = s_offset;
               return 0;
            }

            return -tag.value;
         }


         /* -- Description
              */
         if( tag == tags::description_0x10E ) { /* 270 */
            if( type == t_field::ascii() ) {
               if( length > 4 ) {
                  t.description = string( ( const char* )t.buf( offset ), length );
               } else {
                  t.description = string( ( const char* ) & offset, length );
               }

               return 0;
            }

            return -tag.value;
         }


         /* -- Strip offsets
              */
         if( tag == tags::strip_offsets_0x111 ) {

            if( type == t_field::long_() ) {
               t.mStripes     = length;
               t.mIsDwordOffset  = true;
               t.mStripeOffset = offset;
               return 0;
            }

            if( type == t_field::short_() ) {
               t.mStripes     = length;
               t.mIsDwordOffset  = false;
               t.mStripeOffset = ( unsigned long )s_offset;
               return 0;
            }

            return -tag.value;
         }


         /* -- Samples per Pixel
              */
         if( tag == tags::samples_per_pixel_0x115 ) {
            if( type == t_field::short_() ) {
               t.mSamples = ( unsigned long )s_offset;
               return 0;
            }

            return -tag.value;
         }

         /* -- Rows Per Strip
              */
         if( tag == tags::rows_per_strip_0x116 ) {
            if( type == t_field::long_() ) {
               t.mRowsPerStrip = offset;
               return 0;
            }

            if( type == t_field::short_() ) {
               t.mRowsPerStrip = ( unsigned long )s_offset;
               return 0;
            }

            return -tag.value;
         }

         /* -- Strip byte_t Counts
              */
         if( tag == tags::stripByte_counts_0x117 ) {

            return 0;
         }


         // -- Colormap  ( '*lut' in G_IM )
         if( tag == tags::colormap_0x140 ) {
            if( type == t_field::short_() ) {
               t.mOffsetColorMap = offset;
               return 0;
            }

            return -tag.value;
         }

         /* -- Planar Configuration                 */
         if( tag == tags::planar_configuration_0x11C ) {
            if( type == t_field::short_() ) {
               t.mPlanarConfiguration = s_offset;
               return 0;
            }

            return -tag.value;
         }

         return 0;

      }


      int cTiffInterface::ReadFile( vector<uint8_t> const& buf_ ) {

         //      systif.mIsSwap = true;  //   -- big endian
         //      systif.mIsSwap = false; //   -- little endian
         //      return: 1 -->  big    endian   ( MAC, MIPS-SGI )
         //              0 -->  little endian   ( PC, DECalpha )


         mBuf = buf_;
         char const* logo = ( char const* )buf( 0 );
         int lLogo = II_IntelLogo.size();
         char const* IntelLogo = II_IntelLogo.c_str();
         char const* MacLogo = MM_MacLogo.c_str();

         if( strncmp( logo, MacLogo, lLogo ) != 0  && strncmp( logo, IntelLogo, lLogo ) != 0 ) {
            string e;
            e = "Tifflogo is not 'II' or 'MM'";
            throw cTiffException( e );
         }

         bool big_endian = IsBigEndian();

         if( strncmp( logo, IntelLogo, lLogo ) == 0 ) {
            mSwap = big_endian;  /* -- II and little endian = no swap */
            /* -- II and big    endian = swap    */
            /* -- II and Intel CPU  = no swap */
            /* -- II and MAC CPU    = swap    */
         }

         if( strncmp( logo, MacLogo, lLogo ) == 0 ) {
            mSwap = !big_endian;  /* -- MM and little endian = swap  */
            /* -- MM and big endian = no swap  */
            /* -- MM and Intel CPU  = swap  */
            /* -- MM and MAC CPU    = no swap  */
         }

         return mBuf.size();
      }



      // return 0, if error
      // return 1, if ok

      void cTiff::read( vector<uint8_t> const& buf,  tImgLinear& im ) {



         try {
            t.ReadFile( buf );
         } catch( cTiffException const& ex ) {
            throw ex;
         }

         /* -- offset of first dir, 4 bytes after file start
           */
         int count = 0;
         uint32_t offset1 = 0;

         try {
            offset1      = t.GetLongVariable( 4 );

            if( t.mSwap ) {
               DWORD_swap( offset1 );
            }

            count = 0;

            // offset == 0, end of file
            while( offset1 ) {
               offset1 = DecodeTiff( offset1, im );

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
            throw cTiffException( s );
         }
      }

      // return: offset of next tiff directory
      // return 0, if no more dirs exist
      int cTiff::DecodeTiff( uint32_t offse, tImgLinear& im ) {

         uint16_t  entry_count = t.GetShortVariable( offse );

         if( t.mSwap ) {
            WORD_swap( entry_count );
         }

         t.mRowsPerStrip = 0;
         t.mSamples      = 1;

         cTiffEntry entry( t );
         uint8_t* p = t.buf( offse + Size::SHORT() ) ;

         for( int i = 0; i < entry_count; i++ ) {
            // use memcpy, works at BIG and LITTLE endian
            memcpy( &entry.mTag,     p + Offset::TAG(),   Size::SHORT() );
            memcpy( &entry.mType,    p + Offset::TYPE(),  Size::SHORT() );
            memcpy( &entry.mLength,  p + Offset::LENGTH(), Size::LONG() );
            memcpy( &entry.mOffset,  p + Offset::OFFSET(), Size::LONG() );
            p += Size::DIR_ENTRY();

            if( DecodeTiffEntry( &entry ) < 0 ) {
               char str[20];
               sprintf( str, "%0X", i );
               //LOGT_ERROR( ", Error in Tiff tag entry: " + tString( str ) );
               return -1;
            }
         }

         // last entry, offset of next IFD
         int offset_to_next_IFD = *( uint32_t* )p;
         int ret = CopyTiffStructureToImage( im );

         if( ret == -1 ) {
            return -1;
         }

         return offset_to_next_IFD;
      }

      // ret == -1 error
      // ret ==  0 ok
      int cTiff::CopyTiffStructureToImage( tImgLinear& img ) {

         // size and description are used in all image formats
         img.size() = uint32_xy( t._size.x(), t._size.y() );
         img.description( t.description );

         if( t.mRowsPerStrip <= 0  && t.mStripes == 1 ) {
            // all rows in one stripe
            t.mRowsPerStrip = t.size().y();
         }

         //
         // -- 8, 12, 16  bit gray images
         //
         if( t.photometric_interpr == phi::GRAY_white_is_min() /* 0 */
               || t.photometric_interpr == phi::GRAY_black_is_min() /* 1 */ ) {
            if( t.mBitsGray == 8 && t.mSamples == 1 ) {
               img.set_mono8();
               img.alloc_data_buffer();
               t.CopyToGrayImage( img );
            }

            if( t.mBitsGray == 16 && t.mSamples == 1 ) {
               //img.set_mono16();
               //img.alloc_data_buffer();
               // t.CopyToGrayImage( img );
               string e = "16 bit gray image is not supported: '";
               throw cTiffException( e );

            }

            // set a gray palette to images with no palette
            //for( int ic = 0; ic < 256; ic++ ) {
            //img::tRGB a( ic, ic, ic );
            //im.SetPaletteEntry( a, ic );
            //}

         }

         //
         // color map tiff
         //
         if( t.mBitsGray == 8 && t.mSamples == 1 && t.photometric_interpr == phi::PALETTE() /* 3 */ ) {
            img.set_palette();
            img.alloc_data_buffer();

            t.CopyToPaletteImage( img );
            uint32_t size_of_color_map = 256 * 3;
            vector<uint16_t> wbuf( size_of_color_map );
            memcpy( &wbuf[0], t.buf( t.mOffsetColorMap ), size_of_color_map * sizeof( uint16_t ) );
            vector<tRGBA> palette( 0x100 );

            for( int ic = 0; ic < 0x100; ic++ ) {
               tRGBA a;
               a.a() = 255;
               int r = wbuf [ 0 * 256 + ic ] >> 8 ;
               int g = wbuf [ 1 * 256 + ic ] >> 8 ;
               int b = wbuf [ 2 * 256 + ic ] >> 8 ;
               a.r() = wbuf [ 0 * 256 + ic ] >> 8 ;
               a.g() = wbuf [ 1 * 256 + ic ] >> 8 ;
               a.b() = wbuf [ 2 * 256 + ic ] >> 8 ;
               palette[ ic ] = a;
            }
            img.palette(palette);

         }

         // check for impossible combination of parameters
         if( t.photometric_interpr == phi::GRAY_white_is_min() /* 0 */
               || t.photometric_interpr == phi::GRAY_black_is_min() /* 1 */ ) {
            if( ( t.mBitsGray != 8 && t.mBitsGray != 16 && t.mBitsGray != 12 ) || t.mSamples != 1 ) {
               // ASSERT(false);
               // s << "l: "<<__LINE__<<", f: "<<__FILE__<<"  ";

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
         if( t.photometric_interpr ==  phi::RGB()  /* 2 */ && t.mBitsRed == 8 ) { // 8 bit RGB
            img.set_rgb();
            img.alloc_data_buffer();
            img.description( t.description );

            switch( t.mPlanarConfiguration ) {
            case 1:                                  // RGBRGBRGB...

               // N = StripsPerImage for PlanarConfiguration equal to 1; N = SamplesPerPixel * StripsPerImage for PlanarConfiguration equal to 2
               if( t.mStripes == 1 ) {

                  vector<uint8_t>v( t.buf( t.mStripeOffset ), t.buf( t.mStripeOffset ) +  t.Pixels() * 3 );
                  img.insert_data( v, 0 );
               } else {
                  uint32_t copied = 0;
                  uint32_t bytes = t.mRowsPerStrip * t.size().x() * 3;
                  uint32_t offset;
                  uint32_t i;
                  uint32_t step = t.mIsDwordOffset ? 4 : 2;
                  const uint8_t* poff = t.buf( t.mStripeOffset );
                  size_t offset_img = 0;
                  int a = 0;

                  for( i = 0; i < ( t.mStripes - 1 ); i++ ) {
                     offset = t.GetOffset( poff );
                     //cout << i << endl;
                     vector<uint8_t>v( t.buf( offset ), t.buf( offset ) +  bytes );
                     if( i == t.mStripes / 2 ){
                        a = 1;
                     }
                     img.insert_data( v, offset_img );
                     offset_img += bytes;
                     poff   += step;
                     copied += bytes;
                  }

                  uint32_t pix = img.bytes();
                  uint32_t last = pix - copied;
                  offset = t.GetOffset( poff );
                  vector<uint8_t>v2( t.buf( offset ), t.buf( offset ) +  last );
                  img.insert_data( v2, offset_img );

               }

               break;

            case 2://RRRR GGGGG BBBB

               if( t.mStripes == 3 ) {
                  uint32_t roffset;
                  uint32_t goffset;
                  uint32_t boffset;

                  if( t.mIsDwordOffset ) {
                     memcpy( &roffset, t.buf( t.mStripeOffset + 0 ), 4 );
                     memcpy( &goffset, t.buf( t.mStripeOffset + 4 ), 4 );
                     memcpy( &boffset, t.buf( t.mStripeOffset + 8 ), 4 );
                  } else {
                     uint16_t a;
                     memcpy( &a, t.buf( t.mStripeOffset + 0 ), 2 );
                     roffset = ( unsigned long )a;
                     memcpy( &a, t.buf( t.mStripeOffset + 2 ), 2 );
                     goffset = ( unsigned long )a;
                     memcpy( &a, t.buf( t.mStripeOffset + 4 ), 2 );
                     boffset = ( unsigned long )a;
                  }

                  vector<uint8_t> v( img.bytes() );
                  size_t count = t.Pixels();
                  //size_t index = 0;
                  uint8_t* vptr = &v[0];
                  tRGB* target = ( tRGB* )( vptr );
                  uint8_t const* red = t.buf( roffset );
                  uint8_t const* red_end = red + count;
                  uint8_t const* green = t.buf( goffset );
                  uint8_t const* blue = t.buf( boffset );

                  while( red < red_end ) {
                     //tRGB rgb;
                     //rgb.r() = *red;
                     //rgb.g() = *green;
                     //rgb.b() = *blue;
                     *target++ = tRGB(*red++, *green++, *blue++  );
                     //target++;
                     //red++;
                     //green++;
                     //blue++;
                     //index++;
                  }

                  img.insert_data( v, 0 );
                  break;
               } else {
                  string e;
                  e =  "unknown tiff file type 'TiffRead', ";
                  //e += file;
                  e += "24 bit image, strips per image != 3";
                  throw cTiffException( e );
               }

            default:
               string e = "planar configuration is wrong: '";
               e += stringify( t.mPlanarConfiguration );
               e += "' in 24 bit image: ";
               //e +=        im.fullname() ;
               throw cTiffException( e );
            }
         } // end rgb 8 bit

         if( t.photometric_interpr ==  phi::RGB()  /* 2 */ && t.mBitsRed == 16 ) { // 16 bit RGB
            string e = "planar configuration RGB and 16 bit is not supported: '";
            e += "' in 24 bit RGB image: ";
            throw cTiffException( e );

            img.set_rgb();
            img.alloc_data_buffer();
            img.description( t.description );

            switch( t.mPlanarConfiguration ) {
            case 1:                                  // RGBRGBRGB...

               // N = StripsPerImage for PlanarConfiguration equal to 1; N = SamplesPerPixel * StripsPerImage for PlanarConfiguration equal to 2
               if( t.mStripes == 1 ) {

                  vector<uint8_t>v( t.buf( t.mStripeOffset ), t.buf( t.mStripeOffset ) +  t.Pixels() * 3 * 2 );
                  img.insert_data( v, 0 );
               } else {
                  t.mBuf16.clear();
                  uint32_t copied = 0;
                  uint32_t bytes = t.mRowsPerStrip * t.size().x() * 3;
                  uint32_t offset;
                  uint32_t i;
                  uint32_t step = t.mIsDwordOffset ? 4 : 2;
                  const uint8_t* poff = t.buf( t.mStripeOffset );
                  size_t offset_img = 0;
int a = 0;
                  uint8_t const* b = nullptr;
                  uint16_t const* b16 = nullptr;
                  for( i = 0; i < ( t.mStripes - 1 ); i++ ) {
                     offset = t.GetOffset( poff );
                     //cout << i << endl;
                     b = t.buf( offset );
                     b16 = ( uint16_t const* )b;

                     t.add_to_v16(b16, b16 +  bytes  );
                     if( i == t.mStripes / 2 ){
                        a = 1;
                     }
                     offset_img += bytes;
                     poff   += step;
                     copied += bytes;
                  }

                  uint32_t pix = img.bytes();
                  uint32_t last = pix - copied;
                  offset = t.GetOffset( poff );
                  b = t.buf( offset );
                  b16 = ( uint16_t const* )b;
                  t.add_to_v16(b16, b16 +  last  );

               }
               t.mBuf = to_v8( t.mBuf16 );

               break;

            case 2://RRRR GGGGG BBBB

               if( t.mStripes == 3 ) {
                  uint32_t roffset;
                  uint32_t goffset;
                  uint32_t boffset;

                  if( t.mIsDwordOffset ) {
                     memcpy( &roffset, t.buf( t.mStripeOffset + 0 ), 4 );
                     memcpy( &goffset, t.buf( t.mStripeOffset + 4 ), 4 );
                     memcpy( &boffset, t.buf( t.mStripeOffset + 8 ), 4 );
                  } else {
                     uint16_t a;
                     memcpy( &a, t.buf( t.mStripeOffset + 0 ), 2 );
                     roffset = ( unsigned long )a;
                     memcpy( &a, t.buf( t.mStripeOffset + 2 ), 2 );
                     goffset = ( unsigned long )a;
                     memcpy( &a, t.buf( t.mStripeOffset + 4 ), 2 );
                     boffset = ( unsigned long )a;
                  }

                  vector<uint8_t> v( img.bytes() );
                  size_t count = t.Pixels();
                  size_t index = 0;
                  uint8_t* vptr = &v[0];
                  tRGB* target = ( tRGB* )( vptr );
                  uint8_t const* red = t.buf( roffset );
                  uint8_t const* green = t.buf( goffset );
                  uint8_t const* blue = t.buf( boffset );

                  while( index < count ) {
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
                  throw cTiffException( e1 );
         }
            default:
               string e2 = "planar configuration is wrong: '";
               e2 += stringify( t.mPlanarConfiguration );
               e2 += "' in 24 bit image: ";
               //e +=        im.fullname() ;
               throw cTiffException( e2 );
            }
         }


         //        img.set_rgb();
         //        img.alloc_data_buffer();
         //        img.description( t.description );

         //        switch( t.mPlanarConfiguration ) {
         //        case 1:                                  // RGBRGBRGB...

         //          // N = StripsPerImage for PlanarConfiguration equal to 1; N = SamplesPerPixel * StripsPerImage for PlanarConfiguration equal to 2
         //          if( t.mStripes == 1 ) {

         //            vector<uint8_t>v( t.buf( t.mStripeOffset ), t.buf( t.mStripeOffset ) +  t.Pixels() * 3 * 2  );
         //            img.insert_data( v, 0 );
         //          } else {
         //            uint32_t copied = 0;
         //            uint32_t bytes = t.mRowsPerStrip * t.size().x() * 3 * sizeof(uint16_t);
         //            uint32_t offset;
         //            uint32_t i;
         //            uint32_t step = t.mIsDwordOffset ? 4 : 2;
         //            const byte_t* poff = t.buf( t.mStripeOffset );
         //            size_t offset_img = 0;

         //            for( i = 0; i < ( t.mStripes - 1 ); i++ ) {
         //              offset = t.GetOffset( poff );
         //              //cout << i << endl;
         //              vector<uint8_t>v( t.buf( offset ), t.buf( offset ) +  bytes );
         //              img.insert_data( v, offset_img );
         //              offset_img += bytes;
         //              poff   += step;
         //              copied += bytes;
         //            }

         //            uint32_t pix = img.bytes();
         //            uint32_t last = pix - copied;
         //            offset = t.GetOffset( poff );
         //            vector<uint8_t>v2( t.buf( offset ), t.buf( offset ) +  last );
         //            img.insert_data( v2, offset_img );

         //          }

         //          break;

         //        case 2://RRRR GGGGG BBBB

         //          if( t.mStripes == 3 ) {
         //            uint32_t roffset;
         //            uint32_t goffset;
         //            uint32_t boffset;

         //            if( t.mIsDwordOffset ) {
         //              memcpy( &roffset, t.buf( t.mStripeOffset + 0 ), 4 );
         //              memcpy( &goffset, t.buf( t.mStripeOffset + 4 ), 4 );
         //              memcpy( &boffset, t.buf( t.mStripeOffset + 8 ), 4 );
         //            } else {
         //              uint16_t a;
         //              memcpy( &a, t.buf( t.mStripeOffset + 0 ), 2 );
         //              roffset = ( unsigned long )a;
         //              memcpy( &a, t.buf( t.mStripeOffset + 2 ), 2 );
         //              goffset = ( unsigned long )a;
         //              memcpy( &a, t.buf( t.mStripeOffset + 4 ), 2 );
         //              boffset = ( unsigned long )a;
         //            }

         //            vector<byte_t> v( img.bytes() );
         //            size_t count = t.Pixels();
         //            size_t index = 0;
         //            byte_t* vptr = &v[0];
         //            ns_img::tRGB* target = ( ns_img::tRGB* )( vptr );
         //            byte_t const* red = t.buf( roffset );
         //            byte_t const* green = t.buf( goffset );
         //            byte_t const* blue = t.buf( boffset );

         //            while( index < count ) {
         //              ns_img::tRGB rgb;
         //              rgb.r() = *red;
         //              rgb.g() = *green;
         //              rgb.b() = *blue;
         //              *target = rgb;
         //              target++;
         //              red++;
         //              green++;
         //              blue++;
         //            }

         //            img.insert_data( v, 0 );
         //            break;
         //          } else {
         //            string e;
         //            e =  "unknown tiff file type 'TiffRead', ";
         //            //e += file;
         //            e += "24 bit image, strips per image != 3";
         //            throw cTiffException( e );
         //          }

         //        default:
         //          string e = "planar configuration is wrong: '";
         //          e += stringify( t.mPlanarConfiguration );
         //          e += "' in 24 bit image: ";
         //          //e +=        im.fullname() ;
         //          throw cTiffException( e );
         //        }
         //      } // end rgb 16bit



         return 0;
      }


      string cTiff::Description()const {
         return t.description;
      }

      void cTiff::Description( string  const& n ) {
         t.description = n;
      }

      /*
         . ctor{
         */
      cTiff ::cTiff(): t() {

      }

      cTiff::~cTiff() {
         // delete &t;
      }

      const uint32_t number_of_colors = 256;

      void cTiffEntry::ToBuffer( uint8_t* buf )const {
         memcpy( buf + Offset::TAG()   , &mTag,    Size::SHORT() );
         memcpy( buf + Offset::TYPE()  , &mType,   Size::SHORT() );
         memcpy( buf + Offset::LENGTH(), &mLength, Size::LONG() );
         memcpy( buf + Offset::OFFSET(), &mOffset, Size::LONG() );
      }


      void read( vector<uint8_t> const& buf, tImgLinear& img ) {

         cTiff tr;
         tr.read( buf, img );
      }


      void write( string const& file, tImgViewLinear const& img )  {

         // Check for supported file types
         if( !img.is_mono8() &&  !img.is_rgb() ) {
            throw tImgWriteEx( msg_image_format_not_supported );
         }

         cTiff tif;
         // defaults  == Gray images
         tif.BitsGray( 8 );
         tif.BitsRed( 0 );
         tif.BitsGreen( 0 );
         tif.BitsBlue( 0 );
         tif.Samples( 1 );
         tif.PlanarConfiguration( planar_cfg::none() /* 0 */ );

         /* -- initialize parameters
         */

         if( img.is_mono8() ) {
            tif.PhotometricInterpretation( phi::GRAY_black_is_min() );
         }

         if( img.is_rgb() != false ) {
            tif.PhotometricInterpretation( phi::RGB() );
            tif.PlanarConfiguration( planar_cfg::RGBRGBRGB() /* 1 */ );   // RGBRGBRGB...
            tif.BitsGray( 0 );
            tif.BitsRed( 8 );
            tif.BitsGreen( 8 );
            tif.BitsBlue( 8 );
            tif.Samples( 3 );
         }

         tif.size( img.size() );
         tif.Description( img.description() );

         try {
            tif.write( file, img );
         } catch( cTiffException& ) {
            throw tImgWriteEx( msg_image_format_not_supported );
         }
      }

      void WriteToFile( string const& file, vector<uint8_t> const& b );

      void cTiff::write( string const& file, tImgViewLinear const& im ) {
         cTiffEntry entry( t );
         uint32_t string_offset;
         uint8_t* dir_ptr;
         uint32_t color_map_offset;
         //int l;

         size_t entry_count = 0;

         if( t.photometric_interpr == phi::GRAY_black_is_min() ) { // 1
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

         if( t.photometric_interpr == phi::RGB() ) { // 2
            entry_count = 12;    /* 12 dir entries, 11 + einmal 24 bit      */
         }

         if( t.photometric_interpr == phi::PALETTE() ) { // 3
            entry_count = 12;    /* 12 dir entries, 11 + einmal 8 bit colormap */
         }


         /* -- full size stripes
           */
         t.mRowsPerStrip =  t.size().y();
         t.mStripes      =  1;
         uint32_t strip_byte_counts =  t.Pixels() * t.mSamples;

         if( t.mBitsGray == 16 ) {
            strip_byte_counts =  t.Pixels() * t.mSamples * 2;
         }


         /* -- buffer for the pointer and data
           */

         // -- calculate data size
         uint32_t data_size = 4 + Size::LONG();                     // -- first 4 + dir offset

         uint32_t dir_entry_offset = data_size;

         data_size += Size::SHORT();                       // -- entry count
         uint32_t dir_offset = data_size;

         data_size += ( entry_count + 1 ) * Size::DIR_ENTRY(); // -- tiff dir
         data_size = align32( data_size );

         string_offset = data_size;




         /* -- tag 0x10E  -- Image Description
           */
         int namelength = t.description.length() + 1;

         if( namelength > 4 ) {
            data_size += namelength;
         }

         /* -- tag 0x102  -- Bits Per Sample
           */
         uint32_t bits_per_sample_offset = data_size;

         if( t.mSamples == 3 ) {
            data_size += 3 * Size::SHORT();
         }

         data_size = align32( data_size );


         /* -- tag 0x140  -- Colormap
           */
         int colormap_size = number_of_colors * 3 ;
         color_map_offset  = data_size;

         if( t.photometric_interpr == phi::PALETTE() ) { // 3
            data_size  += colormap_size * sizeof( uint16_t );
         }

         data_size = align32( data_size );

         /* -- img
           */
         uint32_t img_offset = data_size;

         if( t.mBitsGray == 16 ) {
            data_size += 2 * t.Pixels() * t.mSamples;
         } else {
            data_size += t.Pixels() * t.mSamples;
         }


         vector< uint8_t > buf( data_size );
         uint8_t* ptrbuf = &buf[0];

         /* -- write first 4 Bytes
           */
         *( ptrbuf )   = 'I';
         *( ptrbuf + 1 ) = 'I';
         *( ptrbuf + 2 ) = '*';
         *( ptrbuf + 3 ) = '\0';

         /* -- write offset of directory
           */
         memcpy( ptrbuf + 4, &dir_entry_offset, Size::LONG() );

         memcpy( ptrbuf + dir_entry_offset, &entry_count, Size::SHORT() );


         size_t dir_count = 0;
         dir_ptr    = ptrbuf + dir_offset;

         /* -- 0. tag 0xFE    -- new subfile type
           */
         entry.mTag    = tags::new_subfile_type_0xFE; /*  0xFE */
         entry.mType   = t_field::long_();
         entry.mLength = 1    ;
         entry.mOffset = 0    ;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 1. tag 0x100    -- Image Width
           */
         entry.mTag    = tags::image_width_0x100;
         entry.mType   = t_field::long_();
         entry.mLength = 1    ;
         entry.mOffset = t.size().x();
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 2. tag 0x101    -- Image Length
           */
         entry.mTag    = tags::image_length_0x101;
         entry.mType   = t_field::long_();
         entry.mLength = 1    ;
         entry.mOffset = t.size().y();
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 3. tag 0x102  -- Bits Per Sample
           */
         entry.mTag    = tags::bits_per_sample_0x102;
         entry.mType   = t_field::short_() ;

         if( t.mSamples == 3 ) {
            uint8_t* data = ( ptrbuf + bits_per_sample_offset );
            entry.mLength = 3    ;
            entry.mOffset = bits_per_sample_offset;
            t.PutShort( data, ( uint16_t )t.mBitsRed );
            data += 2;
            t.PutShort( data, ( uint16_t )t.mBitsGreen );
            data += 2;
            t.PutShort( data, ( uint16_t )t.mBitsBlue );
            data += 2;
         } else {
            entry.mLength = 1    ;
            entry.mOffset = ( unsigned long )t.mBitsGray & 0xffff;
         }

         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;


         /* -- 4. tag 0x103  -- Compression
           */
         entry.mTag    = tags::compression_0x103;
         entry.mType   = t_field::short_();
         entry.mLength = 1    ;
         entry.mOffset = ( unsigned long )t.mCompression & UINT16_MAX;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 5. tag 0x106  -- Photometric Interpretation
           */
         entry.mTag    = tags::photometric_interpretation_0x106;
         entry.mType   = t_field::short_();
         entry.mLength = 1    ;
         entry.mOffset = ( unsigned long )t.photometric_interpr & UINT16_MAX;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 6. tag 0x10E  -- Image Description
           */
         entry.mTag    = tags::description_0x10E;
         entry.mType   = t_field::ascii();

         int descriptionlength = t.description.length() + 1;

         if( descriptionlength > 4 ) {
            strcpy( ( char* )( ptrbuf + string_offset ), t.description.c_str() );
            entry.mLength = descriptionlength;
            entry.mOffset = string_offset;
            string_offset += descriptionlength;
         } else {
            entry.mLength = descriptionlength;
            strcpy( ( char* )( &( entry.mOffset ) ), t.description.c_str() );
         }

         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;


         /* -- 7. tag 0x111  -- Stripe Offsets
           */
         entry.mTag    = 0x111 ;
         entry.mType   = t_field::short_();
         entry.mLength = 1;
         entry.mOffset = img_offset;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 8. tag 0x115  -- Samples per Pixel
           */
         entry.mTag    = tags::samples_per_pixel_0x115;
         entry.mType   = t_field::short_();
         entry.mLength = 1    ;
         entry.mOffset = t.mSamples;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 9. tag 0x116  -- Rows Per Strip
           */
         entry.mTag    = tags::rows_per_strip_0x116;
         entry.mType   = t_field::long_();
         entry.mLength = 1    ;
         entry.mOffset = t.mRowsPerStrip;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;

         /* -- 10. tag 0x117  -- Strip byte_t Counts
           */
         entry.mTag    = tags::stripByte_counts_0x117;
         entry.mType   = t_field::long_() ;
         entry.mLength = 1;
         entry.mOffset = strip_byte_counts;
         entry.ToBuffer( dir_ptr );
         dir_ptr += Size::DIR_ENTRY();
         dir_count++;



         /* -- Planar Configuration   -- 24 bit images
           //      tif.photometric_interpr = 2;
           //      tif.mPlanarConfiguration     = 1;   // RGBRGBRGB...
           */
         if( t.mPlanarConfiguration == planar_cfg::RGBRGBRGB()    /* 1 */
               &&
               t.photometric_interpr == phi::RGB() /* 2 */
           ) {

            entry.mTag    = tags::planar_configuration_0x11C;
            entry.mType   = t_field::short_();
            entry.mLength = 1    ;
            entry.mOffset = ( unsigned long )t.mPlanarConfiguration & UINT16_MAX;
            entry.ToBuffer( dir_ptr );
            dir_ptr += Size::DIR_ENTRY();
            dir_count++;
         }

         if( t.photometric_interpr == phi::PALETTE() ) {

            /* -- 11. tag 0x140  -- Colormap
             */
            entry.mTag    = tags::colormap_0x140;
            entry.mType   = t_field::short_();
            entry.mLength = colormap_size;
            entry.mOffset = color_map_offset;
            entry.ToBuffer( dir_ptr );
            dir_ptr += Size::DIR_ENTRY();
            dir_count++;
         }

         /* -- last entry == 0
           */
         entry.mTag    = 0;
         entry.mType   = 0;
         entry.mLength = 0;
         entry.mOffset = 0;
         entry.ToBuffer( dir_ptr );

         /* -- copy the colormap
           */
         if( t.photometric_interpr == phi::PALETTE() ) { // 3
            uint16_t* plut = ( uint16_t* )( ptrbuf + color_map_offset );
            size_t count = 0;
            size_t nr = number_of_colors;

            vector<tRGBA> v( 256 );

            for( count = 0; count < nr; count++ ) {
               uint16_t temp = 0;
               temp = v[ count ].r();
               temp = ( temp << 8 );
               *( plut + 0 * nr + count )  = temp;
               temp = v[ count ].g();
               temp = ( temp << 8 );
               *( plut + 1 * nr + count )  = temp;
               temp = v[ count ].b();
               temp = ( temp << 8 );
               *( plut + 2 * nr + count )  = temp;
            }
         }

         /* -- write number of entries
           */
         entry_count = dir_count;

         /* -- copy the image
           */
         if( t.mSamples == 1 && t.mBitsGray == 8 ) {
            uint8_t* p8 = ptrbuf + img_offset;
            size_t sx = im.pitch();

            for( size_t iy = 0; iy < im.size().y(); iy++ ) {
               uint8_t const* buf1 = im.row_ptr( iy );
               memcpy( p8, buf1, im.size().x() );
               p8 += sx;
            }
         }

         if( t.mSamples == 3 ) {
            uint8_t* ptr = ptrbuf + img_offset;

            for( size_t iy = 0; iy < im.size().y(); iy++ ) {
               tRGB const* source = ( tRGB const* )( im.row_ptr( iy ) );
               tRGB* target = ( tRGB* )ptr;

               size_t count = im.size().x();
               size_t index = 0;

               while( index < count ) {
                  *target = *source;
                  target++;
                  source++;
                  index++;
               }

               ptr += t.size().x() * t.mSamples;
            }
         }

         WriteToFile( file,  buf );
      }


      void WriteToFile( string const& file, vector<uint8_t> const& b ) {

         try {
            bin_data::tWriteBin()( file, b );
         } catch( bin_data::tBinWriteEx& ) {
            throw  cTiffException( " write_file failed " );
         }
      }



      void cTiff::PhotometricInterpretation( int i ) {
         t.photometric_interpr = i;
      }
      void cTiff::x( int x_ ) {
         t.sx( x_ );
      }
      void cTiff::y( int y_ ) {
         t.sy( y_ );
      }
      void cTiff::size( uint32_xy s ) {
         t.size( s );
      }
      void cTiff::PlanarConfiguration( int p ) {
         t.mPlanarConfiguration = p;
      }
      void cTiff::BitsGray( int i ) {
         t.mBitsGray = i;
      }
      void cTiff::BitsRed( int i ) {
         t.mBitsRed = i;
      }
      void cTiff::BitsGreen( int i ) {
         t.mBitsGreen = i;
      }

      void cTiff::BitsBlue( int i ) {
         t.mBitsBlue = i;
      }

      void cTiff::Samples( int i ) {
         t.mSamples = i;
      }


   } // end of namespace tiff_local



   void t_write_tiff::write( string const& fn, tImgViewLinear const& img )   {
      // Check for supported file types
      if( !img.is_mono8() &&  !img.is_rgb() ) {
         throw tImgWriteEx( rlf::tiff_local::msg_image_format_not_supported );
      }

      rlf::tiff_local::write( fn, img );
   }
   void t_write_tiff::write( string const& fn, tImgPlanar const& img )   {
      tImgLinear temp;
      convert( img, temp );
      tImgViewLinear v( temp );
      write( fn, v ) ;
   }

   void t_read_tiff::read( string const& fn, tImgLinear& img )  {
      if( !fs::file_exists( fn ) ) {
         string s = bin_data::msg::file_not_exists( fn );
         throw tImgReadEx( s );
      }

      vector<uint8_t>  buf;

      try {
         bin_data::tReadBin()( fn, buf );
      }  catch( bin_data::tBinReadEx ex ) {
         throw tImgReadEx( bin_data::msg::file_not_exists( fn ) );
      }

      try {
         tiff_local::read( buf, img );
      } catch( tiff_local::cTiffException& ex ) {
         throw tImgReadEx( ex.what() + fn );
      }

   }

   // read write panar, simple forward to tImgLinear
   void t_read_tiff::read( string const& fn, tImgPlanar& planar )  {
      if( !fs::file_exists( fn ) ) {
         string s = bin_data::msg::file_not_exists( fn );
         throw tImgReadEx( s );
      }

      tImgLinear rimg;
      read( fn, rimg );
      tImgViewLinear v( rimg );
      convert( v, planar );
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






