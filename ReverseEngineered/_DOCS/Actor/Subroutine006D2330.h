void Actor::Subroutine006D2330(TESForm* Arg1_Item, EquipManager::EquipRequest* Arg2) {
   void* edi = Arg2; // BaseExtraList* unk00; BGSEquipSlit* unk04;
   auto esi = this;
   esp14 = 0;
   esp10 = 0;
   if (Arg2->extraData) {
      esp18 = 0;
      Arg2->extraData->TESV_0040E810(&esp14, &esp18);
      eax = Arg2->extraData->GetExtraReferenceHandle(&esp28);
      NiPointer<TESObjectREFR> esp50;
      TESObjectREFRHandleInterface::GetRefByHandle(eax, &esp50);
      TESObjectREFR* eax = esp50;
      if (eax)
         edx = eax->formID;
   }
   bool esp50 = (Arg2->equipSlot == GetLeftHandSlot()); // at 0x006D23B5
   this->TESV_006A75F0(); // AI flags
   auto ebp = Arg1;
   if (Arg1->formType == 0x2A) { // ammo
      ecx = Arg2->extraData;
      if (!ecx)
         jump to 0x006D2462;
      bool al = ecx->TESV_00418FE0(); // ref handle
      //
      // ...
      //
   } else {
      // at 0x006D2426
      ecx = edi->extraData;
      if (ecx) {
         bool al = ecx->HasExtraCannotWear();
         if (al && edi->unk11) {
            ShowNotification("You cannot equip this item.", ebx, 1);
            return;
         }
      }
      // at 0x006D2462
      auto ecx = GetOrCreateExtraContainerChangesData(esi);
      edx = eax->TESV_0047A4D0(ebp);
      ebx = esp50;
      esp4C = edx;
      if (ebp->formType != 0x2A && ebp->formType != 0x29) {
         ecx = Arg2->extraData;
         if (ecx && ecx->CheckExtraWorn(false, ebx))
            return;
         edx = esp4C;
      }
      // at 0x006D24A2
      if (edx <= 0)
         return;
      if (Arg2->count <= 0 && ebp->formType == 0x2A)
         Arg2->count = edx;
      switch (ebp->formType) {
         case kFormType_Weapon: // at 0x006D24D7 // 29
            if (edi->count > 0) {
               bool al = ebp->TESV_00474CA0();
               if (!al)
                  edi->count = 1;
            }
            // at 0x006D24EF
            ecx = edi->extraData;
            edx = edi->count;
            esi->Unk_120(ebp, edx, ecx, Arg2->preventUnequip, 1, ebx);
            TESV_006C4880(ebp, esi);
            esi->Unk_C9();
            if (!edi->extraData) {
               auto ecx = esi->processManager;
               if (ecx) {
                  InventoryEntryData* eax = ecx->TESV_0071EDD0(ebx);
                  if (eax) {
                     eax = eax->GetExtendDataForWorn(ebx);
                     esi->TESV_006ED980(ebp, eax, ebx);
                     break;
                  }
               }
            }
            eax = Arg2->extraData; // at 0x006D254C
            esi->TESV_006ED980(ebp, eax, ebx);
            break;
         case kFormType_Projectile: // at 0x006D255D // 32
            ecx = edi->extraData;
            edx = edi->count;
            esi->Unk_120(ebp, edx, ecx, Arg2->preventUnequip, 0, 0);
            TESV_006C4880(ebp, esi);
            (*g_thePlayer)->TESV_007337C0(0xFF);
            esi->Unk_C9();
            break;
         case kFormType_Armor: // at 0x006D25A0 // 1A
            esi->Unk_C9();
            bool bl = esi->TESV_006E9030(ebp, Arg2->extraData); // can cast worn enchantment?
            if (bl) {
               if (Arg2->count < 1)
                  Arg2->count = 1;
               edx = edi->extraData;
               eax = edi->count;
               bool al = esi->Unk_120(ebp, eax, edx, Arg2->preventUnequip, 1, ebx);
               if (al) {
                  esi->TESV_006E8650(ebp, Arg2->extraData);
                  esp50 = esi;
                  Unknown012E32E8::GetInstance()->TESV_006BA7E0(&esp50);
                  TESV_006C4880(ebp, esi);
               }
            }
            // at 0x006D260F
            if (esi != *(Actor**)0x01310588)
               jump to 0x006D298D;
            (*g_thePlayer)->TESV_00737BB0();
            if (bl)
               break;
            ShowNotification("You cannot equip this item right now.", 0, 1);
            return;
         case kFormType_Light: // at 0x006D264A // 1F
            auto eax = 0;
            if (esi == *(Actor**)0x01310588) {
               auto ecx = MenuManager::GetInstance();
               if (ecx->unkC8 > 0)
                  eax = 2;
            }
            esi->TESV_006A9BC0(&esp40, "ITMTorchHeldEquip", 0, (eax | 0x10), 1); // at 0x006D267E
            bool al;
            if (Arg2->equipSlot)
               al = esp50;
            else
               al = true;
            esi->Unk_120(ebp, Arg2->count, Arg2->extraData, (uint32_t)Arg2->preventUnequip, 1, al);
            TESV_006C4880(ebp, esi);
            break;
         case kFormType_Ammo: // at 0x006D26BA // 2A
            esi->Unk_120(ebp, Arg2->count, Arg2->extraData, (uint32_t)Arg2->preventUnequip, 0, false);
            TESV_006C4880(ebp, esi);
            break;
         case kFormType_ScrollItem: // at 0x006D26E1 // 17
            auto ecx = Arg2->extraData
            if (ecx) {
               if (ecx->TESV_00418FE0()) {
                  if (esi == *(Actor**)0x01310588) { // at 0x006D26F0
                     eax = "You can not eat quest items.";
                     jump to 0x006D2961;
                  }
               }
            }
            // at 0x006D2706
            esi->Unk_120(ebp, Arg2->count, Arg2->extraData, (uint32_t)Arg2->preventUnequip, 0, ebx);
            // jumped to 0x006D26A3 for part of the last call
            TESV_006C4880(ebp, esi);
            break;
         case kFormType_Ingredient: // at 0x006D2717 // 1E
            ecx = esp10;
            edx = esp14;
            UInt32 esp2C;
            eax = esi->GetOrCreateRefHandle(&esp2C);
            refr_ptr esp18(esp2C);
            auto ev = BSTEventSourceHolder::GetInstance();
            ev->SendEvent270(&esp18, ebp->formID, edx, ecx, 1);
            esp18->~refr_ptr();
            auto ecx = Arg2->extraData;
            if (ecx) {
               bool al = ecx->TESV_00418FE0();
               if (al) {
                  if (esi != *(Actor**)0x01310588)
                     return;
                  ShowNotification("You can not eat quest items.", 0, 1);
                  return;
               }
            }
            // at 0x007D278B
            //
            // ... event 1B0 and some other stuff...
            //
         case kFormType_Potion: // at 0x006D2838 // 2E
            auto ecx = Arg2->extraData;
            if (ecx) {
               bool al = ecx->TESV_00418FE0();
               if (al)
                  jump to 0x006D26F0;
            }
            // at 0x006D284B
            ecx = esi->formID;
            edx = ebp->formID;
            esp24 = ecx;
            ecx = Arg2->extraData;
            eax = 0;
            ebx = 0;
            esp1C = edx;
            esp4C = 0;
            if (ecx) {
               ecx->TESV_0040E810(&esp4C, &esp20);
               eax = ecx->GetExtraReferenceHandle(&esp34);
               NiPointer<TESObjectREFR> esp50;
               esp50.set_from_handle(eax);
               ebx = esp50 ? esp50->formID : 0;
               esp50.~NiPointer();
            }
            // at 0x006D28A8
            auto ev = BSTEventSourceHolder::GetInstance();
            ev->SendEvent1B0(esp24, 0, esp1C, 1, ebx, esp4C);
            bool al = ebp->Unk_62();
            if (al) {
               if (esi != *(Actor**)0x01310588)
                  jump to 0x006D298D;
               (*g_thePlayer)->TESV_00746230(ebp);
               return;
            }
            // at 0x006D28FB
            //
            // ... send an event, and more stuff ...
            //
         cases 9: // at 0x006D2954 // 18, 19, 1C, 1D, 20, 21, 22, 23, 24, 25, 26, 27, 28, 2B, 2C, 2D, 2F, 30, 31
            if (esi != *(Actor**)0x01310588)
               break;
            ShowNotification("You cannot equip this item.", 0, 1);
            if (esi != *(Actor**)0x01310588)
               break;
         case kFormType_Book: // at 0x006D296E // 1B // BREAK
            break;
      }
      // at 0x006D296E
      if (Arg2->showMessage) {
         esi->Unk_A2(ebp, 1, 0);
      }
      ebp = ebp->formID;
      //
      // ... send event 270 ...
      //
      eax = esp50;
      if (!eax)
         return;
   }
   // at 0x006D29FD
   eax->handleRefObject.DecRefHandle(); // esp50->~NiPointer();
}