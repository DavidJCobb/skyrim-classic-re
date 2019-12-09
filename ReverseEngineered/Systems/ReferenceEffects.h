#pragma once
#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class OwnedController;

   // NiRefObject > BSTempEffect > ReferenceEffect > ShaderReferenceEffect
   class ShaderReferenceEffect : public NiRefObject {
      public:
         static constexpr intptr_t vtbl = 0x010C9F14;
         // virtual methods seen up to 0x38
         //
         UInt32 unk08;
         OwnedController* unk0C;
         UInt32 unk10;
         UInt32 unk14;
         UInt32 id; // 18 // a counter is incremented for each new instance; its value gets written to the instance before increment
         // above: BSTempEffect
         void*  unk1C; // 1C // has a VTBL
         UInt32 refHandle; // 20
         UInt32 unk24;
         UInt8  unk28;
         UInt8  unk29; // pad
         UInt8  unk2A; // pad
         UInt8  unk2B; // pad
         UInt32 unk2C[(0x90 - 0x2C) / sizeof(UInt32)];
         TESEffectShader* shader; // 90
   };
}