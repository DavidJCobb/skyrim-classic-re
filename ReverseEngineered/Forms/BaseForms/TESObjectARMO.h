#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace;
   class TESObjectARMO : public ::TESObjectARMO {
      public:
         MEMBER_FN_PREFIX(TESObjectARMO);
         DEFINE_MEMBER_FN(ApplyArmorAddon,     bool, 0x004A21C0, TESRace* race, void** actorWeightModelData, bool isFemale);
         DEFINE_MEMBER_FN(TestBodyPartByIndex, bool, 0x004A1DB0, UInt32 index);
         //
         inline bool IsShield() {
            return (this->flags & 0x40); // "is shield" flag, according to xEdit source for ARMO record
         };
   };
}