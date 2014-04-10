#ifndef IMAGECOMPANY_H
#define IMAGECOMPANY_H

#include <QObject>
#include <QImage>

#include "t_filename.h"
#include "rimg_rgb_bgra_types.h"
#include "rimg_planar.h"
#include "rimg_read_write.h"
#include "rimg_lookuptable.h"

using rlf::tRGBA;
using rlf::rimg::tLUT;
using rlf::rimg_planar::tImgPlanar;

static size_t max_index = 20;

class tImages {

      vector<tImgPlanar> mImages;

public:
   tImages(): mImages(max_index) {}

   static const size_t max_index = 20;
   static const size_t img_raw = 0;
   static const size_t img_sobel = 1;
   static const size_t img_gradient = 2;
   static const size_t img_lut_mono = 3;
   static const size_t img_lut_rgb = 4;

   tImgPlanar & getRead(){
      return mImages[img_raw];
   }
   tImgPlanar & getSobel(){
      return mImages[img_sobel];
   }
   tImgPlanar & getGradient(){
      return mImages[img_gradient];
   }
   tImgPlanar & getLutMono(){
      return mImages[img_lut_mono];
   }
   tImgPlanar & getLutRGB(){
      return mImages[img_lut_rgb];
   }


};

class tLutKey {
public:
   std::string value;
   tLutKey( std::string v ): value( v ) {}

   static tLutKey glowscale;
   static tLutKey green;
   static tLutKey spectrum;
   static tLutKey gray;
   static tLutKey range;


   bool operator==( tLutKey const& in )const {
      if( in.value == value ) {
         return true;
      }

      return false;
   }
};
class tLutValues {
public:
   tLutKey key;
   tLUT table;

   tLutValues( tLutKey key_, tLUT const& t ): key( key_ ), table( t ) {
   }

};
class tLutList {

public:
   tLutList():  luts() {}

   std::vector<tLutValues> luts;

   void Add( tLutKey key ) {
      if( key == tLutKey::glowscale ) {
         //luts.push_back( tLutValues( key, tLUT::Glowscale() ) );
         return;
      }

      if( key == tLutKey::green )  {
         luts.push_back( tLutValues( key, tLUT::Green() ) );
         return;
      }

      if( key == tLutKey::spectrum ) {
         //luts.push_back( tLutValues( key, tLUT::Spectrum() ) );
         return;
      }

      if( key == tLutKey::gray )   {
         luts.push_back( tLutValues( key, tLUT::Gray() ) );
         return;
      }


      luts.push_back( tLutValues( tLutKey::gray , tLUT::Linearize() ) );
   }

   void clear() {
      luts.clear();
   }
   size_t size()const {
      return luts.size();
   }
   tLutValues const& operator[]( size_t i )const {
      return luts[i];
   }


};



class ImageCompany : public QObject {
   Q_OBJECT
public:
   enum Color { Grey, GreyA, RGB, RGBA };

   explicit ImageCompany( QObject* parent = 0 );
   void showCurrentImage();
   void rgba_to_qimage();
   void mono_to_qimage();

   tImgPlanar & readImg() {
      return mImages.getRead();
   }
signals:
   // emitted, if it has sth to show
   void display( QImage const& image, string const& msg );
   void changeLabelText( QString text);

public slots:

private:
   tImages mImages;

   vector<QRgb> qrgb_buffer;
   QVector<QRgb> colors ;

   QImage _image_shown;

   size_t index;
  string mMsg;
};

#endif // IMAGECOMPANY_H
