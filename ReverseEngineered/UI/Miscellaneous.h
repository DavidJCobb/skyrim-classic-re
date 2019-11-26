#pragma once
#include "ReverseEngineered/Shared.h"

namespace RE {
   DEFINE_SUBROUTINE_EXTERN(void, ShowNotification, 0x008997A0, const char* message, const char* soundEditorID, bool one);
};

/*

   NOTES:

   BarterMenu::unk1C is most likely an instance of struct InventoryData whose unk28 is a BStArray of StandardItemData.
   
   ---

   void sub00843AD0(InventoryData*, ExtraContainerChanges::Data*, void*, BarterParams&)

   This function most likely loops over the barter-target's items somehow and stores information somewhere. I can't
   ID the type of the third parameter, but the fourth is:

   struct BarterParams {
      TESObjectREFR* barterTarget; // 00 // actually a smart pointer
      bool unk04; // 04
      bool bypassVendorStolenCheck;  // 05 // player actor value BypassVendorStolenCheck != 0?
      bool bypassVendorKeywordCheck; // 06 // player actor value BypassVendorKeywordCheck != 0?

      MEMBER_FN_PREFIX(BarterParams);
      DEFINE_MEMBER_FN(Constructor, BarterParams&, 0x00842D80, UInt32 refHandle);
      DEFINE_MEMBER_FN(TestItem,    bool,          0x00842830, void* item);
   }

   the subroutine at 0x00843AD0 eventually calls member function 0x008433B0 on its first argument, passing an inventory 
   item and a pointer to other structure (possibly an out-variable); that member function creates a StandardItemData 
   instance (I think; SKSE hooked the function to hell and back and I don't know what those hooks are offhand) and stores 
   it in a tArray at offset 0x28 on this.

*/