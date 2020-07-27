#include "Crafting.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/ExtraData.h"

namespace RE {
   namespace CraftingSubMenus {
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
                     other->countDelta += entry->countDelta;
                     auto* eList = entry->extendDataList;
                     auto* oList = other->extendDataList;
                     if (eList && eList->items.data && !eList->items.next) {
                        //
                        // InventoryEntryData does not retain BaseExtraLists that consist solely of ExtraCount. 
                        // I don't know if it requires its BaseExtraLists to have an ExtraCount if they cover 
                        // the entire stack, e.g. if you have a stack of 5 Nightshade with the same owner, I 
                        // don't know if that stack is required to have an ExtraCount of 5 in its list. So, if 
                        // (entry) consists of a single BaseExtraList, we're going to require that it have a 
                        // count before we migrate it over.
                        //
                        auto source = (RE::BaseExtraList*)eList->items.data;
                        if (!CALL_MEMBER_FN(source, HasType)(kExtraData_Count))
                           CALL_MEMBER_FN(source, SetExtraCount)(entry->countDelta);
                     }
                     //
                     if (eList && eList->items.data) {
                        bool sever = true;
                        if (!oList) {
                           other->extendDataList = eList;
                        } else {
                           //
                           // Both (oList) and (eList) just contain pointers to the original BaseExtraList 
                           // instances in the container's inventory, since (entry) and (other) are both 
                           // shallow copies of the original InventoryEntryData instances. Moreover, the 
                           // array insertion for AlchemyMenu::availableIngredients will copy the inserted 
                           // InventoryEntryData instance yet again, with us destroying the original. That 
                           // allows us to do a neat trick:
                           //
                           // Instead of individually inserting each (eList) element onto (oList) one at a 
                           // time, we can just have (oList) "steal" (eList) entirely.
                           //
                           auto* last = &oList->items;
                           auto* node = &eList->items;
                           while (last->next)
                              last = last->next;
                           if (!last->data) {
                              //
                              // It's possible for oList to be an empty list. If that's the case, then simply 
                              // appending eList onto oList will result in a list that has an empty node in 
                              // the middle. That's a problem because somewhere in Bethesda's code for the 
                              // InventoryEntryData::GenerateName function, they do this:
                              //
                              //    if (node->next || node->data)
                              //       node->data->YepImSureThisIsSafe();
                              //
                              // Brilliant.
                              //
                              last->data = node->data;
                              last->next = node->next;
                              node->next = nullptr;
                              sever = false; // we just severed the list here, save for one "dead" node that needs to be left behind so it can be deleted
                           } else {
                              last->next = node;
                           }
                        }
                        if (sever)
                           entry->extendDataList = nullptr;
                     }
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
   }
}