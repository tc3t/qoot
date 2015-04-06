// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtInspectImp.h 3354 2009-10-27 22:42:36Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtInspectImp
#define ROOT_TQtInspectImp

///////////////////////////////////////////////////////////////
//                                                           //
//   TQtInspectImp is a special Qt type of browser           //
//   to back TObject::Inspect  member function               //
//                                                           //
///////////////////////////////////////////////////////////////

#include "TInspectorImp.h"

#include <QTableWidget>
class QTableWidget;
class QTableWidgetItem;

class TQtInspectImp;

class TQtInspectWidget : public QTableWidget {

Q_OBJECT

private:
  const TObject *fObject;             // Pointer to displayed object
  TQtInspectImp *fInspector;
   
protected:
  friend class TQtInspectImp;
  virtual void AddValues();
  virtual void CreateInspector(const TObject *obj);
  virtual void MakeHeaders();
  virtual void MakeTitle();
  TQtInspectImp *Disconnect();

public:
  TQtInspectWidget(QWidget *parent=0,const TObject *obj=0);
  TQtInspectWidget(TQtInspectImp *parent,const TObject *obj=0);
  virtual ~TQtInspectWidget();
  virtual void Hide();
  virtual void Show();

public slots:
  virtual void Selected(QTableWidgetItem *item );
};
  

class TQtInspectImp : public TInspectorImp {
   private:         
     TQtInspectWidget *fWidget;
   protected:
     friend class TQtInspectWidget;
     TQtInspectWidget *Disconnect();
   
   public:
     TQtInspectImp(const TObject *obj=0, UInt_t width=400, UInt_t height=300);
     virtual ~TQtInspectImp();
     virtual void Hide();
     virtual void Show();
};

#endif
