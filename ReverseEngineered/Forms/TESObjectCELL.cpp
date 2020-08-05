#include "TESObjectCELL.h"
#include "ReverseEngineered\NetImmerse\nodes.h"
#include "ReverseEngineered\Systems\GameData.h"

namespace RE {
   bool TESObjectCELL::IsLoaded() {
      return CALL_MEMBER_FN(*g_TES, CheckCellLoaded)(this, 0);
   }
   NiNode* TESObjectCELL::GetMultiBoundRoomContainer() noexcept {
      //
      // Modeled on code seen at 0x00434303, with the meanings of these nodes verified 
      // through memory inspection. Equivalent to:
      // 
      //    return CALL_MEMBER_FN(this, GetSpecializedNode)(7);
      //
      // Apparently that gets inlined in a few places.
      //
      auto node = CALL_MEMBER_FN(this, GetRootNode)();
      if (!node)
         return nullptr;
      if (node->m_children.m_size <= 7)
         return nullptr;
      return (NiNode*)node->m_children.m_data[7];
   }
   // content goes in child 3
}