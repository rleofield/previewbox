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

/*! \file rlf_math.cpp
\brief math functions

*/


#include <algorithm>
#include <functional>
#include <limits.h>
#include <assert.h>

//#include "rimg_imgsize_type.h"

//#include "rimg_xy_types.h"
#include "rlf_math.h"
#ifdef _WIN32
#pragma warning( disable:4267) // possible loss of data ( size_t nach int )
#endif

using namespace std;


namespace rlf {
   namespace math {




      double Gauss( double stddev, double x ) {
         double gauss = ::exp( -0.5 * ( x * x ) / ( stddev * stddev ) ) / sqrt( 2 * PI );
         return gauss / stddev;
      }

      double  atan2( double x, double y ) {
         double e =  0.000001;

         if( fabs( x ) < e ) {
            x = e;
         }

         if( fabs( y ) < e ) {
            y = e;
         }

         return ::atan2( y, x );
      }

      double length2d( const double_xy& xy ) {
         double square = xy.x() * xy.x() + xy.y() * xy.y();
         return ::sqrt( square );
      }
      double length3d( const double_xyz& xy ) {
         double square = xy.x() * xy.x() + xy.y() * xy.y() + xy.z() * xy.z();
         return ::sqrt( square );
      }


      double distance( double_xy const& xy0, double_xy const& xy1 ) {
         return ::sqrt( square_distance( xy0, xy1 ) );
      }
      double distance( double_xyz const& xy0, double_xyz const& xy1 ) {
         return ::sqrt( square_distance( xy0, xy1 ) );
      }


      double square_distance( double_xy const& xy0, double_xy const& xy1 ) {
         double dx = xy0.x() - xy1.x();
         double dy = xy0.y() - xy1.y();
         return dx * dx + dy * dy;
      }
      double square_distance( double_xyz const& xy0, double_xyz const& xy1 ) {
         double dx = xy0.x() - xy1.x();
         double dy = xy0.y() - xy1.y();
         double dz = xy0.z() - xy1.z();
         return dx * dx + dy * dy + dz * dz;
      }

      double interpolate( double x, double_xy xy0, double_xy xy1 ) {

         double x0 = xy0.x();
         double y0 = xy0.y();

         double dy = xy0.y() - xy1.y();
         double dx = xy0.x() - xy1.x();
         // -- and interpolate
         return  y0 + ( ( x - x0 ) / dx ) * dy ;
      }


      double std_dev( double const* buf, double mean, int size ) {
         double var = 0.0;
         double isize = size;

         while( isize-- ) {
            double diff = ( double )( *buf++ )  - mean;
            var += diff * diff;
         }

         return( sqrt( ( var / ( ( double )( size - 1 ) ) ) ) );
      }

      struct stddev_adder: public unary_function<double, void> {
         double _mean, _var;
         stddev_adder( double mean ): _mean( mean ), _var( 0 ) {}
         void operator()( double d ) {
            double diff = d - _mean;
            _var += diff * diff;
         }
         double result()const {
            return _var;
         }
      };

      double std_dev( vector<double> const& v, double mean ) {
         if( v.size() < 2 ) {
            return 0.0;
         }

         double  varianz = for_each( v.begin(), v.end(), stddev_adder( mean ) ).result();
         return sqrt( varianz / ( ( double )( v.size() - 1 ) ) ) ;
      }



      double std_dev( vector<double> const& v ) {
         double temp = mean( v );
         return std_dev( v, temp );
      }


      double mean( const double* buf, int size ) {
         double sum = 0;
         int isize = size;
         int count = 0;

         while( isize-- ) {
            sum += *buf++;
            count++;
         }

         return( ( double )sum / ( double )size );
      }
      namespace {
         template<class T> class TSum {
            T sum;
         public:
            TSum( TSum<T> const& in ) {
               sum = in.sum;
            }
            TSum<T>& operator=( TSum<T> const& in ) {
               if( this != &in ) {
                  sum = in.sum;
               }

               return *this;
            }
            TSum(): sum( T() ) {}
            void operator()( T const& d ) {
               sum += d;
            }
            T result()const {
               return sum;
            }
         };
         template<class T> T Mean( vector<T> const& v ) {
            return for_each( v.begin(), v.end(), TSum<T>() ).result() / ( T )v.size();
         }
      }
      double mean( vector<double> const& v ) {
         if( v.size() == 0 ) {
            return 0.0;
         }

         return Mean<double>( v );
      }


      double Distance( const double_xyz& d ) {
         return VectorLength( d );
      }

      double Distance( const double_xy& d ) {
         return VectorLength( d );
      }

      double Distance( const int32_xy& d ) {
         return VectorLength( d );
      }

      unsigned long Area( const int32_xy& t ) {
         return t.x() * t.y();
      }

      unsigned long Pixels( const int32_xy& t ) {
         return t.x() * t.y();
      }

      double Area( const double_xy& t ) {
         return t.x() * t.y();
      }

      double Pixels( const double_xy& t ) {
         return Area( t );
      }

      double VectorLength( const double_xyz& d ) {
         return sqrt( d.x() * d.x() + d.y() * d.y() + d.z() * d.z() );
      }

      double VectorLength( const double_xy& d ) {
         return sqrt( d.y() * d.y() + d.y() * d.y() );
      }

      double VectorLength( const int32_xy& d ) {
         double x = d.x();
         double y = d.y();
         return sqrt( x * x + y * y );
      }


      double inch_to_cm( double x ) {
         return inch   * x;
      }
      double cm_to_inch( double x ) {
         return ( 1 / inch ) * x;
      }
      double inch_to_m( double x ) {
         return inch_to_cm( x ) / 100;
      }
      double m_to_inch( double x ) {
         return cm_to_inch( x ) * 100;
      }
      double inch_to_mm( double x ) {
         return inch_to_cm( x ) * 10;
      }
      double mm_to_inch( double x ) {
         return cm_to_inch( x ) / 10;
      }


      // dx == +- Abstand vom Zentrum
      // stddev = Standardabweichung vom Zentrum

      double gaussfunction( double stddev, double dx ) {
         double piunderone = 1.0 / ( sqrt( 2 * PI ) );
         double exponent = dx * dx;
         exponent *= -0.5;
         exponent /= stddev * stddev;
         double gauss = exp( exponent );
         gauss *= piunderone;
         gauss /= stddev;
         return gauss;
      }

      double gaussfunction( double stddev0, double dx0, double stddev1, double dx1 ) {
         //double piunderone = 1.0/(sqrt(2*PI));
         double f0 = dx0 * dx0 / ( stddev0 * stddev0 );
         double f2 = dx1 * dx1 / ( stddev1 * stddev1 );

         double exponent = f0 + f2;
         exponent *= -0.5;

         double gauss = exp( exponent );

         gauss /= 2 * PI;

         gauss /= stddev0;
         gauss /= stddev1;

         return gauss;
      }

      /* -- Gauss Faltung mit Kern (1 2 1), fr histogram arrays
      */
      void gauss_convolution( vector<uint32_t> arr ) {
         auto size = arr.size();
         vector<uint32_t> v( size + 2 );

         size_t i = 0;

         for( i = 1; i < size + 1; i++ ) {
            v[i] = arr[i - 1];
         }

         v.front() = arr[0];
         v.back()  = arr[size - 1];

         i = 0;

         while( size-- ) {
            arr[i] = ( v[i] + ( v[i + 1] << 1 ) + v[i + 2] ) >> 2;
            i++;
         }
      }
      void gauss_convolution( uint32_t* arr, int size ) {
         vector<uint32_t> v( size + 2 );

         int i = 0;

         for( i = 1; i < size + 1; i++ ) {
            v[i] = arr[i - 1];
         }

         v.front() = arr[0];
         v.back()  = arr[size - 1];

         i = 0;

         while( size-- ) {
            arr[i] = ( v[i] + ( v[i + 1] << 1 ) + v[i + 2] ) >> 2;
            i++;
         }
      }

      void gauss_convolution( uint8_t* arr, int size ) {
         vector<uint8_t> v( size + 2 );
         int i = 0;

         for( i = 1; i < size + 1; i++ ) {
            v[i] = arr[i - 1];
         }

         v.front() = arr[0];
         v.back()  = arr[size - 1];

         i = 0;

         while( size-- ) {
            arr[i] = ( v[i] + ( v[i + 1] << 1 ) + v[i + 2] ) >> 2;
            i++;
         }
      }


      double rand_gauss() {
         static double dval1, dval2, sum;
         static bool phase = false;
         double gauss;

         if( phase == 0 ) {
            do {
               dval1 = 2.0 * rand_double() - 1.0;
               dval2 = 2.0 * rand_double() - 1.0;
               sum = dval1 * dval1 + dval2 * dval2;
            } while( sum >= 1.0 || sum == 0.0 );

            gauss = dval1 * sqrt( -2 * log( sum ) / sum );
         } else {
            gauss = dval2 * sqrt( -2 * log( sum ) / sum );
         }

         phase = !phase;
         return gauss;
      }

      const unsigned long a = 16807;          /* multiplier */
      const unsigned long m = INT_MAX;        /* 2**31 - 1 */
      //#define q 127773L       /* m div a */
      //#define r 2836          /* m mod a */

      static long nextlongrand( long seed ) {
         unsigned long lo, hi;
         lo = a * ( long )( seed & 0xFFFF );
         hi = a * ( long )( ( unsigned long )seed >> 16 );
         lo += ( hi & 0x7FFF ) << 16;

         if( lo > m ) {
            lo &= m;
            ++lo;
         }

         lo += hi >> 15;

         if( lo > m ) {
            lo &= m;
            ++lo;
         }

         return ( long )lo;
      }

      static long randomnum = 1;

      /* -- return next random long
      */
      long rand_long( void ) {
         randomnum = nextlongrand( randomnum );
         return randomnum;
      }


      double rand_double( void ) {
         randomnum = nextlongrand( randomnum );
         return( ( double )randomnum / ( double )INT_MAX );
      }

      void rand_long_seed( unsigned long seed ) {
         randomnum = seed ? ( seed & m ) : 1;  /* nonzero seed */
      }

      void rand_double_seed( double dseed ) {
         unsigned long seed;
         seed = ( unsigned long )( dseed * ( double )( INT_MAX ) );
         rand_long_seed( seed );
      }

      int roundToInt( double d ) {
         double dfloor    = floor( d );
         double drem = d - dfloor;

         if( drem > 0.5 ) {
            return ( int )ceil( d );
         }

         return ( int )dfloor;
      }
      int Min( int x, int y ) {
         int r = y + ( ( x - y ) & -( x < y ) ); // min(x, y)
         return r;
      }
      int Max( int x, int y ) {
         int r = x - ( ( x - y ) & -( x < y ) ); // max(x, y)
         return r;
      }


      /* -- shellsort
      */
      void    shell_sort( int* base, int n ) {
         int i, j;
         int gap, tmp ;
         int* p1, *p2;

         for( gap = 1; gap <= n; gap = 3 * gap + 1 ) {}

         for( gap /= 3;  gap > 0  ; gap /= 3 )
            for( i = gap; i < n; i++ )
               for( j = i - gap; j >= 0 ; j -= gap ) {
                  p1 = base + ( j );
                  p2 = base + ( ( j + gap ) );

                  if( *p1  <= *p2 ) {
                     break;
                  }

                  tmp   = *p1;
                  *p1 = *p2;
                  *p2 = tmp;
               }
      }
      void    shell_sort( vector<size_t> &base, size_t n ) {
         int i, j;
         int gap, tmp ;
         int nn = n;

         for( gap = 1; gap <= nn; gap = 3 * gap + 1 ) {}

         for( gap /= 3;  gap > 0  ; gap /= 3 )
            for( i = gap; i < nn; i++ )
               for( j = i - gap; j >= 0 ; j -= gap ) {
                  auto int1 = base[j];
                  auto int2 = base[ j + gap ];

                  if( int1  <= int2 ) {
                     break;
                  }

                  tmp   = base[j];
                  base[j] = base[ j + gap ];
                  base[ j + gap ] = tmp;
               }
      }

      /*
      void    utl_shell_sort( WORD *base, int n ){
          int i, j;
          int gap;
          WORD *p1, *p2;
          for( gap = 1; gap <= n; gap = 3*gap + 1 );

          for( gap /= 3;  gap > 0  ; gap /= 3 )
              for( i = gap; i < n; i++ )
                  for( j = i-gap; j >= 0 ; j -= gap ){
                      p1 = base + ( j   );
                      p2 = base + ((j+gap) );

                      if( *p1  <= *p2 )
                          break;
                      WORD tmp   = *p1;
                      *p1 = *p2;
                      *p2 = tmp;
      }
      }
      */




      /*********************************/
      /*          quicksort            */
      /*                               */
      /*********************************/
      void   quick_sort( int lo, int hi, uint8_t* base ) {
         int i, j;
         uint8_t  pivot, temp;

         if( lo < hi ) {
            for( i = lo, j = hi, pivot = base[hi]; i < j; ) {
               while( i < hi && base[i] <= pivot ) {
                  ++i;
               }

               while( j > lo && base[j] >= pivot ) {
                  --j;
               }

               if( i < j ) {
                  temp = base[i];
                  base[i] = base[j];
                  base[j] = temp;
               }
            }

            temp = base[i];
            base[i] = base[hi];
            base[hi] = temp;

            quick_sort( lo, i - 1, base );
            quick_sort( i + 1, hi, base );
         }
      }


      /*
       void heap_sort_o( int *arr, int n ){
         utl_heap_sort( int *arr, int n );
      }
      */


      /* -- heap sort
      */
      void heap_sort( int* arr, int n ) {
         int l, j, ir, i;
         int* ra = arr - 1;
         int rra;
         l = ( n >> 1 ) + 1;
         ir = n;

         for( ;; ) {
            if( l > 1 ) {
               rra = ra[--l];
            } else {
               rra = ra[ir];
               ra[ir] = ra[1];

               if( --ir == 1 ) {
                  ra[1] = rra;
                  return;
               }
            }

            i = l;
            j = l << 1;

            while( j <= ir ) {
               if( j < ir && ra[j] < ra[j + 1] ) {
                  ++j;
               }

               if( rra < ra[j] ) {
                  ra[i] = ra[j];
                  j += ( i = j );
               } else {
                  j = ir + 1;
               }
            }

            ra[i] = rra;
         }
      }
      void heap_sort( uint16_t* arr, int n ) {
         int l, j, ir, i;
         uint16_t* ra = arr - 1;
         uint16_t rra;
         l = ( n >> 1 ) + 1;
         ir = n;

         for( ;; ) {
            if( l > 1 ) {
               rra = ra[--l];
            } else {
               rra = ra[ir];
               ra[ir] = ra[1];

               if( --ir == 1 ) {
                  ra[1] = rra;
                  return;
               }
            }

            i = l;
            j = l << 1;

            while( j <= ir ) {
               if( j < ir && ra[j] < ra[j + 1] ) {
                  ++j;
               }


               if( rra < ra[j] ) {
                  ra[i] = ra[j];
                  j += ( i = j );
               } else {
                  j = ir + 1;
               }
            }

            ra[i] = rra;
         }
      }

      void linear_zoom( const vector<uint8_t> &s,
                        vector<uint8_t>  &t ) {

         double ds = 1.0 / s.size();
         double dt = 1.0 / t.size();

         for( int i = 0; i < ( int )t.size(); i++ ) {
            double x = i * dt;
            int x1   = ( int )( x / ds );
            int x2   = x1 + 1;

            if( x2 == ( int )s.size() ) {
               t[i] = s[x1];
            } else {
               double_xy xy0( x1 * ds, s[x1] );
               double_xy xy1( x2 * ds, s[x2] );
               double inter = interpolate( x, xy0, xy1 );
               t[i] = ( uint8_t )inter;
            }
         }
      }

   } // end of namespace
}


#ifdef TEST_RANDOM_NUMBERS

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char* argv[] ) {
   long reps, k, num;
   unsigned long seed;

   reps = 10000;
   seed = 1;

   /*
   ** correctness test: after 10000 reps starting with seed 1,
   ** result should be 1043618065
   */

   if( argc > 1 ) {
      reps = atol( argv[1] );
   }

   if( argc > 2 ) {
      seed = atol( argv[2] );
   }

   printf( "seed %ld for %ld reps...\n", seed, reps );
   slongrand( seed );

   for( k = 0; k < reps; ++k ) {
      num = longrand();
   }

   printf( "%ld\n", num );

   return 0;
}


#endif /* TEST_RANDOM_NUMBERS */




//EOF


