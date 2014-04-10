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

#ifndef tObjPositionSize_H
#define tObjPositionSize_H

#include <assert.h>
#include <string>

#include "t_2D_xy.h"



using rlf_minit::float_xy;


namespace rlf_minit {




   class t_pos_size {
   public:

      t_pos_size(): _pos( ), _size( ) {}
      t_pos_size( float_xy pos, float_xy size ): _pos( pos ), _size( size ) {}

      t_pos_size( const t_pos_size& in ): _pos( in._pos ), _size( in._size )  {}

      ~t_pos_size() {}

      float left() const {
         return _pos.left();
      }
      void setleft( float left_ ) {
         return _pos.setleft( left_ );
      }
      float top() const {
         return _pos.top();
      }
      void settop( float left_ ) {
         return _pos.settop( left_ );
      }

      float w() {
         return _size.w();
      }
      void setw( float w_ ) {
         return _size.setw( w_ );
      }
      float h() {
         return _size.h();
      }
      void seth( float h_ ) {
         return _size.seth( h_ );
      }

   private:
      float_xy _pos;
      float_xy  _size;
   };


} // end of ns pnt



#endif



//EOF
