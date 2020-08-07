#pragma once
#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/Shared.h"
#include "skse/Utilities.h"

namespace RE {
   class NiDefaultAVObjectPalette;
   
   class NiTimeController : public NiObject { // sizeof == 0x34
      public:
         NiTimeController();
         ~NiTimeController();
         //
         virtual void Start(float time); // 21 // just starts the current controller; doesn't traverse the list. apparently only called by a containing NiControllerManager anyway.
         virtual void Stop(); // 22 // likely only called by a containing NiControllerManager
         virtual void Update(const float& time) = 0; // 23 // actually, the argument is actually a NiAVObject::ControllerUpdateContext&
         virtual void SetTarget(NiObjectNET * node); // 24
         virtual bool Unk_25(); // 25
         virtual bool Unk_26(void);
         virtual void Unk_27(float);
         virtual bool Unk_28(void);
         virtual bool Unk_29();
         virtual bool Unk_2A() = 0;
         //
         enum {
            kAnimType_AppTime = 0 << 0,
            kAnimType_AppInit = 1 << 0,
            //
            kCycleType_Loop = 0 << 1,
            kCycleType_Reverse = 1 << 1,
            kCycleType_Clamp = 2 << 1,
            //
            kActive = 1 << 3,
            //
            kPlayBackwards = 1 << 4,
         };
         //
         UInt16       flags;        // 008
         UInt8        pad00A[2];    // 00A
         float        frequency; // 00C
         float        phase;     // 010
         float        lowKeyTime; // 014
         float        highKeyTime; // 018
         float        startTime; // 01C
         NiObjectNET* owner; // 20
         float        unk024;       // 024 - updated in Unk_27
         float        unk028;       // 028
         UInt8        unk02C;       // 02C
         UInt8        pad02D[3];    // 02D
         NiPointer<NiTimeController> next; // 30 // singly linked list of controllers
         //
         MEMBER_FN_PREFIX(NiTimeController);
         DEFINE_MEMBER_FN(Constructor, NiTimeController&, 0x00ABFCB0);
   };

   class NiControllerSequence : public NiObject { // sizeof == 0x74
      public:
         static constexpr uint32_t vtbl = 0x0110F31C;
         //
         char*    unk08 = nullptr; // name?
         uint32_t unk0C = 0;
         uint32_t unk10 = 0;
         uint32_t unk14 = 0;
         uint32_t unk18 = 0;
         float    unk1C = 1.0F;
         uint32_t unk20 = 0;
         uint32_t unk24 = 0;
         float    unk28 = 1.0F;
         float    unk2C = FLT_MAX;
         float    unk30 = -FLT_MAX;
         float    unk34 = -FLT_MAX;
         float    unk38 = -FLT_MAX;
         float    unk3C = -FLT_MAX;
         uint32_t unk40 = 0;
         uint32_t unk44 = 0;
         float    unk48 = -FLT_MAX;
         float    unk4C = -FLT_MAX;
         float    unk50 = -FLT_MAX;
         float    unk54 = -FLT_MAX;
         uint32_t unk58 = 0;
         uint32_t unk5C = 0;
         uint32_t unk60 = 0;
         uint32_t unk64 = 0;
         uint16_t unk68 = 0xFFFF;
         uint16_t unk6A;
         uint32_t unk6C = 0;
         uint16_t unk70 = 0;
         uint8_t  unk72 = 0;
         uint8_t  pad73;
         //
         MEMBER_FN_PREFIX(NiControllerSequence);
         DEFINE_MEMBER_FN(Constructor, NiControllerSequence&, 0x00A6FD20);
         DEFINE_MEMBER_FN(Subroutine00A72930, void, 0x00A72930, UInt32, UInt32, float, float, void*, UInt8); // TODO: verify that this is a NiControllerSequence member function
   };
   class NiControllerManager : public NiTimeController { // sizeof == 0x88
      public:
         static constexpr uint32_t vtbl = 0x0110EF9C;
         //
         NiTArray<NiPointer<NiControllerSequence>> sequences; // 34 // actually NiTObjectArray<NiPointer<NiControllerSequence>>
         uint32_t unk44 = 0;
         uint32_t unk48 = 0;
         uint32_t unk4C = 0;
         BSTHashMap<uint32_t, uint32_t> unk50;
         void*    unk70; // something to do with anim note listeners
         uint8_t  unk74 = 0;
         uint8_t  pad75[3];
         uint32_t unk78 = 0;
         uint32_t unk7C = 0;
         uint32_t unk80 = 0;
         NiDefaultAVObjectPalette* unk84 = nullptr; // 84

         MEMBER_FN_PREFIX(NiControllerManager);
         DEFINE_MEMBER_FN(Constructor,  NiControllerManager&, 0x00A6DB10);
         DEFINE_MEMBER_FN(Constructor2, NiControllerManager&, 0x00A6D9F0, NiDefaultAVObjectPalette*, uint32_t);
         DEFINE_MEMBER_FN(Subroutine0x004D6800, void, 0x004D6800, bool); // <-- Calls {this->unk44[i]->Unk_21(0, arg);} for every i
   };

   class BSSimpleScaleController : public NiTimeController { // sizeof == 0x40
      public:
         static constexpr uint32_t vtbl = 0x010D1494;
         //
         uint32_t unk34 = 0;
         uint32_t unk38 = 0;
         uint32_t unk3C = 0;
         //
         MEMBER_FN_PREFIX(BSSimpleScaleController);
         DEFINE_MEMBER_FN(Constructor, BSSimpleScaleController&, 0x0072FC80);
   };

   class NiInterpController : public NiTimeController {
      public:
         static constexpr uint32_t vtbl = 0x0110F5D4;
         //
         MEMBER_FN_PREFIX(NiTimeController);
         DEFINE_MEMBER_FN(Constructor, NiTimeController&, 0x00A76240);
   };
   class NiSingleInterpController : public NiInterpController { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x011102C4;
         //
         uint32_t unk34 = 0;
         //
         MEMBER_FN_PREFIX(NiSingleInterpController);
         DEFINE_MEMBER_FN(Constructor, NiSingleInterpController&, 0x00A7D400);
   };
   class NiFloatInterpController : public NiSingleInterpController { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x01116C9C;
         //
         MEMBER_FN_PREFIX(NiFloatInterpController);
         DEFINE_MEMBER_FN(Constructor, NiFloatInterpController&, 0x00AA9E50);
   };
   class NiPoint3InterpController : public NiSingleInterpController { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x01116ED4;
         MEMBER_FN_PREFIX(NiPoint3InterpController);
         DEFINE_MEMBER_FN(Constructor, NiPoint3InterpController&, 0x00AAA590);
   };
   class NiTransformController : public NiSingleInterpController { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x011117FC;
         MEMBER_FN_PREFIX(NiTransformController);
         ERASE_MEMBER_FN(Constructor, NiTransformController&);
   };

   class NiPosData : public NiObject { // sizeof == 0x18
      public:
         static constexpr uint32_t vtbl = 0x0110FF5C;
         //
         uint32_t unk08 = 0; // count?
         uint32_t unk0C = 0; // enum?
         uint32_t unk10 = 0; // enum
         uint8_t  unk14 = 0; // enum?
         uint8_t  pad15[3];
   };

   class BSLightingShaderPropertyFloatController : public NiFloatInterpController { // sizeof == 0x3C
      public:
         static constexpr uint32_t vtbl = 0x01157738;
         //
         uint32_t unk38;
         //
         MEMBER_FN_PREFIX(BSLightingShaderPropertyFloatController);
         ERASE_MEMBER_FN(Constructor, BSLightingShaderPropertyFloatController&);
   };
   class BSFrustumFOVController : public NiFloatInterpController { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x01112294;
         MEMBER_FN_PREFIX(BSFrustumFOVController);
         DEFINE_MEMBER_FN(Constructor, BSFrustumFOVController&, 0x00A8AE80);
   };

   class NiLightColorController : public NiPoint3InterpController { // sizeof == 0x3C
      public:
         static constexpr uint32_t vtbl = 0x01113F64;
         //
         uint16_t unk38 = 0;
         uint16_t pad3A;
         //
         MEMBER_FN_PREFIX(NiLightColorController);
         DEFINE_MEMBER_FN(Constructor, NiLightColorController&, 0x00A945B0);
   };
   class BSEffectShaderPropertyColorController : public NiPoint3InterpController { // sizeof == 0x3C
      public:
         static constexpr uint32_t vtbl = 0x01157390;
         enum class color_t : uint32_t {
            emissive = 0,
         };
         //
         color_t color; // 38 // assumed
         //
         MEMBER_FN_PREFIX(BSEffectShaderPropertyColorController);
         ERASE_MEMBER_FN(Constructor, BSEffectShaderPropertyColorController&);
   };
   class BSLightingShaderPropertyColorController : public NiPoint3InterpController { // sizeof == 0x3C
      public:
         static constexpr uint32_t vtbl = 0x01157568;
         enum class color_t : uint32_t {
            specular = 0,
            emissive = 1,
         };
         //
         color_t color; // 38 // assumed
         //
         MEMBER_FN_PREFIX(BSLightingShaderPropertyColorController);
         ERASE_MEMBER_FN(Constructor, BSLightingShaderPropertyColorController&);
   };
};