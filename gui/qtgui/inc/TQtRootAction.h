//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtRootAction.h 3531 2012-11-16 20:10:58Z fineroot $
**
** Copyright (C) 2003 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtRootAction
#define ROOT_TQtRootAction

#include <QtGlobal>
#include <QAction>
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtRootAction                                                        //
//                                                                      //
// QAction Qt class to build ROOT-related menu and tool bars     .      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// Structure to initialize the QAction class
struct TQtBrowserMenuItem_t  {
   const char *fMenuText;
   int         fId;
   int         fAccelerator;
   const char *fToolTip;
   const char *iconName;
};  

class TQtRootAction : public QAction {
  public:
    TQtRootAction(QObject * parent, const TQtBrowserMenuItem_t  &data);
    int Id() const { return fId; }

    bool addTo(QWidget* pWidget)
    {
        if (pWidget)
        {
            pWidget->addAction(this);
            return true;
        }
        else
            return false;
    }

    bool isOn() const { return isChecked(); }
    void setOn(bool b) { setChecked(b); }

    void setToggleAction(bool b) { setCheckable(b); }
    bool isToggleAction() const { isCheckable(); }

    QString menuText() { return text(); }

  private:
     int  fId;
};
#endif
