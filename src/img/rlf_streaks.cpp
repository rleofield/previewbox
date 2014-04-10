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

/*! \file rlf_streaks.cpp
\brief impl for streaks in images


*/



#include <assert.h>
#include <memory.h>
#include <numeric>

#include "t_2D_xy.h"
//#include "rimg_xy_types.h"
#include "rlf_streak.h"


namespace rlf {

   /* -- OPERATORS --
   */
   tStreak tStreak::Create( uint32_xy const& xy ) {
      return tStreak( xy );
   }


}
//EOF

