void ActorWeightData::Subroutine00470050(float Arg1, UInt32 Arg2) {
   auto edi = this;
   auto ebp = 0;
   auto esp10 = this;
   auto esp14 = 0;
   NiPointer<TESObjectREFR> currentRef = nullptr; // esp14
   TESObjectREFRHandleInterface::ExchangeHandleForRef(&this->unkA88, &currentRef);
   TESObjectREFR* esi = currentRef;
   if (!esi)
      return;
   if (this->unk04) { // at 0x00470088
      auto  esp34 = esi->GetShieldBodyPartIndex();
      auto  edx   = GMST:fBodyMorphWeaponAdjustMult;
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
               esp38.TESV_004145C0(); // smart pointer destructor
               // currentRef destructor
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
                     this->TESV_0046D570(edx, esp4C, esp30); // updates visibility of partitions in the node's BSDismemberSkinInstance / geometry
                     esi->Reset();
                     if (*(byte*)(0x012E5CA4)) {
                        NiNode* eax = this->bodyParts[esp4C].unk10;
                        if (eax)
                           this->TESV_0046D0B0(eax, esp4C); // performs some process on a 3D node tree
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
            this->TESV_0046C8A0(&this->unk548[esp4C], 1, 0); // call includes garbage collection
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
               this->bodyParts[esp4C].unk14.TESV_0060DEE0(ecx); // NiPointer assign
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
                  if (this->TESV_0046AE90(esp4C)) {
                     eax = currentRef;
                     if (eax != *g_thePlayer) {
                        if (INI:BackgroundLoad:bLoadHelmetsInBackground && INI:BackgroundLoad:bUseMultiThreadedFaceGen) {
                           ecx->TESV_0046F4E0(0, 0, esp4C); // ecx is never assigned?
                           continue;
                        }
                     }
                  }
               }
               eax = &edx->name.TESV_00A50AC0(); // StringCache::Ref::TESV_00A50AC0
               if (!eax)
                  continue;
               esi = 0;
               NiPointer<NiNode> esp20(nullptr);
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
                  NiCloningProcess esp98(1.0F); // constructor at 0x0046DF40
                  edi = esp60;
                  eax = esp60->unk124;
                  NiPointer esp10(nullptr);
                  NiPointer esp54(nullptr);
                  NiPointer esp58(nullptr);
                  esp50 = 0;
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
                     a = currentRef->GetFormWeight();
                  }
                  // FPU: [100.0F, a]
                  // FPU: [100.0F, 100.0F, a]
                  // FPU: [100.0F, a / 100.0F]
                  float esp28 = a / 100.0F;
                  // FPU: [100.0F]
                  // FPU: [Arg1, 100.0F]
                  // FPU: [0, Arg1, 100]
                  if (0.0F <= Arg1) { // at 0x00470543
                     esp28 = Arg1;
                  }
                  TESForm* ecx = this->bodyParts[esp4C].item;
                  //
                  // Now, we want to generate the name of the NiNode we'll be using for the armor-addon.
                  //
                  double f6 = esp28 * 100.0F
                  UInt32 f5 = ecx->formID; // eax
                  TESObjectARMA* edx = this->bodyParts[esp4C].addon;
                  UInt32 f2 = edx->formID; // ebp
                  TESObjectARMA* esi = edx;
                  auto f4 = ecx->GetEditorID(); // at 0x00470571
                  auto f3 = edi->GetSex();
                  auto f1 = esi->GetEditorID(); // at 0x00470587
                  char esp1F0[MAX_PATH];
                  snprintf(&esp1F0, MAX_PATH, "%s (%08X)[%d]/%s (%08X) [%2.0f%%]", f1, f2, f3, f4, f5, f6); // at 0x0047059C // this gets treated like a file path, but it blatantly isn't
                  //
                  ecx = esp2C;
                  //
                  // I think the TESV_00AF5030 call might load a mesh file and create a node? 
                  // It's... difficult to be sure. I wonder if it's inlined -- that might 
                  // explain why they check whether esp1F0 is a file path when it pretty 
                  // definitely is not a file path.
                  //
                  if ((ecx != *(PlayerCharacter**)0x01310588) && /*bool*/ TESV_00AF5030(&esp1F0, &this->bodyParts[esp4C].unk1C)) { // at 0x004705AE
                     esi = esp10 = this->bodyParts[esp4C].unk1C->unk1C->Clone(&esp98); // call is completed at 0x00470817
                  } else {
                     bool al = TESV_00B06360(esp20); // NiPointer<NiNode> esp20 // at 0x004705DA
                     if (al) {
                        eax = (*g_TES)->TESV_00432070(esp20, &esp98); // clones the node and does... something... to it
                        NiPointer esp54 = eax;
                        eax = esp54;
                        esi = esp54;
                     } else {
                        // at 0x00470618
                        bool x = INI:General:bUseBodyMorphs;
                        bool y = this->bodyParts[unk4C].addon;
                        bool z = this->bodyParts[unk4C].addon->unk2A[edi->GetSex()] > 1; // at 0x0047063E
                        if (!(x && y && z)) {
                           // at 0x0047080B
                           esi = esp10 = esp20->Clone(&esp98); // call is completed at 0x00470817
                        } else {
                           esi = this->bodyParts[unk4C].unk08->Unk_04();
                           ebp = eax = strlen(esi); // inlined
                           edi = 0;
                           char espEC[MAX_PATH];
                           strcpy_s(&espEC, MAX_PATH, esi);
                           esp1C = 0;
                           bool dl = esp27;
                           eax = 3; // at 0x0047067B
                           struct {
                              UInt32 unk00 = 3;
                              UInt32 unk04 = 3;
                              bool   unk08 = false;
                              bool   unk09 = dl;
                              bool   unk0A = false;
                              bool   unk0B = true;
                           } esp68;
                           esp40 = edi;
                           if (esi[ebp - 5] == '1') {
                              // at 0x004706A7
                              esp1C = esp20; // NiPointer& NiPointer::assign(NiPointer& other);
                              espEC[ebp - 5] = '0';
                              eax = TESV_00AF5680(&espEC, &esp40, &esp68); // at 0x004706D5
                              if (eax) {
                                 // jumped to 0x00470761
                                 esi = esp1C;
                                 if (esi != edi) {
                                    esi->DecRef(); // inlined, incl. destructor if refcount hits zero
                                    esp1C = edi; // NiPointer esp1C
                                 }
                                 // jumped to 0x00470784
                                 esp20 = esp1C; // NiPointer& NiPointer::assign(NiPointer& other);
                                 esi = esp10;
                                 // fall through to 0x004707F7
                              } else {
                                 esp20 = esp40->unk1C; // NiPointer& NiPointer::assign(NiPointer& other); // at 0x004706F1
                                 // at 0x004706F6
                                 if (0.0F ?? esp24.unk04) {
                                    esi = esp20->Clone(&esp98);
                                    esp50 = 0x3F; // or char '?'
                                    // fall through to 0x004707F7
                                 } else if (1.0F ?? esp24.unk04) { // at 0x004707A7
                                    esi = esp1C->Clone(&esp98);
                                    esp50 = 0x3F; // or char '?'
                                    // fall through to 0x004707F7
                                 } else {
                                    // at 0x004707CA
                                    edx = esp1C;
                                    eax = esp20;
                                    uint8_t al = TESV_00B0EEF0(esp20, esp1C, (1.0F - esp24.unk04), &esp58);
                                    esi = esp58;
                                    esp26 = al;
                                    // fall through to 0x004707F7
                                 }
                              }
                           } else {
                              // at 0x0047072D
                              espEC[7 + ebp] = '1';
                              eax = TESV_00AF5680(&espEC, &esp40, &esp68); // at 0x00470747
                              if (!eax) {
                                 esp20 = esp40->unk1C; // NiPointer& NiPointer::assign(NiPointer& other);
                                 jump to 0x004706F6;
                              }
                              // at 0x00470784
                              esp20 = esp1C; // NiPointer& NiPointer::assign(NiPointer& other);
                              esi = esp10;
                              // fall through to 0x004707F7
                           }
                           // at 0x004707F7
                           esp40.TESV_00407EC0();
                           esp1C.~NiPointer(); // func is 007B1320
                           // fall through to 0x00470822
                        }
                     }
                  }
                  // at 0x00470822
                  if (esi) {
                     eax = esi->GetAsNiNode();
                  } else
                     eax = nullptr;
                  NiPointer<NiAVObject*> esp48(eax); // constructor at 0x00489910
                  NiPointer esp10(nullptr);
                  eax = currentRef->GetNiRootNode(esp2C);
                  ebp = esp48;
                  //
                  // Find all NiGeometry in the node tree (esp48), and perform some 
                  // process on their NiProperties involving (eax); I think this may 
                  // be updating EffectShaders:
                  //
                  TESV_00C6F3B0(esp48, eax); // at 0x0047085E
                  if (ebp) {
                     ebp->m_localTransform.pos = NiPoint3(0, 0, 0);
                     ebp->m_localTransform.rot = ...; // resets position to zero; matrix to identity
                     ebp->SetScale(1.0F);
                     ebp->m_flags &= ~(0x00000001); // clear "app culled" flag
                     SetMotionType(ebp, 4, true, true, true); // set "keyframed" type // at 4708B6
                     ecx = ebx->unk10; // this->bodyParts[unk4C].unk08 ?
                     if (ecx) {
                        TESModelTextureSwap* eax = ecx->Unk_06();
                        if (eax) {
                           eax->TESV_004557B0(ebp);
                        }
                     }
                     // at 0x004708D8
                     eax = ebp->Unk_05();
                     if (eax)
                        eax->TESV_00C6B370(7);
                     // at 0x004708EF
                     eax = esp2C;
                     if (eax != *(PlayerCharacter**)0x01310588 && esp26) {
                        NiPointer esp64(ebp->Clone(&esp98));
                        TESV_00AF54C0(&esp1F0, &esp64, &ebx->unk24, 1);
                        esp64.~NiPointer();
                     }
                     // at 0x0047093F
                     edx = esp30;
                     edi = esp4C;
                     auto eax = esp14->Unknown_Call_Hooked_By_NiOverride(ebp, edi, edx, 0, 0);
                     esp14 = eax; // smart pointer assign
                     NiObjectNET* esi = esp10; // or subclass
                     if (!esi) { // at 0x00470967
                        //
                        // ...
                        //
                     }
                     // at 0x00470ABF
                     StringCache::Ref esp88(&esp1F0);
                     esi->SetName(&esp88); // esp88 is a temporary; this may have been esi->sub(StringCache::Ref(&esp1F0));
                     esp88.Release();
                     NiPointer<ActorWeightData> esp5C(esp14); // at 0x00470AF0; constructor is 0x00C3DF80
                     TESV_0046F010(esp20, ebp, edi, esp18, &esp5C); // at 0x00470B06
                     if (!ebx->unk20) { // byte
                        eax = esi->GetAsNiNode();
                        if (eax && !esi->m_parent) {
                           esp14->unk04->Unk_33(esi, 1);
                        }
                     }
                     // at 0x00470B3B
                     if (!(*(0x012E5CE0) & 1)) { // no direct references to this bitmask from elsewhere, though it could be part of a struct
                        *(0x012E5CE0) |= 1;
                        *(TESForm**)(0x012E5CDC) = GetDOBJByIndex(0x14A); // AIVC: Verlet Cape (ARMO:DLC1VampireLordCape)
                     }
                     TESForm* ecx = *(0x012E5CDC);
                     eax = 0x3F;
                     if (this->bodyParts[esp4C].item != ecx)
                        eax = esp50;
                     // at 0x00470B71
                     TESV_00C746A0(esi, 0, eax);
                     //
                     // THIS IS THE CODE THAT STORES A NiNode ON THE BODY PART STRUCT. 
                     // THAT NODE, THEN, IS THE ONE THAT GETS CREATED AS PART OF THIS 
                     // ENTIRE PROCESS.
                     //
                     this->bodyParts[esp4C].unk10 = &esp10; // NiPointer& NiPointer::operator=(NiPointer& other);
                     esi = esp18;
                     esp14->TESV_0046DC10(edi, esp18);
                     eax = this->bodyParts[esp4C].item;
                     if (eax == *(0x012E5CDC)) { // at 0x00470BA3
                        //
                        // ...
                        //
                     }
                     // at 0x00470BE4
                     esp5C.~NiPointer<ActorWeightData>();
                  }
                  // at 0x00470BED
                  esp10.~NiPointer(); // destructor for these four calls is 007B1320
                  esp48.~NiPointer();
                  esp58.~NiPointer();
                  esp54.~NiPointer();
                  esp98.TESV_00677F00();
                  ebp = esp4C;
               }
               esp20.~NiPointer(); // at 0x00470C21
            }
         }
      } while (++esp4C < 0x2A);
      // at 0x00470C38
      //
      // ...
      //
   }
   //
   // clean up smart pointer currentRef, and then return
   //
}