#pragma once
#include <cstdint>
#include <type_traits>
#include "skse/GameObjects.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Forms/MagicItem.h"
#include "ReverseEngineered/Systems/Magic.h"
#include "ReverseEngineered/Types.h"

namespace RE {
   class Actor;
   class BGSLoadGameBuffer;
   class ModelReferenceEffect;
   class NiNode;
   class TESObjectREFR;

   class ActiveEffectReferenceEffectController {
      public:
         virtual ~ActiveEffectReferenceEffectController();
         // ...

         ActiveEffect* effect; // 04
         UInt32 refHandle; // 08
         //
         MEMBER_FN_PREFIX(ActiveEffectReferenceEffectController);
         DEFINE_MEMBER_FN(SetHandle, void, 0x00668F70, TESObjectREFR*);
   };

   class ActiveEffect {
      public:
         static constexpr uint8_t form_type = kFormType_ActiveMagicEffect;
         enum { kTypeID = kFormType_ActiveMagicEffect };

         enum class Flags : UInt32 {
            none = 0,
            //
            unk_05   = 0x00000020, // 1 << 0x05
            unk_06   = 0x00000040, // 1 << 0x06
            unk_07   = 0x00000080, // 1 << 0x07
            unk_08   = 0x00000100, // 1 << 0x08
            //
            unk_0D   = 0x00002000, // 1 << 0x0D
            unk_0E   = 0x00004000, // 1 << 0x0E
            inactive = 0x00008000, // 1 << 0x0F
            unk_10   = 0x00010000, // 1 << 0x10
            unk_11   = 0x00020000, // 1 << 0x11
            unk_12   = 0x00040000, // 1 << 0x12
            //
            unk_1F   = 0x80000000, // 1 << 0x1F // if set, prevents a call to Unk_05 and consequently the updating of conditions
         };
         using flags_t = std::underlying_type_t<Flags>;

         virtual void ApplyPerkEffects(UInt32 arg1, MagicTarget* arg2); // 00 // modifies duration and magnitude based on the following perk entry points: mod spell duration; mod spell magnitude; mod incoming spell duration; mod incoming spell magnitude
         virtual void Unk_01(UInt32 arg1); // 01 // conditionally modifies flags at this->item->unk54
         virtual void Unk_02();   // 02 // no-op on base class
         virtual Actor* GetTargetActor(); // 03 // returns this->actorTarget->GetTargetActor(); -- most likely the target
         virtual void Unk_04(UInt32 arg1); // 04 // no-op on base class
         virtual void Unk_05(float realTimeSeconds, bool); // 05 // calls DoConditionUpdate and, when needed, TurnOff; arg1 type assumed
         virtual bool Unk_06(); // 06 // no-op on base class; returns false
         virtual void Unk_07(UInt32 arg1); // 07 // no-op on base class
         virtual void Unk_08(void* arg1); // 08 // related to saving data
         virtual void Unk_09(BGSLoadGameBuffer* arg1); // 09
         virtual void Unk_0A(UInt32 arg1); // 0A
         virtual void Unk_0B(UInt32 arg1); // 0B // on the base class, just sets unk60 to 4
         virtual void Unk_0C(UInt32 arg1); // 0C // seems to print data about the effect somewhere; uses 1000.0F as defaults for duration, magnitude, etc., if some flags are set
         virtual void Unk_0D(UInt32 arg1); // 0D // no-op on base class
         virtual void Unk_0E(UInt32 arg1, UInt32 arg2); // 0E // no-op on base class
         virtual void Unk_0F(); // 0F // no-op on base class
         virtual bool Unk_10(); // 10 // checks a flag on the effect
         virtual bool Unk_11(); // 11 // no-op on base class; returns false
         virtual void Unk_12(); // 12 // no-op on base class
         virtual void Dispose(bool); // 13
         virtual void Unk_14(); // 14 // no-op on base class // judging by StaggerEffect, this is responsible for actually making the effect do a thing
         virtual void Unk_15(); // 15 // no-op on base class
         virtual bool Unk_16(); // 16
         virtual bool Unk_17(); // 17 // no-op on base class; returns true
         virtual float Unk_18(); // 18 // no-op on base class; returns 1.0

         //	void** _vtbl; // 00
         ActiveEffectReferenceEffectController controller; // 04
         UInt32 unk10;
         UInt32 unk14;
         UInt32 unk18;
         UInt32 unk1C;
         UInt32 unk20;
         UInt32 unk24;
         UInt32 actorCasterHandle; // 28
         NiNode*  niNode;      // 2C
         MagicItem* item;      // 30
         MagicItem::EffectItem* effect; // 34
         MagicTarget* actorTarget; // 38
         TESForm* sourceItem;  // 3C
         BStList<ModelReferenceEffect>* unk40; // 40 // or NiPointers?
         MagicItem* sourceChem; // 44 // if this effect is an addiction, this is the chem you need to take to temporarily disable that addiction. checked by PlayerHasTakenMyChem
         float    elapsed;     // 48 // real-world time
         float    duration;    // 4C // real-world time
         float    magnitude;   // 50
         flags_t  flags;			// 54
         UInt32   unk58;			// 58 // state enum; negative values are defined; 0 = stop effect? 1 = continue effect? -1 = ???
         UInt32   effectNum;		// 5C // SKSE team says: "somekind of counter used to determine whether the ActiveMagicEffect handle is valid" // 0x00656E7E writes to this as a UInt16
         UInt32   unk60;			// 60 // This is the highest field accessed in the ActiveEffect constructor.

         class ForEachHitEffectVisitor {
            // Subclasses seen: AttachLightHitEffectVisitor; StopHitEffectsVisitor
            public:
               virtual ~ForEachHitEffectVisitor();
               virtual void Visit(void*);
         };

         MEMBER_FN_PREFIX(ActiveEffect);
         DEFINE_MEMBER_FN(DoConditionUpdate, void, 0x00655BA0, float unused, bool force); // Tests whether the condition update interval has passed (unless (force) is true), and if so, runs conditions and turns off if needed.
         DEFINE_MEMBER_FN(ForEachHitEffect, void, 0x00654E20, ForEachHitEffectVisitor*);
         DEFINE_MEMBER_FN(GetBaseObject, EffectSetting*, 0x00654A70); // returns this->effect->mgef; no nullptr checks
         DEFINE_MEMBER_FN(GetMagnitude,  float,          0x00654D80);
         DEFINE_MEMBER_FN(PlayerHasTakenMyChem, bool, 0x00654C40); // called by DoConditionUpdate; causes the effect to switch off even if its conditions match, if it's an addiction and the player is affected by the corresponding chem
         DEFINE_MEMBER_FN(Subroutine00654A40, void, 0x00654A40, TESForm* sourceItem, bool flag00000100); // sets the respective members
         DEFINE_MEMBER_FN(Subroutine00654C40, bool, 0x00654C40); // related to potions and addictions?
         DEFINE_MEMBER_FN(Subroutine00655080, void, 0x00655080); // related to sound
         DEFINE_MEMBER_FN(Subroutine00655550, bool, 0x00655550); // possibly a "should advance time" getter
         DEFINE_MEMBER_FN(Subroutine006560E0, bool, 0x006560E0, TESObjectREFR* optionalCaster); // returns true if the target is within (effect area) feet of the caster; used for self-cast range checks?
         DEFINE_MEMBER_FN(AdvanceTime, void, 0x00656CB0, float realTimeSeconds); // calls Unk_05 and increments elapsed time; argument can be 0.0F to trigger a basic update
         //
         DEFINE_MEMBER_FN(TurnOff, void, 0x006554F0); // called by Unk_05 after the "inactive" flag is set
   };
   static_assert(sizeof(ActiveEffect) >= 0x64, "ActiveEffect is too small!");
   static_assert(sizeof(ActiveEffect) <= 0x64, "ActiveEffect is too large!");
   /*// offsetof is broken, at least in Intellisense pre-2019
   static_assert(offsetof(ActiveEffect, controller) >= 0x04, "ActiveEffect::controller is too early!");
   static_assert(offsetof(ActiveEffect, controller) <= 0x04, "ActiveEffect::controller is too late!");
   static_assert(offsetof(ActiveEffect, item) >= 0x30, "ActiveEffect::item is too early!");
   static_assert(offsetof(ActiveEffect, item) <= 0x30, "ActiveEffect::item is too late!");
   static_assert(offsetof(ActiveEffect, elapsed) >= 0x48, "ActiveEffect::elapsed is too early!");
   static_assert(offsetof(ActiveEffect, elapsed) <= 0x48, "ActiveEffect::elapsed is too late!");
   static_assert(offsetof(ActiveEffect, unk60) >= 0x60, "ActiveEffect::unk60 is too early!");
   static_assert(offsetof(ActiveEffect, unk60) <= 0x60, "ActiveEffect::unk60 is too late!");
   //*/

   class BoundItemEffect : public ActiveEffect {
      public:
         enum { kVTBL = 0x010C8D74 };
         //
         UInt32         unk64; // 64 // apparently BSTEventSink<ActorInventoryEvent> (member, not a pointer); sizeof == 0x4
         tArray<UInt32> unk68; // 68
         UInt8 unk74 = 0;
         UInt8 unk75 = 1;
         UInt8 unk76 = 0;
         UInt8 pad77;
   };
   class CloakEffect : public ActiveEffect { // sizeof == 0x68
      public:
         enum { kVTBL = 0x010C8DDC };
         //
         UInt32 unk64 = 0; // 64
   };
   class CommandEffect : public ActiveEffect { // sizeof == 0x68
      public:
         enum { kVTBL = 0x010C8E84 };
         //
         UInt32 unk64 = 0; // 64
   };
   class ReanimateEffect : public CommandEffect { // sizeof == 0x6C
      public:
         UInt8 unk68 = 1;
         UInt8 pad69[3];
   };
   class CommandSummonedEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C8EEC };
   };
   class CureEffect : public ActiveEffect { // sizeof == 0x6C
      public:
         enum { kVTBL = 0x010C8F54 };
         //
         UInt32 unk64 = 1;
         UInt32 unk68 = -1; // enum of some kind
   };
   class DetectLifeEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C9044 };
   };
   class DisguiseEffect : public ActiveEffect { // sizeof == 0x68
      public:
         enum { kVTBL = 0x010C9114 };
         //
         UInt32 unk64 = 0; // 64
   };
   class DispelEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C919C };
   };
   class EtherealizationEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C92A4 };
   };
   class GuideEffect : public ActiveEffect { // sizeof == 0x78
      public:
         enum { kVTBL = 0x010C93A4 };
         //
         UInt32 unk64 = 0; // 64
         UInt32 unk68 = 0; // 68
         tArray<UInt32> unk6C; // 6C
   };
   class LightEffect : public ActiveEffect { // sizeof == 0x68
      public:
         enum { kVTBL = 0x010C94A4 };
         //
         void* unk64 = 0; // 64 // refcounted object; possibly NiRefObject subclass
   };
   class LockEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C951C };
   };
   class OpenEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010C9A54 };
   };
   class ScriptEffect : public ActiveEffect { // sizeof == 0x6C
      public:
         enum { kVTBL = 0x010C9DE4 };
         //
         UInt32 unk64; // 64
         UInt32 unk68 = 0; // 68
   };
   class ScriptedRefEffect : public ScriptEffect { // sizeof == 0x6C
      public:
         enum { kVTBL = 0x010C9E4C };
   };
   class SoulTrapEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010CA0F4 };
   };
   class SpawnHazardEffect : public ActiveEffect { // sizeof == 0x6C
      public:
         enum { kVTBL = 0x010CF20C };
         //
         UInt32 unk64 = 0; // 64
         UInt32 unk68 = 0; // 68
   };
   class StaggerEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010CA15C };
         //
         virtual void Unk_14(); // 14 // the only override
   };
   class SummonCreatureEffect : public ActiveEffect {
      public:
         enum { kVTBL = 0x010CA1F4 };
         //
         NiPoint3 unk64; // 64
         NiPoint3 unk70; // 70
         UInt32   unk7C = 0; // 7C
         UInt32   unk80 = 0; // 80
         UInt8    unk84 = 0;
         UInt8    unk85 = 0;
         UInt8    unk86 = 0;
         UInt8    pad87;
   };
   class TelekinesisEffect : public ActiveEffect { // sizeof == 0x7C
      public:
         enum { kVTBL = 0x010CA744 };
         //
         UInt32 unk64 = 0; // 64
         UInt32 unk68 = 0; // 68
         UInt32 unk6C = 0; // 6C
         UInt32 unk70 = 0; // 70
         UInt32 unk74 = 0; // 74
         UInt8  unk78 = 0; // 78
         UInt8  unk79 = 0; // 79
         UInt8  pad7A[2];
   };
   class VampireLordEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010CA8BC };
   };
   class WerewolfEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010CA924 };
   };
   class WerewolfFeedEffect : public ActiveEffect { // sizeof == 0x64
      public:
         enum { kVTBL = 0x010CA98C };
   };
   //
   // ValueModifierEffect and subclasses:
   //
   class ValueModifierEffect : public ActiveEffect { // sizeof == 0x6C
      public:
         enum { kVTBL = 0x010CA834 };
         //
         virtual void SetActorValueIndex(UInt32 avIndex); // 19
         virtual bool Unk_1A();
         virtual void Unk_1B();
         virtual bool Unk_1C();
         virtual void Unk_1D(float);
         virtual bool Unk_1E();
         virtual void Unk_1F(Actor* caster, Actor* target, float magnitude);
         virtual void Unk_20(Actor* target, float magnitude, SInt32 overrideAVIndex); // if overrideAVIndex == -1, then uses this->actorValue
         //
         UInt32 actorValue; // 64
         float  unk68;      // 68
   };
   //
   class AccumulatingValueModifierEffect : public ValueModifierEffect { // sizeof == 0x78
      public:
         UInt32 unk6C; // 6C
         UInt32 unk70;
         UInt32 unk74 = 0;
   };
   class DualValueModifierEffect : public ValueModifierEffect { // sizeof == 0x70
      public:
         UInt32 unk6C; // 6C
   };
   class EnhanceWeaponEffect : public DualValueModifierEffect { // sizeof == 0x78
      public:
         UInt32 unk70;     // this (and possibly unk74 with it) may be a BSTEventSink<ActorInventoryEvent>
         UInt32 unk74 = 0; //
   };
   class GrabActorEffect : public ValueModifierEffect { // sizeof == 0x7C
      public:
         UInt32 unk6C = 0; // 6C
         UInt32 unk70 = 0;
         UInt32 unk74 = 0;
         UInt8  unk78 = 0;
         UInt8  unk79 = 0;
         UInt8  pad7A[2];
   };
   class PeakValueModifierEffect : public ValueModifierEffect { // sizeof == 0x70
      public:
         UInt32 unk6C = 0;
   };
   class AbsorbEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class BanishEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class CalmEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class DarknessEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class DemoralizeEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class FrenzyEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class InvisibilityEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class ParalysisEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class RallyEffect : public ValueModifierEffect {}; // sizeof == 0x6C
   class TurnUndeadEffect : public ValueModifierEffect {}; // sizeof == 0x6C
};