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
File: cRGB_channel.h $
*/

#ifndef CRGB_CHANNEL_H
#define CRGB_CHANNEL_H


namespace rlf {
   namespace nImg {



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



   } // end of namespace nImg
} // end of namespace rlf

#endif

//EOF
