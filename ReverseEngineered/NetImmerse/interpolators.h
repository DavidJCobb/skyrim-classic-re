#pragma once
#include "ReverseEngineered/NetImmerse/objects.h"
#include "skse/NiInterpolators.h"

namespace RE {
   class NiTransformInterpolator : public NiKeyBasedInterpolator { // sizeof == 0x38
      public:
         static constexpr uint32_t vtbl = 0x011101AC;
         //
         NiPoint3     translation = NiPoint3(-FLT_MAX, -FLT_MAX, -FLT_MAX); // 0C
         NiQuaternion rotation    = NiQuaternion{-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX}; // 18
         float        scale       = -FLT_MAX; // 28
         uint32_t unk2C; // constructor arg 1; possibly NiTransformData*
         uint16_t unk30 = 0;
         uint16_t unk32 = 0;
         uint8_t  unk34;
         uint8_t  unk35;
         uint8_t  unk36 = 0;
         uint8_t  unk37;
         //
         MEMBER_FN_PREFIX(NiTransformInterpolator);
         DEFINE_MEMBER_FN(Constructor, NiTransformInterpolator&, 0x00A7C940, uint32_t);
   };
}