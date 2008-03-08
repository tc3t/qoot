// @(#)root/eve:$Id$
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveProjectionBases.h"
#include "TEveManager.h"

#include <cassert>

//==============================================================================
//==============================================================================
// TEveProjectable
//==============================================================================

//______________________________________________________________________________
//
// Abstract base-class for non-linear projectable objects.
//
// Via ProjectedClass() method it returns a TClass instance for the
// projected class and keeps references to the projected objects.
//
// See also TEveProjectionManager::ImportElements().

ClassImp(TEveProjectable);

//______________________________________________________________________________
TEveProjectable::TEveProjectable()
{
   // Constructor.
}

//______________________________________________________________________________
TEveProjectable::~TEveProjectable()
{
   // Destructor.
   // Force projected replicas to unreference *this, then destroy them.

   while ( ! fProjectedList.empty())
   {
      TEveProjected* p = fProjectedList.front();
      p->UnRefProjectable(this);
      TEveElement* el = dynamic_cast<TEveElement*>(p);
      if (el)
      {
         gEve->PreDeleteElement(el);
         delete el;
      }
   }
}

//______________________________________________________________________________
void TEveProjectable::AddProjectedsToSet(std::set<TEveElement*>& set)
{
   // Add the projected elements to the set, dyn-casting them to
   // TEveElement.

   for (ProjList_i i=fProjectedList.begin(); i!=fProjectedList.end(); ++i)
   {
      set.insert(dynamic_cast<TEveElement*>(*i));
   }
}


//==============================================================================
//==============================================================================
// TEveProjected
//==============================================================================

//______________________________________________________________________________
//
// Abstract base class for classes that hold results of a non-linear
// projection transformation.
//

ClassImp(TEveProjected);

//______________________________________________________________________________
TEveProjected::TEveProjected() :
   fManager     (0),
   fProjectable (0),
   fDepth       (0)
{
   // Constructor.
}

//______________________________________________________________________________
TEveProjected::~TEveProjected()
{
   // Destructor.
   // If fProjectable is non-null, *this is removed from its list of
   // projected replicas.

   if (fProjectable) fProjectable->RemoveProjected(this);
}

//______________________________________________________________________________
void TEveProjected::SetProjection(TEveProjectionManager* mng, TEveProjectable* model)
{
   // Sets projection manager and reference in the projectable object. Method called
   // immediately after default constructor.
   // See also TEveProjectionManager::ImportElements().

   fManager   = mng;
   if (fProjectable) fProjectable->RemoveProjected(this);
   fProjectable = model;
   if (fProjectable) fProjectable->AddProjected(this);
}

//______________________________________________________________________________
void TEveProjected::UnRefProjectable(TEveProjectable* assumed_parent)
{
   // Remove reference to projectable.

   static const TEveException eH("TEveProjected::UnRefProjectable ");

   assert(fProjectable == assumed_parent);

   fProjectable->RemoveProjected(this);
   fProjectable = 0;
}

//______________________________________________________________________________
void TEveProjected::SetDepthCommon(Float_t d, TEveElement* el, Float_t* bbox)
{
   // Utility function to update the z-values of the bounding-box.
   // As this is an abstract interface, the element and bbox pointers
   // must be passed from outside.

   Float_t delta = d - fDepth;
   fDepth = d;
   if (bbox) {
      bbox[4] += delta;
      bbox[5] += delta;
      el->StampTransBBox();
   }
}
