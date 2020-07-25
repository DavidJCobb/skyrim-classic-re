#pragma once
#include "types.h"
#include "objects.h"

namespace RE {
   class NiNode : public NiAVObject { // sizeof == 0xB8
      public:
         virtual void AttachChild(NiAVObject* obj, bool firstAvail);
         virtual void DetachChild(UInt32 unk1, NiAVObject* obj);
         virtual void Unk_35(void);
         virtual void RemoveChild(NiAVObject* obj);
         virtual void Unk_37(void);
         virtual void RemoveAt(UInt32 i);
         virtual void Unk_39(void);
         virtual void Unk_3A(void);
         virtual void UpdateUpwardPass(void);

         NiTArray<NiAVObject*> m_children; // A8

         static NiNode* make(UInt32 preallocateChildrenArraySize = 0);

         MEMBER_FN_PREFIX(NiNode);
         DEFINE_MEMBER_FN(Constructor, NiNode*, 0x00AAED00, UInt32 preallocateChildrenArraySize);
   };

   DEFINE_SUBROUTINE_EXTERN(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
}
