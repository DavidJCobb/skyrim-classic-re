#include "MessageBoxCallbacks.h"
#include "skse/GameAPI.h"

namespace RE {
   namespace SkyrimScript {
      /*static*/ ScriptMessageBoxCallback* ScriptMessageBoxCallback::make(uint32_t stackID, decltype(unk0C) unk0C) {
         auto memory = FormHeap_Allocate(sizeof(ScriptMessageBoxCallback));
         *(uint32_t*)memory = vtbl;
         auto instance = (ScriptMessageBoxCallback*)memory;
         instance->stackID = stackID;
         instance->unk0C   = unk0C;
         return instance;
      }
   }
}