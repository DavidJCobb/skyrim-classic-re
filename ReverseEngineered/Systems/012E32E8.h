#pragma once
#include "skse/GameForms.h"
#include "skse/GameReferences.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Miscellaneous.h"

namespace RE {
   class BGSLoadGameBuffer;
   class BGSSaveFormBuffer;
   class ShaderReferenceEffect; // see ReferenceEffects.h
   class TESBoundObject;

   struct WitnessedCrime { // sizeof == 0x48
      enum class crime_type : UInt32 {
         unspecified = -1, // for GetCrime condition only
         theft = 0,
         pickpocketing = 1,
         trespassing = 2,
         assault = 3,
         murder = 4,
         escape_jail = 5,
         werewolf_transform = 6,
      };
      //
      UInt32     unk00 = 0;
      crime_type unk04 = crime_type::theft; // 04
      UInt32     victimHandle   = 0; // 08 // actor refHandle
      UInt32     criminalHandle = 0; // 0C // actor refHandle; type ID'd from loading code
      UInt8      unk10 = 0;
      UInt8      pad11[3];
      TESBoundObject* unk14 = nullptr; // 14 // type ID'd from loading code
      UInt32 unk18 = 0; // 18
      tArray<UInt32> witnesses; // 1C // array of actor refHandles; type ID'd from loading code
      TESForm* unk28 = nullptr; // 28 // type ID'd from loading code
      UInt32 unk2C = 0;
      UInt8  unk30 = 0;
      UInt8  pad31[3];
      UInt32 date  = 0; // 34 // Constructor_B initializes this based on time globals; see SetDate below
      float  time  = -FLT_MAX; // 38 // Constructor_B initializes this to g_timeNow
      UInt32 unk3C = 0;
      TESFaction* unk40 = nullptr; // 40 // type ID'd from loading code
      UInt8  unk44 = 0;
      UInt8  pad45;
      UInt16 unk46 = 0;
      //
      MEMBER_FN_PREFIX(WitnessedCrime);
      DEFINE_MEMBER_FN(Constructor,   WitnessedCrime&, 0x00775F80);
      DEFINE_MEMBER_FN(Constructor_B, WitnessedCrime&, 0x00775FE0, crime_type type, UInt32 victimHandle, UInt32 criminalHandle, TESBoundObject* unk14, UInt32 unk18, UInt32 unk28);
      DEFINE_MEMBER_FN(Load, void, 0x00776840, BGSLoadGameBuffer*);
      DEFINE_MEMBER_FN(WasWitnessedBy, bool, 0x007765F0, UInt32 actorHandle);
      //
      inline void SetDate(UInt32 year, UInt32 month, UInt32 dayOfMonth) noexcept {
         UInt32 result = (((year << 4) | month) << 9) | dayOfMonth;
         // bits: YYYYMMMMDDDDDDDDD
         this->date = result;
      }
   };

   class Unknown012E32E8 {
      public:
         inline static Unknown012E32E8* GetInstance() { // game accesses the singleton through this pointer...
            return *(Unknown012E32E8**)0x012E32E8;
         };
         static constexpr Unknown012E32E8* instance = (Unknown012E32E8*)0x01B2ECC0; // ...but the singleton instance is always constructed here
         //
         UInt8  unk000;                   // 008
         bool   enableDetection;          // 001 // if true, then AI detection is enabled; see console:ToggleDetection
         bool   unk002;                   // 002 // if true, then AI detection stats should be printed to the screen; see console:ToggleDetectionStats
         UInt8  unk003;                   // 003
         UInt32 unk004;                   // 004
         bool   enableHighProcess;        // 008 // if true, then AI processing for actors in high is on; see console:ToggleHighProcess
         bool   enableLowProcess;         // 009 // if true, then AI processing for actors in low is on; see console:ToggleLowProcess
         bool   enableMiddleHighProcess;  // 00A // if true, then AI processing for actors in middle-high is on; see console:ToggleMiddleHighProcess
         bool   enableMiddleLowProcess;   // 00B // if true, then AI processing for actors in middle-low is on; see console:ToggleMiddleLowProcess
         bool   enableAISchedules;        // 00C // if true, then AI processing for actors' editor schedules is on; see console:ToggleAISchedules
         bool   showDialogueSubtitles;    // 00D // modified by Console:ShowDialogSubtitles
         UInt8  unk00E;                   // 00E
         UInt8  unk00F;                   // 00F
         SInt32 numActorsInHighProcess;   // 010 // returned by Console:GetActorsInHigh // reset to 0 in DoAIProcessing and then incremented for every high AI processed
         UInt32 unk014;                   // 014
         UInt32 unk018;                   // 018
         float  removeExcessComplexDeadTime; // 1C // GMST:fRemoveExcessComplexDeadTime
         HANDLE unk20; // 20 // semaphore
         UInt32 unk24;
         //
         // The next four arrays track actors in various process states. The first 
         // state is "high process;" the next three are lower process levels. 
         // Skyrim only has four arrays here, and it has console commands related 
         // to every process level except Middle, so my assumption is that Skyrim 
         // doesn't have the "middle" process level anymore.
         //
         // Middle-high is used for actors that were in recently-loaded cells.
         //
         tArray<UInt32>  actorsHigh; // 028 // ref handles for Actors in high process. the TDETECT command loops over this to delete AI/detection state in actors. general AI processing loops over this as well.
         tArray<UInt32>  actorsLow;  // 034 // verified by memory inspection: this had the most handles of all four arrays, so it has to be Low
         tArray<UInt32>  unk40; // 040
         tArray<UInt32>  unk4C; // 04C
         //
         tArray<UInt32>* unk058; // 058 // initialized to &this->unk28 // referenced by opcode at 00529EF2 // examine subroutine 00529EC0 for further information
         tArray<UInt32>* unk05C; // 05C // initialized to &this->unk40
         tArray<UInt32>* unk060; // 060 // initialized to &this->unk4C
         tArray<UInt32>* unk064; // 064 // initialized to &this->unk34
         BStList<WitnessedCrime>* witnessedCrimes[7]; // 068 // index == crime type, so seven lists, one for each crime type
         tArray<ShaderReferenceEffect*> unk84; // 084 // VisualEffects?
         SimpleLock                     unk90; // 090 // lock for unk84
         tArray<ShaderReferenceEffect*> activeEffectShaders;    // 098
         SimpleLock                     activeEffectShaderLock; // 0A4
         tArray<UInt32> unkAC; // AC
         SimpleLock     unkB8; // B8 // lock for unkAC
         UInt32 unkC0;
         UInt32 unkC4;
         tArray<UInt32>  unkC8; // C8 // list of ref handles, possibly refs queued to reset
         BStList<UInt32> unkD4; // D4 // list of ref handles
         UInt32 unk0DC[(0x0E8 - 0x0DC) / sizeof(UInt32)]; // 0DC
         SimpleLock unkE8; // 0E8 // lock for unkF0
         tArray<UInt32> unkF0; // 0F0 // tArray of ref handles; AI processing checks whether each actor-in-high is in here and branches
         UInt32 unk0FC[(0x108 - 0x0FC) / sizeof(UInt32)]; // 0FC
         float  unk108; // 108 // related to Z-keying?
         UInt32 unk10C;
         UInt32 unk110;
         UInt32 unk114;
         UInt32 unk118;
         bool   enableActorAI;        // 11C // if true, then all AI processing is on
         bool   enableActorMovement;  // 11D // if true, then all actor movement processing is on
         bool   enableActorAnimation; // 11E // if true, then all actor animation processing is on
         UInt8  unk11F;               // 11F // modified by opcode at 004A6BDE and opcode at 0x00754780; checked at 0x006D83E3

         typedef void(*HandleCallback)(UInt32& handle);
         struct HandleFunctor {
            virtual void Execute(UInt32 handle);
         };

         //
         // NOTE: Not all of the functions below should be called by mods.
         //
         MEMBER_FN_PREFIX(Unknown012E32E8);
         DEFINE_MEMBER_FN(Constructor, Unknown012E32E8&, 0x0075D000);
         DEFINE_MEMBER_FN(Destructor,  void, 0x0075E430);
         DEFINE_MEMBER_FN(AddHandleToUnk0C8,         void,   0x00756940, Actor* actor); // aborts if actor is already in the array
         DEFINE_MEMBER_FN(AddActorToAIList,          void,   0x00756370, UInt32 value, UInt32 processLevel); // appends to one of unk028, unk034, unk040, or unk04C; no bounds-checking on (which
         DEFINE_MEMBER_FN(ClearTempEffects,          void,   0x00755480); // powers Papyrus Game.ClearTempEffects()
         DEFINE_MEMBER_FN(DoAIProcessing,            void,   0x0075CBB0, float time, bool isSkippingTime); // no-oping this has the same effect as global TAI
         DEFINE_MEMBER_FN(DoMovementProcessing,      void,   0x00756460, UInt32, UInt32); // no-oping this has the same effect as TMOVE
         DEFINE_MEMBER_FN(GetUnkD4, BStList<UInt32>*, 0x00754050);
         DEFINE_MEMBER_FN(Load,                      void,   0x007549B0, BGSLoadGameBuffer*);
         DEFINE_MEMBER_FN(RemoveActorFromAIList,     void,   0x007563E0, UInt32 value, UInt32 processLevel); // finds and removes from one of unk028, unk034, unk040, or unk04C; no bounds-checking on (which)
         DEFINE_MEMBER_FN(RemoveHandleFromUnkD4,     void,   0x00756720, UInt32& refHandle);
         DEFINE_MEMBER_FN(ResetAllDetection,         void,   0x00542970, bool* unused);
         DEFINE_MEMBER_FN(ForEachActorInHighProcess,   void,   0x006931E0, HandleFunctor& functor); // for each of this->actorsHigh, call functor->Unk_00(item)
         DEFINE_MEMBER_FN(ForEachActorInHighProcess_B, void,   0x006A09E0, HandleCallback func); // for each of this->actorsHigh, runs func(&item); terminates early if func returns false
         DEFINE_MEMBER_FN(GetCrimeKnown,               bool,   0x00758640, Actor* victim, Actor* criminal, Actor* witness, WitnessedCrime::crime_type, UInt32, UInt32);
         DEFINE_MEMBER_FN(UnkF0InsertHandle, void, 0x0075BA50, UInt32 refHandle);
         DEFINE_MEMBER_FN(UnkF0ContainsHandle,       SInt32, 0x0075BA10, UInt32 refHandle);
         DEFINE_MEMBER_FN(Save,                      void,   0x007544F0, BGSSaveFormBuffer*);
         DEFINE_MEMBER_FN(SearchWitnessedCrimeListForInstance, UInt32, 0x00754440, WitnessedCrime::crime_type, WitnessedCrime* searchFor);
         DEFINE_MEMBER_FN(SendWerewolfTransformCrime, void, 0x0075B2A0); // backend for Papyrus Game.SendWereWolfTransformation; calls Actor::SendWerewolfTransformAlarm on an actor in high who is detecting the player
         DEFINE_MEMBER_FN(StopCombatAlarmOnActor, void, 0x00758910, Actor* target, bool maybeAlsoClearArrested);
         DEFINE_MEMBER_FN(StopEffectShader,          void,   0x00754840, TESObjectREFR*, TESEffectShader*);
         DEFINE_MEMBER_FN(Subroutine006E7470,        void,   0x006E7470, void*); // loops over actors in high process; seems related to melee attacks, and can generate hit structs
         DEFINE_MEMBER_FN(Subroutine00753F80,        float,  0x00753F80);        // getter for float at 0x01B39E38
         DEFINE_MEMBER_FN(Subroutine00753F90,        void,   0x00753F90, float); // setter for float at 0x01B39E38
         DEFINE_MEMBER_FN(Subroutine00754750,        void,   0x00754750, UInt32);
         DEFINE_MEMBER_FN(Subroutine00754790, void, 0x00754790, WitnessedCrime* newlyCreated);
         DEFINE_MEMBER_FN(Subroutine00754900,        void,   0x00754900, TESObjectREFR*, void*); // stops a "model reference shader"?
         DEFINE_MEMBER_FN(Subroutine00759070, void, 0x00759070, UInt32);
         DEFINE_MEMBER_FN(Subroutine007591D0, void, 0x007591D0, ShaderReferenceEffect*); // tries to remove the effect from both lists?
         DEFINE_MEMBER_FN(Subroutine007593D0,        void,   0x007593D0, UInt32 refHandle); // most likely stops all shaders on the given reference
         DEFINE_MEMBER_FN(Subroutine0075B7B0, void, 0x0075B7B0, void* functor); // args seen: Reset3DMobIterator; function iterates over the four list pointers beginning at unk058
         DEFINE_MEMBER_FN(Subroutine0075B880, void, 0x0075B880, UInt32);
         DEFINE_MEMBER_FN(Subroutine0075B940, void, 0x0075B940, UInt32);
         DEFINE_MEMBER_FN(Subroutine0075D280,        void,   0x0075D280);
         DEFINE_MEMBER_FN(Subroutine0075D2A0,        void,   0x0075D2A0);
         DEFINE_MEMBER_FN(Subroutine0075E100,        void,   0x0075E100); // checks whether animation processing is enabled; does stuff based on that
         DEFINE_MEMBER_FN(ToggleMovementProcessing,  void,   0x00754000); // toggles the value of unk11D
         DEFINE_MEMBER_FN(ToggleAnimationProcessing, void,   0x00754020); // toggles the value of unk11E
   };
   static_assert(offsetof(Unknown012E32E8, witnessedCrimes) >= 0x68, "Unknown012E32E8::witnessedCrimes is too early!");
   static_assert(offsetof(Unknown012E32E8, witnessedCrimes) <= 0x68, "Unknown012E32E8::witnessedCrimes is too late!");
   static_assert(offsetof(Unknown012E32E8, unk84) >= 0x84, "Unknown012E32E8::unk84 is too early!");
   static_assert(offsetof(Unknown012E32E8, unk84) <= 0x84, "Unknown012E32E8::unk84 is too late!");

   // Refernces to enableActorAI:
   // 53B1B9 (ToggleAI)
   // 6D5FEE (Actor::Subroutine006D5B70)
   // 7480BB (PlayerCharacter::Subroutine00748010)
   // 75CBEF
   // 75D190 (constructor)
   // 75D280 (...::Subroutine0075D280)
   // 75D2C6 (...::Subroutine0075D2A0)
}