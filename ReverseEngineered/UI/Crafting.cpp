#include "Crafting.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectARMO.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectWEAP.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Systems/Inventory.h"
#include "ReverseEngineered/ExtraData.h"

namespace RE {
   namespace CraftingSubMenus {
      #pragma region AlchemyMenu
      void AlchemyMenu::ImportIngredientsFrom(TESObjectREFR* container, bool merge) {
         auto count = CALL_MEMBER_FN(container, CountItemTypes)(false, true);
         for (uint32_t index = 0; index < count; ++index) {
            auto entry = CALL_MEMBER_FN(container, GetInventoryEntryAt)(index, false); // returns a shallow copy
            if (!entry)
               continue;
            if (entry && entry->type->formType == 0x1E && !CALL_MEMBER_FN(entry, IsQuestObject)()) {
               RE::CraftingSubMenus::AlchemyMenu::EntryData data;
               data.ingredient = entry;
               //
               bool found;
               auto eax = this->GetIndexOfIngredientName(data, found);
               if (eax != -1) {
                  if (merge && found) {
                     auto* other = this->availableIngredients[eax].ingredient;
                     other->Merge(*entry);
                  } else {
                     CALL_MEMBER_FN(&this->availableIngredients, InsertAt)(eax, data);
                  }
               }
               //
               // Upon a successful insertion, (InsertAt) should take the (ingredient) pointer from (data), so 
               // I don't know why Bethesda even has this check (and the destroy code) except maybe as 
               // boilerplate. That works out for purposes, though, since we want to be able to merge duplicate 
               // entries' extra data -- and leave the duplicates to be destroyed.
               //
               if (!data.ingredient)
                  continue;
               entry = data.ingredient;
            }
            CALL_MEMBER_FN(entry, AbandonExtraData)();
            //
            // Vanilla code doesn't actually call the InventoryEntryData destructor, which seems intentional. 
            // I believe we're working with shallow-copy InventoryEntryData instances here, whose extend data 
            // lists may consist of pointers to "real" entries buried in the player's inventory. That seems 
            // pretty reckless on Bethesda's part (I suspect a deep copy would be safer), but it is what it 
            // is, I guess.
            //
            FormHeap_Free(entry);
            entry = nullptr;
         }
      }
      int32_t AlchemyMenu::GetIndexOfIngredientName(EntryData& ed, bool& exists) {
         DEFINE_SUBROUTINE(int32_t, f, 0x0084C710, decltype(availableIngredients)&, EntryData&, uint32_t compareFunctor, bool& found);
         //
         // The subroutine searches a sorted array using the compareFunctor, to find the 
         // index at which you should insert (ed). The (found) bool indicates whether the 
         // sorted array contained an element that matched (ed) based on hte comparator.
         //
         // The comparator we use here compares the names of two forms; evidently the 
         // array is sorted by ingredient name.
         //
         return f(this->availableIngredients, ed, 0x0084D770, exists);
      }
      #pragma endregion

      #pragma region EnchantConstructMenu
      /*static*/ CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry* CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry::make(uint32_t unk08, InventoryEntryData* item) {
         auto memory = (ItemChangeEntry*) FormHeap_Allocate(sizeof(ItemChangeEntry));
         CALL_MEMBER_FN(memory, Constructor)(unk08, item);
         return memory;
      }
      /*static*/ bool CraftingSubMenus::EnchantConstructMenu::IsItemValidForUse(InventoryEntryData* entry) {
         auto form = entry->type;
         if (!form || !form->IsPlayable() || !RE::GetFormName(form)) // is playable?
            return false;
         if (CALL_MEMBER_FN(entry, IsQuestObject)()) // cannot use quest items
            return false;
         return true;
      }
      CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry* CraftingSubMenus::EnchantConstructMenu::GetItemEntry(TESForm* itemBase) {
         for (uint32_t i = 0; i < this->inventory.count; ++i) {
            auto* existing = this->inventory[i];
            auto* entry    = existing->item;
            if (entry->type == itemBase)
               return existing;
         }
         return nullptr;
      }
      void CraftingSubMenus::EnchantConstructMenu::ImportSoulGemsFrom(TESObjectREFR* container, bool merge) {
         auto count = CALL_MEMBER_FN(container, CountItemTypes)(false, true);
         for (uint32_t i = 0; i < count; ++i) {
            auto entry = CALL_MEMBER_FN(container, GetInventoryEntryAt)(i, false); // returns a shallow copy
            if (!entry)
               continue;
            if (IsItemValidForUse(entry)) {
               auto form = entry->type;
               if (form->formType == form_type::soul_gem && CALL_MEMBER_FN(entry, GetSoulSize)() > 0) {
                  ItemChangeEntry* existing = nullptr;
                  if (merge && (existing = this->GetItemEntry(form))) {
                     existing->item->Merge(*entry);
                  } else {
                     auto item  = RE::CraftingSubMenus::EnchantConstructMenu::ItemChangeEntry::make(0x40, entry);
                     auto index = CALL_MEMBER_FN(&this->inventory, Append)(item);
                     if (index != -1) {
                        item->unk0D = true;
                        continue;
                     }
                  }
               }
            }
            CALL_MEMBER_FN(entry, AbandonExtraData)();
            FormHeap_Free(entry);
         }
      }
      #pragma endregion

      #pragma region SmithingMenu
      BGSConstructibleObject* SmithingMenu::get_cobj_for(TESForm* item) const {
         auto result = this->get_cobj_for(item->formID);
         if (!result) {
            if (item->formType == form_type::weapon)
               item = ((RE::TESObjectWEAP*)item)->templateForm;
            else if (item->formType == form_type::armor)
               item = (RE::TESForm*) ((RE::TESObjectARMO*)item)->templateArmor;
            else
               return nullptr;
            if (item)
               result = this->get_cobj_for(item->formID);
         }
         return result;
      }
      #pragma endregion
   }
}