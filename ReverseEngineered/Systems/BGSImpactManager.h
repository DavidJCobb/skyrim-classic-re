#pragma once
#include "skse/Utilities.h"

namespace RE {
   class  BGSImpactManager;
   class  TESObjectREFR;
   extern BGSImpactManager* ptrBGSImpactManager;
   //
   class BGSImpactManager {
      public:
         static BGSImpactManager* GetInstance() {
            return *(BGSImpactManager**)ptrBGSImpactManager;
         };

         MEMBER_FN_PREFIX(BGSImpactManager);
         DEFINE_MEMBER_FN(KnockAreaEffect, void, 0x0068BF90, TESObjectREFR* origin, float magnitude, float radius);
   };
};
