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
         TESModelTextureSwap* eax = this->bodyParts[esp4C].unk08;
         ebx = &this->bodyParts[esp4C] - 8 bytes;
         if (eax) {
            NiNode* ecx = this->unk548[esp4C].unk10;
            if (ecx && ecx->m_parent) {
               if (eax == this->unk548[esp4C].unk08) { // at 0x0047016A
                  TESForm* edx = this->bodyParts[esp4C].item;
                  edi = &this->bodyParts[esp4C];
                  esi = &this->unk548[esp4C];
                  if (edx == esi->unk00) {
                     esi->unk0C = edi->unk0C; // BGSTextureSet* assign
                     *edi = *esi;
                     NiNode* edx = this->bodyParts[esp4C].unk10;
                     this->TESV_0046D570(edx, esp4C, esp30);
                     esi->Reset();
                     if (*(byte*)(0x012E5CA4)) {
                        NiNode* eax = this->bodyParts[esp4C].unk10;
                        if (eax)
                           this->TESV_0046D0B0(eax, esp4C);
                     }
                     TESObjectARMA* ebx = this->bodyParts[esp4C].addon;
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
         if (this->unk548[esp4C].unk10) { // if it has a NiNode // at 0x00470206
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
         TESObjectARMA* eax = this->bodyParts[esp4C].addon;
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
               //
               // ...
               //
            } else {
               if (esp30) { // at 0x004703C0
                  if (eax = esp60) {
                     eax = eax->unkC4;
                     bool al = eax->unk38.TestBodyPartByIndex(esp4C);
                     if (!al)
                        continue;
                  }
               }
               TESModelTextureSwap* edx = this->bodyParts[esp4C].unk08;
               if (!edx)
                  continue;
               TESForm* eax = this->bodyParts[esp4C].item;
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
               eax = &edx->name.TESV_00A50AC0(); // StringCache::Ref::TESV_00A50AC0
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
                  esp98.TESV_0046DF40(1.0F);
                  edi = esp60;
                  eax = esp60->unk124;
                  esp10 = esi;
                  esp54 = esi;
                  esp58 = esi;
                  esp50 = esi;
                  bool esp26 = false;
                  float a;
                  if (eax != esi) { // at 0x004704F2
                     do {
                        ecx = eax->unk124;
                        if (ecx != esi)
                           eax = ecx;
                     } while (eax != esi);
                     a = eax->unk12C;
                  } else {
                     a = esp18->GetFormWeight();
                  }
                  // FPU: [100.0F, a]
                  // FPU: [100.0F, 100.0F, a]
                  // FPU: [100.0F, a / 100.0F]
                  float esp28 = a / 100.0F;
                  // FPU: [100.0F]
                  // FPU: [Arg1, 100.0F]
                  // FPU: [0, Arg1, 100]
                  if (0.0F ?? Arg1) { // at 0x00470543
                     esp28 = Arg1;
                  }
                  ecx = ebx->unk08;
                  double f6 = esp28 * 100.0F
                  // FPU: [100]
                  f5 = ecx->formID; // eax
                  edx = ebx->formID;
                  f2 = edx->formID; // ebp
                  esi = edx;
                  auto f4 = ecx->Unk_32(); // at 0x00470571
                  auto f3 = edi->GetSex();
                  auto f1 = esi->Unk_32(); // at 0x00470587
                  UnkOutput004202A0(&esp210, 0x104, "%s (%08X)[%d]/%s (%08X) [%2.0f%%]", f1, f2, f3, f4, f5, f6); // at 0x0047059C
                  ecx = esp2C;
                  if ((ecx != *(PlayerCharacter**)0x01310588) && /*bool*/ TESV_00AF5030(&esp1F0, &ebx->unk24)) { // at 0x004705AE
                     esi->unk00->unk1C->TESV_00AAFC00(&esp98); // call is completed at 0x00470817
                  } else {
                     bool al = TESV_00B06360(esp20); // at 0x004705DA
                     if (al) {
                        eax = (*g_TES)->TESV_00432070(esp20, &esp98);
                        NiPointer esp54 = eax;
                        eax = esp54;
                        esi = esp54;
                     } else {
                        // at 0x00470618
                        if (INI:General:bUseBodyMorphs && ebx->unk0C) {
                           if (ebx->unk0C->unk2A[edi->GetSex()] > 1) { // at 0x0047063E
                              //
                              // ...
                              //
                           }
                        }
                        // at 0x0047080B
                        ecx = esp20;
                        // at 0x00470817
                        esi = esp10 = ecx->TESV_00AAFC00(&esp98);
                     }
                     // at 0x00470822
                     if (esi) {
                        eax = esi->Unk_03();
                     } else
                        eax = 0;
                     esp48.TESV_00489910(eax);
                     esp10 = 0;
                     eax = esp18->Unk_6F(esp30);
                     ebp = esp48;
                     TESV_00C6F3B0(esp48, eax); // at 0x0047085E
                     if (ebp) {
                        //
                        // ...
                        //
                     }
                     // at 0x00470BED
                     esp10.TESV_007B1320();
                     esp48.TESV_007B1320();
                     esp58.TESV_007B1320();
                     esp54.TESV_007B1320();
                     esp98.TESV_00677F00();
                     ebp = esp4C;
                     // fall through to 0x00470C21
                  }
                  // at 0x00470817
                  //
                  // ...
                  //
               }
               esp20.TESV_007B1320(); // at 0x00470C21
            }
         }
      } while (++esp4C < 0x2A);
      // at 0x00470C38
      //
      // ...
      //
   }
   //
   // clean up smart pointer esp14, and then return
   //
}