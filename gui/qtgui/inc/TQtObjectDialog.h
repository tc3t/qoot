// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtObjectDialog.h 3145 2009-04-23 15:56:41Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// TQtObjectDialog                                                            //
//                                                                            //
// This class provides an interface to  context sensitive popup menus.        //
// These menus pop up when the user hits  the right mouse button,  and        //
// are destroyed when the menu pops downs.                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TQtObjectDialog
#define ROOT_TQtObjectDialog

#include <qdialog.h>

class TObject;
class TMethod;
class TObjArray;

class TQtObjectDialog : public QDialog {
  Q_OBJECT
protected:
  TObjArray *fParArray;
public :
 TQtObjectDialog(TObject *object=0, TMethod *method=0);
 TObjArray *GetParamList(){ return fParArray;}
public slots:
  void accept();
  void emitHelpRequest();
signals:
  void helpRequested();
};

#endif
