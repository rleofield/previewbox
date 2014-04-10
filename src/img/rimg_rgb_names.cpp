///* --------------------------------------------------------------------------
//Copyright 2013 by Richard Albrecht
//richard.albrecht@rleofield.de

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

//You should have received a copy of the GNU Lesser General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------
//*/

////////! \file rimg_rgb_names.cpp
////\brief names for colors

////Lib: librimgrw

////X11 RGB Names with values
////$Xorg: rgb.txt,v 1.3 2000/08/17 19:54:00 cpqbld Exp $
////from /etc/X11/rgb.txt




//#include <vector>
//#include <algorithm>

//#include <boost/assign/list_of.hpp>
//#include <boost/algorithm/string.hpp>

//#include "rimg_rgb_bgra_types.h"
//#include "rimg_rgb_names.h"

//using namespace std;
//using namespace boost::assign;

//namespace rlf {
//   // http://stackoverflow.com/questions/2236197/c-easiest-way-to-initialize-an-stl-vector-with-hardcoded-elements
//   template< typename T, size_t N >
//   std::vector<T> makeVector( const T( &data )[N] ) {
//      return std::vector<T>( data, data + N );
//   }


//   tRGBtxt::tRGBtxt( uint8_t r_, uint8_t g_, uint8_t b_, string const& t_ ): tRGB( r_, g_, b_ ), _txt( t_ ) {
//      boost::trim( _txt );
//   }
//   namespace {
//      struct tRGBKeys {

//         vector<string> s;
//         void operator()( tRGBtxt const& r ) {
//            s.push_back( r.txt() );
//         }

//      };
//      struct tRGBKeyFind {

//         string s;
//         tRGBKeyFind( string sin ): s( sin ) {}
//         bool operator()( tRGBtxt const& r ) {
//            if( r.txt() == s ) {
//               return true;
//            }

//            return false;
//         }

//      };

//   }

//   vector<string> tRGBtxt::keys() {

//      vector<string> v = for_each( rgb_txt.begin(), rgb_txt.end(), tRGBKeys() ).s;
//      return v;
//   }

//   tRGB const& tRGBtxt::lookup( string const& k ) {
//      vector<tRGBtxt> ::const_iterator i = find_if( rgb_txt.begin(), rgb_txt.end(), tRGBKeyFind( k ) );
//      return *i;
//   }

//   const tRGBtxt values[] = { tRGBtxt( 255, 250, 250,      "snow                      " ),
//                              tRGBtxt( 248, 248, 255,      "ghost white               " ),
//                              tRGBtxt( 248, 248, 255,      "GhostWhite                " ),
//                              tRGBtxt( 245, 245, 245,      "white smoke               " ),
//                              tRGBtxt( 245, 245, 245,      "WhiteSmoke                " ),
//                              tRGBtxt( 220, 220, 220,      "gainsboro                 " ),
//                              tRGBtxt( 255, 250, 240,      "floral white              " ),
//                              tRGBtxt( 255, 250, 240,      "FloralWhite               " ),
//                              tRGBtxt( 253, 245, 230,      "old lace                  " ),
//                              tRGBtxt( 253, 245, 230,      "OldLace                   " ),
//                              tRGBtxt( 250, 240, 230,      "linen                     " ),
//                              tRGBtxt( 250, 235, 215,      "antique white             " ),
//                              tRGBtxt( 250, 235, 215,      "AntiqueWhite              " ),
//                              tRGBtxt( 255, 239, 213,      "papaya whip               " ),
//                              tRGBtxt( 255, 239, 213,      "PapayaWhip                " ),
//                              tRGBtxt( 255, 235, 205,      "blanched almond           " ),
//                              tRGBtxt( 255, 235, 205,      "BlanchedAlmond            " ),
//                              tRGBtxt( 255, 228, 196,      "bisque                    " ),
//                              tRGBtxt( 255, 218, 185,      "peach puff                " ),
//                              tRGBtxt( 255, 218, 185,      "PeachPuff                 " ),
//                              tRGBtxt( 255, 222, 173,      "navajo white              " ),
//                              tRGBtxt( 255, 222, 173,      "NavajoWhite               " ),
//                              tRGBtxt( 255, 228, 181,      "moccasin                  " ),
//                              tRGBtxt( 255, 248, 220,      "cornsilk                  " ),
//                              tRGBtxt( 255, 255, 240,      "ivory                     " ),
//                              tRGBtxt( 255, 250, 205,      "lemon chiffon             " ),
//                              tRGBtxt( 255, 250, 205,      "LemonChiffon              " ),
//                              tRGBtxt( 255, 245, 238,      "seashell                  " ),
//                              tRGBtxt( 240, 255, 240,      "honeydew                  " ),
//                              tRGBtxt( 245, 255, 250,      "mint cream                " ),
//                              tRGBtxt( 245, 255, 250,      "MintCream                 " ),
//                              tRGBtxt( 240, 255, 255,      "azure                     " ),
//                              tRGBtxt( 240, 248, 255,      "alice blue                " ),
//                              tRGBtxt( 240, 248, 255,      "AliceBlue                 " ),
//                              tRGBtxt( 230, 230, 250,      "lavender                  " ),
//                              tRGBtxt( 255, 240, 245,      "lavender blush            " ),
//                              tRGBtxt( 255, 240, 245,      "LavenderBlush             " ),
//                              tRGBtxt( 255, 228, 225,      "misty rose                " ),
//                              tRGBtxt( 255, 228, 225,      "MistyRose                 " ),
//                              tRGBtxt( 255, 255, 255,      "white                     " ),
//                              tRGBtxt( 0,   0,   0,        "black                     " ),
//                              tRGBtxt( 47,  79,  79,       "dark slate gray           " ),
//                              tRGBtxt( 47,  79,  79,       "DarkSlateGray             " ),
//                              tRGBtxt( 47,  79,  79,       "dark slate grey           " ),
//                              tRGBtxt( 47,  79,  79,       "DarkSlateGrey             " ),
//                              tRGBtxt( 105, 105, 105,      "dim gray                  " ),
//                              tRGBtxt( 105, 105, 105,      "DimGray                   " ),
//                              tRGBtxt( 105, 105, 105,      "dim grey                  " ),
//                              tRGBtxt( 105, 105, 105,      "DimGrey                   " ),
//                              tRGBtxt( 112, 128, 144,      "slate gray                " ),
//                              tRGBtxt( 112, 128, 144,      "SlateGray                 " ),
//                              tRGBtxt( 112, 128, 144,      "slate grey                " ),
//                              tRGBtxt( 112, 128, 144,      "SlateGrey                 " ),
//                              tRGBtxt( 119, 136, 153,      "light slate gray          " ),
//                              tRGBtxt( 119, 136, 153,      "LightSlateGray            " ),
//                              tRGBtxt( 119, 136, 153,      "light slate grey          " ),
//                              tRGBtxt( 119, 136, 153,      "LightSlateGrey            " ),
//                              tRGBtxt( 190, 190, 190,      "gray                      " ),
//                              tRGBtxt( 190, 190, 190,      "grey                      " ),
//                              tRGBtxt( 211, 211, 211,      "light grey                " ),
//                              tRGBtxt( 211, 211, 211,      "LightGrey                 " ),
//                              tRGBtxt( 211, 211, 211,      "light gray                " ),
//                              tRGBtxt( 211, 211, 211,      "LightGray                 " ),
//                              tRGBtxt( 25,  25, 112,       "midnight blue             " ),
//                              tRGBtxt( 25,  25, 112,       "MidnightBlue              " ),
//                              tRGBtxt( 0,   0, 128,        "navy                      " ),
//                              tRGBtxt( 0,   0, 128,        "navy blue                 " ),
//                              tRGBtxt( 0,   0, 128,        "NavyBlue                  " ),
//                              tRGBtxt( 100, 149, 237,      "cornflower blue           " ),
//                              tRGBtxt( 100, 149, 237,      "CornflowerBlue            " ),
//                              tRGBtxt( 72,  61, 139,       "dark slate blue           " ),
//                              tRGBtxt( 72,  61, 139,       "DarkSlateBlue             " ),
//                              tRGBtxt( 106,  90, 205,      "slate blue                " ),
//                              tRGBtxt( 106,  90, 205,      "SlateBlue                 " ),
//                              tRGBtxt( 123, 104, 238,      "medium slate blue         " ),
//                              tRGBtxt( 123, 104, 238,      "MediumSlateBlue           " ),
//                              tRGBtxt( 132, 112, 255,      "light slate blue          " ),
//                              tRGBtxt( 132, 112, 255,      "LightSlateBlue            " ),
//                              tRGBtxt( 0,   0, 205,        "medium blue               " ),
//                              tRGBtxt( 0,   0, 205,        "MediumBlue                " ),
//                              tRGBtxt( 65, 105, 225,       "royal blue                " ),
//                              tRGBtxt( 65, 105, 225,       "RoyalBlue                 " ),
//                              tRGBtxt( 0,   0, 255,        "blue                      " ),
//                              tRGBtxt( 30, 144, 255,       "dodger blue               " ),
//                              tRGBtxt( 30, 144, 255,       "DodgerBlue                " ),
//                              tRGBtxt( 0, 191, 255,        "deep sky blue             " ),
//                              tRGBtxt( 0, 191, 255,        "DeepSkyBlue               " ),
//                              tRGBtxt( 135, 206, 235,      "sky blue                  " ),
//                              tRGBtxt( 135, 206, 235,      "SkyBlue                   " ),
//                              tRGBtxt( 135, 206, 250,      "light sky blue            " ),
//                              tRGBtxt( 135, 206, 250,      "LightSkyBlue              " ),
//                              tRGBtxt( 70, 130, 180,      "steel blue                " ),
//                              tRGBtxt( 70, 130, 180,      "SteelBlue                 " ),
//                              tRGBtxt( 176, 196, 222,      "light steel blue          " ),
//                              tRGBtxt( 176, 196, 222,      "LightSteelBlue            " ),
//                              tRGBtxt( 173, 216, 230,      "light blue                " ),
//                              tRGBtxt( 173, 216, 230,      "LightBlue                 " ),
//                              tRGBtxt( 176, 224, 230,      "powder blue               " ),
//                              tRGBtxt( 176, 224, 230,      "PowderBlue                " ),
//                              tRGBtxt( 175, 238, 238,      "pale turquoise            " ),
//                              tRGBtxt( 175, 238, 238,      "PaleTurquoise             " ),
//                              tRGBtxt( 0, 206, 209,      "dark turquoise            " ),
//                              tRGBtxt( 0, 206, 209,      "DarkTurquoise             " ),
//                              tRGBtxt( 72, 209, 204,      "medium turquoise          " ),
//                              tRGBtxt( 72, 209, 204,      "MediumTurquoise           " ),
//                              tRGBtxt( 64, 224, 208,      "turquoise                 " ),
//                              tRGBtxt( 0, 255, 255,      "cyan                      " ),
//                              tRGBtxt( 224, 255, 255,      "light cyan                " ),
//                              tRGBtxt( 224, 255, 255,      "LightCyan                 " ),
//                              tRGBtxt( 95, 158, 160,      "cadet blue                " ),
//                              tRGBtxt( 95, 158, 160,      "CadetBlue                 " ),
//                              tRGBtxt( 102, 205, 170,      "medium aquamarine         " ),
//                              tRGBtxt( 102, 205, 170,      "MediumAquamarine          " ),
//                              tRGBtxt( 127, 255, 212,      "aquamarine                " ),
//                              tRGBtxt( 0, 100,   0,      "dark green                " ),
//                              tRGBtxt( 0, 100,   0,      "DarkGreen                 " ),
//                              tRGBtxt( 85, 107,  47,      "dark olive green          " ),
//                              tRGBtxt( 85, 107,  47,      "DarkOliveGreen            " ),
//                              tRGBtxt( 143, 188, 143,      "dark sea green            " ),
//                              tRGBtxt( 143, 188, 143,      "DarkSeaGreen              " ),
//                              tRGBtxt( 46, 139,  87,      "sea green                 " ),
//                              tRGBtxt( 46, 139,  87,      "SeaGreen                  " ),
//                              tRGBtxt( 60, 179, 113,      "medium sea green          " ),
//                              tRGBtxt( 60, 179, 113,      "MediumSeaGreen            " ),
//                              tRGBtxt( 32, 178, 170,      "light sea green           " ),
//                              tRGBtxt( 32, 178, 170,      "LightSeaGreen             " ),
//                              tRGBtxt( 152, 251, 152,      "pale green                " ),
//                              tRGBtxt( 152, 251, 152,      "PaleGreen                 " ),
//                              tRGBtxt( 0, 255, 127,      "spring green              " ),
//                              tRGBtxt( 0, 255, 127,      "SpringGreen               " ),
//                              tRGBtxt( 124, 252,   0,      "lawn green                " ),
//                              tRGBtxt( 124, 252,   0,      "LawnGreen                 " ),
//                              tRGBtxt( 0, 255,   0,      "green                     " ),
//                              tRGBtxt( 127, 255,   0,      "chartreuse                " ),
//                              tRGBtxt( 0, 250, 154,      "medium spring green       " ),
//                              tRGBtxt( 0, 250, 154,      "MediumSpringGreen         " ),
//                              tRGBtxt( 173, 255,  47,      "green yellow              " ),
//                              tRGBtxt( 173, 255,  47,      "GreenYellow               " ),
//                              tRGBtxt( 50, 205,  50,      "lime green                " ),
//                              tRGBtxt( 50, 205,  50,      "LimeGreen                 " ),
//                              tRGBtxt( 154, 205,  50,      "yellow green              " ),
//                              tRGBtxt( 154, 205,  50,      "YellowGreen               " ),
//                              tRGBtxt( 34, 139,  34,      "forest green              " ),
//                              tRGBtxt( 34, 139,  34,      "ForestGreen               " ),
//                              tRGBtxt( 107, 142,  35,      "olive drab                " ),
//                              tRGBtxt( 107, 142,  35,      "OliveDrab                 " ),
//                              tRGBtxt( 189, 183, 107,      "dark khaki                " ),
//                              tRGBtxt( 189, 183, 107,      "DarkKhaki                 " ),
//                              tRGBtxt( 240, 230, 140,      "khaki                     " ),
//                              tRGBtxt( 238, 232, 170,      "pale goldenrod            " ),
//                              tRGBtxt( 238, 232, 170,      "PaleGoldenrod             " ),
//                              tRGBtxt( 250, 250, 210,      "light goldenrod yellow    " ),
//                              tRGBtxt( 250, 250, 210,      "LightGoldenrodYellow      " ),
//                              tRGBtxt( 255, 255, 224,      "light yellow              " ),
//                              tRGBtxt( 255, 255, 224,      "LightYellow               " ),
//                              tRGBtxt( 255, 255,   0,      "yellow                    " ),
//                              tRGBtxt( 255, 215,   0,      "gold                      " ),
//                              tRGBtxt( 238, 221, 130,      "light goldenrod           " ),
//                              tRGBtxt( 238, 221, 130,      "LightGoldenrod            " ),
//                              tRGBtxt( 218, 165,  32,      "goldenrod                 " ),
//                              tRGBtxt( 184, 134,  11,      "dark goldenrod            " ),
//                              tRGBtxt( 184, 134,  11,      "DarkGoldenrod             " ),
//                              tRGBtxt( 188, 143, 143,      "rosy brown                " ),
//                              tRGBtxt( 188, 143, 143,      "RosyBrown                 " ),
//                              tRGBtxt( 205,  92,  92,      "indian red                " ),
//                              tRGBtxt( 205,  92,  92,      "IndianRed                 " ),
//                              tRGBtxt( 139,  69,  19,      "saddle brown              " ),
//                              tRGBtxt( 139,  69,  19,      "SaddleBrown               " ),
//                              tRGBtxt( 160,  82,  45,      "sienna                    " ),
//                              tRGBtxt( 205, 133,  63,      "peru                      " ),
//                              tRGBtxt( 222, 184, 135,      "burlywood                 " ),
//                              tRGBtxt( 245, 245, 220,      "beige                     " ),
//                              tRGBtxt( 245, 222, 179,      "wheat                     " ),
//                              tRGBtxt( 244, 164,  96,      "sandy brown               " ),
//                              tRGBtxt( 244, 164,  96,      "SandyBrown                " ),
//                              tRGBtxt( 210, 180, 140,      "tan                       " ),
//                              tRGBtxt( 210, 105,  30,      "chocolate                 " ),
//                              tRGBtxt( 178,  34,  34,      "firebrick                 " ),
//                              tRGBtxt( 165,  42,  42,      "brown                     " ),
//                              tRGBtxt( 233, 150, 122,      "dark salmon               " ),
//                              tRGBtxt( 233, 150, 122,      "DarkSalmon                " ),
//                              tRGBtxt( 250, 128, 114,      "salmon                    " ),
//                              tRGBtxt( 255, 160, 122,      "light salmon              " ),
//                              tRGBtxt( 255, 160, 122,      "LightSalmon               " ),
//                              tRGBtxt( 255, 165,   0,      "orange                    " ),
//                              tRGBtxt( 255, 140,   0,      "dark orange               " ),
//                              tRGBtxt( 255, 140,   0,      "DarkOrange                " ),
//                              tRGBtxt( 255, 127,  80,      "coral                     " ),
//                              tRGBtxt( 240, 128, 128,      "light coral               " ),
//                              tRGBtxt( 240, 128, 128,      "LightCoral                " ),
//                              tRGBtxt( 255,  99,  71,      "tomato                    " ),
//                              tRGBtxt( 255,  69,   0,      "orange red                " ),
//                              tRGBtxt( 255,  69,   0,      "OrangeRed                 " ),
//                              tRGBtxt( 255,   0,   0,      "red                       " ),
//                              tRGBtxt( 255, 105, 180,      "hot pink                  " ),
//                              tRGBtxt( 255, 105, 180,      "HotPink                   " ),
//                              tRGBtxt( 255,  20, 147,      "deep pink                 " ),
//                              tRGBtxt( 255,  20, 147,      "DeepPink                  " ),
//                              tRGBtxt( 255, 192, 203,      "pink                      " ),
//                              tRGBtxt( 255, 182, 193,      "light pink                " ),
//                              tRGBtxt( 255, 182, 193,      "LightPink                 " ),
//                              tRGBtxt( 219, 112, 147,      "pale violet red           " ),
//                              tRGBtxt( 219, 112, 147,      "PaleVioletRed             " ),
//                              tRGBtxt( 176,  48,  96,      "maroon                    " ),
//                              tRGBtxt( 199,  21, 133,      "medium violet red         " ),
//                              tRGBtxt( 199,  21, 133,      "MediumVioletRed           " ),
//                              tRGBtxt( 208,  32, 144,      "violet red                " ),
//                              tRGBtxt( 208,  32, 144,      "VioletRed                 " ),
//                              tRGBtxt( 255,   0, 255,      "magenta                   " ),
//                              tRGBtxt( 238, 130, 238,      "violet                    " ),
//                              tRGBtxt( 221, 160, 221,      "plum                      " ),
//                              tRGBtxt( 218, 112, 214,      "orchid                    " ),
//                              tRGBtxt( 186,  85, 211,      "medium orchid             " ),
//                              tRGBtxt( 186,  85, 211,      "MediumOrchid              " ),
//                              tRGBtxt( 153,  50, 204,      "dark orchid               " ),
//                              tRGBtxt( 153,  50, 204,      "DarkOrchid                " ),
//                              tRGBtxt( 148,   0, 211,      "dark violet               " ),
//                              tRGBtxt( 148,   0, 211,      "DarkViolet                " ),
//                              tRGBtxt( 138,  43, 226,      "blue violet               " ),
//                              tRGBtxt( 138,  43, 226,      "BlueViolet                " ),
//                              tRGBtxt( 160,  32, 240,      "purple                    " ),
//                              tRGBtxt( 147, 112, 219,      "medium purple             " ),
//                              tRGBtxt( 147, 112, 219,      "MediumPurple              " ),
//                              tRGBtxt( 216, 191, 216,      "thistle                   " ),
//                              tRGBtxt( 255, 250, 250,      "snow1                     " ),
//                              tRGBtxt( 238, 233, 233,      "snow2                     " ),
//                              tRGBtxt( 205, 201, 201,      "snow3                     " ),
//                              tRGBtxt( 139, 137, 137,      "snow4                     " ),
//                              tRGBtxt( 255, 245, 238,      "seashell1                 " ),
//                              tRGBtxt( 238, 229, 222,      "seashell2                 " ),
//                              tRGBtxt( 205, 197, 191,      "seashell3                 " ),
//                              tRGBtxt( 139, 134, 130,      "seashell4                 " ),
//                              tRGBtxt( 255, 239, 219,      "AntiqueWhite1             " ),
//                              tRGBtxt( 238, 223, 204,      "AntiqueWhite2             " ),
//                              tRGBtxt( 205, 192, 176,      "AntiqueWhite3             " ),
//                              tRGBtxt( 139, 131, 120,      "AntiqueWhite4             " ),
//                              tRGBtxt( 255, 228, 196,      "bisque1                   " ),
//                              tRGBtxt( 238, 213, 183,      "bisque2                   " ),
//                              tRGBtxt( 205, 183, 158,      "bisque3                   " ),
//                              tRGBtxt( 139, 125, 107,      "bisque4                   " ),
//                              tRGBtxt( 255, 218, 185,      "PeachPuff1                " ),
//                              tRGBtxt( 238, 203, 173,      "PeachPuff2                " ),
//                              tRGBtxt( 205, 175, 149,      "PeachPuff3                " ),
//                              tRGBtxt( 139, 119, 101,      "PeachPuff4                " ),
//                              tRGBtxt( 255, 222, 173,      "NavajoWhite1              " ),
//                              tRGBtxt( 238, 207, 161,      "NavajoWhite2              " ),
//                              tRGBtxt( 205, 179, 139,      "NavajoWhite3              " ),
//                              tRGBtxt( 139, 121,   94,     "NavajoWhite4              " ),
//                              tRGBtxt( 255, 250, 205,      "LemonChiffon1             " ),
//                              tRGBtxt( 238, 233, 191,      "LemonChiffon2             " ),
//                              tRGBtxt( 205, 201, 165,      "LemonChiffon3             " ),
//                              tRGBtxt( 139, 137, 112,      "LemonChiffon4             " ),
//                              tRGBtxt( 255, 248, 220,      "cornsilk1                 " ),
//                              tRGBtxt( 238, 232, 205,      "cornsilk2                 " ),
//                              tRGBtxt( 205, 200, 177,      "cornsilk3                 " ),
//                              tRGBtxt( 139, 136, 120,      "cornsilk4                 " ),
//                              tRGBtxt( 255, 255, 240,      "ivory1                    " ),
//                              tRGBtxt( 238, 238, 224,      "ivory2                    " ),
//                              tRGBtxt( 205, 205, 193,      "ivory3                    " ),
//                              tRGBtxt( 139, 139, 131,      "ivory4                    " ),
//                              tRGBtxt( 240, 255, 240,      "honeydew1                 " ),
//                              tRGBtxt( 224, 238, 224,      "honeydew2                 " ),
//                              tRGBtxt( 193, 205, 193,      "honeydew3                 " ),
//                              tRGBtxt( 131, 139, 131,      "honeydew4                 " ),
//                              tRGBtxt( 255, 240, 245,      "LavenderBlush1            " ),
//                              tRGBtxt( 238, 224, 229,      "LavenderBlush2            " ),
//                              tRGBtxt( 205, 193, 197,      "LavenderBlush3            " ),
//                              tRGBtxt( 139, 131, 134,      "LavenderBlush4            " ),
//                              tRGBtxt( 255, 228, 225,      "MistyRose1                " ),
//                              tRGBtxt( 238, 213, 210,      "MistyRose2                " ),
//                              tRGBtxt( 205, 183, 181,      "MistyRose3                " ),
//                              tRGBtxt( 139, 125, 123,      "MistyRose4                " ),
//                              tRGBtxt( 240, 255, 255,      "azure1                    " ),
//                              tRGBtxt( 224, 238, 238,      "azure2                    " ),
//                              tRGBtxt( 193, 205, 205,      "azure3                    " ),
//                              tRGBtxt( 131, 139, 139,      "azure4                    " ),
//                              tRGBtxt( 131, 111, 255,      "SlateBlue1                " ),
//                              tRGBtxt( 122, 103, 238,      "SlateBlue2                " ),
//                              tRGBtxt( 105,  89, 205,      "SlateBlue3                " ),
//                              tRGBtxt( 71,  60, 139,      "SlateBlue4                " ),
//                              tRGBtxt( 72, 118, 255,      "RoyalBlue1                " ),
//                              tRGBtxt( 67, 110, 238,      "RoyalBlue2                " ),
//                              tRGBtxt( 58,  95, 205,      "RoyalBlue3                " ),
//                              tRGBtxt( 39,  64, 139,      "RoyalBlue4                " ),
//                              tRGBtxt( 0,   0, 255,      "blue1                     " ),
//                              tRGBtxt( 0,   0, 238,      "blue2                     " ),
//                              tRGBtxt( 0,   0, 205,      "blue3                     " ),
//                              tRGBtxt( 0,   0, 139,      "blue4                     " ),
//                              tRGBtxt( 30, 144, 255,      "DodgerBlue1               " ),
//                              tRGBtxt( 28, 134, 238,      "DodgerBlue2               " ),
//                              tRGBtxt( 24, 116, 205,      "DodgerBlue3               " ),
//                              tRGBtxt( 16,  78, 139,      "DodgerBlue4               " ),
//                              tRGBtxt( 99, 184, 255,      "SteelBlue1                " ),
//                              tRGBtxt( 92, 172, 238,      "SteelBlue2                " ),
//                              tRGBtxt( 79, 148, 205,      "SteelBlue3                " ),
//                              tRGBtxt( 54, 100, 139,      "SteelBlue4                " ),
//                              tRGBtxt( 0, 191, 255,      "DeepSkyBlue1              " ),
//                              tRGBtxt( 0, 178, 238,      "DeepSkyBlue2              " ),
//                              tRGBtxt( 0, 154, 205,      "DeepSkyBlue3              " ),
//                              tRGBtxt( 0, 104, 139,      "DeepSkyBlue4              " ),
//                              tRGBtxt( 135, 206, 255,      "SkyBlue1                  " ),
//                              tRGBtxt( 126, 192, 238,      "SkyBlue2                  " ),
//                              tRGBtxt( 108, 166, 205,      "SkyBlue3                  " ),
//                              tRGBtxt( 74, 112, 139,      "SkyBlue4                  " ),
//                              tRGBtxt( 176, 226, 255,      "LightSkyBlue1             " ),
//                              tRGBtxt( 164, 211, 238,      "LightSkyBlue2             " ),
//                              tRGBtxt( 141, 182, 205,      "LightSkyBlue3             " ),
//                              tRGBtxt( 96, 123, 139,      "LightSkyBlue4             " ),
//                              tRGBtxt( 198, 226, 255,      "SlateGray1                " ),
//                              tRGBtxt( 185, 211, 238,      "SlateGray2                " ),
//                              tRGBtxt( 159, 182, 205,      "SlateGray3                " ),
//                              tRGBtxt( 108, 123, 139,      "SlateGray4                " ),
//                              tRGBtxt( 202, 225, 255,      "LightSteelBlue1           " ),
//                              tRGBtxt( 188, 210, 238,      "LightSteelBlue2           " ),
//                              tRGBtxt( 162, 181, 205,      "LightSteelBlue3           " ),
//                              tRGBtxt( 110, 123, 139,      "LightSteelBlue4           " ),
//                              tRGBtxt( 191, 239, 255,      "LightBlue1                " ),
//                              tRGBtxt( 178, 223, 238,      "LightBlue2                " ),
//                              tRGBtxt( 154, 192, 205,      "LightBlue3                " ),
//                              tRGBtxt( 104, 131, 139,      "LightBlue4                " ),
//                              tRGBtxt( 224, 255, 255,      "LightCyan1                " ),
//                              tRGBtxt( 209, 238, 238,      "LightCyan2                " ),
//                              tRGBtxt( 180, 205, 205,      "LightCyan3                " ),
//                              tRGBtxt( 122, 139, 139,      "LightCyan4                " ),
//                              tRGBtxt( 187, 255, 255,      "PaleTurquoise1            " ),
//                              tRGBtxt( 174, 238, 238,      "PaleTurquoise2            " ),
//                              tRGBtxt( 150, 205, 205,      "PaleTurquoise3            " ),
//                              tRGBtxt( 102, 139, 139,      "PaleTurquoise4            " ),
//                              tRGBtxt( 152, 245, 255,      "CadetBlue1                " ),
//                              tRGBtxt( 142, 229, 238,      "CadetBlue2                " ),
//                              tRGBtxt( 122, 197, 205,      "CadetBlue3                " ),
//                              tRGBtxt( 83, 134, 139,      "CadetBlue4                " ),
//                              tRGBtxt( 0, 245, 255,      "turquoise1                " ),
//                              tRGBtxt( 0, 229, 238,      "turquoise2                " ),
//                              tRGBtxt( 0, 197, 205,      "turquoise3                " ),
//                              tRGBtxt( 0, 134, 139,      "turquoise4                " ),
//                              tRGBtxt( 0, 255, 255,      "cyan1                     " ),
//                              tRGBtxt( 0, 238, 238,      "cyan2                     " ),
//                              tRGBtxt( 0, 205, 205,      "cyan3                     " ),
//                              tRGBtxt( 0, 139, 139,      "cyan4                     " ),
//                              tRGBtxt( 151, 255, 255,      "DarkSlateGray1            " ),
//                              tRGBtxt( 141, 238, 238,      "DarkSlateGray2            " ),
//                              tRGBtxt( 121, 205, 205,      "DarkSlateGray3            " ),
//                              tRGBtxt( 82, 139, 139,      "DarkSlateGray4            " ),
//                              tRGBtxt( 127, 255, 212,      "aquamarine1               " ),
//                              tRGBtxt( 118, 238, 198,      "aquamarine2               " ),
//                              tRGBtxt( 102, 205, 170,      "aquamarine3               " ),
//                              tRGBtxt( 69, 139, 116,      "aquamarine4               " ),
//                              tRGBtxt( 193, 255, 193,      "DarkSeaGreen1             " ),
//                              tRGBtxt( 180, 238, 180,      "DarkSeaGreen2             " ),
//                              tRGBtxt( 155, 205, 155,      "DarkSeaGreen3             " ),
//                              tRGBtxt( 105, 139, 105,      "DarkSeaGreen4             " ),
//                              tRGBtxt( 84, 255, 159,      "SeaGreen1                 " ),
//                              tRGBtxt( 78, 238, 148,      "SeaGreen2                 " ),
//                              tRGBtxt( 67, 205, 128,      "SeaGreen3                 " ),
//                              tRGBtxt( 46, 139,   87,     "SeaGreen4                 " ),
//                              tRGBtxt( 154, 255, 154,      "PaleGreen1                " ),
//                              tRGBtxt( 144, 238, 144,      "PaleGreen2                " ),
//                              tRGBtxt( 124, 205, 124,      "PaleGreen3                " ),
//                              tRGBtxt( 84, 139,   84,     "PaleGreen4                " ),
//                              tRGBtxt( 0, 255, 127,      "SpringGreen1              " ),
//                              tRGBtxt( 0, 238, 118,      "SpringGreen2              " ),
//                              tRGBtxt( 0, 205, 102,      "SpringGreen3              " ),
//                              tRGBtxt( 0, 139,    69,     "SpringGreen4              " ),
//                              tRGBtxt( 0, 255,     0,     "green1                    " ),
//                              tRGBtxt( 0, 238,     0,     "green2                    " ),
//                              tRGBtxt( 0, 205,     0,     "green3                    " ),
//                              tRGBtxt( 0, 139,     0,     "green4                    " ),
//                              tRGBtxt( 127, 255,    0,     "chartreuse1               " ),
//                              tRGBtxt( 118, 238,    0,     "chartreuse2               " ),
//                              tRGBtxt( 102, 205,    0,     "chartreuse3               " ),
//                              tRGBtxt( 69, 139,    0,     "chartreuse4               " ),
//                              tRGBtxt( 192, 255,   62,     "OliveDrab1                " ),
//                              tRGBtxt( 179, 238,   58,     "OliveDrab2                " ),
//                              tRGBtxt( 154, 205,   50,     "OliveDrab3                " ),
//                              tRGBtxt( 105, 139,   34,     "OliveDrab4                " ),
//                              tRGBtxt( 202, 255, 112,      "DarkOliveGreen1           " ),
//                              tRGBtxt( 188, 238, 104,      "DarkOliveGreen2           " ),
//                              tRGBtxt( 162, 205,   90,     "DarkOliveGreen3           " ),
//                              tRGBtxt( 110, 139,   61,     "DarkOliveGreen4           " ),
//                              tRGBtxt( 255, 246, 143,      "khaki1                    " ),
//                              tRGBtxt( 238, 230, 133,      "khaki2                    " ),
//                              tRGBtxt( 205, 198, 115,      "khaki3                    " ),
//                              tRGBtxt( 139, 134,   78,     "khaki4                    " ),
//                              tRGBtxt( 255, 236, 139,      "LightGoldenrod1           " ),
//                              tRGBtxt( 238, 220, 130,      "LightGoldenrod2           " ),
//                              tRGBtxt( 205, 190, 112,      "LightGoldenrod3           " ),
//                              tRGBtxt( 139, 129,   76,     "LightGoldenrod4           " ),
//                              tRGBtxt( 255, 255, 224,      "LightYellow1              " ),
//                              tRGBtxt( 238, 238, 209,      "LightYellow2              " ),
//                              tRGBtxt( 205, 205, 180,      "LightYellow3              " ),
//                              tRGBtxt( 139, 139, 122,      "LightYellow4              " ),
//                              tRGBtxt( 255, 255,    0,     "yellow1                   " ),
//                              tRGBtxt( 238, 238,    0,     "yellow2                   " ),
//                              tRGBtxt( 205, 205,    0,     "yellow3                   " ),
//                              tRGBtxt( 139, 139,    0,     "yellow4                   " ),
//                              tRGBtxt( 255, 215,    0,     "gold1                     " ),
//                              tRGBtxt( 238, 201,    0,     "gold2                     " ),
//                              tRGBtxt( 205, 173,    0,     "gold3                     " ),
//                              tRGBtxt( 139, 117,    0,     "gold4                     " ),
//                              tRGBtxt( 255, 193,   37,     "goldenrod1                " ),
//                              tRGBtxt( 238, 180,   34,     "goldenrod2                " ),
//                              tRGBtxt( 205, 155,   29,     "goldenrod3                " ),
//                              tRGBtxt( 139, 105,   20,     "goldenrod4                " ),
//                              tRGBtxt( 255, 185,   15,     "DarkGoldenrod1            " ),
//                              tRGBtxt( 238, 173,   14,     "DarkGoldenrod2            " ),
//                              tRGBtxt( 205, 149,   12,     "DarkGoldenrod3            " ),
//                              tRGBtxt( 139, 101,    8,     "DarkGoldenrod4            " ),
//                              tRGBtxt( 255, 193, 193,      "RosyBrown1                " ),
//                              tRGBtxt( 238, 180, 180,      "RosyBrown2                " ),
//                              tRGBtxt( 205, 155, 155,      "RosyBrown3                " ),
//                              tRGBtxt( 139, 105, 105,      "RosyBrown4                " ),
//                              tRGBtxt( 255, 106, 106,      "IndianRed1                " ),
//                              tRGBtxt( 238,  99,   99,     "IndianRed2                " ),
//                              tRGBtxt( 205,  85,   85,     "IndianRed3                " ),
//                              tRGBtxt( 139,  58,   58,     "IndianRed4                " ),
//                              tRGBtxt( 255, 130,   71,     "sienna1                   " ),
//                              tRGBtxt( 238, 121,   66,     "sienna2                   " ),
//                              tRGBtxt( 205, 104,   57,     "sienna3                   " ),
//                              tRGBtxt( 139,  71,   38,     "sienna4                   " ),
//                              tRGBtxt( 255, 211, 155,      "burlywood1                " ),
//                              tRGBtxt( 238, 197, 145,      "burlywood2                " ),
//                              tRGBtxt( 205, 170, 125,      "burlywood3                " ),
//                              tRGBtxt( 139, 115,   85,     "burlywood4                " ),
//                              tRGBtxt( 255, 231, 186,      "wheat1                    " ),
//                              tRGBtxt( 238, 216, 174,      "wheat2                    " ),
//                              tRGBtxt( 205, 186, 150,      "wheat3                    " ),
//                              tRGBtxt( 139, 126, 102,      "wheat4                    " ),
//                              tRGBtxt( 255, 165,   79,     "tan1                      " ),
//                              tRGBtxt( 238, 154,   73,     "tan2                      " ),
//                              tRGBtxt( 205, 133,   63,     "tan3                      " ),
//                              tRGBtxt( 139,  90,   43,     "tan4                      " ),
//                              tRGBtxt( 255, 127,   36,     "chocolate1                " ),
//                              tRGBtxt( 238, 118,   33,     "chocolate2                " ),
//                              tRGBtxt( 205, 102,   29,     "chocolate3                " ),
//                              tRGBtxt( 139,  69,   19,     "chocolate4                " ),
//                              tRGBtxt( 255,  48,   48,     "firebrick1                " ),
//                              tRGBtxt( 238,  44,   44,     "firebrick2                " ),
//                              tRGBtxt( 205,  38,   38,     "firebrick3                " ),
//                              tRGBtxt( 139,  26,   26,     "firebrick4                " ),
//                              tRGBtxt( 255,  64,   64,     "brown1                    " ),
//                              tRGBtxt( 238,  59,   59,     "brown2                    " ),
//                              tRGBtxt( 205,  51,   51,     "brown3                    " ),
//                              tRGBtxt( 139,  35,   35,     "brown4                    " ),
//                              tRGBtxt( 255, 140, 105,      "salmon1                   " ),
//                              tRGBtxt( 238, 130,   98,     "salmon2                   " ),
//                              tRGBtxt( 205, 112,   84,     "salmon3                   " ),
//                              tRGBtxt( 139,  76,   57,     "salmon4                   " ),
//                              tRGBtxt( 255, 160, 122,      "LightSalmon1              " ),
//                              tRGBtxt( 238, 149, 114,      "LightSalmon2              " ),
//                              tRGBtxt( 205, 129,   98,     "LightSalmon3              " ),
//                              tRGBtxt( 139,  87,   66,     "LightSalmon4              " ),
//                              tRGBtxt( 255, 165,    0,     "orange1                   " ),
//                              tRGBtxt( 238, 154,    0,     "orange2                   " ),
//                              tRGBtxt( 205, 133,    0,     "orange3                   " ),
//                              tRGBtxt( 139,  90,    0,     "orange4                   " ),
//                              tRGBtxt( 255, 127,    0,     "DarkOrange1               " ),
//                              tRGBtxt( 238, 118,    0,     "DarkOrange2               " ),
//                              tRGBtxt( 205, 102,    0,     "DarkOrange3               " ),
//                              tRGBtxt( 139,  69,    0,     "DarkOrange4               " ),
//                              tRGBtxt( 255, 114,   86,     "coral1                    " ),
//                              tRGBtxt( 238, 106,   80,     "coral2                    " ),
//                              tRGBtxt( 205,  91,   69,     "coral3                    " ),
//                              tRGBtxt( 139,  62,   47,     "coral4                    " ),
//                              tRGBtxt( 255,  99,   71,     "tomato1                   " ),
//                              tRGBtxt( 238,  92,   66,     "tomato2                   " ),
//                              tRGBtxt( 205,  79,   57,     "tomato3                   " ),
//                              tRGBtxt( 139,  54,   38,     "tomato4                   " ),
//                              tRGBtxt( 255,  69,    0,     "OrangeRed1                " ),
//                              tRGBtxt( 238,  64,    0,     "OrangeRed2                " ),
//                              tRGBtxt( 205,  55,    0,     "OrangeRed3                " ),
//                              tRGBtxt( 139,  37,    0,     "OrangeRed4                " ),
//                              tRGBtxt( 255,   0,    0,     "red1                      " ),
//                              tRGBtxt( 238,   0,    0,     "red2                      " ),
//                              tRGBtxt( 205,   0,    0,     "red3                      " ),
//                              tRGBtxt( 139,   0,    0,     "red4                      " ),
//                              tRGBtxt( 215,   7,  81,      "DebianRed                 " ),
//                              tRGBtxt( 255,  20, 147,      "DeepPink1                 " ),
//                              tRGBtxt( 238,  18, 137,      "DeepPink2                 " ),
//                              tRGBtxt( 205,  16, 118,      "DeepPink3                 " ),
//                              tRGBtxt( 139,  10,   80,     "DeepPink4                 " ),
//                              tRGBtxt( 255, 110, 180,      "HotPink1                  " ),
//                              tRGBtxt( 238, 106, 167,      "HotPink2                  " ),
//                              tRGBtxt( 205,  96, 144,      "HotPink3                  " ),
//                              tRGBtxt( 139,  58,  98,      "HotPink4                  " ),
//                              tRGBtxt( 255, 181, 197,      "pink1                     " ),
//                              tRGBtxt( 238, 169, 184,      "pink2                     " ),
//                              tRGBtxt( 205, 145, 158,      "pink3                     " ),
//                              tRGBtxt( 139,  99, 108,      "pink4                     " ),
//                              tRGBtxt( 255, 174, 185,      "LightPink1                " ),
//                              tRGBtxt( 238, 162, 173,      "LightPink2                " ),
//                              tRGBtxt( 205, 140, 149,      "LightPink3                " ),
//                              tRGBtxt( 139,  95, 101,      "LightPink4                " ),
//                              tRGBtxt( 255, 130, 171,      "PaleVioletRed1            " ),
//                              tRGBtxt( 238, 121, 159,      "PaleVioletRed2            " ),
//                              tRGBtxt( 205, 104, 137,      "PaleVioletRed3            " ),
//                              tRGBtxt( 139,  71,   93,     "PaleVioletRed4            " ),
//                              tRGBtxt( 255,  52, 179,      "maroon1                   " ),
//                              tRGBtxt( 238,  48, 167,      "maroon2                   " ),
//                              tRGBtxt( 205,  41, 144,      "maroon3                   " ),
//                              tRGBtxt( 139,  28,   98,     "maroon4                   " ),
//                              tRGBtxt( 255,  62, 150,      "VioletRed1                " ),
//                              tRGBtxt( 238,  58, 140,      "VioletRed2                " ),
//                              tRGBtxt( 205,  50, 120,      "VioletRed3                " ),
//                              tRGBtxt( 139,  34,   82,     "VioletRed4                " ),
//                              tRGBtxt( 255,   0, 255,      "magenta1                  " ),
//                              tRGBtxt( 238,   0, 238,      "magenta2                  " ),
//                              tRGBtxt( 205,   0, 205,      "magenta3                  " ),
//                              tRGBtxt( 139,   0, 139,      "magenta4                  " ),
//                              tRGBtxt( 255, 131, 250,      "orchid1                   " ),
//                              tRGBtxt( 238, 122, 233,      "orchid2                   " ),
//                              tRGBtxt( 205, 105, 201,      "orchid3                   " ),
//                              tRGBtxt( 139,  71, 137,      "orchid4                   " ),
//                              tRGBtxt( 255, 187, 255,      "plum1                     " ),
//                              tRGBtxt( 238, 174, 238,      "plum2                     " ),
//                              tRGBtxt( 205, 150, 205,      "plum3                     " ),
//                              tRGBtxt( 139, 102, 139,      "plum4                     " ),
//                              tRGBtxt( 224, 102, 255,      "MediumOrchid1             " ),
//                              tRGBtxt( 209,  95, 238,      "MediumOrchid2             " ),
//                              tRGBtxt( 180,  82, 205,      "MediumOrchid3             " ),
//                              tRGBtxt( 122,  55, 139,      "MediumOrchid4             " ),
//                              tRGBtxt( 191,  62, 255,      "DarkOrchid1               " ),
//                              tRGBtxt( 178,  58, 238,      "DarkOrchid2               " ),
//                              tRGBtxt( 154,  50, 205,      "DarkOrchid3               " ),
//                              tRGBtxt( 104,  34, 139,      "DarkOrchid4               " ),
//                              tRGBtxt( 155,  48, 255,      "purple1                   " ),
//                              tRGBtxt( 145,  44, 238,      "purple2                   " ),
//                              tRGBtxt( 125,  38, 205,      "purple3                   " ),
//                              tRGBtxt( 85,  26, 139,      "purple4                   " ),
//                              tRGBtxt( 171, 130, 255,      "MediumPurple1             " ),
//                              tRGBtxt( 159, 121, 238,      "MediumPurple2             " ),
//                              tRGBtxt( 137, 104, 205,      "MediumPurple3             " ),
//                              tRGBtxt( 93,  71, 139,      "MediumPurple4             " ),
//                              tRGBtxt( 255, 225, 255,      "thistle1                  " ),
//                              tRGBtxt( 238, 210, 238,      "thistle2                  " ),
//                              tRGBtxt( 205, 181, 205,      "thistle3                  " ),
//                              tRGBtxt( 139, 123, 139,      "thistle4                  " ),
//                              tRGBtxt( 0,   0,   0,      "gray0                     " ),
//                              tRGBtxt( 0,   0,   0,      "grey0                     " ),
//                              tRGBtxt( 3,   3,   3,      "gray1                     " ),
//                              tRGBtxt( 3,   3,   3,      "grey1                     " ),
//                              tRGBtxt( 5,   5,   5,      "gray2                     " ),
//                              tRGBtxt( 5,   5,   5,      "grey2                     " ),
//                              tRGBtxt( 8,   8,   8,      "gray3                     " ),
//                              tRGBtxt( 8,   8,   8,      "grey3                     " ),
//                              tRGBtxt( 10,  10,  10,      "gray4                     " ),
//                              tRGBtxt( 10,  10,  10,      "grey4                     " ),
//                              tRGBtxt( 13,  13,  13,      "gray5                     " ),
//                              tRGBtxt( 13,  13,  13,      "grey5                     " ),
//                              tRGBtxt( 15,  15,  15,      "gray6                     " ),
//                              tRGBtxt( 15,  15,  15,      "grey6                     " ),
//                              tRGBtxt( 18,  18,  18,      "gray7                     " ),
//                              tRGBtxt( 18,  18,  18,      "grey7                     " ),
//                              tRGBtxt( 20,  20,  20,      "gray8                     " ),
//                              tRGBtxt( 20,  20,  20,      "grey8                     " ),
//                              tRGBtxt( 23,  23,  23,      "gray9                     " ),
//                              tRGBtxt( 23,  23,  23,      "grey9                     " ),
//                              tRGBtxt( 26,  26,  26,      "gray10                    " ),
//                              tRGBtxt( 26,  26,  26,      "grey10                    " ),
//                              tRGBtxt( 28,  28,  28,      "gray11                    " ),
//                              tRGBtxt( 28,  28,  28,      "grey11                    " ),
//                              tRGBtxt( 31,  31,  31,      "gray12                    " ),
//                              tRGBtxt( 31,  31,  31,      "grey12                    " ),
//                              tRGBtxt( 33,  33,  33,      "gray13                    " ),
//                              tRGBtxt( 33,  33,  33,      "grey13                    " ),
//                              tRGBtxt( 36,  36,  36,      "gray14                    " ),
//                              tRGBtxt( 36,  36,  36,      "grey14                    " ),
//                              tRGBtxt( 38,  38,  38,      "gray15                    " ),
//                              tRGBtxt( 38,  38,  38,      "grey15                    " ),
//                              tRGBtxt( 41,  41,  41,      "gray16                    " ),
//                              tRGBtxt( 41,  41,  41,      "grey16                    " ),
//                              tRGBtxt( 43,  43,  43,      "gray17                    " ),
//                              tRGBtxt( 43,  43,  43,      "grey17                    " ),
//                              tRGBtxt( 46,  46,  46,      "gray18                    " ),
//                              tRGBtxt( 46,  46,  46,      "grey18                    " ),
//                              tRGBtxt( 48,  48,  48,      "gray19                    " ),
//                              tRGBtxt( 48,  48,  48,      "grey19                    " ),
//                              tRGBtxt( 51,  51,  51,      "gray20                    " ),
//                              tRGBtxt( 51,  51,  51,      "grey20                    " ),
//                              tRGBtxt( 54,  54,  54,      "gray21                    " ),
//                              tRGBtxt( 54,  54,  54,      "grey21                    " ),
//                              tRGBtxt( 56,  56,  56,      "gray22                    " ),
//                              tRGBtxt( 56,  56,  56,      "grey22                    " ),
//                              tRGBtxt( 59,  59,  59,      "gray23                    " ),
//                              tRGBtxt( 59,  59,  59,      "grey23                    " ),
//                              tRGBtxt( 61,  61,  61,      "gray24                    " ),
//                              tRGBtxt( 61,  61,  61,      "grey24                    " ),
//                              tRGBtxt( 64,  64,  64,      "gray25                    " ),
//                              tRGBtxt( 64,  64,  64,      "grey25                    " ),
//                              tRGBtxt( 66,  66,  66,      "gray26                    " ),
//                              tRGBtxt( 66,  66,  66,      "grey26                    " ),
//                              tRGBtxt( 69,  69,  69,      "gray27                    " ),
//                              tRGBtxt( 69,  69,  69,      "grey27                    " ),
//                              tRGBtxt( 71,  71,  71,      "gray28                    " ),
//                              tRGBtxt( 71,  71,  71,      "grey28                    " ),
//                              tRGBtxt( 74,  74,  74,      "gray29                    " ),
//                              tRGBtxt( 74,  74,  74,      "grey29                    " ),
//                              tRGBtxt( 77,  77,  77,      "gray30                    " ),
//                              tRGBtxt( 77,  77,  77,      "grey30                    " ),
//                              tRGBtxt( 79,  79,  79,      "gray31                    " ),
//                              tRGBtxt( 79,  79,  79,      "grey31                    " ),
//                              tRGBtxt( 82,  82,  82,      "gray32                    " ),
//                              tRGBtxt( 82,  82,  82,      "grey32                    " ),
//                              tRGBtxt( 84,  84,  84,      "gray33                    " ),
//                              tRGBtxt( 84,  84,  84,      "grey33                    " ),
//                              tRGBtxt( 87,  87,  87,      "gray34                    " ),
//                              tRGBtxt( 87,  87,  87,      "grey34                    " ),
//                              tRGBtxt( 89,  89,  89,      "gray35                    " ),
//                              tRGBtxt( 89,  89,  89,      "grey35                    " ),
//                              tRGBtxt( 92,  92,  92,      "gray36                    " ),
//                              tRGBtxt( 92,  92,  92,      "grey36                    " ),
//                              tRGBtxt( 94,  94,  94,      "gray37                    " ),
//                              tRGBtxt( 94,  94,  94,      "grey37                    " ),
//                              tRGBtxt( 97,  97,  97,      "gray38                    " ),
//                              tRGBtxt( 97,  97,  97,      "grey38                    " ),
//                              tRGBtxt( 99,  99,  99,      "gray39                    " ),
//                              tRGBtxt( 99,  99,  99,      "grey39                    " ),
//                              tRGBtxt( 102, 102, 102,      "gray40                    " ),
//                              tRGBtxt( 102, 102, 102,      "grey40                    " ),
//                              tRGBtxt( 105, 105, 105,      "gray41                    " ),
//                              tRGBtxt( 105, 105, 105,      "grey41                    " ),
//                              tRGBtxt( 107, 107, 107,      "gray42                    " ),
//                              tRGBtxt( 107, 107, 107,      "grey42                    " ),
//                              tRGBtxt( 110, 110, 110,      "gray43                    " ),
//                              tRGBtxt( 110, 110, 110,      "grey43                    " ),
//                              tRGBtxt( 112, 112, 112,      "gray44                    " ),
//                              tRGBtxt( 112, 112, 112,      "grey44                    " ),
//                              tRGBtxt( 115, 115, 115,      "gray45                    " ),
//                              tRGBtxt( 115, 115, 115,      "grey45                    " ),
//                              tRGBtxt( 117, 117, 117,      "gray46                    " ),
//                              tRGBtxt( 117, 117, 117,      "grey46                    " ),
//                              tRGBtxt( 120, 120, 120,      "gray47                    " ),
//                              tRGBtxt( 120, 120, 120,      "grey47                    " ),
//                              tRGBtxt( 122, 122, 122,      "gray48                    " ),
//                              tRGBtxt( 122, 122, 122,      "grey48                    " ),
//                              tRGBtxt( 125, 125, 125,      "gray49                    " ),
//                              tRGBtxt( 125, 125, 125,      "grey49                    " ),
//                              tRGBtxt( 127, 127, 127,      "gray50                    " ),
//                              tRGBtxt( 127, 127, 127,      "grey50                    " ),
//                              tRGBtxt( 130, 130, 130,      "gray51                    " ),
//                              tRGBtxt( 130, 130, 130,      "grey51                    " ),
//                              tRGBtxt( 133, 133, 133,      "gray52                    " ),
//                              tRGBtxt( 133, 133, 133,      "grey52                    " ),
//                              tRGBtxt( 135, 135, 135,      "gray53                    " ),
//                              tRGBtxt( 135, 135, 135,      "grey53                    " ),
//                              tRGBtxt( 138, 138, 138,      "gray54                    " ),
//                              tRGBtxt( 138, 138, 138,      "grey54                    " ),
//                              tRGBtxt( 140, 140, 140,      "gray55                    " ),
//                              tRGBtxt( 140, 140, 140,      "grey55                    " ),
//                              tRGBtxt( 143, 143, 143,      "gray56                    " ),
//                              tRGBtxt( 143, 143, 143,      "grey56                    " ),
//                              tRGBtxt( 145, 145, 145,      "gray57                    " ),
//                              tRGBtxt( 145, 145, 145,      "grey57                    " ),
//                              tRGBtxt( 148, 148, 148,      "gray58                    " ),
//                              tRGBtxt( 148, 148, 148,      "grey58                    " ),
//                              tRGBtxt( 150, 150, 150,      "gray59                    " ),
//                              tRGBtxt( 150, 150, 150,      "grey59                    " ),
//                              tRGBtxt( 153, 153, 153,      "gray60                    " ),
//                              tRGBtxt( 153, 153, 153,      "grey60                    " ),
//                              tRGBtxt( 156, 156, 156,      "gray61                    " ),
//                              tRGBtxt( 156, 156, 156,      "grey61                    " ),
//                              tRGBtxt( 158, 158, 158,      "gray62                    " ),
//                              tRGBtxt( 158, 158, 158,      "grey62                    " ),
//                              tRGBtxt( 161, 161, 161,      "gray63                    " ),
//                              tRGBtxt( 161, 161, 161,      "grey63                    " ),
//                              tRGBtxt( 163, 163, 163,      "gray64                    " ),
//                              tRGBtxt( 163, 163, 163,      "grey64                    " ),
//                              tRGBtxt( 166, 166, 166,      "gray65                    " ),
//                              tRGBtxt( 166, 166, 166,      "grey65                    " ),
//                              tRGBtxt( 168, 168, 168,      "gray66                    " ),
//                              tRGBtxt( 168, 168, 168,      "grey66                    " ),
//                              tRGBtxt( 171, 171, 171,      "gray67                    " ),
//                              tRGBtxt( 171, 171, 171,      "grey67                    " ),
//                              tRGBtxt( 173, 173, 173,      "gray68                    " ),
//                              tRGBtxt( 173, 173, 173,      "grey68                    " ),
//                              tRGBtxt( 176, 176, 176,      "gray69                    " ),
//                              tRGBtxt( 176, 176, 176,      "grey69                    " ),
//                              tRGBtxt( 179, 179, 179,      "gray70                    " ),
//                              tRGBtxt( 179, 179, 179,      "grey70                    " ),
//                              tRGBtxt( 181, 181, 181,      "gray71                    " ),
//                              tRGBtxt( 181, 181, 181,      "grey71                    " ),
//                              tRGBtxt( 184, 184, 184,      "gray72                    " ),
//                              tRGBtxt( 184, 184, 184,      "grey72                    " ),
//                              tRGBtxt( 186, 186, 186,      "gray73                    " ),
//                              tRGBtxt( 186, 186, 186,      "grey73                    " ),
//                              tRGBtxt( 189, 189, 189,      "gray74                    " ),
//                              tRGBtxt( 189, 189, 189,      "grey74                    " ),
//                              tRGBtxt( 191, 191, 191,      "gray75                    " ),
//                              tRGBtxt( 191, 191, 191,      "grey75                    " ),
//                              tRGBtxt( 194, 194, 194,      "gray76                    " ),
//                              tRGBtxt( 194, 194, 194,      "grey76                    " ),
//                              tRGBtxt( 196, 196, 196,      "gray77                    " ),
//                              tRGBtxt( 196, 196, 196,      "grey77                    " ),
//                              tRGBtxt( 199, 199, 199,      "gray78                    " ),
//                              tRGBtxt( 199, 199, 199,      "grey78                    " ),
//                              tRGBtxt( 201, 201, 201,      "gray79                    " ),
//                              tRGBtxt( 201, 201, 201,      "grey79                    " ),
//                              tRGBtxt( 204, 204, 204,      "gray80                    " ),
//                              tRGBtxt( 204, 204, 204,      "grey80                    " ),
//                              tRGBtxt( 207, 207, 207,      "gray81                    " ),
//                              tRGBtxt( 207, 207, 207,      "grey81                    " ),
//                              tRGBtxt( 209, 209, 209,      "gray82                    " ),
//                              tRGBtxt( 209, 209, 209,      "grey82                    " ),
//                              tRGBtxt( 212, 212, 212,      "gray83                    " ),
//                              tRGBtxt( 212, 212, 212,      "grey83                    " ),
//                              tRGBtxt( 214, 214, 214,      "gray84                    " ),
//                              tRGBtxt( 214, 214, 214,      "grey84                    " ),
//                              tRGBtxt( 217, 217, 217,      "gray85                    " ),
//                              tRGBtxt( 217, 217, 217,      "grey85                    " ),
//                              tRGBtxt( 219, 219, 219,      "gray86                    " ),
//                              tRGBtxt( 219, 219, 219,      "grey86                    " ),
//                              tRGBtxt( 222, 222, 222,      "gray87                    " ),
//                              tRGBtxt( 222, 222, 222,      "grey87                    " ),
//                              tRGBtxt( 224, 224, 224,      "gray88                    " ),
//                              tRGBtxt( 224, 224, 224,      "grey88                    " ),
//                              tRGBtxt( 227, 227, 227,      "gray89                    " ),
//                              tRGBtxt( 227, 227, 227,      "grey89                    " ),
//                              tRGBtxt( 229, 229, 229,      "gray90                    " ),
//                              tRGBtxt( 229, 229, 229,      "grey90                    " ),
//                              tRGBtxt( 232, 232, 232,      "gray91                    " ),
//                              tRGBtxt( 232, 232, 232,      "grey91                    " ),
//                              tRGBtxt( 235, 235, 235,      "gray92                    " ),
//                              tRGBtxt( 235, 235, 235,      "grey92                    " ),
//                              tRGBtxt( 237, 237, 237,      "gray93                    " ),
//                              tRGBtxt( 237, 237, 237,      "grey93                    " ),
//                              tRGBtxt( 240, 240, 240,      "gray94                    " ),
//                              tRGBtxt( 240, 240, 240,      "grey94                    " ),
//                              tRGBtxt( 242, 242, 242,      "gray95                    " ),
//                              tRGBtxt( 242, 242, 242,      "grey95                    " ),
//                              tRGBtxt( 245, 245, 245,      "gray96                    " ),
//                              tRGBtxt( 245, 245, 245,      "grey96                    " ),
//                              tRGBtxt( 247, 247, 247,      "gray97                    " ),
//                              tRGBtxt( 247, 247, 247,      "grey97                    " ),
//                              tRGBtxt( 250, 250, 250,      "gray98                    " ),
//                              tRGBtxt( 250, 250, 250,      "grey98                    " ),
//                              tRGBtxt( 252, 252, 252,      "gray99                    " ),
//                              tRGBtxt( 252, 252, 252,      "grey99                    " ),
//                              tRGBtxt( 255, 255, 255,      "gray100                   " ),
//                              tRGBtxt( 255, 255, 255,      "grey100                   " ),
//                              tRGBtxt( 169, 169, 169,      "dark grey                 " ),
//                              tRGBtxt( 169, 169, 169,      "DarkGrey                  " ),
//                              tRGBtxt( 169, 169, 169,      "dark gray                 " ),
//                              tRGBtxt( 169, 169, 169,      "DarkGray                  " ),
//                              tRGBtxt( 0  ,   0, 139,      "dark blue                 " ),
//                              tRGBtxt( 0  ,   0, 139,      "DarkBlue                  " ),
//                              tRGBtxt( 0  , 139, 139,      "dark cyan                 " ),
//                              tRGBtxt( 0  , 139, 139,      "DarkCyan                  " ),
//                              tRGBtxt( 139,   0, 139,      "dark magenta              " ),
//                              tRGBtxt( 139,   0, 139,      "DarkMagenta               " ),
//                              tRGBtxt( 139,   0,   0,      "dark red                  " ),
//                              tRGBtxt( 139,   0,   0,      "DarkRed                   " ),
//                              tRGBtxt( 144, 238, 144,      "light green               " ),
//                              tRGBtxt( 144, 238, 144,      "LightGreen                " )
//                            };
//   vector<tRGBtxt> tRGBtxt::rgb_txt = makeVector( values );

//   vector<tRGBtxt> rgb_txt_ = boost::assign::list_of

//                              ( tRGBtxt( 255, 250, 250,      "snow                      " ) )
//                              ( tRGBtxt( 248, 248, 255,      "ghost white               " ) );
//#ifdef USE_RGB_COLORNAMES
//   ( tRGBtxt( 248, 248, 255,      "GhostWhite                " ) )
//   ( tRGBtxt( 245, 245, 245,      "white smoke               " ) )
//   ( tRGBtxt( 245, 245, 245,      "WhiteSmoke                " ) )
//   ( tRGBtxt( 220, 220, 220,      "gainsboro                 " ) )
//   ( tRGBtxt( 255, 250, 240,      "floral white              " ) )
//   ( tRGBtxt( 255, 250, 240,      "FloralWhite               " ) )
//   ( tRGBtxt( 253, 245, 230,      "old lace                  " ) )
//   ( tRGBtxt( 253, 245, 230,      "OldLace                   " ) )
//   ( tRGBtxt( 250, 240, 230,      "linen                     " ) )
//   ( tRGBtxt( 250, 235, 215,      "antique white             " ) )
//   ( tRGBtxt( 250, 235, 215,      "AntiqueWhite              " ) )
//   ( tRGBtxt( 255, 239, 213,      "papaya whip               " ) )
//   ( tRGBtxt( 255, 239, 213,      "PapayaWhip                " ) )
//   ( tRGBtxt( 255, 235, 205,      "blanched almond           " ) )
//   ( tRGBtxt( 255, 235, 205,      "BlanchedAlmond            " ) )
//   ( tRGBtxt( 255, 228, 196,      "bisque                    " ) )
//   ( tRGBtxt( 255, 218, 185,      "peach puff                " ) )
//   ( tRGBtxt( 255, 218, 185,      "PeachPuff                 " ) )
//   ( tRGBtxt( 255, 222, 173,      "navajo white              " ) )
//   ( tRGBtxt( 255, 222, 173,      "NavajoWhite               " ) )
//   ( tRGBtxt( 255, 228, 181,      "moccasin                  " ) )
//   ( tRGBtxt( 255, 248, 220,      "cornsilk                  " ) )
//   ( tRGBtxt( 255, 255, 240,      "ivory                     " ) )
//   ( tRGBtxt( 255, 250, 205,      "lemon chiffon             " ) )
//   ( tRGBtxt( 255, 250, 205,      "LemonChiffon              " ) )
//   ( tRGBtxt( 255, 245, 238,      "seashell                  " ) )
//   ( tRGBtxt( 240, 255, 240,      "honeydew                  " ) )
//   ( tRGBtxt( 245, 255, 250,      "mint cream                " ) )
//   ( tRGBtxt( 245, 255, 250,      "MintCream                 " ) )
//   ( tRGBtxt( 240, 255, 255,      "azure                     " ) )
//   ( tRGBtxt( 240, 248, 255,      "alice blue                " ) )
//   ( tRGBtxt( 240, 248, 255,      "AliceBlue                 " ) )
//   ( tRGBtxt( 230, 230, 250,      "lavender                  " ) )
//   ( tRGBtxt( 255, 240, 245,      "lavender blush            " ) )
//   ( tRGBtxt( 255, 240, 245,      "LavenderBlush             " ) )
//   ( tRGBtxt( 255, 228, 225,      "misty rose                " ) )
//   ( tRGBtxt( 255, 228, 225,      "MistyRose                 " ) )
//   ( tRGBtxt( 255, 255, 255,      "white                     " ) )
//   ( tRGBtxt( 0,   0,   0,      "black                     " ) )
//   ( tRGBtxt( 47,  79,  79,      "dark slate gray           " ) )
//   ( tRGBtxt( 47,  79,  79,      "DarkSlateGray             " ) )
//   ( tRGBtxt( 47,  79,  79,      "dark slate grey           " ) )
//   ( tRGBtxt( 47,  79,  79,      "DarkSlateGrey             " ) )
//   ( tRGBtxt( 105, 105, 105,      "dim gray                  " ) )
//   ( tRGBtxt( 105, 105, 105,      "DimGray                   " ) )
//   ( tRGBtxt( 105, 105, 105,      "dim grey                  " ) )
//   ( tRGBtxt( 105, 105, 105,      "DimGrey                   " ) )
//   ( tRGBtxt( 112, 128, 144,      "slate gray                " ) )
//   ( tRGBtxt( 112, 128, 144,      "SlateGray                 " ) )
//   ( tRGBtxt( 112, 128, 144,      "slate grey                " ) )
//   ( tRGBtxt( 112, 128, 144,      "SlateGrey                 " ) )
//   ( tRGBtxt( 119, 136, 153,      "light slate gray          " ) )
//   ( tRGBtxt( 119, 136, 153,      "LightSlateGray            " ) )
//   ( tRGBtxt( 119, 136, 153,      "light slate grey          " ) )
//   ( tRGBtxt( 119, 136, 153,      "LightSlateGrey            " ) )
//   ( tRGBtxt( 190, 190, 190,      "gray                      " ) )
//   ( tRGBtxt( 190, 190, 190,      "grey                      " ) )
//   ( tRGBtxt( 211, 211, 211,      "light grey                " ) )
//   ( tRGBtxt( 211, 211, 211,      "LightGrey                 " ) )
//   ( tRGBtxt( 211, 211, 211,      "light gray                " ) )
//   ( tRGBtxt( 211, 211, 211,      "LightGray                 " ) )
//   ( tRGBtxt( 25,  25, 112,      "midnight blue             " ) )
//   ( tRGBtxt( 25,  25, 112,      "MidnightBlue              " ) )
//   ( tRGBtxt( 0,   0, 128,      "navy                      " ) )
//   ( tRGBtxt( 0,   0, 128,      "navy blue                 " ) )
//   ( tRGBtxt( 0,   0, 128,      "NavyBlue                  " ) )
//   ( tRGBtxt( 100, 149, 237,      "cornflower blue           " ) )
//   ( tRGBtxt( 100, 149, 237,      "CornflowerBlue            " ) )
//   ( tRGBtxt( 72,  61, 139,      "dark slate blue           " ) )
//   ( tRGBtxt( 72,  61, 139,      "DarkSlateBlue             " ) )
//   ( tRGBtxt( 106,  90, 205,      "slate blue                " ) )
//   ( tRGBtxt( 106,  90, 205,      "SlateBlue                 " ) )
//   ( tRGBtxt( 123, 104, 238,      "medium slate blue         " ) )
//   ( tRGBtxt( 123, 104, 238,      "MediumSlateBlue           " ) )
//   ( tRGBtxt( 132, 112, 255,      "light slate blue          " ) )
//   ( tRGBtxt( 132, 112, 255,      "LightSlateBlue            " ) )
//   ( tRGBtxt( 0,   0, 205,      "medium blue               " ) )
//   ( tRGBtxt( 0,   0, 205,      "MediumBlue                " ) )
//   ( tRGBtxt( 65, 105, 225,      "royal blue                " ) )
//   ( tRGBtxt( 65, 105, 225,      "RoyalBlue                 " ) )
//   ( tRGBtxt( 0,   0, 255,      "blue                      " ) )
//   ( tRGBtxt( 30, 144, 255,      "dodger blue               " ) )
//   ( tRGBtxt( 30, 144, 255,      "DodgerBlue                " ) )
//   ( tRGBtxt( 0, 191, 255,    "deep sky blue             " ) )
//   ( tRGBtxt( 0, 191, 255,    "DeepSkyBlue               " ) )
//   ( tRGBtxt( 135, 206, 235,      "sky blue                  " ) )
//   ( tRGBtxt( 135, 206, 235,      "SkyBlue                   " ) )
//   ( tRGBtxt( 135, 206, 250,      "light sky blue            " ) )
//   ( tRGBtxt( 135, 206, 250,      "LightSkyBlue              " ) )
//   ( tRGBtxt( 70, 130, 180,      "steel blue                " ) )
//   ( tRGBtxt( 70, 130, 180,      "SteelBlue                 " ) )
//   ( tRGBtxt( 176, 196, 222,      "light steel blue          " ) )
//   ( tRGBtxt( 176, 196, 222,      "LightSteelBlue            " ) )
//   ( tRGBtxt( 173, 216, 230,      "light blue                " ) )
//   ( tRGBtxt( 173, 216, 230,      "LightBlue                 " ) )
//   ( tRGBtxt( 176, 224, 230,      "powder blue               " ) )
//   ( tRGBtxt( 176, 224, 230,      "PowderBlue                " ) )
//   ( tRGBtxt( 175, 238, 238,      "pale turquoise            " ) )
//   ( tRGBtxt( 175, 238, 238,      "PaleTurquoise             " ) )
//   ( tRGBtxt( 0, 206, 209,    "dark turquoise            " ) )
//   ( tRGBtxt( 0, 206, 209,    "DarkTurquoise             " ) )
//   ( tRGBtxt( 72, 209, 204,      "medium turquoise          " ) )
//   ( tRGBtxt( 72, 209, 204,      "MediumTurquoise           " ) )
//   ( tRGBtxt( 64, 224, 208,      "turquoise                 " ) )
//   ( tRGBtxt( 0, 255, 255,    "cyan                      " ) )
//   ( tRGBtxt( 224, 255, 255,      "light cyan                " ) )
//   ( tRGBtxt( 224, 255, 255,      "LightCyan                 " ) )
//   ( tRGBtxt( 95, 158, 160,      "cadet blue                " ) )
//   ( tRGBtxt( 95, 158, 160,      "CadetBlue                 " ) )
//   ( tRGBtxt( 102, 205, 170,      "medium aquamarine         " ) )
//   ( tRGBtxt( 102, 205, 170,      "MediumAquamarine          " ) )
//   ( tRGBtxt( 127, 255, 212,      "aquamarine                " ) )
//   ( tRGBtxt( 0, 100,   0,    "dark green                " ) )
//   ( tRGBtxt( 0, 100,   0,    "DarkGreen                 " ) )
//   ( tRGBtxt( 85, 107,  47,      "dark olive green          " ) )

//   ( tRGBtxt( 85, 107,  47,      "DarkOliveGreen            " ) )
//   ( tRGBtxt( 143, 188, 143,      "dark sea green            " ) )
//   ( tRGBtxt( 143, 188, 143,      "DarkSeaGreen              " ) )
//   ( tRGBtxt( 46, 139,  87,      "sea green                 " ) )
//   ( tRGBtxt( 46, 139,  87,      "SeaGreen                  " ) )
//   ( tRGBtxt( 60, 179, 113,      "medium sea green          " ) )
//   ( tRGBtxt( 60, 179, 113,      "MediumSeaGreen            " ) )
//   ( tRGBtxt( 32, 178, 170,      "light sea green           " ) )
//   ( tRGBtxt( 32, 178, 170,      "LightSeaGreen             " ) )
//   ( tRGBtxt( 152, 251, 152,      "pale green                " ) )
//   ( tRGBtxt( 152, 251, 152,      "PaleGreen                 " ) )
//   ( tRGBtxt( 0, 255, 127,    "spring green              " ) )
//   ( tRGBtxt( 0, 255, 127,    "SpringGreen               " ) )
//   ( tRGBtxt( 124, 252,   0,      "lawn green                " ) )
//   ( tRGBtxt( 124, 252,   0,      "LawnGreen                 " ) )
//   ( tRGBtxt( 0, 255,   0,    "green                     " ) )
//   ( tRGBtxt( 127, 255,   0,      "chartreuse                " ) )
//   ( tRGBtxt( 0, 250, 154,    "medium spring green       " ) )
//   ( tRGBtxt( 0, 250, 154,    "MediumSpringGreen         " ) )
//   ( tRGBtxt( 173, 255,  47,      "green yellow              " ) )
//   ( tRGBtxt( 173, 255,  47,      "GreenYellow               " ) )
//   ( tRGBtxt( 50, 205,  50,      "lime green                " ) )
//   ( tRGBtxt( 50, 205,  50,      "LimeGreen                 " ) )
//   ( tRGBtxt( 154, 205,  50,      "yellow green              " ) )
//   ( tRGBtxt( 154, 205,  50,      "YellowGreen               " ) )
//   ( tRGBtxt( 34, 139,  34,      "forest green              " ) )
//   ( tRGBtxt( 34, 139,  34,      "ForestGreen               " ) )
//   ( tRGBtxt( 107, 142,  35,      "olive drab                " ) )
//   ( tRGBtxt( 107, 142,  35,      "OliveDrab                 " ) )
//   ( tRGBtxt( 189, 183, 107,      "dark khaki                " ) )
//   ( tRGBtxt( 189, 183, 107,      "DarkKhaki                 " ) )
//   ( tRGBtxt( 240, 230, 140,      "khaki                     " ) )
//   ( tRGBtxt( 238, 232, 170,      "pale goldenrod            " ) )
//   ( tRGBtxt( 238, 232, 170,      "PaleGoldenrod             " ) )
//   ( tRGBtxt( 250, 250, 210,      "light goldenrod yellow    " ) )
//   ( tRGBtxt( 250, 250, 210,      "LightGoldenrodYellow      " ) )
//   ( tRGBtxt( 255, 255, 224,      "light yellow              " ) )
//   ( tRGBtxt( 255, 255, 224,      "LightYellow               " ) )
//   ( tRGBtxt( 255, 255,   0,      "yellow                    " ) )
//   ( tRGBtxt( 255, 215,   0,      "gold                      " ) )
//   ( tRGBtxt( 238, 221, 130,      "light goldenrod           " ) )
//   ( tRGBtxt( 238, 221, 130,      "LightGoldenrod            " ) )
//   ( tRGBtxt( 218, 165,  32,      "goldenrod                 " ) )
//   ( tRGBtxt( 184, 134,  11,      "dark goldenrod            " ) )
//   ( tRGBtxt( 184, 134,  11,      "DarkGoldenrod             " ) )
//   ( tRGBtxt( 188, 143, 143,      "rosy brown                " ) )
//   ( tRGBtxt( 188, 143, 143,      "RosyBrown                 " ) )
//   ( tRGBtxt( 205,  92,  92,      "indian red                " ) )
//   ( tRGBtxt( 205,  92,  92,      "IndianRed                 " ) )
//   ( tRGBtxt( 139,  69,  19,      "saddle brown              " ) )
//   ( tRGBtxt( 139,  69,  19,      "SaddleBrown               " ) )
//   ( tRGBtxt( 160,  82,  45,      "sienna                    " ) )
//   ( tRGBtxt( 205, 133,  63,      "peru                      " ) )
//   ( tRGBtxt( 222, 184, 135,      "burlywood                 " ) )
//   ( tRGBtxt( 245, 245, 220,      "beige                     " ) )
//   ( tRGBtxt( 245, 222, 179,      "wheat                     " ) )
//   ( tRGBtxt( 244, 164,  96,      "sandy brown               " ) )
//   ( tRGBtxt( 244, 164,  96,      "SandyBrown                " ) )
//   ( tRGBtxt( 210, 180, 140,      "tan                       " ) )
//   ( tRGBtxt( 210, 105,  30,      "chocolate                 " ) )
//   ( tRGBtxt( 178,  34,  34,      "firebrick                 " ) )
//   ( tRGBtxt( 165,  42,  42,      "brown                     " ) )
//   ( tRGBtxt( 233, 150, 122,      "dark salmon               " ) )
//   ( tRGBtxt( 233, 150, 122,      "DarkSalmon                " ) )
//   ( tRGBtxt( 250, 128, 114,      "salmon                    " ) )
//   ( tRGBtxt( 255, 160, 122,      "light salmon              " ) )
//   ( tRGBtxt( 255, 160, 122,      "LightSalmon               " ) )
//   ( tRGBtxt( 255, 165,   0,      "orange                    " ) )
//   ( tRGBtxt( 255, 140,   0,      "dark orange               " ) )
//   ( tRGBtxt( 255, 140,   0,      "DarkOrange                " ) )
//   ( tRGBtxt( 255, 127,  80,      "coral                     " ) )
//   ( tRGBtxt( 240, 128, 128,      "light coral               " ) )
//   ( tRGBtxt( 240, 128, 128,      "LightCoral                " ) )
//   ( tRGBtxt( 255,  99,  71,      "tomato                    " ) )
//   ( tRGBtxt( 255,  69,   0,      "orange red                " ) )
//   ( tRGBtxt( 255,  69,   0,      "OrangeRed                 " ) )
//   ( tRGBtxt( 255,   0,   0,      "red                       " ) )
//   ( tRGBtxt( 255, 105, 180,      "hot pink                  " ) )
//   ( tRGBtxt( 255, 105, 180,      "HotPink                   " ) )
//   ( tRGBtxt( 255,  20, 147,      "deep pink                 " ) )
//   ( tRGBtxt( 255,  20, 147,      "DeepPink                  " ) )
//   ( tRGBtxt( 255, 192, 203,      "pink                      " ) )
//   ( tRGBtxt( 255, 182, 193,      "light pink                " ) )
//   ( tRGBtxt( 255, 182, 193,      "LightPink                 " ) )
//   ( tRGBtxt( 219, 112, 147,      "pale violet red           " ) )
//   ( tRGBtxt( 219, 112, 147,      "PaleVioletRed             " ) )
//   ( tRGBtxt( 176,  48,  96,      "maroon                    " ) )
//   ( tRGBtxt( 199,  21, 133,      "medium violet red         " ) )
//   ( tRGBtxt( 199,  21, 133,      "MediumVioletRed           " ) )
//   ( tRGBtxt( 208,  32, 144,      "violet red                " ) )
//   ( tRGBtxt( 208,  32, 144,      "VioletRed                 " ) )
//   ( tRGBtxt( 255,   0, 255,      "magenta                   " ) )
//   ( tRGBtxt( 238, 130, 238,      "violet                    " ) )
//   ( tRGBtxt( 221, 160, 221,      "plum                      " ) )
//   ( tRGBtxt( 218, 112, 214,      "orchid                    " ) )
//   ( tRGBtxt( 186,  85, 211,      "medium orchid             " ) )
//   ( tRGBtxt( 186,  85, 211,      "MediumOrchid              " ) )
//   ( tRGBtxt( 153,  50, 204,      "dark orchid               " ) )
//   ( tRGBtxt( 153,  50, 204,      "DarkOrchid                " ) )
//   ( tRGBtxt( 148,   0, 211,      "dark violet               " ) )
//   ( tRGBtxt( 148,   0, 211,      "DarkViolet                " ) )
//   ( tRGBtxt( 138,  43, 226,      "blue violet               " ) )
//   ( tRGBtxt( 138,  43, 226,      "BlueViolet                " ) )
//   ( tRGBtxt( 160,  32, 240,      "purple                    " ) )
//   ( tRGBtxt( 147, 112, 219,      "medium purple             " ) )
//   ( tRGBtxt( 147, 112, 219,      "MediumPurple              " ) )
//   ( tRGBtxt( 216, 191, 216,      "thistle                   " ) )
//   ( tRGBtxt( 255, 250, 250,      "snow1                     " ) )
//   ( tRGBtxt( 238, 233, 233,      "snow2                     " ) )
//   ( tRGBtxt( 205, 201, 201,      "snow3                     " ) )
//   ( tRGBtxt( 139, 137, 137,      "snow4                     " ) )
//   ( tRGBtxt( 255, 245, 238,      "seashell1                 " ) )
//   ( tRGBtxt( 238, 229, 222,      "seashell2                 " ) )
//   ( tRGBtxt( 205, 197, 191,      "seashell3                 " ) )
//   ( tRGBtxt( 139, 134, 130,      "seashell4                 " ) )
//   ( tRGBtxt( 255, 239, 219,      "AntiqueWhite1             " ) )
//   ( tRGBtxt( 238, 223, 204,      "AntiqueWhite2             " ) )
//   ( tRGBtxt( 205, 192, 176,      "AntiqueWhite3             " ) )
//   ( tRGBtxt( 139, 131, 120,      "AntiqueWhite4             " ) )
//   ( tRGBtxt( 255, 228, 196,      "bisque1                   " ) )
//   ( tRGBtxt( 238, 213, 183,      "bisque2                   " ) )
//   ( tRGBtxt( 205, 183, 158,      "bisque3                   " ) )
//   ( tRGBtxt( 139, 125, 107,      "bisque4                   " ) )
//   ( tRGBtxt( 255, 218, 185,      "PeachPuff1                " ) )
//   ( tRGBtxt( 238, 203, 173,      "PeachPuff2                " ) )
//   ( tRGBtxt( 205, 175, 149,      "PeachPuff3                " ) )
//   ( tRGBtxt( 139, 119, 101,      "PeachPuff4                " ) )
//   ( tRGBtxt( 255, 222, 173,      "NavajoWhite1              " ) )
//   ( tRGBtxt( 238, 207, 161,      "NavajoWhite2              " ) )
//   ( tRGBtxt( 205, 179, 139,      "NavajoWhite3              " ) )
//   ( tRGBtxt( 139, 121,   94,     "NavajoWhite4              " ) )
//   ( tRGBtxt( 255, 250, 205,      "LemonChiffon1             " ) )
//   ( tRGBtxt( 238, 233, 191,      "LemonChiffon2             " ) )
//   ( tRGBtxt( 205, 201, 165,      "LemonChiffon3             " ) )
//   ( tRGBtxt( 139, 137, 112,      "LemonChiffon4             " ) )
//   ( tRGBtxt( 255, 248, 220,      "cornsilk1                 " ) )
//   ( tRGBtxt( 238, 232, 205,      "cornsilk2                 " ) )
//   ( tRGBtxt( 205, 200, 177,      "cornsilk3                 " ) )
//   ( tRGBtxt( 139, 136, 120,      "cornsilk4                 " ) )
//   ( tRGBtxt( 255, 255, 240,      "ivory1                    " ) )
//   ( tRGBtxt( 238, 238, 224,      "ivory2                    " ) )
//   ( tRGBtxt( 205, 205, 193,      "ivory3                    " ) )
//   ( tRGBtxt( 139, 139, 131,      "ivory4                    " ) )
//   ( tRGBtxt( 240, 255, 240,      "honeydew1                 " ) )
//   ( tRGBtxt( 224, 238, 224,      "honeydew2                 " ) )
//   ( tRGBtxt( 193, 205, 193,      "honeydew3                 " ) )
//   ( tRGBtxt( 131, 139, 131,      "honeydew4                 " ) )
//   ( tRGBtxt( 255, 240, 245,      "LavenderBlush1            " ) )
//   ( tRGBtxt( 238, 224, 229,      "LavenderBlush2            " ) )
//   ( tRGBtxt( 205, 193, 197,      "LavenderBlush3            " ) )
//   ( tRGBtxt( 139, 131, 134,      "LavenderBlush4            " ) )
//   ( tRGBtxt( 255, 228, 225,      "MistyRose1                " ) )
//   ( tRGBtxt( 238, 213, 210,      "MistyRose2                " ) )
//   ( tRGBtxt( 205, 183, 181,      "MistyRose3                " ) )
//   ( tRGBtxt( 139, 125, 123,      "MistyRose4                " ) )
//   ( tRGBtxt( 240, 255, 255,      "azure1                    " ) )
//   ( tRGBtxt( 224, 238, 238,      "azure2                    " ) )
//   ( tRGBtxt( 193, 205, 205,      "azure3                    " ) )
//   ( tRGBtxt( 131, 139, 139,      "azure4                    " ) )
//   ( tRGBtxt( 131, 111, 255,      "SlateBlue1                " ) )
//   ( tRGBtxt( 122, 103, 238,      "SlateBlue2                " ) )
//   ( tRGBtxt( 105,  89, 205,      "SlateBlue3                " ) )
//   ( tRGBtxt( 71,  60, 139,      "SlateBlue4                " ) )
//   ( tRGBtxt( 72, 118, 255,      "RoyalBlue1                " ) )
//   ( tRGBtxt( 67, 110, 238,      "RoyalBlue2                " ) )
//   ( tRGBtxt( 58,  95, 205,      "RoyalBlue3                " ) )
//   ( tRGBtxt( 39,  64, 139,      "RoyalBlue4                " ) )
//   ( tRGBtxt( 0,   0, 255,    "blue1                     " ) )
//   ( tRGBtxt( 0,   0, 238,    "blue2                     " ) )
//   ( tRGBtxt( 0,   0, 205,    "blue3                     " ) )
//   ( tRGBtxt( 0,   0, 139,    "blue4                     " ) )
//   ( tRGBtxt( 30, 144, 255,      "DodgerBlue1               " ) )
//   ( tRGBtxt( 28, 134, 238,      "DodgerBlue2               " ) )
//   ( tRGBtxt( 24, 116, 205,      "DodgerBlue3               " ) )
//   ( tRGBtxt( 16,  78, 139,      "DodgerBlue4               " ) )
//   ( tRGBtxt( 99, 184, 255,      "SteelBlue1                " ) )
//   ( tRGBtxt( 92, 172, 238,      "SteelBlue2                " ) )
//   ( tRGBtxt( 79, 148, 205,      "SteelBlue3                " ) )
//   ( tRGBtxt( 54, 100, 139,      "SteelBlue4                " ) )
//   ( tRGBtxt( 0, 191, 255,    "DeepSkyBlue1              " ) )
//   ( tRGBtxt( 0, 178, 238,    "DeepSkyBlue2              " ) )
//   ( tRGBtxt( 0, 154, 205,    "DeepSkyBlue3              " ) )
//   ( tRGBtxt( 0, 104, 139,    "DeepSkyBlue4              " ) )
//   ( tRGBtxt( 135, 206, 255,      "SkyBlue1                  " ) )
//   ( tRGBtxt( 126, 192, 238,      "SkyBlue2                  " ) )
//   ( tRGBtxt( 108, 166, 205,      "SkyBlue3                  " ) )
//   ( tRGBtxt( 74, 112, 139,      "SkyBlue4                  " ) )
//   ( tRGBtxt( 176, 226, 255,      "LightSkyBlue1             " ) )
//   ( tRGBtxt( 164, 211, 238,      "LightSkyBlue2             " ) )
//   ( tRGBtxt( 141, 182, 205,      "LightSkyBlue3             " ) )
//   ( tRGBtxt( 96, 123, 139,      "LightSkyBlue4             " ) )
//   ( tRGBtxt( 198, 226, 255,      "SlateGray1                " ) )
//   ( tRGBtxt( 185, 211, 238,      "SlateGray2                " ) )
//   ( tRGBtxt( 159, 182, 205,      "SlateGray3                " ) )
//   ( tRGBtxt( 108, 123, 139,      "SlateGray4                " ) )
//   ( tRGBtxt( 202, 225, 255,      "LightSteelBlue1           " ) )
//   ( tRGBtxt( 188, 210, 238,      "LightSteelBlue2           " ) )
//   ( tRGBtxt( 162, 181, 205,      "LightSteelBlue3           " ) )
//   ( tRGBtxt( 110, 123, 139,      "LightSteelBlue4           " ) )
//   ( tRGBtxt( 191, 239, 255,      "LightBlue1                " ) )
//   ( tRGBtxt( 178, 223, 238,      "LightBlue2                " ) )
//   ( tRGBtxt( 154, 192, 205,      "LightBlue3                " ) )
//   ( tRGBtxt( 104, 131, 139,      "LightBlue4                " ) )
//   ( tRGBtxt( 224, 255, 255,      "LightCyan1                " ) )
//   ( tRGBtxt( 209, 238, 238,      "LightCyan2                " ) )
//   ( tRGBtxt( 180, 205, 205,      "LightCyan3                " ) )
//   ( tRGBtxt( 122, 139, 139,      "LightCyan4                " ) )
//   ( tRGBtxt( 187, 255, 255,      "PaleTurquoise1            " ) )
//   ( tRGBtxt( 174, 238, 238,      "PaleTurquoise2            " ) )
//   ( tRGBtxt( 150, 205, 205,      "PaleTurquoise3            " ) )
//   ( tRGBtxt( 102, 139, 139,      "PaleTurquoise4            " ) )
//   ( tRGBtxt( 152, 245, 255,      "CadetBlue1                " ) )
//   ( tRGBtxt( 142, 229, 238,      "CadetBlue2                " ) )
//   ( tRGBtxt( 122, 197, 205,      "CadetBlue3                " ) )
//   ( tRGBtxt( 83, 134, 139,      "CadetBlue4                " ) )
//   ( tRGBtxt( 0, 245, 255,    "turquoise1                " ) )
//   ( tRGBtxt( 0, 229, 238,    "turquoise2                " ) )
//   ( tRGBtxt( 0, 197, 205,    "turquoise3                " ) )
//   ( tRGBtxt( 0, 134, 139,    "turquoise4                " ) )
//   ( tRGBtxt( 0, 255, 255,    "cyan1                     " ) )
//   ( tRGBtxt( 0, 238, 238,    "cyan2                     " ) )
//   ( tRGBtxt( 0, 205, 205,    "cyan3                     " ) )
//   ( tRGBtxt( 0, 139, 139,    "cyan4                     " ) )
//   ( tRGBtxt( 151, 255, 255,      "DarkSlateGray1            " ) )
//   ( tRGBtxt( 141, 238, 238,      "DarkSlateGray2            " ) )
//   ( tRGBtxt( 121, 205, 205,      "DarkSlateGray3            " ) )
//   ( tRGBtxt( 82, 139, 139,      "DarkSlateGray4            " ) )
//   ( tRGBtxt( 127, 255, 212,      "aquamarine1               " ) )
//   ( tRGBtxt( 118, 238, 198,      "aquamarine2               " ) )
//   ( tRGBtxt( 102, 205, 170,      "aquamarine3               " ) )
//   ( tRGBtxt( 69, 139, 116,      "aquamarine4               " ) )
//   ( tRGBtxt( 193, 255, 193,      "DarkSeaGreen1             " ) )
//   ( tRGBtxt( 180, 238, 180,      "DarkSeaGreen2             " ) )
//   ( tRGBtxt( 155, 205, 155,      "DarkSeaGreen3             " ) )
//   ( tRGBtxt( 105, 139, 105,      "DarkSeaGreen4             " ) )
//   ( tRGBtxt( 84, 255, 159,      "SeaGreen1                 " ) )
//   ( tRGBtxt( 78, 238, 148,      "SeaGreen2                 " ) )
//   ( tRGBtxt( 67, 205, 128,      "SeaGreen3                 " ) )
//   ( tRGBtxt( 46, 139,   87,     "SeaGreen4                 " ) )
//   ( tRGBtxt( 154, 255, 154,      "PaleGreen1                " ) )
//   ( tRGBtxt( 144, 238, 144,      "PaleGreen2                " ) )
//   ( tRGBtxt( 124, 205, 124,      "PaleGreen3                " ) )
//   ( tRGBtxt( 84, 139,   84,     "PaleGreen4                " ) )
//   ( tRGBtxt( 0, 255, 127,    "SpringGreen1              " ) )
//   ( tRGBtxt( 0, 238, 118,    "SpringGreen2              " ) )
//   ( tRGBtxt( 0, 205, 102,    "SpringGreen3              " ) )
//   ( tRGBtxt( 0, 139,    69,     "SpringGreen4              " ) )
//   ( tRGBtxt( 0, 255,     0,     "green1                    " ) )
//   ( tRGBtxt( 0, 238,     0,     "green2                    " ) )
//   ( tRGBtxt( 0, 205,     0,     "green3                    " ) )
//   ( tRGBtxt( 0, 139,     0,     "green4                    " ) )
//   ( tRGBtxt( 127, 255,    0,     "chartreuse1               " ) )
//   ( tRGBtxt( 118, 238,    0,     "chartreuse2               " ) )
//   ( tRGBtxt( 102, 205,    0,     "chartreuse3               " ) )
//   ( tRGBtxt( 69, 139,    0,     "chartreuse4               " ) )
//   ( tRGBtxt( 192, 255,   62,     "OliveDrab1                " ) )
//   ( tRGBtxt( 179, 238,   58,     "OliveDrab2                " ) )
//   ( tRGBtxt( 154, 205,   50,     "OliveDrab3                " ) )
//   ( tRGBtxt( 105, 139,   34,     "OliveDrab4                " ) )
//   ( tRGBtxt( 202, 255, 112,      "DarkOliveGreen1           " ) )
//   ( tRGBtxt( 188, 238, 104,      "DarkOliveGreen2           " ) )
//   ( tRGBtxt( 162, 205,   90,     "DarkOliveGreen3           " ) )
//   ( tRGBtxt( 110, 139,   61,     "DarkOliveGreen4           " ) )
//   ( tRGBtxt( 255, 246, 143,      "khaki1                    " ) )
//   ( tRGBtxt( 238, 230, 133,      "khaki2                    " ) )
//   ( tRGBtxt( 205, 198, 115,      "khaki3                    " ) )
//   ( tRGBtxt( 139, 134,   78,     "khaki4                    " ) )
//   ( tRGBtxt( 255, 236, 139,      "LightGoldenrod1           " ) )
//   ( tRGBtxt( 238, 220, 130,      "LightGoldenrod2           " ) )
//   ( tRGBtxt( 205, 190, 112,      "LightGoldenrod3           " ) )
//   ( tRGBtxt( 139, 129,   76,     "LightGoldenrod4           " ) )
//   ( tRGBtxt( 255, 255, 224,      "LightYellow1              " ) )
//   ( tRGBtxt( 238, 238, 209,      "LightYellow2              " ) )
//   ( tRGBtxt( 205, 205, 180,      "LightYellow3              " ) )
//   ( tRGBtxt( 139, 139, 122,      "LightYellow4              " ) )
//   ( tRGBtxt( 255, 255,    0,     "yellow1                   " ) )
//   ( tRGBtxt( 238, 238,    0,     "yellow2                   " ) )
//   ( tRGBtxt( 205, 205,    0,     "yellow3                   " ) )
//   ( tRGBtxt( 139, 139,    0,     "yellow4                   " ) )
//   ( tRGBtxt( 255, 215,    0,     "gold1                     " ) )
//   ( tRGBtxt( 238, 201,    0,     "gold2                     " ) )
//   ( tRGBtxt( 205, 173,    0,     "gold3                     " ) )
//   ( tRGBtxt( 139, 117,    0,     "gold4                     " ) )
//   ( tRGBtxt( 255, 193,   37,     "goldenrod1                " ) )
//   ( tRGBtxt( 238, 180,   34,     "goldenrod2                " ) )
//   ( tRGBtxt( 205, 155,   29,     "goldenrod3                " ) )
//   ( tRGBtxt( 139, 105,   20,     "goldenrod4                " ) )
//   ( tRGBtxt( 255, 185,   15,     "DarkGoldenrod1            " ) )
//   ( tRGBtxt( 238, 173,   14,     "DarkGoldenrod2            " ) )
//   ( tRGBtxt( 205, 149,   12,     "DarkGoldenrod3            " ) )
//   ( tRGBtxt( 139, 101,    8,     "DarkGoldenrod4            " ) )
//   ( tRGBtxt( 255, 193, 193,      "RosyBrown1                " ) )
//   ( tRGBtxt( 238, 180, 180,      "RosyBrown2                " ) )
//   ( tRGBtxt( 205, 155, 155,      "RosyBrown3                " ) )
//   ( tRGBtxt( 139, 105, 105,      "RosyBrown4                " ) )
//   ( tRGBtxt( 255, 106, 106,      "IndianRed1                " ) )
//   ( tRGBtxt( 238,  99,   99,     "IndianRed2                " ) )
//   ( tRGBtxt( 205,  85,   85,     "IndianRed3                " ) )
//   ( tRGBtxt( 139,  58,   58,     "IndianRed4                " ) )
//   ( tRGBtxt( 255, 130,   71,     "sienna1                   " ) )
//   ( tRGBtxt( 238, 121,   66,     "sienna2                   " ) )
//   ( tRGBtxt( 205, 104,   57,     "sienna3                   " ) )
//   ( tRGBtxt( 139,  71,   38,     "sienna4                   " ) )
//   ( tRGBtxt( 255, 211, 155,      "burlywood1                " ) )
//   ( tRGBtxt( 238, 197, 145,      "burlywood2                " ) )
//   ( tRGBtxt( 205, 170, 125,      "burlywood3                " ) )
//   ( tRGBtxt( 139, 115,   85,     "burlywood4                " ) )
//   ( tRGBtxt( 255, 231, 186,      "wheat1                    " ) )
//   ( tRGBtxt( 238, 216, 174,      "wheat2                    " ) )
//   ( tRGBtxt( 205, 186, 150,      "wheat3                    " ) )
//   ( tRGBtxt( 139, 126, 102,      "wheat4                    " ) )
//   ( tRGBtxt( 255, 165,   79,     "tan1                      " ) )
//   ( tRGBtxt( 238, 154,   73,     "tan2                      " ) )
//   ( tRGBtxt( 205, 133,   63,     "tan3                      " ) )
//   ( tRGBtxt( 139,  90,   43,     "tan4                      " ) )
//   ( tRGBtxt( 255, 127,   36,     "chocolate1                " ) )
//   ( tRGBtxt( 238, 118,   33,     "chocolate2                " ) )
//   ( tRGBtxt( 205, 102,   29,     "chocolate3                " ) )
//   ( tRGBtxt( 139,  69,   19,     "chocolate4                " ) )
//   ( tRGBtxt( 255,  48,   48,     "firebrick1                " ) )
//   ( tRGBtxt( 238,  44,   44,     "firebrick2                " ) )
//   ( tRGBtxt( 205,  38,   38,     "firebrick3                " ) )
//   ( tRGBtxt( 139,  26,   26,     "firebrick4                " ) )
//   ( tRGBtxt( 255,  64,   64,     "brown1                    " ) )
//   ( tRGBtxt( 238,  59,   59,     "brown2                    " ) )
//   ( tRGBtxt( 205,  51,   51,     "brown3                    " ) )
//   ( tRGBtxt( 139,  35,   35,     "brown4                    " ) )
//   ( tRGBtxt( 255, 140, 105,      "salmon1                   " ) )
//   ( tRGBtxt( 238, 130,   98,     "salmon2                   " ) )
//   ( tRGBtxt( 205, 112,   84,     "salmon3                   " ) )
//   ( tRGBtxt( 139,  76,   57,     "salmon4                   " ) )
//   ( tRGBtxt( 255, 160, 122,      "LightSalmon1              " ) )
//   ( tRGBtxt( 238, 149, 114,      "LightSalmon2              " ) )
//   ( tRGBtxt( 205, 129,   98,     "LightSalmon3              " ) )
//   ( tRGBtxt( 139,  87,   66,     "LightSalmon4              " ) )
//   ( tRGBtxt( 255, 165,    0,     "orange1                   " ) )
//   ( tRGBtxt( 238, 154,    0,     "orange2                   " ) )
//   ( tRGBtxt( 205, 133,    0,     "orange3                   " ) )
//   ( tRGBtxt( 139,  90,    0,     "orange4                   " ) )
//   ( tRGBtxt( 255, 127,    0,     "DarkOrange1               " ) )
//   ( tRGBtxt( 238, 118,    0,     "DarkOrange2               " ) )
//   ( tRGBtxt( 205, 102,    0,     "DarkOrange3               " ) )
//   ( tRGBtxt( 139,  69,    0,     "DarkOrange4               " ) )
//   ( tRGBtxt( 255, 114,   86,     "coral1                    " ) )
//   ( tRGBtxt( 238, 106,   80,     "coral2                    " ) )
//   ( tRGBtxt( 205,  91,   69,     "coral3                    " ) )
//   ( tRGBtxt( 139,  62,   47,     "coral4                    " ) )
//   ( tRGBtxt( 255,  99,   71,     "tomato1                   " ) )
//   ( tRGBtxt( 238,  92,   66,     "tomato2                   " ) )
//   ( tRGBtxt( 205,  79,   57,     "tomato3                   " ) )
//   ( tRGBtxt( 139,  54,   38,     "tomato4                   " ) )
//   ( tRGBtxt( 255,  69,    0,     "OrangeRed1                " ) )
//   ( tRGBtxt( 238,  64,    0,     "OrangeRed2                " ) )
//   ( tRGBtxt( 205,  55,    0,     "OrangeRed3                " ) )
//   ( tRGBtxt( 139,  37,    0,     "OrangeRed4                " ) )
//   ( tRGBtxt( 255,   0,    0,     "red1                      " ) )
//   ( tRGBtxt( 238,   0,    0,     "red2                      " ) )
//   ( tRGBtxt( 205,   0,    0,     "red3                      " ) )
//   ( tRGBtxt( 139,   0,    0,     "red4                      " ) )
//   ( tRGBtxt( 215,   7,  81,      "DebianRed                 " ) )
//   ( tRGBtxt( 255,  20, 147,      "DeepPink1                 " ) )
//   ( tRGBtxt( 238,  18, 137,      "DeepPink2                 " ) )
//   ( tRGBtxt( 205,  16, 118,      "DeepPink3                 " ) )
//   ( tRGBtxt( 139,  10,   80,     "DeepPink4                 " ) )
//   ( tRGBtxt( 255, 110, 180,      "HotPink1                  " ) )
//   ( tRGBtxt( 238, 106, 167,      "HotPink2                  " ) )
//   ( tRGBtxt( 205,  96, 144,      "HotPink3                  " ) )
//   ( tRGBtxt( 139,  58,  98,      "HotPink4                  " ) )
//   ( tRGBtxt( 255, 181, 197,      "pink1                     " ) )
//   ( tRGBtxt( 238, 169, 184,      "pink2                     " ) )
//   ( tRGBtxt( 205, 145, 158,      "pink3                     " ) )
//   ( tRGBtxt( 139,  99, 108,      "pink4                     " ) )
//   ( tRGBtxt( 255, 174, 185,      "LightPink1                " ) )
//   ( tRGBtxt( 238, 162, 173,      "LightPink2                " ) )
//   ( tRGBtxt( 205, 140, 149,      "LightPink3                " ) )
//   ( tRGBtxt( 139,  95, 101,      "LightPink4                " ) )
//   ( tRGBtxt( 255, 130, 171,      "PaleVioletRed1            " ) )
//   ( tRGBtxt( 238, 121, 159,      "PaleVioletRed2            " ) )
//   ( tRGBtxt( 205, 104, 137,      "PaleVioletRed3            " ) )
//   ( tRGBtxt( 139,  71,   93,     "PaleVioletRed4            " ) )
//   ( tRGBtxt( 255,  52, 179,      "maroon1                   " ) )
//   ( tRGBtxt( 238,  48, 167,      "maroon2                   " ) )
//   ( tRGBtxt( 205,  41, 144,      "maroon3                   " ) )
//   ( tRGBtxt( 139,  28,   98,     "maroon4                   " ) )
//   ( tRGBtxt( 255,  62, 150,      "VioletRed1                " ) )
//   ( tRGBtxt( 238,  58, 140,      "VioletRed2                " ) )
//   ( tRGBtxt( 205,  50, 120,      "VioletRed3                " ) )
//   ( tRGBtxt( 139,  34,   82,     "VioletRed4                " ) )
//   ( tRGBtxt( 255,   0, 255,      "magenta1                  " ) )
//   ( tRGBtxt( 238,   0, 238,      "magenta2                  " ) )
//   ( tRGBtxt( 205,   0, 205,      "magenta3                  " ) )
//   ( tRGBtxt( 139,   0, 139,      "magenta4                  " ) )
//   ( tRGBtxt( 255, 131, 250,      "orchid1                   " ) )
//   ( tRGBtxt( 238, 122, 233,      "orchid2                   " ) )
//   ( tRGBtxt( 205, 105, 201,      "orchid3                   " ) )
//   ( tRGBtxt( 139,  71, 137,      "orchid4                   " ) )
//   ( tRGBtxt( 255, 187, 255,      "plum1                     " ) )
//   ( tRGBtxt( 238, 174, 238,      "plum2                     " ) )
//   ( tRGBtxt( 205, 150, 205,      "plum3                     " ) )
//   ( tRGBtxt( 139, 102, 139,      "plum4                     " ) )
//   ( tRGBtxt( 224, 102, 255,      "MediumOrchid1             " ) )
//   ( tRGBtxt( 209,  95, 238,      "MediumOrchid2             " ) )
//   ( tRGBtxt( 180,  82, 205,      "MediumOrchid3             " ) )
//   ( tRGBtxt( 122,  55, 139,      "MediumOrchid4             " ) )
//   ( tRGBtxt( 191,  62, 255,      "DarkOrchid1               " ) )
//   ( tRGBtxt( 178,  58, 238,      "DarkOrchid2               " ) )
//   ( tRGBtxt( 154,  50, 205,      "DarkOrchid3               " ) )
//   ( tRGBtxt( 104,  34, 139,      "DarkOrchid4               " ) )
//   ( tRGBtxt( 155,  48, 255,      "purple1                   " ) )
//   ( tRGBtxt( 145,  44, 238,      "purple2                   " ) )
//   ( tRGBtxt( 125,  38, 205,      "purple3                   " ) )
//   ( tRGBtxt( 85,  26, 139,      "purple4                   " ) )
//   ( tRGBtxt( 171, 130, 255,      "MediumPurple1             " ) )
//   ( tRGBtxt( 159, 121, 238,      "MediumPurple2             " ) )
//   ( tRGBtxt( 137, 104, 205,      "MediumPurple3             " ) )
//   ( tRGBtxt( 93,  71, 139,      "MediumPurple4             " ) )
//   ( tRGBtxt( 255, 225, 255,      "thistle1                  " ) )
//   ( tRGBtxt( 238, 210, 238,      "thistle2                  " ) )
//   ( tRGBtxt( 205, 181, 205,      "thistle3                  " ) )
//   ( tRGBtxt( 139, 123, 139,      "thistle4                  " ) )
//   ( tRGBtxt( 0,   0,   0,    "gray0                     " ) )
//   ( tRGBtxt( 0,   0,   0,    "grey0                     " ) )
//   ( tRGBtxt( 3,   3,   3,    "gray1                     " ) )
//   ( tRGBtxt( 3,   3,   3,    "grey1                     " ) )
//   ( tRGBtxt( 5,   5,   5,    "gray2                     " ) )
//   ( tRGBtxt( 5,   5,   5,    "grey2                     " ) )
//   ( tRGBtxt( 8,   8,   8,    "gray3                     " ) )
//   ( tRGBtxt( 8,   8,   8,    "grey3                     " ) )
//   ( tRGBtxt( 10,  10,  10,      "gray4                     " ) )
//   ( tRGBtxt( 10,  10,  10,      "grey4                     " ) )
//   ( tRGBtxt( 13,  13,  13,      "gray5                     " ) )
//   ( tRGBtxt( 13,  13,  13,      "grey5                     " ) )
//   ( tRGBtxt( 15,  15,  15,      "gray6                     " ) )
//   ( tRGBtxt( 15,  15,  15,      "grey6                     " ) )
//   ( tRGBtxt( 18,  18,  18,      "gray7                     " ) )
//   ( tRGBtxt( 18,  18,  18,      "grey7                     " ) )
//   ( tRGBtxt( 20,  20,  20,      "gray8                     " ) )
//   ( tRGBtxt( 20,  20,  20,      "grey8                     " ) )
//   ( tRGBtxt( 23,  23,  23,      "gray9                     " ) )
//   ( tRGBtxt( 23,  23,  23,      "grey9                     " ) )
//   ( tRGBtxt( 26,  26,  26,      "gray10                    " ) )
//   ( tRGBtxt( 26,  26,  26,      "grey10                    " ) )
//   ( tRGBtxt( 28,  28,  28,      "gray11                    " ) )
//   ( tRGBtxt( 28,  28,  28,      "grey11                    " ) )
//   ( tRGBtxt( 31,  31,  31,      "gray12                    " ) )
//   ( tRGBtxt( 31,  31,  31,      "grey12                    " ) )
//   ( tRGBtxt( 33,  33,  33,      "gray13                    " ) )
//   ( tRGBtxt( 33,  33,  33,      "grey13                    " ) )
//   ( tRGBtxt( 36,  36,  36,      "gray14                    " ) )
//   ( tRGBtxt( 36,  36,  36,      "grey14                    " ) )
//   ( tRGBtxt( 38,  38,  38,      "gray15                    " ) )
//   ( tRGBtxt( 38,  38,  38,      "grey15                    " ) )
//   ( tRGBtxt( 41,  41,  41,      "gray16                    " ) )
//   ( tRGBtxt( 41,  41,  41,      "grey16                    " ) )
//   ( tRGBtxt( 43,  43,  43,      "gray17                    " ) )
//   ( tRGBtxt( 43,  43,  43,      "grey17                    " ) )
//   ( tRGBtxt( 46,  46,  46,      "gray18                    " ) )
//   ( tRGBtxt( 46,  46,  46,      "grey18                    " ) )
//   ( tRGBtxt( 48,  48,  48,      "gray19                    " ) )
//   ( tRGBtxt( 48,  48,  48,      "grey19                    " ) )
//   ( tRGBtxt( 51,  51,  51,      "gray20                    " ) )
//   ( tRGBtxt( 51,  51,  51,      "grey20                    " ) )
//   ( tRGBtxt( 54,  54,  54,      "gray21                    " ) )
//   ( tRGBtxt( 54,  54,  54,      "grey21                    " ) )
//   ( tRGBtxt( 56,  56,  56,      "gray22                    " ) )
//   ( tRGBtxt( 56,  56,  56,      "grey22                    " ) )
//   ( tRGBtxt( 59,  59,  59,      "gray23                    " ) )
//   ( tRGBtxt( 59,  59,  59,      "grey23                    " ) )
//   ( tRGBtxt( 61,  61,  61,      "gray24                    " ) )
//   ( tRGBtxt( 61,  61,  61,      "grey24                    " ) )
//   ( tRGBtxt( 64,  64,  64,      "gray25                    " ) )
//   ( tRGBtxt( 64,  64,  64,      "grey25                    " ) )
//   ( tRGBtxt( 66,  66,  66,      "gray26                    " ) )
//   ( tRGBtxt( 66,  66,  66,      "grey26                    " ) )
//   ( tRGBtxt( 69,  69,  69,      "gray27                    " ) )
//   ( tRGBtxt( 69,  69,  69,      "grey27                    " ) )
//   ( tRGBtxt( 71,  71,  71,      "gray28                    " ) )
//   ( tRGBtxt( 71,  71,  71,      "grey28                    " ) )
//   ( tRGBtxt( 74,  74,  74,      "gray29                    " ) )
//   ( tRGBtxt( 74,  74,  74,      "grey29                    " ) )
//   ( tRGBtxt( 77,  77,  77,      "gray30                    " ) )
//   ( tRGBtxt( 77,  77,  77,      "grey30                    " ) )
//   ( tRGBtxt( 79,  79,  79,      "gray31                    " ) )
//   ( tRGBtxt( 79,  79,  79,      "grey31                    " ) )
//   ( tRGBtxt( 82,  82,  82,      "gray32                    " ) )
//   ( tRGBtxt( 82,  82,  82,      "grey32                    " ) )
//   ( tRGBtxt( 84,  84,  84,      "gray33                    " ) )
//   ( tRGBtxt( 84,  84,  84,      "grey33                    " ) )
//   ( tRGBtxt( 87,  87,  87,      "gray34                    " ) )
//   ( tRGBtxt( 87,  87,  87,      "grey34                    " ) )
//   ( tRGBtxt( 89,  89,  89,      "gray35                    " ) )
//   ( tRGBtxt( 89,  89,  89,      "grey35                    " ) )
//   ( tRGBtxt( 92,  92,  92,      "gray36                    " ) )
//   ( tRGBtxt( 92,  92,  92,      "grey36                    " ) )
//   ( tRGBtxt( 94,  94,  94,      "gray37                    " ) )
//   ( tRGBtxt( 94,  94,  94,      "grey37                    " ) )
//   ( tRGBtxt( 97,  97,  97,      "gray38                    " ) )
//   ( tRGBtxt( 97,  97,  97,      "grey38                    " ) )
//   ( tRGBtxt( 99,  99,  99,      "gray39                    " ) )
//   ( tRGBtxt( 99,  99,  99,      "grey39                    " ) )
//   ( tRGBtxt( 102, 102, 102,      "gray40                    " ) )
//   ( tRGBtxt( 102, 102, 102,      "grey40                    " ) )
//   ( tRGBtxt( 105, 105, 105,      "gray41                    " ) )
//   ( tRGBtxt( 105, 105, 105,      "grey41                    " ) )
//   ( tRGBtxt( 107, 107, 107,      "gray42                    " ) )
//   ( tRGBtxt( 107, 107, 107,      "grey42                    " ) )
//   ( tRGBtxt( 110, 110, 110,      "gray43                    " ) )
//   ( tRGBtxt( 110, 110, 110,      "grey43                    " ) )
//   ( tRGBtxt( 112, 112, 112,      "gray44                    " ) )
//   ( tRGBtxt( 112, 112, 112,      "grey44                    " ) )
//   ( tRGBtxt( 115, 115, 115,      "gray45                    " ) )
//   ( tRGBtxt( 115, 115, 115,      "grey45                    " ) )
//   ( tRGBtxt( 117, 117, 117,      "gray46                    " ) )
//   ( tRGBtxt( 117, 117, 117,      "grey46                    " ) )
//   ( tRGBtxt( 120, 120, 120,      "gray47                    " ) )
//   ( tRGBtxt( 120, 120, 120,      "grey47                    " ) )
//   ( tRGBtxt( 122, 122, 122,      "gray48                    " ) )
//   ( tRGBtxt( 122, 122, 122,      "grey48                    " ) )
//   ( tRGBtxt( 125, 125, 125,      "gray49                    " ) )
//   ( tRGBtxt( 125, 125, 125,      "grey49                    " ) )
//   ( tRGBtxt( 127, 127, 127,      "gray50                    " ) )
//   ( tRGBtxt( 127, 127, 127,      "grey50                    " ) )
//   ( tRGBtxt( 130, 130, 130,      "gray51                    " ) )
//   ( tRGBtxt( 130, 130, 130,      "grey51                    " ) )
//   ( tRGBtxt( 133, 133, 133,      "gray52                    " ) )
//   ( tRGBtxt( 133, 133, 133,      "grey52                    " ) )
//   ( tRGBtxt( 135, 135, 135,      "gray53                    " ) )
//   ( tRGBtxt( 135, 135, 135,      "grey53                    " ) )
//   ( tRGBtxt( 138, 138, 138,      "gray54                    " ) )
//   ( tRGBtxt( 138, 138, 138,      "grey54                    " ) )
//   ( tRGBtxt( 140, 140, 140,      "gray55                    " ) )
//   ( tRGBtxt( 140, 140, 140,      "grey55                    " ) )
//   ( tRGBtxt( 143, 143, 143,      "gray56                    " ) )
//   ( tRGBtxt( 143, 143, 143,      "grey56                    " ) )
//   ( tRGBtxt( 145, 145, 145,      "gray57                    " ) )
//   ( tRGBtxt( 145, 145, 145,      "grey57                    " ) )
//   ( tRGBtxt( 148, 148, 148,      "gray58                    " ) )
//   ( tRGBtxt( 148, 148, 148,      "grey58                    " ) )
//   ( tRGBtxt( 150, 150, 150,      "gray59                    " ) )
//   ( tRGBtxt( 150, 150, 150,      "grey59                    " ) )
//   ( tRGBtxt( 153, 153, 153,      "gray60                    " ) )
//   ( tRGBtxt( 153, 153, 153,      "grey60                    " ) )
//   ( tRGBtxt( 156, 156, 156,      "gray61                    " ) )
//   ( tRGBtxt( 156, 156, 156,      "grey61                    " ) )
//   ( tRGBtxt( 158, 158, 158,      "gray62                    " ) )
//   ( tRGBtxt( 158, 158, 158,      "grey62                    " ) )
//   ( tRGBtxt( 161, 161, 161,      "gray63                    " ) )
//   ( tRGBtxt( 161, 161, 161,      "grey63                    " ) )
//   ( tRGBtxt( 163, 163, 163,      "gray64                    " ) )
//   ( tRGBtxt( 163, 163, 163,      "grey64                    " ) )
//   ( tRGBtxt( 166, 166, 166,      "gray65                    " ) )
//   ( tRGBtxt( 166, 166, 166,      "grey65                    " ) )
//   ( tRGBtxt( 168, 168, 168,      "gray66                    " ) )
//   ( tRGBtxt( 168, 168, 168,      "grey66                    " ) )
//   ( tRGBtxt( 171, 171, 171,      "gray67                    " ) )
//   ( tRGBtxt( 171, 171, 171,      "grey67                    " ) )
//   ( tRGBtxt( 173, 173, 173,      "gray68                    " ) )
//   ( tRGBtxt( 173, 173, 173,      "grey68                    " ) )
//   ( tRGBtxt( 176, 176, 176,      "gray69                    " ) )
//   ( tRGBtxt( 176, 176, 176,      "grey69                    " ) )
//   ( tRGBtxt( 179, 179, 179,      "gray70                    " ) )
//   ( tRGBtxt( 179, 179, 179,      "grey70                    " ) )
//   ( tRGBtxt( 181, 181, 181,      "gray71                    " ) )
//   ( tRGBtxt( 181, 181, 181,      "grey71                    " ) )
//   ( tRGBtxt( 184, 184, 184,      "gray72                    " ) )
//   ( tRGBtxt( 184, 184, 184,      "grey72                    " ) )
//   ( tRGBtxt( 186, 186, 186,      "gray73                    " ) )
//   ( tRGBtxt( 186, 186, 186,      "grey73                    " ) )
//   ( tRGBtxt( 189, 189, 189,      "gray74                    " ) )
//   ( tRGBtxt( 189, 189, 189,      "grey74                    " ) )
//   ( tRGBtxt( 191, 191, 191,      "gray75                    " ) )
//   ( tRGBtxt( 191, 191, 191,      "grey75                    " ) )
//   ( tRGBtxt( 194, 194, 194,      "gray76                    " ) )
//   ( tRGBtxt( 194, 194, 194,      "grey76                    " ) )
//   ( tRGBtxt( 196, 196, 196,      "gray77                    " ) )
//   ( tRGBtxt( 196, 196, 196,      "grey77                    " ) )
//   ( tRGBtxt( 199, 199, 199,      "gray78                    " ) )
//   ( tRGBtxt( 199, 199, 199,      "grey78                    " ) )
//   ( tRGBtxt( 201, 201, 201,      "gray79                    " ) )
//   ( tRGBtxt( 201, 201, 201,      "grey79                    " ) )
//   ( tRGBtxt( 204, 204, 204,      "gray80                    " ) )
//   ( tRGBtxt( 204, 204, 204,      "grey80                    " ) )
//   ( tRGBtxt( 207, 207, 207,      "gray81                    " ) )
//   ( tRGBtxt( 207, 207, 207,      "grey81                    " ) )
//   ( tRGBtxt( 209, 209, 209,      "gray82                    " ) )
//   ( tRGBtxt( 209, 209, 209,      "grey82                    " ) )
//   ( tRGBtxt( 212, 212, 212,      "gray83                    " ) )
//   ( tRGBtxt( 212, 212, 212,      "grey83                    " ) )
//   ( tRGBtxt( 214, 214, 214,      "gray84                    " ) )
//   ( tRGBtxt( 214, 214, 214,      "grey84                    " ) )
//   ( tRGBtxt( 217, 217, 217,      "gray85                    " ) )
//   ( tRGBtxt( 217, 217, 217,      "grey85                    " ) )
//   ( tRGBtxt( 219, 219, 219,      "gray86                    " ) )
//   ( tRGBtxt( 219, 219, 219,      "grey86                    " ) )
//   ( tRGBtxt( 222, 222, 222,      "gray87                    " ) )
//   ( tRGBtxt( 222, 222, 222,      "grey87                    " ) )
//   ( tRGBtxt( 224, 224, 224,      "gray88                    " ) )
//   ( tRGBtxt( 224, 224, 224,      "grey88                    " ) )
//   ( tRGBtxt( 227, 227, 227,      "gray89                    " ) )
//   ( tRGBtxt( 227, 227, 227,      "grey89                    " ) )
//   ( tRGBtxt( 229, 229, 229,      "gray90                    " ) )
//   ( tRGBtxt( 229, 229, 229,      "grey90                    " ) )
//   ( tRGBtxt( 232, 232, 232,      "gray91                    " ) )
//   ( tRGBtxt( 232, 232, 232,      "grey91                    " ) )
//   ( tRGBtxt( 235, 235, 235,      "gray92                    " ) )
//   ( tRGBtxt( 235, 235, 235,      "grey92                    " ) )
//   ( tRGBtxt( 237, 237, 237,      "gray93                    " ) )
//   ( tRGBtxt( 237, 237, 237,      "grey93                    " ) )
//   ( tRGBtxt( 240, 240, 240,      "gray94                    " ) )
//   ( tRGBtxt( 240, 240, 240,      "grey94                    " ) )
//   ( tRGBtxt( 242, 242, 242,      "gray95                    " ) )
//   ( tRGBtxt( 242, 242, 242,      "grey95                    " ) )
//   ( tRGBtxt( 245, 245, 245,      "gray96                    " ) )
//   ( tRGBtxt( 245, 245, 245,      "grey96                    " ) )
//   ( tRGBtxt( 247, 247, 247,      "gray97                    " ) )
//   ( tRGBtxt( 247, 247, 247,      "grey97                    " ) )
//   ( tRGBtxt( 250, 250, 250,      "gray98                    " ) )
//   ( tRGBtxt( 250, 250, 250,      "grey98                    " ) )
//   ( tRGBtxt( 252, 252, 252,      "gray99                    " ) )
//   ( tRGBtxt( 252, 252, 252,      "grey99                    " ) )
//   ( tRGBtxt( 255, 255, 255,      "gray100                   " ) )
//   ( tRGBtxt( 255, 255, 255,      "grey100                   " ) )
//   ( tRGBtxt( 169, 169, 169,      "dark grey                 " ) )
//   ( tRGBtxt( 169, 169, 169,      "DarkGrey                  " ) )
//   ( tRGBtxt( 169, 169, 169,      "dark gray                 " ) )
//   ( tRGBtxt( 169, 169, 169,      "DarkGray                  " ) )
//   ( tRGBtxt( 0  ,   0, 139,      "dark blue                 " ) )
//   ( tRGBtxt( 0  ,   0, 139,      "DarkBlue                  " ) )
//   ( tRGBtxt( 0  , 139, 139,      "dark cyan                 " ) )
//   ( tRGBtxt( 0  , 139, 139,      "DarkCyan                  " ) )
//   ( tRGBtxt( 139,   0, 139,      "dark magenta              " ) )
//   ( tRGBtxt( 139,   0, 139,      "DarkMagenta               " ) )
//   ( tRGBtxt( 139,   0,   0,      "dark red                  " ) )
//   ( tRGBtxt( 139,   0,   0,      "DarkRed                   " ) )
//   ( tRGBtxt( 144, 238, 144,      "light green               " ) )
//   ( tRGBtxt( 144, 238, 144,      "LightGreen                " ) )

//   ;



//#endif

//} //end of nd rlf

////EOF

