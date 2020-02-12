void TESObjectREFR::Subroutine004DD620() {
   //
   // This function is responsible for emitting the decal(s) for a placed decal emitter 
   // (a reference with a TextureSet base form) or a projectile. Its sole caller is 
   // ApplyAllDecalsRefFunctor::Unk_01(TESObjectREFR*).
   //
   BGSTextureSet* esp38;
   if (TESForm* eax = this->baseForm) {
      if (eax->formType == kFormType_TextureSet)
         esp38 = eax;
      else if (eax->formType == kFormType_Projectile)
         esp38 = eax->unk9C;
      else
         esp38 = nullptr;
   }
   if (!esp38)
      return;
   FormDecalData* ebp = eax->unk68; // decal data (DODT)
   if (!ebp)
      return;
   NiPoint3 esp74 = this->pos;
   NiMatrix33 esp40;
   this->GetRotationAsMatrix(&esp40);
   if (eax) {
      if (eax->formType == kFormType_TextureSet) { // at 0x004DD6CB
         esp14 = NiPoint3(esp40[0], esp40[3], esp40[6]); // leftmost column of matrix
         //
         // ...
         //
      } else if (eax->formType == kFormType_Projectile) { // at 0x004DD78F
         //
         // ...
         //
      }
   }
   // push esi; esp14 is now esp18; esp74 is now esp78; etc.
   bhkWorld* esi; // at 0x004DD872
   if (TESObjectCELL* ecx = this->parentCell)
      esi = ecx->sub004C5430();
   else
      esi = nullptr;
   tArray<?> esp30;
   NiPoint3 espAC = esp14; // now esp18
   sub00D15470(esi, &esp78, &espAC, &esp30); // raycast?
   if (esp30.count > 0) {
      DecalGroup* ebx = this->GetDecalGroup();
      if (!ebx) {
         ebx = new DecalGroup;
         this->extraData.SetDecalGroup(ebx);
      }
      float espA0 = GetRandomFloat(ebp->widthMin,  ebp->widthMax);
      float espA4 = GetRandomFloat(ebp->heightMin, ebp->heightMax);
      SInt32 esp13;
      if (ebp->flags & DecalData::flags::no_subtextures)
         esp13 = -1;
      else
         esp13 = GetRandomIntBetween(0, 4);
      esp3C = 0;
      if (esp30.count > 0) {
         NiPoint3 a = -esp18; // espA8:esp9C:esp98
         do {
            auto espB8 = Struct0045CDB0();
            espB8.unk6C = this->parentCell;
            espB8.position = esp78;
            espB8.unk0C    = espA8;
            espB8.unk95    = 1;
            espB8.unk14    = esp98;
            espB8.unk28    = esp30[esp3C]; // NiPointer assign, inlined
            espB8.unk30    = esp40; // at 0x004DDA7D
            espB8.depth    = ebp->depth;
            espB8.width    = espA0;
            espB8.height   = espA4;
            espB8.unk48    = esp44;
            espB8.subtextureIndex = esp13;
            espB8.parallax = 
            espB8.parallaxScale = 
            espB8.parallaxPasses = 
            espB8.unk7C = 90.0F;
            espB8.red   = 
            espB8.green = 
            espB8.blue  = 
            Singleton012E5B8C::get()->sub0045F1C0(&espB8, 1, ebx);
            // inlined destructor for espB8 and its smart pointer
         } while (++esp3C < esp30.count);
      }
   }
   // pop esi; esp30 is now esp2C
   if (esp2C.count) {
      esp2C.Free(); // tArray::Free
      esp2C.count = 0;
   }
}