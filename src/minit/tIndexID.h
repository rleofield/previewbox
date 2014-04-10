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

#ifndef tIndexID_H
#define tIndexID_H

#include <assert.h>
#include <string>
#include <float.h>
#include <vector>
#include <cmath>
#include <cstdint>


namespace rlf_minit {



   class tIndexID {

      size_t _index;
   protected :
      tIndexID( size_t i ): _index( i ), INVALID_ID( 0 ) {}
   public:
      size_t operator()()const {
         return _index;
      }

      virtual ~tIndexID() {}
      tIndexID& operator=( tIndexID const& id ) {
         if( this != &id ) {
            _index = id._index;
         }

         return *this;
      }
      bool operator==( tIndexID const& id )const {
         return id._index == _index;
      }
      bool operator!=( tIndexID const& id )const {
         return id._index != _index;
      }
      tIndexID const& newUniqueID() {
         _index++;
         return *this;
      }
      bool isValid()const {
         return _index != INVALID_ID;
      }
      void resetIndexTo( size_t i ) {
         _index = i;
      }
      size_t index()const {
         return _index;
      }

      const size_t INVALID_ID;

   };


   class tLayerID: public tIndexID {
   public :
      tLayerID(): tIndexID( INVALID_ID ) {}
      explicit tLayerID( size_t i ): tIndexID( i ) {}

      tLayerID& operator=( tLayerID const& id ) {
         if( this != &id ) {
            tIndexID::operator=( id );
         }

         return *this;
      }

      bool operator!=( tLayerID const& id )const {
         return tIndexID::operator!=( id );
      }
      bool operator==( tLayerID const& id )const {
         return tIndexID::operator()() == id();
      }

      operator int16_t()const {
         return static_cast<int16_t>( index() );
      }

      bool operator<( tLayerID const& id )const {
         return tIndexID::operator()() < id();
      }
      void reset() {
         resetIndexTo( INVALID_ID );
      }
      size_t index()const {
         return tIndexID::index();
      }
      bool isValid()const {
         return  tIndexID::isValid();
      }


   };

   class tCamID: public tIndexID {

   public :
      tCamID(): tIndexID( INVALID_ID ) {}

      explicit tCamID( size_t i ): tIndexID( i ) {}

      size_t operator()()const {
         return index();
      }
      tCamID operator=( tCamID const& id ) {
         if( this != &id ) {
            tIndexID::operator=( id );
         }

         return *this;
      }

      bool operator==( tCamID const& id )const {
         return tIndexID::operator==( id );
      }
      bool operator!=( tCamID const& id )const {
         return tIndexID::operator!=( id );
      }
      void reset() {
         resetIndexTo( INVALID_ID );
      }

      bool operator<( tCamID const& id )const {
         return tIndexID::operator()() < id();
      }



   };



   class tCanvasID: public tIndexID {
   public :
      tCanvasID(): tIndexID( INVALID_ID ) {}
      tCanvasID( size_t i ): tIndexID( i ) {}
      size_t operator()()const {
         return tIndexID::operator()();
      }
      tCanvasID operator=( tCanvasID const& id ) {
         if( this != &id ) {
            tIndexID::operator=( id );
         }

         return *this;
      }
      bool operator!=( tCanvasID const& id )const {
         return tIndexID::operator!=( id );
      }
      bool operator==( tCanvasID const& id )const {
         return tIndexID::operator()() == id();
      }
      void reset() {
         resetIndexTo( INVALID_ID );
      }
      bool operator<( tCanvasID const& id )const {
         return tIndexID::operator()() < id();
      }


   };


}


#endif



//EOF
