#pragma once
#include "skse/NiTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered\Forms\BGSBodyPartData.h"

namespace RE {
   class Actor;
   class Projectile;
   class TESObjectREFR;
   class TESObjectWEAP;
   struct Struct00797220 { // sizeof == 0x78
      NiPoint3 unk00;
      NiPoint3 unk0C; // 0C // set to Projectile::unk94 and then vector-normalized; direction of movement?
      UInt32 attackerRefHandle   = 0; // 18 // refhandle -- attacking actor
      UInt32 attackedRefHandle   = 0; // 1C // refhandle -- attacked  actor
      UInt32 projectileRefHandle = 0; // 20 // refhandle -- projectile
      UInt32 unk24 = 0;
      TESObjectWEAP* unk28 = nullptr; // 28
      UInt32 unk2C = 0;
      UInt32 unk30 = 0;
      void*  unk34 = nullptr;
      float  damage = 0.0F; // 38 // most damage-modifying GMSTs are applied to this
      UInt32 unk3C = 0;    // 3C // weapon data -- the result of the Calculate Weapon Damage perk entry point
      float  unk40 = 0.0F; // 40 // fCombatPlayerLimbDamageMult is applied to this
      UInt32 unk44 = 0;
      UInt32 unk48 = 0;
      UInt32 unk4C = 0;
      float  unk50 = 0.0F;
      float  unk54 = 1.0F;
      float  unk58 = 1.0F;
      UInt32 unk5C = 0;
      UInt32 unk60 = 0;
      UInt32 unk64 = 0;
      UInt32 unk68 = 0; // 68 // flags
      UInt32 unk6C = 0;
      SInt32 unk70 = -1; // 70 // actor value i.e. the weapon skill, unless the weapon is the Unarmed form, in which case == -1
      SInt32 unk74 = -1; // 74 // this is BGSBodyPartData::PartType as an SInt32? see code at 00797CBA

      MEMBER_FN_PREFIX(Struct00797220);
      DEFINE_MEMBER_FN(Constructor, Struct00797220*, 0x00797220);
      DEFINE_MEMBER_FN(Subroutine00798180, void, 0x00798180); // does most damage calculations
      DEFINE_MEMBER_FN(Subroutine00798AC0, void, 0x00798AC0, Actor* attacker, Actor* attacked, Projectile* projectile); // one of the callers for 00798180
      DEFINE_MEMBER_FN(Subroutine00797BC0, void, 0x00797BC0, Projectile*); // computes limb damage?
   };
};