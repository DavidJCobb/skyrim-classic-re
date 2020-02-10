#pragma once
#include "skse/NiNodes.h"
#include "skse/NiObjects.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Shared.h"

class bhkAabbPhantom;
namespace RE {
   class bhkCollisionObject;
   class bhkRigidBody;
   class TESObjectREFR;

   class NiObject : public NiRefObject {
      //
      // Notes:
      //  - NiNode->unk1C instanceof bhkCollisionObject (or anything that can cast to it)
      //
      public:
         static constexpr uint32_t vtbl = 0x011173FC;
         //
      public:
         //
         // Standard NetImmerse:
         //
         virtual NiRTTI*		GetRTTI(void); // 02
         // 
         // Casts:
         //
         virtual NiNode*			GetAsNiNode(); // 03
         virtual NiSwitchNode*	GetAsNiSwitchNode();
         virtual BSFadeNode*		GetAsBSFadeNode(); // 05
         virtual UInt32			   Unk_06();
         virtual NiGeometry*		GetAsNiGeometry(); // 07
         virtual NiTriBasedGeom*	GetAsNiTriBasedGeom(); // 08
         virtual NiTriStrips*		GetAsNiTriStrips();
         virtual NiTriShape*		GetAsNiTriShape();
         virtual BSSegmentedTriShape* GetAsBSSegmentedTriShape();
         virtual UInt32			Unk_0C(void);
         virtual UInt32			Unk_0D(void);
         virtual UInt32			Unk_0E(void);
         virtual bhkCollisionObject* GetAsBhkCollisionObject(); // or a superclass? // virtual method 0x0F
         virtual UInt32			Unk_10(void);
         virtual bhkRigidBody* GetAsBhkRigidBody(); // or a superclass?
         virtual UInt32        Unk_12(void);
         //
         // Back to NetImmerse:
         //
         virtual NiObject*		CreateClone(NiCloningProcess cloner); // 13
         //
         virtual void			LoadBinary(NiStream * stream); // 14
         virtual void			LinkObject(NiStream * stream); // 15
         virtual bool			RegisterStreamables(NiStream * stream); // 16
         virtual void			SaveBinary(NiStream * stream); // 17
         virtual bool			IsEqual(NiObject * object); // 18
         //
         virtual void			ProcessClone(NiCloningProcess * cloner); // 19
         virtual void			PostLinkObject(NiStream * stream); // 1A
         virtual bool			StreamCanSkip(void); // 1B
         virtual const NiRTTI*	GetStreamableRTTI(void) const; // 1C
         virtual UInt32         GetBlockAllocationSize(void) const; // 1D
         virtual NiObjectGroup* GetGroup(void) const; // 1E
         virtual void			SetGroup(NiObjectGroup * group); // 1F
         //
         // Begin bethesda extensions?
         //
         virtual UInt32			Unk_20(void); // 20
         //
         MEMBER_FN_PREFIX(NiObject);
         DEFINE_MEMBER_FN(Constructor, NiObject&, 0x00AAFD30);
         DEFINE_MEMBER_FN(Clone,       NiObject*, 0x00AAFC00, NiCloningProcess*);
         DEFINE_MEMBER_FN(VerifyType,  void*,     0x004202E0, const NiRTTI*); // If object is of type, returns self; otherwise returns NULL.
   };
   static_assert(sizeof(NiObject) == 0x08, "RE::NiObject is not the right size!");

   class NiObjectNET : public NiObject { // sizeof == 0x18
      public:
         static constexpr uint32_t vtbl = 0x01117824;
         //
      public:
         const char*       m_name;
         NiTimeController* m_controller; // 0C next pointer at +0x30
         NiExtraData**     m_extraData;  // 10 extra data
         UInt16 m_extraDataLen;      // 14 max valid entry
         UInt16 m_extraDataCapacity; // 16 array len

         // UNTESTED:
         void AddExtraData(NiExtraData * extraData);
         bool RemoveExtraData(NiExtraData * extraData);
         SInt32 GetIndexOf(NiExtraData * extraData);
         NiExtraData* GetExtraData(BSFixedString name);

         MEMBER_FN_PREFIX(NiObjectNET);
         DEFINE_MEMBER_FN(Constructor, NiObjectNET&, 0x00AB4180);
         DEFINE_MEMBER_FN(DynamicCastController, void*, 0x00AB40C0, const NiRTTI*); // dynamic-casts this->m_controller
         DEFINE_MEMBER_FN(SetName, void, 0x00AB4020, StringCache::Ref*);
   };
   static_assert(sizeof(NiObjectNET) == 0x18, "RE::NiObjectNET is not the right size!");

   class NiAVObject : public NiObjectNET { // sizeof == 0xA8
      public:
         enum {
            kFlag_SelectiveUpdate             = 0x00000002,
            kFlag_UpdatePropertyControllers   = 0x00000004,
            kFlag_SelectiveUpdateRigid        = 0x00000010,
            kFlag_OverrideSelectiveTransforms = 0x00000080,
         };
         //
         struct ControllerUpdateContext {
            enum {
               kDirty = 1 << 0,
            };
            //
            float  delta;
            UInt32 flags;
         };
         //
         virtual void UpdateControllers(ControllerUpdateContext * ctx); // 21	// calls controller vtbl+0x8C
         virtual void UpdateNodeBound(ControllerUpdateContext * ctx);
         virtual void ApplyTransform(NiMatrix33 * mtx, NiPoint3 * translate, bool postTransform);
         virtual void SetPropertyState(NiProperty * prop);
         virtual void Unk_25(UInt32 arg0);
         virtual void Unk_26(UInt32 arg0);
         virtual NiAVObject* GetObjectByName(const char ** name);	// BSFixedString? alternatively BSFixedString is a typedef of a netimmerse type
         virtual void SetSelectiveUpdateFlags(bool * selectiveUpdate, bool selectiveUpdateTransforms, bool * rigid);
         virtual void UpdateDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateSelectedDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateRigidDownwardPass(ControllerUpdateContext * ctx, UInt32 unk1);
         virtual void UpdateWorldBound(void);
         virtual void UpdateWorldData(ControllerUpdateContext * ctx);
         virtual void UpdateNoControllers(ControllerUpdateContext * ctx);
         virtual void UpdateDownwardPassTempParent(NiNode * parent, ControllerUpdateContext * ctx);
         virtual void Unk_30(void);	// calls virtual function on parent
         virtual void Unk_31(UInt32 arg0);
         virtual void Unk_32(UInt32 arg0);
         //
         NiNode*     m_parent;			 // 18
         bhkCollisionObject* collision; // 1C
         NiTransform	m_localTransform;  // 20
         NiTransform	m_worldTransform;  // 54
         NiPoint3 unk88; // 88
         float		unk94;				// 94
         UInt32   m_flags;			   // 98 - bitfield
         float		unk9C;				// 9C
         TESObjectREFR* unkA0;				// A0 // per 004D7C90
         UInt8		unkA4;				// A4
         UInt8		unkA5;				// A5 - bitfield
         //
         MEMBER_FN_PREFIX(NiAVObject);
         DEFINE_MEMBER_FN(UpdateNode, void, 0x00AAF320, ControllerUpdateContext * ctx);
   };
   static_assert(sizeof(NiAVObject) == 0xA8, "RE::NiAVObject is not the right size!");

   class LoadedAreaBound : public NiRefObject {
      public:
         virtual ~LoadedAreaBound();
         //
         bhkAabbPhantom* unk08; // bhkAabbPhantom*
         bhkAabbPhantom* unk0C; // bhkAabbPhantom*
         bhkAabbPhantom* unk10; // bhkAabbPhantom*
         bhkAabbPhantom* unk14; // bhkAabbPhantom*
         bhkAabbPhantom* unk18; // bhkAabbPhantom*
         bhkAabbPhantom* unk1C; // bhkAabbPhantom*
         TESObjectCELL* unk20; // TESObjectCELL*
         TESObjectCELL* cell;  // 24
         UInt32 unk28;
         UInt32 unk2C;
         UInt32 unk30;
         UInt32 unk34;
         UInt32 unk38; // inited 0xDEADBEEF
         UInt32 unk3C;
         UInt32 unk40;
         NiPoint3 boundsMax; // 44
         NiPoint3 boundsMin; // 50
         float  unk5C; // ==   20.0 when observed
         float  unk60; // ==  600.0 when observed
         float  unk64; // == 1000.0 when observed
         float  unk68; // ==    0.3 when observed
   };
   static_assert(sizeof(LoadedAreaBound) == 0x6C, "LoadedAreaBound is the wrong size.");
   static_assert(offsetof(LoadedAreaBound, boundsMax) == 0x44, "LoadedAreaBound::boundsMax is at the wrong offset.");

   DEFINE_SUBROUTINE_EXTERN(bhkCollisionObject*, GetBhkCollisionObjectForNiObject, 0x0046A240, NiObject* obj); // returns obj->unk1C ? obj->unk1C : obj->GetAsBhkCollisionObject();
   DEFINE_SUBROUTINE_EXTERN(bool,                NiObjectIs,                       0x0042A960, const NiRTTI*, const NiObject*);
};