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
         DEFINE_MEMBER_FN(GetChild, NiAVObject*, 0x00455AD0, uint32_t index);
         DEFINE_MEMBER_FN(InstantRemoveAllChildren, void, 0x00AAE920); // used by destructor. doesn't call virtual methods; just severs parent/child relationships and decrements refcounts as appropriate

         NiObject* FindObjectByName(const char*) const noexcept; // doesn't require a BSFixedString, but will probably be slower
         void MakeBSFadeNodeFullyVisible();
   };
   static_assert(sizeof(NiNode) == 0xB8, "RE::NiNode is the wrong size!");

   class NiSwitchNode : public NiNode { // sizeof == 0xD8
      public:
         static constexpr uint32_t vtbl = 0x0111798C;
         //
         uint16_t unkB8 = 3; // mask
         uint16_t unkBA;
         int32_t  visibleChildIndex = -1; // BC // which child should render
         float    unkC0 = 0.0F;
         uint32_t unkC4 = 1;
         NiTArray<uint32_t> unkC8; // actually a NiTPrimitiveArray<unsigned int>
         //
         MEMBER_FN_PREFIX(NiSwitchNode);
         DEFINE_MEMBER_FN(Constructor, NiSwitchNode&, 0x00AB61B0);
         DEFINE_MEMBER_FN(SetVisibleChildIndex, void, 0x004559A0, uint32_t index); // does bounds-checking
   };

   class BSNiNode : public NiNode { // does not define any new fields; probably just overrides some virtual methods
      public:
         static constexpr uint32_t vtbl = 0x0112083C;
         MEMBER_FN_PREFIX(BSNiNode);
         DEFINE_MEMBER_FN(Constructor, BSNiNode&, 0x00B25820);
   };

   class BSOrderedNode : public BSNiNode { // sizeof == 0xCC
      //
      // Child nodes are always drawn in order, regardless of depth, such that later children 
      // always render overtop earlier children.
      //
      public:
         static constexpr uint32_t vtbl = 0x01157078;
         //
         NiPoint3 alphaSortBound;      // B8 // bound position?
         float    alphaSortBoundW = 0; // C4 // bound radius?
         bool     staticBound = false; // C8
         uint8_t  padC9[3];
         //
         MEMBER_FN_PREFIX(BSOrderedNode);
         DEFINE_MEMBER_FN(Constructor, BSOrderedNode&, 0x00CC21E0);
   };

   class BSMultiBoundNode : public BSNiNode { // sizeof == 0xC4
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

   class ShadowSceneNode : public NiNode { // sizeof == 0x224
      public:
         static constexpr uint32_t vtbl = 0x01150210;
         //
         inline static ShadowSceneNode* get() {
            return *(ShadowSceneNode**)0x01BA7680;
         }
         //
         MEMBER_FN_PREFIX(ShadowSceneNode);
         DEFINE_MEMBER_FN(Constructor, ShadowSceneNode&, 0x00C783D0, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t); // all args are ints or flags
         DEFINE_MEMBER_FN(Subroutine00C781D0, void, 0x00C781D0, NiNode*, uint32_t zero); // called on a flora NiNode after toggling the NiSwitchNode inside
         DEFINE_MEMBER_FN(Subroutine00C77EB0, void, 0x00C77EB0, NiAVObject*, uint32_t one, uint32_t maybeBool); // crashes when called in tests
         DEFINE_MEMBER_FN(Subroutine00C78D80, void, 0x00C78D80, NiAVObject*);
         DEFINE_MEMBER_FN(Subroutine00C78F60, void, 0x00C78F60, NiAVObject*);
   };

   DEFINE_SUBROUTINE_EXTERN(bool, InterpolateWeightModels, 0x00B0EEF0, NiPointer<NiNode> minWeightModel, NiPointer<NiNode> maxWeightModel, float weight, NiPointer<NiNode>& out);
   static DEFINE_SUBROUTINE(void, SetNodeMotionType, 0x00D11180, NiNode*, uint32_t motionSystem, bool iterateObjectChildren, bool skipBSXFlagsCheck, bool allowActivate);
}
