#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QColor>
#include "rimg_planar.h"


class Frame : public QFrame {
   Q_OBJECT
public:
   explicit Frame( QWidget* parent = 0 );

   void drawImage( QPainter* painter );
signals:

public slots:
   void setImage( QImage image, string const& msg );

protected:
   virtual void paintEvent( QPaintEvent* );
   virtual void mousePressEvent( QMouseEvent* );
   virtual void mouseReleaseEvent( QMouseEvent* );
   virtual void mouseMoveEvent( QMouseEvent* );

private:
   QImage mImage;
   QColor mColor;
   QPoint mPos;
   string mMsg;

};

#endif // FRAME_H
