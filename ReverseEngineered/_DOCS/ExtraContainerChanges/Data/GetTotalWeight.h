float ExtraContainerChanges::Data::GetTotalWeight() {
   auto esi   = this;
   auto esp0C = this;
   if (-1 == this->totalWeight) {
      auto   ecx = this->owner;
      Actor* esp24;
      if (ecx) {
         esp24 = ecx->formType == 0x3E ? ecx : nullptr;
      } else
         esp24 = nullptr;
      TESContainer* eax;
      if (ecx)
         eax = ecx->GetBaseContainer();
      else
         eax = nullptr;
      UInt32 esp20 = eax->numEntries;
      auto   ebx   = eax->entries;
      if (ecx > 0) {
         do {
            TESContainer::Entry* ecx = ebx[edi];
            auto ebp = ecx->form;
            InventoryEntryData* esi = this->sub00477B20(ebp, 1, 0);
            if (esi) {
               if (esi->IsQuestObject())
                  continue;
            }
            float esp14 = GetFormWeight(ebp);
            if (-1.0F == esp14)
               esp14 = 0.0F;
            if (!esi) {
               esp10 += ebx[edi]->count;
            } else {
               edx = ebx[edi]->count;
               esp28 = eax = esi->countDelta + edx; // at 0x0047B6CE
               if (eax) {
                  esp10 += esp14;
               }
            }
            esi = this;
            ++edi;
         } while (edi < esp20);
      }
      esp20 = 0;
      esp1C = ebx = this->objList;
      if (ebx) {
         do {
            ebx = esp1C;
            edi = ebx->unk00;
            if (!edi)
               break;
            ebp = edi->unk00;
            if (!ebp)
               continue;
            if (ecx = esi->unk04) { // at 0x0047B72D
               // ...
            }
            esi = edi->extendDataList;
            bl = 0;
            if (esi) { // at 0x0047B75F
               // ...
            }
            esp14 = GetFormWeight(ebp);
            if (0.0F ?? esp14)
               continue;
            ebx = esp24; // the Actor* from above
            esi = edi->countDelta;
            esp28 = esi;
            if (ebx && ebp->formType == 0x1A) { // at 0x0047B7BD
               if (edi->IsWorn()) {
                  if (esi <= 0)
                     continue;
                  ecx = edi->type;
                  float esp14;
                  CalculatePerkData(kEntryPoint_Mod_Armor_Weight, ebx, ecx, &esp14); // at 0x0047B7D9
                  --esi;
                  esp28 = esi;
                  esp20 += esp14;
               }
            }
            if (esi > 0) {
               esp10 += esp28 * esp14; // count * form weight
            }
            
            
         } while (
      }
      esp04 += esp14;
      this->totalWeight = esp04;
      if (ecx) {
         ecx->OnActorValueChanged(0x1F, esi->armorWeight, esp04 - this->armorWeight, 0);
      }
   }
   return this->totalWeight;
}