// @(#)root/gui:$Name$:$Id: TQtLockWidget.h 3594 2013-02-19 03:50:53Z fineroot $
// Author: Valeri Fine 07/07/2006

#ifndef ROOT_TQtLockWidget
#define ROOT_TQtLockWidget

#include <QObject>

#include <QPointer>
#include <QWidget>


// Class to block the widget signal and update
class TQtLockWidget {
  private:
     QPointer<QWidget>    fWidget; //  Widget to lock
     bool                 fWasEnabled;
     bool                 fWasBlocked;
  public:
     TQtLockWidget(QWidget *w) : fWidget(w)
        , fWasEnabled(FALSE), fWasBlocked(FALSE)
         {
            if (fWidget) {
               if (( fWasEnabled = w->isEnabled()) )
                  fWidget->setUpdatesEnabled(!fWasEnabled);
               if (!(fWasBlocked=w->signalsBlocked())) 
                  fWidget->blockSignals(!fWasBlocked);
            }
         }
     ~TQtLockWidget() {
        if (fWidget) {
           if (fWasEnabled)  fWidget->setUpdatesEnabled(fWasEnabled);
           if (!fWasBlocked) fWidget->blockSignals(fWasBlocked) ;
        }
     }
};

#endif
