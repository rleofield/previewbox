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
/*! \file rlf_bin_filter.h
\brief a filter for binary objects

*/



#ifndef BANDPASS_H
#define BANDPASS_H


#include "t_2D_xy.h"

#include "rlf_blobs.h"

namespace rlf {



   /* --
         copy >= min_area and <= max_area copied back to image with color
         all other objects     are deleted
         connected with border are deleted

         p is a binary image ( pixel != 0 is object )
         p is bit 8
   */
   uint32_t CellAreaFilter( tImgPlanar& p, size_t min_area, size_t max_area, uint8_t color = 255 );


   /* --
         copy > min_area and < max_area copied back to image with color
         all other objects     are deleted
         connected with border are not deleted
         connected with border and <= size_to_remove are deleted
         connected with border and > max_size_to_remove remains in image

         p is a binary image ( pixel != 0 is object )
         p is bit 8
   */
   uint32_t CellAreaFilter( tImgPlanar& p, size_t min, size_t max, uint8_t color,
                            size_t max_size_to_remove );







}
#endif
//EOF



