// Author: Valeri Fine   21/01/2002
/****************************************************************************
** $Id: TQtGui.h 2785 2008-06-16 05:31:57Z fine $
**
** Copyright (C) 2002 by Valeri Fine.  All rights reserved.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
*****************************************************************************/

#ifndef ROOT_TQtNAMESPACE
#define ROOT_TQtNAMESPACE

class QPixmap;
class QString;

namespace TQtGui {
   enum TQtIconViewOptions { kViewLargeIcons, kViewSmallIcons, kViewList, kViewDetails,kNotDefinedYet};
   bool  AddPicture(const QPixmap &pic, const char *pictureName, bool checkCache=true);
   const QPixmap &GetPicture(QString &pictureName);
   const QPixmap &GetPicture(const char *pictureName);
}
#endif
