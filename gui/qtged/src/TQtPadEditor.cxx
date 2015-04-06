// @(#)root/ged:$Name:  TQtPadEditor.cxx
// Author: Valeri Fine   24/06/04

/****************************************************************************
** $Id: TQtPadEditor.cxx 2524 2007-07-06 00:13:50Z fine $
**
** Copyright (C) 2004 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

/*************************************************************************
 * This source is based on TPadEditor, a ROOT GUI toolkit.               *
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 * Author: Ilka Antcheva                                                 *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TQtPadEditor                                                        //
//                                                                      //
//  Editor of pad/canvas objects.                                       //
//       color and fill style,                                          //             
//      'Edit' check box sets pad/canvad editable,                      //
//      'Crosshair' sets a cross hair on the pad,                       //
//      'Fixed aspect ratio' can be set when resizing the pad           //
//      'TickX' and 'TickY' set ticks along the X and Y axis            //
//      'GridX' and 'GridY' set a grid along the X and Y axis           //
//       pad/canvas border size can be set if a sinken or a raised      //
//       border mode is selected; no border mode can be set to          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//Begin_Html
/*
<img src="gif/TPadEditor.gif">
*/
//End_Html

#include "TQtPadEditor.h"
#if QT_VERSION < 0x40000
#  include <qbuttongroup.h> 
#  include <qhbox.h> 
#  include <qvbox.h> 
#else /* QT_VERSION */
#  include <QVBoxLayout>
#  include <QHBoxLayout>
#  include <QGroupBox>
#  include <QButtonGroup>
#endif /* QT_VERSION */
#include <qradiobutton.h> 
#include <qcheckbox.h> 
#include <qtooltip.h>
#include <qlabel.h> 

#include "TCanvas.h"

ClassImp(TQtPadEditor)

//______________________________________________________________________________
TQtPadEditor::TQtPadEditor(QMainWindow *p, TCanvas *canvas, Int_t id, Int_t width,
                       Int_t height, UInt_t options, Pixel_t back)
   : TQtGedAttFrame<TPad>(p, "Pad/Canvas", canvas, id, width, height, options, back)
{
   
}
//______________________________________________________________________________
TQtPadEditor::TQtPadEditor(TCanvas *canvas, QWidget *p,Int_t id, Int_t width,
                       Int_t height, UInt_t options, Pixel_t back)
   : TQtGedAttFrame<TPad>("Pad/Canvas", p, canvas, id, width, height, options, back)
{
   
}
//______________________________________________________________________________
void TQtPadEditor::BuildView(QWidget  *editorPanel)
{// Constructor of TPad editor GUI.

   delete editorPanel->layout();
   QVBoxLayout *frame= new QVBoxLayout(editorPanel);

   // QVBox *optFrame = new QVBox(frame);
   fFixedAR = new QCheckBox("Fixed aspect ratio",editorPanel);
#if QT_VERSION < 0x40000
   QToolTip::add(fFixedAR,"Set fixed aspect ratio");
#else   
   fFixedAR->setToolTip(tr("Set fixed aspect ratio"));   
#endif   
   fFixedAR->setEnabled(false);
   frame->addWidget(fFixedAR);

#if QT_VERSION < 0x40000
   QButtonGroup *f2 = new   QButtonGroup (2, Qt::Horizontal, frame);
   f2->setFlat(true); f2->setInsideMargin(0); 

   fCrosshair = new QCheckBox("Crosshair",f2);
   QToolTip::add(fCrosshair,"Set crosshair");

   fEditable = new QCheckBox("Edit",f2);
   QToolTip::add(fEditable,"Set editable mode");

   fGridX = new QCheckBox("GridX", f2);
   QToolTip::add(fGridX,"Set grid along X");

   fGridY = new QCheckBox("GridY", f2);
   QToolTip::add(fGridY,"Set grid along Y");

   fTickX = new QCheckBox("TickX", f2);
   QToolTip::add(fTickX,"Set tick marks along X");

   fTickY = new QCheckBox("TickY", f2);
   QToolTip::add(fTickY,"Set tick marks along Y");
#else /* QT_VERSION */
   QWidget *f2 = new   QWidget(editorPanel);
   frame->addWidget(f2);
   QGridLayout *f2Layout = new QGridLayout(f2);
   f2Layout->setSpacing(0);
   
   fCrosshair = new QCheckBox("Crosshair");
   fCrosshair->setToolTip(tr("Set crosshair"));
   f2Layout->addWidget(fCrosshair,0,0);
   
   fEditable = new QCheckBox("Edit");
   fEditable->setToolTip(tr("Set editable mode"));
   f2Layout->addWidget(fEditable,0,1);

   fGridX = new QCheckBox("GridX");
   fGridX->setToolTip(tr("Set grid along X"));
   f2Layout->addWidget(fGridX,1,0);

   fGridY = new QCheckBox("GridY");
   fGridY->setToolTip(tr("Set grid along Y"));
   f2Layout->addWidget(fGridY,1,1);

   fTickX = new QCheckBox("TickX");
   fTickX->setToolTip(tr("Set tick marks along X"));
   f2Layout->addWidget(fTickX,2,0);

   fTickY = new QCheckBox("TickY");
   fTickY->setToolTip(tr("Set tick marks along Y"));
   f2Layout->addWidget(fTickY,2,1);
   
#endif /* QT_VERSION */
 
//------------------------
#if QT_VERSION < 0x40000
   f2 = new  QButtonGroup (1, Qt::Vertical , "Log Scale", frame);
#else /* QT_VERSION */
   f2 = new  QGroupBox ("Log Scale", editorPanel);
   frame->addWidget(f2);
   QHBoxLayout *hbox = new QHBoxLayout(f2);
#endif /* QT_VERSION */
   hbox->addWidget(
     fLogX = new QCheckBox(":X",f2));

   hbox->addWidget(
         fLogY = new QCheckBox(":Y", f2));

   hbox->addWidget(
         fLogZ = new QCheckBox(":Z", f2));
   
#if QT_VERSION < 0x40000
   QToolTip::add(fLogX, "Set logarithmic scale along X");
   QToolTip::add(fLogY, "Set logarithmic scale along Y");
   QToolTip::add(fLogZ, "Set logarithmic scale along Z");
#else
   fLogX->setToolTip(tr("Set logarithmic scale along X"));
   fLogY->setToolTip(tr("Set logarithmic scale along Y"));
   fLogZ->setToolTip(tr("Set logarithmic scale along Z"));
#endif
// ----------------------
#if QT_VERSION < 0x40000
   QButtonGroup *bgr =  new   QButtonGroup (1, Qt::Horizontal , "Border Mode", frame);
   bgr->setExclusive(true); 
   
   bgr->insert(fBmode = new QRadioButton("Sinken border", bgr));
   QToolTip::add(fBmode,"Set a sinken border of the pad/canvas");
    
   bgr->insert(fBmode0 = new QRadioButton(" No border", bgr));
   QToolTip::add(fBmode0,"Set no border of the pad/canvas");

   bgr->setButton(
      bgr->insert(fBmode1 = new QRadioButton(" Raised border", bgr))
      );
   QToolTip::add(fBmode1,"Set a raised border of the pad/canvas");
#else /* QT_VERSION */
   QGroupBox *bgr =  new   QGroupBox ("Border Mode",editorPanel );
   frame->addWidget(bgr);
   QButtonGroup *group = new QButtonGroup(bgr);   
   QVBoxLayout *vbox = new QVBoxLayout(bgr);

   group->setExclusive(true); 
   
   group->addButton(fBmode = new QRadioButton("Sinken border", bgr));
   vbox->addWidget(fBmode);
   fBmode->setToolTip(tr("Set a sinken border of the pad/canvas"));
    
   group->addButton(fBmode0 = new QRadioButton(" No border", bgr));
   vbox->addWidget(fBmode0);
   fBmode0->setToolTip(tr("Set no border of the pad/canvas"));

   group->addButton(fBmode1 = new QRadioButton(" Raised border", bgr));
   vbox->addWidget(fBmode1);
   fBmode1->setChecked(true);
   
   fBmode1->setToolTip(tr("Set a raised border of the pad/canvas"));
   
#endif /* QT_VERSION */

   
   //f7 = new QCompositeFrame(this, 80, 20, kHorizontalFrame);
   //QLabel *fSizeLbl = new QLabel(f7, "Size:");                              
   //f7->AddFrame(fSizeLbl, new QLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
   //fBsize = new QLineWidthComboBox(f7, kPAD_BSIZE);
   //fBsize->connect("Selected(Int_t)",  this, "DoBorderSize(Int_t))); 
   //fBsize->Resize(92, 20);
   //f7->AddFrame(fBsize, new QLayoutHints(kLHintsLeft, 13, 1, 0, 0));
   //fBsize->Associate(this);
   //AddFrame(f7, new QLayoutHints(kLHintsTop, 1, 1, 0, 0));
 
   // SetActive(kTRUE);
}

//______________________________________________________________________________
TQtPadEditor::~TQtPadEditor()
{  }

//______________________________________________________________________________
void TQtPadEditor::ConnectSignals2Slots()
{ 
   // Connect signals to slots.

   ConnectView(fFixedAR,  SIGNAL(toggled(bool)),this,SLOT(DoFixedAspectRatio(bool)));
   ConnectView(fCrosshair,SIGNAL(toggled(bool)),this,SLOT(DoCrosshair(bool))       );
   ConnectView(fEditable, SIGNAL(toggled(bool)),this,SLOT(DoEditable(bool))        );
   ConnectView(fGridX,    SIGNAL(toggled(bool)),this,SLOT(DoGridX(bool))           );
   ConnectView(fGridY,    SIGNAL(toggled(bool)),this,SLOT(DoGridY(bool))           );
   ConnectView(fTickX,    SIGNAL(toggled(bool)),this,SLOT(DoTickX(bool))           );
   ConnectView(fTickY,    SIGNAL(toggled(bool)),this,SLOT(DoTickY(bool))           );
   ConnectView(fLogX,     SIGNAL(toggled(bool)),this,SLOT(DoLogX(bool))            );
   ConnectView(fLogY,     SIGNAL(toggled(bool)),this,SLOT(DoLogY(bool))            );
   ConnectView(fLogZ,     SIGNAL(toggled(bool)),this,SLOT(DoLogZ(bool))            );
   ConnectView(fBmode,    SIGNAL(clicked()),    this,SLOT(DoBorderMode())          );
   ConnectView(fBmode0,   SIGNAL(clicked()),    this,SLOT(DoBorderMode())          );
   ConnectView(fBmode1,   SIGNAL(clicked()),    this,SLOT(DoBorderMode())          );
}

//______________________________________________________________________________
void TQtPadEditor::ChangeView()
{
   // Pick up the used fill attributes.

   fFixedAR  ->setChecked(fModel->HasFixedAspectRatio());
   fCrosshair->setChecked(fModel->HasCrosshair());
   fEditable-> setChecked(fModel->IsEditable());
   fGridX->    setChecked(fModel->GetGridx());
   fGridY->    setChecked(fModel->GetGridy());
   fLogX->     setChecked(fModel->GetLogx());
   fLogY->     setChecked(fModel->GetLogy());
   fLogZ->     setChecked(fModel->GetLogz());
   fTickX->    setChecked(fModel->GetTickx());
   fTickY->    setChecked(fModel->GetTicky());

   Int_t par  =  fModel->GetBorderMode();
   if      (par == -1) fBmode ->setChecked(true);
   else if (par ==  1) fBmode1->setChecked(true);
   else                fBmode0->setChecked(true);

   par = fModel->GetBorderSize();
   //if (par < 1) par = 1;
   //if (par > 16) par = 16;
   //fBsize->Select(par);
}

//______________________________________________________________________________
void TQtPadEditor::DoEditable(bool on)
{
   // Slot connected to the check box 'Editable'.
   fModel->SetEditable(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoCrosshair(bool on)
{
   // Slot connected to the check box 'Crosshair'.
   fModel->SetCrosshair(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoFixedAspectRatio(bool on)
{
   // Slot connected to the check box 'Fixed aspect ratio'.
   fModel->SetFixedAspectRatio(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoGridX(bool on)
{
   // Slot connected to the check box 'GridX'.

  fModel->SetGridx(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoGridY(bool on)
{
   // Slot connected to the check box 'GridY'.

   fModel->SetGridy(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoLogX(bool on)
{
   // Slot connected to the check box 'LogX'.

   fModel->SetLogx(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoLogY(bool on)
{
   // Slot connected to the check box 'LogY'.

   fModel->SetLogy(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoLogZ(bool on)
{
   // Slot connected to the check box 'LogZ'.

   fModel->SetLogz(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoTickX(bool on)
{
   // Slot connected to the check box 'TickX'.

   fModel->SetTickx(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoTickY(bool on)
{
   // Slot connected to the check box 'TickY'.

   fModel->SetTicky(on);
}

//______________________________________________________________________________
void TQtPadEditor::DoBorderMode()
{
   // Slot connected to the border mode settings.
   Int_t mode = 0;
   if      (fBmode-> isChecked()) mode = -1;
   else if (fBmode0->isChecked()) mode =  0;
   else                           mode =  1;

   //if (!mode) HideFrame(f7);
   //else ShowFrame(f7);
   //Layout();

   fModel->SetBorderMode(mode);
}

//______________________________________________________________________________
void TQtPadEditor::DoBorderSize(Int_t size)
{
   // Slot connected to the border size settings.

   fModel->SetBorderSize(size);
}
// Register the GED factory interfaces:
static TQtGedFactory<TQtPadEditor>   gQtPadEditor;
