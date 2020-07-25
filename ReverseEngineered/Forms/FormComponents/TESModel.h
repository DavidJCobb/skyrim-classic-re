#pragma once
#include "skse/GameFormComponents.h"
#include "ReverseEngineered/Shared.h"

class NiNode;
namespace RE {
   class BGSLoadFormBuffer;

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
         void*  unk08;	// 08 // array content for unk10; sizeof(item) == 4
         void*  unk0C;	// 0C // array content for unk12; sizeof(item) == 4
         UInt16 unk10;	// 10 // array length (unk10)
         UInt8  unk12;	// 12 // array length (unk0C)
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

         MEMBER_FN_PREFIX(TESModelTextureSwap);
         DEFINE_MEMBER_FN(Subroutine004557B0, void, 0x004557B0, NiNode*); // apply the texture swaps to a model?
   };
   class TESModelRDT : public TESModel { // sizeof == 0x14
   };

   DEFINE_SUBROUTINE_EXTERN(void, LoadMODTSubrecord, 0x00454AF0, TESModel*, BGSLoadFormBuffer*);
};
