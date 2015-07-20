/* --------------------------------------------------------------------------
Copyright 2012 by Richard Albrecht
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

/*! \file rimg_lookuptable.h
\brief look up tables

*/




#ifndef RIMG_LookupTableH
#define RIMG_LookupTableH

#include <boost/lexical_cast.hpp>

#include <string>
#include <vector>


//#include "rimg_typedefs.h"
#include "rimg_rgb_bgra_types.h"
#include "rimg_rw_exceptions.h"

using std::vector;





namespace rlf {




   namespace rimg {

      /*! \brief Brief description.
       *         Brief description continued.
       *
       *  Detailed description starts here.
       */

      /*! \class Test
          \brief A test class.

          A more detailed class description.
      */
      class tLUT {

      public:
         tLUT();
         tLUT( vector<uint8_t> const& l );
         ~tLUT();

         tLUT& operator=( const tLUT& in );
         tLUT( const tLUT& );

         vector<uint8_t> const& operator()()const;
         uint8_t& operator[]( size_t i );
         uint8_t operator[]( size_t i )const;



         static tLUT Linearize();                   // linaerize intern lut
         static tLUT Ramp();
         static tLUT Glowscale();
         static tLUT Green();
         static tLUT Spectrum();


         static tLUT Gray();
         static tLUT Log() ;
         static tLUT Exp() ;
         static tLUT Invert( tLUT l );
         static tLUT Negate( tLUT l );

         // < thre --> 0,  >= thre --> color
         static tLUT Threshold( size_t thre, size_t color = max_byte );

         // < thre --> 0,  >= thre --> ramp
         static tLUT ThresholdRamp( size_t thre );

         // < thre --> ramp,  >= thre --> 255
         static tLUT ThresholdRampHigh( size_t thre );

         // val = i * gain + level
         // clipped to 0..255
         static tLUT GainLevel( double gain, double level );


         static tLUT Scale( uint32_t scale ) ;

         static tLUT Multiplicate( uint32_t factor ) ;

         size_t size()const {
            return _lut.size();
         }
      private:
         vector<uint8_t> _lut;
      };


      class incrementRGB {
         tRGB val;
      public:
         incrementRGB(): val() {}
         incrementRGB& operator++() {
            val.r()++;
            val.g()++;
            val.b()++;
            return *this;
         }
         tRGB operator()() {
            return val;
         }
         operator tRGB() {
            return val;
         }
         operator string() {
            return "incrementRGB";
         }

      };

      class tPalette {
         vector<tRGB> lut;
      public:
         tPalette(): lut( 256 ) {
            boost::iota( lut, incrementRGB() );
         }
         ~tPalette() {}
         tRGB& operator[]( size_t index ) {
            if( index >= lut.size() ) {
               throw tImgIndexEx( "index out of range, is: "
                                  + boost::lexical_cast<string>( index )
                                  + "' max: "
                                  + boost::lexical_cast<string>( 256 ) );
            }

            return lut[index];
         }
         tRGB operator[]( size_t index )const {
            return lut[index];

            if( index >= lut.size() ) {
               throw tImgIndexEx( "index out of range, is: "
                                  + boost::lexical_cast<string>( index )
                                  + "' max: "
                                  + boost::lexical_cast<string>( 256 ) );
            }
         }
      };


      class cRGB_channel {
         bool red;
         bool green;
         bool blue;

      public:
         cRGB_channel()
            : red( true ), green( true ), blue( true )  {
         }
         cRGB_channel( bool r, bool g, bool b ):
            red( r ), green( g ), blue( b ) {}
         bool R()const {
            return red;
         }
         bool G()const {
            return green;
         }
         bool B()const {
            return blue;
         }
         bool IsR()const {
            if( red && !green && !blue ) {
               return true;
            }

            return false;
         }
         bool IsG()const {
            if( !red && green && !blue ) {
               return true;
            }

            return false;
         }
         bool IsB()const {
            if( !red && !green && blue ) {
               return true;
            }

            return false;
         }
         bool IsRG()const {
            if( red && green && !blue ) {
               return true;
            }

            return false;
         }
         bool IsRB()const {
            if( red && !green && blue ) {
               return true;
            }

            return false;
         }

         bool IsGB()const {
            if( !red && green && blue ) {
               return true;
            }

            return false;
         }
         bool IsRGB()const {
            if( red && green && blue ) {
               return true;
            }

            return false;
         }
         void Set( bool r, bool g, bool b ) {
            red   = r;
            green = g;
            blue  = b;
         }


      };

      typedef long RGBAlphaBmpOrder;
      const size_t RGBA_BLUE  = 0;
      const size_t RGBA_GREEN = 1;
      const size_t RGBA_RED = 2;
      const size_t RGBA_ALPHA = 3;

      class cRGBAlpha {


         uint8_t x;
         uint8_t y;
         uint8_t z;
         uint8_t a;
         cRGBAlpha& operator+=( const cRGBAlpha& s );
         cRGBAlpha& operator-=( const cRGBAlpha& s );
         cRGBAlpha& operator*=( int d );

      public:

         cRGBAlpha(): x( 0 ), y( 0 ), z( 0 ), a( 255 ) {}
         cRGBAlpha( uint8_t r, uint8_t g, uint8_t b, uint8_t ia = 0 ): x( r ), y( g ), z( b ), a( ia ) {}

         bool operator==( const cRGBAlpha& s )const {
            return s.x == x && s.y == y && s.z == z && s.a == a;
         }

         bool operator!=( const cRGBAlpha& s )const {
            return !( s == *this );
         }

         RGBAlphaBmpOrder ToRGBAlphaBmpOrder() {
            RGBAlphaBmpOrder p = 0;
            uint8_t* bp = reinterpret_cast<uint8_t*>(&p);
            *( bp + RGBA_BLUE ) = z;
            *( bp + RGBA_GREEN ) = y;
            *( bp + RGBA_RED ) = x;
            *( bp + RGBA_ALPHA ) = a;
            return p;
         }

         uint8_t R()const {
            return x;
         }
         uint8_t G()const {
            return y;
         }
         uint8_t B()const {
            return z;
         }
         uint8_t Alpha()const {
            return a;
         }
         cRGBAlpha& R( size_t i ) {
            x  = static_cast<uint8_t>(i);
            return *this;
         }
         cRGBAlpha& G( size_t i ) {
            y  = static_cast<uint8_t>(i);
            return *this;
         }
         cRGBAlpha& B( size_t i ) {
            z  = static_cast<uint8_t>(i);
            return *this;
         }
         cRGBAlpha& Alpha( size_t i ) {
            a  = static_cast<uint8_t>(i);
            return *this;
         }
         cRGBAlpha& AddX( size_t i ) {
            x = static_cast<uint8_t>( x + i );
            return *this;
         }
         cRGBAlpha& AddY( size_t i ) {
            y = static_cast<uint8_t>( y + i );
            return *this;
         }
         cRGBAlpha& AddZ( size_t i ) {
            z = static_cast<uint8_t>( z + i );
            return *this;
         }
         cRGBAlpha& AddAlpha( size_t i ) {
            a = static_cast<uint8_t>( a + i );
            return *this;
         }
      };

      class c_RGBLut {

      public:


         static bool Write( string const& file, std::vector<cRGBAlpha> const& l );    // write to disk
         static std::vector<cRGBAlpha> Read( string const& file );          // read from disk
         static std::vector<cRGBAlpha> ReadHex( string const& file );       // read photoshop format
         static bool WriteHex( string const& file, std::vector<cRGBAlpha> const& l ); // write photoshop format

         static std::vector<cRGBAlpha> GlowScale();
         static std::vector<cRGBAlpha> Green();
         static std::vector<cRGBAlpha> Spectrum();
         static std::vector<cRGBAlpha> Linearize();
         static std::vector<cRGBAlpha> Gray();


         //   0..< low = blue, high..< 256 = red
         static std::vector<cRGBAlpha> Range( uint32_t low, uint32_t high );
         static std::vector<cRGBAlpha> Invert();



      };




   } // end of ns rimg

}



#endif
//EOF

