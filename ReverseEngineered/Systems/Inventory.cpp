#include "Inventory.h"
#include "ReverseEngineered/Types.h"
#include "skse/GameAPI.h"
#include "skse/GameTypes.h"

namespace RE {
   UInt8 InventoryEntryData::GetSoulSize() const {
      auto form = this->type;
      if (!type || type->formType != kFormType_SoulGem)
         return 0;
      UInt8 size = ((TESSoulGem*)type)->soulSize;
      if (size)
         return size;
      if (!this->extendDataList)
         return 0;
      auto extra = &((RE::tList<RE::BaseExtraList>*)this->extendDataList)->items;
      for (; extra; extra = extra->next) {
         auto list = extra->data;
         if (!list)
            continue;
         size = CALL_MEMBER_FN(list, GetExtraSoulSize)();
         if (size)
            return size;
      }
      return 0;
   };
   void InventoryEntryData::Merge(InventoryEntryData& other) { // (other) will have its extra-data lists removed and brought over here
      this->countDelta += other.countDelta;
      auto* tList = this->extendDataList;
      auto* oList = other.extendDataList;
      if (oList && oList->items.data) {
         if (!oList->items.next) {
            //
            // InventoryEntryData does not retain BaseExtraLists that consist solely of ExtraCount. 
            // I don't know if it requires its BaseExtraLists to have an ExtraCount if they cover 
            // the entire stack, e.g. if you have a stack of 5 Nightshade with the same owner, I 
            // don't know if that stack is required to have an ExtraCount of 5 in its list. So, if 
            // (entry) consists of a single BaseExtraList, we're going to require that it have a 
            // count before we migrate it over.
            //
            auto source = (RE::BaseExtraList*)oList->items.data;
            if (!CALL_MEMBER_FN(source, HasType)(kExtraData_Count))
               CALL_MEMBER_FN(source, SetExtraCount)(other.countDelta);
         }
         //
         bool sever = true;
         if (!tList) {
            this->extendDataList = oList;
         } else {
            auto* last = &tList->items;
            auto* node = &oList->items;
            while (last->next)
               last = last->next;
            if (!last->data) {
               //
               // It's possible for tList to be an empty list. If that's the case, then simply 
               // appending oList onto tList will result in a list that has an empty node in 
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
            other.extendDataList = nullptr; // tList and tList::Node are technically the same type
      }
      other.countDelta = 0;
   }
}