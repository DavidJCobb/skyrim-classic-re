#pragma once
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/NetImmerse/objects.h"
#include "skse/NiNodes.h"

typedef void(*BhkCollisionIteratorFunction)(RE::bhkCollisionObject* collision, void* parameters);
static DEFINE_SUBROUTINE(void, IterateOverBhkCollisionObjects,        0x00D0E520, RE::NiNode* root, void* parameters, BhkCollisionIteratorFunction* iterator);
//
// Alternate name:
//
static DEFINE_SUBROUTINE(void, RecurseOverCollisionObjectsInNodeTree, 0x00D0E520, RE::NiNode* root, void* parameters, BhkCollisionIteratorFunction* iterator);

//
// It would appear that hk_____ classes are native Havok, while bhk_____ are Bethesda wrappers and extensions.
//
class hkpAllRayHitTempCollector;
namespace RE {
   class NiNode;
   //
   class bhkWorld;
   //
   // TODO: Virtual methods for hkp_____ classes.
   // TODO: Virtual methods for bhk_____ classes.
   //

   template<typename T> struct hkArray { // found RTTI mentioning it; todo: investigate further
      T*     data;
      UInt32 size;
      UInt32 unk08;
   };
   template<typename T> struct hkSmallArray { // found RTTI mentioning it; todo: investigate further
      T*     data;
      UInt16 size;
      UInt16 unk08;
   };
   struct hkVector4 { // found RTTI mentioning it; todo: investigate further
      float a; // 00
      float b; // 04
      float c; // 08
      float d; // 0C

      MEMBER_FN_PREFIX(hkVector4);
      DEFINE_MEMBER_FN(Subroutine004D4520, void, 0x004D4520, void*);
      DEFINE_MEMBER_FN(Subroutine006FB4F0, void, 0x006FB4F0, void*);
      DEFINE_MEMBER_FN(Subroutine00D5C490, void, 0x00D5C490, void*, void*);

      static void ConstructFromNiPoint3(hkVector4& out, const NiPoint3& position) {
         DEFINE_SUBROUTINE(void, f, 0x0045C770, hkVector4 & out, const NiPoint3& position);
         f(out, position);
      }
   };

   struct Struct00DC50C0 { // sizeof == 0xC4
      uint32_t unk00;
      uint32_t unk04;
      uint32_t unk08;
      uint8_t  unk0C = 1;
      uint8_t  unk0D;
      uint16_t unk0E = 0xFFFF;
      uint32_t unk10 = 0;
      uint32_t unk14 = 0;
      uint32_t unk18 = 0;
      uint32_t unk1C = 0;
      float    unk20 = 1.0F;
      uint32_t unk24 = 0;
      uint32_t unk28 = 0;
      uint32_t unk2C = 0;
      uint32_t unk30 = 0;
      uint32_t unk34 = 0;
      uint32_t unk38 = 0;
      uint32_t unk3C = 0;
      uint32_t unk40 = 0;
      uint32_t unk44 = 0;
      uint32_t unk48 = 0;
      uint32_t unk4C = 0;
      float    unk50 = 1.0F;
      uint32_t unk54 = 0;
      uint32_t unk58 = 0;
      uint32_t unk5C = 0;
      uint32_t unk60 = 0;
      float    unk64 = 1.0F;
      uint32_t unk68 = 0;
      uint32_t unk6C = 0;
      uint32_t unk70 = 0;
      uint32_t unk74 = 0;
      float    unk78 = 1.0F;
      uint32_t unk7C = 0;
      uint32_t unk80;
      float    unk84;
      uint32_t unk88;
      uint32_t unk8C;
      float    unk90 = 1.0F;
      float    unk94 = 0.0F;
      float    unk98 = 0.05F;
      float    unk9C = 1.0F;
      uint32_t unkA0;
      float    unkA4 = 0.5F;
      float    unkA8 = 1.0F;
      float    unkAC = 0.4F;
      float    unkB0 = 200.0F;
      float    unkB4 = 200.0F;
      float    unkB8 = -1.0F;
      uint8_t  unkBC;
      uint8_t  unkBD = 1;
      uint8_t  unkBE = 2;
      uint8_t  unkBF = 0xFF;
      uint8_t  unkC0 = 0;
      uint8_t  unkC1 = 0;
      uint8_t  unkC2 = 0;
      uint8_t  unkC3 = 0;
      //
      MEMBER_FN_PREFIX(Struct00DC50C0);
      DEFINE_MEMBER_FN(Constructor, Struct00DC50C0&, 0x00DC50C0);
   };
   struct Struct00D378B0 {
      uint32_t unk00 = 0;
      float    unk04 = 0;
      uint8_t  unk08 = 1;
      uint8_t  pad09[3];
      uint32_t unk0C = 0;
      uint32_t unk10 = 0;
      uint32_t unk14;
      uint16_t unk18;
      uint16_t unk1A = 0xFFFF;
      uint32_t unk1C;
      Struct00DC50C0 unk20;
      //
      MEMBER_FN_PREFIX(Struct00D378B0);
      DEFINE_MEMBER_FN(Constructor, Struct00D378B0&, 0x00D378B0);
      DEFINE_MEMBER_FN(Destructor, void, 0x004E28C0);
      //
      ~Struct00D378B0() { CALL_MEMBER_FN(this, Destructor)(); }
   };
   struct Struct00D69170 { // sizeof == 0x2C
      UInt32 unk00 = 1;
      UInt32 unk04;
      UInt32 unk08;
      UInt32 unk0C;
      UInt32 unk10 = 0;
      UInt32 unk14;
      UInt32 unk18;
      UInt32 unk1C;
      UInt16 unk20 = 0x0000;
      UInt16 unk22 = 0x0000;
      UInt32 unk24 = 0;
      UInt32 unk28 = 0;
   };
   struct Struct00D57F60 { // sizeof == 0x50
      UInt8  unk00;
      UInt8  unk01;
      UInt8  unk02;
      UInt8  unk03;
      UInt32 unk04 = 0xFFFFFFFF;
      void*  unk08; // no VTBL
      UInt32 unk0C = 0;
      UInt8  unk10 = 0;
      UInt8  unk11 = 8;
      UInt16 unk12 = 0x0000;
      UInt32 unk14 = 0;
      UInt8  unk18;
      UInt8  unk19 = 0x7F;
      UInt8  unk1A = 0xFF;
      UInt8  unk1B;
      UInt32 unk1C = 0;
      Struct00D69170 unk20;
      float  unk4C = -1.0;
   };
   struct Struct00D5FC10 { // sizeof == 0x4
      UInt32 unk00 = 0;
   };

   struct Struct00DDB260 { // sizeof >= 0xA4
      struct Struct00DDB2B0 { // sizeof == 0x50?
         UInt32 unk00[0x50 / 4]; // five DQWords?
      };
      //
      UInt32 unk04[0x40 / 4]; // unk10, unk20, and unk30 are DQWords?
      Struct00DDB2B0 unk40;
      UInt32 unk90[(0xA0 - 0x90) / 4];
      float  unkA0 = 1.0;
   };
   static_assert(sizeof(RE::Struct00DDB260) == 0xA4, "RE::Struct00DDB260 is not the right size!");

   struct Struct0045C7E0 { // apparently used to request a raycast from a bhkWorldM
      UInt32 unk00;
      UInt32 unk04;
      UInt32 unk08;
      UInt32 unk0C;
      UInt32 unk10;
      UInt32 unk14;
      UInt32 unk18;
      UInt32 unk1C;
      UInt8  unk20 = 0;
      UInt8  unk21;
      UInt8  unk22;
      UInt8  unk23;
      UInt32 unk24 = 0;
      UInt32 unk28;
      UInt32 unk2C;
      UInt32 unk30;
      UInt32 unk34;
      UInt32 unk38;
      UInt32 unk3C;
      float  unk40 = 1.0F;
      UInt32 unk44 = -1;
      UInt32 unk48 = -1;
      UInt32 unk4C;
      UInt32 unk50 = -1;
      UInt32 unk54;
      UInt32 unk58;
      UInt32 unk5C;
      UInt32 unk60;
      UInt32 unk64;
      UInt32 unk68;
      UInt32 unk6C;
      UInt32 unk70 = 0;
      UInt32 unk74;
      UInt32 unk78;
      UInt32 unk7C;
      UInt32 unk80 = 0;
      UInt32 unk84;
      UInt32 unk88;
      UInt32 unk8C;
      float  unk90[4] = {0.0F, 0.0F, 0.0F, 0.0F};
      UInt32 unkA0 = 0;
      UInt32 unkA4 = 0;
      UInt32 unkA8 = 0;
      hkpAllRayHitTempCollector* unkAC = nullptr;
      UInt8  unkB0 = 0;
      UInt8  unkB1;
      UInt8  unkB2;
      UInt8  unkB3;
   };

   class hkBaseObject {
      public:
         virtual ~hkBaseObject();
   };
   class hkReferencedObject : public hkBaseObject { // sizeof == 0x08
      public:
         enum { kVTBL = 0x01087638 };

         virtual ~hkReferencedObject();
         virtual NiRTTI* GetRTTI(); // 01 // ...or a Havok struct that works exactly the same way
         virtual void   Unk_02(UInt32, UInt32); // RTTI-/type-related

         UInt16 memSize;  // 04
         UInt16 refCount; // 06
   };
   class bhkRefObject : public NiObject {
      public:
         enum { kVTBL = 0x01164848 };
         virtual void Unk_21(UInt32); // 21
         virtual void Unk_22(UInt32);

         void* wrappedHavokObject; // 08 // probably wrapped object i.e. decltype(bhkSomething::unk08) == hkpSomething*

         MEMBER_FN_PREFIX(bhkRefObject);
         DEFINE_MEMBER_FN(Constructor, bhkRefObject&, 0x00D39470);
   };

   #pragma region Motion
   class hkpMotion { // sizeof == 0x120
      public:
         enum MotionSystem : UInt8 {
            kMotionSystem_Invalid        = 0,
            kMotionSystem_Dynamic        = 1,
            kMotionSystem_SphereInertia  = 2,
            kMotionSystem_BoxInertia     = 3,
            kMotionSystem_Keyframed      = 4,
            kMotionSystem_Fixed          = 5,
            kMotionSystem_ThinBoxInertia = 6,
            kMotionSystem_Character      = 7
         };
         //
         virtual ~hkpMotion();
         //
         UInt16 unk004;
         UInt16 unk006 = 0x0001;
         MotionSystem motionSystem; // 08
         UInt8  unk009 = 0xFF; // DeactivatorType?
         UInt8  unk00A;        // SolverDeactivation?
         UInt8  unk00B;        // MotionQuality?
         UInt16 unk00C = 0x0000;
         UInt16 unk00E;
         Struct00DDB260 unk010;
         UInt16 unk0B4;
         UInt16 unk0B6;
         UInt32 unk0B8;
         UInt8  unk0BC;
         UInt8  unk0BD;
         UInt8  unk0BE;
         UInt8  unk0BF;
         UInt32 unk0C0[(0xF0 - 0xC0) / 4]; // not initialized
         UInt32 unk0F0[0x10 / 4]; // DQWORD
         UInt32 unk100[0x10 / 4]; // DQWORD
         UInt32 unk110 = 0;
         UInt32 unk114 = 0;
         UInt32 unk118;
         UInt8  unk11C;
         UInt8  unk11D;
         UInt16 unk11E;
         //
         MEMBER_FN_PREFIX(hkpMotion);
         DEFINE_MEMBER_FN(SetUnkBC, void, 0x00D4A3C0, UInt8 value);
   };
   static_assert(sizeof(RE::hkpMotion) == 0x120, "RE::hkpMotion is not the right size!");
   class hkpMaxSizeMotion : public hkpMotion { // sizeof == 0x120
      public:
         virtual ~hkpMaxSizeMotion();
   };
   class hkpKeyframedRigidMotion : public hkpMotion { // sizeof == 0x120
      public:
         virtual ~hkpKeyframedRigidMotion();
   };
   class hkpFixedRigidMotion : public hkpKeyframedRigidMotion { // sizeof == 0x120
      public:
         virtual ~hkpFixedRigidMotion();
   };
   #pragma endregion

   class hkpWorldObject { // sizeof == 0x8C
      public:
         virtual ~hkpWorldObject();
         //
         UInt16 unk004;
         UInt16 unk006 = 0x0001;
         void*  unk008 = NULL; // class with no RTTI
         void*  unk00C; // Most likely a pointer back to the owning bhkWorldObject instance.
         Struct00D57F60 unk010;
         UInt32 unk060 = 0;
         UInt32 unk064 = 0;
         UInt32 unk068 = 0;
         SInt32 unk06C = -0xF;
         UInt32 unk070;
         UInt16 unk074 = 0x8000;
         UInt16 unk076;
         Struct00D5FC10 unk078;
         UInt32 unk07C = 0;
         UInt32 unk080 = 0;
         UInt32 unk084 = 0x80000000;
         SInt32 unk088 = -1;
   };
   static_assert(sizeof(RE::hkpWorldObject) == 0x8C, "RE::hkpWorldObject is not the right size!");

   class hkpEntity : public hkpWorldObject { // sizeof >= 0x21C
      public:
         virtual ~hkpEntity();
         //
         UInt16 unk08C;
         UInt16 unk08E;
         float  unk090 = 0.5;
         float  unk094 = 0.4;
         UInt32 unk098 = 0;
         float  unk09C = 1.0;
         UInt32 unk0A0 = 0;
         UInt32 unk0A4 = 0;
         UInt16 unk0A8 = 0xFFFF;
         UInt16 unk0AA;
         hkSmallArray<UInt32> unk0AC;
         UInt32 unk0B4 = 0;
         UInt32 unk0B8 = 0;
         UInt32 unk0BC = 0x80000000;
         UInt32 unk0C0 = 0;
         UInt32 unk0C4 = 0;
         UInt32 unk0C8 = 0;
         UInt32 unk0CC = 0;
         UInt8  unk0D0;
         UInt8  unk0D1 = 0;
         UInt8  unk0D2;
         UInt8  unk0D3;
         UInt32 unk0D4 = 0xFFFFFFFF;
         UInt32 unk0D8 = 0;
         UInt16 unk0DC = 0x0000;
         UInt8  unk0DE = 3;
         UInt8  unk0DF = 1;
         hkpMaxSizeMotion unk0E0;
         UInt32 unk200 = 0;
         UInt16 unk204 = 0x0000;
         UInt16 unk206 = 0x8000;
         UInt32 unk208 = 0;
         UInt16 unk20C = 0x0000;
         UInt16 unk20E = 0x8000;
         UInt32 unk210 = 0;
         UInt32 unk214 = 0;
         UInt32 unk218 = 0xFFFFFFFF;
   };
   static_assert(sizeof(RE::hkpEntity) == 0x21C, "RE::hkpEntity is not the right size!");

   class hkpRigidBody : public hkpEntity { // sizeof == 0x220?
      public:
         virtual ~hkpRigidBody();
         //
         UInt32 unk21C;
   };
   static_assert(sizeof(RE::hkpRigidBody) == 0x220, "RE::hkpRigidBody is not the right size!");

   class bhkSerializable : public bhkRefObject {
      //
      // VTBL: 0x01088364
      //
      public:
         //
         // Argument signatures assumed to match bhkRigidBody, but none of these have been analyzed yet. 
         // Remember: any methods we rename in one of these classes must be renamed in all of them.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23();
         virtual UInt32 Unk_24();
         virtual bool Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28();
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk);
   };
   class bhkWorldObject : public bhkSerializable {
      //
      // VTBL: 0x010884FC
      //
      public:
         //hkpWorldObject* myHkpObject; // 08 // mimics the owner class, e.g. hkpRigidBody* for a bhkRigidBody
         //
         // Argument signatures assumped to match bhkRigidBody, but none of these have been analyzed yet. 
         // Remember: any methods we rename in one of these classes must be renamed in all of them.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23();
         virtual UInt32 Unk_24();
         virtual bool Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28();
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk);
         virtual void Unk_2E(void);

         inline hkpWorldObject* asHkpWorldObject() { return (hkpWorldObject*) this->wrappedHavokObject; };
   };
   class bhkEntity : public bhkWorldObject {
      //
      // VTBL: 0x010885BC
      // Doesn't add any new virtual methods, though it may override existing ones.
      //
   };
   class bhkRigidBody : public bhkEntity { // sizeof == 0x24
      //
      // VTBL: 0x01089214
      //
      public:
         //
         UInt32 unk0C; // could belong to bhkWorldObject
         UInt16 unk10;
         UInt16 unk12;
         SInt32 unk14;
         UnkArray unk18;
         //
         // Remember: if you rename these virtual methods, rename their counterparts (if any) in the superclasses.
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual UInt32 Unk_23(); // returns this->unk08 ? this->unk08->unk08 : NULL;
         virtual UInt32 Unk_24(); // same as Unk_23, but it checks if this exists first
         virtual bool Unk_25(bhkWorld*);
         virtual void Unk_26(); // does stuff (not sure what) and then recursively executes self on all unk18 elements
         virtual void Unk_27(bool);
         virtual UInt32 Unk_28(); // returns 0xF0
         virtual void Unk_29();
         virtual void Unk_2A(void*);
         virtual void Unk_2B(void);
         virtual void Unk_2C();
         virtual void Unk_2D(UInt32 unk); // returns Unk_27(1);
         virtual void Unk_2E(void);
         virtual void Unk_2F(void);
         virtual void Unk_30(void);
         virtual void Unk_31(const hkVector4& position); // fourth vector component is zero
         virtual void Unk_32(const hkVector4& rotation); // quaternion
         // ...?

         MEMBER_FN_PREFIX(bhkRigidBody);
         DEFINE_MEMBER_FN(Constructor, bhkRigidBody&, 0x004E06D0, Struct00D378B0& params);
         DEFINE_MEMBER_FN(Subroutine00D0D3C0, void, 0x00D0D3C0, NiNode*, UInt32 zero);

         static bhkRigidBody* make(Struct00D378B0&);
   };

   class NiCollisionObject : public NiObject { // sizeof == 0x14
      //
      // Reverse-engineering suggests that this only derives from NiRefObject and not NiObject, but 
      // NiObject has a cast method for bhkCollisionObject, so this obviously DOES derive from NiObject.
      //
      public:
         enum Flags : UInt32 {
            kFlag_Active  = 0x00000001,
            kFlag_Notify  = 0x00000004,
            kFlag_Local   = 0x00000008,
            kFlag_Reset   = 0x00000040,
            kFlag_Unknown = 0x00000080, // set by subroutine 0x00D0DF10
         };
         //
         UInt32        unk08 = 0; // 08 // almost certainly the NiNode* that the collision object is attached to, but I'd like to confirm that
         Flags         flags;     // 0C
         bhkRigidBody* unk10;     // 10
   };
   class bhkNiCollisionObject : public NiCollisionObject { // sizeof == 0x14
      public:
         static constexpr uint32_t vtbl = 0x01162170;
         //
         MEMBER_FN_PREFIX(bhkNiCollisionObject);
         DEFINE_MEMBER_FN(Constructor, bhkNiCollisionObject&, 0x00D16D00, NiNode*);
         DEFINE_MEMBER_FN(SetRigidBody, void, 0x00D16D30, bhkRigidBody*);
         //
         static bhkNiCollisionObject* make(NiNode*);
   };
   class bhkCollisionObject : public bhkNiCollisionObject { // sizeof == 0x14
      //
      // Could have as many as 0x89 virtual methods.
      //
      public:
         static constexpr uint32_t vtbl = 0x010881F4;
         //
         virtual ~bhkCollisionObject();
         //
         virtual void Unk_21(void);
         virtual void Unk_22(void);
         virtual void Unk_23(void);
         virtual void Unk_24(void);
         virtual void Unk_25(void);
         virtual void Unk_26(void);
         virtual void Unk_27(void);
         virtual void Unk_28(void);
         virtual void Unk_29(void);
         virtual void SetMotionSystem(UInt32 motionSystemToSet, UInt32 unkInt, UInt32 unkBool);
         //
         static bhkCollisionObject* make(NiNode*);
   };

   class hkCharacterProxy {
      // bhkRigidBody* unk2A8;
   };

   class bhkBoxShape : public NiRefObject { // sizeof == 0x14
      //
      // Multiple inheritance? from bhkSphereRepShape and bhkConvexShape?
      //
      public:
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         //
         MEMBER_FN_PREFIX(bhkBoxShape);
         DEFINE_MEMBER_FN(Constructor, bhkBoxShape&, 0x004D9110, const hkVector4&);
         //
         static bhkBoxShape* make(const hkVector4&);
   };
   class bhkSphereShape : public NiRefObject {
      //
      // Multiple inheritance? from bhkSphereRepShape and bhkConvexShape?
      //
      public:
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         //
         // ...?
   };

   class bhkWorld : public bhkRefObject { // sizeof == 0x6320
      public:
         virtual ~bhkWorld();
         virtual UInt32 Unk_23() const; // 23 // returns this->unk08
         virtual UInt32 Unk_24() const; // 24 // returns this->unk08
         virtual bool   Unk_25();
         virtual bool   Unk_26();
         virtual void   Unk_27(UInt32);
         virtual UInt32 Unk_28(); // actually returns an integer
         virtual UInt32 Unk_29(UInt32);
         virtual void   Unk_2A(UInt32);
         virtual UInt32 Unk_2B(UInt32);
         virtual void   Unk_2C();
         virtual void   Unk_2D(void);
         virtual bool   Unk_2E(UInt32);
         virtual bool   Unk_2F(Struct0045C7E0&); // 2F // involved in raycasting
         virtual bool   Unk_30();
         virtual bool   Unk_31(UInt32, UInt32, UInt32, UInt32, UInt32);
         virtual bool   Unk_32(UInt32, UInt32, UInt32, UInt32, UInt32);
         //
         // ...
   };
   class bhkWorldM : public bhkWorld {
      //
      // doesn't override any virtual methods besides Dispose, Unk_01, and GetRTTI
      //
   };
   extern bhkWorldM* GetDefaultHavokWorld();

   class hkpAction : public hkReferencedObject {
      //
      // Online sources state that Havok "actions" are meant to serve as an interface between 
      // user-controllable physics behavior (i.e. what a game developer is allowed to influence) 
      // and the physics engine core.
      //
      // hkpAction is the base class for all Havok actions. Presumably, bhkAction is a wrapper 
      // and its unk08 field is an hkpAction.
      //
      public:
         enum { kVTBL = 0x011655C4 };

         virtual ~hkpAction();
         virtual void   Unk_03(UInt32) = 0; // 03 // pure // does a lotta stuff to a mouse spring action's rigid body...
         virtual void   Unk_04(void*) = 0; // 04 // pure here; defined on hkpUnaryAction
         virtual void   Unk_05(UInt32);
         virtual void   Unk_06(UInt32) = 0; // 06 // pure here; defined on hkpUnaryAction
         virtual void   Unk_07(void) = 0; // 07 // pure

         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10;
         UInt32 unk14; // actually a struct with a single dword member
   };
   class hkpUnaryAction : public hkpAction {
      public:
         enum { kVTBL = 0x011655E8 };
         hkpRigidBody* rigidBody; // 18

         MEMBER_FN_PREFIX(hkpUnaryAction);
         DEFINE_MEMBER_FN(Constructor, hkpUnaryAction&, 0x00D46CC0, hkpRigidBody* rigidBody, UInt32 unk10);
   };
   class hkpMouseSpringAction : public hkpUnaryAction { // sizeof == 0x60
      //
      // Online sources state that this action is built into Havok and is specifically intended 
      // to let users drag a rigid body around with the mouse.
      //
      // Makes sense, then, that Bethesda uses a wrapper for this, bhkMouseSpringAction, to 
      // handle telekinesis and Z-keying.
      //
      // I don't see anything in here that's actually responsible for feeding the player and 
      // camera position and angle to the object, and I KNOW from testing that all three of 
      // those factors affect dragging. In third-person, rotating the camera influences the 
      // dragged object (though not NEARLY enough). Perhaps I need to look for code that 
      // loops over PlayerCharacter::Unk550, an array of bhkMouseSpringAction.
      //
      public:
         enum { kVTBL = 0x0116D014 };

         typedef void (*unkSubroutine)(hkpMouseSpringAction* self, UInt32, const hkVector4&); // called in a loop by Unk_03

         UInt32    unk1C; // 1C
         hkVector4 unk20; // 20
         hkVector4 unk30; // 30
         float  springDamping;     // 40
         float  springElasticity;  // 44
         float  maxForce = 250.0F; // 48 // initializes to the same value as Struct00734540::maxForce, but doesn't seem to be initialized from a Struct00734540 instance
         float  objectDamping;     // 4C
         UInt32 unk50 = 0xFFFFFFFF; // 50
         hkArray<unkSubroutine> unk54; // 54

         MEMBER_FN_PREFIX(hkpMouseSpringAction);
         DEFINE_MEMBER_FN(Constructor, hkpMouseSpringAction&, 0x00D67610,
            const hkVector4& unk20,
            const hkVector4& unk30,
            const float springDamping,
            const float springElasticity,
            const float objectDamping,
            hkpRigidBody* rigidBody,
            hkArray<unkSubroutine>* unk54_canBeNullptr
         );
         DEFINE_MEMBER_FN(Subroutine00D67200, void, 0x00D67200, const void* maybeNiPoint3);
   };

   struct Struct00734540 { // sizeof == 0x40
      //
      // Used to construct an hkpMouseSpringAction.
      //
      UInt32 unk00 = 0;
      hkpRigidBody* unk04 = 0; // 04 // Arg6 to hkpMouseSpringAction constructor
      void*  unk08; // 08
      UInt32 unk0C;
      hkVector4 unk10; // 10 // these four floats are treated as a unit, using XMM registers; gotta be an hkVector4
      hkVector4 unk20; // 20 // these four floats are treated as a unit, using XMM registers; gotta be an hkVector4
      float  springDamping    = 0.50F;  // 30 // Arg3 to hkpMouseSpringAction constructor // these are initialized from GMSTs for Z-keying or telekinesis
      float  springElasticity = 0.30F;  // 34 // Arg4 to hkpMouseSpringAction constructor
      float  maxForce         = 250.0F; // 38
      float  objectDamping    = 0.95F;  // 3C // Arg5 to hkpMouseSpringAction constructor

      MEMBER_FN_PREFIX(Struct00734540);
      DEFINE_MEMBER_FN(Constructor, Struct00734540&, 0x00734540);
   };

   class bhkAction : public bhkRefObject { // sizeof == 0x10
      public:
         enum { kVTBL = 0x010A719C };
         virtual UInt32 Unk_23(); // 23
         virtual UInt32 Unk_24(); // 24
         virtual bool   Unk_25(UInt32); // 25
         virtual bool   Unk_26(); // 26
         virtual void   Unk_27(bool); // 27
         virtual bool   Unk_28(); // 28 // on bhkAction, always returns true
         virtual UInt32 Unk_29(UInt32); // 29
         virtual void   Unk_2A(Struct00734540&) = 0; // 2A // pure
         virtual void   Unk_2B(void*) = 0; // 2B // pure // creates unk0C if it doesn't exist; returns it either way
         virtual void   Unk_2C(); // 2C
         virtual void   Unk_2D(UInt32); // 2D // tail call; return type unknown
         virtual void   Unk_2E(UInt32); // 2E

         void* unk0C = nullptr; // 0C // no vtbl; constructor at 00A49530
   };
   class bhkUnaryAction : public bhkAction { // sizeof == 0x10
      public:
         enum { kVTBL = 0x010A725C };

         MEMBER_FN_PREFIX(bhkUnaryAction);
         DEFINE_MEMBER_FN(AddToWorld, void, 0x00D4A070, void*); // called by Unk_25
   };
   class bhkMouseSpringAction : public bhkUnaryAction { // sizeof == 0x10
      public:
         enum { kVTBL = 0x010D1C7C };

         MEMBER_FN_PREFIX(bhkMouseSpringAction);
         DEFINE_MEMBER_FN(Constructor, bhkMouseSpringAction&, 0x00732EB0, Struct00734540&);
   };
};