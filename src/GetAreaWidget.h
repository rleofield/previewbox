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

      C++, GCC
File: GetAreaWidget.h
*/
#ifndef GETAREAWIDGET_H
#define GETAREAWIDGET_H

#include "timer.h"

using namespace rlf_htime;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QWidget>

namespace Ui {
   class GetAreaWidget;
}

class GetAreaWidget : public QWidget {
   Q_OBJECT

public:
   explicit GetAreaWidget( QWidget* parent = 0 );
   ~GetAreaWidget();

signals:
   void changeLabelText( QString text);

public slots:
   void activateButton( bool );

private slots:
   void on_pushButton_clicked();
   void on_areaHorizontalScrollBar_valueChanged(int value);


   private:
   Ui::GetAreaWidget* ui;

   int areaHorizontalScrollBar;
   tTimer t;

};
#pragma GCC diagnostic pop

#endif // GETAREAWIDGET_H
