#include "BGSListForm.h"
#include <algorithm>
#include "ReverseEngineered/ExtraData.h"

namespace RE {
   SInt32 BGSListForm::CountMatchingItemsInInventory(InventoryChanges& inventory) {
      CountMatchingItemsInInventoryVisitor visitor(inventory);
      CALL_MEMBER_FN(this, CountMatchingItemsInInventory)(visitor);
      return visitor.count;
   }
   bool BGSListForm::visit(visitor& v) {
      v.greet(*this);
      //
      // ESP-defined forms:
      //
      for (uint32_t i = 0; i < forms.count; i++) {
         TESForm* entry = nullptr;
         if (forms.GetNthItem(i, entry)) {
            if (v.test(entry))
               return true;
         }
      }
      //
      // Script-added forms:
      //
      if (addedForms) {
         for (uint32_t i = 0; i < addedForms->count; i++) {
            UInt32 formID = 0;
            addedForms->GetNthItem(i, formID);
            auto entry = (RE::TESForm*)LookupFormByID(formID);
            if (v.test(entry))
               return true;
         }
      }
      return false;
   }
   //
   void RecursiveFormListSearch::greet(BGSListForm& list) {
      this->seen.push_back(&list);
   }
   bool RecursiveFormListSearch::test(TESForm* form) {
      if (!form)
         return false;
      if (form == this->desired)
         return true;
      if (form->formType == form_type::formlist) {
         auto nested = (BGSListForm*)form;
         if (std::find(this->seen.begin(), this->seen.end(), nested) != this->seen.end())
            return false;
         this->seen.push_back(nested);
         return nested->visit(*this);
      }
      return false;
   }
}