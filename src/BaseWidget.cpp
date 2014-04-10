
#include "BaseWidget.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_BasisWidget.h"
#pragma GCC diagnostic pop



BasisWidget::BasisWidget( QWidget* parent_ ) :
   QWidget( parent_ ),
   ui( new Ui::BasisWidget ) {
   ui->setupUi( this );
}

BasisWidget::~BasisWidget() {
   delete ui;
}


void BasisWidget::activateButton( bool checked ) {
   this->ui->pushButton->setEnabled( checked );
}
