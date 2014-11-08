// @(#)root/gui:$Id: TQtRedirectOutputGuard.h 3506 2010-05-14 00:12:46Z fine@BNL.GOV $
// Author: G. Ganis   10/10/2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TQtRedirectOutputGuard
#define ROOT_TQtRedirectOutputGuard

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TQtRedirectOutputGuard                                                //
//                                                                      //
// This class provides output redirection to a TGTextView in guaranteed //
// exception safe way. Use like this:                                   //
// {                                                                    //
//    TQtRedirectOutputGuard guard(textview);                            //
//    ... // do something                                               //
//    guard.Update();                                                   //
//    ... // do something else                                          //
// }                                                                    //
// when guard goes out of scope, Update() is called to flush what left  //
// on the screed and the output is automatically redirected again to    //
// the standard units.                                                  //
// The exception mechanism takes care of calling the dtors              //
// of local objects so it is exception safe.                            //
// Optionally the output can also be saved into a file:                 //
// {                                                                    //
//    TQtRedirectOutputGuard guard(textview, file, mode);                //
//    ... // do something                                               //
// }                                                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TRedirectGuardInterface
#include "TRedirectGuardInterface.h"
#endif

#include <QTextEdit>
#include <QPlainTextEdit>

template <class T>
class TQtRedirectOutputGuard : TRedirectGuardInterface {

private:
   T  &fTextView;

public:
   TQtRedirectOutputGuard(T &tv,
                         const char *flog = 0, const char *mode = "a"):
      TRedirectGuardInterface(flog,mode),fTextView(tv){}
   virtual ~TQtRedirectOutputGuard();
   void AddLine(const TString &line);
};

template <>
void  TQtRedirectOutputGuard<QTextEdit>::AddLine(const TString &line)
{ fTextView.append(line.Data()); }

template <>
 void  TQtRedirectOutputGuard<QPlainTextEdit>::AddLine(const TString &line) 
 {fTextView.appendPlainText(line.Data()); }    

#endif
