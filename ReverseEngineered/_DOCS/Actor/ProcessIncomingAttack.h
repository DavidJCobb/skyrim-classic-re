void Actor::ProcessIncomingAttack(Struct00797220* hit) {
   auto esi = this;
   if ((this->actorState.flags04 & 0x0E000000) == 0x0A000000)
      return;
   auto ebp = 0;
   if (!this->processManager)
      return;
   this->CreateOrClearHitStruct();
   if (ShouldUseBSTaskPoolForTask()) {
      ptrBSTaskPool->QueueTaskType10(this, hit);
      return;
   }
   auto edi = hit;
   bool esp22 = this->flags & 0x00004000;
   this->ImportHitStruct(hit);
   auto esp14 = hit->attackerRefHandle;
   refr_ptr esp1C = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(&esp14, &esp1C);
   TESObjectWEAP* eax = hit->unk28; // at 0x006E4804
   if (eax)
      ebx = eax->formID;
   else
      ebx = (*ptrUnarmedForm)->formID;
   auto eax = this->GetOrCreateRefHandle(&esp30);
   refr_ptr esp3C = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(eax, &esp3C);
   TESObjectREFR* ebp = esp1C; // attacker
   if (ebp) {
      eax = *ebp->GetOrCreateRefHandle(&esp30);
      esp14 = eax;
      eax = &esp14;
   } else
      eax = g_invalidRefHandle;
   esp14 = *eax;
   refr_ptr esp38 = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(&esp14, &esp38);
   BSTEventSourceHolder::GetInstance()->SendHitEvent(&esp3C, &esp38, ebx, 0, hit);
   auto esp28 = hit->damage;
   auto esp30 = hit->unk3C;
   auto unk2C = hit->damageBlocked;
   if (ebp && ebp == *(PlayerCharacter**)0x01310588) { // attacker
      if (!this->IsDead(0)) {
         if (this != *(PlayerCharacter**)0x01310588) { // at 0x006E48E3
            memset(&esp79, 0, 499);
            if (hit->flags >> 3 & 1) { // Struct00797220::kFlag_IsCriticalHit
               ebx = GMST:sCriticalStrike; // "Critical Strike on"
               snprintf(&esp78, 500, "%s %s", ebx, this->sub004DE820());
               ShowNotification(&esp78, "UISneakAttack", 1);
               sub006E4250(ebp, hit->unk28, 0);
            }
            // at 0x006E4955
            if (hit->flags >> 0xB & 1) { // Struct00797220::kFlag_IsSneakAttack
               //
               // ...
               //
            }
         }
      }
   }
   // at 0x006E4A0B
   bool esp23 = !this->IsDead(0);
   esp14 = hit->projectileRefHandle;
   refr_ptr esp24 = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(&esp14, &esp24);
   if (0.0F ?? esp2C) { // at 0x006E4A50 // damage blocked
      //
      // ...
      //
   }
   // at 0x006E4A70
   float esp34 = this->actorValueOwner.GetCurrent(0x18); // health
   bool al = this->sub006AC240(esp28, ebp, 0);
   bool esp13;
   if (!al) {
      if (!ebp || !(ebp->unk138 << 0xE & 1)) {
         esp13 = this->GetAIProcessBool215();
      } else
         esp13 = true;
   } else
      esp13 = true;
   if (esp23) { // at 0x006E4AC5 // actor wasn't dead beforehand
      if (hit->flags & kFlag_IsCriticalHit && hit->unk2C) {
         bool a = this->IsEssentialOrProtected();
         if (a || !esp13)
            a = !(hit->flags & kFlag_CritEffectOnDeath);
         if (a && INI:General:bDisableAllGore == false) { // at 0x006E4B00
            ActorMagicCaster* ebx = this->GetOrCreateMagicCaster(3);
            if (ebx) {
               auto eax = hit->unk2C;
               ebx->sub0065E290(eax);
               esp14 = 0;
               ebx->sub0065E290(1.0F, &esp14, 0, 0, 0);
               this->magicTarget.sub00663DF0(hit->unk2C);
            }
         }
      }
      if (esp13 && ebp) { // at 0x006E4B58
         auto ecx = ebp->processManager; // killer
         bool bl;
         bool esp14 = bl = hit->flags & kFlag_00020000;
         if (ecx) {
            esp14 = ecx->sub0071EDD0(esp14);
            if (esp14) {
               if (auto eax = esp14->sub00475840()) { // branch at 0x006E4B8C
                  //
                  // ... soul trapping? ...
                  //
               }
            }
         }
      }
   }
   if (esp24) { // at 0x006E4BAB
      ebp = esp24->Unk_8E();
      if (ebp) {
         BGSBodyPartData* ebx   = this->GetBodyPartData();
         BGSBodyPartData* esp40 = ebx;
         if (ebx) {
            ebp = ebp->sub00790230(this);
            if (ebp) {
               do {
                  auto ecx = ebp->unk04;
                  if (!ecx || !ebp->unk00)
                     break;
                  ebp = ecx;
                  BGSBodyPartData::Data* eax = ebx->GetPart(ebp->unk00->unk00);
                  if (!eax)
                     continue;
                  SInt32 ecx   = eax->limbData.actorValue;
                  SInt32 esp50 = ecx;
                  if (ecx >= ActorValueList::kNumActorValues) // at 0x006E4C25
                     continue;
                  UInt8 bl = eax->limbData.healthPercent;
                  double esp44 = esp28; // == attack damage
                  float a = this->actorValueOwner.GetBase(0x18); // health
                  TESObjectWEAP* eax = hit->unk28;
                  float b = bl * 0.01F * a;
                  float esp18 = (esp44 / b) * 100.0F;
                  eax = eax ? eax : *ptrUnarmedForm;
                  CalculatePerkData(4, this, ebx, eax, &esp18);
                  auto  edx = esp50; // AV index
                  float c   = -esp18;
                  // Internal function for changing an actor value:
                  this->sub006E0760(kAVModifier_Damage, edx, c, ebx);
                  ebp = esp40;
               } while (ebp);
            }
         }
      }
   }
   // at 0x006E4CA7
   if (auto eax = this->GetNiNode()) {
      auto esp48 = hit->unk20;
      refr_ptr esp18 = nullptr;
      TESObjectREFRHandleInterface::GetRefByHandle(&esp3C, &esp18);
      TESObjectREFR* eax = esp18;
      if (eax) { // at 0x006E4CE2
         ecx = 0;
         if (eax->formType == 0x3E) {
            ecx = eax->extraData.GetExtraLeveledCreature();
         }
      }
      if (!esp22) { // at 0x006E4CFF
         if (esp23) {
            //
            // ...
            //
         } else
            ebp = esp1C;
         if (!esp13) {
            // skip: ebp = esp1C;
            esp34 = esp28 / esp34;
            if (!this->sub006A8890() && !this->PackageTypeIsInGameDialogue()) {
               if (!this->processManager->sub006FBBF0()) {
                  float esp44 = esp34;
                  if (GMST:fCombatSpeakHitThreshold ?? esp34) {
                     if (sub00489930(GMST:fCombatSpeakHitChance))
                        esp21 = true;
                  } else
                     esp21 = true; // at 0x006E4DE8
               }
            }
            // at 0x006E4DED
            if (!this->sub006A7470()) {
               if (hit->flags & 0x8000) {
                  this->sub006B6480(1);
               } else {
                  float esp34 = hit->unk50;
                  if (0.0F ?? esp34) { // at 0x006E4E28
                     if (this != *(PlayerCharacter**)0x01310588 || !INI:Combat:bPlayerAlwaysStaggered == false) {
                        //
                        // ...
                        //
                     }
                     // at 0x006E4E7A
                     this->sub006BD700RelatedToStagger(hit->unk50, ebp);
                  }
               }
            }
         }
      }
      // esp18 destructor at 0x006E4E8D
   }
   if (this->GetNiNode()) {
      if (0.0F ?? esp2C) { // at 0x006E4EB3
         this->sub006B63E0();
         auto ecx = *(0x01B3D1CC);
         ecx->sub007B3260(this, 0, 3, 0x23, 0, 0);
         ebp = esp1C;
         if (!esp24 && ebp && !(hit->flags & 0x4000) && !this->IsDead(0)) {
            ebp->sub006B6480(0); // ebp == attacker?
         }
         // at 0x006E4F0B
         //
         // ...
         //
         // jump to 0x006E5321 for the CALL opcode:
         VibrateController(1, GMST:fRumbleBlockStrength, GMST:fRumbleBlockTime);
      } else {
         if (!esp22) { // at 0x006E5014
            //
            // ...
            //
         } else
            ebp = esp1C;
         ecx = this->sub006AE280(*(float*)0x01B4ADE0);
         if (ebp && ebp == *(PlayerCharacter**)0x01310588) { // at 0x006E51C0
            ecx &= 0xFFFFFFF7;
         }
         if (!(hit->flags & 0x4000) && !(hit->flags & 0x8000)) { // at 0x006E51D7
            //
            // ...
            //
         }
         // at 0x006E5234
         if (this == *(PlayerCharacter**)0x01310588 || (hit->unk18 == *RefHandle01B2E8E8 && esp24 == 0)) {
            float esp14 = GMST:fRumbleStruckStrength;
            float esp18 = GMST:fRumbleStruckTime;
            auto ebx = *(0x012E7560);
            if (ebp && ebp == *(PlayerCharacter**)0x01310588 && !ebx->unk10) { // at 0x006E5288
               auto eax = ebp->GetNiNode();
               auto eax = sub0051B8D0(GMST:fGetHitPainMult, eax);
               sub004B5A00(eax);
            } else {
               // at 0x006E52B6
               esp14 = GMST:fRumbleHitStrength;
               esp18 = GMST:fRumbleHitTime;
               if (ebx->unk10) {
                  if (/*byte*/ebx->unk40) {
                     if (MenuManager::GetInstance()->unkC8 <= 0) {
                        /*byte*/ebx->unk40 = 0;
                     }
                  }
                  esp18 *= *(float*)0x012C2410;
               }
            }
            // at 0x006E52FE
            if (MenuManager::GetInstance()->unkC8 <= 0) { // at 0x006E530B
               VibrateController(0, esp14, esp18); // CALL opcode at 0x006E5321
            }
         }
      }
      // at 0x006E5329
      if (hit->unk30 && ebp) {
         ebp->sub006E4560(hit->unk30, this);
      }
      if (!esp22) { // at 0x006E5342
         
      }
   } else
      ebp = esp1C; // at 0x006E538F
   if (!ebp && (ebx = esp24)) {
      edi = ebx->Unk_51();
      if (edi && /*bool*/edi->sub005ACC20()) {
         UInt32 esp30 = edi->unk00; // refHandle
         refr_ptr esp2C(&esp30);
         refr_ptr esp1C = esp2C;
         // esp2C->~refr_ptr();
         ebp = esp1C;
      }
   } else
      ebx = esp24; // at 0x006E53E9
   edi = this->unkD8; // at 0x006E53ED
   if (ebp && (auto ecx = this->processManager)) { // at 0x006E53FF
      if (ecx->sub0071FDC0()) {
         esp1C.assign(nullptr); // refr_ptr::assign
         ebp = esp1C;
      }
   }
   this->sub006E2740(ebp, 0);
   if (/*bool*/this->sub006A8890() || this->PackageTypeIsInGameDialogue()) {
      esp21 = false;
   }
   if (!edi) {
      edi = this->unkD8;
      if (edi) { // at 0x006E544A
         edi->sub007ACD20(ebp, /*float*/esp28);
      }
   }
   if (esp21) { // at 0x006E5461
      (*(0x01B3D1CC))->sub007B3260(this, ebp, 3, 0x1D, 1, edi);
   }
   this->CreateOrClearHitStruct();
   // ebx->~refr_ptr();
   // esp38->~refr_ptr();
   // esp3C->~refr_ptr();
   // ebp->~refr_ptr();
}