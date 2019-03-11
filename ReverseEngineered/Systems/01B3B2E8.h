#pragma once
#include "ReverseEngineered/Forms/Actor.h"
#include "skse/GameTypes.h"

namespace RE {
   struct  Struct0077ED30;
   typedef Struct0077ED30 Singleton01B3B2E8;
   //
   struct Struct0077ED30 { // sizeof == 0x2C
      tArray<ActorKnowledge*> unk00;
      SimpleLock     unk0C;
      tArray<UInt32> unk14;
      tArray<UInt32> unk20;
      //
      inline Struct0077ED30* GetInstance() {
         return (Struct0077ED30*)0x01B3B2E8;
      };
   };
};