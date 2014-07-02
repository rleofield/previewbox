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

/*! \file rimg_timer.h
\brief a small timer

*/



#ifndef RIMG_TIMER_H
#define RIMG_TIMER_H

#include <chrono>
#include <string>


namespace rlf_htime {
   class tTimerImpl;

   class tTimer {
      tTimer( tTimer const& );
      void operator=( tTimer const& );

   public:
      tTimer();
      ~tTimer();
      double seconds()const;
      double millies()const;
      double microns()const;
      double nanos()const;
      static std::string now( std::string format = "%H:%M:%S." ) ;

   private:
      tTimerImpl* const impl;
   };
}
#endif
//EOF
