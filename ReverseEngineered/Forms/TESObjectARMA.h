#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/FormComponents/TESModel.h"
#include "skse/GameObjects.h"

namespace RE {
   class TESRace;
   class TESObjectARMA : public TESObject {
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
				UInt8 weightSliderFlags[2]; // 02 // one per sex
		      UInt8	unk04; // 04 // subrecord byte 03
		      UInt8	unk05; // 05 // subrecord byte 04
		      UInt8	detectionSoundValue; // 06 // subrecord byte 05
		      UInt8	pad07; // 07
            float weaponAdjust; // 08 // subrecord dword 02 (bytes 08...0C)
	      };
         //
	      Data data; // 28 // DATA
         TESModelTextureSwap worldModels[2]; // 34, 50 // male, female
         TESModelTextureSwap firstPersonModels[2]; // 6C, 88 // male, female
	      void* skinTexture[2]; // A4 // NAM0, NAM1
	      void* skinTextureSwapList[2]; // AC // NAM2, NAM3
	      tArray<TESRace*> additionalRaces; // B4
	      BGSFootstepSet* footstepSet; // C0
	      TESForm* artObject; // C4 // ONAM

         MEMBER_FN_PREFIX(TESObjectARMA);
         DEFINE_MEMBER_FN(Constructor, TESObjectARMA&, 0x004A1250);
         DEFINE_MEMBER_FN(GetSkinTextureSwapListCount, UInt32, 0x004A0F70, UInt8 sex); // checks the initial form count of the texture swap list
         DEFINE_MEMBER_FN(RaceIsCompatible,   bool, 0x004A11B0, TESRace* race);
         DEFINE_MEMBER_FN(Subroutine004A1BB0, void, 0x004A1BB0, TESObjectARMO* armor, ActorWeightModel*, bool isFemale); // picks the appropriate texture swap from the skin texture swap list
   };
}