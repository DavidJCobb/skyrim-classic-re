#pragma once
#include "skse/GameFormComponents.h"

namespace RE {
   class TESContainer : public BaseFormComponent { // sizeof == 0xC
      public:
         struct Entry {
            struct Data {
               UInt32 unk00;
               UInt32 unk04;
               float  unk08;
            };
            //
            UInt32   count;
            TESForm* form;
            Data*    data; // extra data?
         };

         Entry** entries;    // 04
         UInt32  numEntries; // 08

         MEMBER_FN_PREFIX(TESContainer);
         DEFINE_MEMBER_FN(Contains, bool, 0x0044F220, TESForm* item);
   };
}
