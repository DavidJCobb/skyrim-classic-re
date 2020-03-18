bool _nested_loop(tList<BaseExtraList>* list) {
   do {
      auto ecx = list->data;
      if (!ecx)
         return false;
      if (ecx->CheckExtraWorn(0, 1))
         return true;
   } while (list = list->next);
   return false;
}

float ExtraContainerChanges::Data::GetEquippedItemsWeight(TESObjectREFR* subject) {
   auto  ebp    = this->objList;
   float result = 0.0F; // &result == esp + 0x4
   float weight = 0.0F;
   if (!ebp)
      return result;
   do {
      InventoryEntryData* esi = ebp->data;
      if (!esi)
         break;
      auto edi = esi->extendDataList;
      if (!edi)
         continue;
      if (!_nested_loop(edi)) // modifies the value of (edi)
         continue;
      if (esi->type->formType == 0x2A)
         continue;
      weight = GetFormWeight(esi->type);
      edi = 1;
      if (esi->type->formType == 0x29) { // at 0x004761DB
         edi = 0;
         if (esi->GetExtendDataForWorn(1))
            edi = 1;
         if (esi->GetExtendDataForWorn(0))
            ++edi;
      } else if (esi->type->formType == 0x1A) {
         CalculatePerkData(kEntryPoint_Mod_Armor_Weight, subject, eax, &weight);
      }
      result += edi * weight; // &result == esp + 0xC
   } while (ebp = ebp->next);
   return result;
}