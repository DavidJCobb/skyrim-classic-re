#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace;
   class TESObjectARMA : public ::TESObject {
      public:
	      enum { kTypeID = kFormType_ARMA };
         static constexpr intptr_t vtbl = 0x0108450C;
         //
	      // Parents:
         //
	      TESRaceForm			race;	// 14
	      BGSBipedObjectForm	biped;	// 1C
         //
	      // Members:
         //
	      struct Data { // sizeof == 0xC
		      UInt8	priority[2]; // 00 // broken? one per sex but the game loads the male value to both? see 004A0EF0
		      UInt8	unk02; // 02 // subrecord byte 01: female priority?
		      UInt8	unk03; // 03 // subrecord byte 02: weight slider flags, male
		      UInt8	unk04; // 04 // subrecord byte 03: weight slider flags, female
		      UInt8	unk05; // 05 // subrecord byte 04
		      UInt8	detectionSoundValue; // 06 // subrecord byte 05
		      UInt8	pad07; // 07
            float weaponAdjust; // 08 // subrecord dword 02 (bytes 08...0C)
	      };
         //
	      Data data; // 28 // DATA
	      TESModelTextureSwap models[2][2]; // 34
	      void* skinTexture[2]; // A4 // NAM0, NAM1
	      void* skinTextureSwapList[2]; // AC // NAM2, NAM3
	      tArray<TESRace*> additionalRaces; // B4
	      BGSFootstepSet* footstepSet; // C0
	      UInt32 unkC4; // C4

         MEMBER_FN_PREFIX(TESObjectARMA);
         DEFINE_MEMBER_FN(Constructor, TESObjectARMA&, 0x004A1250);
         DEFINE_MEMBER_FN(RaceIsCompatible,   bool,   0x004A11B0, TESRace* race);
         DEFINE_MEMBER_FN(Subroutine004A0F70, UInt32, 0x004A0F70, bool index);
         DEFINE_MEMBER_FN(Subroutine004A1BB0, void,   0x004A1BB0, TESObjectARMO* armor, ActorWeightModel*, bool isFemale);
   };
}