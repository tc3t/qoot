//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtContextMenuImp.cxx 3631 2014-03-17 02:10:47Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
*****************************************************************************/

#include "TQtContextMenuImp.h"
#if QT_VERSION >= 0x40000
#  include <QMenu>
#  include <QClipboard>
#  include <QDebug>
#if QT_VERSION < 0x50000
  // I can find thsi on Mac ! under qt 5.2 yet
#  include <QWebView>  
#endif
//#  include <QNetworkProxy>
#  include <QString>
#  include <QUrl>
#else
#  include "qpopupmenu.h"
#  include <qclipboard.h>
#  include <qlabel.h>
#endif /* QT_VERSION */

#include <QGridLayout>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "TGQt.h"
#include "TSystem.h"
#include "TQtLock.h"
#include <qclipboard.h> 
#include "TCanvas.h"
#include "TClass.h"

#include "TMethod.h"
#include "TBrowser.h"
#include "TMethodArg.h"
#include "TDataType.h"
#include "TMethodCall.h"
#include "TROOT.h"
#include "TEnv.h"

#include "TContextMenuImp.h"
#include "TObjArray.h"
#include "TApplication.h"
#include "TQtApplication.h"
#include "TQtObjectDialog.h"
#include "TObjectExecute.h"

#include <TF1.h>


//______________________________________________________________________________
TQtContextMenuImp::TQtContextMenuImp(TContextMenu *c) :  TContextMenuImp(c), fPopupMenu(0)
,fExecute(0), fHelpWidget(0)
{
   // Create the ROOT Context menu implmenation and optional WebView
   // to access the class defininition from ROOT Web site
    fExecute = new TObjectExecute();
}
//______________________________________________________________________________
TQtContextMenuImp::~TQtContextMenuImp()
{
   // destroy the WebView if any
#if QT_VERSION < 0x50000
   delete fHelpWidget;
#endif
   fHelpWidget = 0;
   // destroy all menu item first
   foreach (TQtMenutItem *it, fItems) { delete it; }
   DeletePopup();
}
//______________________________________________________________________________
void TQtContextMenuImp::DeletePopup()
{
  if (fPopupMenu) {
    fPopupMenu->disconnect(this);
    QMenu   *m = fPopupMenu; fPopupMenu = 0;
    delete m; 
  }
  ClearProperties();
  if (fExecute) {
    TObjectExecute  *e=fExecute; fExecute = 0; 
    delete e;
  }
}
//______________________________________________________________________________
void TQtContextMenuImp::CreatePopup ( TObject * /*object*/ ) 
{}
//______________________________________________________________________________
void TQtContextMenuImp::CreatePopup () {
  TContextMenu *c;

//*-*   Find the parent canvas window

  if ( (c=GetContextMenu()) ) {
    TCanvas *canvas = (TCanvas*)c->GetSelectedCanvas();
    QWidget *parent = 0;
    if (canvas) {
      parent = (QWidget *)TGQt::iwid(canvas->GetCanvasID());
    }
//*-*  Add a title. 
    if (fPopupMenu) delete fPopupMenu;
    fPopupMenu = 0;
    fPopupMenu = new QMenu("ContextMenu",parent);
    fPopupMenu->setSeparatorsCollapsible(false);
    connect(fPopupMenu,SIGNAL(destroyed()),this,SLOT(Disconnect()));
    connect(fPopupMenu,SIGNAL(aboutToShow () ),this,SLOT( AboutToShow() ));
//    fPopupMenu->setWindowTitle("Title");

//*-*  Include the menu title
    TObject *object = c? c->GetSelectedObject() : 0;
    QString titleBar = fContextMenu->CreatePopupTitle(object);
    fPopupMenu->setTitle(titleBar);
    QAction *action = fPopupMenu->addAction(titleBar);
    QFont af = action->font();
    af.setBold(true);
    action->setFont(af);

    fPopupMenu->addSeparator(); fPopupMenu->addSeparator();
//*-*  Include the standard static items into the context menu
    QAction *a = fPopupMenu->addAction("&Inspect",    this,SLOT(InspectCB()));
    a->setToolTip(tr("Open the ROOT Object Inspector"));
    a = fPopupMenu->addAction("&Copy",       this,SLOT(CopyCB()),QKeySequence::Copy);
    a->setToolTip(tr("Copy the object pointer to the clipboard"));
    a = fPopupMenu->addAction("&Browse",     this,SLOT(BrowseCB()));
    a->setToolTip(tr("Open the ROOT Object Browser"));
    if (object && dynamic_cast<TVirtualPad*>(object) != nullptr)
    {
        QMenu* pMenu = fPopupMenu->addMenu(tr("&Insert item to pad"));
        if (pMenu)
        {
            a = pMenu->addAction(tr("TF1"), this, SLOT(InsertItemToPad_TF1()));
        }
    }
  }
}
//______________________________________________________________________________
void TQtContextMenuImp::AboutToShow()
{
   // Slot to propagate the QPopupMenu signal further to the TContextMenu "clients"
   if (fPopupMenu) {
        emit AboutToShow(fPopupMenu,GetContextMenu());
   }
}
//______________________________________________________________________________
void TQtContextMenuImp::ClearProperties()
{ }
//______________________________________________________________________________
void  TQtContextMenuImp::Dialog( TObject *object, TMethod *method )
{
  if ( !( object && method ) ) return;
  TQtObjectDialog *d = new TQtObjectDialog(object,method);
  connect(d,SIGNAL(helpRequested()),this,SLOT(HelpCB()));
  if (d->exec() == QDialog::Accepted )  {
    TObjArray *parList = d->GetParamList();
    //if (fExecute) fExecute->Execute(object,method,parList);
    TContextMenu* c = GetContextMenu();
    if (c)
        c->Execute(object,method,parList); 
    if (fHelpWidget) {
#if QT_VERSION < 0x50000
          fHelpWidget->hide();
#endif
      }
  }
  delete d;
}
//______________________________________________________________________________
void  TQtContextMenuImp::Dialog( TObject *object, TFunction *function )
{
  if ( !( object && function ) ) return;
  TQtObjectDialog *d = new TQtObjectDialog(object,(TMethod *)function);
  connect(d,SIGNAL(helpRequested()),this,SLOT(HelpCB()));
  if (d->exec() == QDialog::Accepted )  {
    TObjArray *parList = d->GetParamList();
    if (fExecute) fExecute->Execute(function,parList);
    // TContextMenu *c=GetContextMenu();
    //  c->Execute(0,function,parList); 
#if QT_VERSION < 0x50000
      fHelpWidget->hide();
#endif
  }
  delete d;
}
//______________________________________________________________________________
void TQtContextMenuImp::Disconnect()
{
  TQtLock lock;
   // Popup menu has been destroyed from outside
  if (fPopupMenu) fPopupMenu = 0;
}
//______________________________________________________________________________
void  TQtContextMenuImp::DisplayPopup ( Int_t x, Int_t y)
{
  TContextMenu *m = GetContextMenu();
  if (!m) return;

  CreatePopup();

//*-*   Update a popup

  UpdateProperties();

//*-*   Display Popup
  QWidget *w = (QWidget *)fPopupMenu->parent();
  QPoint pop = QPoint(x,y);
  if (w) pop = w->mapToGlobal(QPoint(x,y));
  fPopupMenu->popup(pop);
}
//______________________________________________________________________________
void TQtContextMenuImp::UpdateProperties()
{
  TContextMenu *c = GetContextMenu();
  TObject *object = c? c->GetSelectedObject() : 0;
  if (object)
  {
    //*-*   Change title
    fPopupMenu->setTitle(fContextMenu->CreatePopupTitle(object));
    
    //*-*  Include the "Properties" item "by canvases"
    fPopupMenu->addSeparator();
    QMenu *propertiesMenu =fPopupMenu; 
    // Use the ROOT default style
    const char *env =  gEnv->GetValue("Gui.Factory","qtgui");
    if (env && !strcmp(env,"qtgui")) {
       // USe QtRoot default style
       propertiesMenu = fPopupMenu->addMenu("&Properties");
    }

    //*-*  Create Menu "Properties"

    TClass *classPtr = NULL;
    TMethod *method  = NULL;

    //*-*  Create a linked list
    TList *methodList = new TList();
    object->IsA()->GetMenuItems( methodList );
    TIter next( methodList );
    foreach (TQtMenutItem *it, fItems) { delete it; }
    fItems.clear();
    while ( ( method = (TMethod *) next () ) ) {

       if ( classPtr != method->GetClass() ) {
          //*-*  Add a separator.
          if (classPtr) propertiesMenu->addSeparator();
          classPtr = method->GetClass();
       }
       //*-*  Create a popup item.
       TQtMenutItem *menuItem = new TQtMenutItem(c,method,object);
       fItems.push_back(menuItem);
       propertiesMenu->addAction(method->GetName(),menuItem,SLOT(Exec()));
    }
    // Delete linked list of methods.
    delete methodList;
  }
}
//______________________________________________________________________________
void TQtContextMenuImp::InspectCB()
{
   // Open the ROOT Object Inspector 
  TContextMenu *c = GetContextMenu();
  TObject *object = c? c->GetSelectedObject() : 0;
  if (object) object->Inspect();
}
//______________________________________________________________________________
void TQtContextMenuImp::BrowseCB()
{
   // Open the ROOT Object Browser 
  TContextMenu *c = GetContextMenu();
  TObject *object = c? c->GetSelectedObject() : 0;
  if (object) new TBrowser(object->GetName(),object);
}
//______________________________________________________________________________
void TQtContextMenuImp::InsertItemToPad_TF1()
{
    auto pC = GetContextMenu();
    auto pPad = pC ? dynamic_cast<TVirtualPad*>(pC->GetSelectedObject())
                    : nullptr;
    if (pPad)
    {
        QDialog dlg;
        dlg.setWindowTitle(tr("Define new TF1"));
        QGridLayout* pLayout = new QGridLayout(&dlg);
        auto pEditName = new QLineEdit(&dlg);
        auto pEditFormula = new QLineEdit(&dlg);
        auto pEditMinX = new QLineEdit(&dlg);
        auto pEditMaxX = new QLineEdit(&dlg);
        pLayout->addWidget(new QLabel(tr("Name"), &dlg), 0, 0);
        pLayout->addWidget(pEditName, 0, 1);
        pLayout->addWidget(new QLabel(tr("Formula"), &dlg), 1, 0);
        pLayout->addWidget(pEditFormula, 1, 1);
        pLayout->addWidget(new QLabel(tr("Minimum x"), &dlg), 2, 0);
        pLayout->addWidget(pEditMinX, 2, 1);
        pLayout->addWidget(new QLabel(tr("Maximum x"), &dlg), 3, 0);
        pLayout->addWidget(pEditMaxX, 3, 1);
        auto pDlgButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                    QDialogButtonBox::Cancel,
                                                  &dlg);
        connect(pDlgButtonBox, SIGNAL(accepted()), &dlg, SLOT(accept()));
        connect(pDlgButtonBox, SIGNAL(rejected()), &dlg, SLOT(reject()));
        pLayout->addWidget(pDlgButtonBox, 4, 1, 1, 1);
        if (dlg.exec() == QDialog::Accepted)
        {
            pPad->CreateAndDrawPrimitive<TF1>("same",
                                                pEditName->text().toLatin1(),
                                                pEditFormula->text().toLatin1(),
                                                pEditMinX->text().toDouble(),
                                                pEditMaxX->text().toDouble());
            pPad->Update();
        }
    }
}
//______________________________________________________________________________
void TQtContextMenuImp::CopyCB()
{
   // Copy the object pointer to the system clipboard
  TContextMenu *c = GetContextMenu();
  TObject *object = c? c->GetSelectedObject() : 0;
  QString className = object->ClassName();
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(
               QString("((%1 *)0x%2)->")
              .arg(className)
              .arg((ulong)object,0,16)
              ,clipboard->supportsSelection()
              ?QClipboard::Selection 
              :QClipboard::Clipboard);
}
//______________________________________________________________________________
void TQtContextMenuImp::HelpCB()
{
  // Pop Web page with the class HTML doc from web site defined by "Browser.StartUrl"
  TObject *obj = fContextMenu->GetSelectedObject(); 
  if (obj) { 
     QString clname = obj->ClassName(); 
     QString url = gEnv->GetValue("Browser.StartUrl", "http://root.cern.ch/root/html/"); 
     if (url.endsWith(".html",Qt::CaseInsensitive))  url = url.left(url.lastIndexOf("/")+1);
     if (!url.endsWith("/")) url += "/";
     if (fContextMenu->GetSelectedMethod()) { 
         TString smeth = fContextMenu->GetSelectedMethod()->GetName(); 
         TMethod *method = obj->IsA()->GetMethodAllAny(smeth.Data()); 
         if (method) clname = method->GetClass()->GetName(); 
         url += QString("%1.html#%1:%2").arg(clname).arg(smeth.Data()); 
     } else { 
         url += QString("%1.html").arg(clname); 
     } 
#if QT_VERSION < 0x50000
     if (!fHelpWidget) {
        // Set proxy
        QString http_proxy = gSystem->Getenv("http_proxy");
        if (!http_proxy.isEmpty()) {
           if (!http_proxy.contains("//")) {
              http_proxy = "http://" + http_proxy;
           }
           QNetworkProxy proxy;
           QUrl url(http_proxy);
           proxy.setType(QNetworkProxy::HttpProxy);
           proxy.setHostName(url.host());
           proxy.setPort(url.port());
           QNetworkProxy::setApplicationProxy(proxy); 
        }
#if QT_VERSION >= 0x40500
        else {
           // attempt to get the system defined proxy
           QList<QNetworkProxy> list = QNetworkProxyFactory::systemProxyForQuery();
           // Find one that suitable for http:
           for (int i = 0; i < list.size(); ++i) {
              if (list.at(i).type() == QNetworkProxy::HttpProxy) {
                QNetworkProxy::setApplicationProxy(list.at(i));
                break;
              }
           }
        }
#endif
        fHelpWidget = new QWebView;
        fHelpWidget->resize(400,400);
     }
     fHelpWidget->setUrl(url);
     fHelpWidget->show();
#endif
  }
}
