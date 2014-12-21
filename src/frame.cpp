#include <QPainter>
#include <QMouseEvent>

#include "frame.h"




Frame::Frame( QWidget* parent_ ) :
   QFrame( parent_ ), mImage() {
   mColor = QColor( 255, 0, 0 );
   setMouseTracking( true );
}

void Frame::setImage( QImage image , const string& msg ) {
   mImage = image;
   mMsg = msg;
   update();
}

void Frame::drawImage( QPainter* painter ) {

   if( !mImage.isNull() ) {
      QSize us = rect().size();
      QImage image = mImage.scaled( us, Qt::KeepAspectRatio, Qt::SmoothTransformation );

      QSize diff = us - image.size();
      painter->drawImage( diff.width() / 2, diff.height() / 2, image );


   }

}

void Frame::paintEvent( QPaintEvent* /*event*/ ) {
   QPainter painter( this );
   drawImage( &painter );
   int w = width();
   int h = height();
   //painter.setPen( QPen( mColor, 5 ) );
   //painter.drawLine(0,0,w,h);
   //painter.drawLine(QPoint(0,0),mPos);
   // QFrame::paintEvent( event );
}

void Frame::mousePressEvent( QMouseEvent* ) {
   mColor = QColor( 255, 255, 0 );
   update();
}

void Frame::mouseReleaseEvent( QMouseEvent* ) {
   mColor = QColor( 255, 0, 0 );
   update();
}

void Frame::mouseMoveEvent( QMouseEvent* e ) {
   mPos = e->pos();
   update();
}
