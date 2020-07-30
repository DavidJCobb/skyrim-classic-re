#pragma once
#include "skse/Utilities.h"
#include "ReverseEngineered/UI/Miscellaneous.h"

class MagicMenu;
namespace RE {
   class SpendSoulCallback : public IMessageBoxCallback { // sizeof == 0xC // This is for DRAGON souls, not soul gems!
      public:
         MagicMenu* owner;
   };

   namespace CraftingSubMenus {
      namespace EnchantConstructMenu {
         class EnchantMenuCallback : public IMessageBoxCallback {
            public:
         };
         class EnchantMenuCraftCallback : public EnchantMenuCallback {
            public:
               void*  unk08; // probably a class for the enchantment menu -- sorry, EnchantConstructMenu
         };
      };
   };
   typedef CraftingSubMenus::EnchantConstructMenu::EnchantMenuCraftCallback ConfirmEnchantItemCallback;

   namespace SkyrimScript { // sizeof == 0x10
      class ScriptMessageBoxCallback : public IMessageBoxCallback { // this is actually in an anonymous namespace nested under SkyrimScript
         public:
            static constexpr uint32_t vtbl = 0x010F0124;
            virtual void Callback(uint8_t bi) override { CALL_MEMBER_FN(this, VanillaCallback)(bi); }
            //
            uint32_t stackID; // 08 // Papyrus script stack ID to resume
            void*    unk0C;   // 0C // unk0C->unk00->unkA8 == VMClassRegistry*; this callback calls VMClassRegistry::ResumeStack(stackID, VMValue(button);
            //
            MEMBER_FN_PREFIX(ScriptMessageBoxCallback);
            DEFINE_MEMBER_FN(VanillaCallback, void, 0x008EB2B0, uint8_t buttonIndex);
            //
            static ScriptMessageBoxCallback* make(uint32_t stackID, decltype(unk0C) unk0C);
      };
   }
};