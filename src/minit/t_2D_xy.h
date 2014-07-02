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

#ifndef t_2D_xy_H
#define t_2D_xy_H

#include <algorithm>
#include <assert.h>
#include <string>
#include <float.h>
#include <vector>
#include <cmath>
#include <cstdint>


namespace rlf_minit {

   const float loc_FLT_EPSILON = FLT_EPSILON * 10.0f;
   const double loc_DBL_EPSILON = DBL_EPSILON * 10.0f;

   /**
   *  2D xy pairsrlf_xy
   */

   template <typename T>
   class xy_t {
      T _x;
      T _y;
   public:
      xy_t(): _x( 0 ), _y( 0 ) {}
      xy_t( T const& x_, T const& y_ ): _x( x_ ), _y( y_ ) {}
      explicit xy_t( T const& val_ ): _x( val_ ), _y( val_ ) {}
      xy_t( xy_t const& t ): _x( t._x ), _y( t._y ) {}

      ~xy_t() {}

      xy_t& operator=( xy_t const& t ) {
         if( this != &t ) {
            _x = t._x;
            _y = t._y;
         }

         return *this;
      }

      operator xy_t<float> ()const {
         return xy_t<float>( static_cast<float>( _x ), static_cast<float>( _y ) );
      }
      operator xy_t<double> ()const {
         return xy_t<double>( static_cast<double>( _x ), static_cast<double>( _y ) );
      }



      // as x and y
      void setx( T const& x_ ) {
         _x = x_;
      }
      void sety( T const& y_ ) {
         _y = y_;
      }
      T const& x()const {
         return _x;
      }
      T const& y()const {
         return _y;
      }
      T& refx() {
         return _x;
      }
      T& refy() {
         return _y;
      }

      // as w and h
      void setw( T const& w_ ) {
         _x = w_;
      }
      void seth( T const& h_ ) {
         _y = h_;
      }
      T const& w()const {
         return _x;
      }
      T const& h()const {
         return _y;
      }

      // as left/top
      void setleft( T const& left_ ) {
         _x = left_;
      }
      void settop( T const& top_ ) {
         _y = top_;
      }
      T const& left()const {
         return _x;
      }
      T const& top()const {
         return _y;
      }

      void operator+=( xy_t const& t ) {
         _x += t._x;
         _y += t._y;
      }
      void operator+=( T const& val ) {
         _x += val;
         _y += val;
      }
      void addX( T val ) {
         _x += val;
      }
      void addY( T val ) {
         _y += val;
      }
      void operator-=( xy_t const& t ) {
         _x -= t._x;
         _y -= t._y;
      }
      void operator-=( T const& val ) {
         _x -= val;
         _y -= val;
      }
      void operator*=( xy_t const& t ) {
         _x *= t._x;
         _y *= t._y;
      }
      void operator*=( T const& t ) {
         _x *= t;
         _y *= t;
      }
      void operator/=( xy_t const& t ) {
         _x /= t._x;
         _y /= t._y;
      }
      void operator/=( T const& t ) {
         _x /= t;
         _y /= t;
      }


      // other
      bool operator==( T i )const {
         if( _x == i && _y == i ) {
            return true;
         }

         return false;
      }
      bool operator!=( T i )const {
         return !operator==( i );
      }
      bool operator==( xy_t const& t )const {
         if( _x == t._x && _y == t._y ) {
            return true;
         }

         return false;
      }
      bool operator!=( xy_t const& t )const {
         return !operator==( t );
      }

      bool operator<( T i )const {
         if( _x < i && _y < i ) {
            return true;
         }

         return false;
      }
      bool operator>( T i )const {
         if( _x > i && _y > i ) {
            return true;
         }

         return false;
      }

      bool zero()const {
         return operator==( 0 );
      }



      xy_t center()const {
         return xy_t( _x / 2, _y / 2 );
      }
      T size()const {
         return _x * _y;
      }

   };

   //   template <class T>
   //   inline vector< t_xy<T> > operator+( vector<t_xy<T> > v1, vector<t_xy<T> > v2){
   //         vector<t_xy<T> > v = v1;
   //         insert( v.end(), v2.begin(), v2.end());
   //         return v;
   //      }



   class xy_b_t {
      bool _x;
      bool _y;
   public:
      xy_b_t(): _x( false ), _y( false ) {}
      xy_b_t( bool x_, bool y_ ): _x( x_ ), _y( y_ ) {}
      xy_b_t( xy_b_t const& t ): _x( t._x ), _y( t._y ) {}
      xy_b_t( xy_t<float> const& t ):
         _x( ( t.x() > 0.0f ) ? true : false ),
         _y( ( t.y() > 0.0f ) ? true : false ) {}
      xy_b_t( xy_t<double> const& t ):
         _x( ( t.x() > 0.0 ) ? true : false ),
         _y( ( t.y() > 0.0 ) ? true : false ) {}

      ~xy_b_t() {}

      xy_b_t& operator=( xy_b_t t ) {
         if( this != &t ) {
            _x = t._x;
            _y = t._y;
         }

         return *this;
      }

      // as x and y
      void sethor( bool x_ ) {
         _x = x_;
      }
      void setver( bool y_ ) {
         _y = y_;
      }
      bool x()const {
         return _x;
      }
      bool y()const {
         return _y;
      }
      bool hor()const {
         return _x;
      }
      bool ver()const {
         return _y;
      }

      // other
      bool operator==( xy_b_t t )const {
         if( _x == t._x && _y == t._y ) {
            return true;
         }

         return false;
      }
      bool operator!=( xy_b_t t )const {
         return !operator==( t );
      }
   };



   template <class T, class T2>
   inline xy_t<T> to_T_xy( T2 const& x, T2 const& y ) {
      return xy_t<T>( static_cast<T>( x ), static_cast<T>( y ) );
   }


   template <class T>
   inline bool operator==( xy_t<T> const& left, T const& right ) {
      if( left.x() == right && left.y() == right ) {
         return true;
      }

      return false;
   }

   template <class T>
   inline xy_t<T> operator+( xy_t<T> const& left, xy_t<T> const& right ) {
      xy_t<T> temp( left );
      temp += right;
      return temp;
   }
   template <class T>
   inline xy_t<T> operator+( xy_t<T> const& left, T const& right ) {
      xy_t<T> temp( left );
      temp +=  xy_t<T> ( right, right );
      return temp;
   }
   template <class T>
   inline xy_t<T> operator-( xy_t<T> const& left, xy_t<T> const& right ) {
      xy_t<T> temp( left );
      temp -= right;
      return temp;
   }
   template <class T>
   inline xy_t<T> operator-( xy_t<T> const& left, T const& a ) {
      xy_t<T> temp( left );
      temp -=  xy_t<T> ( a, a );
      return temp;
   }

   template <class T>
   inline xy_t<T> operator*( xy_t<T> const& left, xy_t<T> const& right ) {
      xy_t<T> temp( left );
      temp *= right;
      return temp;
   }
   template <class T>
   inline xy_t<T> operator*( xy_t<T> const& left, T const& right ) {
      xy_t<T> temp( left );
      temp *=  xy_t<T> ( right, right );
      return temp;
   }
   template <class T>
   inline xy_t<T> operator/( xy_t<T> const& left, xy_t<T> const& right ) {
      xy_t<T> temp( left );
      temp /= right;
      return temp;
   }
   template <class T>
   inline xy_t<T> operator/( xy_t<T> const& left, T a ) {
      xy_t<T> temp( left );
      temp /= xy_t<T> ( a, a );
      return temp;
   }

   template <class T>
   inline size_t lengthsquared( xy_t<T> const& a, xy_t<T> const& b ) {
      return a.x() * b.x() + a.y() + b.y();

   }


   // const float loc_FLT_EPSILON = FLT_EPSILON * 10.0f;
   // const double loc_DBL_EPSILON = DBL_EPSILON * 10.0f;


   // see http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
   // "What Every Computer Scientist Should Know About Floating-Point Arithmetic"

   // test float/double compare, diese Vergleiche sollen so funktionieren
   // diese differenzen und kleiner ergeben true
   //float_xy xy0(1000.0,1000.0);
   //float_xy xy1(1000.001,1000.0); // == with epsilon = 1.1920929e-006

   //float_xy xy0(0.0001,0.0001);
   //float_xy xy1(0.0001000001,0.0001);  // == with epsilon = 1.1920929e-006


   //double_xy xy0(1000.0,1000.0);
   //double_xy xy1(1000.00000000000001,1000.0);    // == with epsilon =    2.2204460492503131e-015


   //double_xy xy0( 0.0001, 0.0001 );
   //double_xy xy1( 0.00010000000000000001, 0.0001 );  // == with epsilon = 2.2204460492503131e-015



   // specialization for double
   template<>
   inline bool xy_t<double>::operator==( xy_t<double> const& t )const {
      if( _x == 0.0 && _y == 0.0 && t._x == 0.0 && t._y == 0.0 ) {
         return true;
      }

      double epsilon = loc_DBL_EPSILON;
      double xdiff = fabs( _x - t._x );
      double ydiff = fabs( _y - t._y );
      double xabs = std::max<double>( fabs( _x ), fabs( t._x ) );
      double yabs = std::max<double>( fabs( _y ), fabs( t._y ) );
      double max_ = std::max<double>( xabs, yabs );

      if( max_ == 0.0 ) {
         return true;
      }

      double epsilon_max = epsilon * max_;

      // floats are identical, if diff is smaller than epsilon scaled
      if( xdiff < epsilon_max && ydiff < epsilon_max ) {
         return true;
      }

      return false;
   }
   template<>
   inline bool xy_t<double>::operator==( double t )const {
      return operator==( xy_t<double>( t, t ) );
   }
   // specialization for float
   template<>
   inline bool xy_t<float>::operator==( xy_t<float> const& t )const {
      if( _x == 0.0f && _y == 0.0f && t._x == 0.0f && t._y == 0.0f ) {
         return true;
      }

      float epsilon = loc_FLT_EPSILON;
      float xdiff = std::fabs( _x - t._x );
      float ydiff = std::fabs( _y - t._y );
      float xabs = std::max<float>( fabs( _x ), fabs( t._x ) );
      float yabs = std::max<float>( fabs( _y ), fabs( t._y ) );
      float max_ = std::max<float>( xabs, yabs );

      if( max_ == 0.0f ) {
         return true;
      }

      float epsilon_max = epsilon * max_;

      // floats are identical, if diff is smaller than epsilon scaled
      if( xdiff < epsilon_max && ydiff < epsilon_max ) {
         return true;
      }

      return false;
   }



   template<>
   inline bool xy_t<float>::operator==( float t )const {
      return operator==( xy_t<float>( t, t ) );
   }


   inline xy_t<double> center( xy_t<double> const& t ) {
      return xy_t<double>( t.x() / 2, t.y() / 2 );
   }

   typedef xy_t<double> double_xy;
   typedef xy_t<float> float_xy;
   typedef xy_t<uint16_t> uint16_xy;
   typedef xy_t<int16_t> int16_xy;
   typedef xy_t<uint32_t> uint32_xy;
   typedef xy_t<int32_t> int32_xy;
   typedef xy_t<uint64_t>  uint64_xy;
   typedef xy_t<int64_t>  int64_xy;
   typedef xy_t<size_t>  size_t_xy;



   typedef xy_b_t bool_xy;
   //typedef xy_b_t xy_b;


   const float_xy eins_xy = float_xy( 1.0f, 1.0f );
   const float_xy minusEins_xy = float_xy( -1.0f, -1.0f );

   inline float ratio( uint32_xy const& p ) {
      assert( p.h() > 0 );
      return p.w() / ( float )p.h();
   }
   inline float ratio( float_xy const& p ) {
      assert( fabs( p.h() ) > loc_FLT_EPSILON );
      return p.w() / p.h();
   }
   inline double ratio( double_xy const& p ) {
      assert( fabs( p.h() ) > loc_DBL_EPSILON );
      return p.w() / p.h();
   }

   inline float_xy operator/( uint32_xy cv, uint32_xy img ) {
      float_xy temp( ( float )cv.x(), ( float )cv.y() );
      temp /= float_xy( ( float )img.x(), ( float )img.y() );
      return temp;
   }

   inline double length( double_xy const& xy ) {
      return sqrt( xy.x() * xy.x() + xy.y() * xy.y() );
   }

   inline double distance( double_xy const& a, double_xy const& b ) {
      double dx( a.x() - b.x() );
      double dy( a.y() - b.y() );
      return sqrt( dx * dx + dy * dy );
   }



   inline uint32_xy to_uint32_xy( int32_xy const& xy ) {
      return uint32_xy( xy.x(), xy.x() );
   }
   inline int32_xy to_int32_xy( uint32_xy const& xy ) {
      return int32_xy( xy.x(), xy.y() );
   }

   inline int32_xy to_tixy( double_xy const& xy ) {
      return int32_xy( static_cast<int32_t>( xy.x() ), static_cast<int32_t>( xy.y() ) );
   }








} // end of ns pnt





#endif



//EOF
