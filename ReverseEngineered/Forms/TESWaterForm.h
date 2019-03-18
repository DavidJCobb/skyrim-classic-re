#pragma once
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Shared.h"
#include "skse/Utilities.h"

namespace RE {
   class BGSSoundDescriptorForm;
   class TESObjectACTI;
   class WaterMaterialData; // subclass NiRefObject
   //
   class TESWaterForm : public TESForm { // sizeof == 0x1D34
      public:
         enum { kTypeID = kFormType_Water };

         // parents
         TESFullName			fullName;		// 14
         TESAttackDamageForm	attackDamage;	// 1C

         enum WaterFlags : UInt8 {
            kWaterFlag_CausesDamage = 1,
            kWaterFlag_04 = 4,
         };

         struct Data2C { // sizeof == 0x10
            UInt32	unk0;	// 0
            UInt32	unk4;	// 4
            UInt32	unk8;	// 8
            UInt32	unkC;	// C
         };
         struct DNAM { // sizeof == 0xE4
            float	unk00;		// 00 - init'd to .1
            float	unk04;		// 04 - init'd to 90
            float	unk08;		// 08 - init'd to .5
            float	unk0C;		// 0C - init'd to 1
            float	sunSpecularPower;		// 10 - init'd to 50
            float	reflectivity;		// 14 - init'd to .5
            float	fresnel;		// 18 - init'd to .025
            UInt32	unk1C;		// 1C - init'd to 0
            UInt32	unk20;		// 20 - init'd to 0
            UInt32	unk24;		// 24 - init'd to 0
            UInt32	colorShallow;		// 28 - init'd to 00808000
            UInt32	colorDeep;		// 2C - init'd to 00190000
            UInt32	colorReflection;		// 30 - init'd to 00FFFFFF
            UInt8	unk34;		// 34 - init'd to 0
            UInt8	pad35[3];	// 35
            float	unk38;		// 38 - init'd to 0.1
            float	unk3C;		// 3C - init'd to 0.6
            float	unk40;		// 40 - init'd to 0.985
            float	unk44;		// 44 - init'd to 2
            float	displacementStartSize;		// 48 - init'd to .01
            float	displacementForce;		// 4C - init'd to .4
            float	displacementVelocity;		// 50 - init'd to .6
            float	displacementFalloff;		// 54 - init'd to .985
            float	displacementDamper;		// 58 - init'd to 10
            float	unk5C;		// 5C - init'd to .05
            float	noiseFalloff;		// 60 - init'd to 300
            UInt32	unk64;		// 64 - init'd to 0
            UInt32	unk68;		// 68 - init'd to 0
            UInt32	unk6C;		// 6C - init'd to 0
            UInt32	unk70;		// 70 - init'd to 0
            UInt32	unk74;		// 74 - init'd to 0
            UInt32	unk78;		// 78 - init'd to 0
            float	unk7C;		// 7C - init'd to 300
            float	unk80;		// 80 - init'd to 300
            float	unk84;		// 84 - init'd to 1
            UInt32	unk88;		// 88 - not init'd
            float	unk8C;		// 8C - init'd to 1
            UInt32	unk90;		// 90 - init'd to 0
            float	unk94;		// 94 - init'd to 1000
            float	unk98;		// 98 - init'd to 250
            float	unk9C;		// 9C - init'd to 100
            float	unkA0;		// A0 - init'd to 1
            float	unkA4;		// A4 - init'd to 10000
            float	unkA8;		// A8 - init'd to 1
            float	unkAC;		// AC - init'd to 100
            float	unkB0;		// B0 - init'd to 100
            float	unkB4;		// B4 - init'd to 100
            UInt32	unkB8;		// B8 - init'd to 0
            UInt32	unkBC;		// BC - init'd to 0
            UInt32	unkC0;		// C0 - init'd to 0
            float	unkC4;		// C4 - init'd to 1
            float	unkC8;		// C8 - init'd to 1
            float	unkCC;		// CC - init'd to 1
            float	unkD0;		// D0 - init'd to 1
            float	unkD4;		// D4 - init'd to 1
            float	unkD8;		// D8 - init'd to 1
            float	unkDC;		// DC - init'd to 1
            float	unkE0;		// E0 - init'd to 1
         };

         UInt8		unk024;		// 024
         UInt8		pad025[3];	// 025
         UInt32		unk028;		// 028
         Data2C		unk02C;		// 02C
         Data2C		unk03C;		// 03C
         Data2C		unk04C;		// 04C
         TESTexture	noiseLayer1; // 05C
         TESTexture	noiseLayer2; // 064
         TESTexture	noiseLayer3; // 06C
         UInt8		opacity; // 074 // ANAM
         UInt8		waterFlags; // 075 // FNAM
         UInt8		pad076[2];	// 076
         UInt32 material;   // 078 // TNAM
         BGSSoundDescriptorForm* openSound;  // 07C // SNAM // despite the name, this is actually the loop for the underwater sound
         DNAM   visualData; // 080 // DNAM
         UInt32		unk164[3];	// 164 // GNAM
         UInt32		unk170;		// 170
         UInt32		unk174;		// 174
         UInt32		unk178;		// 178
         UInt32		unk17C;		// 17C
         UInt32		unk180;		// 180
         float		unk184;		// 184
         float		unk188;		// 188
         float		unk18C;		// 18C
         float		unk190;		// 190
         void*  spell; // 194 // XNAM
         UInt32		unk198;		// 198
         UInt32		unk19C;		// 19C
         UInt32		unk1A0;		// 1A0
         TESObjectACTI* unk1A4 = nullptr;		// 1A4
         UInt32		unk1A8;		// 1A8
         WaterMaterialData* unk1AC;	// 1AC - refcounted ptr
         UInt32		unk1B0;		// 1B0
         UInt8		unk1B4;		// 1B4
         UInt8		pad1B5[3];	// 1B5
         TESImageSpace* imagespace;		// 1B8 // INAM: imagespace applied when the camera is underwater // Update.esm sets this to [IMGS:01000946]UnderwaterImageSpace for most waters.
         NiPoint3 linearVelocity;  // 1BC // NAM0
         NiPoint3 angularVelocity; // 1C8 // NAM1

         MEMBER_FN_PREFIX(TESWaterForm);
         DEFINE_MEMBER_FN(GetOrCreateUnk1A4, TESObjectACTI*, 0x004ED8E0);
         DEFINE_MEMBER_FN(GetWaterFlag04, bool, 0x004ED300);
   };

   // possible sound teardown code at 00630C10

   struct Struct001B2DA70 { // not sure where this singleton begins or ends; it's named for its actual physical location in memory, not for a pointer to its location in memory
      BGSSoundDescriptorForm* waterSound; // only stored here to prevent something from running twice on the same sound; doesn't seem to DO anything otherwise
      UInt32 unk04;
      UInt32 unk08;
   };

   extern ExtraSound::Data* const waterSoundData_0126ED98;

   DEFINE_SUBROUTINE_EXTERN(void, Subroutine0062EEE0, 0x0062EEE0, TESObjectCELL* parent, NiNode*); // apparently creates a TESObjectREFR to serve as a cell's water
};
