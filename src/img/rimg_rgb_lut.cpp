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
File: cRGBLut.cpp
*/


#include <assert.h>

#include <fstream>
#include <iostream>
#include <list>
#include <iterator>


#include <cRGBLut.h>

#include "rList.h"
#include "wList.h"
#include "tostring.h"
#include "exception_std.h"

using namespace rlf;

namespace rlf {
   namespace nImg {


      namespace {
         static const byte lgreen[ 768 ] = {
            0, 252,   0,
            0, 252,   0,
            0, 252,   0,
            0, 248,   0,
            0, 248,   0,
            0, 248,   0,
            0, 248,   0,
            0, 244,   0,
            0, 244,   0,
            0, 244,   0,
            0, 244,   0,
            0, 240,   0,
            0, 240,   0,
            0, 240,   0,
            0, 240,   0,
            0, 236,   0,
            0, 236,   0,
            0, 236,   0,
            0, 236,   0,
            0, 232,   0,
            0, 232,   0,
            0, 232,   0,
            0, 232,   0,
            0, 228,   0,
            0, 228,   0,
            0, 228,   0,
            0, 228,   0,
            0, 224,   0,
            0, 224,   0,
            0, 224,   0,
            0, 224,   0,
            0, 220,   0,
            0, 220,   0,
            0, 220,   0,
            0, 220,   0,
            0, 216,   0,
            0, 216,   0,
            0, 216,   0,
            0, 216,   0,
            0, 212,   0,
            0, 212,   0,
            0, 212,   0,
            0, 212,   0,
            0, 208,   0,
            0, 208,   0,
            0, 208,   0,
            0, 208,   0,
            0, 204,   0,
            0, 204,   0,
            0, 204,   0,
            0, 204,   0,
            0, 200,   0,
            0, 200,   0,
            0, 200,   0,
            0, 200,   0,
            0, 196,   0,
            0, 196,   0,
            0, 196,   0,
            0, 196,   0,
            0, 192,   0,
            0, 192,   0,
            0, 192,   0,
            0, 192,   0,
            0, 188,   0,
            0, 188,   0,
            0, 188,   0,
            0, 188,   0,
            0, 184,   0,
            0, 184,   0,
            0, 184,   0,
            0, 184,   0,
            0, 180,   0,
            0, 180,   0,
            0, 180,   0,
            0, 180,   0,
            0, 176,   0,
            0, 176,   0,
            0, 176,   0,
            0, 176,   0,
            0, 172,   0,
            0, 172,   0,
            0, 172,   0,
            0, 172,   0,
            0, 168,   0,
            0, 168,   0,
            0, 168,   0,
            0, 168,   0,
            0, 164,   0,
            0, 164,   0,
            0, 164,   0,
            0, 164,   0,
            0, 160,   0,
            0, 160,   0,
            0, 160,   0,
            0, 160,   0,
            0, 156,   0,
            0, 156,   0,
            0, 156,   0,
            0, 156,   0,
            0, 152,   0,
            0, 152,   0,
            0, 152,   0,
            0, 152,   0,
            0, 148,   0,
            0, 148,   0,
            0, 148,   0,
            0, 148,   0,
            0, 144,   0,
            0, 144,   0,
            0, 144,   0,
            0, 144,   0,
            0, 140,   0,
            0, 140,   0,
            0, 140,   0,
            0, 140,   0,
            0, 136,   0,
            0, 136,   0,
            0, 136,   0,
            0, 136,   0,
            0, 132,   0,
            0, 132,   0,
            0, 132,   0,
            0, 132,   0,
            0, 128,   0,
            0, 128,   0,
            0, 128,   0,
            0, 128,   0,
            0, 124,   0,
            0, 124,   0,
            0, 124,   0,
            0, 124,   0,
            0, 120,   0,
            0, 120,   0,
            0, 120,   0,
            0, 120,   0,
            0, 116,   0,
            0, 116,   0,
            0, 116,   0,
            0, 116,   0,
            0, 112,   0,
            0, 112,   0,
            0, 112,   0,
            0, 112,   0,
            0, 108,   0,
            0, 108,   0,
            0, 108,   0,
            0, 108,   0,
            0, 104,   0,
            0, 104,   0,
            0, 104,   0,
            0, 104,   0,
            0, 100,   0,
            0, 100,   0,
            0, 100,   0,
            0, 100,   0,
            0,  96,   0,
            0,  96,   0,
            0,  96,   0,
            0,  96,   0,
            0,  92,   0,
            0,  92,   0,
            0,  92,   0,
            0,  92,   0,
            0,  88,   0,
            0,  88,   0,
            0,  88,   0,
            0,  88,   0,
            0,  84,   0,
            0,  84,   0,
            0,  84,   0,
            0,  84,   0,
            0,  80,   0,
            0,  80,   0,
            0,  80,   0,
            0,  80,   0,
            0,  76,   0,
            0,  76,   0,
            0,  76,   0,
            0,  76,   0,
            0,  72,   0,
            0,  72,   0,
            0,  72,   0,
            0,  72,   0,
            0,  68,   0,
            0,  68,   0,
            0,  68,   0,
            0,  68,   0,
            0,  64,   0,
            0,  64,   0,
            0,  64,   0,
            0,  64,   0,
            0,  60,   0,
            0,  60,   0,
            0,  60,   0,
            0,  60,   0,
            0,  56,   0,
            0,  56,   0,
            0,  56,   0,
            0,  56,   0,
            0,  52,   0,
            0,  52,   0,
            0,  52,   0,
            0,  52,   0,
            0,  48,   0,
            0,  48,   0,
            0,  48,   0,
            0,  48,   0,
            0,  44,   0,
            0,  44,   0,
            0,  44,   0,
            0,  44,   0,
            0,  40,   0,
            0,  40,   0,
            0,  40,   0,
            0,  40,   0,
            0,  36,   0,
            0,  36,   0,
            0,  36,   0,
            0,  36,   0,
            0,  32,   0,
            0,  32,   0,
            0,  32,   0,
            0,  32,   0,
            0,  28,   0,
            0,  28,   0,
            0,  28,   0,
            0,  28,   0,
            0,  24,   0,
            0,  24,   0,
            0,  24,   0,
            0,  24,   0,
            0,  20,   0,
            0,  20,   0,
            0,  20,   0,
            0,  20,   0,
            0,  16,   0,
            0,  16,   0,
            0,  16,   0,
            0,  16,   0,
            0,  12,   0,
            0,  12,   0,
            0,  12,   0,
            0,  12,   0,
            0,  11,   0,
            0,  10,   0,
            0,   9,   0,
            0,   8,   0,
            0,   7,   0,
            0,   6,   0,
            0,   5,   0,
            0,   4,   0,
            0,   4,   0,
            0,   3,   0,
            0,   2,   0,
            0,   1,   0,
            0,   0,   0
         };


         /* -- leica red lut
         */
         static const byte glowscale[ 768 ] = {
            0,     0,     0,
            0,     0,     0,
            4,     0,     0,
            4,     0,     0,
            8,     0,     0,
            8,     0,     0,
            12,     0,     0,
            12,     0,     4,
            16,     0,     4,
            16,     0,     4,
            20,     0,     4,
            20,     0,     4,
            24,     0,     4,
            24,     0,     4,
            28,     0,     4,
            28,     0,     8,
            32,     0,     8,
            32,     0,     8,
            36,     0,     8,
            36,     0,     8,
            40,     0,     8,
            40,     0,     8,
            44,     0,     8,
            44,     0,     8,
            48,     0,    12,
            48,     0,    12,
            52,     0,    12,
            52,     0,    12,
            56,     0,    12,
            56,     0,    12,
            60,     0,    12,
            60,     4,    12,
            64,     4,    12,
            64,     4,    12,
            68,     4,    12,
            68,     4,    12,
            72,     4,    16,
            72,     4,    16,
            76,     4,    16,
            80,     4,    16,
            80,     4,    16,
            84,     4,    16,
            84,     4,    16,
            88,     4,    16,
            88,     8,    16,
            92,     8,    16,
            92,     8,    16,
            96,     8,    16,
            96,     8,    16,
            100,     8,    16,
            100,     8,    16,
            104,     8,    16,
            104,     8,    16,
            108,    12,    16,
            108,    12,    16,
            112,    12,    16,
            112,    12,    16,
            116,    12,    16,
            116,    12,    16,
            120,    12,    16,
            124,    12,    16,
            124,    12,    16,
            128,    16,    16,
            128,    16,    16,
            132,    16,    20,
            132,    16,    20,
            136,    16,    20,
            136,    16,    20,
            140,    16,    20,
            140,    20,    20,
            144,    20,    20,
            144,    20,    20,
            148,    20,    20,
            148,    20,    20,
            152,    24,    20,
            152,    24,    20,
            156,    24,    20,
            156,    24,    20,
            156,    24,    20,
            160,    28,    20,
            160,    28,    20,
            164,    28,    20,
            164,    28,    20,
            168,    32,    20,
            168,    32,    20,
            168,    32,    20,
            172,    32,    20,
            172,    36,    20,
            176,    36,    20,
            176,    36,    20,
            176,    36,    20,
            180,    40,    20,
            180,    40,    20,
            180,    40,    20,
            184,    44,    20,
            184,    44,    20,
            188,    44,    20,
            188,    48,    20,
            188,    48,    20,
            192,    48,    20,
            192,    52,    20,
            192,    52,    20,
            192,    52,    20,
            196,    56,    20,
            196,    56,    20,
            196,    60,    20,
            200,    60,    20,
            200,    60,    20,
            200,    64,    20,
            200,    64,    20,
            204,    68,    20,
            204,    68,    20,
            204,    68,    20,
            208,    72,    20,
            208,    72,    20,
            208,    76,    20,
            208,    76,    20,
            208,    80,    20,
            212,    80,    20,
            212,    84,    20,
            212,    84,    20,
            212,    88,    20,
            212,    88,    20,
            216,    92,    20,
            216,    92,    20,
            216,    96,    20,
            216,    96,    20,
            216,   100,    20,
            220,   100,    20,
            220,   104,    20,
            220,   104,    20,
            220,   108,    20,
            220,   108,    20,
            220,   112,    20,
            220,   112,    20,
            224,   116,    20,
            224,   116,    24,
            224,   120,    24,
            224,   120,    24,
            224,   124,    24,
            224,   124,    24,
            224,   128,    24,
            228,   128,    24,
            228,   132,    24,
            228,   132,    28,
            228,   136,    28,
            228,   136,    28,
            228,   140,    28,
            228,   140,    28,
            228,   144,    32,
            232,   144,    32,
            232,   144,    32,
            232,   148,    32,
            232,   148,    32,
            232,   152,    36,
            232,   152,    36,
            232,   156,    36,
            232,   156,    36,
            236,   160,    40,
            236,   160,    40,
            236,   160,    40,
            236,   164,    40,
            236,   164,    44,
            236,   168,    44,
            236,   168,    44,
            236,   172,    48,
            236,   172,    48,
            236,   172,    48,
            240,   176,    52,
            240,   176,    52,
            240,   180,    52,
            240,   180,    56,
            240,   180,    56,
            240,   184,    60,
            240,   184,    60,
            240,   188,    60,
            240,   188,    64,
            240,   188,    64,
            240,   192,    68,
            244,   192,    68,
            244,   192,    72,
            244,   192,    72,
            244,   192,    76,
            244,   200,    76,
            244,   200,    80,
            244,   200,    80,
            244,   204,    84,
            244,   204,    84,
            244,   204,    88,
            244,   208,    88,
            244,   208,    92,
            244,   208,    92,
            248,   212,    96,
            248,   212,    96,
            248,   212,   100,
            248,   216,   104,
            248,   216,   104,
            248,   216,   108,
            248,   220,   108,
            248,   220,   112,
            248,   220,   112,
            248,   224,   116,
            248,   224,   120,
            248,   224,   120,
            248,   224,   124,
            248,   228,   124,
            248,   228,   128,
            248,   228,   128,
            248,   228,   132,
            248,   232,   136,
            248,   232,   136,
            252,   232,   140,
            252,   232,   144,
            252,   236,   144,
            252,   236,   148,
            252,   236,   148,
            252,   236,   152,
            252,   240,   156,
            252,   240,   156,
            252,   240,   160,
            252,   240,   160,
            252,   240,   164,
            252,   240,   168,
            252,   244,   168,
            252,   244,   172,
            252,   244,   172,
            252,   244,   176,
            252,   244,   180,
            252,   244,   180,
            252,   248,   184,
            252,   248,   188,
            252,   248,   188,
            252,   248,   192,
            252,   248,   192,
            252,   248,   196,
            252,   248,   200,
            252,   248,   200,
            252,   252,   204,
            252,   252,   208,
            252,   252,   208,
            252,   252,   212,
            252,   252,   212,
            252,   252,   216,
            252,   252,   220,
            252,   252,   220,
            252,   252,   224,
            252,   252,   228,
            252,   252,   228,
            252,   252,   232,
            252,   252,   236,
            252,   252,   236,
            252,   252,   240,
            252,   252,   244,
            252,   252,   244,
            252,   252,   248,
            255,   255,   255
         };

         static const byte log_table[ 256 ] = {
            0,   0,  31,  50,  63,  74,  82,  89,  95, 101, 105, 110, 114, 118, 121, 124,
            127, 130, 133, 135, 137, 140, 142, 144, 146, 148, 149, 151, 153, 154, 156, 158,
            159, 160, 162, 163, 164, 166, 167, 168, 169, 170, 172, 173, 174, 175, 176, 177,
            178, 179, 180, 180, 181, 182, 183, 184, 185, 186, 186, 187, 188, 189, 189, 190,
            191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 199, 200, 201,
            201, 202, 202, 203, 203, 204, 204, 205, 206, 206, 207, 207, 208, 208, 209, 209,
            210, 210, 210, 211, 211, 212, 212, 213, 213, 214, 214, 215, 215, 215, 216, 216,
            217, 217, 217, 218, 218, 219, 219, 219, 220, 220, 221, 221, 221, 222, 222, 222,
            223, 223, 223, 224, 224, 225, 225, 225, 226, 226, 226, 227, 227, 227, 228, 228,
            228, 229, 229, 229, 229, 230, 230, 230, 231, 231, 231, 232, 232, 232, 232, 233,
            233, 233, 234, 234, 234, 234, 235, 235, 235, 236, 236, 236, 236, 237, 237, 237,
            237, 238, 238, 238, 238, 239, 239, 239, 239, 240, 240, 240, 240, 241, 241, 241,
            241, 242, 242, 242, 242, 243, 243, 243, 243, 244, 244, 244, 244, 244, 245, 245,
            245, 245, 246, 246, 246, 246, 246, 247, 247, 247, 247, 247, 248, 248, 248, 248,
            249, 249, 249, 249, 249, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251, 252,
            252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 255
         };

         static const byte ldos[] = {
            0,  0,  0,    // Schwarz
            0,  0, 168,   // Blau
            0, 168,  0,   // Grn
            0, 168, 168,  // Cyan
            168,  0,  0,    // Rot
            168,  0, 168,   // Magenta
            168, 84,  0,    // Braun
            168, 168, 168,  // Hellgrau
            84, 84, 84,    // Grau
            84, 84, 252,   // Hellblau
            84, 252, 84,   // Hellgrn
            84, 252, 252,  // HellCyan
            252, 84, 84,    // Hellrot
            252, 84, 252,   // Hell lila
            252, 252, 84,   // Gelb
            252, 252, 252   // Weiss
         };



         static void init_lut_spectrum( std::vector<cRGBAlpha> &p ) {
            int i;
            byte table[512];

            for( i = 0;  i < 128; i++ ) {
               table[i] = 0;
            }

            for( i = 0;  i <  64; i++ ) {
               table[i + 128] = ( byte )( 4 * i + 3 );
            }

            for( i = 192; i < 256; i++ ) {
               table[i] = ( byte )255;
            }

            // invert table to 256-512
            for( i = 0;  i < 256; i++ ) {
               table[i + 256] = table[255 - i];
            }


            // -- write red
            for( i = 0; i < 256; i++ ) {
               p[i].R( table[i] );
            }

            // -- write green
            for( i = 0; i < 256; i++ ) {
               p[i].G( table[i + 128] );
            }

            // -- write blue

            for( i = 0; i < 256; i++ ) {
               p[i].B( table[i + 256] );
            }
         }
      }



      vector<string> tokenize( string const& str, const string& delimiters ) {
         string::size_type pos_not_delimiter = str.find_first_not_of( delimiters, 0 );
         string::size_type pos_delimiter     = str.find_first_of( delimiters, pos_not_delimiter );
         vector<string> tokens;

         while( string::npos != pos_delimiter || string::npos != pos_not_delimiter ) {
            string::size_type length = pos_delimiter - pos_not_delimiter;
            tokens.push_back( str.substr( pos_not_delimiter, length ) );
            pos_not_delimiter = str.find_first_not_of( delimiters, pos_delimiter );
            pos_delimiter = str.find_first_of( delimiters, pos_not_delimiter );
         }

         return tokens;
      }

      vector<string> split( string const& l, string const& delimiters ) {
         vector<string> v = tokenize( l, delimiters );
         return v;
      }

      const word32 RGBlutSize = 256;


      std::vector<cRGBAlpha> c_RGBLut::Spectrum() {
         std::vector<cRGBAlpha> v( RGBlutSize );
         init_lut_spectrum( v );
         return v;
      }
      std::vector<cRGBAlpha> c_RGBLut::Gray() {
         std::vector<cRGBAlpha> v( RGBlutSize );

         for( word32 i = 0; i < v.size(); i++ ) {
            v[ i ].R( i );
            v[ i ].G( i );
            v[ i ].B( i );
         }

         return v;
      }


      std::vector<cRGBAlpha> c_RGBLut::Green() {
         std::vector<cRGBAlpha> v( RGBlutSize );

         for( size_t i = 0; i < v.size(); i++ ) {
            v[ i ] = cRGBAlpha( 0, i, 0 );
         }

         return v;
      }

      std::vector<cRGBAlpha> c_RGBLut::GlowScale() {

         byte const* p = glowscale;
         std::vector<cRGBAlpha> v( RGBlutSize );

         for( size_t i = 0; i < v.size(); i++ ) {
            v[ i ].R( p[ 3 * i     ] );
            v[ i ].G( p[ 3 * i + 1 ] );
            v[ i ].B( p[ 3 * i + 2 ] );
         }

         return v;
      }
      std::vector<cRGBAlpha> c_RGBLut::Range( word32 low, word32 high ) {
         word32 i;
         std::vector<cRGBAlpha> v = Linearize();

         for( i = 0;  i < low + 1;  i++ ) {
            v[i].R( 0 );    /* r */
            v[i].G( 0 );    /* g */
            v[i].B( 255 );  /* b */
         }

         for( i = high;  i < v.size();  i++ ) {
            v[i].R( 255 );  /* r */
            v[i].G( 0 );    /* g */
            v[i].B( 0 );    /* b */
         }

         return v;
      }

      std::vector<cRGBAlpha> c_RGBLut::Invert() {
         word32 i;
         std::vector<cRGBAlpha> v( RGBlutSize );

         for( i = 0; i < v.size(); i++ ) {
            int i1 = 255 - i;
            v[ i1 ].R( v[ i ].R() );
            v[ i1 ].G( v[ i ].G() );
            v[ i1 ].B( v[ i ].B() );
         }

         return v;
      }


      std::vector<cRGBAlpha> c_RGBLut::Linearize() {
         return Gray();
      }


      bool c_RGBLut::Write( string const& name, std::vector<cRGBAlpha> const& l )  {

         try {
            std::list<std::string> lines;

            for( word32 i = 0;  i < l.size();  i++ ) {

               tString s =
                  toString( l[i].R(), 6 );
               s += toString( l[i].G(), 6 );
               s += toString( l[i].B(), 6 );
               lines.push_back( s );
            }

            asciitext::tWriteAscii w;
            w.operator()( lines, name );

         } catch( asciitext::BadAsciiWrite& bw ) {
            throw rlf::cStdException( bw.what() );
         }

         return true;
      }

      std::vector<cRGBAlpha> c_RGBLut::Read( string const& file ) {
         std::vector<cRGBAlpha> v;

         try {
            asciitext::tReadAscii reader;
            std::list<std::string>  l = reader.operator()( file );

            std::list<std::string>::const_iterator begin = l.begin();
            std::list<std::string>::const_iterator end = l.end();

            while( begin != end ) {
               cRGBAlpha rgb;
               string line = *begin;
               vector<std::string> vec = tokenize( line, " " );

               if( vec.size() == 3 ) {
                  rgb.R( tString( vec[0] ).toULong() );
                  rgb.B( tString( vec[1] ).toULong() );
                  rgb.B( tString( vec[2] ).toULong() );
                  v.push_back( rgb );
               } else {
                  throw asciitext::BadAsciiRead( "incorrect RGB file, "  + file );
               }

               begin++;

            }

            return v;
         } catch( asciitext::BadAsciiRead& br ) {
            throw rlf::cStdException( br.what() );
            return v;
         }

      }

      std::vector<cRGBAlpha> c_RGBLut::ReadHex( string const& file ) {
         std::ifstream f( file.c_str(), std::ios::in | std::ios::binary );
         std::vector<cRGBAlpha> v;

         if( !f.bad() ) {
            f.seekg( 0, std::ios::end );
            word32 file_size = f.tellg();
            f.seekg( 0, std::ios::beg );


            if( file_size == 3 * ( word32 )256 ) {
               char m[5000];
               f.read( m, file_size );

               for( word32 i = 0; i < 256; i++ ) {
                  cRGBAlpha rgb;
                  rgb.R( m[ 3 * i + 0 ] );
                  rgb.G( m[ 3 * i + 1 ] );
                  rgb.B( m[ 3 * i + 2 ] );
                  v.push_back( rgb );
               }
            }
         }

         return v;
      }

      bool c_RGBLut::WriteHex( string const& file, std::vector<cRGBAlpha> const& l ) {
         bool ret = true;
         std::ofstream f( file.c_str(), std::ios::out | std::ios::binary );

         if( !f.bad() ) {
            int file_size = 768;
            std::vector<byte> m( 3 * l.size() );

            for( word32 i = 0; i < l.size(); i++ ) {
               m[ 3 * i + 0 ] = l[ i ].R();
               m[ 3 * i + 1 ] = l[ i ].G();
               m[ 3 * i + 2 ] = l[ i ].B();
            }

            f.write( ( char* )&m[0], file_size );
            int iocount = f.tellp();

            if( iocount != file_size ) {
               ret = false;
            }
         }

         return ret;
      }

   } // end of namespace nImg
} // end of namespace rlf

//EOF

