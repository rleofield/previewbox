
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

/*! \file rimg_binedit.cpp
\brief math functions for blobs

*/

/*
Workfile: rimg_binedit.cpp
*/

#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>
#include <iostream>
#include <math.h>

#include "t_2D_xy.h"

#include "rimg_imgsize_type.h"

#include "rimg_math.h"
#include "timer.h"
#include "str_util.h"

using namespace std;




namespace rlf {


   namespace rimg_math {


      /* logigal || of all 8 neighbours                                     */

      /* min 8 pixel in Umgebung == pixel bleibt stehen                     */
      uint8_t lut8[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
         /*                    */
         /*0                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*1                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*2                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*3                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*4                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*5                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*6                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*7                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*8                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*9                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*A                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*B                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*C                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*D                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*E                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*F                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1
      };

      /* min 7 pixel in Umgebung == pixel bleibt stehen                     */
      uint8_t lut7[256] = {
         //    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
         //  0
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         //  1
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*2                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*3                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*4                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*5                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*6                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*7                   */
         //                                                          7f
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*8                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*9                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*A                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*B                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*C                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*D                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*E                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*F                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1
      };


      /* min 6 pixel in Umgebung == pixel bleibt stehen                     */
      uint8_t lut6[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
         /*                    */
         /*0                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*1                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*2                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*3                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*4                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*5                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*6                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*7                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*8                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*9                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*A                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*B                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*C                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*D                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*E                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*F                   */
         /*                    */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1
      };



      /* min 5 pixel in Umgebung == pixel bleibt stehen                     */
      uint8_t lut5[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F   */
         /*                    */
         /*0                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

         /*                    */
         /*1                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*2                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*3                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*4                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*5                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*6                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*7                   */
         /*                    */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                    */
         /*8                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                    */
         /*9                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*A                   */
         /*                    */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                    */
         /*B                   */
         /*                    */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*C                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                     */
         /*D                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*E                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*F                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };


      /* min 4 pixel in Umgebung == pixel bleibt stehen                      */
      uint8_t lut4[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F    */
         /*                     */
         /*0                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,

         /*                     */
         /*1                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  0,  1,

         /*                     */
         /*2                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                     */
         /*3                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*4                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                     */
         /*5                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*6                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*7                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*8                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                     */
         /*9                    */
         /*                     */
         0,  0,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*A                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*B                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*C                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*D                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*E                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*F                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };

      /* min 3 pixel in Umgebung == pixel bleibt stehen                      */
      uint8_t lut3[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F    */
         /*                     */
         /*0                    */
         /*                     */
         0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  1,

         /*                     */
         /*1                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*2                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*3                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*4                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*5                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*6                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*7                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*8                    */
         /*                     */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*9                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*A                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*B                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*C                    */
         /*                     */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*D                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*E                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                     */
         /*F                    */
         /*                     */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };


      /* min 2 pixel in Umgebung == pixel bleibt stehen                       */
      uint8_t lut2[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F     */
         /*                      */
         /*0                     */
         /*                      */
         0,  0,  0,  1,  0,  1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*1                     */
         /*                      */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*2                     */
         /*                      */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*3                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*4                     */
         /*                      */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*5                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*6                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*7                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*8                     */
         /*                      */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*9                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*A                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*B                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*C                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*D                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*E                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*F                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };


      /* min 1 pixel in Umgebung == pixel bleibt stehen                       */
      uint8_t lut1[256] = {
         /*    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F     */
         /*                      */
         /*0                     */
         /*                      */
         0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*1                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*2                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*3                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*4                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*5                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*6                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*7                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*8                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*9                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*A                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*B                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*C                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*D                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*E                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

         /*                      */
         /*F                     */
         /*                      */
         1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };



      /*
      muster
      0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F

      0

      0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      1

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      2

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      3

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      4

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      5

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      6

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      7

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      8

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      9

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      A

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      B

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      C

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      D

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      E

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,


      F

      1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
      };

      */

   } // end of ns math

} // end of namespace rlf

//EOF





