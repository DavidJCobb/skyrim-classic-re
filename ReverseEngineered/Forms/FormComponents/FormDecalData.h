#pragma once
#include "skse/GameTypes.h"

namespace RE {
   class FormDecalData { // DODT
      public:
         struct flags {
            flags() = delete;
            enum type : UInt8 {
               parallax       = 0x01,
               alpha_blending = 0x02,
               alpha_testing  = 0x04,
               no_subtextures = 0x08,
            };
         };
         using flags_type = std::underlying_type_t<flags>;
         //
         float  widthMin;       // 00
         float  widthMax;       // 04
         float  heightMin;      // 08
         float  heightMax;      // 0C
         float  depth;          // 10 // a.k.a. max range
         float  shininess;      // 14
         float  parallaxScale;  // 18
         UInt8  parallaxPasses; // 1C
         UInt8  flags;          // 1D
         UInt16 pad1E;
         UInt32 colors;         // 20
   };
}