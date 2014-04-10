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

/*! \file rlf_blobs.h
\brief blob analyse

*/


/*
Workfile: cBinObjectList.h
*/


#ifndef TBINARYOBJECTLIST_H
#define TBINARYOBJECTLIST_H

#include <vector>
#include <list>
#include "rlf_blob.h"

using namespace rlf::rimg_planar;

namespace rlf {


   class tBlobsImpl;

   class tBlobs {
      enum { RESERVE_SIZE = 2000 };

      tBlobsImpl* _impl;

      //void resize( int i );

      void push_back( const tBlob& s );

   public:
      tBlobs();
      ~tBlobs();


      bool operator==( const tBlobs& s )const;
      tBlobs& operator=( const tBlobs& s );
      tBlobs( const tBlobs& s );

      /* -- OPERATORS --
      */
      tBlob& operator[]( uint32_t i );
      tBlob const& operator[]( uint32_t i )const;


      /* -- OPERATIONS --
      */
      //void clear();
      bool empty()const;

      void calculateMidpnt();
      void calculateSize();
      void calculateIntegral();
      void calculateArea();
      void calculatePeakValue();



      vector<tBlob> const& blobs()const ;

      uint32_t threshold()const ;
      uint32_t& threshold() ;

      uint32_t background()const;
      uint32_t& background();

      uint32_t& min_area();
      uint32_t min_area()const;

      uint32_t& max_area();
      uint32_t max_area()const;

      // operations
      void extract( const tImgPlanar& im );

      tBlob getBlobAt( uint32_xy const& xy );

      bool applyFilter( tBlob const& obj )const ;
      size_t count( )const ;
      size_t count_filtered( )const ;

   };

   int writeParam( tBlobs const& bl, string const& fn );
   void writeToImage( tBlobs const& bl,  tImgPlanar& p, int color, int bordercolor = -1 );
}
#endif

//EOF

