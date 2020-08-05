#pragma once
#include "types.h"
#include "objects.h"

namespace RE {
   class BSMultiBound;

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
         virtual void UpdateUpwardPass(); // 3B

         NiTArray<NiAVObject*> m_children; // A8

         static NiNode* make(UInt32 preallocateChildrenArraySize = 0);

         MEMBER_FN_PREFIX(NiNode);
         DEFINE_MEMBER_FN(Constructor, NiNode*, 0x00AAED00, UInt32 preallocateChildrenArraySize);

         NiObject* FindObjectByName(const char*) const noexcept; // doesn't require a BSFixedString, but will probably be slower
         void MakeBSFadeNodeFullyVisible();
   };
   static_assert(sizeof(NiNode) == 0xB8, "RE::NiNode is the wrong size!");

   class BSMultiBoundNode : public NiNode { // sizeof == 0xC4
      public:
         NiPointer<BSMultiBound> unkB8 = nullptr;
         uint32_t unkBC = 0;
         float    unkC0 = 0.0F;
         //
         MEMBER_FN_PREFIX(BSMultiBoundNode);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundNode&, 0x00B16110);
         DEFINE_MEMBER_FN(SetMultiBound, void, 0x004291E0, BSMultiBound*);
   };

   class BSMultiBoundRoom : public BSMultiBoundNode { // sizeof == 0xF4
      public:
         static constexpr uint32_t vtbl = 0x0111DA1C;
         //
         uint32_t unkC4 = 0;
         uint32_t unkC8 = 0;
         uint32_t unkCC = 0;
         uint32_t unkD0 = 0;
         uint32_t unkD4 = 0;
         uint32_t unkD8 = 0;
         uint32_t unkDC = 0;
         uint32_t unkE0 = 0;
         uint32_t unkE4 = 0;
         tArray<uint32_t> unkE8;
         //
         MEMBER_FN_PREFIX(BSMultiBoundRoom);
         DEFINE_MEMBER_FN(Constructor, BSMultiBoundRoom&, 0x00AFCD30);
   };

   class BSFadeNode : public NiNode { // sizeof == 0xE4
      public:
         static constexpr uint32_t vtbl = 0x0114D988;
         //
         float    unkB8 = FLT_MAX;
         float    unkBC = FLT_MAX;
         float    unkC0 = 0.0F; // C0 // not 100% certain but this seems to control the BSFadeNode-related opacity. set to 1.0F to make the model fully visible instantly
         uint32_t unkC4 = 0;
         uint32_t unkC8 = 0;
         uint32_t unkCC = 0;
         float    unkD0 = 1.0F;
         uint32_t unkD4 = 0;
         uint32_t unkD8 = 0;
         float    unkDC = 1.0F;
         uint16_t unkE0; // ?
         uint8_t  unkE2 = 0xF3;
         uint8_t  unkE3; // mask; lower four bits are a single value
         //
         static BSFadeNode* make();
         //
         MEMBER_FN_PREFIX(BSFadeNode);
         DEFINE_MEMBER_FN(Constructor, BSFadeNode&, 0x00C6BBE0);
         DEFINE_MEMBER_FN(ResetFadeNodeState, void, 0x00C6B440); // called in constructor
         DEFINE_MEMBER_FN(Subroutine00AAF790, bool, 0x00C6B370); // checks for a specific flag on any NiTimeControllers on the node
         DEFINE_MEMBER_FN(Subroutine00C6B370, void, 0x00C6B370, uint8_t);

         inline void MakeFullyVisible() { this->unkC0 = 1.0F; }
   };

   DEFINE_SUBROUTINE_EXTERN(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
   static DEFINE_SUBROUTINE(void, SetNodeMotionType, 0x00D11180, NiNode*, uint32_t motionSystem, bool iterateObjectChildren, bool skipBSXFlagsCheck, bool allowActivate);
}
