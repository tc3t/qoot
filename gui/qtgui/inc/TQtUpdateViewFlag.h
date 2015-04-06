//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

#ifndef ROOT_TQTUPDATEVIEWFLAG
#define ROOT_TQTUPDATEVIEWFLAG
// Author: Valeri Fine   30/04/2003
/****************************************************************************
** $Id: TQtUpdateViewFlag.h 3601 2013-02-24 04:20:15Z fineroot $
**
** Copyright (C) 2002 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/
#include <QtGlobal>
#include "qoot/QtRoot/Qt3ToQt5PortingHelpers.h"

/////////////////////////////////////////////////////////////
//                                                         //
//  Small counter class to avoid redundant lock/ unlock    //
//  Scrollview to update                                   //
//                                                         //
/////////////////////////////////////////////////////////////
class TQtUpdateViewFlag {
private:
   unsigned int  fCounter;
   TQtUpdateViewFlag(const TQtUpdateViewFlag &){;}
public:
   TQtUpdateViewFlag(): fCounter (0){};
   void FreezeToUpdate(Q3ScrollView *view){
      if (!fCounter) {
         view->setUpdatesEnabled( false );
         view->viewport()->setUpdatesEnabled(false);
      }
      fCounter++;
   }
   void FreezeToUpdate(Q3ListView *view)
   {
   }

   void FreezeToUpdate(Q3IconView *view)
   {
   }

   void UnFreezeToUpdate(Q3ScrollView *view){ 
      if (fCounter) fCounter--; 
      if (!fCounter) {
         view->viewport()->setUpdatesEnabled( true );
         view->setUpdatesEnabled( true );
         view->repaintContents();
      }
   }

   void UnFreezeToUpdate(Q3ListView *view)
   {
   }

   void UnFreezeToUpdate(Q3IconView *view)
   {
   }
};

#endif


