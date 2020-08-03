#include "Functors.h"

namespace RE {
   namespace SkyrimScript {
      /*static*/ SetMotionTypeFunctor* SetMotionTypeFunctor::make(const ref_handle& handle, uint32_t motionType, bool allowActivate, VMClassRegistry* registry, uint32_t stackID) {
         auto memory = (SetMotionTypeFunctor*) FormHeap_Allocate(sizeof(SetMotionTypeFunctor));
         CALL_MEMBER_FN(memory, Constructor)(handle, motionType, allowActivate, registry, stackID);
         return memory;
      }
   }
}