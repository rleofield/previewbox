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

/*! \file timer.cpp
\brief impl for read and write test raw images

*/

#include <iostream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <mutex>

#include "timer.h"

using namespace std;

namespace rlf_htime {
   static std::mutex local_mutex;

#define HAS_STD_PUT_TIME 0


   class tTimerImpl {
   public:

      tTimerImpl(): chrono_start( chrono::high_resolution_clock::now() ), chrono_end() {
      }


      ~tTimerImpl() {}

      int64_t elapsed() {
         chrono_end = chrono::high_resolution_clock::now();
         return chrono::duration_cast<chrono::nanoseconds>
                ( chrono_end - chrono_start ).count();

      }
      static string now1( string format = "%H:%M:%S." ) ;

   private:
      chrono::time_point<chrono::high_resolution_clock> chrono_start;
      chrono::time_point<chrono::high_resolution_clock> chrono_end;
   };



   std::string tTimerImpl::now1( string format ) {

      std::stringstream now;

      auto tp = std::chrono::system_clock::now();
      auto ms = std::chrono::duration_cast<std::chrono::milliseconds>( tp.time_since_epoch() );
      size_t modulo = ms.count() % 1000;

      time_t seconds = std::chrono::duration_cast<std::chrono::seconds>( ms ).count();
#if HAS_STD_PUT_TIME
      now << std::put_time( std::localtime( &seconds ), "%T." );
#else

      char buffer[80]; // holds "21:31:42"

      // localtime() is not threadsafe
      std::lock_guard<std::mutex> lock( local_mutex );
      struct tm* today = localtime( &seconds );

      if( strftime( buffer, 80, format.c_str(), localtime( &seconds ) ) ) {
         now << buffer;
      }


#endif // HAS_STD_PUT_TIME

      // ms
//      now.fill( '0' );
//      now.width( 3 );
//      now << modulo;

      return now.str();
   }



   tTimer::tTimer(): impl( new tTimerImpl() ) {}
   tTimer::~tTimer() {
      delete impl;
   }

   std::string tTimer::now( string format ) {
      return tTimerImpl::now1( format );

   }

   double tTimer::nanos()const {
      int64_t v = impl->elapsed();
      return ( double )v;
   }
   double tTimer::microns()const {
      double v = nanos();
      return ( double )v / 1000.0;
   }

   double tTimer::millies()const {
      double m = microns();
      return ( double )m / 1000.0;
   }



   double tTimer::seconds()const {
      double d = millies();
      return d / 1000.0;
   }

} // end of namespace rlf_htime

//EOF

