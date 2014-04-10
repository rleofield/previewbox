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
File: GetAreaWidget.cpp
*/
#include "stringhelper.h"
#include "GetAreaWidget.h"


using namespace rlf_hstring;
using std::string;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_GetAreaWidget.h"
#pragma GCC diagnostic pop


GetAreaWidget::GetAreaWidget( QWidget* parent_ ) :
   QWidget( parent_ ),
   ui( new Ui::GetAreaWidget ), areaHorizontalScrollBar(0) {
   ui->setupUi( this );
}

GetAreaWidget::~GetAreaWidget() {
   delete ui;
}

void GetAreaWidget::on_pushButton_clicked() {


}

void GetAreaWidget::activateButton( bool checked ) {
   this->ui->pushButton->setEnabled( checked );
}

void GetAreaWidget::on_areaHorizontalScrollBar_valueChanged(int value)
{
    areaHorizontalScrollBar = value;
    string msg = toString(value);

    QString qs = QString::fromStdString(msg);
    ui->areaLabelImg->setText(qs);

}

