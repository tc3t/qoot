// Author: Valeri Fine   10/08/2004
/****************************************************************************
** $Id: TQtTabValidator.h 2563 2007-08-25 04:03:52Z fine $
**
** Copyright (C) 2003 by Valeri Fine.  All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
*****************************************************************************/

#ifndef ROOT_TQtTabValidator
#define ROOT_TQtTabValidator

#ifndef __CINT__
#  include <qvalidator.h> 
#else
  class QObject;
  class QValidator;
  typedef int State;
#endif

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtTabValidator                                                      //
//                                                                      //
// A Qt QValidator to use the ROOT TTabCom class to validate the string //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


class QString;
class TTabCom;

class TQtTabValidator  : public QValidator {
private:
    TQtTabValidator(const TQtTabValidator &t) : QValidator (0) {}
    void operator=(const TQtTabValidator &) {}
protected:
   static TTabCom *fgTabCom;  
   TQtTabValidator() : QValidator(0) {}
public:
   TQtTabValidator(QObject *parent, const char *name=0) :
#if QT_VERSION >= 0x040000
      QValidator(parent){ if (name && name[0]) {} }
#else
      QValidator(parent,name){}
#endif
   static void Clear();

   virtual State validate(QString &input, int &pos) const;
};
#endif
