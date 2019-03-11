#pragma once
#include "ReverseEngineered/Types.h"

namespace RE {
   struct TESFile { // sizeof == 0x434
      //
      // No VTBL. Name shows up in RTTIs of templated structs that use 
      // this as a typename.
      //
      UInt32 unk000 = 0;
      UInt32 unk004 = 0;
      UInt32 unk008 = 0;
      void*  unk00C = nullptr; // 00C // unknown, but has virtual methods
      void*  unk010 = nullptr; // 010 // unknown, but has virtual methods
      UInt32 unk014;
      UInt32 unk018;
      UInt32 unk01C;
      UInt32 unk020; // 020
      UInt32 unk024[(0x124 - 0x024) / 4];
      UInt32 unk124; // 124
      UInt32 unk128[(0x228 - 0x128) / 4];
      UInt32 unk228 = 0;
      UInt32 unk22C = 0x10000; // 22C // constructor arg
      UInt32 unk230 = 0;
      UInt32 unk234 = 0;
      UInt32 unk238 = 0;
      UInt32 unk23C = 0;
      UInt32 unk240 = 0;
      UInt32 unk244 = 0;
      UInt32 unk248 = 0;
      UInt32 unk24C = 0;
      UInt32 unk250 = 0;
      UInt32 unk254 = 0;
      UInt32 unk258 = 0;
      UInt32 unk25C = 0;
      UInt32 unk260 = 0;
      UInt32 unk264 = 0;
      UInt32 unk268 = 0;
      UInt32 unk26C = 0;
      UInt32 unk270[(0x288 - 0x270) / 4];
      UInt32 unk288 = 0;
      UInt32 unk28C = 0;
      UInt32 unk290 = 0;
      UInt32 unk294 = 0;
      UInt32 unk298;
      UInt32 unk29C[0x50]; // 29C // set to 0 via memset
      float  unk3DC = 1.7F; // 3DC
      UInt32 unk3E0 = 0;
      UInt32 unk3E4 = 0x800; // 3E4
      UInt32 unk3E8 = 0;
      UInt32 unk3EC = 0;
      UInt32 unk3F0 = 0;
      UInt32 unk3F4 = 0;
      UInt32 unk3F8 = 0;
      UInt32 unk3FC = 0;
      UInt32 unk400 = 0;
      UInt32 unk404 = 0;
      UInt32 unk408 = 0;
      UInt8  unk40C = 0xFF;
      UInt8  unk40D;
      UInt8  unk40E;
      UInt8  unk40F;
      BSString unk410; // possibly BSStringT
      BSString unk418; // possibly BSStringT
      UInt32 unk420 = 0;
      UInt32 unk424 = 0;
      UInt32 unk428 = 0;
      UInt32 unk42C = 0;
      UInt32 unk430 = 0;
   }
}