#pragma once
#include "TESForm.h"
#include "skse/Utilities.h"

namespace RE {
   class BGSEquipSlot : public TESForm { // sizeof == 0x24
      public:
         enum { kTypeID = kFormType_EquipSlot };

         tArray<BGSEquipSlot*> parentSlots;
         UInt32 unk20;

         MEMBER_FN_PREFIX(BGSEquipSlot);
         DEFINE_MEMBER_FN(IsSelfOrAncestor, bool, 0x0054C780, BGSEquipSlot*);
         DEFINE_MEMBER_FN(IsSelfOrRelated,  bool, 0x0054C7C0, BGSEquipSlot*);
   };
};