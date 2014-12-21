#include <boost/range/algorithm_ext/iota.hpp>

#include "imagecompany.h"
#include "t_filename.h"

using rlf_filefn::t_filename;
using rlf::tSize;
using rlf::tBGRA;
using rlf::tRGBA;
using rlf::tRGB;

using namespace rlf::rimg_planar;

class incrementQRgb {
   QRgb val;
   int r, g, b;
public:
   incrementQRgb(): val(), r( 0 ), g( 0 ), b( 0 ) {}
   incrementQRgb& operator++() {
      r++;
      g++;
      b++;
      val = qRgb( r, g, b );
      return *this;
   }
   QRgb operator()() {
      return val;
   }
   operator QRgb() {
      return val;
   }
   operator string() {
      return "incrementQRgb";
   }

};



ImageCompany::ImageCompany( QObject* parent_ ) :
   QObject( parent_ ), mImages(), colors( 256 ), index( 0 ) {
   boost::iota( colors, incrementQRgb() );
}

void ImageCompany::showCurrentImage() {
   mMsg = "";
   tImgPlanar const& pl = readImg();
   size_t pixels = pl.pixels();
   size_t bytes = pl.bytes();
   qrgb_buffer.resize( bytes );

   if( pl.is_rgb() ) {
      rgba_to_qimage();
   } else {
      mono_to_qimage();
   }

   tSize s = pl.size();
   size_t sx = pl.aligned_sx();
   size_t sy = pl.sy();
   size_t xaligned = rlf::align32( sx );
   QImage img( ( uchar const* )&qrgb_buffer[0], xaligned, sy,  QImage::Format_ARGB32_Premultiplied );
   _image_shown = img;
   emit display( _image_shown, mMsg );
   QString qs = QString::fromStdString( mMsg );
   emit changeLabelText( qs );
}

void ImageCompany::rgba_to_qimage() {
   //currentToQImage();
   tImgPlanar const& pl = readImg();

   size_t sx = pl.aligned_sx();
   size_t sy = pl.sy();


   void* ov = &qrgb_buffer[0];
   QRgb* p = static_cast<QRgb*>( ov );

   tPlane8 const& plane_r = pl[tMask::pos_r];
   tPlane8 const& plane_g = pl[tMask::pos_g];
   tPlane8 const& plane_b = pl[tMask::pos_b];
   tPlane8::const_iterator r_begin = plane_r.begin();
   tPlane8::const_iterator r_end = plane_r.end();
   tPlane8::const_iterator g_begin = plane_g.begin();
   tPlane8::const_iterator b_begin = plane_b.begin();

   while( r_begin != r_end ) {
      vLine8::const_iterator vr_begin = r_begin->begin();
      vLine8::const_iterator vr_end = r_begin->end();
      vLine8::const_iterator vg_begin = g_begin->begin();
      vLine8::const_iterator vb_begin = b_begin->begin();

      QRgb* p1 = p;
      size_t count = 0;
      QRgb v;

      while( vr_begin < vr_end ) {
         // 0xAARRGGBB
         v = qRgb( *vr_begin, *vg_begin, *vb_begin );
         ++vr_begin;
         ++vg_begin;
         ++vb_begin;
         *p1 =  v;
         p1++;
         count++;
      }

      // padding
      while( count < sx ) {
         QRgb v1 = qRgb( v, v, v );
         *p1 =  v1;
         p1++;
         count++;
         mMsg = "padding";
      }

      ++r_begin;
      ++g_begin;
      ++b_begin;
      p += sx;
   }

}


void ImageCompany::mono_to_qimage() {
   //currentToQImage();
   tImgPlanar const& pl = readImg();
   tSize s = pl.size();
   size_t s1 = 0;

   size_t sx = pl.aligned_sx();
   size_t sy = pl.sy();

   void* ov = &qrgb_buffer[0];
   QRgb* p = static_cast<QRgb*>( ov );
   // 0xAARRGGBB
   tPlane8 const& plane = pl[tMask::pos_mono];
   tPlane8::const_iterator p_begin = plane.begin();
   tPlane8::const_iterator p_end = plane.end();

   while( p_begin != p_end ) {
      vLine8 const& l =  *p_begin++;
      vLine8::const_iterator begin = l.begin();
      vLine8::const_iterator end = l.end();
      QRgb* p1 = p;
      size_t count = 0;
      QRgb v;

      while( begin != end ) {
         v = qRgb( *begin, *begin, *begin );
         begin++;
         *p1 =  v;
         p1++;
         count++;
      }

      // padding
      while( count < sx ) {
         QRgb v1 = qRgb( v, v, v );
         *p1 =  v1;
         p1++;
         count++;
         mMsg = "padding";
      }

      p += sx;
   }
}


