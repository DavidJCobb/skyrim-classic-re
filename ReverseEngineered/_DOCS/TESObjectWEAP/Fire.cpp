extern const UInt8* const iDefaultProjectilesPerShot = (UInt8*)0x0124351C;

void TESObjectWEAP::Fire(TESObjectREFR* Arg1, TESAmmo* Arg2, UInt32 Arg3, UInt32 Arg4) {
   TESObjectREFR* edi   = Arg1;
   TESObjectWEAP* esp40 = this;
   if (!Arg1 || !Arg1->parentCell)
      return;
   Actor* esi   = Arg1->formType == kFormType_Character ? Arg1 : nullptr;
   Actor* esp50 = esi;
   bool   esp3F = esi == *g_thePlayer;
   ActorProcessManager* ebx   = esi ? esi->processManager : nullptr;
   ActorProcessManager* esp74 = ebx;
   TESAmmo* espC8 = eax;
   if (!eax || eax->formType != 0x2A)
      eax = Arg1->Unk_9E();
   if (!espC8) {
      if (!ebx)
         return;
      if (this->gameData.type == kType_CrossBow) { // crossbow
         ActorWeightData* eax = Arg1->Unk_7F();
         NiNode* eax = ebx->TESV_00723860(eax);
         esi->TESV_006A2F50(eax);
         return;
      }
      ActorWeightData* eax = Arg1->Unk_7F();
      NiNode* eax = ebx->ActorWeaponNodeFromWeightModel(eax);
      esi->TESV_006A2F50(eax);
      return;
   }
   BGSProjectile* ecx   = Arg2->settings.projectile; // at 0x004AA77E
   BGSProjectile* espB0 = ecx;
   //
   // Decide on a base position for the projectile...
   //
   NiPoint3* eax;
   if (esi) { // at 0x004AA78D
      NiPoint3* projectileOffsetAxes = (NiPoint3*)0x012C52D0; // == NiPoint3(0, 0, 1)
      NiPoint3* eax = projectileOffsetAxes->Multiply(&espB4, 96.0F);
      eax = Arg1->pos.Add(&esp90, eax);
      //
      // So, fire from the shooter-actor's position + (0, 0, 96).
      //
   } else
      eax = &Arg1->pos; // if the shooter is not an actor, just use its position
   NiPoint3 esp78 = *eax;
   //
   // The position chosen above will be overridden if we're able to get a proper 
   // firing node on the shooter.
   //
   float pitch; // esp84
   float yaw;   // esp88
   if (esi) {
      pitch = esi->GetAimPitch();
      yaw   = esi->GetAimYaw();
   } else {
      pitch = Arg1->rot.x;
      yaw   = Arg1->rot.z;
   }
   NiNode* eax;
   if (ebx) { // at 0x004AA80E
      //
      // If we patch this branch to never run, then the bug doesn't 
      // occur -- even if we don't patch the 0x004AA870 branch.
      //
      if (this->gameData.type == kType_CrossBow) {
         ActorWeightModel* eax = Arg1->Unk_80();
         eax = ebx->TESV_00723860(eax);
      } else {
         ActorWeightModel* eax = Arg1->Unk_80();
         eax = ebx->ActorWeaponNodeFromWeightModel(eax);
      }
   } else {
      // at 0x004AA83F
      if (esp3F)
         eax = (*g_thePlayer)->Unk_8D();
      else
         eax = Arg1->GetNiNode();
      eax = this->FindEmbeddedOrProjectileDescendantNode(eax);
   }
   NiNode* esi = eax; // at 0x004AA86C
   if (esi) { // at 0x004AA870
      //
      // If we patch this branch to never run, then the bug doesn't 
      // occur. However, I think the problem may be with code from 
      // earlier in the function...
      //
      esp78 = esi->m_worldTransform.pos;
      //
      // This call isn't what's breaking things:
      //
      Arg1->AdjustProjectileFireTrajectory(esi, &pitch, &yaw, &esp78); // doesn't appear to modify the position, at least in vanilla, but it may be worth no-oping this call to confirm that
      if (INI:Combat:bLaserSights) { // at 0x004AA8BA
         //
         // ...
         //
      }
   }
   //
   // I think by this point, the projectile initial position is fully decided on.
   //
   auto esi = this; // at 0x004AAA0B
   if (!esp3F) { // shooter is an NPC
      if (esp50) {
         if (esp50->IsInCombatAndNotIgnoringCombat()) {
            if (!esp50->unkD8->TESV_007ACF50(esi, &esp60)) {
               // jumped to 0x004AA736
               if (this->gameData.type == kType_CrossBow) { // crossbow
                  ActorWeightData* eax = Arg1->Unk_7F();
                  NiNode* eax = ebx->TESV_00723860(eax);
                  ptrBSTaskPool->QueueTaskType3C(eax);
                  return;
               }
               ActorWeightData* eax = Arg1->Unk_7F();
               NiNode* eax = ebx->ActorWeaponNodeFromWeightModel(eax);
               ptrBSTaskPool->QueueTaskType3C(eax);
               return;
            }
            if (!espB0 || !(espB0->data.unk00 & 0x400)) { // at 0x004AAA5F
               //
               // ...
               //
            }
            if (*(UInt8*)(0x01243530) == 0)
               jump to 0x004AAD14;
            // at 0x004AAAAF
            esi = esp50->unk40->TESV_00432C00();
            if (esi) {
               //
               // ... works with bone names, NiNodes ...
               //
            }
         } else {
            // at 0x004AABBE
            //
            // ...
            //
         }
         esi = this;
      }
   }
   // at 0x004AAD14
   if (edi == *(PlayerCharacter**)0x01310588) { // at 0x004AAD1A
      //
      // ... controller vibration, menu stuff, etc. ...
      //
   }
   bool esp3E;
   if (ebx) // at 0x004AAD8E
      esp3E = ebx->TESV_0071F500();
   else
      esp3E = true;
   bool   al = (this->gameData.unk00) ? this->gameData.unk00->unk18 : *(UInt8*)(0x0124351C);
   auto   ecx = *(UInt32*)(0x012E7560);
   SInt32 esi = (UInt32)al * (SInt32)esp3E;
   espA0 = 0;
   if (ecx->unk10 == 4) {
      if (Arg1 == esp60) // Arg1 == esp50
         esp54 = ecx->TESV_007A5B60();
   }
   esp3D = false;
   esp5B = false;
   if (ebx && !Arg1->IsInCombatAndNotIgnoringCombat()) { // at 0x004AADF6 // Arg1 == esp50
      esp3D = ebx->Subroutine0071FD40();
      esp5B = ebx->GetMiddleProcessUnk208();
      if (esp5B)
         ebx->SetMiddleProcessUnk208(false);
   }
   // at 0x004AAE28
   bool esp5A = false;
   if (esp54) {
      if (esp54->unk09) {
         bool esp5A = TESV_005ACA60(esp54->unk00);
      }
   }
   if (esp50 && ebx) { // at 0x004AAE5A
      ebx->TESV_006FBE40(esi);
   }
   UInt32   esp44 = *g_invalidRefHandle;
   refr_ptr espAC = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(esp44, &espAC);
   ecx = esp54;
   eax = esp50;
   esi = 0;
   esp48 = 0;
   if (ecx && eax && eax == (*(0x012E7560))->unk64) {
      refr_ptr esp44;
      esp44.set_from_handle(&ecx->unk0C);
      espAC = esp44; // refr_ptr& refr_ptr::operator=(const refr_ptr& other)
      esp44.~refr_ptr();
      esp48 = espAC; // refr_ptr& refr_ptr::operator=(const refr_ptr& other)
   } else {
      // at 0x004AAEE4
      if (esp3D) {
         eax->extraData.TESV_0040E7C0(&esp90);
         refr_ptr esp44;
         esp44.set_from_handle(&eax);
         esp48 = esp44; // refr_ptr& refr_ptr::operator=(const refr_ptr& other)
         esp44.~refr_ptr();
      }
   }
   // at 0x004AAF18
   if (esp3E) {
      auto eax = edi->Unk_9E();
      if (eax && ebx && esp3F) { // two unknown conditions, plus shooter is the player
         esi = this;
         ActorWeightModel* eax = (*g_thePlayer)->GetWeightModel(0);
         if (this->gameData.type == 9) {
            eax = ebx->TESV_00723860(eax);
         } else {
            eax = ebx->ActorWeaponNodeFromWeightModel(eax);
         }
         (*(0x01B38308))->TESV_006A2F50(eax);
         ActorWeightModel* eax = (*g_thePlayer)->GetWeightModel(1);
         if (this->gameData.type == 9) {
            eax = ebx->TESV_00723860(eax);
            (*(0x01B38308))->TESV_006A2F50(eax);
         } else {
            eax = ebx->ActorWeaponNodeFromWeightModel(eax);
            (*(0x01B38308))->TESV_006A2F50(eax);
            esi = 0;
         }
      } else {
         // at 0x004AAFD6
         //
         // ...
         //
      }
   }
   // at 0x004AB00F
   espA4 = esi;
   if (esp3E) { // at 0x004AB01B
      bool bl = esp3D;
      do {
         if (esp5A) { // at 0x004AB02A
            if (0.0 ?? *(float*)(0x012E7568) && esp54->unk09 > 0) {
               esp3D = true;
            } else {
               esp3D = esp54->unk08 == 0;
            }
         } else
            esp3D = false;
         // at 0x004AB05D
         esi = 0;
         bool esp59 = true;
         espCC = 0;
         UInt8 esp4F = esp40->GetProjectilesPerShot();
         if (esp4F) { // at 0x004AB07A
            do {
               //
               // LOOP: Create LaunchData for multiple projectiles, and then 
               // create/launch them. After they're created/launched, perform 
               // a little bit of additional processing on them.
               //
               // This depends on the number of projectiles per shot; if there 
               // is no data for that on the base weapon, then we use the 
               // iDefaultProjectilesPerShot constant (whose value is 1).
               //
               float esp74 = yaw;
               float esp5C = pitch;
               if (bl) {
                  //
                  // ...
                  //
               } else
                  esi = esp50;
               espE8 = Projectile::LaunchData();
               espE8.projectileBase = espB0;
               espE8.unk20 = edi; // shooter?
               eax = esi ? esi->unkD8 : nullptr;
               espE8.unk24 = eax;
               espE8.shooterWeapon = this;
               espE8.shooterAmmo   = espC8;
               espE8.position = esp78; // NiPoint3 assign
               espE8.yaw   = esp74;
               espE8.pitch = esp5C;
               espE8.unk38 = esp48;
               espE8.unk68 = bl;
               if (!esp5B) {
                  espE8.unk69 = !!esp3D;
               } else
                  espE8.unk69 = true;
               edx = edi->parentCell; // at 0x004AB2D3
               eax = ebp->unk10;
               ecx = ebp->unk14;
               espE8.unk48 = edx;
               espE8.unk54 = eax;
               espE8.unk58 = ecx;
               TESV_007A53F0(&espE4, &espE8); // creates/launches the projectile? sets espE4 to the new projectile refHandle?
               TESObjectREFRHandleInterface::ExchangeHandleForRef(&espE4, &espCC);
               TESObjectREFR* esi = espDC; // the launched projectile?
               if (esi) { // at 0x004AB327
                  espA4 = std::max<float>(esi->unkFC, espA4);
               }
               //
               // A patch to this spot indicates that as of this point, the 
               // projectile has a sensible, not-glitch-prone trajectory.
               //
               if (esp54) { // at 0x004AB362
                  if (eax->unk08 || eax->unk09) { // at 0x004AB372
                     if (TESV_005ACA60(esp54->unk00)) { // at 0x004AB385
                        if (!esp3D) { // at 0x004AB38C
                           ecx = *(0x012E7560);
                           if (!(this->gameData.flags2 & kFlags_Automatic) || !ecx->unk28) {
                              //
                              // This is not the function call that breaks our position. 
                              // No-oping it does nothing.
                              //
                              ecx->TESV_007A59E0(esi); // at 0x004AB3A8
                           }
                        }
                     }
                  }
               }
               if (!esp59) // at 0x004AB3AD
                  esi->unkD8 = 0;
               else
                  esp59 = 0;
               esp4F--;
               espE8.~LaunchData(); // at 0x004AB3D0
            } while (esp4F);
         }
         if (esp3D) // at 0x004AB3E5
            espA0++;
         if (esp54) { // at 0x004AB3F3
            if (!esp54->unk08 && !esp54->unk09) {
               if (TESV_005ACA60(esp54->unk00)) {
                  bool al;
                  if (esp3D && esp54->unk09) {
                     al = esp54->unk09;
                     esp54->unk09--;
                  } else {
                     al = --(esp54->unk08);
                  }
                  float esp44 = std::min<float>(GMST:fVATSShotLongBurstTime, GMST:fVATSShotBurstTime);
                  float esp60 = GMST:fVATSShotBurstTime / esp44;
                  *(float*)(0x012E7568) += esp60;
                  if (*(float*)(0x012E7568) >= esp60) { // uh, okay
                     *(float*)(0x012E7568) -= 1.0F;
                  }
                  if (cl && esp54->unk08 != cl) { // at 0x004AB4AD
                     ecx = *(0x012E7560);
                     ecx->TESV_007A7770(esi); // not responsible for the arrow bug
                  }
               }
            }
         }
         esp3E--; // at 0x004AB4BB
         if (esi) { // at 0x004AB4C1
            esi->handleRefObject.DecRef();
         }
      } while (esp3E); // at 0x004AB4E5
      if (espA0 && esp40->gameData.type == kType_Bow) { // at 0x004AB500
         if (esp50 && esp50 == *(PlayerCharacter**)0x01310588) { // at 0x004AB510
            //
            // ... send the player-bow-shot event ...
            //
         }
      }
   }
   // at 0x004AB555
   UInt32 espD0 = -1;
   bool   espD4 = false;
   UInt32 unkD8 = 0;
   auto   ecx = GetUnknown01B9ED08Instance();
   //
   // No-oping the 00B9CB60 call(s) doesn't fix the bug.
   //
   if (esp3F && edi->unk108) { // if shooter == player and another condition
      ecx->TESV_00B9CB60(&espD0, &edi->unk108.unk14, 0x12);
   } else {
      if (eax = edi->unk104) { // at 0x004AB598
         if (TESV_00653AF0(eax, &esp78) {
            eax = edi->unk104 ? &edi->unk104.unk14 : nullptr;
            ecx->TESV_00B9CB60(&espD0, eax, 0x10);
         }
      }
   }
   // at 0x004AB5DC
   NiPoint3 a = esp78;
   espD0.TESV_00B9A0F0(a); // these two calls aren't related to the arrow bug
   espD0.TESV_00B99ED0();
   if (esi = esp50) { // shooter-actor // at 0x004AB617
      esi->Unk_D1(espA0); // espA0 == number of projectiles fired
      eax = SoundLevelEnumToLoudness(edi->unk130);
      esi->TESV_006A9170(eax); // responsible for alerting other actors?
   }
   if (esp48) // at 0x004AB649
      esp48.~refr_ptr();
   if (espAC) // at 0x004AB675
      espAC.~refr_ptr();
   return; // at 0x004AB694
}