#pragma once
#include "skse/NiTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered\Forms\BGSBodyPartData.h"
#include "ReverseEngineered\Shared.h"

namespace RE {
   class Actor;
   class Projectile;
   class TESObjectREFR;
   class TESObjectWEAP;

   // Given the amount of damage that an attack does, and the target's armor rating as a 
   // percentage, this function returns the amount of damage that the armor items should 
   // take (i.e. for equipment degradation). This is called during attack processing and 
   // the result is written to a struct that contains all information about the attack, 
   // although if the attack was unarmed, then the value is not stored (i.e. the game is 
   // intended not to let unarmed attacks damage equipment).
   //
   DEFINE_SUBROUTINE_EXTERN(float, ComputeDamageTakenByArmor, 0x00599A80, float attackDamage, float armorRatingPct);

   // This subroutine computes the amount of damage that a weapon should take (i.e. for 
   // equipment degradation) with every strike or shot. The damage taken by the weapon 
   // is equal to the damage that the weapon deals with each attack, multiplied based 
   // on the weapon type: melee weapons use the fDamageToWeaponMeleeMult gamesetting as 
   // their multiplier, while bows and crossbows use fDamageToWeaponGunMult. Weapons of 
   // any type can override their multiplier (see flags and value on TESObjectWEAP), 
   // and the default multiplier for staves is 1.0. Some weapon settings can prevent a 
   // weapon from degrading (e.g. the "embedded weapon" flag).
   //
   DEFINE_SUBROUTINE_EXTERN(float, ComputeDamageTakenByWeapon, 0x00599A10, TESObjectWEAP* baseForm);

   // Computes either the amount of damage blocked or the percentage of damage blocked. 
   // Functionality is hard to describe in prose, so here's the code:
   //
   //    float arDiff = GMST:fArmorRatingMax - GMST:fArmorRatingBase;
   //    float a = GMST:fArmorRatingBase + (0.01 * blockAV * arDiff); // Block ranges from 0 to 100, so multiply by 0.01 first to use it as a percentage
   //    float b;
   //    if (!arg5 && !arg4) {
   //       b = (a + shieldPerksAV) * weaponDamage;
   //       b = (GMST:fShieldScalingFactor * b * 0.01) + GMST:fShieldBaseBase; // guessing fShieldScalingFactor is also from 0 to 100
   //    } else {
   //       b = a * weaponDamage;
   //       b = (GMST:fBlockWeaponScaling * b * 0.01) + GMST:fBlockWeaponBase; // guessing fBlockWeaponScaling is also from 0 to 100
   //    }
   //    if (isPowerAttack)
   //       b *= GMST:fBlockPowerAttackMult;
   //    return b;
   //
   DEFINE_SUBROUTINE_EXTERN(float, ComputeDamageBlocked, 0x00599E80, float weaponDamage, float blockAV, float shieldPerksAV, bool arg4, bool arg5, bool isPowerAttack);

   struct Struct00797220 { // sizeof == 0x78
      enum Flags : UInt32 {
         kFlag_00000001 = 0x00000001, // flag is set at 00797888
         kFlag_00000004 = 0x00000004, // based on Projectile flag 0x08000000
         kFlag_00000020 = 0x00000020, // flag is set at 00798702
         kFlag_00000040 = 0x00000040, // flag is set at 00798744
         kFlag_00000080 = 0x00000080, // flag is set at 0079874F
         kFlag_00000100 = 0x00000100, // flag is set at 007986A9 if the limb damage (unk40) is enough to drop the target limb's health to or below 0, but only if the limb is not already at 0, and only if the limb is flagged as explodable
         kFlag_IsPowerAttack = 0x00010000, // checked at 00797928
      };

      NiPoint3 unk00;
      NiPoint3 unk0C; // 0C // set to Projectile::unk94 and then vector-normalized; direction of movement?
      UInt32 attackerRefHandle   = 0; // 18 // refhandle -- attacking actor
      UInt32 attackedRefHandle   = 0; // 1C // refhandle -- attacked  actor
      UInt32 projectileRefHandle = 0; // 20 // refhandle -- projectile
      void*          unk24 = nullptr; // 24 // refcounted object; the return value of Actor::Subroutine006E1860
      TESObjectWEAP* unk28 = nullptr; // 28
      UInt32 unk2C = 0;
      UInt32 unk30 = 0;
      void*  unk34 = nullptr;
      float  damage = 0.0F; // 38 // most damage-modifying GMSTs are applied to this; it's probably damageBase or damageToActor or such
      float  unk3C  = 0.0F; // 3C // one of the damage values, possibly damage done by the weapon (i.e. without most modifiers)
      float  damageToTargetLimb = 0.0F; // 40 // fCombatPlayerLimbDamageMult is applied to this
      float  damageBlocked      = 0.0F; // 44 // see ComputeDamageBlocked
      float  damageToArmorItems = 0.0F; // 48 // the damage that should be taken by the armor itself (for equipment degradation); set at 0079771D
      float  damageToWeaponItem = 0.0F; // 4C // the damage taken by the weapon; see ComputeDamageTakenByWeapon
      float  unk50 = 0.0F; // 50 // possibly one of the damage values
      float  unk54 = 1.0F;
      float  unk58 = 1.0F;
      UInt32 unk5C = 0;
      UInt32 unk60 = 0;
      float  unk64 = 0.0F; // 64 // related to the ReflectDamage AV; see 00797A7C; apparently set to (unk38 * ReflectDamage / 100.0F)
      UInt32 unk68 = 0; // 68 // flags
      UInt32 unk6C = 0;
      SInt32 unk70 = -1; // 70 // actor value i.e. the weapon skill, unless the weapon is the Unarmed form, in which case == -1
      SInt32 unk74 = -1; // 74 // this is BGSBodyPartData::PartType as an SInt32? see code at 00797CBA

      MEMBER_FN_PREFIX(Struct00797220);
      DEFINE_MEMBER_FN(Constructor, Struct00797220*, 0x00797220);
      DEFINE_MEMBER_FN(Copy,        Struct00797220*, 0x00797540, const Struct00797220& other); // copy-constructor
      DEFINE_MEMBER_FN(SetFlags,    void,            0x006E0B90, UInt32 flags, bool clearOrSet); // apparently only used in one place :(
      DEFINE_MEMBER_FN(Subroutine007975E0, void, 0x007975E0, UInt32); // computes armor degradation and several damage-related values
      DEFINE_MEMBER_FN(Subroutine00798180, void, 0x00798180); // does most damage calculations
      DEFINE_MEMBER_FN(Subroutine00798580, void, 0x00798580); // sets some flags; may handle limb explosion chance
      DEFINE_MEMBER_FN(Subroutine007987B0, void, 0x007987B0, UInt32, UInt32, UInt32, UInt32); // computes weapon degradation and some other things
      DEFINE_MEMBER_FN(Subroutine00798AC0, void, 0x00798AC0, Actor* attacker, Actor* attacked, Projectile* projectile); // one of the callers for 00798180
      DEFINE_MEMBER_FN(Subroutine00797BC0, void, 0x00797BC0, Projectile*); // computes limb damage?

      //
      // FUN FACTS:
      //
      //  - In 0x00798AC0, the damage float (unk38) is scaled by, well, the attacker's scale! If the 
      //    attacker has been physically scaled to 50% size, then they'll do 50% damage. This only 
      //    affects the immediate scale on the TESObjectREFR, so racial height differences and any 
      //    RaceMenu skeletal scaling modifiers won't make a difference.
      //
      //    It doesn't look like the target's scale affects anything, but maybe I just haven't found 
      //    that yet.
      //
      //  - Damage taken by armor is computed and set at 0079771D. Unarmed attacks cannot damage the 
      //    armor worn by an enemy, and attacks with no TESObjectWEAP can't either.
      //
   };
};