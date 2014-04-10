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

#ifndef tLine_XY_H
#define tLine_XY_H

#include <assert.h>
#include <string>

#include "t_2D_xy.h"

namespace rlf_minit {



   /**
    *  A start and an end point will be handled as a line
    */
   template <typename T>
   class tLine {
   public:
      tLine(): begin(), end() {}
      tLine( tLine const& l ): begin( l.begin ), end( l.end ) {}
      tLine( xy_t<T> const& begin_, xy_t<T> const& end_ ): begin( begin_ ), end( end_ ) {}
      tLine& operator=( tLine const& l ) {
         if( this != &l ) {
            begin = l.begin;
            end = l.end;
         }

         return *this;
      }
      xy_t<T> begin;
      xy_t<T> end;

      xy_t<T> center()const {
         return ( begin + end ) / ( T )2.0;
      }
      double length()const {
         xy_t<T> d = end - begin;
         return sqrt( static_cast<double>( d.x() * d.x() + d.y() * d.y() ) );
      }
      double angle() const {
         xy_t<T> d = end - begin;
         return atan2( static_cast<double>( d.y() ), static_cast<double>( d.x() ) );
      }
      T dx()const {
         xy_t<T> d = end - begin;
         return d.x();
      }
      T dy()const {
         xy_t<T> d = end - begin;
         return d.y();
      }


   };






   typedef tLine<double> double_line;
   typedef tLine<float> float_line;

} // end of ns pnt







#endif



//EOF
