float ExtraContainerChanges::Data::GetTotalWeight() {
   auto esi   = this;
   auto esp0C = this;
   if (-1 == this->totalWeight) {
      float total_weight = 0.0F; // esp10
      float item_weight; // esp14
      //
      auto   ecx   = this->owner;
      Actor* owner = nullptr; // esp24
      if (ecx)
         owner = ecx->formType == 0x3E ? ecx : nullptr;
      //
      // First, count the items that are in the base container.
      //
      TESContainer* eax = ecx ? ecx->GetBaseContainer() : nullptr;
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
            float item_weight = GetFormWeight(ebp);
            if (-1.0F == item_weight)
               item_weight = 0.0F;
            if (!esi) {
               total_weight += item_weight * ebx[edi]->count;
            } else {
               edx = ebx[edi]->count;
               esp28 = eax = esi->countDelta + edx; // at 0x0047B6CE
               if (eax) {
                  total_weight += item_weight * esp28;
               }
            }
            esi = this;
            ++edi;
         } while (edi < esp20);
      }
      //
      // Next, count the items that are in the container-changes.
      //
      float worn_armor_weight = 0.0F; // esp20
      esp1C = ebx = this->objList;
      if (esp1C) {
         do {
            InventoryEntryData* edi = esp1C->unk00;
            if (!edi)
               break;
            ebp = edi->type;
            if (!ebp)
               continue;
            if (ecx = this->owner) { // at 0x0047B72D
               //
               // If the item in question exists in the base container, skip it, since we 
               // counted it in the loop above. Note that the loop above handles all items 
               // in the TESContainer but ALSO the countDeltas on any items of the same 
               // type in the container-changes data.
               //
               // This implies that if you spawn with a piece of armor in your inventory 
               // (i.e. the ActorBase's initial items), that piece of armor (and any armor 
               // of that type) will be exempt from the Mod Armor Weight perk applied below. 
               // Testing confirms that this is indeed a problem affecting the Iron Shield, 
               // as that item is applied to the player's inventory via [NPC:00000007] and 
               // removed during the game's intro (the other Iron Armor is applied via a 
               // Default Outfit on the ActorBase and so is not affected by this issue).
               //
               if (auto eax = ecx->GetBaseContainer()) {
                  // and then we redundantly check this->owner and get the container again...
                  bool al = eax->Contains(ebp);
                  if (al)
                     continue;
               }
            }
            esi = edi->extendDataList;
            bool bl = false;
            if (esi) { // at 0x0047B75F
               //
               // Skip this item if it's a quest object.
               //
               if (esi->next || esi->data) {
                  do {
                     if (bl)
                        continue;
                     if (esi->data->IsQuestObject())
                        bl = true;
                  } while (esi = esi->next); // at 0x0047B788
               }
               if (bl)
                  continue;
            }
            item_weight = GetFormWeight(ebp); // esp14
            if (0.0F >= item_weight)
               continue;
            auto ebx   = owner;
            auto count = edi->countDelta; // esp28
            if (ebx && ebp->formType == 0x1A) { // at 0x0047B7BD
               if (edi->IsWorn()) {
                  if (count <= 0)
                     continue;
                  ecx = edi->type;
                  CalculatePerkData(kEntryPoint_Mod_Armor_Weight, ebx, ecx, &item_weight); // at 0x0047B7D9
                  --count;
                  worn_armor_weight += item_weight;
                  //
                  // And here, we have a bug. Bethesda clearly intended for Mod Armor Weight to only 
                  // apply to the one piece of armor that you're wearing. However, we modified the 
                  // same (item_weight) float that will be used outside of this branch, so in effect 
                  // we have changed the weight of all armor of this type. This is what breaks the 
                  // Steed Stone and makes it so that if you have, say, ten Glass Armors and you 
                  // equip one, none of them add to your carry weight anymore.
                  //
                  // The solution would be to patch this branch to use a separate float, which we 
                  // initialize as a copy of (item_weight).
                  //
               }
            }
            if (count > 0) {
               total_weight += count * item_weight; // count * form weight
            }
         } while (esp1C = esp1C->next);
      }
      // esp += 0xC
      total_weight += worn_armor_weight;
      //
      // And now we're done! :)
      //
      this->totalWeight = total_weight;
      if (ecx) {
         ecx->OnActorValueChanged(0x1F, esi->armorWeight, esp04 - this->armorWeight, 0);
      }
   }
   return this->totalWeight;
}