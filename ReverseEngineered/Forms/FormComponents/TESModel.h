#pragma once
#include "skse/GameFormComponents.h"

namespace RE {
   class TESModel : public BaseFormComponent { // sizeof == 0x14
      public:
         virtual const char*	GetModelName(void);
         virtual void			SetModelName(const char * name);
         virtual UInt32			Unk_06(void);

         enum FacegenModelFlags : UInt8 { // assuming consistency with FO3
            kFacegenModelFlag_Head      = 1,
            kFacegenModelFlag_Torso     = 2,
            kFacegenModelFlag_RightHand = 4,
            kFacegenModelFlag_LeftHand  = 8,
         };

         StringCache::Ref name;	// 04
         UInt32 unk08;	// 08
         UInt32 unk0C;	// 0C
         UInt16 unk10;	// 10
         UInt8  unk12;	// 12
         UInt8  facegenModelFlags; // 13 // MODD/MOSD
   };
   class TESModelTextureSwap : public TESModel { // sizeof == 0x1C
      public:
         struct SwapInfo { // sizeof == 0xC
            BGSTextureSet* textureSet;	// 00
            UInt32			unk04;			// 04 - index?
            BSFixedString	name;			// 08
         };
         SwapInfo* swaps;	// 14
         UInt32 count;		// 18
   };
   class TESModelRDT : public TESModel { // sizeof == 0x14
   };
};
