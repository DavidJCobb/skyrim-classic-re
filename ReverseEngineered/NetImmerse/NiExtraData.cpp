#include "NiExtraData.h"
#include "skse/GameAPI.h"

namespace RE {
   /*static*/ NiExtraData* NiExtraData::make() {
      auto memory = (NiExtraData*) FormHeap_Allocate(sizeof(NiExtraData));
      CALL_MEMBER_FN(memory, Constructor)();
      return memory;
   }
   /*static*/ BSXFlags* BSXFlags::make() {
      auto memory = (BSXFlags*) FormHeap_Allocate(sizeof(BSXFlags));
      CALL_MEMBER_FN(memory, Constructor)();
      return memory;
   }
}