#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filedealer.h"
#include "imagecompany.h"

namespace Ui {
   class MainWindow;
}

class QStackedLayout;
class GetAreaWidget;
class ImageSeriesWidget;
class ImageSingle;

class MainWindow : public QMainWindow {
   Q_OBJECT

public:
   explicit MainWindow( QWidget* parent = 0 );
   ~MainWindow();

private slots:
   void openImage( const QString& filename );
   void on_actionQuit_triggered();
   void on_actionOpen_triggered();
   void on_actionGet_Area_triggered();
   void on_actionImage_Series_triggered();
   void on_actionFilter_triggered();
   void on_actionImage_Single_triggered();

   private:
   Ui::MainWindow* ui;
   FileDealerImageSingle mImagesDealer;
   ImageCompany mCompany;
   QStackedLayout* mStackedLayout;
   GetAreaWidget* mGetAreaWidget;
   ImageSeriesWidget* mImageSeriesWidget;
   ImageSingle* mImageSingle;

   void setupRecentImagesMenu();
   void setupImageCompany();
   void setupGetArea();
   void setupStackedGUI();
};

#endif // MAINWINDOW_H
