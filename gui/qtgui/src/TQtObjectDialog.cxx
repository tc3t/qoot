// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtObjectDialog.cxx 3631 2014-03-17 02:10:47Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/

#include "TQtObjectDialog.h"
#include "TGQt.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QDebug>
#include <QString>
#include <QTextCodec> 

#include "TClass.h"

#include "TContextMenu.h"
#include "TMethod.h"
#include "TMethodArg.h"
#include "TDataType.h"
#include "TMethodCall.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TObjString.h"

//______________________________________________________________________________
TQtObjectDialog::TQtObjectDialog( TObject *object, TMethod *method ):QDialog(0,0), fParArray(0)
{
    if ( !( object && method ) ) return;
    // Create Dummy TContextMenu for the sake of its static method ???
    setWindowModality ( Qt::WindowModal );
    TContextMenu dummy(0);

    setWindowTitle(dummy.CreateDialogTitle( object, method ));

    /* Some common values for all controls */

        TMethodArg *argument = NULL;
        TIter next( method->GetListOfMethodArgs() );

        QVBoxLayout *layout = new QVBoxLayout( this );
        while ( (argument = (TMethodArg *) next() ) ) {
            // Create a label gadget.
//*-* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//*-*          Label      as STATIC control
          const char *argname  = dummy.CreateArgumentTitle( argument );
//------------------ begin of copy / paste from void TRootContextMenu::Dialog(TObject *object, TMethod *method)
         const Text_t *type       = argument->GetTypeName();
         TDataType    *datatype   = gROOT->GetType(type);
         const Text_t *charstar   = "char*";
         Text_t        basictype [32];
         if (datatype) {
           strcpy(basictype, datatype->GetTypeName());
         } else {
            if (strncmp(type, "enum", 4) != 0)
       //        Warning("Dialog", "data type is not basic type, assuming (int)");
           strcpy(basictype, "int");
        }
        if (strchr(argname, '*')) {
           strcat(basictype, "*");
           type = charstar;
        }
        TDataMember *m = argument->GetDataMember();
        Text_t val[256]= "";
        if (m && m->GetterMethod()) {
         // WARNING !!!!!!!!
         // MUST "reset" getter method!!! otherwise TAxis methods doesn't work!!!
         Text_t gettername[256] = "";
         strcpy(gettername, m->GetterMethod()->GetMethodName());
         m->GetterMethod()->Init(object->IsA(), gettername, "");
         // Get the current value and form it as a text:
         if (!strncmp(basictype, "char*", 5)) {
            Text_t *tdefval;
            m->GetterMethod()->Execute(object, "", &tdefval);
            strncpy(val, tdefval, 255);
         } else if (!strncmp(basictype, "float", 5) ||
                    !strncmp(basictype, "double", 6)) {
            Double_t ddefval;
            m->GetterMethod()->Execute(object, "", ddefval);
            sprintf(val, "%g", ddefval);
         } else if (!strncmp(basictype, "char", 4) ||
                    !strncmp(basictype, "int", 3)  ||
                    !strncmp(basictype, "long", 4) ||
                    !strncmp(basictype, "short", 5)) {
            Long_t ldefval;
            m->GetterMethod()->Execute(object, "", ldefval);
            sprintf(val, "%li", ldefval);
         }
         // Find out whether we have options ...
         TList *opt;
         if ((opt = m->GetOptions())) {
           // -- Warning("Dialog", "option menu not yet implemented", opt);
         }
       } else {    // if m not found ...
         const char *tval = argument->GetDefault();
         if (tval) strncpy(val, tval, 255);
      }
//------------------ end of copy / paste from void TRootContextMenu::Dialog(TObject *object, TMethod *method)

       QLabel *label = new QLabel(argname,this);
       layout->addWidget(label);

//*-*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//*-*                          text window
//*-*                Input TEXT window as QLineEdit widget

//*-*     Save  an "active" wId
          QLineEdit *line = new QLineEdit( gQt->GetTextDecoder()->toUnicode(val),this);
          layout->addWidget(line);
           // remember the first widget to put initial focus on it
        }
        // Create Layout
        QDialogButtonBox *box = new QDialogButtonBox(this);
        layout->addWidget(box);

        box->addButton(QDialogButtonBox::Ok);
        connect(box,SIGNAL(accepted ()),this,SLOT(accept ()));

        box->addButton(QDialogButtonBox::Cancel);
        connect(box,SIGNAL(rejected ()),this,SLOT(reject()));

        box->addButton(QDialogButtonBox::Help);
        connect(box,SIGNAL(helpRequested()),this,SLOT(emitHelpRequest()));
}
//______________________________________________________________________________
void TQtObjectDialog::accept () 
{
   // Collect the results from all LinedEdit widgets
   QList<QLineEdit*> list = findChildren<QLineEdit*>();
   fParArray = new TObjArray(list.size());
   Int_t index = 0;
   for (int i = 0; i < list.size(); ++i) {
      QString r = gQt->GetTextDecoder()->fromUnicode( list.at(i)->text() );
      fParArray->AddAt((TObject *)(new TObjString(r.toLatin1().data())),index);
      index++;
   }
  QDialog::accept();
}
//______________________________________________________________________________
void TQtObjectDialog::emitHelpRequest()
{  
   // [slot] to emit signal on behalf of the internal dialog box
   emit helpRequested();
}
