#pragma once
#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/Shared.h"

class bhkSimpleShapePhantom;
class bhkSphereShape;
class BGSProjectile;
namespace RE {
   class Actor;

   struct Struct005ACB60 { // sizeof == 0x18
      UInt32   unk00 = 0; // 00 // refHandle
      NiPoint3 pos; // 04
      UInt32   index; // 10 // this is set to a counter at 0x01B1E0D8, which is then incremented; it is, presumably, our index in (tList*)0x01B1E0DC.
      UInt32   unk14 = 0; // 14

      MEMBER_FN_PREFIX(Struct005ACB60);
      DEFINE_MEMBER_FN(Constructor, void, 0x005ACB60, Actor* shooter);
   };
   struct Struct0073E790 { // sizeof == 0x30, apparently
      UInt32 unk00 = 0;
      UInt32 unk04 = 0;
      tArray<UInt32> unk08; // 08
      tArray<UInt32> unk14; // 14
      tArray<UInt32> unk20; // 20
      UInt8  unk2C = 0;
      UInt8  unk2D;
      UInt8  unk2E;
      UInt8  unk2F;
   };

   class Projectile : public TESObjectREFR { // sizeof == 0x140
      //
      // Notes on projectiles:
      //
      //  - Using the console to modify a projectile's pitch mid-flight will 
      //    fail: nothing will change.
      //
      //  - Using the console to modify a projectile's yaw mid-flight will 
      //    produce a visual change, but will not affect the projectile's 
      //    movement.
      //
      //  - Projectiles that are disabled mid-flight will resume their original 
      //    path of movement when enabled, even if their angle was changed via 
      //    the console.
      //
      // Investigate ArrowProjectile::Unk_AB; it (eventually) calls down to the 
      // function that seems to do timesteps for projectiles in flight, which is 
      // Projectile::Subroutine007A24C0(float maybeIngameTime).
      //
      public:
         virtual ~Projectile();
         virtual bool   Unk_A2();
         virtual bool   Unk_A3();
         virtual bool   Unk_A4();
         virtual bool   Unk_A5();
         virtual bool   Unk_A6();
         virtual bool   Unk_A7();
         virtual void   Unk_A8();
         virtual void   Unk_A9();
         virtual void   Unk_AA(UInt32);
         virtual void   Unk_AB(void); // pure // might be a frame handler
         virtual bool   Unk_AC();
         virtual void   Unk_AD();
         virtual bool   Unk_AE();
         virtual void   Unk_AF();
         virtual float  Unk_B0();
         virtual bool   Unk_B1();
         virtual void   Unk_B2();
         virtual bool   Unk_B3(UInt32, UInt32); // related to sound
         virtual bool   Unk_B4();
         virtual float  Unk_B5();
         virtual void   Unk_B6();
         virtual bool   Unk_B7(); // related to raycasts?
         virtual bool   Unk_B8();
         virtual bool   Unk_B9();
         virtual bool   Unk_BA();
         virtual UInt32 Unk_BB() const; // BB // actually returns an integer
         virtual bhkSphereShape* Unk_BC(); // BC // Reuses a bhkSphereShape based on the base projectile's data, or creates one if none of the existing ones are appropriate.
         virtual UInt32 Unk_BD(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         virtual bool   Unk_BE(UInt32);
         virtual void   Unk_BF();
         virtual void   Unk_C0();
         virtual bool   Unk_C1();

         class LaunchData {
            public:
               virtual ~LaunchData();
               //
               NiPoint3 position; // 04 // where the projectile is fired from
               NiPoint3 unk10; // 10
               BGSProjectile* projectileBase = nullptr; // 1C
               Actor*   unk20 = nullptr; // 20 // shooter?
               void*    unk24 = nullptr; // 24
               TESObjectWEAP* shooterWeapon = nullptr; // 28
               TESAmmo*       shooterAmmo   = nullptr; // 2C
               float    yaw   = 0; // 30 // launch yaw
               float    pitch = 0; // 34 // launch pitch
               void*    unk38 = nullptr; // 38 // refcounted object; becomes unkB0 on the projectile
               TESObjectREFR* unk3C = nullptr; // 3C
               UInt32   unk40 = 0;
               UInt32   unk44 = 0;
               TESObjectCELL* createInCell = nullptr;
               void*    unk4C = nullptr;
               UInt32   unk50 = 4;
               UInt32   unk54 = 0; // used for ArrowProjectile 148, maybe other types as well
               UInt32   unk58 = 0; // used for ArrowProjectile 14C, maybe other types as well
               SInt32   unk5C = 0;
               float    unk60 = 1.0F;
               float    unk64 = 1.0F;
               UInt8    unk68 = 0;
               UInt8    unk69 = 0;
               UInt8    unk6A = 1;
               UInt8    unk6B = 0;
               UInt8    unk6C = 0;
               UInt8    unk6D = 0;
               UInt8    unk6E = 0;
               UInt8    pad6F;
               //
               // The (position), (yaw), and (pitch) fields are passed to TESObjectREFR::SetPosition, 
               // TESObjectREFR::SetYaw, and TESObjectREFR::SetPitch by the Projectile constructor.
               //
         };
         //
         /*
         Projectile() {};
         Projectile(LaunchData& data) {
            this->unkE4  = data.unk4C;
            this->unkE8  = data.unk50;
            this->unk134 = data.unk64;
            RE::Actor* shooter = data.unk20;
            CALL_MEMBER_FN(this, SetPosition)(&data.unk04);
            CALL_MEMBER_FN(this, SetYaw)(data.unk30);
            CALL_MEMBER_FN(this, SetPitch)(data.unk34);
            CALL_MEMBER_FN(this, SetRoll)(0);
            this->unk100 = data.projectileBase->data.unk08; // most likely speed
            if (this->unkE4)
               this->unkEC = this->unkE4->TESV_00406C70();
            if (!shooter || (shooter->handleRefObject.m_uiRefCount & 0x3FF) == 0) {
               this->refHandleShooter = *g_invalidRefHandle;
            } else {
               CreateRefHandleByREFR(&this->refHandleShooter, (::TESObjectREFR*) shooter);
            }
            this->SetBaseForm(data.projectileBase);
            if (data.shooterWeapon) {
               this->weaponBase = data.shooterWeapon;
               this->ammoBase = data.shooterAmmo;
               float ebp = 1.0F;
               ActorValueOwner* avOwner;
               if (shooter && shooter->formType == kFormType_Character) {
                  auto ai = shooter->processManager;
                  if (ai) {
                     auto eax = ecx->TESV_0071EDD0();
                     if (eax) {
                        auto eax = shooter->processManager->TESV_0071EDD0(0);
                        ebp = eax->TESV_00475360();
                     }
                     avOwner = &shooter->actorValueOwner;
                  }
               } else
                  avOwner = nullptr;
               this->unk10C = TESV_0059A6B0(avOwner, data.shooterWeapon, data.shooterAmmo, ebp, 1.0, false); // computes projectile base damage?
               auto  eax = this->weaponBase->gameData.unk00;
               UInt8 ecx = eax ? eax->unk18 : 1;
               this->unk10C /= ecx;
            }
            if (shooter && shooter->formType == kFormType_Character) {
               if (shooter->IsInCombat()) {
                  if (shooter != *(RE::Actor**)0x01310588)
                     this->unk138 |= 0x80000;
               }
            }
            if (data.unk6C) {
               this->unk138 |= 0x20000000;
               CALL_MEMBER_FN(this, ModifyFlag00000008)(false);
               return;
            }
            this->unk138 &= ~0x20000000;
            CALL_MEMBER_FN(this, ModifyFlag00000008)(false);
         }
         */
         //
         BStList<UInt32> unk54; // 54
         NiTransform unk5C; // 5C
         bhkSimpleShapePhantom* unk90 = nullptr; // 90
         NiPoint3 unk94; // 94
         NiPoint3 unkA0; // A0
         UInt32 unkAC = 0;
         void*  unkB0 = nullptr; // B0 // NiPointer; don't know what it points to
         void*  unkB4 = nullptr; // B4 // no VTBL
         UInt32 refHandleShooter = 0; // B8
         UInt32 unkBC = 0; // BC // refHandle; taken from LaunchData::unk3C
         UInt32 unkC0 = 0xFFFFFFFF; // C0
         UInt8 unkC4 = 0; // C4 // constructors treat it like a byte, but it looks VERY much like a float or pointer; union, perhaps?
         UInt8 unkC5; // C5
         UInt8 unkC6;
         UInt8 unkC7;
         UInt32 unkC8 = 0; // C8 // actually a number
         UInt32 unkCC = 0xFFFFFFFF; // CC
         UInt8 unkD0 = 0; // D0
         UInt8 unkD1;
         UInt8 unkD2;
         UInt8 unkD3;
         UInt32 unkD4 = 0;
         UInt32 unkD8 = 0;
         UInt32 unkDC = 0;
         UInt32 unkE0 = 0; // E0 // related to explosions
         void*  unkE4; // E4 // LaunchData::unk4C
         UInt32 unkE8; // E8
         void*  unkEC = nullptr; // EC // related to explosions
         void*  unkF0 = nullptr; // F0 // QueuedFile*?
         UInt32 unkF4 = 0;
         UInt32 unkF8 = 0;
         float  unkFC = 1.0F; // FC // related to pitch / arrow-drop
         float  unk100 = 1.0F;
         float  range; // 104 // game engine likes to constrain this to [0.0F, 10000.0F], but fired arrows have 60000.0F
         float  unk108 = 0.0F;
         float  damage = 0.0F; // 10C // constructor sets this based on the weapon, ammo, and shooter. if a weapon does X damage and fires Y projectiles per shot, then each projectile does X/Y damage. projectiles with no weapon deal no damage.
         float  unk110 = 1.0F;
         float  unk114 = FLT_MAX;
         UInt32 unk118 = 0;
         UInt32 unk11C;
         UInt32 unk120;
         TESObjectWEAP* weaponBase = nullptr; // 124
         TESAmmo*       ammoBase   = nullptr; // 128
         float  unk12C = 0.0F; // 12C // very likely "distance traveled"
         UInt32 unk130 = 0;
         float  unk134; // 134
         UInt32 unk138 = 0; // 138 // flags
         UInt8  unk13C = 0;
         UInt8  unk13D = 0;
         UInt8  pad13E;
         UInt8  pad13F;
         //
         MEMBER_FN_PREFIX(Projectile);
         DEFINE_MEMBER_FN(ApplyProjectileGravity, void, 0x0079CF90, NiPoint3&, float); // no-oping this causes arrows to fly perfectly straight
         DEFINE_MEMBER_FN(Kill,                   void, 0x0079DFA0); // deletes or disables the projectile as appropriate
         DEFINE_MEMBER_FN(MoveToHitPositionIfAny, void, 0x007A0090, const NiPoint3& raycastStart, const NiPoint3& raycastEnd);
         DEFINE_MEMBER_FN(SetConstrainedPitch,    void, 0x0079C210, float); // will not exceed 89deg
         DEFINE_MEMBER_FN(SetProjectileFlag, void, 0x0068DD10, UInt32 flags, bool);
         DEFINE_MEMBER_FN(SetUnkB0,          void, 0x0079C4A0, void*); // receives bare pointer; writes to smart pointer
         DEFINE_MEMBER_FN(SetUnk90,          void, 0x006E1150, bhkSimpleShapePhantom*);
         DEFINE_MEMBER_FN(SetUnk90Position,  void, 0x0079CED0, const NiPoint3&);
   };

   // 150
   class BarrierProjectile : public Projectile {
      enum { kTypeID = kFormType_BarrierProj };
   };

   class BeamProjectile : public Projectile { // sizeof == 0x178
      public:
         enum { kTypeID = kFormType_BeamProj };
         //
         /*
         BeamProjectile() {};
         BeamProjectile(LaunchData& data) {
            this->unk174 = data.unk6E;
         }
         */
         //
         UInt32 unk140; // 140 // this is a BSTEventSink<BSProceduralGeomEvent>, a class that consists solely of a vtbl
         BSTEventSource<UInt32> unk144; // 144
         UInt8  unk174 = 0; // 174
   };

   // 16C
   class ConeProjectile : public Projectile {
      public:
         enum { kTypeID = kFormType_ConeProj };
   };

   class FlameProjectile : public Projectile { // sizeof == 0x148
      public:
         enum { kTypeID = kFormType_FlameProj };
   };

   class GrenadeProjectile : public Projectile { // sizeof == 0x148
      public:
         enum { kTypeID = kFormType_Grenade };
   };

   
   class MissileProjectile : public Projectile { // sizeof == 0x148
      public:
         enum { kTypeID = kFormType_Missile };

         UInt32 unk140;
         UInt8  unk144; // looks like a pointer in memory; possible union
         UInt8  unk145[3];
   };
   class ArrowProjectile : public MissileProjectile { // sizeof == 0x150
      public:
         enum { kTypeID = kFormType_Arrow };

         UInt32 unk148;
         UInt32 unk14C;
   };

   DEFINE_SUBROUTINE_EXTERN(UInt32&, LaunchProjectile, 0x007A53F0, UInt32& outHandle, Projectile::LaunchData*);
}