#ifndef STAR_TQROOTCOMMNANDCOMBO
#define STAR_TQROOTCOMMNANDCOMBO
// @(#)root/gt:$Name$:$Id: TQtRootCommandCombo.h 3497 2010-09-23 18:50:01Z fineroot $
// Author: Valeri Fine   11/01/2009

/****************************************************************************
** $Id: TQtRootCommandCombo.h 3497 2010-09-23 18:50:01Z fineroot $
**
** Copyright (C) 2009 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/
///////////////////////////////////////////////////////////////////////////
//
//  The TQtRootCommandCombo is a regular Qt QComboBox class that can send the text
//  entered by the user to the RootCint processor as soon as the user hits "return" key
//  by emitting the "CommandEntered(const QString &);" signal
//
//  The steering code can enable/disable the ROOT command execution via  
//  SetRootCommandExecute(bool on) method.
//
///////////////////////////////////////////////////////////////////////////
#include <QComboBox>

class QString;

class TQtRootCommandCombo : public QComboBox 
{
      Q_OBJECT
   private:
      QString fLastComboLine;
      bool   fRootCommandExecute;
   protected:
      void ConnectTreeSlots();
      void Init();

   public:
      TQtRootCommandCombo(QWidget *parent = 0);
      virtual ~TQtRootCommandCombo();
      const QString &ComboLine() const { return fLastComboLine;}
      void SetRootCommandExecute(bool on=true);
      bool IsRootCommnadExecute() const { return fRootCommandExecute;}
   public slots:
      void rootCommandExecute();
      void InsertFromHistory(int index=0);
   signals:
      void CommandEntered(const QString &);

};
#endif
