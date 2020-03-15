#pragma once
#include "skse/GameForms.h"
#include "TESForm.h"

namespace RE {
   class TESLoadScreen : public TESForm { // sizeof == 0x20
      public:
         enum { kTypeID = kFormType_LoadScreen };
         static constexpr uint8_t form_type = kFormType_LoadScreen;

         struct ModelData { // sizeof == 0x34
            TESObjectSTAT* object = nullptr; // 00 // NNAM
            float  unk04 = 1.0F;
            UInt16 unk08 = 0;
            UInt16 unk0A = 0;
            UInt16 unk0C = 0;
            UInt16 unk0E = 0;
            SInt16 unk10 = 0;
            UInt16 pad12;
            NiPoint3 initialTranslation; // 14 // XNAM
            TESModel cameraPath; // 20

            MEMBER_FN_PREFIX(ModelData);
            DEFINE_MEMBER_FN(Constructor, ModelData&, 0x005137A0);
         };

         void*            conditions; // 14
         ModelData*       model;      // 18
         StringCache::Ref text;       // 1C

         MEMBER_FN_PREFIX(TESLoadScreen);
         DEFINE_MEMBER_FN(GetModel, TESModelTextureSwap*, 0x00513780);
         DEFINE_MEMBER_FN(HasNoConditions, bool, 0x00513810);
         DEFINE_MEMBER_FN(TestConditions,  bool, 0x00513800);
         DEFINE_MEMBER_FN(Subroutine00513820, void, 0x00513820, NiPoint3& out);
   };
}