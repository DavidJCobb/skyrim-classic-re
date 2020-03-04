#pragma once
#include "TESForm.h"
#include "skse/GameForms.h"

namespace RE {
   class BGSMessage : public TESForm { // sizeof == 0x40
      public:
         enum { kTypeID = kFormType_Message };

         struct Button {
            BSFixedString text; // 00
            UInt32 conditions; // 04 // type unknown
         };

         // parents
         TESFullName		fullName;		// 14
         TESDescription	description;	// 1C

          // members
         TESForm* icon;		// 28 - init'd to 0
         TESQuest* quest;	// 2C - init'd to 0
         tList<Button>	buttons;	// 30
         UInt32					unk38;		// 38 - init'd to 1
         UInt32					unk3C;		// 3C - init'd to 2

         std::string GetDescription() {
            BSString str;
            CALL_MEMBER_FN(&description, Get)(&str, (::TESForm*)this, 'DESC');
            return str.Get();
         };
   };
}