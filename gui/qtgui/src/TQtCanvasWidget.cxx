//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtCanvasWidget.cxx 3624 2013-03-10 16:17:14Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/

// Definition of TQtWinget class
// "double-buffere widget

#include "TQtCanvasWidget.h"
#include "TQtWidget.h"
#include <QEvent>
#include <QApplication>

#ifdef WIN32
#include "Windows4Root.h"
#endif

//_____________________________________________________________________________
TQtCanvasWidget::TQtCanvasWidget(QWidget* parent):QMainWindow(parent)
{ 
//   setAttribute(Qt::WA_DeleteOnClose);
}
//_____________________________________________________________________________
void TQtCanvasWidget::ChangeDocking(bool)
{ 
   // The change of the docking may have lead to the TCanvas size change also
   // We should attempt to resize the TCanvas
   // adjustSize();
   ExitSizeEvent(TQtWidget::kFORCESIZE);
   update();
}
//_____________________________________________________________________________
bool TQtCanvasWidget::ExitSizeEvent (int update)
{
  // Disable update during resizing (for the sake of the optimization) 
  QWidget *w = centralWidget();
  if (!w) return false;

  // QCustomEvent is Qt3 component, disabled as this doesn't seem strictly necessary.
  //QApplication::sendEvent(w,new QCustomEvent(int(QEvent::User+update)) );
  
  return true;
}
//_____________________________________________________________________________
void TQtCanvasWidget::closeEvent(QCloseEvent *event)
{
   if ( event->spontaneous()) {
      event->ignore();
      emit WMCloseCanvas();
   } else {
      QWidget::closeEvent(event);
   }
}

#ifdef R__WIN32
//_____________________________________________________________________________
bool TQtCanvasWidget::winEvent ( MSG *msg )
{
   // QT 3.x compliant version of the method
   long result;
   return winEvent ( msg , &result);
}
//_____________________________________________________________________________
bool    TQtCanvasWidget::winEvent ( MSG *msg , long * result )
{
   // In your reimplementation of this function, if you want to stop the event 
   // being handled by Qt, return true. If you return FALSE, this native event
   // is passed back to Qt, which translates the event into a Qt event and sends
   // it to the widget. 
  bool res = false;
  switch ( msg->message ) {

  case WM_ENTERSIZEMOVE: { res = ExitSizeEvent(TQtWidget::kENTERSIZEMOVE); break;}
  case WM_EXITSIZEMOVE:  { res = ExitSizeEvent(TQtWidget::kEXITSIZEMOVE);  break;}

  default: break;
  };
  *result = 0;
  return res;
}
#endif
