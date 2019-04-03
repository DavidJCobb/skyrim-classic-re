bool Actor::Unk_120(TESForm* item, Arg2, Arg3, Arg4, Arg5) {
   Actor*              ebx = esp28 = this;
   ActorWeightModel*   edi = esp18 = this->GetWeightModel(0);
   TESForm*            ebp = Arg1;
   BGSBipedObjectForm* esi = esp30 = ToBGSBipedObjectForm(Arg1);
   bool esp13 = false;
   auto esp34 = GetOrCreateExtraContainerChangesDataFor(this);
   if (esi) { // at 0x006B60A3
      auto eax = edi->unk00;
      if (eax) { // weird pointer validation
         eax = this->baseForm->TESV_00560F70();
         edi = 0; // loop iterator: body part slot index
         esp20 = eax;
         esp1C = 0;
         do { // at 0x006B60D0
            if (esi->TestBodyPartByIndex(edi)) {
               eax = 0;
               esp18 = 0; // loop var
               do {
                  ecx = esp14;
                  esi = ecx->unk00[esp18].unk00; // guessed
                  esp24 = esi;
                  eax = DYNAMIC_CAST(esi, TESForm, BGSBipedObjectForm);
                  BGSBipedObjectForm* esp28 = eax;
                  if (!eax)
                     continue;
                  TESObjectARMO* ecx = esp20;
                  if (ecx)
                     BGSBipedObjectForm* ecx = &ecx->unkE0;
                  else
                     BGSBipedObjectForm* ecx = nullptr;
                  if (eax == ecx)
                     continue;
                  bool al = eax->TestBodyPartByIndex(edi);
                  if (!al)
                     continue;
                  if (esi->formType == 0xE)
                     continue;
                  eax = this->extraData.GetExtraContainerChangesData();
                  InventoryEntryData* ebp = eax->TESV_00477380(edi, 0, 0, 0); // worn entry data?
                  esi = 0;
                  if (ebp) { // at 0x006B6167
                     eax = ebp->extendDataList;
                     if (eax) {
                        if (eax->data) {
                           BaseExtraList* ecx = eax;
                           if (ecx->HasExtraCannotWear())
                              esp13 = true;
                           eax = ebp->next;
                        }
                     }
                  }
                  if (!esp13) { // at 0x006B618E
                     if (!ebp)
                        continue;
                     bool al = EquipManager::GetInstance()->TESV_006ED8B0(*ebp);
                     if (!al) {
                        EquipManager::GetInstance()->UnequipItem(this, *ebp, esi, 1, nullptr, false, false, true, false, nullptr);
                     }
                     ebx = 0;
                     esi = 0;
                     do {
                        //
                        // ...
                        //
                     } while ((esi += 0x20) < 0x400);
                     ebx = esp2C;
                     edi = esp1C;
                  }
                  if (ebp) {
                     ebp->RedundantlyDestroyList();
                     delete ebp;
                  }
               } while ((esp18 += 0x20) < 0x40));
            }
         } while ((esp1C = ++edi) < 0x20);
         if (esp13)
            return false;
      }
   }
   // at 0x006B626E
   if (auto ecx = this->processManager) {
      //
      // unequip old ammo/projectile if we're switching to another
      //
   }
   // at 0x006B62C5
   //
   // ...
   //
   if (ebp->formType == kFormType_Armor) { // at 0x006B62ED
      bool al = *(bool*)(0x01310584) != false;
      this->Unk_121(al ? 0 : Arg1); // applies armor-addons
      auto eax = TESV_0044FF90(Arg1, esi);
      if (eax)
         eax->TESV_004083C0(); // preload models?
   }
   //
   // ...
   //
}