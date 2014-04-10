#ifndef BASISWIDGET_H
#define BASISWIDGET_H


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

#include <QWidget>




namespace Ui {
   class BasisWidget;
}

class BasisWidget : public QWidget {
   Q_OBJECT

public:
   explicit BasisWidget( QWidget* parent = 0 );
   ~BasisWidget();

public slots:
   void activateButton( bool );

private slots:


private:
   Ui::BasisWidget* ui;
};
#pragma GCC diagnostic pop

#endif // BASISWIDGET_H
