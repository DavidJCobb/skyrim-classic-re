#pragma once
#include "skse/GameTypes.h"

namespace RE {
   class MiscStatManager {
      public:
         struct Stat {
            StringCache::Ref unk00;
            StringCache::Ref unk04;
            UInt32 unk08;
            UInt32 unk0C;
            UInt8  unk10;
            UInt8  pad11[3];
         };
         struct FindStatByCRC {
            static constexpr UInt32 vtbl = 0x0108E954;
            //
            virtual bool Test(StringCache::Ref& unk00, StringCache::Ref& unk04, UInt32 unk0C, UInt32 unk08, UInt8 unk10);
            //
            UInt32 unk04;
            UInt32 goalCRC; // 08
         };
      public:
         tArray<Stat> unk00; // sizeof entry == 0x14
         //
         MEMBER_FN_PREFIX(MiscStatManager);
         DEFINE_MEMBER_FN(Subroutine00488120, void, 0x00488120, FindStatByCRC** functor);
   };

   constexpr MiscStatManager* g_miscStatManager = (MiscStatManager*)0x012E6C34;
}
