//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   16/06/2006
/****************************************************************************
** $Id: TQtToolBar.h 3594 2013-02-19 03:50:53Z fineroot $
**
** Copyright (C) 2006 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtToolBar
#define ROOT_TQtToolBar

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtToolBar                                                           //
//                                                                      //
// This class creates a main window with menubar, scrollbars and a      //
// drawing area.                                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <QtGlobal>

//#include <q3intdict.h>
#include <QToolBar>
#include <QHash>
#include "TQtRootAction.h"


#define TOOLBARCLASSNAME QToolBar

class QMainWindow;
class TQtRootAction;
struct TQtBrowserMenuItem_t;
class TQtRootAction;

class TQtToolBar : public TOOLBARCLASSNAME {

Q_OBJECT

private:
   //Q3IntDict<TQtRootAction> fActions;
    QHash<int, TQtRootAction*> fActions;

public:

   TQtToolBar(const QString &label, QMainWindow *mainWindow, QWidget *parent, bool newLine=false, const char *name=0,Qt::WindowFlags f=0);
   TQtToolBar(QMainWindow *parent);
   virtual ~TQtToolBar();
   void AddAction(const TQtBrowserMenuItem_t &action);
   void AddAction(TQtRootAction *action);

protected:
   void  Build();

protected slots:
   void ProcessToolMessage();

   // ClassDef(TQtToolBar,0)  // Class to provide the ROOT tool bar
};

#endif
