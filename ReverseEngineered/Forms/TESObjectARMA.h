#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace;
   class TESObjectARMA : public ::TESObjectARMA {
      public:
         MEMBER_FN_PREFIX(TESObjectARMA);
         DEFINE_MEMBER_FN(RaceIsCompatible,   bool,   0x004A11B0, TESRace* race);
         DEFINE_MEMBER_FN(Subroutine004A0F70, UInt32, 0x004A0F70, bool index);
         DEFINE_MEMBER_FN(Subroutine004A1BB0, void,   0x004A1BB0, TESObjectARMO* armor, ActorWeightModel*, bool isFemale);
   };
}