#ifndef ROOT_THEDITOR_TREE_DEPENDENCIES
#define ROOT_THEDITOR_TREE_DEPENDENCIES

#ifdef GED_NO_TREE_DEPENDENCY

  class TSelectorDraw
  {
  public:
    void TakeAction() {}
  };

  class TTreePlayer
  {
  public:
      void* GetHistogram() {return nullptr;}
      TSelectorDraw* GetSelector() {return nullptr;}
      Int_t GetNfill() const {return 0;}
  };
 
#else
  #include "TTreePlayer.h"
  #include "TSelectorDraw.h"
#endif

namespace
{
    inline TTreePlayer* GetCurrentTreePlayer()
    {
        #ifdef GED_NO_TREE_DEPENDENCY
            return nullptr;
        #else
            return (TTreePlayer*)TVirtualTreePlayer::GetCurrentPlayer();
        #endif
    }
}

#endif
