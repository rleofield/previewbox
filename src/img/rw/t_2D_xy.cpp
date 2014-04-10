///*
//--------------------------------------------------------------------------
//Copyright 2013 by Richard Albrecht
//richard.albrecht@rleofield.de
//www.rleofield.de

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

//You should have received a copy of the GNU Lesser General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------


//*/
///*! \file rimg_rw_raw.cpp
//\brief impl for read and write raw images

//Lib: librimgrw
//*/



//#include <iostream>
//#include <sstream>
//#include <string>


//#include "boost_cstdint.h"
//#include "t_2D_xy.h"

//using rlf_minit::float_xy;
//using rlf_minit::double_xy;
//using rlf_minit::uint32_xy;
//using rlf_minit::int32_xy;

//namespace rlf {

//   float ratio( float_xy const& p ) {
//      assert( fabs( p.h() ) > rlf_minit::loc_FLT_EPSILON );
//      return p.w() / p.h();
//   }
//   double ratio( double_xy const& p ) {
//      assert( fabs( p.h() ) > rlf_minit::loc_DBL_EPSILON );
//      return p.w() / p.h();
//   }

//   float_xy operator/( uint32_xy cv, uint32_xy img ) {
//      float_xy temp( ( float )cv.x(), ( float )cv.y() );
//      temp /= float_xy( ( float )img.x(), ( float )img.y() );
//      return temp;
//   }

//   double length( double_xy const& xy ) {
//      return sqrt( xy.x() * xy.x() + xy.y() * xy.y() );
//   }

//   double distance( double_xy const& a, double_xy const& b ) {
//      double dx( a.x() - b.x() );
//      double dy( a.y() - b.y() );
//      return sqrt( dx * dx + dy * dy );
//   }


//}
////EOF






