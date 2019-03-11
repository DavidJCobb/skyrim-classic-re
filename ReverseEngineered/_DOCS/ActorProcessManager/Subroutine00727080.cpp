void ActorProcessManager::SetUnk80(Actor* myActor, UInt32 handle) {
   if (this->unk04) {
      this->TESV_007227C0(myActor, handle);
      return;
   }
   this->unk80 = handle;
   return;
}

void ActorProcessManager::Subroutine00727080(Actor* myActor) {
   auto esi = this;
   if (this->unk04) {
      this->ClearUnk08Selectively();
      TESPackage* edi = this->TESV_006F4D40();
      if (!edi)
         return false;
      if (!edi->unk28 && edi->type != kPackageType_Sandbox && edi->type != kPackageType_Patrol)
         return false;
      auto           esp10 = *g_invalidRefHandle;
      TESObjectREFR* esp0C = nullptr;
      TESObjectREFRHandleInterface::GetRefByHandle(&esp10, &esp0C);
      this->TESV_00726800(myActor, edi, esp0C);
      edi = esp0C;
      UInt32 c;
      if (edi && !(edi->handleRefObject.m_uiRefCount & 0x3FF))
         c = *g_invalidRefHandle;
      else
         CreateRefHandleByREFR(&c, edi);
      this->SetUnk80(myActor, c);
      if (edi) {
         if (edi->flags & 0x00200000) {
            this->ClearUnk80Selectively();
            return false;
         }
      } else {
         return false;
      }
      if (edi->flags & 0x20) {
         UInt32 esp18;
         edi->extraData.GetExtraReferenceHandle(&esp18);
         if (esp18) {
            this->SetUnk80(myActor, &esp18);
         }
      }
      NiPoint3* esi = &this->middleProcess->unk90;
      *esi = edi->pos;
      edi->SetFormFlag00040000(true);
      return;
   }
   edi = this->unk0C;
   UInt32         esp10 = *g_invalidRefHandle;
   TESObjectREFR* esp0C = nullptr;
   TESObjectREFRHandleInterface::GetRefByHandle(&esp10, &esp0C);
   this->TESV_00726800(myActor, this->unk0C, esp0C);
   UInt32 c;
   if (edi && !(edi->handleRefObject.m_uiRefCount & 0x3FF))
      c = *g_invalidRefHandle;
   else
      CreateRefHandleByREFR(&c, edi);
   this->SetUnk80(myActor, c);
   if (!edi)
      return false;
   edi->handleRefObject.DecRefHandle();
   return true;
}