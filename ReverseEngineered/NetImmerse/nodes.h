#pragma once
#include "types.h"
#include "objects.h"

namespace RE {
   class NiNode : public NiAVObject { // sizeof == 0xB8
      public:
         virtual void AttachChild(NiAVObject* obj, bool firstAvail); // 33 // if (obj) has a parent node, it will be removed from that parent by way of parent::RemoveChild
         virtual void DetachChild(UInt32 unk1, NiAVObject* obj); // 34
         virtual void Unk_35(void);
         virtual void RemoveChild(NiAVObject* obj); // 36
         virtual void Unk_37(void);
         virtual void RemoveAt(UInt32 i); // 38
         virtual void Unk_39(void);
         virtual void Unk_3A(void);
         virtual void UpdateUpwardPass(void);

         NiTArray<NiAVObject*> m_children; // A8

         static NiNode* make(UInt32 preallocateChildrenArraySize = 0);

         MEMBER_FN_PREFIX(NiNode);
         DEFINE_MEMBER_FN(Constructor, NiNode*, 0x00AAED00, UInt32 preallocateChildrenArraySize);
   };

   class BSMultiBoundNode : public NiNode { // sizeof == 0xC4
      public:
         uint32_t unkB8 = 0;
         uint32_t unkBC = 0;
         uint32_t unkC0 = 0;
         //
         MEMBER_FN_PREFIX(BSMultiBoundNode);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundNode&, 0x00B16110);
   };

   DEFINE_SUBROUTINE_EXTERN(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
}
