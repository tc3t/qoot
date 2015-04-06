// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtCanvasWidget.h 3531 2012-11-16 20:10:58Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_QtCanvasWidget
#define ROOT_QtCanvasWidget

#include <QtGlobal>
#include <QMainWindow> 

class  TQtCanvasWidget : public QMainWindow {
 Q_OBJECT
public:
  TQtCanvasWidget( QWidget* parent=0);
  virtual ~TQtCanvasWidget(){;}
 
protected slots:
  virtual bool ExitSizeEvent(int updt);
  void   ChangeDocking(bool);

#ifdef WIN32
protected:
  //  Some WIN32 optimization. It will work with out these methods too.
  virtual bool winEvent(MSG *msg);
  virtual bool winEvent(MSG *msg, long * result);
#endif
protected:
  virtual void closeEvent(QCloseEvent *event);
signals:
  void WMCloseCanvas();
};

#endif
