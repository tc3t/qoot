//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// @(#)root/gui:$Name$:$Id: TQtPatternSelect.h 3594 2013-02-19 03:50:53Z fineroot $
// Author: Valeri Fine  21/05/2004

#ifndef ROOT_TQtPatternSelect
#define ROOT_TQtPatternSelect

/****************************************************************************
** $Id: TQtPatternSelect.h 3594 2013-02-19 03:50:53Z fineroot $
**
** Copyright (C) 2004 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// The TQtPatternSelect widget is like a checkbutton but instead of the //
// check mark there is color area with a little down arrow. When        //
// clicked on the arrow the TQtPatternPopup pops up.                    //
//                                                                      //
// Selecting a color in this widget will generate the event:            //
// kC_COLORSEL, kCOL_SELCHANGED, widget id, pixel.                      //
// and the signal:                                                      //
// ColorSelected(Pixel_t pixel)                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "GuiTypes.h"
#include "Gtypes.h"
#include "Riostream.h"
#include "TObject.h"
#ifndef ROOT_TQObject
#include "TQObject.h"
#endif

#ifndef __CINT__
#  include <QObject>
#  include <QEvent>
#endif

class QWidget;
class TQtPatternSelectButton;
class QEvent;

//----------------------------------------------------------------------
class TEmitRootSignal {
public:
   TEmitRootSignal(){}
   virtual ~TEmitRootSignal(){}
   virtual  void EmitRootSignal(ULong_t ) { }
};

//----------------------------------------------------------------------

class TQtPatternSelect : 
#ifndef __CINT__
   public QObject, 
#endif
   public TObject, public TQObject, public TEmitRootSignal {
#ifndef __CINT__
Q_OBJECT
#endif
private:
   TQtPatternSelectButton *fPatternSelector;
   TQtPatternSelect(const TQtPatternSelect&);
   TQtPatternSelect &operator=(const TQtPatternSelect&);
protected:
   friend class ev;
   virtual void ConstructorThread(QWidget *p=0, Style_t pixel=0, Int_t id=-1);

public:
   TQtPatternSelect(QWidget *p=0, Style_t pattern=0, Int_t id=-1);
   virtual ~TQtPatternSelect();
   virtual void EmitRootSignal(ULong_t style); // *SIGNAL*
   TQtPatternSelectButton *GetPatternSelectButton() const { return fPatternSelector;}
   virtual void Constructor(QWidget *p=0, Style_t pattern=0, Int_t id=-1);
   void         SetPattern(Style_t pattern);
   Style_t      GetPattern() const;
   
   virtual void SavePrimitive(std::ofstream &out, Option_t *);
   virtual void SavePrimitive(std::ostream &out, Option_t *);

protected:
   bool event(QEvent *e);
signals:
   void PatternSelected(Style_t);
      // 
#ifndef Q_MOC_RUN
   ClassDef(TQtPatternSelect,0)  // Color selection checkbutton
#endif
};

#endif
