
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


/*! \file rimg_planar.h
    \brief planar images

    Lib: librimgrw
*/


#ifndef RIMG_tImgPlaneLinearH
#define RIMG_tImgPlaneLinearH


#include <cstdint>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "t_2D_xy.h"

//#include "rimg_typedefs.h"
#include "rimg_rgb_bgra_types.h"
#include "rimg_rw_exceptions.h"
#include "rimg_imgsize_type.h"
//#include "rimg_planar_mask.h"
//#include "rimg_alloc_raw_data.h"

using std::vector;



using rlf::tSize;
using rlf::tRGB;
using rlf_minit::size_t_xy;

namespace rlf {


   namespace rimg_planar {

      template<typename T>
      class tPlaneLinear {
      private:
         vector<T>    _data;
         size_t_xy  _size;
      public:
         tPlaneLinear(): _data(), _size() {}
         ~tPlaneLinear() {}
         size_t_xy size()const {
            return _size;
         }
         size_t_xy& size() {
            return _size;
         }
         tPlaneLinear( tPlaneLinear const& imgIn ):
            _data( imgIn._data ),
            _size( imgIn._size ) {}

         tPlaneLinear& operator=( tPlaneLinear const& imgIn ) {
            if( this != & imgIn ) {
               _size = imgIn._size;
               _data = imgIn._data;
               _data.shrink_to_fit();
            }

            return *this;
         }

         size_t pitch()const {
            return align32( size().x() );
         }

         void resize() {
            _data.resize( _size.y() * pitch() );
            _data.shrink_to_fit();
         }

         size_t bytes()const {
            return size().y() * pitch();
         }

         void clear( T val = 0 ) {
            if( _data != 0 ) {
               memset( _data, val , bytes() );
            }
         }
         T pixel( size_t y, size_t x )const {
            return _data[y * pitch() + x];
         }
         void pixel( size_t y, size_t x, size_t v ) {
            _data[y * pitch() + x] = v;
         }

         typedef typename vector<T>::const_iterator T_const_iterator;
         typedef typename vector<T>::iterator T_iterator;
         void insert_data( T_const_iterator  begin, T_const_iterator  end, size_t offset ) {
            ptrdiff_t s = end - begin;

            if( s > 0 && s + offset <= bytes() ) {
               T_iterator pdata = _data.begin() + offset;

               while( begin != end ) {
                  *pdata++ = *begin++;
               }
            }

         }
         T_const_iterator iterator( size_t offset ) {
            return _data.begin() + offset;
         }

         T operator[]( size_t index )const {
            return *( _data.begin() + index ) ;
         }

         void insert_data( vector<T> const& v, size_t offset ) {
            ptrdiff_t s = v.size();

            if( s > 0 && s + offset <= bytes() ) {
               T_iterator pdata = _data.begin() + offset;
               T_const_iterator vbegin = v.begin();
               T_const_iterator vend = v.end();

               while( vbegin != vend ) {
                  *pdata++ = *vbegin++;
               }
            }

         }

      protected:
         vector<T> const&   get_data()const {
            return _data;
         }
         vector<T> &  get_data() {
            return _data;
         }

      };

      typedef tPlaneLinear<uint8_t> tPlaneLinear8;



   } // end of ns rimg_planar

}    // end of ns rlf



#endif
//EOF

