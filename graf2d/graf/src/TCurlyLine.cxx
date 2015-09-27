// @(#)root/graf:$Id$
// Author: Otto Schaile   20/11/99

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//______________________________________________________________________________
//
// This class implements curly or wavy polylines typically used to draw Feynman diagrams.
// Amplitudes and wavelengths may be specified in the constructors,
// via commands or interactively from popup menus.
// The class make use of TPolyLine by inheritance, ExecuteEvent methods
// are highly inspired from the methods used in TPolyLine and TArc.
// The picture below has been generated by the tutorial feynman.
//Begin_Html
/*
<img src="gif/feynman.gif">
*/
//End_Html
//______________________________________________________________________________

#include "Riostream.h"
#include "TCurlyLine.h"
#include "TROOT.h"
#include "TVirtualPad.h"
#include "TVirtualX.h"
#include "TMath.h"
#include "TLine.h"
#include "TPoint.h"

Double_t TCurlyLine::fgDefaultWaveLength = 0.02;
Double_t TCurlyLine::fgDefaultAmplitude  = 0.01;
Bool_t   TCurlyLine::fgDefaultIsCurly    = kTRUE;

ClassImp(TCurlyLine)


//______________________________________________________________________________
TCurlyLine::TCurlyLine()
{
   // Default constructor.

   fX1         = 0.;
   fY1         = 0.;
   fX2         = 0.;
   fY2         = 0.;
   fWaveLength = 0.;
   fAmplitude  = 0.;
   fIsCurly    = fgDefaultIsCurly;
   fNsteps     = 0;
}


//______________________________________________________________________________
TCurlyLine::TCurlyLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t wl, Double_t amp)
{
   // Create a new TCurlyLine with starting point (x1, y1), end point (x2,y2).
   // The wavelength and amplitude are given in percent of the pad height.

   fX1         = x1;
   fY1         = y1;
   fX2         = x2;
   fY2         = y2;
   fWaveLength = wl;
   fAmplitude  = amp;
   fIsCurly    = fgDefaultIsCurly;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::Build()
{
   // Create a curly (Gluon) or wavy (Gamma) line.

   Double_t pixeltoX = 1;
   Double_t pixeltoY = 1;

   Double_t wavelengthPix,amplitudePix, lengthPix, hPix;
   Double_t px1, py1, px2, py2;
   if (gPad) {
      Double_t ww = (Double_t)gPad->GetWw();
      Double_t wh = (Double_t)gPad->GetWh();
      Double_t pxrange = gPad->GetAbsWNDC()*ww;
      Double_t pyrange = - gPad->GetAbsHNDC()*wh;
      Double_t xrange  = gPad->GetX2() - gPad->GetX1();
      Double_t yrange  = gPad->GetY2() - gPad->GetY1();
      pixeltoX  = xrange / pxrange;
      pixeltoY  = yrange/pyrange;
      hPix  = TMath::Max(gPad->GetAbsHNDC() * gPad->GetWh(), gPad->GetAbsWNDC() * gPad->GetWw());
      px1      = gPad->XtoAbsPixel(fX1);
      py1      = gPad->YtoAbsPixel(fY1);
      px2      = gPad->XtoAbsPixel(fX2);
      py2      = gPad->YtoAbsPixel(fY2);

      lengthPix = TMath::Sqrt((px2-px1)*(px2-px1) + (py1-py2)*(py1-py2));
      wavelengthPix = hPix*fWaveLength;
      amplitudePix  = hPix*fAmplitude;
   } else {
      wavelengthPix = fWaveLength;
      amplitudePix  = fAmplitude;
      px1           = fX1;
      py1           = fY1;
      px2           = fX2;
      py2           = fY2;
      lengthPix = TMath::Sqrt((px2-px1)*(px2-px1) + (py1-py2)*(py1-py2));
   }
   // construct the curly / wavy line in pixel coordinates at angle 0
   Double_t anglestep = 40;
   Double_t phimaxle  = TMath::Pi() * 2. / anglestep ;
   Double_t dx        = wavelengthPix / 40;
   Double_t len2pi    = dx * anglestep;

   // make sure there is a piece of straight line a both ends

   Double_t  lengthcycle = 0.5 * len2pi + 2 * amplitudePix;
   // if (fIsCurly) lengthcycle +=  amplitudePix;
   Int_t nperiods = (Int_t)((lengthPix - lengthcycle) / len2pi);
   Double_t restlength = 0.5 * (lengthPix - nperiods * len2pi - lengthcycle);
   fNsteps = (Int_t)(anglestep * nperiods + anglestep / 2 + 4);
   if (fNsteps < 2) fNsteps = 2;
   SetPolyLine(fNsteps);
   Double_t *xv = GetX();
   Double_t *yv = GetY();
   xv[0] = 0;          yv[0] = 0;
   xv[1] = restlength; yv[1] = 0;
   Double_t phase =  1.5 * TMath::Pi();
   Double_t x0 = amplitudePix + restlength;
   Int_t i;
   for(i = 2; i < fNsteps-1; i++){
   // distinguish between curly and wavy
      if (fIsCurly) xv[i] = x0 + amplitudePix * TMath::Sin(phase);
      else         xv[i] = x0;
      yv[i]  = amplitudePix*TMath::Cos(phase);
      phase += phimaxle;
      x0    += dx;
   }
   xv[fNsteps-1] = lengthPix; yv[fNsteps-1] = 0;

   if (InheritsFrom("TCurlyArc")) return;  // called by TCurlyArc

   // rotate object and transform back to user coordinates
   Double_t angle = TMath::ATan2(py2-py1, px2-px1);
   if (angle < 0) angle += 2*TMath::Pi();

   Double_t cosang = TMath::Cos(angle);
   Double_t sinang = TMath::Sin(angle);
   Double_t xx, yy;

   for(i = 0; i < fNsteps; i++){
      xx = xv[i] * cosang - yv[i] * sinang;
      yy = xv[i] * sinang + yv[i] * cosang;
      if (gPad) {
         xx *= pixeltoX;
         yy *= pixeltoY;
      }
      xv[i] = xx + fX1;
      yv[i] = yy + fY1;
   }
   if (gPad) gPad->Modified();
}


//______________________________________________________________________________
Int_t TCurlyLine::DistancetoPrimitive(Int_t px, Int_t py)
{
   // Compute distance from point px,py to a line.

   return DistancetoLine(px,py,fX1,fY1,fX2,fY2);
}


//______________________________________________________________________________
void TCurlyLine::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
   // Execute action corresponding to one event.
   //
   //  This member function is called when a  TCurlyLine is clicked with the locator
   //
   //  If Left button clicked on one of the line end points, this point
   //     follows the cursor until button is released.
   //
   //  if Middle button clicked, the line is moved parallel to itself
   //     until the button is released.
   //

   Int_t kMaxDiff = 20;
   static Int_t d1,d2,px1,px2,py1,py2;
   static Int_t pxold, pyold, px1old, py1old, px2old, py2old;
   static Bool_t p1, p2, pL;
   Int_t dx, dy;

   Bool_t opaque  = gPad->OpaqueMoving();

   switch (event) {

   case kArrowKeyPress:
   case kButton1Down:
      if (!opaque) {
         gVirtualX->SetLineColor(-1);
         TAttLine::Modify();  //Change line attributes only if necessary
      }

      // No break !!!

   case kMouseMotion:

      px1 = gPad->XtoAbsPixel(fX1);
      py1 = gPad->YtoAbsPixel(fY1);
      px2 = gPad->XtoAbsPixel(fX2);
      py2 = gPad->YtoAbsPixel(fY2);

      p1 = p2 = pL = kFALSE;

      d1  = TMath::Abs(px1 - px) + TMath::Abs(py1-py); //simply take sum of pixels differences
      if (d1 < kMaxDiff) { //*-*================>OK take point number 1
         px1old = px1; py1old = py1;
         p1 = kTRUE;
         gPad->SetCursor(kPointer);
         return;
      }
      d2  = TMath::Abs(px2 - px) + TMath::Abs(py2-py); //simply take sum of pixels differences
      if (d2 < kMaxDiff) { //*-*================>OK take point number 2
         px2old = px2; py2old = py2;
         p2 = kTRUE;
         gPad->SetCursor(kPointer);
         return;
      }

      pL = kTRUE;
      pxold = px; pyold = py;
      gPad->SetCursor(kMove);

      break;

   case kArrowKeyRelease:
   case kButton1Motion:

      if (p1) {
         if (!opaque) {
            gVirtualX->DrawLine(px1old, py1old, px2, py2);
            gVirtualX->DrawLine(px, py, px2, py2);
         }
         else this->SetStartPoint(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py));
         px1old = px;
         py1old = py;
      }
      if (p2) {
         if (!opaque) {
            gVirtualX->DrawLine(px1, py1, px2old, py2old);
            gVirtualX->DrawLine(px1, py1, px, py);
         }
         else this->SetEndPoint(gPad->AbsPixeltoX(px), gPad->AbsPixeltoY(py));
         px2old = px;
         py2old = py;
      }
      if (pL) {
         if (!opaque) gVirtualX->DrawLine(px1, py1, px2, py2);
         dx = px-pxold;  dy = py-pyold;
         px1 += dx; py1 += dy; px2 += dx; py2 += dy;
         if (!opaque) gVirtualX->DrawLine(px1, py1, px2, py2);
         pxold = px;
         pyold = py;
         if (opaque) {
            this->SetStartPoint(gPad->AbsPixeltoX(px1),gPad->AbsPixeltoY(py1));
            this->SetEndPoint(gPad->AbsPixeltoX(px2), gPad->AbsPixeltoY(py2));
         }
      }

      if (opaque) {
         if (p1) {
            //check in which corner the BBox is eddited
            if (fX1>fX2) {
               if (fY1>fY2)
                  gPad->ShowGuidelines(this, event, '2', true);
               else
                  gPad->ShowGuidelines(this, event, '3', true);
            }
            else {
               if (fY1>fY2)
                  gPad->ShowGuidelines(this, event, '1', true);
               else
                  gPad->ShowGuidelines(this, event, '4', true);
            }
         }
         if (p2) {
            //check in which corner the BBox is eddited
            if (fX1>fX2) {
               if (fY1>fY2)
                  gPad->ShowGuidelines(this, event, '4', true);
               else
                  gPad->ShowGuidelines(this, event, '1', true);
            }
            else {
               if (fY1>fY2)
                  gPad->ShowGuidelines(this, event, '3', true);
               else
                  gPad->ShowGuidelines(this, event, '2', true);
            }
         }
         if (pL) {
            gPad->ShowGuidelines(this, event, 'i', true);
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      break;

   case kButton1Up:

      if (opaque) {
         gPad->ShowGuidelines(this, event);
      } else {   
         if (p1) {
            fX1 = gPad->AbsPixeltoX(px);
            fY1 = gPad->AbsPixeltoY(py);
         }
         if (p2) {
            fX2 = gPad->AbsPixeltoX(px);
            fY2 = gPad->AbsPixeltoY(py);
         }
         if (pL) {
            fX1 = gPad->AbsPixeltoX(px1);
            fY1 = gPad->AbsPixeltoY(py1);
            fX2 = gPad->AbsPixeltoX(px2);
            fY2 = gPad->AbsPixeltoY(py2);
         }
      }
      Build();
      gPad->Modified();
      if (!opaque) gVirtualX->SetLineColor(-1);
   }
}


//______________________________________________________________________________
void TCurlyLine::SavePrimitive(ostream &out, Option_t * /*= ""*/)
{
   // Save primitive as a C++ statement(s) on output stream out

   if (gROOT->ClassSaved(TCurlyLine::Class())) {
      out<<"   ";
   } else {
      out<<"   TCurlyLine *";
   }
   out<<"curlyline = new TCurlyLine("
     <<fX1<<","<<fY1<<","<<fX2<<","<<fY2<<","
     <<fWaveLength<<","<<fAmplitude<<");"<<endl;
   if (!fIsCurly) {
      out<<"   curlyline->SetWavy();"<<endl;
   }
   SaveLineAttributes(out,"curlyline",1,1,1);
   out<<"   curlyline->Draw();"<<endl;
}


//______________________________________________________________________________
void TCurlyLine::SetCurly()
{
   // Set curly.

   fIsCurly = kTRUE;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetWavy()
{
   // Set wavy.

   fIsCurly = kFALSE;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetWaveLength(Double_t x)
{
   // Set wave length.

   fWaveLength = x;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetAmplitude(Double_t x)
{
   // Set amplitude.

   fAmplitude = x;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetStartPoint(Double_t x, Double_t y)
{
   // Set start point.

   fX1 = x;
   fY1 = y;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetEndPoint(Double_t x, Double_t y)
{
   // Set edn point.

   fX2 = x;
   fY2 = y;
   Build();
}


//______________________________________________________________________________
void TCurlyLine::SetDefaultWaveLength(Double_t WaveLength)
{
   // Set default wave length.

   fgDefaultWaveLength = WaveLength;
}


//______________________________________________________________________________
void TCurlyLine::SetDefaultAmplitude(Double_t Amplitude)
{
   // Set default amplitude.

   fgDefaultAmplitude  = Amplitude;
}


//______________________________________________________________________________
void TCurlyLine::SetDefaultIsCurly(Bool_t IsCurly)
{
   // Set defaul "IsCurly".

   fgDefaultIsCurly    = IsCurly;
}


//______________________________________________________________________________
Double_t TCurlyLine::GetDefaultWaveLength()
{
   // Get default wave length.

   return fgDefaultWaveLength;
}


//______________________________________________________________________________
Double_t TCurlyLine::GetDefaultAmplitude()
{
   // Get default amplitude.

   return fgDefaultAmplitude;
}


//______________________________________________________________________________
Bool_t TCurlyLine::GetDefaultIsCurly()
{
   // Get default "IsCurly".

   return fgDefaultIsCurly;
}

//______________________________________________________________________________
Rectangle_t TCurlyLine::GetBBox()
{
   // Return the bounding Box of the CurlyLine

   Rectangle_t BBox;
   Int_t px1, py1, px2, py2;
   px1 = gPad->XtoPixel(fX1);
   px2 = gPad->XtoPixel(fX2);
   py1 = gPad->YtoPixel(fY1);
   py2 = gPad->YtoPixel(fY2);
   
   Int_t tmp;
   if (px1>px2) { tmp = px1; px1 = px2; px2 = tmp;}
   if (py1>py2) { tmp = py1; py1 = py2; py2 = tmp;}
   
   BBox.fX = px1;
   BBox.fY = py1;
   BBox.fWidth = px2-px1; 
   BBox.fHeight = py2-py1;

   return (BBox);
}

//______________________________________________________________________________
TPoint TCurlyLine::GetBBoxCenter()
{
   // Return the center of the BoundingBox as TPoint in pixels

   TPoint p;
   p.SetX(gPad->XtoPixel(TMath::Min(fX1,fX2)+0.5*(TMath::Max(fX1, fX2)-TMath::Min(fX1, fX2))));
   p.SetY(gPad->YtoPixel(TMath::Min(fY1,fY2)+0.5*(TMath::Max(fY1, fY2)-TMath::Min(fY1, fY2))));
   return(p);
}

//______________________________________________________________________________
void TCurlyLine::SetBBoxCenter(const TPoint &p)
{
   // Set center of the BoundingBox

   Double_t w = TMath::Max(fX1, fX2)-TMath::Min(fX1, fX2);
   Double_t h = TMath::Max(fY1, fY2)-TMath::Min(fY1, fY2);
   Double_t x1, x2, y1, y2;
   x1 = x2 = y1 = y2 = 0;

   if (fX2>fX1) {
      x1 = gPad->PixeltoX(p.GetX())-0.5*w;
      x2 = gPad->PixeltoX(p.GetX())+0.5*w;
   }
   else {
      x2 = gPad->PixeltoX(p.GetX())-0.5*w;
      x1 = gPad->PixeltoX(p.GetX())+0.5*w;
   }
   if (fY2>fY1) {
      y1 = gPad->PixeltoY(p.GetY()-gPad->VtoPixel(0))-0.5*h;
      y2 = gPad->PixeltoY(p.GetY()-gPad->VtoPixel(0))+0.5*h;
   }
   else {
      y2 = gPad->PixeltoY(p.GetY()-gPad->VtoPixel(0))-0.5*h;
      y1 = gPad->PixeltoY(p.GetY()-gPad->VtoPixel(0))+0.5*h;
   }
   this->SetStartPoint(x1, y1);
   this->SetEndPoint(x2, y2);
}

//______________________________________________________________________________
void TCurlyLine::SetBBoxCenterX(const Int_t x)
{
   // Set X coordinate of the center of the BoundingBox

   Double_t w = TMath::Max(fX1, fX2)-TMath::Min(fX1, fX2);
   if (fX2>fX1) {
      this->SetStartPoint(gPad->PixeltoX(x)-0.5*w, fY1);
      this->SetEndPoint(gPad->PixeltoX(x)+0.5*w, fY2);
   }
   else {
      this->SetEndPoint(gPad->PixeltoX(x)-0.5*w, fY2);
      this->SetStartPoint(gPad->PixeltoX(x)+0.5*w, fY1);
   }
}

//______________________________________________________________________________
void TCurlyLine::SetBBoxCenterY(const Int_t y)
{
   // Set Y coordinate of the center of the BoundingBox

   Double_t h = TMath::Max(fY1, fY2)-TMath::Min(fY1, fY2);
   if (fY2>fY1) {
      this->SetStartPoint(fX1, gPad->PixeltoY(y-gPad->VtoPixel(0))-0.5*h);
      this->SetEndPoint(fX2, gPad->PixeltoY(y-gPad->VtoPixel(0))+0.5*h);
   }
   else {
      this->SetEndPoint(fX2, gPad->PixeltoY(y-gPad->VtoPixel(0))-0.5*h);
      this->SetStartPoint(fX1, gPad->PixeltoY(y-gPad->VtoPixel(0))+0.5*h);
   }
}

//______________________________________________________________________________
void TCurlyLine::SetBBoxX1(const Int_t x)
{
   // Set lefthandside of BoundingBox to a value
   // (resize in x direction on left)

   if (fX2>fX1)
      this->SetStartPoint(gPad->PixeltoX(x), fY1);
   else
      this->SetEndPoint(gPad->PixeltoX(x), fY2);
}

//______________________________________________________________________________
void TCurlyLine::SetBBoxX2(const Int_t x)
{
   // Set righthandside of BoundingBox to a value
   // (resize in x direction on right)

   if (fX2>fX1)
      this->SetEndPoint(gPad->PixeltoX(x), fY2);
   else
      this->SetStartPoint(gPad->PixeltoX(x), fY1);
}

//_______________________________________________________________________________
void TCurlyLine::SetBBoxY1(const Int_t y)
{
   // Set top of BoundingBox to a value (resize in y direction on top)

   if (fY2>fY1)
      this->SetEndPoint(fX2, gPad->PixeltoY(y - gPad->VtoPixel(0)));
   else
      this->SetStartPoint(fX1, gPad->PixeltoY(y - gPad->VtoPixel(0)));
}

//_______________________________________________________________________________
void TCurlyLine::SetBBoxY2(const Int_t y)
{
   // Set bottom of BoundingBox to a value
   // (resize in y direction on bottom)

   if (fY2>fY1)
      this->SetStartPoint(fX1, gPad->PixeltoY(y - gPad->VtoPixel(0)));
   else
      this->SetEndPoint(fX2, gPad->PixeltoY(y - gPad->VtoPixel(0)));
}
