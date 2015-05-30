// @(#)root/qtgsi:$Id$
// Author: Denis Bertini, M. Al-Turany  01/11/2000

/*************************************************************************
 * Copyright (C) 1995-2006, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#include "TQRootApplication.h"
#include "TSystem.h"
#include <stdlib.h>

bool TQRootApplication::fgDebug=kFALSE;
bool TQRootApplication::fgWarning=kFALSE;

ClassImp(TQRootApplication)
   
//______________________________________________________________________________
void qMessageOutput(QtMsgType type, const QMessageLogContext &, const QString& sMsg)
{
   const auto s = sMsg.toLatin1();
   const auto msg = s.data();
   switch ( type ) {
      case QtDebugMsg:
         if(TQRootApplication::fgDebug)
            fprintf( stderr, "QtRoot-Debug: \n %s\n", msg );
         break;
      case QtWarningMsg:
         if(TQRootApplication::fgWarning)
            fprintf( stderr, "QtRoot-Warning: \n %s\n", msg );
         break;
      case QtFatalMsg:
         fprintf( stderr, "QtRoot-Fatal: \n %s\n", msg );
         abort();         // dump core on purpose
         break;
      case QtCriticalMsg:
         fprintf( stderr, "QtRoot-Fatal: \n %s\n", msg );
         abort();         // dump core on purpose
         break;
   }
}

//______________________________________________________________________________
TQRootApplication::TQRootApplication(int &argc, char **argv, int poll) : 
      QApplication(argc,argv), fQTimer(0), fRTimer(0)
{
   // Connect ROOT via Timer call back.

   if (poll == 0) {
      fQTimer = new QTimer( this );
      QObject::connect( fQTimer, SIGNAL(timeout()),this, SLOT(Execute()) );
      fQTimer->setSingleShot(false);
      fQTimer->start( 20 );
      fRTimer = new TTimer(20);
      fRTimer->Start(20, kFALSE);
   }

   // install a msg-handler
   fgWarning = fgDebug = kFALSE;
   qInstallMessageHandler(qMessageOutput);
}

//______________________________________________________________________________
TQRootApplication::~TQRootApplication()
{
   // dtor
}

//______________________________________________________________________________
void TQRootApplication::Execute()
{
   // Call the inner loop of ROOT.

   gSystem->InnerLoop();
}

//______________________________________________________________________________
void TQRootApplication::Quit()
{
   // Set a Qt-Specific error handler.

   gSystem->Exit( 0 );
}
