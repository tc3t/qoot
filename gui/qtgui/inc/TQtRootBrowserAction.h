// Author: Valeri Fine  08/03/2003
/****************************************************************************
** $Id: TQtRootBrowserAction.h 331 2003-03-09 05:34:32Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtRootBrowserAction
#define ROOT_TQtRootBrowserAction

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtRootBrowserAction                                                 //
//                                                                      //
// define the ROOT browser actions.                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <qobject.h>

class TQMimeTypes;
class TObject;

class TQtRootBrowserAction : public QObject {
Q_OBJECT
private:
   static TQtRootBrowserAction *gfRootBrowserAction;
protected:
   TQMimeTypes *fMimeTypeList;
   TQtRootBrowserAction(): fMimeTypeList(0) { IconList();}
   virtual void IconList();
public:
   virtual  ~TQtRootBrowserAction();
   static TQtRootBrowserAction *Instance();
   static TQMimeTypes *GetMimeTypeList(){ return Instance()->fMimeTypeList;}
public slots:
    virtual void  ExecuteDefaultAction(TObject *obj);
};

#endif

