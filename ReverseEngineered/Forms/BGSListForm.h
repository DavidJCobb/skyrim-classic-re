#pragma once
#include "TESForm.h"
#include "skse/GameForms.h"
#include "skse/Utilities.h"

namespace RE {
   class InventoryChanges;

   class BGSListForm : public TESForm { // sizeof == 0x28
      public:
         enum { kTypeID = kFormType_List };

         tArray<TESForm*>	forms;	// 14
         tArray<UInt32> *	addedForms;	// 20
         UInt32				unk24;	// 24

         MEMBER_FN_PREFIX(BGSListForm);
         DEFINE_MEMBER_FN(AddFormToList,       void,   0x004FB380, TESForm* form);
         DEFINE_MEMBER_FN(GetInitialFormCount, UInt32, 0x00422F80);
         DEFINE_MEMBER_FN(RemoveFormFromList,  void,   0x004FB4A0, TESForm* form);
         DEFINE_MEMBER_FN(RevertList,          void,   0x004FB2F0);

         using Visitor = ::BGSListForm::Visitor;

         //bool Visit(Visitor& visitor);

         struct CountMatchingItemsInInventoryVisitor {
            uint32_t          count = 0;
            InventoryChanges& inventory;
            //
            CountMatchingItemsInInventoryVisitor(InventoryChanges& ic) : inventory(ic) {}
         };
         DEFINE_MEMBER_FN(CountMatchingItemsInInventory, void, 0x00905F30, CountMatchingItemsInInventoryVisitor&);
         SInt32 CountMatchingItemsInInventory(InventoryChanges&);
   };
}
