//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtIconBrowserImp.h 2792 2008-06-18 14:31:19Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtIconBrowserImp
#define ROOT_TQtIconBrowserImp

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtIconBrowserImp                                                        //
//                                                                      //
// ABC describing GUI independent browser implementation protocol.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TBrowserImp.h"
#include "TQtUpdateViewFlag.h"
#include "TQtGui.h"

#include <qstring.h>

#include "qoot/QtRoot/Qt3ToQt5PortingHelpers.h"

class TQtBrowserItem;
class TBrowserCustom;

  class TQtIconBrowserItem : public QObject, public Q3IconViewItem {
    Q_OBJECT
protected:
  TObject *fObject;
  const QIcon *fIconSet;
public:

  TQtIconBrowserItem( TObject *obj,Q3IconView * parent, QString label1) : Q3IconViewItem(parent,label1), fObject(obj) {;}
  TQtIconBrowserItem( TObject *obj,Q3IconView * parent, Q3IconViewItem * after ): Q3IconViewItem(parent,after ), fObject(obj){}

  void Browse(TBrowser *b){ if (fObject && b) fObject->Browse(b);}
  TObject *Object(){ return fObject;}
  void SetIconSet(const QIcon *set, Q3Icon::Size size, QIcon::Mode mode=QIcon::Normal, QIcon::State state=QIcon::Off)
  {fIconSet = set; SetPixmap(size,mode,state); }
  void SetPixmap(Q3Icon::Size size, QIcon::Mode mode=QIcon::Normal, QIcon::State state=QIcon::Off)
  { Q3IconViewItem::setPixmap(fIconSet->pixmap(size,mode,state));}
};

class TQMimeTypes;
class TQtIconBrowserImp :  public QObject, public TBrowserImp {
  Q_OBJECT
    friend class ev;
protected:
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
  QWidgetStack     *fBrowserImpID;
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
  Q3WidgetStack     *fBrowserImpID;
//MOC_SKIP_END
#endif /* QT_VERSION */
  Int_t             fIconWidgetId;
  Int_t             fDetailWidgetID;
  Int_t             fX,fY;
  UInt_t            fWidth,fHeight;
  QString           fTitle;  
  Bool_t            fFolderExpanded;
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
  QIconSet::Size    fIconSize;
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
  Q3Icon::Size    fIconSize;
//MOC_SKIP_END
#endif /* QT_VERSION */
  TQtUpdateViewFlag fUpdate;
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
  QListView        *fStackBrowser;
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
  Q3ListView        *fStackBrowser;
//MOC_SKIP_END
#endif /* QT_VERSION */
  TQtBrowserItem   *fCurrentItem;
  TBrowserCustom   *fBrowserCustom;
  TObject          *fRootObject;
  TQtGui::TQtIconViewOptions fCurentViewMode;

  static TQMimeTypes *gfMimeTypeList;

  static void         CreateIcons();
public:

   enum EListOfIcons {kMainROOTIcon, kCanvasIcon, kBrowserIcon, kClosedFolderIcon, kOpenedFolderIcon,  kDocumentIcon, kTotalNumOfICons };

   static TQMimeTypes *IconList();
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
  static const QIconSet  *Shape2GeoShapeIcon(const char *shapeName);
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
   static const QIcon  *Shape2GeoShapeIcon(const char *shapeName);
//MOC_SKIP_END
#endif /* QT_VERSION */

   TQtIconBrowserImp(TBrowser *b=0,bool initFlag=true);
   TQtIconBrowserImp(TBrowser *b, const char *title, UInt_t width, UInt_t height,bool initFlag=true);
   TQtIconBrowserImp(TBrowser *b, const char *title, Int_t x, Int_t y, UInt_t width, UInt_t height,bool initFlag=true);
   virtual ~TQtIconBrowserImp();

   TQtIconBrowserItem * Add(TObject *obj, const char *caption,const char *iconKey, Int_t check);

   // The base class overloaded methods:

   virtual void  Add(TObject *, const char *caption, Int_t check);
   virtual void  AddCheckBox(TObject *, Bool_t check);
   virtual void  CheckObjectItem(TObject *, Bool_t check);
   virtual void  RemoveCheckBox(TObject *);

   virtual void  Add(TObject *obj, const char *caption);
   virtual void  BrowseObj(TObject *);
   virtual void  ExecuteDefaultAction(TObject *);
   virtual Option_t *GetDrawOption() const;
   virtual void  Iconify();
   virtual void  RecursiveRemove(TObject *);
   virtual void  Refresh(Bool_t = kFALSE);
   virtual void  SetDrawOption(Option_t *option="");
   virtual void  Show();

   // TObject overloaded methods:
   const QString& GetTitle() const;
   QPaintDevice *GetBrowserID();
   // This class own data-members:
protected:
   // This class own methods:
   void   CreateStackBrowser();
   void   CreateDetailView();
   // void ResetPixmaps();

public:
   virtual Int_t InitWindow(Bool_t show=kFALSE);

public slots:
   void Add(const TQtBrowserItem *item);
   void BrowseObject(TObject *obj);
   void BrowseParentObject();
   void Chdir(const TQtBrowserItem *item);
   void Clear(TObject *);
   void FolderExpanded(TObject *obj, Bool_t expand=kTRUE);
   void EnableUpdates(Bool_t updt=kTRUE);
   void SetViewMode(int mode);
   void SetSortIndicator(int section);
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
   void SetIconSize(QIconSet::Size size);
   void StackClicked(QListViewItem *item);
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
   void SetIconSize(Q3Icon::Size size);
   void StackClicked(Q3ListViewItem *item);
//MOC_SKIP_END
#endif /* QT_VERSION */


protected slots:
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
   void ReplaceStack(const QPtrVector<TQtBrowserItem> &folderList);
   void ClickedItem(QIconViewItem *item);
   void PopMenu(QIconViewItem *item, const QPoint &pos);
   void SelectionChanged(QIconViewItem *item);
#endif
#else /* QT_VERSION */
//MOC_SKIP_BEGIN
   void ReplaceStack(const Q3PtrVector<TQtBrowserItem> &folderList);
   void ClickedItem(Q3IconViewItem *item);
   void PopMenu(Q3IconViewItem *item, const QPoint &pos);
   void SelectionChanged(Q3IconViewItem *item);
//MOC_SKIP_END
#endif /* QT_VERSION */

signals:
   void ActivateObject(TObject *);
   void ActivateParent(TObject *);
   void StackHasBeenCreated(Q3ListView *);
   void SwitchTreeView(Int_t flag=0);
   void ResetActionRequest(int viewMode);
};
inline  const QString& TQtIconBrowserImp::GetTitle() const { return fTitle;        }
#if QT_VERSION < 0x40000
#ifndef Q_MOC_RUN
inline  void TQtIconBrowserImp::SetIconSize(QIconSet::Size size) { fIconSize = size;     }
#endif
#else /* QT_VERSION */
inline  void TQtIconBrowserImp::SetIconSize(Q3Icon::Size size) { fIconSize = size;     }
#endif /* QT_VERSION */

#endif
