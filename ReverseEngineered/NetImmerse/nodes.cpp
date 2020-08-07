#include "nodes.h"
#include "skse/GameAPI.h"

namespace RE {
   /*static*/ NiNode* NiNode::make(UInt32 preallocateChildrenArraySize) {
      auto memory = (NiNode*) FormHeap_Allocate(sizeof(NiNode));
      CALL_MEMBER_FN(memory, Constructor)(preallocateChildrenArraySize);
      return memory;
   }
   NiObject* NiNode::FindObjectByName(const char* name) const noexcept {
      if (this->m_name && strcmp(this->m_name, name) == 0)
         return const_cast<NiNode*>(this); // obnoxious
      auto size = this->m_children.m_size;
      for (uint32_t i = 0; i < size; ++i) {
         auto child = this->m_children.m_data[i];
         if (!child)
            continue;
         if (child->m_name && strcmp(child->m_name, name) == 0)
            return child;
         auto next = child->GetAsNiNode();
         if (next) {
            auto result = next->FindObjectByName(name);
            if (result)
               return result;
         }
      }
      return nullptr;
   }
   void NiNode::MakeBSFadeNodeFullyVisible() {
      auto bs = this->GetAsBSFadeNode();
      if (bs)
         bs->MakeFullyVisible();
   }

   /*static*/ BSFadeNode* BSFadeNode::make() {
      auto instance = (BSFadeNode*)FormHeap_Allocate(sizeof(BSFadeNode));
      CALL_MEMBER_FN(instance, Constructor)();
      return instance;
   }

   DEFINE_SUBROUTINE(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
}