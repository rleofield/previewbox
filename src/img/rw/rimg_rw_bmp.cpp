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
/*! \file rimg_rw_bmp.cpp
\brief impl for read and write bmp images

Lib: librimgrw
*/

#include <iostream>
#include <sstream>
#include <string>
#include <boost/range/algorithm_ext/iota.hpp>

#include "boost_cstdint.h"
#include "rimg_read_write_bin.h"
#include "rimg_rgb_bgra_types.h"

#include "rimg_linear.h"
#include "rimg_planar.h"
#include "rimg_convert.h"
#include "rimg_fs.h"
#include "rimg_read_write.h"
#include "rimg_rw_exceptions.h"

using namespace std;

using rlf::t_read_bmp;
using rlf::t_write_bmp;

/*
* Microsoft DIB (".BMP") format images.
http://en.wikipedia.org/wiki/BMP_file_format
http://www.fileformat.info/format/bmp/sample/index.htm
*/

using rlf::tRGB;
using rlf::tBGRA;

using rlf::rimg_linear::tImgLinear;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_linear::tImgViewLinear;

using namespace rlf::rimg_convert;


namespace rlf {


   namespace bmp {
      namespace  {

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

         const string msg_image_format_not_supported  = "Image format not supported (must be 8 or 24 bit)";
         const string msg_write_file = "Couldn't write file: '%s'";
         inline string write_file( string const& s0 ) {
            return replace( msg_write_file, s0 );
         }


      } // end of local ns
   } // end of ns bmp

   namespace bmp_intern {


      /*! align to multiple of 4 == 32 bit boundary
      \return converted value
      \param s value to align
      */
      uint32_t align32( uint32_t s ) {
         if( s & 0x3 ) {  // 0011,  1,2, oder 3
            s |= 0x3;     // set to 0x11, (3)
            ++s;          // add 1 to align
         }

         return s;
      }

      class incrementBGRA {
         tBGRA val;
      public:
         incrementBGRA(): val() {}
         incrementBGRA& operator++() {
            val.b++;
            val.r++;
            val.g++;
            val.a = 255;
            return *this;
         }
         tBGRA operator()() {
            return val;
         }
         operator tBGRA() {
            return val;
         }
         operator string() {
            return "incrementRGBA";
         }

      };




      class INFO {
      public:
         uint32_t size;
         int32_t width;
         int32_t height;
         uint16_t  planes;
         uint16_t  bit_count;
         uint32_t compression;
         uint32_t size_image;
         int32_t  xPelsPerMeter;
         int32_t  yPelsPerMeter;
         uint32_t colors_used;
         uint32_t colors_important;
      };



#if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#pragma pack(push,2)
      struct  FILEHEADER {
         uint16_t   signature;
         uint32_t   size;
         uint16_t   reserved1;
         uint16_t   reserved2;
         uint32_t   offset_bits;
      } ;
#pragma pack(pop)
#endif
#if defined(__GNUC__) || defined(__CYGWIN__) || defined(__MWERKS__) || defined(__WATCOMC__) || defined(__PGI) || defined(__LCC__)
      struct  FILEHEADER {
         uint16_t   signature;
         uint32_t   size;
         uint16_t   reserved1;
         uint16_t   reserved2;
         uint32_t   offset_bits;
      } __attribute__( ( packed ) );
#endif

      const size_t fileheader_size =  sizeof( FILEHEADER );
      const size_t fileheader_offset = 0;

      const size_t infoheader_size =  sizeof( INFO );
      const size_t infoheader_offset =  fileheader_size;

      const size_t header_size = fileheader_size + infoheader_size;

      const size_t lut_size8 = 256 * sizeof( tBGRA );
      const size_t lut_offset8 = header_size;

      const size_t offsetpixeldata8      =   lut_offset8 + lut_size8;
      const size_t offsetpixeldata24     =   header_size;


      FILEHEADER& referenceToFILEHEADER( vector<uint8_t> const& v ) {
         return ( FILEHEADER& )( v[fileheader_offset] );
      }
      INFO& referenceToINFO( vector<uint8_t> const& v ) {
         return ( INFO& )( v[infoheader_offset] );
      }
      tBGRA* referenceToLUT( vector<uint8_t> const& v ) {
         return ( tBGRA* )( &v[lut_offset8] );
      }


      FILEHEADER& referenceToFILEHEADER( string const& v ) {
         return ( FILEHEADER& )( v[fileheader_offset] );
      }
      INFO& referenceToINFO( string const& v ) {
         return ( INFO& )( v[infoheader_offset] );
      }
      tBGRA* referenceToLUT( string const& v ) {
         return ( tBGRA* )( &v[lut_offset8] );
      }


      // compression
      const size_t  bmpBI_RGB  =      0; //     BI_RGB  none  Most common
      const size_t  bmpBI_RLE8 =      1; //     RLE 8-bit/pixel  Can be used only with 8-bit/pixel bitmaps
      const size_t  bmpBI_RLE4 =      2; //     RLE 4-bit/pixel   Can be used only with 4-bit/pixel bitmaps
      const size_t  bmpBI_BITFIELDS = 3; //     BI_BITFIELDS   Bit field or Huffman 1D compression for BITMAPCOREHEADER2   Pixel format defined by bit masks or Huffman 1D compressed bitmap for BITMAPCOREHEADER2
      const size_t  bmpBI_JPEG =      4; //     JPEG or RLE-24 compression for BITMAPCOREHEADER2   The bitmap contains a JPEG image or RLE-24 compressed bitmap for BITMAPCOREHEADER2
      const size_t  bmpBI_PNG =       5; //     PNG   The bitmap contains a PNG image

      //tLUT lutRamp;


      vector<tBGRA> getLinearLut() {
         vector<tBGRA> lut( 256 );
         boost::iota( lut, incrementBGRA() );
         return lut;
      }

      bool is_ramp( tBGRA const*   bmiColors ) {
         static vector<tBGRA> lut = getLinearLut();


         for( size_t i = 0; i < lut.size(); i++ ) {
            if( !(
                     lut[i].r == bmiColors[i].r
                     &&
                     lut[i].g == bmiColors[i].g
                     &&
                     lut[i].b == bmiColors[i].b )

              ) {
               return false;
            }
         }

         return true;
      }
      bool is_zero( tBGRA const*   bmiColors ) {

         for( size_t i = 0; i < 256; i++ ) {
            if( tBGRA() != bmiColors[i] ) {
               return false;
            }
         }

         return true;
      }


      const uint16_t BMP_signature = 0x4d42; // == MB, intel CPU
      //uint8_t bmp00 = *((char*) p);
      //uint8_t bmp11 = *(((char*)p)+1);
      //char sig0 = sig & 0xff;       // upper byte_t == first in file
      //char sig1 = sig >> 8 & 0xff;  // lower byte_t == second in file
      //char s0 = *((char*)bitmapfileheader);
      //char s1 = *((char*)(bitmapfileheader)+1);

      size_t info_width( INFO const&   info ) {
         return info.width < 0 ? -info.width : info.width;
      }

      size_t info_height( INFO const&   info ) {
         return info.height < 0 ? -info.height : info.height;
      }
   } // end of ns bmp_intern

   using namespace bmp_intern;

   namespace bmp_read_local {
      void read_8bit( uint8_t const* dataptr,
                      tSize size,
                      vector<uint8_t*> const& rows ) {


         // Copy and reverse sequence of image rows
         size_t w = size.w();
         size_t h = size.h();
         size_t aligned_w = bmp_intern::align32( static_cast<uint32_t>( w ) );

         for( size_t y = 0; y < h; y++ ) {
            size_t offset = ( h - y - 1 ) * aligned_w;
            uint8_t const* source = dataptr + offset;
            uint8_t* target = rows[ y ];
            memcpy( target, source, w );
         }
      }

      void read_8bit_as_rgb( tBGRA const* lut, uint8_t const* dataptr,
                             tSize size,
                             vector<uint8_t*> const& rows ) {


         // Copy and reverse sequence of image rows

         size_t w = size.w();
         size_t h = size.h();
         size_t aligned_w = bmp_intern::align32( static_cast<uint32_t>( w ) );

         for( size_t y = 0; y < h; y++ ) {
            size_t offset = ( h - y - 1 ) * aligned_w;
            uint8_t const* source = dataptr + offset;
            tRGB* target = reinterpret_cast< tRGB*>( rows[ y ] );
            // to rgb
            size_t count = w;

            while( count-- ) {
               tRGB rgb;
               rgb.r() = lut[ *source ].r;
               rgb.g() = lut[ *source ].g;
               rgb.b() = lut[ *source ].b;
               source++;
               *target = rgb;
               target++;
            }
         }
      }

      void read_rgb( uint8_t const* dataptr, tSize size, vector<uint8_t*> const& rows )  {

         size_t h = size.y();
         size_t w = size.x();
         size_t aligned_w = bmp_intern::align32( static_cast<uint32_t>( w * 3 ) );

         uint8_t const* ptr = dataptr + h * aligned_w;

         for( size_t y = 0; y < h; y++ ) {
            ptr -= aligned_w;
            tBGR const* source = reinterpret_cast< tBGR const* >( ptr );
            tRGB* target       = reinterpret_cast< tRGB*>( rows[ y ] );
            size_t count = w;

            while( count-- ) {
               *target++ = *source++;
            }

         }

         return;
      }

      void read_rgba( uint8_t const* dataptr, tSize size, vector<uint8_t*> const& rows )  {

         size_t h = size.y();
         size_t w = size.x();
         size_t aligned_w = bmp_intern::align32( static_cast<uint32_t>( w * 4 ) );

         uint8_t const* ptr = dataptr + h * aligned_w;

         for( size_t y = 0; y < h; y++ ) {
            ptr -= aligned_w;
            tBGRA const* source = reinterpret_cast< tBGRA const* >( ptr );
            tRGBA* target       = reinterpret_cast< tRGBA*>( rows[ y ] );
            size_t count = w;

            while( count-- ) {
               *target++ = *source++;
            }
         }

         return;
      }


      void fromFile( const std::string& filename, string& s0 ) {
         std::ifstream stream( filename.c_str() );
         s0.assign( std::istreambuf_iterator<char>( stream ) ,  std::istreambuf_iterator<char>() );
      }

      void read( string fn, tImgLinear& rimg ) {

         string buf0;
         vector<uint8_t> buf;

         try {
            bin_data::tReadBin()( fn, buf );
            //fromFile( fn, buf0 );

         }  catch( bin_data::tBinReadEx ex ) {
            throw tImgReadEx( bin_data::msg::file_not_exists( fn ) );
            //throw ;
         }


         FILEHEADER const& fileheader = referenceToFILEHEADER( buf );
         uint16_t sig = fileheader.signature;


         if( sig != BMP_signature ) {
            return ;
         }

         if( buf.size() == fileheader.size ) {
            try {
               INFO const& bitmapinfoheader = referenceToINFO( buf );
               size_t w = info_width( bitmapinfoheader );
               size_t h = info_height( bitmapinfoheader );
               // copy data to tImgLinear
               rimg.size() = tSize( w, h );

               uint8_t const* pbuf = &buf[0];
               uint8_t const* dataptr = pbuf + fileheader.offset_bits;


               // 8 bit
               if( bitmapinfoheader.bit_count == 8 ) {
                  // LUT is not supported, convert to rgb, if lut is not an gray ramp lut
                  tBGRA const* lut = referenceToLUT( buf );

                  if( is_ramp( lut ) ) {
                     rimg.set_mono8();
                     rimg.alloc_data_buffer();
                     read_8bit( dataptr, rimg.size(), rimg.rows() );
                     return;
                  }

                  if( is_zero( lut ) ) {
                     rimg.set_mono8();
                     rimg.alloc_data_buffer();
                     read_8bit( dataptr, rimg.size(), rimg.rows() );
                     return;
                  }

                  rimg.set_rgb();
                  rimg.alloc_data_buffer();
                  read_8bit_as_rgb( lut, dataptr, rimg.size(), rimg.rows() );
                  return;
               }

               // 24 bit
               if( bitmapinfoheader.bit_count == 24 ) {
                  rimg.set_rgb();
                  rimg.alloc_data_buffer();
                  read_rgb( dataptr, rimg.size(), rimg.rows() );
               }

               if( bitmapinfoheader.bit_count == 32 ) {
                  // read an rgba image, write an rgb image, alpha is lost
                  rimg.set_rgba();
                  rimg.alloc_data_buffer();
                  read_rgba( dataptr, rimg.size(), rimg.rows() );
               }



            } catch( tImgAllocEx& ex ) {
               throw tImgReadEx( ex.what() );
            }

         }
      }
   } // end of ns bmp_read_local
   namespace bmp_write_local {
      void write_mono8( vector<uint8_t>& buf, tSize size, vector<uint8_t*> const& rows )  {

         FILEHEADER& bitmapfileheader = referenceToFILEHEADER( buf );
         bitmapfileheader.signature = BMP_signature;
         bitmapfileheader.size =   buf.size();
         bitmapfileheader.reserved1 = 0;
         bitmapfileheader.reserved2 = 0;
         bitmapfileheader.offset_bits = offsetpixeldata8;

         INFO& bitmapinfoheader = referenceToINFO( buf );
         bitmapinfoheader.size = infoheader_size;
         bitmapinfoheader.width = size.x();
         bitmapinfoheader.height = size.y();
         bitmapinfoheader.bit_count = 8;
         bitmapinfoheader.planes = 1;
         bitmapinfoheader.colors_used = 0;
         bitmapinfoheader.colors_important = 0;
         bitmapinfoheader.compression = bmpBI_RGB;
         bitmapinfoheader.size_image = 0;
         bitmapinfoheader.xPelsPerMeter = 0;
         bitmapinfoheader.yPelsPerMeter = 0;

         tBGRA*    bmiColors = referenceToLUT( buf );

         // LUT is not supported, write a gray lut
         for( size_t i = 0; i < 256; i++ ) {
            bmiColors[i].r  = static_cast<uint8_t>( i );
            bmiColors[i].g  = static_cast<uint8_t>( i );
            bmiColors[i].b  = static_cast<uint8_t>( i );
            bmiColors[i].a  = 255;
         }

         uint8_t* dataptr = &buf[0] + bitmapfileheader.offset_bits;
         size_t h = info_height( bitmapinfoheader );
         size_t w = info_width( bitmapinfoheader );
         size_t aligned_w = bmp_intern::align32( w );

         for( size_t y = 0; y < h; y++ ) {
            size_t data_offset = ( h - y - 1 ) * aligned_w;
            uint8_t* target = dataptr + data_offset;
            uint8_t const* source = rows[ y ];
            size_t count = w;

            while( count-- ) {
               *target++ = *source++;
            }
         }
      }
      void write_rgb( vector<uint8_t>& buf, tSize size, vector<uint8_t*> const& rows )  {

         uint32_t aligned_w = bmp_intern::align32( size.x() );
         uint32_t size_image = aligned_w * size.y();

         uint32_t aligned_bgr_line_size = bmp_intern::align32( size.x()  * sizeof( tBGR ) );
         uint32_t filesize = header_size +  aligned_bgr_line_size  * size.y();

         int32_t w = size.w();
         int32_t h = size.h();

         FILEHEADER& bitmapfileheader = referenceToFILEHEADER( buf );
         bitmapfileheader.signature   = BMP_signature;
         bitmapfileheader.size        = filesize;
         bitmapfileheader.reserved1   = 0;
         bitmapfileheader.reserved2   = 0;
         bitmapfileheader.offset_bits = header_size;


         INFO& bitmapinfoheader =  referenceToINFO( buf );
         bitmapinfoheader.size   = infoheader_size;
         bitmapinfoheader.width  = w;
         bitmapinfoheader.height = h;
         bitmapinfoheader.bit_count = 24;
         bitmapinfoheader.planes = 1;
         bitmapinfoheader.colors_used = 0;
         bitmapinfoheader.colors_important = 0;
         bitmapinfoheader.compression = bmpBI_RGB;
         bitmapinfoheader.size_image = size_image;
         bitmapinfoheader.xPelsPerMeter = 0;
         bitmapinfoheader.yPelsPerMeter = 0;


         uint8_t* data = &buf[0] + bitmapfileheader.offset_bits;
         int32_t y = 0;

         for( y = 0; y < h; y++ ) {
            size_t data_offset = ( h - y - 1 ) * aligned_bgr_line_size;
            tBGR*  target = reinterpret_cast< tBGR* >( data + data_offset );
            uint8_t const* ptr8 = rows[ y ];
            tRGB const* source = reinterpret_cast< tRGB const* >( ptr8 );
            size_t count = w;

            while( count-- ) {
               *target++ = *source++;
            }
         }
      }
      void write_rgba( vector<uint8_t>& buf, tSize size, vector<uint8_t*> const& rows )  {

         size_t aligned_w = bmp_intern::align32( size.x() );
         uint32_t size_image = aligned_w * size.y();

         uint32_t aligned_bgr_line_size = bmp_intern::align32( size.x()  * sizeof( tBGRA ) );
         uint32_t filesize = header_size +  aligned_bgr_line_size  * size.y();

         int32_t w = size.w();
         int32_t h = size.h();

         FILEHEADER& bitmapfileheader = referenceToFILEHEADER( buf );
         bitmapfileheader.signature   = BMP_signature;
         bitmapfileheader.size        = filesize;
         bitmapfileheader.reserved1   = 0;
         bitmapfileheader.reserved2   = 0;
         bitmapfileheader.offset_bits = header_size;


         INFO& bitmapinfoheader =  referenceToINFO( buf );
         bitmapinfoheader.size   = infoheader_size;
         bitmapinfoheader.width  = w;
         bitmapinfoheader.height = h;
         bitmapinfoheader.bit_count = 32;
         bitmapinfoheader.planes = 1;
         bitmapinfoheader.colors_used = 0;
         bitmapinfoheader.colors_important = 0;
         bitmapinfoheader.compression = bmpBI_RGB;
         bitmapinfoheader.size_image = size_image;
         bitmapinfoheader.xPelsPerMeter = 0;
         bitmapinfoheader.yPelsPerMeter = 0;


         uint8_t* data = &buf[0] + bitmapfileheader.offset_bits;
         int32_t y = 0;

         for( y = 0; y < h; y++ ) {
            size_t data_offset = ( h - y - 1 ) * aligned_bgr_line_size;
            tBGRA*  target = reinterpret_cast< tBGRA* >( data + data_offset );
            uint8_t const* ptr8 = rows[ y ];
            tRGBA const* source = reinterpret_cast< tRGBA const* >( ptr8 );
            size_t count = w;

            while( count-- ) {
               *target++ = *source++;
            }


         }
      }


      void write( string const& fn, tImgViewLinear const& img )  {
         try {
            vector<uint8_t> buf;

            if( img.is_mono8() ) {
               buf.resize( offsetpixeldata8 + img.bytes() );
               write_mono8( buf, img.size(), img.rows() );
            }

            if( img.is_rgb() ) {
               uint32_t aligned_line_size = bmp_intern::align32( img.size().x()  * sizeof( tBGR ) );
               uint32_t filesize = header_size +  aligned_line_size  * img.size().y();
               buf.resize( filesize );
               write_rgb( buf, img.size(), img.rows() );
            }

            if( img.is_rgba() ) {
               uint32_t aligned_line_size = bmp_intern::align32( img.size().x()  * sizeof( tBGRA ) );
               uint32_t filesize = header_size +  aligned_line_size  * img.size().y();
               buf.resize( filesize );
               write_rgba( buf, img.size(), img.rows() );
            }

            bin_data::tWriteBin()( fn, buf );
         } catch( bin_data::tBinWriteEx& ) {
            throw  tImgWriteEx( bin_data::msg::write_file( fn ) );
         }
      }


   } // end of ns bmp_write_local



   void t_write_bmp::write( string const& fn, tImgViewLinear const& img )   {
      if( !img.is_mono8() &&  !img.is_rgb() &&  !img.is_rgba() ) {
         throw tImgWriteEx( bmp::msg_image_format_not_supported );
      }

      return bmp_write_local::write( fn, img );
   }
   void t_write_bmp::write( string const& fn, tImgPlanar const& img )   {
      tImgLinear temp;
      convert( img, temp );
      write( fn, temp ) ;
   }
   void t_read_bmp::read( string const& fn, tImgLinear& rimg )  {
      bmp_read_local::read( fn, rimg );
   }


   void t_read_bmp::read( string const& fn, tImgPlanar& planar )  {
      tImgLinear temp;
      bmp_read_local::read( fn, temp );
      convert( temp, planar );
   }



}
//EOF





