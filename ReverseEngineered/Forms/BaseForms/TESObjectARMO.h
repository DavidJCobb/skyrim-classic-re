#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace;
   class TESObjectARMO : public ::TESObjectARMO {
      public:
         MEMBER_FN_PREFIX(TESObjectARMO);
         DEFINE_MEMBER_FN(ApplyArmorAddon, bool, 0x004A21C0, TESRace* race, void** actorWeightModelData, bool isFemale);
   };
}