
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

/*
Workfile: rimg_binedit.cpp


changed

*/

#include <assert.h>

#include "t_2D_xy.h"
//#include "rimg_xy_types.h"
#include "rimg_math.h"
#include "rimg_planar.h"

using namespace std;




namespace rlf {


   namespace rimg_math {



      void rimginterface::ApplyLut( tPlane8& source, rlf::rimg::tLUT const& lut ) {


         struct x {
            rlf::rimg::tLUT  const& l;
            void func( uint8_t& v ) {
               v = l[ v ];
            }
            x( rlf::rimg::tLUT const& l_ ): l( l_ ) {}
            void operator()( uint8_t& v ) {
               func( v );
            }
         };

         struct y {
            rlf::rimg::tLUT  const& l;
            y( rlf::rimg::tLUT const& l_ ): l( l_ ) {}
            void operator()( vLine8& v ) {
               for_each( v.begin(), v.end(), x( l ) );
            }
         };
         for_each( source.begin(), source.end(), y( lut ) );
      }

      namespace average {

         void Average( vLine8& target, const vLine8& source ) {


            auto xbegin = source.begin();
            auto xend = source.end();
            auto target_xbegin = target.begin();

            while( xbegin != xend ) {


               *target_xbegin = ( uint8_t )( ( ( uint32_t ) * target_xbegin + ( uint32_t ) * xbegin ) >> 1 );

               ++xbegin;
               ++target_xbegin;
            }


         }


         void Average( tPlane8& target, const tPlane8& source ) {

            auto ybegin = source.begin();
            auto yend = source.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               Average( *target_ybegin, *ybegin );
               ++ybegin;
               ++target_ybegin;
            }


         }

      }

      void rimginterface::Average( tImgPlanar& target, const tImgViewPlanar& source ) {
         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty
         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               average::Average( target_mbegin->plane, mbegin->plane );
            }



            ++mbegin;
            ++target_mbegin;
         }



      }
      namespace nsor {
         void Or( tPlane8& target,  const tPlane8& or_ ) {
            auto ybegin = or_.begin();
            auto yend = or_.end();
            auto t_ybegin = target.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto t_xbegin = t_ybegin->begin();

               while( xbegin != xend ) {


                  *t_xbegin  = *t_xbegin | *xbegin;

                  ++xbegin;
                  ++t_xbegin;
               }

               ++ybegin;
               ++t_ybegin;
            }

         }
      }
      void rimginterface::Or( tImgPlanar& target,  const tImgViewPlanar& or_ ) {
         tImgViewPlanar const& source = or_;


         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty


         auto mbegin = source.begin();
         auto mend = source.end();
         auto t_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               nsor::Or( t_mbegin->plane, mbegin->plane );
            }

            ++mbegin;
            ++t_mbegin;
         }

      }
      namespace nsxor {

         void Xor( vLine8& target, const vLine8& xor_ ) {
            auto xbegin = xor_.begin();
            auto xend = xor_.end();
            auto target_xbegin = target.begin();

            while( xbegin != xend ) {
               *target_xbegin  ^= *xbegin;
               ++xbegin;
               ++target_xbegin;
            }
         }

         void Xor( tPlane8& target, const tPlane8& xor_ ) {

            auto ybegin = xor_.begin();
            auto yend = xor_.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               Xor( *target_ybegin, *ybegin );
               ++ybegin;
               ++target_ybegin;
            }


         }
      }
      void rimginterface::Xor( tImgPlanar& target, const tImgViewPlanar& xor_ ) {
         tImgViewPlanar const& source = xor_;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty

         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               nsxor::Xor( target_mbegin->plane, mbegin->plane );
            }

            ++mbegin;
            ++target_mbegin;
         }

      }

      void operator&( tImgPlanar& im, const tImgViewPlanar& and_ ) {
         rlf::rimg_math::rimginterface r;
         r.BitwiseAnd( im, and_ );
      }

      namespace bitwiseAnd {
         void BitwiseAnd( tPlane8& target, const tPlane8& and_ ) {

            auto ybegin = and_.begin();
            auto yend = and_.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto target_xbegin = target_ybegin->begin();

               while( xbegin != xend ) {


                  *target_xbegin  &= *xbegin;

                  ++xbegin;
                  ++target_xbegin;
               }

               ++ybegin;
               ++target_ybegin;
            }

         }
      }
      void rimginterface::BitwiseAnd( tImgPlanar& target, const tImgViewPlanar& and_ ) {
         tImgViewPlanar const& source = and_;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty


         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               bitwiseAnd::BitwiseAnd( target_mbegin->plane, mbegin->plane );
            }

            ++mbegin;
            ++target_mbegin;
         }

      }

      namespace logicalAnd {
         void LogicalAnd( tPlane8& target, const tPlane8& second ) {

            auto ybegin = second.begin();
            auto yend = second.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto target_xbegin = target_ybegin->begin();

               while( xbegin != xend ) {
                  *target_xbegin  = *target_xbegin && *xbegin;
                  ++xbegin;
                  ++target_xbegin;
               }

               ++ybegin;
               ++target_ybegin;
            }

         }
      }


      void rimginterface::LogicalAnd( tImgPlanar& target, const tImgViewPlanar& second ) {
         tImgViewPlanar const& source = second;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty

         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               logicalAnd::LogicalAnd( target_mbegin->plane, mbegin->plane );
            }

            ++mbegin;
            ++target_mbegin;
         }

      }

      namespace andBitwiseNot {
         void AndBitwiseNot( tPlane8& target, const tPlane8& second ) {
            auto ybegin = second.begin();
            auto yend = second.end();
            auto target_ybegin = target.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto target_xbegin = target_ybegin->begin();

               while( xbegin != xend ) {


                  *target_xbegin  = !( *target_xbegin & *xbegin );

                  ++xbegin;
                  ++target_xbegin;
               }

               ++ybegin;
               ++target_ybegin;
            }

         }
      }
      void rimginterface::AndBitwiseNot( tImgPlanar& target, const tImgViewPlanar& second ) {
         tImgViewPlanar const& source = second;

         assert( target.equals_size_mask( source ) );
         // use all channels, not used channels are empty

         auto mbegin = source.begin();
         auto mend = source.end();
         auto target_mbegin = target.begin();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               andBitwiseNot::AndBitwiseNot( target_mbegin->plane, mbegin->plane );
            }

            ++mbegin;
            ++target_mbegin;
         }

      }




      /* --  multiplikation  mit Werten < 1  ( 1/scale ) (e.g. 5 = mult with 0.2 = 1/5 )
      */
      namespace scale {
         void Scale( tPlane8& source, uint32_t scale ) {

            rimginterface r;
            r.ApplyLut( source, rimg::tLUT::Scale( scale ) );
         }
      }
      void rimginterface::Scale( tImgPlanar& source, int scale ) {
         if( scale < 1 ) {
            return;
         }

         // use all channels, not used channels are empty
         auto mbegin = source.begin();
         auto mend = source.end();

         rimg::tLUT l = rimg::tLUT::Scale( scale );

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               ApplyLut( mbegin->plane, l );

            }

            ++mbegin;

         }


      }

      /* --  multiplikation  mit Werten > 1  ( factor )
      */

      void rimginterface::Multiplicate( tImgPlanar& source, int factor ) {


         if( factor < 1 ) {
            return;
         }

         auto mbegin = source.begin();
         auto mend = source.end();

         rimg::tLUT l = rimg::tLUT::Multiplicate( factor ) ;

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               ApplyLut( mbegin->plane, l );
            }

            ++mbegin;
         }
      }

      namespace maximum {
         uint32_t Maximum( tPlane8& source ) {
            struct x {
               uint32_t& m;
               x( uint32_t& m_ ): m( m_ ) {}
               void operator()( uint8_t& v ) {
                  if( v > m ) {
                     m = v;
                  }
               }
            };

            struct y {
               uint32_t m;
               y(): m( 0 ) {}
               void operator()( vLine8& v ) {
                  for_each( v.begin(), v.end(), x( m ) );
               }
            };
            return for_each( source.begin(), source.end(), y() ).m;

         }
      }

      vminmax rimginterface::Maximum( tImgPlanar& source ) {

         vminmax m( source.planes() );
         auto mbegin = source.begin();
         auto mend = source.end();
         uint32_t mindex = 0;

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               m[ mindex ] = maximum::Maximum( mbegin->plane );
            }

            ++mbegin;
            mindex++;
         }

         return m;
      }

      namespace minimum {
         uint32_t Minimum( tPlane8& source ) {
            struct x {
               uint32_t& m;
               x( uint32_t& m_ ): m( m_ ) {}
               void operator()( uint8_t& v ) {
                  if( v < m ) {
                     m = v;
                  }
               }
            };

            struct y {
               uint32_t m;
               y(): m( 0xff ) {}
               void operator()( vLine8& v ) {
                  for_each( v.begin(), v.end(), x( m ) );
               }
            };
            return for_each( source.begin(), source.end(), y() ).m;

         }
      }



      vminmax rimginterface::Minimum( tImgPlanar& source ) {

         vminmax m( source.planes(), 0xff );

         auto mbegin = source.begin();
         auto mend = source.end();
         uint32_t mindex = 0;

         while( mbegin  != mend ) {

            if( source.has_bit_at( mbegin ) ) {
               m[ mindex ] = minimum::Minimum( mbegin->plane );
            }

            ++mbegin;
            mindex++;
         }

         return m;
      }

      namespace normalize {
         void Normalize( tPlane8& source, uint32_t norm ) {
            int max     = 0;
            int min     = 0xff;

            auto ybegin = source.begin();
            auto yend = source.end();


            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();


               while( xbegin != xend ) {


                  if( min > *xbegin ) {
                     min = *xbegin;
                  }

                  if( max < *xbegin ) {
                     max = *xbegin;
                  }

                  ++xbegin;
               }

               ++ybegin;


            }

            int delta = max - min;
            assert( delta != 0 );


            ybegin = source.begin();
            yend = source.end();


            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();


               while( xbegin != xend ) {

                  int n  = *xbegin - min;
                  n = ( ( n * norm ) / delta );

                  if( n > 0xff ) {
                     n = 0xff;
                  }

                  *xbegin = ( uint8_t )n;

                  ++xbegin;
               }

               ++ybegin;


            }

         }
      }

      // img = img * norm / ( max - min );
      void rimginterface::Normalize( tImgPlanar& source, uint32_t norm ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               normalize::Normalize( mbegin->plane, norm );
            }

            ++mbegin;
         }

      }
      namespace  compareBinWithGray {
         void CompareBinWithGray( tPlane8 const& gray, tPlane8& bin, uint32_t thre, uint32_t c ) {


            auto ybegin = gray.begin();
            auto yend = gray.end();
            auto bin_ybegin = bin.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto bin_xbegin = bin_ybegin->begin();

               while( xbegin != xend ) {
                  if( *bin_xbegin > 0 || *xbegin > thre ) {
                     *bin_xbegin = ( uint8_t )c;
                  } else {
                     *bin_xbegin = 0;
                  }

                  ++xbegin;
                  ++bin_xbegin;
               }

               ++ybegin;
               ++bin_ybegin;
            }
         }
      }

      // if bin > 0 or gray > thre  -> then gray = color else gray = 0
      // can be used to compare a result bin with new thresholded gray

      void rimginterface::CompareBinWithGray(
         tImgViewPlanar const& gray, tImgPlanar& bin, uint32_t thre, uint32_t c ) {

         assert( bin.equals_size_mask( gray ) );
         // use all channels, not used channels are empty

         auto mbegin = gray.begin();
         auto mend = gray.end();
         auto bin_mbegin = bin.begin();

         while( mbegin  != mend ) {
            if( gray.has_bit_at( mbegin ) ) {
               compareBinWithGray::CompareBinWithGray( mbegin->plane, bin_mbegin->plane, thre, c );
            }

            ++mbegin;
            ++bin_mbegin;
         }

      }

      namespace binMaskGray {
         void BinMaskGray( const tPlane8& bin, tPlane8& gray, int color ) {


            auto ybegin = gray.begin();
            auto yend = gray.end();
            auto bin_ybegin = bin.begin();

            while( ybegin != yend ) {
               auto xbegin = ybegin->begin();
               auto xend = ybegin->end();
               auto bin_xbegin = bin_ybegin->begin();

               while( xbegin != xend ) {
                  if( *bin_xbegin > 0 ) {
                     *xbegin = ( uint8_t )color;
                  } else {
                     *xbegin = *bin_xbegin;
                  }

                  ++xbegin;
                  ++bin_xbegin;
               }

               ++ybegin;
               ++bin_ybegin;
            }

         }
      }

      // if bin, then gray = color
      void rimginterface::BinMaskGray( tImgViewPlanar const& gray, tImgPlanar& bin, uint32_t color ) {

         assert( bin.equals_size_mask( gray ) );
         // use all channels, not used channels are empty

         auto mbegin = gray.begin();
         auto mend = gray.end();
         auto bin_mbegin = bin.begin();

         while( mbegin  != mend ) {
            if( gray.has_bit_at( mbegin ) ) {
               binMaskGray::BinMaskGray( mbegin->plane, bin_mbegin->plane, color );
            }

            ++mbegin;
            ++bin_mbegin;
         }

      }

      namespace invert {
         void Invert( tPlane8& source ) {
            struct x {
               x() {}
               void operator()( uint8_t& v ) {
                  v = 0xff - v;
               }
            };
            struct y {
               y() {}
               void operator()( vLine8& v ) {
                  for_each( v.begin(), v.end(), x() );
               }
            };
            for_each( source.begin(), source.end(), y() );
         }



      }

      void rimginterface::Invert( tImgPlanar& gray ) {

         // use all channels, not used channels are empty

         auto mbegin = gray.begin();
         auto mend = gray.end();

         while( mbegin  != mend ) {
            if( gray.has_bit_at( mbegin ) ) {
               invert::Invert( mbegin->plane );
            }

            ++mbegin;
         }
      }



      /* --
      erosion is the logical || of the neigbours
      d.h pixel bleibt, wenn alle nachbarn != 0       (strength = 8)

      dilation is the logical || of the neigbours
      d.h pixel wird gesetzt, wenn einer der nachbarn != 0  (strength = 8)

      */
      const int DILATE = 1;
      const int ERODE  = 0;


      bool rimginterface::Closing( tPlane8& im, int strength, uint32_t color ) {
         bool b = Dilate( im, strength, color );

         if( !b ) {
            b = Erode( im, strength, color );
         }

         return b;
      }
      bool rimginterface::Closing( tImgPlanar& source, int strength, uint32_t color ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               Closing( mbegin->plane, strength, color );
            }

            ++mbegin;
         }

         return true;

      }

      bool rimginterface::Opening( tPlane8& im, int strength, uint32_t color ) {
         bool b = Erode( im, strength, color );

         if( !b ) {
            b = Dilate( im, strength, color );
         }

         return b;
      }
      bool rimginterface::Opening( tImgPlanar& source, int strength, uint32_t color ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               Opening( mbegin->plane, strength, color );
            }

            ++mbegin;
         }

         return true;

      }


      bool rimginterface::Dilate( tPlane8& im, int strength, uint32_t color ) {
         return BinEdit( im, strength, color, DILATE );
      }

      bool rimginterface::Erode( tPlane8& im, int strength, uint32_t color ) {
         return BinEdit( im, strength, color, ERODE );
      }
      bool rimginterface::Dilate( tImgPlanar& source, int strength, uint32_t color ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               Dilate( mbegin->plane, strength, color );
            }

            ++mbegin;
         }

         return true;


      }
      bool rimginterface::Erode( tImgPlanar& source, int strength, uint32_t color ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               Erode( mbegin->plane, strength, color );
            }

            ++mbegin;
         }

         return true;
      }

      namespace {
         uint8_t dilate( tPlane8 const& im, int ix, int iy, uint8_t const* table, int color ) {

            uint8_t ret = 0;

            if( im[iy][ix] == 0 ) {      /* -- if pixel is not, check to set  */

               int index = 0;
               int y = iy - 1;
               int x = ix - 1;

               if( im[y][x++] ) {
                  index |= 0x01;
               }

               if( im[y][x++] ) {
                  index |= 0x02;
               }

               if( im[y][x] ) {
                  index |= 0x04;
               }

               y++;
               x = ix - 1;

               if( im[y][x++] ) {
                  index |= 0x08;   // 00001000
               }

               x++;

               if( im[y][x] ) {
                  index |= 0x10;
               }

               y++;
               x = ix - 1;

               if( im[y][x++] ) {
                  index |= 0x20;
               }

               if( im[y][x++] ) {
                  index |= 0x40;
               }

               if( im[y][x] ) {
                  index |= 0x80;
               }

               uint8_t b = table[ index ];

               if( b ) {
                  ret = ( uint8_t )color;
               }

            } else { // pixel is
               ret = ( uint8_t )color;
            }

            return ret;
         }


         uint8_t erode( tPlane8 const& im, int ix, int iy, uint8_t const* table, int color ) {
            uint8_t ret = 0;

            if( im[iy][ix] != 0 ) {      /* -- if pixel is, check to remove */

               int index = 0;

               if( im[iy - 1][ix - 1] ) {
                  index +=   1;
               }

               if( im[iy - 1][ix  ] ) {
                  index +=   2;
               }

               if( im[iy - 1][ix + 1] ) {
                  index +=   4;
               }

               if( im[iy  ][ix - 1] ) {
                  index +=   8;
               }

               if( im[iy  ][ix + 1] ) {
                  index +=  16;
               }

               if( im[iy + 1][ix - 1] ) {
                  index +=  32;
               }

               if( im[iy + 1][ix  ] ) {
                  index +=  64;
               }

               if( im[iy + 1][ix + 1] ) {
                  index += 128;
               }

               if( table[ index ] ) {
                  ret  = ( uint8_t )color;
               }
            }

            return ret;
         }
         void Clear( vLine8& im, uint32_t val ) {
            std::fill( im.begin(), im.end(), val );
         }
         void Clear( tPlane8& im, uint32_t val ) {

            auto pbegin = im.begin();
            auto pend = im.end();

            while( pbegin != pend ) {
               Clear( *pbegin, val );
               std::fill( pbegin->begin(), pbegin->end(), val );
               ++pbegin;
            }


         }
      }

      void rimginterface::BinEdit( tImgPlanar& source, int strength, uint32_t color, int edit_case ) {
         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( source.has_bit_at( mbegin ) ) {
               BinEdit( mbegin->plane, strength, color, edit_case );
            }

            ++mbegin;
         }


      }


      bool rimginterface::BinEdit( tPlane8& source, int strength, uint32_t color, int edit_case ) {


         int dilation = 1;
         //   int erosion  = 0;

         if( strength < 1 || strength > 8 ) {
            return false;
         }

         uint8_t* table = lut1;
         int e = edit_case;

         switch( strength + ( e * 8 ) ) {
         case  1:                        // s = 1, e = ero 1
         case 16:
            table = lut1;
            break;  // s = 8, e = dil 8

         case  2:                        // s = 2, e = ero 2
         case 15:
            table = lut2;
            break;  // s = 7, e = dil 7

         case  3:                        // s = 3, e = ero 3
         case 14:
            table = lut3;
            break;  // s = 6, e = dil 6

         case  4:                        // s = 4, e = ero 4
         case 13:
            table = lut4;
            break;  // s = 5, e = dil 5

         case  5:                        // s = 5, e = ero 5
         case 12:
            table = lut5;
            break;  // s = 4, e = dil 4

         case  6:                        // s = 6, e = ero 6
         case 11:
            table = lut6;
            break;  // s = 3, e = dil 3

         case  7:                        // s = 7, e = ero 7
         case 10:
            table = lut7;
            break;  // s = 2, e = dil 2

         case  8:                                 // s = 8, e = ero 8
         case  9:
            table = lut8;
            break;  // s = 1, e = dil 1

         default:
            table = lut1;
         }

         uint32_t halfkernel  = 1;
         size_t x = source.size();
         size_t y = source[0].size();
         size_t x2 = x + halfkernel * 2;
         size_t y2 = y + halfkernel * 2;

         /* -- allocate buffer ( each col/row = image+2 )
         */
         tPlane8 temp( x2, vLine8( y2 ) );

         if( edit_case == dilation ) {
            Clear( temp, 0 );
         } else {
            Clear( temp, max_byte );
         }


         /* -- copy image to new im ( border = 1 )
         */

         rimg_math::ToExtendedImageArea( source, temp, halfkernel );

         /* -- image to new copied
         now do the edit                */

         Clear( source, 0 );

         for( size_t iy = 0; iy < y ; iy++ ) {
            for( size_t ix = 0; ix < x ; ix++ ) {
               if( edit_case == dilation ) {
                  source[iy][ix] = dilate( temp, ix + 1, iy + 1, table, color );
               } else {
                  source[iy][ix] = erode( temp,  ix, iy, table, color );
               }

            }
         }

         return true;
      }

      void rimginterface::Binarize( tPlane8& source, uint8_t thre, uint8_t color ) {

         auto ybegin = source.begin();
         auto yend = source.end();

         while( ybegin != yend ) {
            auto xbegin = ybegin->begin();
            auto xend = ybegin->end();

            while( xbegin != xend ) {

               if( *xbegin >= thre ) {
                  *xbegin = ( uint8_t )color;
               } else {
                  *xbegin = 0;
               }


               ++xbegin;
            }

            ++ybegin;
         }

      }
      void rimginterface::Binarize( tImgPlanar& source, uint8_t thre, uint8_t color ) {

         auto mbegin = source.begin();
         auto mend = source.end();

         while( mbegin  != mend ) {
            if( mbegin->plane.size() > 0 ) {
               Binarize( mbegin->plane, thre, color ) ;
            }

            ++mbegin;
         }

      }

#ifdef gggggggggg
      void cImageBuffer::GradientAtLevel( const cImageBuffer& target, // {
                                          int l, int md, int c, int lh )const {

         assert( IsBit8() );

         if( *this != target ) {
            return ;
         }

         int low_high = lh;
         int color = c;
         int level = l;
         int min_delta = md;

         Dword sx = Sx();
         Dword sy = Sy();
         Dword ix, iy;


         /* -- scan vertical
         */
         for( ix = 0; ix < sx; ix++ ) {
            for( iy = 1; iy < sy; iy++ ) {
               int old = img8[iy - 1][ix];  // above
               int lfd = img8[iy][ix];
               int delta = old - lfd;

               if( delta < 0 ) {
                  delta = - delta;
               }

               /* -- test for positive gradient
               */
               if( old < level || lfd >= level || delta > min_delta ) {
                  if( low_high < 0 ) {
                     target.img8[iy - 1][ix] = ( Byte )color;
                  } else {
                     target.img8[iy][ix]   = ( Byte )color;
                  }
               }

               /* -- test for negative gradient
               */
               if( ( old > level ) || ( lfd <= level )  || ( delta > min_delta ) ) {
                  if( low_high < 0 ) {
                     target.img8[iy][ix]   = ( Byte )color;
                  } else {
                     target.img8[iy - 1][ix] = ( Byte )color ;
                  }
               }
            }
         }


         /* -- scan horizontal
         */
         for( iy = 0; iy < sy; iy++ ) {

            for( ix = 1; ix < sx; iy++ ) {
               int old = img8[iy][ix - 1 ];
               int lfd = img8[iy][ix];
               int delta = old - lfd;

               if( delta < 0 ) {
                  delta = - delta;
               }


               /* -- test for positive gradient
               */
               if( ( old < level ) || ( lfd >= level )  || ( delta > min_delta ) ) {
                  if( low_high < 0 ) {
                     target.img8[iy][ix - 1] = ( Byte )color ;
                  } else {
                     target.img8[iy][ix]     = ( Byte )color;
                  }
               }

               /* -- test for negative gradient
               */
               if( ( old > level ) || ( lfd <= level )  || ( delta > min_delta ) ) {
                  if( low_high < 0 ) {
                     target.img8[iy][ix]     = ( Byte )color;
                  } else {
                     target.img8[iy][ix - 1] = ( Byte )color ;
                  }
               }
            }
         }
      }

#endif


   }

} // end of namespace rlf

//EOF





