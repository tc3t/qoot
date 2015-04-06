//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtControlBarImp.cxx 3601 2013-02-24 04:20:15Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/

#include "TQtControlBarImp.h"
#include "TControlBar.h"
#include "TClass.h"
#include "TControlBarButton.h"

#include <QApplication>

#include <QPushButton>
#include <QToolTip>
#include <QLayout>

#include <QVBoxLayout>
#include <QHBoxLayout>
//______________________________________________________________________________
TQtControlBarItem::TQtControlBarItem( TControlBarButton *b,QPushButton *i)
 :fButton(b),fButtonImp(i)
{
  // Find TMethod("Action)
  TClass *buttonClass = b->Class();
  fActionMethod = buttonClass->GetMethodAny("Action");
}

//______________________________________________________________________________
void TQtControlBarItem::Exec(bool on)
{
  if (on) { 
    fExecute.Execute(fButton,fActionMethod);
    fButtonImp->toggle(); 
  } 
}
//______________________________________________________________________________
TQtControlBarImp::TQtControlBarImp(TControlBar *c, const char *title):
 QObject(0),TControlBarImp(c,title),fWidget(0),fClicked(0)
 {
    setObjectName(title);
    fItems.setAutoDelete(true);
    fXpos = -999;       //Initial x position
    fYpos = -999;       //Initial y position
 }
//______________________________________________________________________________
TQtControlBarImp::TQtControlBarImp(TControlBar *c, const char *title,Int_t x, Int_t y):
 QObject(0),TControlBarImp(c,title,x,y),fWidget(0),fClicked(0)
{
    setObjectName(title);
    fItems.setAutoDelete(true);
    fXpos = x;       //Initial x position
    fYpos = y;       //Initial y position
}
//______________________________________________________________________________
TQtControlBarImp::~TQtControlBarImp() {
  if (fWidget) {
    fWidget->disconnect(this);
    delete fWidget; 
    fWidget = 0;
  }
}
//______________________________________________________________________________
void TQtControlBarImp::Create()
{ }
//______________________________________________________________________________
void TQtControlBarImp::Disconnect()
{   
   fWidget = 0; delete this;     
}
//______________________________________________________________________________
void TQtControlBarImp::Hide(){ if (fWidget) fWidget->hide(); }
//______________________________________________________________________________
void TQtControlBarImp::Clicked(bool)
{
   TQtControlBarItem *item = (TQtControlBarItem*)sender();
   fClicked = item->GetClicked();  
}

//______________________________________________________________________________
TControlBarButton *TQtControlBarImp::GetClicked()
{
   TControlBarButton *lastButton = fClicked;
   return lastButton;
}
//______________________________________________________________________________
void TQtControlBarImp::Show() 
{ 
  if (!fWidget) {
    TControlBar *bar = GetControlBar();
    TList *buttons = bar ? bar->GetListOfButtons() : 0;
    if (bar && buttons) {
      fWidget = new QWidget(0);
      fWidget->setAttribute(Qt::WDestructiveClose);
      fWidget->setWindowTitle("controlbar");
      fWidget->setWindowTitle(objectName());
      QObject::connect(fWidget,SIGNAL(destroyed()),this,SLOT(Disconnect()));
      if (fXpos > 0 && fYpos > 0) fWidget->move(fXpos,fYpos);
      Bool_t vertical = bar->GetOrientation() == TControlBar::kVertical;
      QBoxLayout *layout = 0;
      if (vertical) {layout = new QVBoxLayout( fWidget );}
      else          {layout = new QHBoxLayout( fWidget );}
      //layout->setAutoAdd( true );

      TIter nextButton(buttons);
      TControlBarButton *button = 0;
      while (( button = (TControlBarButton *)nextButton()) ) {
        QPushButton *b = new QPushButton(button->GetName(),fWidget);
        b->setCheckable(true);
        b->setToolTip(button->GetTitle() );
        TQtControlBarItem *barItem = new TQtControlBarItem(button,b);
        fItems.push(barItem);
        connect(b,SIGNAL(toggled(bool)),barItem,SLOT(Exec(bool)));
        connect(b,SIGNAL(toggled(bool)),barItem,SIGNAL(ClickedItem(bool)));
        connect(barItem,SIGNAL(ClickedItem(bool)),this,SLOT(Clicked(bool)));
      }
    }
  }
  fWidget->show();
}
