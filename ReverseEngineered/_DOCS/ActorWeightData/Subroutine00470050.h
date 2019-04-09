void ActorWeightData::Subroutine00470050(float Arg1, UInt32 Arg2) {
   auto edi = this;
   auto ebp = 0;
   auto esp10 = this;
   auto esp14 = 0;
   NiPointer<TESObjectREFR> esp14 = nullptr;
   TESObjectREFRHandleInterface::ExchangeHandleForRef(&this->unkA88, &esp14);
   TESObjectREFR* esi = esp14;
   if (!esi)
      return;
   if (this->unk04) { // at 0x00470088
      esp34 = esi->GetShieldBodyPartIndex();
      auto edx = GMST:fBodyMorphWeaponAdjustMult;
      float esp30 = esi->GetFormWeight() / 100.0F * Arg1;
      InterlockedIncrement(&this->refcount);
      ecx = 0;
      esp34 = this;
      bool esp2C = false;
      if (esi == *g_thePlayer) {
         bool al = (*g_thePlayer)->TESV_00736FC0(&esp38);
         if (al) {
            bool esp2C = true;
            if ((*g_thePlayer)->unk727 & 2) {
               this->UpdateWeightData();
               esp38.TESV_004145C0();
               // esp14 destructor
               return;
            }
         }
      }
      // at 0x0047012F
      esp4C = 0; // loop var; also stored in ebp
      //push ebx;
      do { // at 0x00470134
         eax = this->bodyParts[esp4C].unk08;
         ebx = &this->bodyParts[esp4C] - 8 bytes;
         if (eax) {
            ecx = this->unk548[esp4C].unk10;
            if (ecx && ecx->unk18) {
               if (eax == this->unk548[esp4C].unk08) { // at 0x0047016A
                  edx = this->bodyParts[esp4C].item;
                  edi = &this->bodyParts[esp4C];
                  esi = &this->unk548[esp4C];
                  if (edx == esi->unk00) {
                     this->unk548[esp4C].unk0C = this->bodyParts[esp4C].unk0C;
                     *edi = *esi;
                     edx = this->bodyParts[esp4C].unk10;
                     this->TESV_0046D570(edx, esp4C, esp30);
                     esi->Reset();
                     if (*(byte*)(0x012E5CA4)) {
                        eax = this->bodyParts[esp4C].unk10;
                        if (eax)
                           this->TESV_0046D0B0(eax, esp4C);
                     }
                     auto ebx = this->bodyParts[esp4C].addon;
                     if (!ebx)
                        continue;
                     if (ebx == edi->item)
                        continue;
                     esp34 += ebx->data.unk08; // float
                     continue;
                  }
                  edi = this;
               }
            }
         }
         if (this->unk548[esp4C].unk10) { // at 0x00470206
            this->TESV_0046C8A0(&this->unk548[esp4C], 1, 0);
         } else {
            if (eax) { // at 0x0047021E
               edx = this->unk548[esp4C].unk14;
               ecx = &this->unk548[esp4C].unk14;
               if (edx is not a valid NiPointer) // at 0x00470236
                  jump to 0x0047025E;
               if (eax != this->unk548[esp4C].unk08)
                  jump to 0x0047025E;
               eax = this->bodyParts[esp4C].item;
               esi = &this->unk548[esp4C];
               if (eax != this->unk548[esp4C].item) // at 0x0047024F
                  jump to 0x0047025E;
               this->bodyParts[esp4C].unk14.TESV_0060DEE0(ecx);
               esi->Reset();
            } else {
               this->unk548[esp4C].Reset(); // at 0x0047025E
            }
         }
         // at 0x00470269
         auto eax = this->bodyParts[esp4C].addon;
         if (eax && eax != this->bodyParts[esp4C].item) { // at 0x00470273
            esp34 += eax->unk30; // float
         }
         esi = this->bodyParts[esp4C].item;
         if (esi && esi->formType == kFormType_Weapon) { // at 0x0047029D
            //
            // ...
            //
         } else if (esi && esi->formType == kFormType_Light) { // at 0x00470347
            //
            // ...
            //
         } else {
            if (esp4C == 0x29) { // at 0x00470386
               
            } else {
               if (esp30) { // at 0x004703C0
                  if (eax = esp60) {
                     eax = eax->unkC4;
                     bool al = eax->unk38.TestBodyPartByIndex(esp4C);
                     if (!al)
                        continue;
                  }
               }
               edx = this->bodyParts[esp4C].unk08;
               if (!edx)
                  continue;
               eax = this->bodyParts[esp4C].item;
               if (!eax)
                  continue;
               uint8_t al = eax->formType;
               if (al != 0x1A && al != 0x1F)
                  continue;
               if (edx == -1)
                  continue;
               if (Arg2) { // at 0x00470417
                  //
                  // ...
                  //
               }
               eax = &edx->unk04.TESV_00A50AC0(); // StringCache::Ref::TESV_00A50AC0
               if (!eax)
                  continue;
               esi = 0;
               esp20 = 0;
               bool al = this->TESV_0046AE90(esp4C);
               struct {
                  UInt32 unk00 = 3;
                  UInt32 unk04 = 3;
                  bool   unk08 = false;
                  bool   unk09 = al;
                  bool   unk0A = true;
                  bool   unk0B = true;
               } esp74;
               esp27 = al;
               const char* eax = this->bodyParts[esp4C].unk08->GetModelName();
               auto eax = TESV_00AF5820(eax, &esp20, &esp74);
               if (!eax) { // at 0x004704B7
                  
               }
               esp20.TESV_007B1320();
            }
         }
      } while (++esp4C < 0x2A);
      // at 0x00470C38
      
      
   }
   //
   // clean up smart pointer esp14, and then return
   //
}