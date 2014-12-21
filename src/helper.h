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
Source:  elsamuko
*/



#ifndef HELPER_H
#define HELPER_H

#pragma once

using std::string;

namespace rlf {
   class NonCopyable {
      NonCopyable( const NonCopyable& );
      const NonCopyable&  operator= ( const NonCopyable& );
   protected:
      NonCopyable( ) { }
      virtual ~NonCopyable( ) { }
   };

}

namespace helper_read_write_file {

   inline string FindAndReplace( const string& source,
                                 const string& find,
                                 const string& replace ) {
      size_t i;
      size_t start = 0;
      string ret = source;

      for( ; ( i = ret.find( find, start ) ) != std::string::npos; ) {
         ret.replace( i, find.length(), replace );
         start = i + replace.length();
      }

      return ret;
   }

   const std::string marker = "%s";
   inline std::string replace( std::string const& msg, std::string const& s0 = "" ) {

      if( s0.size() > 0 ) {
         return FindAndReplace( msg, marker, s0 );
      }

      return msg;

   }



   inline char* toCharPtr( std::vector<uint8_t>& b ) {
      return reinterpret_cast<char* >( static_cast<uint8_t* >( &b[0] ) );
   }
   inline char const* toCharPtr( std::vector<uint8_t> const& b ) {
      return reinterpret_cast<char const* >( static_cast<uint8_t const* >( &b[0] ) );
   }


}

#endif // HELPER_H
