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

   struct Struct007A6F60;
   struct Struct00797220 { // sizeof == 0x78
      enum Flags : UInt32 {
         kFlag_00000001 = 0x00000001, // flag is set at 00797888
         kFlag_00000004 = 0x00000004, // based on Projectile flag 0x08000000
         kFlag_IsCriticalHit     = 0x00000008, // flag is set at 00798479 // checked by GetLastHitCritical
         kFlag_CritEffectOnDeath = 0x00000010, // flag is set at 007984E4
         kFlag_00000020 = 0x00000020, // flag is set at 00798702
         kFlag_00000040 = 0x00000040, // flag is set at 00798744
         kFlag_00000080 = 0x00000080, // flag is set at 0079874F
         kFlag_00000100 = 0x00000100, // flag is set at 007986A9 if the limb damage (unk40) is enough to drop the target limb's health to or below 0, but only if the limb is not already at 0, and only if the limb is flagged as explodable
         kFlag_IsSneakAttack = 0x00000800, // flag is set at 007983D1; checked at 006E495B // related to sneak attacks
         kFlag_IsPowerAttack = 0x00010000, // checked at 00797928
         kFlag_00020000 = 0x00020000, // flag is set at 0079880E // related to soul trapping?
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
      Struct007A6F60* unk34 = nullptr; // 34
      float  damage = 0.0F; // 38 // most damage-modifying GMSTs are applied to this; it's probably damageBase or damageToActor or such
      float  unk3C  = 0.0F; // 3C // one of the damage values, possibly damage done by the weapon (i.e. without most modifiers)
      float  damageToTargetLimb = 0.0F; // 40 // fCombatPlayerLimbDamageMult is applied to this; a percentage of this damage is also directly dealt to the actor on top of the normal damage
      float  damageBlocked      = 0.0F; // 44 // see ComputeDamageBlocked; seems like this is stored to allow for Block skill advancement
      float  damageToArmorItems = 0.0F; // 48 // the damage that should be taken by the armor itself (for equipment degradation); set at 0079771D
      float  damageToWeaponItem = 0.0F; // 4C // the damage taken by the weapon (for equipment degradation); see ComputeDamageTakenByWeapon
      float  unk50 = 0.0F; // 50 // possibly one of the damage values // set at 00798A43
      float  unk54 = 1.0F;
      float  unk58 = 1.0F;
      UInt32 unk5C = 0;
      UInt32 unk60 = 0;
      float  reflectedDamage = 0.0F; // 64 // amount of damage reflected back at attacker == damage * (Victim:ActorValue:ReflectDamage / 100.0F)
      UInt32 flags = 0; // 68
      UInt32 unk6C = 0;
      SInt32 unk70 = -1; // 70 // actor value i.e. the weapon skill, unless the weapon is the Unarmed form, in which case == -1
      SInt32 unk74 = -1; // 74 // this is BGSBodyPartData::PartType as an SInt32? see code at 00797CBA

      MEMBER_FN_PREFIX(Struct00797220);
      DEFINE_MEMBER_FN(Constructor, Struct00797220*, 0x00797220);
      DEFINE_MEMBER_FN(Copy,        Struct00797220*, 0x00797540, const Struct00797220& other); // copy-constructor
      DEFINE_MEMBER_FN(Destructor,  void, 0x00797240);
      DEFINE_MEMBER_FN(SetFlags,    void,            0x006E0B90, UInt32 flags, bool clearOrSet); // apparently only used in one place :(
      DEFINE_MEMBER_FN(Subroutine007975E0, void, 0x007975E0, UInt32); // computes armor degradation and several damage-related values
      DEFINE_MEMBER_FN(Subroutine00797BC0, void, 0x00797BC0, Projectile*); // computes limb damage?
      DEFINE_MEMBER_FN(Subroutine00798180, void, 0x00798180); // does most damage calculations, as well as critical hit chance/result
      DEFINE_MEMBER_FN(Subroutine00798580, void, 0x00798580); // sets some flags; may handle limb explosion chance
      DEFINE_MEMBER_FN(Subroutine007987B0, void, 0x007987B0, UInt32, UInt32, UInt32, UInt32); // computes weapon degradation and some other things
      DEFINE_MEMBER_FN(Subroutine00798AC0, void, 0x00798AC0, Actor* attacker, Actor* attacked, Projectile* projectile); // one of the callers for 00798180
      DEFINE_MEMBER_FN(Subroutine00798EB0, void, 0x00798EB0, UInt32); // modifies unk34
      DEFINE_MEMBER_FN(Subroutine00798F40, void, 0x00798F40, void* Arg1, Actor* attacked); // initializes from Arg1; Arg1 is of the same type as the arg to TESObjectREFR::Unk_58

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
      // THINGS TO RESEARCH:
      //
      //  - Where is the damage actually applied?
      //
      //     - Actor::006E4470 is a very promising candidate to examine.
      //     - 006E7327
      //
      //     - It SEEMS like the damage is applied using not ActorValueOwner, but the internal Actor 
      //       method (TESV_006E0760) that ActorValueOwner calls when you use it to apply damage. 
      //       However, that may only be true for reflected damage and maybe limb damage.
      //
      //     - The actor takes limb damage at 0x006E4BAB. That is: a certain amount of damage is 
      //       (presumably) done to the limb (by damaging the actor value associated with the limb), 
      //       and then a percentage of THAT damage is done to the actor as bonus damage. The percent-
      //       age is controlled by BGSBodyPartData::Data::BPND::healthPercent.
      //
      //  - How exactly is equipment degradation applied? How does the game take an item, as listed in 
      //    an ExtraContainerChanges, and reduce its health?
      //
      //     - It would be worthwhile to reverse-engineer Oblivion for this, since I've done so much 
      //       work in that game already. I'm pretty sure some of the conditions for checking hit 
      //       details also exist in Oblivion, so we can look at those to quickly locate the hit 
      //       struct in that game. From there, it *should* be *relatively* trivial to find the code 
      //       that sets equipment damage, and the code that applies it.
      //
      //       I assume that we'd need to attach an ExtraHealth to the items in order to track their 
      //       degradation -- and we'd need plenty of hooks elsewhere to forcibly unequip them when 
      //       they break, prevent them from being equipped once they're broken, AND we'd need to 
      //       design a way to repair them -- and that's what Oblivion would help us with: how do we 
      //       apply a new ExtraHealth to just a single one of an item, when the user may potentially 
      //       have multiple identical copies of the item (i.e. how do we separate out and modify 
      //       just one)? The harder concern would of course be serialization: if the game doesn't 
      //       save ExtraHealth, then we'd need to hook the game and find some way to save it without 
      //       making the save file backwards-incompatible.
      //
      //        - I'd expect the GetLastHitCritical condition to exist in Oblivion.
      //
   };

   struct Struct007A6F60 { // sizeof == 0x98; possibly struct for a VATS hit
      UInt32 unk00 = 0xC;
      UInt8  unk04 = 0;
      UInt8  unk05 = 0;
      UInt8  unk06 = 0;
      UInt8  unk07 = 0;
      UInt8  unk08 = 0;
      UInt8  unk09 = 0;
      UInt16 pad0A;
      UInt32 unk0C = 0;
      SInt32 unk10 = -1; // 10 // actor value index, used to lookup a body part; VATS groups body parts by actor value, so this could be "VATS-targeted body part AV"
      Struct00797220 unk14;
      UInt32 unk8C = 0;
      UInt32 unk90 = 0;
      UInt32 unk94 = 0;
   };
};