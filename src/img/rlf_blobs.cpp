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
lib img
C++, VC7, GCC

-- tBlobs.cpp $
--
*/




#include <assert.h>

#include <fstream>
#include <list>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>

#include "str_util.h"

#include "rlf_blob.h"
#include "rlf_blobs.h"

#include "rlf_math.h"


const size_t auto_null = 0;
// auto keyword in VS2010
// http://blogs.msdn.com/b/vcblog/archive/2011/09/12/10209291.aspx

namespace text_write {

   namespace wl {
      namespace err {
         const string marker = "%s";
         inline string replace( string const& msg, string const& s0 = "" ) {
            string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }

         const string msg_file_exists = "File exists: '%s'";
         const string msg_write_file = "Couldn't write file: '%s'";
         const string msg_open_file = "Couldn't open file for write: '%s'";
         const string msg_text_empty = "Text has no lines: '%s'";

         inline string write_file( string const& s0 ) {
            return replace( msg_write_file, s0 );
         }
         inline string file_exists( string const& s0 ) {
            return replace( msg_file_exists, s0 );
         }
         inline string file_open( string const& s0 ) {
            return replace( msg_open_file, s0 );
         }
         inline string text_empty( string const& s0 ) {
            return replace( msg_text_empty, s0 );
         }

      }

      inline bool file_exists( boost::filesystem::path const& p ) {
         if( !boost::filesystem::is_regular_file( p ) ) {
            return false;
         }

         boost::filesystem::file_status s = status( p );

         if( boost::filesystem::exists( s ) ) {
            return true;
         }

         return false;
      }

   }


   /*! BadAsciiWrite,
   Exception, if text write fails
   \param [in] msg  error message
   */
   class BadAsciiWrite: public runtime_error {
   public:
      BadAsciiWrite( const string& msg )
         : runtime_error( msg ) { }
   };

   namespace {
      class writer {
         ofstream& _fp;
         string _f;
      public:
         writer( ofstream& fp, string f ): _fp( fp ), _f( f ) {}
         void operator()( string const& s ) {
            _fp << s << endl;

            if( _fp.bad() ) {
               throw BadAsciiWrite( wl::err::write_file( _f ) );
            }
         }
      };
   }

   /*! \class tWriteAscii
   *  \brief writes ascii files
   *
   * an asciifile is stored in a <b>string</b> list<br>
   */
   class tWriteAscii  {
      // no copy allowed
      tWriteAscii( const tWriteAscii& in );
      tWriteAscii& operator= ( const tWriteAscii& in );

   public:
      tWriteAscii() {}
      ~tWriteAscii() {}
      void operator()( list<string> const& lines , const string& file ) {
         boost::filesystem::path pa( file );

         //if( wl::file_exists( pa ) ) {
         //  throw BadAsciiWrite( wl::err::file_exists( file ) );
         //}

         if( lines.size() == 0 ) {
            throw BadAsciiWrite( wl::err::text_empty( file ) );
         }

         if( lines.size() > 0 ) {
            ofstream fp( file.c_str() );

            if( fp.bad() ) {
               throw BadAsciiWrite( wl::err::file_open( file ) );
            }

            for_each( lines.begin(), lines.end(), writer( fp, file ) );
         }


      }

   };

}// end of ns text_write


namespace rlf {


   class tBlobsImpl {

      vector<tBlob> _blobs;

      uint32_t _background;
      uint32_t _threshold;
      uint32_t _min_area;
      uint32_t _max_area;
      uint32_xy _img_size;
      bool mNoBorderContact;

      tBlobsImpl(): _blobs(),
         _background( 0 ),
         _threshold( 0 ),
         _min_area( 0 ),
         _max_area( -1 ),
         mNoBorderContact( false ) {}



      bool operator==( const tBlobsImpl& s )const {
         auto size1 = _blobs.size();
         auto size2 = s._blobs.size();

         if( size1 != size2 ) {
            return false;
         }

         auto i1 = _blobs.begin();
         auto i2 = s._blobs.begin();

         for( size_t i = 0; i < size1; i++ ) {
            if( *i1 != *i2 ) {
               return false;
            }

            ++i1;
            ++i2;
         }

         return true;
      }


      void calculateMidpnt() {
for( auto & b : _blobs ) {
            b.calculateMidpnt();
         }
      }

      void calculateSize() {
for( auto & b : _blobs ) {
            b.calculateSize();
         }
      }




      void calculateIntegral() {
for( auto & b : _blobs ) {
            b.calculateIntegral( _background );
         }
      }




      void calculateArea() {
for( auto & b : _blobs ) {
            b.calculateArea();
         }
      }



      void calculatePeakValue() {
for( auto & b : _blobs ) {
            b.calculatePeakValue();
         }
      }


      friend class tBlobs;

   };



   tBlobs::tBlobs(): _impl( new tBlobsImpl() ) {}


   tBlobs::~tBlobs() {}


   bool tBlobs::operator==( const tBlobs& s )const {
      return _impl->operator==( *s._impl );
   }
   tBlobs& tBlobs::operator=( const tBlobs& s ) {
      if( this != &s ) {
         *_impl = *s._impl;
      }

      return *this;
   }
   tBlobs::tBlobs( const tBlobs& s ): _impl( new tBlobsImpl( *s._impl ) ) {}


   tBlob& tBlobs::operator[]( uint32_t i ) {
      return _impl->_blobs[i];
   }
   tBlob const& tBlobs::operator[]( uint32_t i )const {
      return _impl->_blobs[i];
   }


   uint32_t  tBlobs::background()const {
      return _impl->_background;
   }
   uint32_t& tBlobs::background() {
      return _impl->_background;
   }

   uint32_t& tBlobs::min_area() {
      return  _impl->_min_area;
   }
   uint32_t tBlobs::min_area()const {
      return _impl->_min_area;
   }

   uint32_t tBlobs::max_area()const {
      return _impl->_max_area;
   }
   uint32_t& tBlobs::max_area() {
      return _impl->_max_area;
   }


   void tBlobs::calculateMidpnt() {
      _impl->calculateMidpnt();
   }


   void tBlobs::calculateSize() {
      _impl->calculateSize();
   }



   void tBlobs::calculateIntegral() {
      _impl->calculateIntegral();
   }

   void tBlobs::calculateArea() {
      _impl->calculateArea();
   }


   void tBlobs::calculatePeakValue() {
      _impl->calculatePeakValue();
   }

   inline uint32_xy to( int32_xy xy ) {
      return uint32_xy( xy.x(), xy.y() );
   }

   void writeToImage( tBlobs const& bl, tImgPlanar& img, int color ) {
      writeToImage( bl,  img, color, color );
   }

   void writeToImage( tBlobs const& bl,  tImgPlanar& img, int color, int bordercolor ) {
      if( bl.empty() ) {
         return;
      }

      struct wimage {
         tBlobs const& bl;
         tImgPlanar& img;
         int color;
         int bordercolor;

         wimage(
            tBlobs const& bl_,
            tImgPlanar& img_,
            int color_,
            int bordercolor_ )
            : bl( bl_ ), img( img_ ), color( color_ ), bordercolor( bordercolor_ ) {}

         void operator()( tBlob const& blob ) {
            if( bl.applyFilter( blob ) ) {
               WriteToImage( blob.streaks(), img, color, bordercolor );
            }

         }
      };

      for_each( bl.blobs().begin(), bl.blobs().end(), wimage( bl,  img, color, bordercolor ) );
   }


	 uint32_t tBlobs::count()const {
		 return static_cast<uint32_t>(_impl->_blobs.size());
   }
   vector<tBlob> const& tBlobs::blobs()const {
      return _impl->_blobs;
   }


   uint32_t tBlobs::threshold()const {
      return _impl->_threshold;
   }
   uint32_t& tBlobs::threshold() {
      return _impl->_threshold;
   }


   void tBlobs::push_back( const tBlob& s ) {
      _impl->_blobs.push_back( s );
   }

   bool tBlobs::empty() const {
      return _impl->_blobs.size() == 0;
   }


   namespace {


		 void extraxt_streaks(vector<tStreak> & streaks, tPlane8 const& b, uint32_t thre) {


				uint32_t y = 0;
         tStreak s;
         auto ybegin = b.begin();
         auto yend = b.size();

         while( y < yend ) {
            auto isstreak =   false;
            uint32_t x     = 0;

            auto begin = ybegin->begin();
            auto end = ybegin->end();

            while( begin < end ) {
               if( *begin > thre ) {
                  if( isstreak == false ) {
                     isstreak = true;
                     s = tStreak::Create( uint32_xy( x, y ) );

                  }

                  s.add( *begin );
               } else {
                  if( isstreak == true ) {
                     streaks.push_back( s );
                     isstreak = false;
                  }
               }

               ++begin;
               ++x;
            }

            // end of line reached
            if( isstreak == true ) {
               streaks.push_back( s );
               isstreak = false;
            }

            ++ybegin;
            y++;
         }
      }


      void seek_overlapping_streaks_in_line(
         vector<tStreak> const& StreakLine,
         vector<bool> & ControlTableLine ,
         tBlob& binaryObject,
         tStreak  current
      ) {
         static size_t null = 0;

         auto countEnd = StreakLine.size();

         if( countEnd == null ) {
            return;
         }

         auto count = null;

         while( count < countEnd ) {
            if( ControlTableLine[count] != false ) {
               if( ( StreakLine[count].x()  <  current.end() ) && ( current.x()  <  StreakLine[count].end() ) ) {
                  binaryObject.push_back( StreakLine[count] );
                  ControlTableLine[count] = false;
               }
            }

            count++;
         }
      }


      void seek_adjacent_streak(
         vector< vector<tStreak> > const& Table, int lines,
         vector< vector<bool> > & ControlTable,
         tBlob& blob,
         tStreak current
      ) {


         //int lines = Table.binObjects.size();
         int currentLine8 = current.y();

         // not last line
         if( currentLine8 < lines - 1 ) {
            int nextLine8     = currentLine8 + 1;
            seek_overlapping_streaks_in_line( Table[nextLine8], ControlTable[nextLine8], blob, current );
         }

         // not top line
         if( currentLine8 > 0 ) {
            auto previousLine     = currentLine8  -  1;
            seek_overlapping_streaks_in_line( Table[previousLine], ControlTable[previousLine], blob, current );
         }
      }

      void streaks_to_blobs( vector<tStreak> const& streaks, size_t lines, vector<tBlob> & blobs ) {
         static size_t null = 0;

         vector< vector<bool> >bool_table( lines );
         vector< vector<tStreak> >streak_table( lines );

         auto begin = streaks.begin();
         auto end = streaks.end();

         // sort streaks by line (y)
         // and add to bool_table
         while( begin != end ) {
            auto yindex = begin->y();
            streak_table[ yindex ].push_back( *begin );
            bool_table[ yindex ].push_back( true );
            ++begin;
         }

         // now all streaks in table
         auto lineCount = null;

         // search 1'st in line
         while( lineCount < lines ) {
            auto  bool_table_line = bool_table[lineCount].begin();

            auto line_begin = streak_table[lineCount].begin();
            auto line_end = streak_table[lineCount].end();

            // search, until line in bool_table is true
            for( ; line_begin < line_end && *bool_table_line == false; ++line_begin, ++bool_table_line ) {}

            if( line_begin < line_end ) {   // found, collect blob, until no neihbours found
               tBlob blob;
               blob.push_back( *line_begin );
               *bool_table_line = false;

               int found = 0;
               int s = blob.streak_count();

               while( s != found ) {
                  // searches for neighbours for all streaks
                  seek_adjacent_streak( streak_table, static_cast<uint32_t>(lines), bool_table, blob, blob[found] ); // seach more streaks,
                  found++;
                  s = blob.streak_count();
               }

               blobs.push_back( blob );
            } else {
               lineCount++;
            }
         }

      }

   }

   // search for overlapping streaks in line
   //            xxxxxxxx                  current
   //       xxxxxx         xxxxxx   xxx    line
   //        ok             not ok
   //

   //                    ^  current.End()
   //            xxxxxxxxe
   //                    xxxxxx                StreakLine[count].X()  <  current.End() - false
   //                   xxxxxx                 StreakLine[count].X()  <  current.End() - true
   //                  xxxxxx                  StreakLine[count].X()  <  current.End() - true
   //                 xxxxxx                   StreakLine[count].X()  <  current.End() - true
   //                 ^  X()
   //

   //            ^ current.X()
   //            xxxxxxxx
   //      xxxxxxe                           current.X()  <  StreakLine[count].End() - false
   //       xxxxxxe                                      current.X()  <  StreakLine[count].End() - true
   //        xxxxxxe                                     current.X()  <  StreakLine[count].End() - true
   //         xxxxxxe                                current.X()  <  StreakLine[count].End() - true
   //               ^  End()
   //
   //
   // return: new top of streak list


   void tBlobs::extract( const tImgPlanar& im ) {

      _impl->_blobs.clear();
      _impl->_blobs.reserve( RESERVE_SIZE );

      _impl->_img_size = im.size();

      vector<tStreak> estreaks;
      tPlane8 const& p8 = im.mono8();
      extraxt_streaks( estreaks, p8, _impl->_threshold );

      if( estreaks.empty() ) {
         return;
      }

      streaks_to_blobs( estreaks, im.sy(), _impl->_blobs );
   }


   tBlob tBlobs::getBlobAt( uint32_xy const& xy ) {
      if( empty() ) {
         return tBlob();
      }

      size_t running_number = 1;
      size_t size_ = count();

      for( size_t i = 0; i < size_; i++ ) {
         tBlob const& bo = _impl->_blobs[i];

         if( applyFilter( bo ) ) {
            bool found = bo.inside( xy );
            //const_cast<tBlob&>( bo ).Nr( running_number );

            if( found ) {
               return bo;
            }

            running_number++;
         }
      }

      return tBlob();
   }
   bool tBlobs::applyFilter( tBlob const& obj )const {
      if( _impl->mNoBorderContact == true ) {
         if( obj.ConnectedWithImageBorder( _impl->_img_size ) == false ) {
            if( ( int )obj.area() >= ( int )_impl->_min_area ) {
               return true;
            }
         }
      } else {
         if( ( int )obj.area() >= ( int )_impl->_min_area ) {
            return true;
         }
      }

      return false;
   }

	 uint32_t  tBlobs::count_filtered()const {



      if( empty() ) {
         return 0;
      }



			uint32_t running = 0;
			uint32_t size_ = count();

      for( size_t i = 0; i < size_; i++ ) {
         tBlob const& bo = _impl->_blobs[i];

         if( applyFilter( bo ) ) {
            running++;
         }
      }

      return running;
   }

   list<string> CollectParameterList( tBlobs const& bl ) {
      list<string> list;
      int nr = bl.count_filtered();
      string d4 = str::toString( nr, 4 );
      list.push_back( "Objectlist: " + d4 + " objects found." );
      string sthre = str::toString( ( int )bl.threshold(), 4 );
      string sbgr = str::toString( bl.background(), 4 );
      string sminarea = str::toString( bl.min_area(), 4 );

      list.push_back( "Parameter: threshold " + sthre + ", subtract " + sbgr + ", minarea " + sminarea );

      size_t size = bl.count();
      vector<double> array;
      double sumintegral = 0;
      double sumarea     = 0;
      int count          = 0;
      int running_number = 1;

      for( size_t i = 0; i < size; i++ ) {
         tBlob const& bo = bl.blobs()[i];

         if( bl.applyFilter( bo ) ) {
            string rn = str::toString( ( int )running_number++, 3 );
            string posx = str::toString( ( int )bo.midpoint().x(), 4 );
            string posy = str::toString( ( int )bo.midpoint().y(), 4 );
            string sx = str::toString( ( int )bo.sx(), 3 );
            string sy = str::toString( ( int )bo.sy(), 3 );
            string area = str::toString( ( int )bo.area(), 5 );
            string integral = str::toString( ( int )bo.integral(), 7 );
            uint32_t a = bo.area();
            string i_a = "no value";

            if( a > 0 ) {
               i_a = str::toString( bo.integral() / double( bo.area() ), 6, 2 );
            }

            string s = "n:" + rn + ", x:" + posx + ", y:" + posy + ", dxy:" + sx + "," + sy
                       + ", area: " + area + ", integral: " + integral + ", i/a:" + i_a + "";

            list.push_back( s );

            sumintegral += bo.integral();
            sumarea += bo.area();
            array.push_back( bo.integral() / double( bo.area() ) );
            count++;
         }
      }

      double mean = rlf::math::mean( array );
      double stddev = rlf::math::std_dev( array, mean );

      list.push_back( "" );

      string scount = str::toString( count );
      string smean = str::toString( mean, 6, 2 );
      string sstddev = str::toString( stddev, 6, 2 );
      string s = "n: " + scount + ", relative intensity mean: " + smean + ", stddev: " + sstddev + "";
      list.push_back( s );

      string ssumarea = str::toString( sumarea );
      string ssumintegral = str::toString( sumintegral, 12, 0 );
      string ssumintegral_sumarea = "no value";

      if( sumarea > 0 ) {
         ssumintegral_sumarea = str::toString( sumintegral / sumarea, 6, 2 );
      }

      s =  "sumarea: " + ssumarea + ", sumintegral: "
           + ssumintegral + ", relative intensity : " + ssumintegral_sumarea + "" ;
      list.push_back( s );

      return list;
   }



   int writeParam( tBlobs const& bl, string const& fn ) {
      size_t size = bl.blobs().size();

      if( size == 0 ) {
         return -1;
      }

      list<string> list = CollectParameterList( bl );

      try {
         text_write::tWriteAscii()( list, fn );
      } catch( text_write::BadAsciiWrite& bw ) {
         string err = bw.what();
         return -1;
      }

      return 0;
   }
}

// search for overlapping streaks in line
//            xxxxxxxx                  current
//       xxxxxx         xxxxxx   xxx    line
//        ok             not ok
//

//                    ^  current.End()
//            xxxxxxxxe
//                    xxxxxx                StreakLine[count].X()  <  current.End() - false
//                   xxxxxx                 StreakLine[count].X()  <  current.End() - true
//                  xxxxxx                  StreakLine[count].X()  <  current.End() - true
//                 xxxxxx                   StreakLine[count].X()  <  current.End() - true
//                 ^  X()
//

//            ^ current.X()
//            xxxxxxxx
//      xxxxxxe                           current.X()  <  StreakLine[count].End() - false
//       xxxxxxe                                      current.X()  <  StreakLine[count].End() - true
//        xxxxxxe                                     current.X()  <  StreakLine[count].End() - true
//         xxxxxxe                                current.X()  <  StreakLine[count].End() - true
//               ^  End()
//
//
// return: new top of streak list


//EOF
