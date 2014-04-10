#include <QMenu>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include "filedealer.h"

FileDealerImageSingle::FileDealerImageSingle( QObject* parent_ ) :
   QObject( parent_ ) {

   mRecentFileActions.reserve( MaxRecentFiles );

   for( int i = 0; i < MaxRecentFiles; i++ ) {
      mRecentFileActions.push_back( 0 );
   }

   setType( ImagesType );
}

void FileDealerImageSingle::setType( FileType type ) {
   mType = type;

   switch( mType ) {
   case ImagesType: {
      mLastDirectory = "lastDirectoryImages";
      mSaveTitle = tr( "Save image" );
      mOpenTitle = tr( "Open image" );
      mIOSelection = tr( "Images (*.bmp *.BMP *.jpg *.jpeg *.JPG *.JPEG *.png *.PNG *.tif *.TIF *.tiff *.TIFF)" );
      mSettingsRecent = "recentImages";

   }
   break;

   case PresetsType: {
      mLastDirectory = "lastDirectoryPresets";
      mSaveTitle = tr( "Save presets" );
      mOpenTitle = tr( "Open presets" );
      mIOSelection = tr( "Presets (*.bs *.BS)" );
      mSettingsRecent = "recentPresets";
   }
   break;

   default:
      qWarning() << "No type set";
   }
}

QString FileDealerImageSingle::getFilename() const {
   return mFilename;
}

void FileDealerImageSingle::setFilename( const QString& filename ) {
   mFilename = filename;
}

QString FileDealerImageSingle::getSaveFilename() const {
   QSettings settings( "elsamuko.com", "previewbox" );
   QString dir = settings.value( mLastDirectory ).toString();

   if( dir.isEmpty() ) {
      dir = QDir::homePath();
   }

   return QFileDialog::getSaveFileName( 0, mSaveTitle, dir, mIOSelection );
}

QString FileDealerImageSingle::getOpenFilename() {
   QSettings settings( "elsamuko.com", "previewbox" );
   QString dir = settings.value( mLastDirectory ).toString();

   if( dir.isEmpty() ) {
      dir = QDir::homePath();
   }

   mFilename = QFileDialog::getOpenFileName( 0, mOpenTitle, dir, mIOSelection );

   return mFilename;
}


//! \brief Setup the menu under File -> Recent
//! \see void MainWindow::setupRecentMenu( )
void FileDealerImageSingle::setupRecentMenu( QMenu* recent ) {

   for( int i = 0; i < MaxRecentFiles; ++i ) {
      mRecentFileActions[i] = new QAction( this );
      mRecentFileActions[i]->setVisible( false );
      connect( mRecentFileActions[i], SIGNAL( triggered() ), this, SLOT( openRecent() ) );
   }

   this->updateRecentFileActions();

   recent->addActions( mRecentFileActions );
}

//! \brief Update GUI state according to mFilename
//! \see void MainWindow::openImage( const QString& filename )
void FileDealerImageSingle::setCurrentFile( ) {
   QSettings settings( "rleofield.de", "previewbox" );

   /* set last opened directory */
   QFileInfo info( mFilename );
   QStringList files = settings.value( mSettingsRecent ).toStringList();

   if( info.exists() ) {
      settings.setValue( mLastDirectory, info.absolutePath() );
      files.removeAll( mFilename );
      files.prepend( mFilename );
   }

   while( files.size() > MaxRecentFiles ) {
      files.removeLast();
   }

   settings.setValue( mSettingsRecent, files );

   this->updateRecentFileActions();
}

//! \brief Refreshs the Recent File Actions in menu and settings
void FileDealerImageSingle::updateRecentFileActions() {
   QList<Qt::Key> numKeys;
   numKeys << Qt::Key_1 << Qt::Key_2 << Qt::Key_3 << Qt::Key_4 << Qt::Key_5;

   QSettings settings( "elsamuko.com", "previewbox" );
   QStringList files = settings.value( mSettingsRecent ).toStringList();

   int numRecentFiles = qMin( files.size(), ( int )MaxRecentFiles );

   int actionNo = 0;

   foreach( QAction * action, mRecentFileActions ) {
      action->setVisible( false );
   }

   for( int i = 0; i < numRecentFiles; ++i ) {
      QFileInfo info( files[i] );

      if( info.exists() ) {
         mRecentFileActions[i]->setText( info.fileName() );
         mRecentFileActions[i]->setData( files[i] );
         mRecentFileActions[i]->setVisible( true );

         if( mType == ImagesType ) {
            mRecentFileActions[i]->setShortcut( Qt::ALT + numKeys[actionNo] );
         }

         actionNo++;
         /* support since Qt 5.1
            https://bugreports.qt-project.org/browse/QTBUG-13663 */
         mRecentFileActions[i]->setToolTip( info.absoluteFilePath() );
      }
   }
}

//! \brief Slot called by mRecentFileActions
void FileDealerImageSingle::openRecent() {
   QAction* action = qobject_cast<QAction*>( sender() );

   if( action ) {
      QString filename;
      filename = action->data().toString();

      /* received by  */
      emit signalOpen( filename );
   }
}
