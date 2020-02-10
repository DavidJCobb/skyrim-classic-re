#pragma once
#include "objects.h"

namespace RE {
   class BSShaderProperty : public NiObjectNET {
      public:
         static constexpr uint32_t vtbl = 0x0114EB18;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA743C;
         //
      public:
         virtual UInt32 Unk_21(); // always returns 1 // not overridden by BSLightingShaderProperty
         virtual void   Unk_22(UInt32);
         virtual bool   Unk_23(UInt32); // always returns true
         virtual void   Unk_24(UInt32, UInt32, UInt32) = 0;
         virtual UInt32 Unk_25(UInt32, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_26(UInt32, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_27(UInt32); // always returns 1
         virtual UInt32 Unk_28(UInt32); // always returns 0
         virtual bool   Unk_29(UInt32); // most likely an operator== overload
         virtual void   Unk_2A(UInt32); // no-op
         virtual float  Unk_2B(); // always returns 1.0F on this class; on BSLightingShaderProperty, returns this->unk3C->unk54
         virtual UInt32 Unk_2C(UInt32); // always returns 1
         virtual void   Unk_2D();
         virtual UInt32 Unk_2E(); // always returns 0 on this class; on BSLightingShaderProperty, always returns 0xB
         virtual UInt32 Unk_2F(UInt32, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_30(); // always returns 0 on this class; on BSLightingShaderProperty, returns this->unk3C->unk3C
         virtual UInt32 Unk_31(UInt32); // always returns 0
         virtual bool   Unk_32(); // always returns false
         virtual void   Unk_33(); // no-op
         virtual void   Unk_34(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32); // no-op
         virtual UInt32 Unk_35(); // always returns 0
         virtual UInt32 Unk_36(); // always returns 0 on this class; on BSLightingShaderProperty, always returns 2
         //
         float  unk18 = 1.0F;
         UInt32 unk1C = 0x7FFFFFFF;
         UInt32 unk20 = 0;
         UInt32 unk24 = 0;
         UInt32 unk28 = 0;
         UInt32 unk2C = 0;
         BSFadeNode* esp30; // set by the constructor to some unknown node at a static pointer
         UInt32 unk34 = 0;
         UInt32 unk38 = 0;
         void*  unk3C = nullptr;
         UInt32 unk40 = 0;
         UInt32 unk44;
         //
         MEMBER_FN_PREFIX(BSShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSShaderProperty&, 0x00C6F9F0);
   };
   //
   class BSLightingShaderProperty : public BSShaderProperty { // sizeof == 0xB0
      public:
         static constexpr uint32_t vtbl = 0x01153680;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA82E0;
         //
      public:
         // This class does not define new virtual member functions, but does override some of them.
         //
         UInt32 unk48 = 0;
         UInt32 unk4C = 0;
         UInt32 unk50 = 0;
         UInt32 unk54 = 0;
         UInt32 unk58 = 0;
         UInt32 unk5C;
         float  unk60 = 1.0F;
         UInt32 unk64 = 0;
         float  unk68 = 1.0F;
         float  unk6C = 1.0F;
         UInt32 unk70 = 0;
         UInt32 unk74 = 0;
         UInt32 unk78 = 0;
         UInt32 unk7C = 0;
         float  unk80;
         float  unk84;
         float  unk88;
         float  unk8C = 1.0F;
         SInt32 unk90 = -1;
         UInt16 unk94 = 0xFFFF;
         UInt8  unk96 = 1;
         UInt8  unk97 = 2;
         tArray<UInt32> unk98; // unknown type
         UInt32 unkA4;
         UInt32 unkA8;
         UInt32 unkAC;
         //
         MEMBER_FN_PREFIX(BSLightingShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSLightingShaderProperty&, 0x00C8C350);
         DEFINE_MEMBER_FN(Subroutine004611B0, void, 0x004611B0, UInt32);
         DEFINE_MEMBER_FN(Subroutine00C6EEA0, void, 0x00C6EEA0, UInt32, UInt32);
         DEFINE_MEMBER_FN(Subroutine00C89DD0, void, 0x00C89DD0, UInt32); // Unk_23 wraps this
   };
   static_assert(sizeof(BSLightingShaderProperty) >= 0xB0, "RE::BSLightingShaderProperty is too small!");
   static_assert(sizeof(BSLightingShaderProperty) <= 0xB0, "RE::BSLightingShaderProperty is too large!");
   //
   class BSGrassShaderProperty : public BSLightingShaderProperty { // sizeof == 0xE0
      public:
         static constexpr uint32_t vtbl = 0x011538A8;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA8304;
         //
      public:
         virtual void* GetUnkB4();       // 37 // returns this->unkB4
         virtual void  SetUnkB4(void*); // 38 // sets this->unkB4
         //
         UInt32 unkB0 = 0;
         void*  unkB4; // warning: this is not initialized by the constructor!
         UInt32 unkB8;
         UInt32 unkBC = 0;
         tArray<UInt32> unkC0; // type unknown
         UInt32 unkCC;
         UInt32 unkD0;
         UInt32 unkD4;
         UInt32 unkD8 = 0;
         UInt32 unkDC;
         //
         MEMBER_FN_PREFIX(BSGrassShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSGrassShaderProperty&, 0x00C8CB10);
         DEFINE_MEMBER_FN(Destructor, void, 0x00C8CAB0);
   };
   static_assert(sizeof(BSGrassShaderProperty) >= 0xE0, "RE::BSGrassShaderProperty is too small!");
   static_assert(sizeof(BSGrassShaderProperty) <= 0xE0, "RE::BSGrassShaderProperty is too large!");
}