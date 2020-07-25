#pragma once
#include "ReverseEngineered\Forms\TESForm.h"

namespace RE {
   class TESObjectSTAT : public TESBoundObject { // sizeof == 0x44
      public:
         enum { kTypeID = kFormType_Static };

         struct DNAM { // sizeof == 0x8
            float maxAngle; // 00
            void* material; // 04 // form of type MATO
         };

         // Parents:
         TESModelTextureSwap texSwap;	// 20

         // Members:
         DNAM directionalMaterial; // 3C
   };
};