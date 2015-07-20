/* --------------------------------------------------------------------------
    Copyright 2013 by Richard Albrecht
    richard.albrecht@rleofield.de

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

/*! \file rlf_bin_filter.cpp
\brief remove blobs with a filter


*/




#include <assert.h>
#include "rlf_blobs.h"
#include "rimg_math.h"

#include "rlf_bin_filter.h"

namespace rlf {

   // ObjectFilter( );
   // RemoveObjectsOutOfSizeInterval();
   uint32_t CellAreaFilter( tImgPlanar& bin, uint32_t min, uint32_t max, uint8_t color ) {

      assert( bin.is_mono8() );

      tBlobs ObjectList;

      uint32_t thre = 1;
      ObjectList.threshold() = thre;
      ObjectList.extract( bin ); // thre >= 1, all objects color >= thre

      uint32_t size = ObjectList.count();

      // write back with color

      rlf::rimg_math::rimginterface r;
      r.fill( bin, 0 );
      uint32_t count = 0;

      for( uint32_t i = 0; i < size; i++ ) {
         tBlob& bo = ObjectList[i];
         bo.calculateArea();
         size_t area = bo.area();

         if( area >= min  &&  area <=  max &&
               bo.ConnectedWithImageBorder( bin.size() ) == false ) {

            count++;
            WriteToImage( bo.streaks(), bin, color );
         }


      }

      return count;
   }

   uint32_t CellAreaFilter( tImgPlanar& bin, uint32_t min, uint32_t max, uint8_t color,
                            size_t max_size_to_remove ) {

      assert( bin.is_mono8() );

      tBlobs ObjectList;
      uint32_t thre = 1;
      ObjectList.threshold() = thre;
      ObjectList.extract( bin ); // thre >= 1, all objects color>= thre

      size_t size = ObjectList.count();

      // write back with color
      // vector<tBlob>::iterator iBegin  = ObjectList.begin();
      // vector<tBlob>::iterator iEnd    = ObjectList.end();
      uint32_t count = 0;

      rlf::rimg_math::rimginterface r;
      r.fill( bin, 0 );


      for( uint32_t i = 0; i < size; i++ ) {

         tBlob& bo = ObjectList[i];
         bo.calculateArea();
         size_t area = bo.area();

         if( area >= min  &&  area <=  max ) {
            if( bo.ConnectedWithImageBorder( bin.size() ) == false
                  || ( area > static_cast<uint32_t>( max_size_to_remove) ) ) {
               count++;
               WriteToImage( bo.streaks(), bin, color );
            }
         }
      }

      return count;
   }

} // end of namespace

//EOF
