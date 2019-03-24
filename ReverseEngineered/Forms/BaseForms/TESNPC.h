#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "skse/GameObjects.h"

namespace RE {
   class InventoryEntryData;
   class TESObjectREFR;
   class TESNPC : public ::TESNPC {
      public:
         enum {
            kFlag_Female = 0x1
         };

         MEMBER_FN_PREFIX(TESNPC);
         DEFINE_MEMBER_FN(PrepForArmorAddonChange, bool, 0x005620A0, TESObjectREFR* target, void**, InventoryEntryData*);
   };
}