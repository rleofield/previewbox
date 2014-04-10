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

/*! \file rimg_math.h
\brief header for image calculations

*/



#ifndef RIMG_MATH_H
#define RIMG_MATH_H


#include "t_2D_xy.h"
#include "t_3D_xyz.h"

#include "rimg_planar_typedefs.h"
#include "rimg_plane_linear.h"
#include "rimg_planar.h"

#include "rimg_aoi.h"
#include "rimg_lookuptable.h"

using namespace rlf;
using rlf_minit::double_xy;
using rlf::rimg::tLUT;
using rlf_minit::double_xyz;
using namespace rlf::rimg_planar;
using rlf::rimg_planar::tPlane8;
using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_planar::tPlaneLinear8;


namespace rlf {
   //namespace rlf {


   namespace rimg_planar {





   }


   namespace rimg_math {

      void ToExtendedImageArea( const tPlane8& in, tPlane8& out, int border ) ;
      void ToExtendedLinearArea( const tPlane8& in, tPlaneLinear8& out, int border ) ;
      // a class is used to find linkage errors at compile time
      class rimginterface {
      public:
         // return: maximum and position of maximum in area as double
         double_xyz MaximumInArea( tImgPlanar const& p, tAoi area );

         size_t max_x( tImgPlanar const& im );
         size_t min_x( tImgPlanar const& im );
         size_t max_y( tImgPlanar const& im );
         size_t min_y( tImgPlanar const& im );


         //testpattern
         void fill( tImgPlanar& img, uint32_t val ) ;
         void rampx( tImgPlanar& img, size_t start = 1, size_t end = 255 );
         void rampy( tImgPlanar& img, size_t start = 1, size_t end = 255 );
         void rampxy( tImgPlanar& img, size_t start = 1, size_t end = 255 );
         bool rectangle( tImgPlanar& img,  uint32_t val, uint32_xy pos, uint32_xy size ) ;

         // rimg_math, insert
         bool Insert( const tImgViewPlanar& from, tImgPlanar& to, int32_xy xy ) ;


         // ring_math, aoi
         bool ExtractAoi( tImgViewPlanar const& s, tImgPlanar& t, tAoi& aoi ) ;


         // helper
         void ExtendImage( const tImgViewPlanar& in, tImgPlanar& extended, size_t kernel ) ;
         void ToExtendedImage( const tImgViewPlanar& in, tImgPlanar& out, int border ) ;
         void FitToImgPlanar( tIntPlane32 const& source, tPlane8& target, int32_xy minmax ) ;
         void FitToImgPlanar( tIntPlane32 const& hor , tImgPlanar& img, int32_xy minmax ) ;
         void ToExtendedImageArea( const tImgViewPlanar& in,
                                   tImgPlanar& extended, size_t border ) ;

         // lowpass
         void Gauss3( tImgPlanar& img );
         void Gauss( tImgPlanar& img, int ker_size ) ;
         void Gauss1( tImgPlanar& img, int ker_size ) ;
         void Gauss31( tImgPlanar& img ) ;

         // edges
         void Sobel( tImgViewPlanar const& source, tImgPlanar& abs, tImgPlanar& orient ) ;

         // bin edit
         void ApplyLut( tPlane8& source, tLUT const& lut ) ;
         void Add( tImgPlanar& sum,   const tImgViewPlanar& summand,  int constant ) ;
         void Add( tImgPlanar& source, int const1 ) ;


         void Subtract( tImgPlanar& target, const tImgViewPlanar& second, int const1 = 0 ) ;
         void Subtract( tImgPlanar& target, int constant ) ;

         void Or( tImgPlanar& target,  const tImgViewPlanar& or_ ) ;
         void Xor( tImgPlanar& target, const tImgViewPlanar& xor_ ) ;

         void BitwiseAnd( tImgPlanar& target, const tImgViewPlanar& xor_ ) ;

         void Average( tImgPlanar& target, const tImgViewPlanar& source ) ;
         void LogicalAnd( tImgPlanar& target, const tImgViewPlanar& second ) ;
         void AndBitwiseNot( tImgPlanar& target, const tImgViewPlanar& second ) ;


         void SubtractShift( tImgPlanar& target, const tImgViewPlanar& sub, int c , int shift ) ;
         void Scale( tImgPlanar& source, int scale ) ;
         void Multiplicate( tImgPlanar& source, int factor ) ;


         vminmax Maximum( tImgPlanar& source ) ;
         vminmax Minimum( tImgPlanar& source ) ;

         void Normalize( tImgPlanar& source, uint32_t norm ) ;

         void CompareBinWithGray( tImgViewPlanar const& gray, tImgPlanar& bin, uint32_t thre, uint32_t c ) ;
         void BinMaskGray( tImgViewPlanar const& gray, tImgPlanar& bin, uint32_t color ) ;
         void Invert( tImgPlanar& gray ) ;


         bool Closing( tImgPlanar& im, int strength, int color );
         bool Opening( tImgPlanar& im, int strength, int color );
         bool Dilate( tImgPlanar& im, int strength, int color );
         bool Erode( tImgPlanar& im, int strength, int color );
         void BinEdit( tImgPlanar& im, int strength, int color, int edit_case );

         bool Closing( tPlane8& im, int strength, int color );
         bool Opening( tPlane8& im, int strength, int color );
         bool Dilate( tPlane8& im, int strength, int color );
         bool Erode( tPlane8& im, int strength, int color );
         bool BinEdit( tPlane8& im, int strength, int color, int edit_case );


         // rimg_graph
         void DrawText( tImgPlanar& img, uint32_xy xy, const string& text, vcolors const& colors ) ;
         void Line( tImgPlanar& img,  int32_xy from, int32_xy to, vcolors const& colors ) ;
         void Circle( tImgPlanar& img, int32_xy xy, int r, vcolors const& colors ) ;
         void FilledCircle( tImgPlanar& img, int32_xy xy, int r, vcolors const& colors ) ;
         void VerticalLine( tImgPlanar& img,  int xpos, vcolors const& colors ) ;
         void HorizontalLine( tImgPlanar& img,  int xpos, vcolors const& colors ) ;
         void WedgeHorizontal( tImgPlanar& img,  int sy ) ;
         void WedgeVertical( tImgPlanar& img,  int sy ) ;

         void OrthogonalCross( tImgPlanar& img, int32_xy pos, int size, vcolors const& colors ) ;
         void DiagonalCross( tImgPlanar& img, int32_xy xy, int const& c ) ;

         void DrawPixelList( tImgPlanar& img, std::vector<int32_xy> v,  uint32_t val ) ;
         std::vector<uint32_t> GetLinePixels( tImgPlanar const& img, uint32_xy from, uint32_xy to ) ;
         int GetLinePixels( tImgPlanar const& img, uint32_xy from, uint32_xy to, std::vector<uint32_t> &v ) ;


         // in rimg_median.cpp
         void Median( tImgPlanar& source, uint32_t kernel );

         void Rank( tPlane8 const& im, tPlane8& min_img, tPlane8& max_img, int kernel ) ;
         void Rank( tImgViewPlanar const& source,  tImgPlanar& min_img,  tImgPlanar& max_img, uint32_t kernel ) ;

         void Binarize( tPlane8& source, uint8_t thre, uint8_t color ) ;
         void Binarize( tImgPlanar& source, uint8_t thre, uint8_t color ) ;

         // zoom
         void ZoomIn( tImgPlanar const& source, tImgPlanar& target, int scale ) ;
         void ZoomOut( tImgPlanar const& source, tImgPlanar& target, int scale ) ;
         void Zoom( tImgPlanar const& source, tImgPlanar& target, double scale );
         void ZoomToSize( tImgPlanar const& source, tImgPlanar& target,  uint32_t sizex, uint32_t sizey ) ;
         void zoom( tImgPlanar const& source, tImgPlanar& target, double xscale, double yscale );
      };


      void operator+( tImgPlanar& img, int constant );
      void operator+( tImgPlanar& img, const tImgPlanar& im ) ;
      void operator-( tImgPlanar& target,  const tImgPlanar& sub ) ;
      void operator-( tImgPlanar& target, int constant ) ;
      void operator&( tImgPlanar& target, const tImgViewPlanar& and_ ) ;

      inline uint32_t ToOddKernelSize( uint32_t i ) {
         if( i < 3 ) {
            return 3;
         }

         if( ( i % 2 ) == 0 ) {
            return i + 1;
         }

         return i;
      }
      inline bool is_odd( uint32_t i ) {
         if( i < 3 ) {
            return false;
         }

         if( ( i % 2 ) == 0 ) {
            return false;
         }

         return true;
      }


      extern uint8_t lut1[256];
      extern uint8_t lut2[256];
      extern uint8_t lut3[256];
      extern uint8_t lut4[256];
      extern uint8_t lut5[256];
      extern uint8_t lut6[256];
      extern uint8_t lut7[256];
      extern uint8_t lut8[256];

   } // end of namespace
}

#endif

//EOF
