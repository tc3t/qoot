//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtGui.h 2785 2008-06-16 05:31:57Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtNAMESPACE
#define ROOT_TQtNAMESPACE

class QPixmap;
class QString;

namespace TQtGui {
   enum TQtIconViewOptions { kViewLargeIcons, kViewSmallIcons, kViewList, kViewDetails,kNotDefinedYet};
   bool  AddPicture(const QPixmap &pic, const char *pictureName, bool checkCache=true);
   const QPixmap &GetPicture(const QString &pictureName);
   const QPixmap &GetPicture(const char *pictureName);
}
#endif
