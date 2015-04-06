//
//
// THIS IS MODIFIED VERSION OF THE FILE, below are the original notes.
// 
//

// Author: Valeri Fine   3/02/2007
/****************************************************************************
** $Id: TQtFloatSlider.cxx 2454 2007-05-22 00:44:46Z fine $
**
** Copyright (C) 2007 by Valeri Fine. Brookhaven National Laboratory.
**                                    All rights reserved.
**
** For the licensing terms see LICENSE_QtRoot.
**
*****************************************************************************/

#include "TQtFloatSlider.h" 
#include "TMath.h"

//______________________________________________________________________________
TQtFloatSlider::TQtFloatSlider( QWidget* parent, const char* name ):
QSlider(parent),fMinValue(0),fMaxValue(1)
{  
    setObjectName(name);
   setRange(0,kFloatSliderPrecision); 
   // Constructs a vertical slider. 
   ConnectSingals();
}
//______________________________________________________________________________
TQtFloatSlider::TQtFloatSlider ( Qt::Orientation orientation, QWidget *parent, const char *name):
QSlider(orientation, parent),fMinValue(0),fMaxValue(1)
{
  // Constructs a slider. 
  // The orientation must be Qt::Vertical or Qt::Horizontal. 
  // The parent and name arguments are sent on to the QWidget constructor. 
    setObjectName(name);
   setRange(0,kFloatSliderPrecision); 
   ConnectSingals();
}

//______________________________________________________________________________
TQtFloatSlider::TQtFloatSlider ( int minValue, int maxValue, int pageStep, int value, Qt::Orientation orientation, QWidget * parent, const char * name)
: QSlider(orientation,parent) 
{
   // special ctor for the integers. It should behave as the original QSlider 
    setMinimum(minValue);
    setMaximum(maxValue);
    setPageStep(pageStep);
    setValue(value);
    setObjectName(name);
   ConnectSingals();
}

//______________________________________________________________________________
TQtFloatSlider::TQtFloatSlider ( double minValue, double maxValue, double pageStep, double value, Qt::Orientation orientation, QWidget * parent, const char * name)
:QSlider(orientation,parent),fMinValue(minValue),fMaxValue(maxValue)
{
   // Constructs a slider whose value can never be smaller than minValue or greater than maxValue, 
   // whose page step size is pageStep and whose value is initially value 
   // (which is guaranteed to be in range using bound()). 
   //
   // If orientation is Qt::Vertical the slider is vertical and 
   // if it is Qt::Horizontal the slider is horizontal. 
   //
   // The parent and name arguments are sent on to the QWidget constructor. 
    setMinimum(0);
    setMaximum(kFloatSliderPrecision);
    SetLineStep(1);
    setPageStep(0);
    setObjectName(name);
   SetValue   (value);
   SetPageStep(pageStep);

   ConnectSingals();
}

//______________________________________________________________________________
void TQtFloatSlider::ConnectSingals()
{
    // Connect the original QSlider signals to our slots
   connect(this,SIGNAL(valueChanged(int)), this, SLOT(EmitValueSignal(int)));
   connect(this,SIGNAL(sliderMoved(int)), this, SLOT(EmitMoveSignal(int)));
}

//______________________________________________________________________________
//
//  Property
//______________________________________________________________________________

//______________________________________________________________________________
void  TQtFloatSlider::SetMinValue( double v) 
{ 
   // Sets the current minimum value of the slider
   // When setting this property, the QSlider::maxValue is adjusted, 
   // if necessary, to ensure that the range remains valid. 
   double currentRange = Range();
   double currentValue = Value();
   fMinValue = v;
   fMaxValue = fMinValue + currentRange;
   // restore the Value
   blockSignals(true);
     SetValue(currentValue);
   blockSignals(false);
}
//______________________________________________________________________________
void  TQtFloatSlider::SetMaxValue( double v) 
{ 
   // Sets the current maximum value of the slider.
   // When setting this property, the QSlider::minValue is adjusted, if necessary, 
   // to ensure that the range remains valid. 
   double currentRange = Range();
   double currentValue = Value();
   fMaxValue = v;
   fMinValue = fMaxValue - currentRange;
   // restore the Value
   blockSignals(true);
     SetValue(currentValue);
   blockSignals(false);
}
   
//______________________________________________________________________________
double TQtFloatSlider::MinValue() const
{ 
   // Returns the current minimum value of the slider.
   return ToDouble(minimum());
}

//______________________________________________________________________________
double	TQtFloatSlider::MaxValue() const
{  
  // Returns the current maximum value of the slider.
  return  ToDouble(maximum());
}

//______________________________________________________________________________
double TQtFloatSlider::Value() const
{ 
   // Returns the current slider value
   return  ToDouble(value());
} 

//______________________________________________________________________________
void  TQtFloatSlider::SetValue   ( double v)
{ 
   // Sets the current slider value. 
   setValue(ToInt(v)); 
}
//______________________________________________________________________________
void  TQtFloatSlider::SetRange( double minValue, double maxValue)
{ 
   // Sets the range control's minimum value to minValue 
   // and its maximum value to maxValue.
   
   // Calls the virtual rangeChange() function if one or both of the 
   // new minimum and maximum values are different from the previous setting. 
   // Calls the virtual valueChange() function if the current value 
   // is adjusted because it was outside the new range. 

   // If maxValue is smaller than minValue, minValue becomes the only legal value. 
   fMinValue = minValue;
   fMaxValue = maxValue;
}

//______________________________________________________________________________
void TQtFloatSlider::SetTickInterval(double v)
{
  setTickInterval(ToInt(v)); 
}
//______________________________________________________________________________
double TQtFloatSlider::TickInterval () const
{
   return ToDouble(tickInterval());
}
//______________________________________________________________________________
double TQtFloatSlider::PageStep() const
{
  return ToDouble(pageStep()); 
}

//______________________________________________________________________________
void TQtFloatSlider::SetPageStep(double v)
{
  setPageStep(ToInt(v)); 
}
//______________________________________________________________________________
void   TQtFloatSlider::SetLineStep(double v)
{
  setSingleStep(ToInt(v)); 
}
//______________________________________________________________________________
double  TQtFloatSlider::LineStep() const
{
   return  ToDouble(singleStep());
}

//______________________________________________________________________________
//
//   Custom signals / slots 
//______________________________________________________________________________
void TQtFloatSlider::EmitValueSignal(int) {
   // emit float value changed signa;
   emit ValueChanged(Value());
}
//______________________________________________________________________________
void TQtFloatSlider::EmitMoveSignal(int) {
   // emit float value changed signa;
   emit SliderMoved(Value());
}

