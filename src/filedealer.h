#ifndef FILEDEALER_H
#define FILEDEALER_H

#include <QObject>
#include <QAction>

//! \brief IO Helper class, manages the filename and recent file actions
//! \see http://qt-project.org/doc/qt-4.8/mainwindows-recentfiles.html
class FileDealerImageSingle : public QObject {
   Q_OBJECT
public:
   enum FileType { ImagesType, PresetsType };

   explicit FileDealerImageSingle( QObject* parent = 0 );

   QString getFilename() const;
   void setFilename( const QString& filename );
   QString getSaveFilename() const;
   QString getOpenFilename();
   void setupRecentMenu( QMenu* recent );
   void setCurrentFile();
   void updateRecentFileActions();
   void setType( FileType type );

signals:
   void signalOpen( QString );

public slots:
   void openRecent();

private:
   QString mFilename;

   QString mLastDirectory;
   QString mSaveTitle;
   QString mOpenTitle;
   QString mIOSelection;
   QString mSettingsRecent;

   enum { MaxRecentFiles = 5 };

   FileType mType;
   QList<QAction*> mRecentFileActions;
};

#endif // FILEDEALER_H
