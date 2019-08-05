void ActorWeightData::Subroutine00470050(float Arg1, UInt32 Arg2) {
   // allocate 0x2E4 bytes on stack, plus register pushes
   auto edi = this;
   auto ebp = 0;
   auto esp10 = this;
   auto esp14 = 0;
   NiPointer<TESObjectREFR> currentRef = nullptr; // esp14
   TESObjectREFRHandleInterface::ExchangeHandleForRef(&this->unkA88, &currentRef);
   TESObjectREFR* esi = currentRef;
   if (!esi)
      return;
   if (this->npcRootNode) { // at 0x00470088
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
         //
         // TIP: "TESModelTextureSwap" is not a texture swap for a model, but 
         // rather a model that supports texture swapping.
         //
         ebx = &this->bodyParts[esp4C] - 8 bytes;
         if (eax) {
            NiNode* ecx = this->unk548[esp4C].renderedArmor;
            if (ecx && ecx->m_parent) {
               if (eax == this->unk548[esp4C].model) { // at 0x0047016A
                  TESForm* edx = this->bodyParts[esp4C].item;
                  edi = &this->bodyParts[esp4C];
                  esi = &this->unk548[esp4C];
                  if (edx == esi->item) {
                     esi->textureSet = edi->textureSet; // BGSTextureSet* assign
                     *edi = *esi;
                     NiNode* edx = this->bodyParts[esp4C].renderedArmor;
                     this->TESV_0046D570(edx, esp4C, esp30); // updates visibility of partitions in the node's BSDismemberSkinInstance / geometry
                     esi->Reset();
                     if (*(byte*)(0x012E5CA4)) {
                        NiNode* eax = this->bodyParts[esp4C].renderedArmor;
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
         if (this->unk548[esp4C].renderedArmor) { // if it has a NiNode // at 0x00470206
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
            if (esp4C == 0x29 && (eax = edi->bodyParts[0x29].item) && eax->formType == kFormType_Ammo) { // body part #47 (unnamed) // at 0x00470386
               auto ecx = esp18;
               if (ecx == *g_thePlayer) {
                  this->TESV_0046FB30(eax); // creates quiver node as necessary
               } else {
                  ecx->TESV_004DC0D0(eax); // calls 0046FB30 and does other stuff
               }
               continue;
            }
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
            if (al != 0x1A && al != 0x1F) // item msut be kFormType_Armor or kFormType_Light
               continue;
            if (edx == -1) // wat?
               continue;
            if (Arg2) { // at 0x00470417
               if (this->BodyPartHasFacegenHeadModelFlag(esp4C)) {
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
            const char* eax = this->bodyParts[esp4C].model->GetModelName();
            auto eax = TESV_00AF5820(eax, &esp20, &esp74); // loads a node?
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
               // definitely is not a file path. Alternatively, maybe there's special-case 
               // functionality for if a NIF's name is its model path.
               //
               if ((ecx != *(PlayerCharacter**)0x01310588) && /*bool*/ TESV_00AF5030(&esp1F0, &this->bodyParts[esp4C].unk1C)) { // at 0x004705AE
                  //
                  // No-oping this call doesn't seem to have any effect on NPCs either, 
                  // even though it's only ever made for them. But then, there could be 
                  // some edge-case involved that a simple test wouldn't catch.
                  //
                  esi = esp10 = this->bodyParts[esp4C].unk1C->unk1C->Clone(&esp98); // call is completed at 0x00470817
               } else {
                  bool al = TESV_00B06360(esp20); // NiPointer<NiNode> esp20 // at 0x004705DA
                  if (al) {
                     //
                     // No-oping this next call doesn't prevent the player's visuals 
                     // from updating when their equipment changes -- at least during 
                     // our tests. We'd need to tamper with the (al) register to be 
                     // sure that this branch is even running when we do our testing.
                     //
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
                        esi = this->bodyParts[unk4C].model->GetModelName();
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
                        //
                        // It looks like an armor filename must end in "1" or "0", and it looks 
                        // like the game swaps it (i.e. if the model path is "a_1.nif", then 
                        // the espEC string is changed to "a_0.nif"). Unless that's actually 
                        // just code to fall back to one file if the other doesn't exist?
                        //
                        if (esi[ebp - 5] == '1') { // testing the last character in the filename, not including the extension (i.e. "a_1.nif" versus "a_0.nif")
                           // at 0x004706A7
                           esp1C = esp20; // NiPointer& NiPointer::assign(NiPointer& other);
                           espEC[ebp - 5] = '0';
                           eax = TESV_00AF5680(&espEC, &esp40, &esp68); // at 0x004706D5
                           if (eax) {
                              // jumped to 0x00470761
                              esp1C = edi; // smart pointer assign; inlined
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
                           espEC[7 + ebp] = '1'; // pretty sure the char index here is wrong; should probably be ebp - 5
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
               //
               // I think by this point, (esi) is a newly-created node or a 
               // repurposed node, and the code below preps it for use as the 
               // armor-addon 3D. Hard to be sure, though.
               //
               // What I don't know is whether it's attached by this point. 
               // The armor-addon node will eventually be (or already has 
               // been) attached as a direct child of the player's root node.
               //
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
                  ecx = this->bodyParts[unk4C].model;
                  if (ecx) {
                     TESModelTextureSwap* eax = ecx->Unk_06();
                     if (eax) {
                        eax->TESV_004557B0(ebp); // most likely applies the texture swap to node (ebp)
                     }
                  }
                  // at 0x004708D8
                  BSFadeNode* eax = ebp->GetAsBSFadeNode(); // NiObject cast; not sure what to
                  if (eax)
                     eax->TESV_00C6B370(7);
                  // at 0x004708EF
                  eax = esp2C;
                  if (eax != *(PlayerCharacter**)0x01310588 && esp26) {
                     NiPointer esp64(ebp->Clone(&esp98));
                     TESV_00AF54C0(&esp1F0, &esp64, &this->bodyParts[unk4C]->unk1C, 1);
                     esp64.~NiPointer();
                  }
                  // at 0x0047093F
                  edx = esp30;
                  edi = esp4C;
                  auto eax = this->CreateArmorNode(ebp, edi, edx, 0, 0);
                  esp14 = eax; // smart pointer assign
                  NiObjectNET* esi = esp10; // or subclass
                  if (!esi) { // at 0x00470967
                     esp10 = ebp; // smart pointer assign
                     if (esp27) {
                        esp44 = esi;
                        esp14->TESV_0046B660(edi, &esp44);
                        if (esp44 != esi) {
                           ecx = 0;
                           edi = 0;
                           if (0 < ebp->m_children.m_size) {
                              do { // at 0x004709B0
                                 NiAVObject*     ecx = ebp->m_children[edi];
                                 NiTriBasedGeom* esi = ecx->GetAsNiTriBasedGeom();
                                 if (!esi)
                                    continue;
                                 NiGeometryData* ecx = esi->m_spModelData;
                                 if (!ecx)
                                    continue;
                                 NiPointer esp1C(nullptr);
                                 NiPointer esp28(nullptr);
                                 esp1C.TESV_00AAFD60(0);
                                 esp28 = esp1C; // smart pointer assign
                                 esi->SetGeometryData(esp28); // virtual 0x37
                                 //esp28.~NiPointer(); // implicit
                                 //esp1C.~NiPointer(); // implicit
                              } while (++edi < ebp->m_children.m_size);
                           }
                        }
                        // at 0x00470A21
                     }
                     // at 0x00470A2E
                     esp44.~NiPointer();
                     edi = esp4C;
                     edx = esp14;
                     eax = edx->unk04;
                     esi = esp2C;
                     TESV_0046B8C0(eax, ebp, 0, esp3C, edi);
                     ecx = esp2C; // at 0x00470A48
                     if (ecx == *(PlayerCharacter**)0x01310588) {
                        bool al = (*g_thePlayer)->Unk_71();
                        al = al xor esp30;
                        if (al) {
                           // at 0x00470A6D
                           TESV_004DA320(ebp);
                        }
                     } else {
                        // at 0x00470A6D
                        TESV_004DA320(ebp);
                     }
                     // at 0x00470A76
                     bool al = TESV_0046AAE0(edi);
                     if (al) {
                        ecx = esi->unk88;
                        if (ecx) {
                           bool al;
                           if (((esi->unk6C << 5) & 7) >= 3 && edi == 0x20)
                              al = true;
                           else
                              al = false;
                           ecx->TESV_007238C0(eax, &esp3C, esi, edi);
                        }
                     }
                     // at 0x00470AB3
                     esi = esp10;
                     if (!esi)
                        jump to 0x00470BED; // "continue" statement, with smart pointer destructors in mind
                  }
                  // at 0x00470ABF
                  esi->SetName(StringCache::Ref(&esp1F0)); // esp88 is used for the argument
                  NiPointer<ActorWeightData> esp5C(this); // esp14 == this // at 0x00470AF0; constructor is 0x00C3DF80
                  TESV_0046F010(esp20, ebp, edi, esp18, &esp5C); // at 0x00470B06
                  if (!ebx->unk20) { // byte
                     eax = esi->GetAsNiNode();
                     if (eax && !esi->m_parent) {
                        this->npcRootNode->AttachChild(esi, 1);
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
                  // NEXT IS THE LINE THAT STORES A NiNode ON THE BODY PART STRUCT. 
                  // THAT NODE, THEN, IS THE ONE THAT GETS CREATED AS PART OF THIS 
                  // ENTIRE PROCESS.
                  //
                  this->bodyParts[esp4C].renderedArmor = &esp10; // NiPointer& NiPointer::operator=(NiPointer& other);
                  esi = esp18;
                  esp14->TESV_0046DC10(edi, esp18);
                  eax = this->bodyParts[esp4C].item;
                  if (eax == *(0x012E5CDC)) { // at 0x00470BA3
                     //
                     // This item is the DLC1 "Verlet Cape." Apparently "verlet" is 
                     // a term for genuine cloth physics; I guess the vampire lord 
                     // cape is handled differently for the sake of being pretty?
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
               ebp = esp4C; // early parts of the loop are programmed so that ebp == esp4C
            }
            esp20.~NiPointer(); // at 0x00470C21
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