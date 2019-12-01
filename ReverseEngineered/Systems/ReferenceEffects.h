#pragma once
#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class ShaderReferenceEffect : public NiObject {
      public:
         static constexpr intptr_t vtbl = 0x010C9F14;
         // virtual methods seen up to 0x38
         //
         UInt32 unk00[(0x1C - 0x00) / sizeof(UInt32)];
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