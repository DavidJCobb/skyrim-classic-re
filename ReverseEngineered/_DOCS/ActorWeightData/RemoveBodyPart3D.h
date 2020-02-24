void ActorWeightData::RemoveBodyPart3D(ActorWeightData::BodyPart* Arg1, bool Arg2, TESModelTextureSwap* Arg3) { // at 0x0046C8A0
   ActorWeightData::BodyPart* esi = Arg1;
   auto ebx   = 0;
   auto edi   = this;
   auto esp18 = this;
   if (esi) {
      NiNode* ecx = Arg1->renderedArmor;
      if (ecx) {
         NiNode* eax = ecx->GetAsNiNode();
         sub004DA3E0(eax);
         sub005F4A70(Arg1->renderedArmor);
         this->sub0046C000(Arg1->renderedArmor);
         auto ebp = &this->ownerHandle;
         auto eax = Arg1->renderedArmor;
         if (!eax)
            jump to 0x0046CB8C;
         NiObject* parent      = nullptr; // eax // unknown type
         uint8_t   weapon_type = 0; // cl
         //
         bool handle_as_weapon = false;
         if (auto ecx = Arg1->item) {
            if (ecx->formType == kFormType_Weapon) {
               weapon_type = ecx->gameData.type;
               if (weapon_type != kType_Staff) {
                  auto node = eax->m_parent;
                  if (node) {
                     parent = node->Unk_05(); // virtual cast
                     handle_as_weapon = parent != nullptr;
                  }
               }
            }
         }
         if (handle_as_weapon) {
            auto ebp = parent->m_parent;
            auto edi = GetNodeByName_Wrapper00458CF0(edi->unk04, GetNodeNameByWeaponType(Arg1->renderedArmor, weapon_type));
            esp14 = edi;
            if (ebp) {
               for(ebx = 0; ebx < ebp->unkB2; ++ebx) {
                  edi = ebp->unkAC[ebx];
                  if (!edi)
                     continue;
                  if (!ShouldUseBSTaskPoolForTask()) {
                     sub0046BF90(&this->ownerHandle, edi); // stops any EffectShaders on (edi) and removes it from the node tree
                  } else {
                     ptrBSTaskPool->QueueTaskType36(&this->ownerHandle, edi);
                  }
               }
               edi = esp14;
            }
            ecx = esp10; // at 0x0046C9CE
            if (ecx) {
               eax = ecx->GetShieldBodyPartIndex();
               if (edi && ebp != edi && eax != -1) { // at 0x0046C9E6
                  if (esi != &this->bodyParts[eax]) { // Arg1 is not the "shield" body part
                     ebx = edi->Unk_03();
                     if (ebx) { // check patched in by Crash Fixes
                        for (ebp = 0; ebp < ebx->unkB2; ++ebp) {
                           edx = ebx->unkAC;
                           edi = edx[ebp];
                           if (!edi)
                              continue;
                           if (!ShouldUseBSTaskPoolForTask()) {
                              sub0046BF90(&this->ownerHandle, edi); // stops any EffectShaders on (edi) and removes it from the node tree
                           } else {
                              ptrBSTaskPool->QueueTaskType36(&this->ownerHandle, edi);
                           }
                        }
                     }
                  }
               }
            }
            ebp = &this->ownerHandle; // at 0x0046CA53
         } else {
            // jumped to 0x0046CB8C
            if (!ShouldUseBSTaskPoolForTask()) {
               sub0046BF90(this->ownerHandle, Arg1->renderedArmor); // stops any EffectShaders on (edi) and removes it from the node tree
            } else {
               ptrBSTaskPool->QueueTaskType36(this->ownerHandle, Arg1->renderedArmor);
            }
            // jump back to 0x0046CA57
         }
         Arg1->renderedArmor = nullptr; // inlined: decrementing the refcount
         if (ecx = Arg1->unk14) {
            refr_ptr esp14 = nullptr;
            TESObjectREFRHandleInterface::ExchangeHandleForRef(&this->ownerHandle, &esp14);
            TESObjectREFR* edi = esp14;
            if (edi) {
               Arg1->unk14->Unk_14(&edi->animGraphHolder);
            }
            ecx   = Arg1->unk14; // at 0x0046CABD // related to tail physics
            esp20 = nullptr; // NiPointer
            if (ecx->Unk_02(&esp20))
               ptrGarbageCollector->Add(&esp20);
            ebp = esp20;
            if (ebp) {
               // decrement refcount
            }
            if (edi) {
               // decrement refcount for esp14
            }
         }
         Arg1->unk14 = nullptr; // inlined: decrementing the refcount
         auto edi = esp10; // at 0x0046CB46
         if (edi) {
            // decrement TESObjectREFR refcount
         }
      }
   }
   if (Arg2) { // bool
      auto ecx = Arg3; // game always passes nullptr, apparently
      Arg1->item  = nullptr;
      Arg1->addon = nullptr;
      Arg1->model = ecx;
   }
   return;
}