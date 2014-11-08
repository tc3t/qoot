// @(#)root/ged:$Name$:$Id: TQtGedEditor.cxx 3189 2009-06-01 15:45:26Z fine $
// Author: Valeri Fine 10/07/2004

/****************************************************************************
** $Id: TQtGedEditor.cxx 3189 2009-06-01 15:45:26Z fine $
**
** Copyright (C) 2004 by Valeri Fine.  All rights reserved.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
*****************************************************************************/

/*************************************************************************
 * This source is based on TGedEditor, a ROOT GUI toolkit.               *
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 * Author: Marek Biskup, Ilka Antcheva   02/12/2003                      *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtGedEditor                                                         //
//                                                                      //
// Editor is a composite frame that contains TQtGedToolBox and          //
// TQtGedAttFrames. It is connected to a Canvas and listens for         //
// selected objects                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <assert.h>

#include "TQtGedEditor.h"
#include "TCanvas.h"
#include "TGQt.h"
#include "TQtGedAttFrame.h"
#include "TQtGedFrames.h"
#include "TQtPadEditor.h"
#include "TQtAxisEditor.h"
#include "TQtArrowEditor.h"
#include "TQtCanvasWidget.h"
#include "TQtWidget.h"

#ifdef R__WIN32
#  include "TPadEditorHelper.h"
#endif

#include <QTimer> 
#include <QDebug> 

std::list<TQtGedFactoryI *> *TQtGedEditor::fgGedFactoriesList = 0;
Qt::DockWidgetArea  TQtGedEditor::fgDockPosition =  Qt::LeftDockWidgetArea; // The default ediotrs position

ClassImp(TQtGedEditor)

enum {
       kOrdinaryWidget = -1
     , kNotInit        =  0
     , kMainWidget     =  1
};
//______________________________________________________________________________
TQtGedEditor::TQtGedEditor(TCanvas* canvas) : fCanvas(canvas)
      , fDock(0)
      , fCanvasImpID(0)
      , fCanvasID(0)
      , fGlobal(kTRUE), fIsMainWindow(kNotInit)
 {
   if (fCanvas)  {
      fCanvasID    = TGQt::wid(canvas->GetCanvasID());
      assert(fCanvasID);
      fCanvasImpID  = dynamic_cast<QMainWindow *>(fCanvasID->topLevelWidget());
      fIsMainWindow = kMainWidget;
      if (!fCanvasImpID) {
         fCanvasImpID = new QMainWindow();
         fIsMainWindow = kOrdinaryWidget;
      }
//      fCanvasImpID  = fCanvasID->topLevelWidget();
      if (!fCanvasImpID) {
         fprintf(stderr,"Can not create Pad editor for <%s> with no QMainWindow\n",canvas->GetTitle());
         assert(fCanvasID);
      }
#ifndef R__WIN32
      fgPadEditor = this;
#else
      TPadEditorHelper::SetPadPointer(this);
#endif
   }
}
//______________________________________________________________________________
void TQtGedEditor::Build()
{
   // Populate editor with the "registered" primitive editors
   if (fCanvasImpID && fgGedFactoriesList) {
     std::list<TQtGedFactoryI *>::const_iterator ged = fgGedFactoriesList->begin();
     // Loop over all existing object factories to create the primitive instances
     for (;ged != fgGedFactoriesList->end(); ++ged)
        Insert((*ged)->Create(fCanvasImpID,fCanvas));
   }
}
//______________________________________________________________________________
void TQtGedEditor::SetDefaultPosition(Qt::DockWidgetArea dockPosition)
{
    // Set the default editor positions
    fgDockPosition = dockPosition;
}

//______________________________________________________________________________
Qt::DockWidgetArea TQtGedEditor::DefaultPosition()
{
    // return the default editor positions
    return fgDockPosition;
}

//______________________________________________________________________________
Bool_t   TQtGedEditor::IsGlobal() const { return fGlobal;}

//______________________________________________________________________________
void     TQtGedEditor::SetGlobal(Bool_t flag) { fGlobal= flag; }

//______________________________________________________________________________
TCanvas* TQtGedEditor::GetCanvas() const
{ return fCanvas;                                              }

//______________________________________________________________________________
void TQtGedEditor::Insert(TQtGedAttInterfaceB *frame) 
{
   // Add the frame to the container of the object editors
   if (frame) {
      fCanvasImpID->removeDockWidget( frame->Dock());
      fCanvasImpID->addDockWidget( fgDockPosition,frame->Dock());
      fGedPropertyFrames.push_back(frame);
      if (!frame->IsInitialized()) frame->CompleteInit();
      frame->ConnectToCanvas(fCanvas);
      //QObject::connect(frame,                 SIGNAL(visibilityChanged(bool))
      //     , (TQtCanvasWidget *)fCanvasImpID, SLOT(ChangeDocking(bool))); 
   }
}
//______________________________________________________________________________
TQtGedAttInterfaceB *TQtGedEditor::CanEdit(const char *className) const
{
   // Check whether there is any object editor for the given class name
   // Returns the base class of the ROOT object editor
   TQtGedAttInterfaceB *editor = 0;
   if (className && className[0]) {
      std::list<TQtGedAttInterfaceB *>::const_iterator it = fGedPropertyFrames.begin(); 
      for (; it != fGedPropertyFrames.end(); ++it) {
          TQtGedAttInterfaceB *frame = *it;
          const char *modelName = frame->ModelName();
          if (modelName && !strcmp(modelName,className)) {
             editor = frame; 
             break;
          }
      }
   }
   return editor;
}

//______________________________________________________________________________
void TQtGedEditor::CloseWindow()
{
   // When closed via WM close button, just unmap (i.e. hide) editor
   // for later use.
   Hide();
}

//______________________________________________________________________________
void TQtGedEditor::Show()
{
    // Show editor.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    fCanvasImpID->setUpdatesEnabled(FALSE);
    TCanvas *newCanvas = 0;
    // Check whether one needs to reconnect the editor to another TCanvas
    if (gPad && (gPad->GetCanvas() != fCanvas)) {
      newCanvas = gPad->GetCanvas();
    }

    if (!fDock ) {
       fDock = new QDockWidget(fCanvasImpID);
       fCanvasImpID->addDockWidget(fgDockPosition,fDock);
       Build();
       QSize size = fDock->frameSize() + QSize(0,50);
       fDock->resize(size);
    }
//    if (fCanvas) fCanvas->ToggleEditor();
    std::list<TQtGedAttInterfaceB *>::iterator it = fGedPropertyFrames.begin();
    for (; it != fGedPropertyFrames.end(); ++it) {
       TQtGedAttInterfaceB *frame = *it;
       if (frame->Model()) frame->Show();
       else                frame->Hide();
       frame->Connect(newCanvas);
    }
    fDock->show();
    fCanvasImpID->setUpdatesEnabled(TRUE);
    if (fIsMainWindow == kOrdinaryWidget ) fCanvasImpID->show();
    else QTimer::singleShot(0,(TQtWidget*)fCanvasID, SLOT(Refresh()));
    QApplication::restoreOverrideCursor();
}

//______________________________________________________________________________
void TQtGedEditor::Hide()
{
   // Hide editor.
    if (!fDock) return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    fCanvasImpID->setUpdatesEnabled(FALSE);
    if (fIsMainWindow == kOrdinaryWidget ) fCanvasImpID->hide();
    else fDock->hide();
    std::list<TQtGedAttInterfaceB *>::iterator it = fGedPropertyFrames.begin();
    for (; it != fGedPropertyFrames.end(); ++it) {
       TQtGedAttInterfaceB *frame = *it;
       frame->Disconnect();
       frame->Hide();
    }
    fCanvasImpID->setUpdatesEnabled(TRUE);
    if (fIsMainWindow != kOrdinaryWidget ) QTimer::singleShot(0,(TQtWidget*)fCanvasID, SLOT(Refresh()));
    QApplication::restoreOverrideCursor();
}

//______________________________________________________________________________
TQtGedEditor::~TQtGedEditor() {  }

//______________________________________________________________________________
bool TQtGedEditor::Register(TQtGedFactoryI *gedFactory,bool /* replace */ )
{
   // Register the GEd factory interface
   //  Under construction
   bool done = false;
   if (gedFactory) {
      if (!fgGedFactoriesList) fgGedFactoriesList = new std::list<TQtGedFactoryI *>;
      fgGedFactoriesList->push_back(gedFactory);
      done = true;
   }
   return done;
}

//______________________________________________________________________________
bool TQtGedEditor::UnRegister(TQtGedFactoryI *gedFactory)
{
       // Remove the Ged factor interface
       //  Under construction
   if (gedFactory) {}
   return true;
}
