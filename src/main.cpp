#include "tLog_Category_default.h"
#include "mainwindow.h"
#include <QApplication>

int main( int argc, char* argv[] ) {
   string lf = "rimg"; // extension is added (".log")
   bool b = logger().setLogfile( lf );

   if( b == false ) {
      cout << "setLogfile: " << lf << " path for logging dosn't exist: '" << lf <<  "'" << endl;
   }

   logger().log_level( eLevel::LDEBUG, eCategory::_default );

   LOGT_INFO( "started" );
   QApplication a( argc, argv );
   MainWindow w;
   w.show();

   return a.exec();
}
