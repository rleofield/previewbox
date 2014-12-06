#include "tLog_Category_default.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QStackedLayout>

#include "rimg_planar.h"
#include "rimg_read_write.h"

#include "ImageSeriesWidget.h"
#include "ImageSingle.h"
#include "GetAreaWidget.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace rlf::rimg_planar;

MainWindow::MainWindow( QWidget* parent_ ) :
   QMainWindow( parent_ ),
   ui( new Ui::MainWindow ),
   mStackedLayout( new QStackedLayout( this->centralWidget() ) ) {


   ui->setupUi( this );
   setupRecentImagesMenu();
   setupImageCompany();
   setupGetArea();
   setupStackedGUI();

}



//! \brief Initializes the recent images menu
void MainWindow::setupImageCompany() {
   connect( &mCompany,
            SIGNAL( display( QImage const&, string const& ) ),
            ui->frame,
            SLOT( setImage( QImage const&, string const& ) ) ,
            Qt::UniqueConnection );

   connect(&mCompany,
           SIGNAL(changeLabelText(QString)),
           ui->statusBar,
           SLOT(showMessage(QString) ),
           Qt::UniqueConnection );

}

//! \brief Initializes the recent images menu
void MainWindow::setupGetArea() {

//   connect(mGetAreaWidget,
//           SIGNAL(changeLabelText(QString)),
//           ui->statusBar,
//           SLOT(showMessage(QString) )
//            );

}

void MainWindow::setupStackedGUI() {

   mGetAreaWidget =  new GetAreaWidget( this ) ;
   mImageSeriesWidget =  new ImageSeriesWidget( this ) ;
   mImageSingle = new ImageSingle( this );

   // mStackedLayout->addWidget( new QWidget( this ) );
   mStackedLayout->addWidget( mGetAreaWidget );
   mStackedLayout->addWidget( mImageSeriesWidget );
   mStackedLayout->addWidget( mImageSingle );



   connect( mImageSeriesWidget, &ImageSeriesWidget::signalNewfile, mGetAreaWidget,  &GetAreaWidget::slotNewFile );
   connect( mImageSeriesWidget, SIGNAL( signalButtonClicked( bool ) ), mGetAreaWidget, SLOT( activateButton( bool ) ) );

   QBoxLayout* boxLayout = new QBoxLayout( QBoxLayout::TopToBottom, ui->container );

   boxLayout->setContentsMargins( 0, 0, 4, 0 );
   boxLayout->addLayout( mStackedLayout );

   ui->container->setContentsMargins( 0, 0, 0, 0 );
   ui->container->setLayout( boxLayout );

   ui->mainToolBar->addAction( ui->actionGet_Area );
   ui->mainToolBar->addAction( ui->actionImage_Series );
   ui->mainToolBar->addAction( ui->actionImage_Single );
   ui->mainToolBar->addAction( ui->actionFilter );


}

//! \brief Initializes the recent images menu
void MainWindow::setupRecentImagesMenu() {
   mImagesDealer.setType( FileDealerImageSingle::ImagesType );
   QMenu* recent = new QMenu( tr( "Recent" ), this->ui->menuFile );
   mImagesDealer.setupRecentMenu( recent );

   connect( &mImagesDealer, SIGNAL( signalOpen( QString ) ), this, SLOT( openImage( QString ) ) , Qt::UniqueConnection );
   this->ui->menuFile->insertMenu( ui->actionSave, recent );
   this->ui->menuFile->insertSeparator( ui->actionSave );
}

//! \brief Opening an image
//! \param filename
//! \see void MainWindow::on_actionOpen_triggered()
void MainWindow::openImage( const QString& filename ) {

   if( !filename.isEmpty() ) {

      ui->statusBar->showMessage( "Loading image" );
      qApp->processEvents();

      string fn = filename.toStdString();

      tImgPlanar & pl = mCompany.readImg();

      try {
         rlf::rimg_rw::read( fn, pl );
      } catch( rlf::tImgReadEx& ex ) {
         string msg = "Cannot load " + fn;
         LOGT_ERROR( msg );
         QMessageBox::information( this, tr( "Open Image" ), QString( tr( "Cannot load %1." ) ).arg( ex.what() ) );
         ui->statusBar->clearMessage();
         return;
      }

      string msg = "loaded: " + fn;
      LOGT_ERROR( msg );


      mCompany.showCurrentImage();

      /* update recent file menu */
      mImagesDealer.setCurrentFile();

      /* construct window title, e.g.  */
      QFileInfo info( filename );
      QString windowTitle_ = "rimg - ";
      windowTitle_ += info.fileName() + " ";
      windowTitle_ += QString::number( pl.size().w() )  + "x";
      windowTitle_ += QString::number( pl.size().h() );
      this->setWindowTitle( windowTitle_ );

      //ui->statusBar->showMessage( "Rendering Preview" );
      qApp->processEvents();
      update();

      //ui->statusBar->clearMessage();
   }
}


MainWindow::~MainWindow() {
   delete ui;
}

void MainWindow::on_actionQuit_triggered() {
   qApp->quit();
}

void MainWindow::on_actionOpen_triggered() {
   QString filename = mImagesDealer.getOpenFilename();
   openImage( filename );

   /* regain focus */
   this->activateWindow();
}

void MainWindow::on_actionGet_Area_triggered() {
   mStackedLayout->setCurrentWidget( mGetAreaWidget );
}

void MainWindow::on_actionImage_Series_triggered() {
   mStackedLayout->setCurrentWidget( mImageSeriesWidget );
}

void MainWindow::on_actionFilter_triggered() {

}

void MainWindow::on_actionImage_Single_triggered()
{
    mStackedLayout->setCurrentWidget( mImageSingle );
}
