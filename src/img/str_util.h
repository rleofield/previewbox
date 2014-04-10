/* --------------------------------------------------------------------------
    Copyright 2012 by Richard Albrecht
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

/*! \file str_util.h
\brief blob analyse

*/

#ifndef STR_UTIL_H
#define STR_UTIL_H


#include <string>
#include <sstream>
#include <iomanip>

#include <typeinfo>
#include <stdexcept>
#include <vector>
#include <boost/lexical_cast.hpp>

//#include "rimg_typedefs.h"

using namespace std;

namespace rlf {
   namespace str {



      /*! BadConversion,

         Exception, wenn eine Konvertierung mit 'toString()' nicht erfolgreich war
         \param [in] msg  Fehlermeldung
         */
      class BadConversion : public runtime_error {
      public:
         BadConversion( const string& msg )
            : runtime_error( msg ) { }
      };


      /*! Konvertiert einen Wert in einen String, Spezialisierung mit 'double'.

           \param val double Wert, der konvertiert werden soll
           \param w   Ausgabebreite, default = 6
           \param prec Gleitkommaformat, default: size = 6 and precision = 3"
         */
      inline string toString( double val, size_t w = 6, size_t prec = 3 ) {
         ostringstream o;
         o << setw( w ) << fixed << setprecision( prec ) << right << setfill( ' ' ) << val ;
         return  o.str();
      }
      inline string toString( int val, size_t w = 6 ) {
         string v = boost::lexical_cast<string>( val );
         ostringstream o;
         o << setw( w ) << right << setfill( ' ' ) << val ;
         return  o.str();
      }
      inline string toString( uint32_t val, size_t w = 6 ) {
         string v = boost::lexical_cast<string>( val );
         ostringstream o;
         o << setw( w ) << right << setfill( ' ' ) << val ;
         return  o.str();
      }




      /*! Konvertiert einen String in einen Wert

        Der Template Parameter 'T' gibt den Typ, in den der String konvertiert werden soll, an.

           \param s String, der konvertiert werden soll
         */
      template<class T>
      inline T string2type( string const& s )  {
         T x;
         istringstream insx( s );

         //insx >> x;
         if( !( insx >> x ) ) {
            throw BadConversion( string( "toString(" ) + typeid( s ).name() + ")" );
         }

         return x;
      }



   } // end ns str
}




#endif // 

//EOF
