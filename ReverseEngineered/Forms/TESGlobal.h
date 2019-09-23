#pragma once
#include "skse/GameForms.h"

namespace RE {
   class TESGlobal : public TESForm {
      public:
         enum { kTypeID = kFormType_Global };
         //
         BSString editorID; // 14
         UInt8    unk1C;    // 1C - type?
         UInt8    pad[3];   // 1D
         float    value;    // 20
         //
         bool IsConstant() const {
            return (this->flags & 0x40) != 0;
         };
   };
};