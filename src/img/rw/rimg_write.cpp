
/*
--------------------------------------------------------------------------
Copyright 2013 by Richard Albrecht
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
/*! \file rimg_rw_raw.cpp
\brief impl for read and write raw images

Lib: librimgrw
*/



#include <iostream>
#include <sstream>
#include <string>


#include "rimg_rw_exceptions.h"


#include "rimg_linear.h"
#include "rimg_planar.h"
#include "rimg_read_write.h"
#include "rimg_fs.h"
#include "rimg_resolve_image_type.h"
#include "rimg_t_write.h"
#include "e_file_type.h"

using namespace std;

using rlf::rimg_planar::tImgPlanar;
using rlf::rimg_planar::tImgViewPlanar;
using rlf::rimg_linear::tImgViewLinear;

namespace rlf {

   namespace rimg_rw {

      namespace msg {
         const string marker = "%s";
         string replace( string const& msg, string const& s0 ) {
            string temp = msg;

            if( s0.size() > 0 ) {
               size_t pos = msg.find( marker );

               if( pos != ::string::npos ) {
                  temp.erase( pos, marker.size() );
                  temp.insert( pos, s0 );
               }
            }

            return temp;

         }

         const string msg_file_not_exists = "File doesn't exist: '%s'";
         const string msg_file_exists = "File exists: '%s'";


         string file_not_exists( string const& s0 ) {
            return replace( msg_file_not_exists, s0 );
         }


         string file_exists( string const& s0 ) {
            return replace( msg_file_exists, s0 );
         }

      } // end of ns msg



      shared_ptr<t_write> create_writer( string const& fn ) {
         string ext = rlf::fs::to_lower( rlf::fs::extension( fn ) );

         if( ext == rlf::rimg_rw::dot_tif || ext == rlf::rimg_rw::dot_tiff ) {
            return shared_ptr<t_write>( new  t_write_tiff() );
         }

         if( ext == rlf::rimg_rw::dot_bmp ) {
            return shared_ptr<t_write>( new  t_write_bmp() );
         }

         return nullptr;
      }

      // jpeg and png with compression or quality
      shared_ptr<t_write> create_c_writer( string const& fn, size_t c ) {
         string ext;
         ext = rlf::fs::extension( fn );
         ext = rlf::fs::to_lower( ext );

         if( ext == rlf::rimg_rw::dot_png ) {
            return shared_ptr<t_write>( new  t_write_png( c ) );
         }

         if( ext == "." + rlf::rimg_rw::dot_jpeg || ext == "." + rlf::rimg_rw::dot_jpg ) {
            return shared_ptr<t_write>( new  t_write_jpeg( c ) );
         }

         return nullptr;
      }

      void write( string const& fn, tImgViewLinear const& img, bool override_, size_t c )   {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         if( c != size_t( -1 ) ) {
            string ext;
            ext = rlf::fs::extension( fn );
            ext = rlf::fs::to_lower( ext );

            if( ext == rlf::rimg_rw::dot_png ) {
               return write_png( fn, img, override_, c );
            }

            if( ext == "." + rlf::rimg_rw::dot_jpeg || ext == "." + rlf::rimg_rw::dot_jpg ) {
               return write_jpeg( fn, img, override_, c );
            }
         }

         shared_ptr<t_write> writer = create_writer( fn );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }
      }
      void write_png( string const& fn, tImgViewLinear const& img, bool override_, size_t c )   {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         shared_ptr<t_write> writer = create_c_writer( fn, c );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }
      }
      void write_jpeg( string const& fn, tImgViewLinear const& img, bool override_, size_t c )   {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         shared_ptr<t_write> writer = create_c_writer( fn, c );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }
      }


      void write( string const& fn, tImgPlanar const& img, bool override_, size_t c ) {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         if( c != size_t( -1 ) ) {
            string ext;
            ext = rlf::fs::extension( fn );
            ext = rlf::fs::to_lower( ext );

            if( ext == rlf::rimg_rw::dot_png ) {
               return write_png( fn, img, override_, c );
            }

            if( ext == rlf::rimg_rw::dot_jpeg || ext == rlf::rimg_rw::dot_jpg ) {
               return write_jpeg( fn, img, override_, c );
            }


         }

         shared_ptr<t_write> writer = create_writer( fn );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }

      }
      void write_png( string const& fn, tImgPlanar const& img, bool override_, size_t c ) {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         shared_ptr<t_write> writer = create_c_writer( fn, c );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }

      }
      void write_jpeg( string const& fn, tImgPlanar const& img, bool override_, size_t c ) {
         if( ! override_ ) {
            if( fs::file_exists( fn ) ) {
               throw tImgWriteEx( msg::file_exists( fn ) );
            }
         }

         shared_ptr<t_write> writer = create_c_writer( fn, c );

         if( writer != nullptr ) {
            writer->write( fn, img );
            return;
         }

      }



   }


}
//EOF





