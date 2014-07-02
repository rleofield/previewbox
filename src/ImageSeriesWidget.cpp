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
File: ImageSeriesWidget.cpp
*/

#include "tLog_Category_rimg.h"
#include "ImageSeriesWidget.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_ImageSeriesWidget.h"
#pragma GCC diagnostic pop


ImageSeriesWidget::ImageSeriesWidget( QWidget* parent_ ) :
   QWidget( parent_ ),
   ui( new Ui::ImageSeriesWidget ) {
   ui->setupUi( this );
}

ImageSeriesWidget::~ImageSeriesWidget() {
   delete ui;
}

void ImageSeriesWidget::on_button_clicked( bool checked ) {
   emit signalButtonClicked( checked );
}

void ImageSeriesWidget::on_button_clicked()
{
     LOGT_RIMG_INFO("on_button_clicked");
}

void ImageSeriesWidget::on_pushButton_clicked()
{
   LOGT_RIMG_INFO("on_pushButton_clicked");
}

void ImageSeriesWidget::on_actionButton_file_pressed_triggered()
{

}
