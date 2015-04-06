// @(#)root/base:$Name$:$Id: TBrowserCustom.h 642 2004-01-24 23:23:43Z fine $
// Author: Valeri Fine  10/01/2004

/****************************************************************************
** $Id: TBrowserCustom.h 642 2004-01-24 23:23:43Z fine $
**
** Copyright (C) 2004 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/


#ifndef ROOT_TBrowserCustom
#define ROOT_TBrowserCustom

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBrowserCustom                                                       //
//                                                                      //
// This browser has the same behavior as the base one.                  //
//                                                                      //
//  The only difference is the parentship.                              //
//  TBrowser creates TBrowserImp                                        //
//  TBrowserCustom is to be created by TBrowserImp                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TBrowser
#include "TBrowser.h"
#endif


class TBrowserCustom : public TBrowser {
public:
   TBrowserCustom(TBrowserImp *imp=0);
   virtual ~TBrowserCustom();
   void SetBrowserImp(TBrowserImp *imp);
   ClassDef(TBrowserCustom,0)  //Customized ROOT Object Browser
};

#endif
