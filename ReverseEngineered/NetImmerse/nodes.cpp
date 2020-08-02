#include "nodes.h"
#include "skse/GameAPI.h"

namespace RE {
   /*static*/ NiNode* NiNode::make(UInt32 preallocateChildrenArraySize) {
      auto memory = (NiNode*) FormHeap_Allocate(sizeof(NiNode));
      CALL_MEMBER_FN(memory, Constructor)(preallocateChildrenArraySize);
      return memory;
   }
   void NiNode::MakeBSFadeNodeFullyVisible() {
      auto bs = this->GetAsBSFadeNode();
      if (bs)
         bs->MakeFullyVisible();
   }

   DEFINE_SUBROUTINE(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
}