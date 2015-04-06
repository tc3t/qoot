#include "TQtTabValidator.h"
#include "TTabCom.h"
#include <sstream>
/****************************************************************************
** $Id: TQtTabValidator.cxx 3601 2013-02-24 04:20:15Z fineroot $
**
** Copyright (C) 2003 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

TTabCom *TQtTabValidator::fgTabCom = 0;
//_________________________________________________________________________________________________________
void TQtTabValidator::Clear()  
{
   if (fgTabCom) fgTabCom->ClearAll();
}
//_________________________________________________________________________________________________________
QValidator::State TQtTabValidator::validate(QString &input, int &pos) const {
   if (!fgTabCom) fgTabCom = new TTabCom();
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,30,0)
   std::stringstream out;
   fgTabCom->Hook(input.toLatin1().data(), &pos,out);
   input = out.str().c_str();
#else
   char buffer[2048];
   qstrcpy(buffer,(const char *)input);
   fgTabCom->Hook(buffer, &pos);
   input = buffer;
#endif   
   return QValidator::Acceptable; // Intermediate;
}
