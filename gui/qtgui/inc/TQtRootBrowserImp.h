//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtRootBrowserImp.h 3066 2009-03-22 22:34:06Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtRootBrowserImp
#define ROOT_TQtRootBrowserImp

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtRootBrowserImp                                                        //
//                                                                      //
// ABC describing GUI independent browser implementation protocol.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TBrowserImp.h"
#include "TQtGui.h"
#include "TQtRootAction.h"

#include <qstring.h>
#include <qobject.h>

#include "qoot/QtRoot/Qt3ToQt5PortingHelpers.h"
// #  include <QActionGroup>

// class TQtBrowserWidget;
class TQtBrowserImp;
class TQtIconBrowserImp;
class TQtRootAction;

class QMainWindow;
class QMenuBar;
class QToolBar;
class QActionGroup;

class TQtRootBrowserImp :  public QObject, public TBrowserImp {
  Q_OBJECT
    friend class ev;
protected:

  QMainWindow       *fBrowserImpID;
  QMenuBar          *fMenuBar;
  TQtBrowserImp     *fTreeView;
  TQtIconBrowserImp *fIconView;

  Int_t   fX,fY;
  UInt_t  fWidth,fHeight;
  QString fTitle;  

  Q3IntDict<TQtRootAction> fActions;

  QActionGroup      *fViewActions;
  QToolBar          *fToolBar;
  QString           fSaveType;
  QString           fSaveFileName;

public:

   TQtRootBrowserImp(TBrowser *b=0,bool initFlag=true);
   TQtRootBrowserImp(TBrowser *b, const char *title, UInt_t width, UInt_t height,bool initFlag=true);
   TQtRootBrowserImp(TBrowser *b, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height,bool initFlag=true);
   virtual ~TQtRootBrowserImp();

  // The base class overloaded methods:

   virtual void  Add(TObject *, const char *caption, Int_t);
   virtual void  Add(TObject *, const char *caption);
   virtual void  AddCheckBox(TObject *, Bool_t = kFALSE);
   virtual void  CheckObjectItem(TObject *, Bool_t = kFALSE);
   virtual void  RemoveCheckBox(TObject *);
   virtual void  BrowseObj(TObject *);
   virtual void  ExecuteDefaultAction(TObject *);
   virtual void  Iconify();
   virtual void  RecursiveRemove(TObject *);
   virtual void  Refresh(Bool_t = kFALSE);
   virtual void  SetDrawOption(Option_t *option="");
   virtual Option_t *GetDrawOption() const;
   virtual void  Show();

   // TObject overloaded methods:
   const QString& GetTitle() const;
   QWidget *GetBrowserID();
   // This class own data-members:
protected:
   // This class own methods:
   void Add(TObject *obj, const char *caption, Bool_t firstFlag);
   void MakeActions();
   void MakeMenu();
   void MakeToolBar() ;
   void MakeStatBar() ;
   void SetViewMode(int mode) ;

public:
   virtual Int_t InitWindow();
protected slots:
  virtual void Disconnect();
  virtual void DisplayTotal(Int_t total, Int_t selected);

  void ProcessMessage();
  void NewBrowserCB();
  void NewCanvasCB();
  void OpenCB();
  void SaveCB();
  void SaveAsCB();
  void CopyCB();
  void PrintCB();
  void CloseCB();
  void QuitCB();
 
//*-*   Items for the View

  void ToolbarCB(bool show);
  void StatusBarCB(bool show);
  void LargeCB()  {SetViewMode(TQtGui::kViewLargeIcons);}
  void SmallCB()  {SetViewMode(TQtGui::kViewSmallIcons);}
  void ListCB()   {SetViewMode(TQtGui::kViewList);}
  void DetailsCB(){SetViewMode(TQtGui::kViewDetails);}
  void ArrangeCB(){;}
  void LineupCB(){;}
  void RefreshCB();
  void HelpCB();
  void AboutCB();
  void ResetAction(int mode) ;

};
inline  const QString& TQtRootBrowserImp::GetTitle() const {return fTitle;}

#endif
