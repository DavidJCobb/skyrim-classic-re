#pragma once
#include "skse/GameFormComponents.h"
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Types.h"

namespace RE {
   class InventoryChanges;
   class TESForm;

   class TESContainer : public BaseFormComponent { // sizeof == 0xC
      public:
         struct Entry {
            struct Data {
               UInt32 unk00;
               UInt32 unk04;
               float  unk08;
            };
            //
            UInt32   count;
            TESForm* form;
            Data*    data; // extra data?
         };

         Entry** entries;    // 04
         UInt32  numEntries; // 08

         MEMBER_FN_PREFIX(TESContainer);
         DEFINE_MEMBER_FN(Contains, bool, 0x0044F220, TESForm* item);

         using for_each_functor_t = BOOL(*)(TESForm*, uint32_t count); // return any value other than 1 to stop iterating
         DEFINE_MEMBER_FN(ForEach, void, 0x00491200, for_each_functor_t);

         struct has_all_items_t {
            bool    result = true;
            uint8_t pad01[3];
            InventoryChanges* subject = nullptr;
         };
         DEFINE_MEMBER_FN(InventoryHasAllOfMyItems, bool, 0x004915B0, has_all_items_t&); // does not modify the state object if the container is empty

         struct get_craft_requirements_t {
            struct info {
               const char* name     = nullptr; // 00
               uint32_t    count    = 0;       // 04
               uint32_t    required = 0;       // 08
            };
            BSTArray<info>    results; // 00
            InventoryChanges* source;  // 0C
            //
            MEMBER_FN_PREFIX(get_craft_requirements_t);
            DEFINE_MEMBER_FN(Visit, BOOL, 0x0084EF40, TESForm*, uint32_t countRequired);
         };
         DEFINE_MEMBER_FN(GetCraftRequirementsForDisplay, void, 0x00850080, get_craft_requirements_t&);
   };
   namespace TESContainerForEachFunctors {
      static DEFINE_SUBROUTINE(BOOL, RemoveAllOfMyItemsFromPlayer, 0x00491100, TESForm*, uint32_t);
   }
}
