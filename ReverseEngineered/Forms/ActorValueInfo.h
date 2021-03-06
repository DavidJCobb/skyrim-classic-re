#pragma once
#include "skse/GameForms.h"

#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   //
   // Some miscellaneous information:
   //
   //  - There can only be 254 Actor Values. Actor::Struct006F2190 uses a byte 
   //    array to map actor value indices to indices in an array of "state objects" 
   //    that are created and destroyed on the fly. Normally, that would limit the 
   //    number of possible actor value indices to 255, but the index -1 is used in 
   //    multiple places (e.g. magic effects) to denote "no actor value," and there 
   //    are parts of the actor value internals that check for it as well.
   //
   //     - This is before we even get into ActorValueInfos being hardcoded forms...
   //
   //  - In order to add more actor values, you would have to repoint the entire 
   //    ActorValueList singleton, or patch every single getter for actor values to 
   //    check an alternate source for indices above 163. In the former case, the 
   //    repointed list would be invisible to other SKSE plug-ins and to any SKSE-
   //    provided Papyrus functions.
   //
   //  - Try patching Actor::IncerceptActorValueChange to do fun things with actor 
   //    values, such as having damage drain from magicka before health. You can 
   //    still call ActorValueOwner::GetCurrent and friends from there in order to 
   //    do things like seeing whether a pending change will bring you below some 
   //    threshold (just don't change the current actor value, or you'll cause 
   //    recursion!).
   //
   typedef float (*OverrideAVBase)(ActorValueOwner*, UInt32 avIndex);
   //
   class ActorValueInfo : public TESForm {
      public:
         static constexpr form_type_t form_type = form_type::actor_value_info;
         struct flag {
            //
            // Notes:
            //
            //  - For actors other than the player, AVs missing the 80 and 800 flags cannot 
            //    change at run-time and are always pulled from the actorbase. This is 
            //    intentional; the checks are at 0x006DF8B6.
            //
            //  - None of the "Variable##" values have any flags set.
            //
            //  - Health only uses 0x040000 and 0x080000.
            //
            flag() = delete;
            enum type : uint32_t {
               unk00000002 = 0x00000002, // possibly "Is Magic/Spell School"       // every magic skill except Enchanting
               unk00000010 = 0x00000010, // possibly "Is Skill" or "Has Perk Tree" // every skill
               unk00000020 = 0x00000020, // IS NOT THE "PERSISTENT" FLAG           // Magicka, [Stat]Condition, Stamina,
               unk00000040 = 0x00000040, // if set, then the base value = (computeBaseFunc) result + ActorBase value; doesn't seem to actually be used by anything, though
               unk00000080 = 0x00000080,
               is_enum     = 0x00000100, // If true, then the only permitted values are integers in the range of [0, enumValueCount); this seems to only be enforced by Papyrus and console commands, though.
               is_negative = 0x00000020, // if set, then the value is inverted: damaging increases the value and restoring decreases it (the value passed in will be negated before it gets passed into ActorValueOwner), and damage modifiers are positive (with a minimum of zero) instead of being negative (with a maximum of zero) // only used for MovementNoiseMult and ShoutRecoveryMult
               unk00000800 = 0x00000800, // IS NOT THE "PERSISTENT" FLAG           // BowSpeedBonus, CarryWeight, CritChance, DamageResist, HealRate, Mass, MeleeDamage, [Stat]Condition, SpeedMult, StaminaRate, UnarmedDamage, VoiceRate,
               unk00002000 = 0x00002000, // IS NOT THE "PERSISTENT" FLAG           // DEPRECATED05, IgnoreCrippledLimbs, [Stat]Condition, 
               scripts_cant_modify = 0x00004000, // Papyrus APIs reject attempts to set AVs that have this flag.
               unk00008000 = 0x00008000, // DetectLifeRange, DiseaseResist, DragonSouls, ElectricResist, Fame, FireResist, FrostResist, IgnoreCrippledLimbs, Infamy, Invisibility, JumpingBonus, MagicResist, NightEye, Paralysis, PoisonResist, Telekinesis, WardDeflection, WardPower, WaterBreathing, WaterWalking,
               unk00020000 = 0x00020000, // IS NOT THE "PERSISTENT" FLAG           // PerceptionCondition, 
               unk00040000 = 0x00040000, // IS NOT THE "PERSISTENT" FLAG           // every skill, Aggression, Assistance, CarryWeight, HealRate, Health, Invisibility, Magicka, MagickaRate, Morality, Paralysis, SpeedMult, Stamina, StaminaRate, VoicePoints, VoiceRate, WardDeflection, WardPower, // ActorProcessManager is involved when working with these AVs' current values
               unk00080000 = 0x00080000, // possibly "Is Persistent"               // every skill, Health, Magicka, Stamina, VoicePoints, // ActorProcessManager is involved when working with these AVs' maximums
               do_not_reduce_in_god_mode = 0x00100000, // see virtual float PlayerCharacter::IncerceptActorValueChange(...)
               unk00200000 = 0x00200000,
            };
         };
         struct type {
            type() = delete;
            enum t : uint32_t {
               attribute       = 0, // e.g. Health, HealRate
               skill           = 1, // e.g. OneHanded, VampirePerks, WerewolfPerks
               ai              = 2, // e.g. Aggression, Energy, Morality
               stat            = 3, // e.g. PoisonResist
               limb_condition  = 4, // e.g. BrainCondition
               status          = 5, // e.g. DetectLifeRange, Invisibility, NightEye, Paralysis
               unk6            = 6, // e.g. ArmorPerks, BowSpeedBonus, Fame, Infamy, IgnoreCrippledLimbs, JumpingBonus, LeftItemCharge, [Skill]Mod, [Skill]SkillAdvance, Telekinesis, Variable01, WardPower
            };
         };
         using flags_t = std::underlying_type_t<flag::type>;
         using type_t  = std::underlying_type_t<type::t>;

         // Parents:
         TESFullName    fullName;    // 14
         TESDescription description; // 1C
         TESIcon        icon;        // 28

         struct UnkB0 { // sizeof == 0x10 // default values mapped out via DefineActorValueInfo(...); uses from SKSE
            float skillUseMult       =  1.0;
            float skillOffsetMult    =  0.0;
            float skillImproveMult   =  1.0;
            float skillImproveOffset = 20.0;
         };

         // Members:
         const char*      name;            // 30 - second ctor argument
         StringCache::Ref unk34;           // 34
         flags_t          actorValueFlags; // 38
         type_t           type;            // 3C
         OverrideAVBase*  computeBaseFunc; // 40 // optional; used to return the base value when it can vary (e.g. HealRate pulls from the actor's race); note that if the AV doesn't have flags 0x80 or 0x800, it will only use this for the player, and all other actors pull from their ActorBase
         UInt32           unk44;           // 44 // unknown; minimum allowed value is 0xF
         UInt32           unk48[0x0F];     // 48 // initialized to 0xA4
         UInt32           enumValueCount;  // 84
         const char*      enumValues[0xA]; // 88
         UnkB0*           unkB0;           // B0
         UInt32           indexInFlag00040000List; // B4 // If the actor value is created with flag 0x40000, this is set to *(0x01B1E0E4), and then that value is incremented.
         UInt32           indexInFlag00080000List; // B8 // If the actor value is created with flag 0x80000, this is set to *(0x01B1E0E8), and then that value is incremented.
         BGSSkillPerkTreeNode* perkTree;   // BC

         UInt8	padC0[8];                   // C0 // not initialized

         // subroutines:
         // InventoryWeight: 0x006A8680
   };
   static_assert(sizeof(ActorValueInfo) >= 0xC8, "RE::ActorValueInfo is too small.");
   static_assert(sizeof(ActorValueInfo) <= 0xC8, "RE::ActorValueInfo is too large.");
   static_assert(offsetof(ActorValueInfo, unkB0) >= 0xB0, "RE::ActorValueInfo::unkB0 is too early.");
   static_assert(offsetof(ActorValueInfo, unkB0) <= 0xB0, "RE::ActorValueInfo::unkB0 is too late.");

   DEFINE_SUBROUTINE_EXTERN(bool, ComputeDynamicBaseActorValue, 0x005AD640, ActorValueOwner* subject, UInt32 avIndex, float* out); // calls the appropriate (computeBaseFunc), if any; returns true if a variable value is computed
   //
   // I'm listing these here, but it's probably a bad idea to go around defining custom actor values. AVs have fixed 
   // form IDs, and I'm not sure how the game generates those. There may not be room for new AVs even if the places 
   // they're stored can expand.
   //
   DEFINE_SUBROUTINE_EXTERN(ActorValueInfo*, DefineActorValueInfo,    0x005AD710, void*, UInt32 index, char* name, UInt32, UInt32 flags, void* subroutine, UInt32);
   DEFINE_SUBROUTINE_EXTERN(void,            DefineActorValueEnum,    0x005AD080, ActorValueInfo*, ...); // defines enum values (char*) for an enum actor value
   DEFINE_SUBROUTINE_EXTERN(ActorValueInfo*, GetActorValueByIndex,    0x005AD350, UInt32 index);
   DEFINE_SUBROUTINE_EXTERN(UInt32,          IndexOfActorValueByName, 0x005AD5F0, const char* name);

   typedef void(*ActorValueChangeHandler)(Actor*, UInt32 avIndex, float oldVal, float changedBy, UInt32 unk);
   extern ActorValueChangeHandler* const g_actorValueChangeHandlers; // as in: (g_actorValueChangeHandlers[avIndex])(...);
};