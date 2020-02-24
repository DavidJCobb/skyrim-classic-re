struct HeadNodeListForDecapitation {
   uint32_t count = 0;
   uint32_t unk04;
   NiNode*  nodes[0x100];
   //
   static HeadNodeListForDecapitation& get() {
      return *(HeadNodeListForDecapitation*)0x01B383F0;
   }
   inline void clear() {
      this->count = 0;
      memset(&this->nodes, 0, sizeof(this->nodes));
   }
   inline void push_back(NiNode* a) {
      if (this->count >= 0x100)
         return;
      this->nodes[this->count] = a;
      ++this->count;
   }
};

void Actor::PrepareHeadNodeListForDecapitation(NiNode* current, bool is_recursing) {
   //
   // Collect (current) and all of its descendants into a static array.
   //
   if (!is_recursing) {
      HeadNodeListForDecapitation::get().clear();
   }
   if (!current || !current->GetAsNiNode())
      return;
   HeadNodeListForDecapitation::get().push_back(current);
   for(uint16_t i = 0; i < current->m_children.size; ++i) // recurse on all descendants
      this->sub006ADB10(current->m_children[i], true);
}
void Actor::Subroutine006DA760(Arg1, Arg2, NiNode* Arg3, bool Arg4) {
   auto ebp = this;
   auto edi = this->GetNiNode(); // direct call to virtual 0x70?
   auto esi = 0;
   esp18 = edi;
   NiNode* esp10;
   if (Arg3)
      esp10 = Arg3->m_parent;
   else
      esp10 = nullptr;
   BGSBodyPartData* bodyPartData; // esp0C
   auto eax = *ptrPlayerBodyPartData;
   if (eax) {
      if (this == *(Actor**)0x01310588) { // is player
         bodyPartData = eax;
      }
   }
   ecx = this->race;
   if (bodyPartData) {
      if (ecx) {
         bodyPartData = ecx->GetBodyPartData();
      }
      if (!bodyPartData) {
         bodyPartData = *ptrDefaultBodyPartData;
         if (!bodyPartData)
            return;
      }
   }
   auto eax = bodyPartData->GetPart(Arg1); // at 0x006DA7DA
   if (!eax || !Arg2)
      return;
   // push ebx: all stack vars shifted
   BSFaceGenNiNode* ebx = this->GetFaceGenNiNode();
   esp18 = eax = 0;
   ecx = 0;
   if (0 >= edi->m_children.m_size) // at 0x006DA810
      return;
   do {
      esi = edi->m_children[esp18];
      if (!esi)
         continue;
      bool al = sub0046B140(esi);
      if (!al)
         continue;
      if (esi == esp14) // esp10 from before (push ebx)
         continue;
      bool al = this->NodeBoneListOverlapsWithHeadNodeListForDecapitation(esi, Arg2);
      if (!al)
         continue;
      if (Arg4) { // bool or uint8_t
         sub0054A5B0(Arg1, esi);
         eax = Arg1;
         if (eax) {
            esi->m_flags |= 1; // "culled"
            continue;
         }
         bodyPartData->sub0054AF50(Arg2, edi, 0, esi);
      } else {
         auto esp20 = NiCloningProcess(1.0F);
         if (esi == ebx)
            edi = esi;
         else {
            edi = esi->Clone(&esp20);
         }
         bool cl = true;
         if (esi != ebx) {
            this->sub006AF880(esi, 1, 0); // related to BSDismemberSkinInstance
            cl = esi == ebx;
         }
         this->sub006AF880(edi, 0, cl); // at 0x006DA8EB // related to BSDismemberSkinInstance
         esp14->AttachChild(edi, true);
         auto eax = Arg3->Unk_03(); // one of the args
         sub0046CDB0(eax, edi, 0, 0);
         esp44.sub00CB55B0(); // destructor
         esp24.sub00CB55B0(); // destructor
      }
   } while (++esp18 < edi->m_children.m_size);
}

bool Actor::NodeBoneListOverlapsWithHeadNodeListForDecapitation(NiNode* Arg1, Arg2) {
   if (!Arg1)
      return false;
   auto geo = Arg1->GetAsNiGeometry();
   if (!geo) {
      auto edi = Arg1->GetAsNiNode();
      if (!edi)
         return false;
      auto eax = Arg1->Unk_05(); // virtual cast
      if (eax)
         return false;
      esi = 0;
      if (0 < edi->m_children.m_size) {
         do {
            if (this->NodeBoneListOverlapsWithHeadNodeListForDecapitation(edi->m_children[esi], Arg2))
               return true;
         } while (++esi < edi->m_children.m_size);
      }
      return false;
   }
   auto eax = Arg1->GetAsNiGeometry(); // redundant
   if (!eax)
      return false;
   NiSkinInstance* esi = Arg1->m_spSkinInstance;
   if (!esi)
      return false;
   auto ebx = esi->m_spSkinData->unk44; // number of bones
   auto esi = esi->m_ppkBones; // array of bones, each a NiAVObject*
   size_t ecx = 0;
   if (ebx <= 0)
      return false;
   size_t edi = HeadNodeListForDecapitation::get().count;
   do {
      size_t eax = 0;
      if (edi <= 0)
         continue;
      NiAVObject* edx = esi[ecx];
      do {
         if (HeadNodeListForDecapitation::get().nodes[eax] == edx)
            return true;
      } while (++eax < edi);
   } while (++ecx < ebx);
   return false;
}

void Actor::Subroutine006DA980(bool Arg1) {
   auto ebp   = this;
   auto esp20 = this;
   if (INI:General:bDisableAllGore != false)
      return;
   auto edi   = this->GetNiNode();
   auto esp28 = this->parentCell->sub004C5430(); // bhkWorld or bhkWorldM pointer
   NiNode* ebx = this->processManager->GetBodyPartNode(kPartType_Head);
   auto esp38 = StringCache::Ref("DismemberedLimb1 Head1");
   auto esi   = 0;
   this->PrepareHeadNodeListForDecapitation(ebx, false);
   ebp = esp1C = new NiNode(0);
   ebp->SetName(&esp38);
   edi->AttachChild(ebp, true);
   ecx = *(0x01B91280); // the DWORD before NiRTTI_NiObjectNET
   dl = *(uint8_t*)0x012C5970; // compile-time constant: 0x24
   //
   // ... constructing one or more objects on the stack ...
   //
   ebp = ebx->Clone(&esp4C);
   const char* bone = NiBoneNames::GetSingleton()->spine2; // "NPC Spine2 [Spn2]"
   eax = esp20 = GetNodeByName(edi, bone, 1);
   if (eax) { // at 0x006DAAD7
      auto esi = new NiNode(0);
      NiPoint3 esp10 = NiPoint3(0, 0, 0);
      NiNode* ecx = esp20;
      eax = ebx;
      if (ebx != esp20) {
         do {
            esp10 += eax->m_localTransform.pos;
            eax = eax->m_parent;
         } while (eax != esp20);
      }
      // esp10 == position of head node relative to spine2 node
      esi->SetName(NiBoneNames::GetSingleton()->spine2);
      NiPoint3 esp20 = -esp10;
      esi->m_localTransform.pos = esp20;
      eax->AttachChild(esi, true);
   }
   edi->m_worldTransform.sub006A67E0(&espA0); // at 0x006DABD5
   eax = espA0.sub004EC9C0(&espD4, &ebx->m_worldTransform.rot);
   edx = eax->unk28;
   memcpy(&espA0, eax, 9 * sizeof(float));
   espC4 = eax->unk24;
   espD0 = eax->unk30;
   ebp->unk44 = eax->unk24;
   double a = espD0;
   ebp->unk4C = eax->unk2C;
   espCC = eax->unk2C;
   espC8 = eax->unk28;
   ebp->unk48 = eax->unk28;
   memcpy(&ebp->unk20, &espA0, 9 * sizeof(float));
   float esp24 = sub00F531CC(a);
   esp24 = esp24; // 'kay
   ebp->unk50 = esp24;
   NiNode* eax = ebp->GetAsNiNode();
   edi = this;
   this->processManager->SetBodyPartNode(kPartType_Head, eax);
   if (ebx->unk1C && ebx->m_children.m_size) { // at 0x006DAC96
      eax = 0;
      ecx = 0;
      esp20 = 0; // loop index
      if (0 < ebx->m_children.m_emptyRunStart) {
         do {
            auto esi = ebx->m_children[esp20];
            if (!esi)
               continue;
            //
            // Recursively modify collision data in the node tree:
            //
            sub00D10C30(esi, 1, 1);
            sub00D11030(esi, 1, 1, 1);
         } while (++esp20 < ebx->m_children.m_emptyRunStart);
      }
   }
   bhkCollisionObject* eax = sub00588E20(ebp);
   if (eax) {
      bhkRigidBody* eax = eax->unk10;
      if (eax)
         eax->sub00D37010();
   }
   auto esi = esp1C;
   esi->AttachChild(ebp, 1);
   sub00D171D0(ebp, 1);
   esp2C.unk00 = 0;
   esp2C.unk04 = 0;
   esi->UpdateNode(&esp2C);
   sub00D14890(ebp, 0x20, 1);
   SetNodeMotionType(ebp, 1, true, true, true);
   sub00588D80(ebp, &ZeroNiPoint3, 1);
   esp28->Unk_31(ebp, 1, 0, 0, 1);
   sub006B06E0(ebp, &esp37);
   this->MarkChanged(4);
   ebx->m_flags |= 1; // "culled" flag
   if (this->processManager->TestEquipFlags(1)) { // at 0x006DADA7
      this->processManager->UpdateEquipment(this);
   }
   this->sub006DA760(1, ebx, ebp, esi);
   if (Arg1 == false) {
      if (ebx->unk18 != esi) { // at 0x006DADD2
         //
         // ...
         //
      }
   }
   eax = this->race;
   if (eax != esi)
      ebx = eax->unk1DC;
   else
      ebx = 0;
   if (ebx != esi) { // at 0x006DAF40
      //
      // ...
      //
   }
   // destructor: esp70: sub00CB55B0; 0x1C <= sizeof <= 0x20
   // destructor: esp50: sub00CB55B0; 0x1C <= sizeof <= 0x20
   // destructor: esp38: StringCache::Ref::Release
}