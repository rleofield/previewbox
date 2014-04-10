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
File: ImageSeriesWidget.h
*/
#ifndef IMAGESERIESWIDGET_H
#define IMAGESERIESWIDGET_H


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QWidget>

namespace Ui {
   class ImageSeriesWidget;
}

class ImageSeriesWidget : public QWidget {
   Q_OBJECT

public:
   explicit ImageSeriesWidget( QWidget* parent = 0 );
   ~ImageSeriesWidget();

signals:
   void signalButtonClicked( bool );

private slots:
   void on_button_clicked( bool checked );

private:
   Ui::ImageSeriesWidget* ui;
};
#pragma GCC diagnostic pop

#endif // IMAGESERIESWIDGET_H
