#pragma once
#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"

#include "skse/Utilities.h"

namespace RE {
   class  bhkMouseSpringAction;
   class  PlayerCharacter;
   extern PlayerCharacter** const g_thePlayer;

   class PlayerCharacter : public Character { // sizeof == 0x72C
      public:
         virtual ~PlayerCharacter();
         // virtual SInt32 GetBountyViolent(TESFaction*);    // Virtual 0x12A
         // virtual SInt32 GetBountyNonViolent(TESFaction*); // Virtual 0x12B
         // virtual void   Unk_12C(TESFaction*);             // Virtual 0x12C // related to paying the player's bounty

         enum GrabType : UInt32 {
            kGrabType_None = 0,
            kGrabType_ZKey = 1,
            kGrabType_Telekinesis = 2,
         };
         enum Flags6E2 : UInt16 {
            kFlag6E2_DisableSaving  = 0x0001, // CharGen flag
            kFlag6E2_DisableWaiting = 0x0002, // CharGen flag
            kFlag6E2_HandsAreBound  = 0x0004, // CharGen flag
         };
         enum Flags726 : UInt8 {
            kFlag726_AIDriven  = 0x08,
            kFlag726_IsYoung   = 0x40, // FO3 leftover
            kFlag726_IsToddler = 0x80, // FO3 leftover
         };
         enum Flags728 : UInt8 {
            kFlag728_Unk02 = 0x02, // related to grabbing
            kFlag728_CrimesAreReported = 0x04,
            kFlag728_Unk08 = 0x08, // player has seen the low-health  tutorial
            kFlag728_Unk10 = 0x10, // player has seen the low-magicka tutorial
            kFlag728_Unk20 = 0x20, // player has seen the low-stamina tutorial
            kFlag728_Unk40 = 0x40, // cleared by PlayerCharacter::ClearPrison; set by PlayerCharacter::PayBounty if removing stolen goods
            kFlag728_Unk80 = 0x80, // GMST:bSaveOnTravel?
         };

         struct Unk494 {
            float  unk00 = -1; // set to GMST:fAICommentTimeWindow when you first start Z-keying something?
            UInt32 unk04 = 0; // ref handle
            UInt32 unk08 = 0x10;
         };
         struct GrabList : public BSTSmallArray<NiPointer<bhkMouseSpringAction>, 4> { // sizeof == 0x18
            MEMBER_FN_PREFIX(GrabList);
            DEFINE_MEMBER_FN(Insert, UInt32, 0x007381B0, NiPointer<bhkMouseSpringAction>&); // adds the smart pointer to the tArray; returns index
         };

         // parents
         BSTEventSink<void*>   menuOpenCloseEvent;			// 19C .?AV?$BSTEventSink@VMenuOpenCloseEvent@@@@
         BSTEventSink<void*>   menuModeChangeEvent;		// 1A0 .?AV?$BSTEventSink@VMenuModeChangeEvent@@@@
         BSTEventSink<void*>   userEventEnabledEvent;		// 1A4 .?AV?$BSTEventSink@VUserEventEnabledEvent@@@@
         BSTEventSource<void*> actorCellEventSource;		// 1A8 .?AV?$BSTEventSource@UBGSActorCellEvent@@@@
         BSTEventSource<void*> actorDeathEventSource;		// 1D8 .?AV?$BSTEventSource@UBGSActorDeathEvent@@@@
         BSTEventSource<void*> positionPlayerEventSource;	// 208 .?AV?$BSTEventSource@UPositionPlayerEvent@@@@

         UInt32	pad238[(0x2A4 - 0x238) >> 2];	// 238

         NiPoint3 unk2A4; // 2A4 - initialized from (NiPoint3*)0x01B910A4
         NiPoint3 unk2B0; // 2B0 - initialized from (NiPoint3*)0x01B910A4
         NiPoint3 unk2BC; // 2BC - initialized from (NiPoint3*)0x01B910A4
         UInt32   unk2C8;
         UInt32   unk2CC;
         UnkFormArray unk2D0; // perks?
         UnkFormArray unk2DC;
         UnkFormArray unk2E8; // perks?
         UnkFormArray unk2F4;
         UnkFormArray unk300;
         UnkFormArray unk30C;
         UnkFormArray unk318;
         UnkFormArray unk324;
         UInt32       unk330 = 0;
         UInt32       unk334 = 0;
         UnkFormArray unk338;
         float  unk344;
         float  unk348;
         UInt32 unk34C = 0;
         UInt32 unk350 = 0;
         UInt32 unk354 = 0;
         UInt32 unk358 = 0xDEADBEEF;
         float  unk35C;
         UInt32 unk360 = 0;
         UInt32 unk364;
         UInt32 unk368;
         UInt32 unk36C = 0;
         UInt32 unk370 = 0;
         UInt32 unk374 = 0;
         UInt32 unk378 = 0xDEADBEEF;
         UInt32 unk37C;
         UInt32 unk380 = 0;
         struct {
            UInt32 unk00 = 0;
            UInt32 unk04 = 0;
         } unk384;
         UInt32 unk38C;        // NiTMapBase...
         UInt32 unk390 = 0x25; //
         UInt32 unk394;        //
         UInt32 unk398 = 0;    // ...end of.
         UInt32 unk39C = 0;
         NiPoint3 wentInsideAt; // 3A0 // all values initialized to FLT_MAX; player's last-known outdoor position; updated whenever the player passes through a loading screen that takes them from an exterior to an interior
         struct Unk3AC {
            UInt32 unk00 = 0;
            TESObjectCELL* unk04 = nullptr; // 04
            NiPoint3 unk08; // initialized to (NiPoint3*)0x01B910A4
            NiPoint3 unk14; // initialized to (NiPoint3*)0x01B910A4
            UInt32 unk20 = 0;
            UInt32 unk24 = 0;
            UInt32 unk28 = 0; // ref handle
            UInt32 unk2C = 0; // ref handle
            bool   unk30 = false;
            bool   unk31 = true;
            bool   unk32 = true;
            UInt8  unk33; // padding?
            //
            MEMBER_FN_PREFIX(Unk3AC);
            DEFINE_MEMBER_FN(Subroutine007475F0, void, 0x007475F0); // reset?
         } unk3AC; // this struct layout is used elsewhere, but it doesn't have VTBL/RTTI, so I can't ID it
         UInt32 unk3E0 = -1;
         UInt32 unk3E4 = 0; // these could be individual bytes
         UInt32 unk3E8 = 0;
         ExtraSound::Data unk3EC; // 3EC
         ExtraSound::Data unk3F8; // 3F8
         TESPackage* unk404 = nullptr; // 404 // DialoguePackage* specifically?
         UInt32 unk408;
         TESPackage* unk40C = nullptr; // 40C // DialoguePackage* specifically?
         UInt32 unk410[(0x420 - 0x410) / 4];
         UInt32 unk420 = 0xFF7FFFFF;
         UInt32 unk424;
         UInt32 unk428;
         TESFaction* unk42C = nullptr; // 42C // faction currently arresting the player?
         UInt32 unk430 = 0;
         UInt32 unk434 = 0;
         UInt32 unk438[(0x48C - 0x438) / 4];
         UInt32 unk48C = 0;
         UInt32	unk490;							// 490 - Handle // own handle?
         Unk494   unk494[0xE]; // 494; sizeof each is 0xC
         UInt32	unk53C[(0x548 - 0x53C) >> 2];
         UInt32   unk548; // 548
         UInt32	unk54C; // 54C - Handle; PlayerCharacter::Subroutine0073D4B0 returns the actor and clears the handle
         GrabList grabSprings; // 550
         UInt32	grabbedRefHandle; // 568
         float    unk56C; // 56C // mass of object being Z-Keyed/telekinesis'd? see PlayerCharacter::TESV_0074C590
         float    grabStartDistance; // 570 // related to Z-keying/telekinesising objects; memory inspection confirms it's set to the distance to the node we're dragging when we start dragging (it's not live-updated)
         float    unk574;
         UInt32	unk578[(0x584 - 0x578) >> 2];
         UInt32   unk584; // 584 // sleep hours, but I don't understand how it's stored
         void*    unk588; // 588 // pointer to something that doesn't have a vtbl
         NiNode*  firstPersonSkeleton; // 58C
         float    unk590;
         UInt32	unk594;
         float    unk598;
         UInt32   unk59C[(0x5AC - 0x59C) >> 2];
         UInt32	lastRiddenHorseHandle;			// 5AC - Handle
         UInt32	unk5B0;
         UInt32   unk5B4;
         UInt32   unk5B8;
         UInt32   unk5BC;
         UInt32   unk5C0;
         UInt32 timesTrainedThisLevel; // 5C4
         UInt32 unk5C8[(0x5FC - 0x5C8) / 4];
            // unk5E8 - incremented when the player does a crime?
         TESObjectCELL* unk5FC; // 5FC // redundant player cell? can be an interior.
         UInt32   unk600;
         UInt32   unk604;
         UInt32   unk608;
         UInt32   unk60C;
         UInt32   unk610;
         PlayerSkills* skills;					// 614
         UInt32	targetHandle;					// 618
         UInt32	unk61C;							// 61C
         UInt32	unk620;							// 620
         UInt32	unk624;							// 624
         UInt32	unk628;							// 628
         UInt32	unk62C;							// 62C
         tArray<UInt32>	hostileHandles;			// 630
         UInt32	unk63C;							// 63C
         TESForm* equippedPoisons[2]; // 640 // set by PlayerCharacter::TESV_007505E0(UInt32 which, TESForm*)
         UInt32	numTeammates;					// 648
         UInt32	pad64C[(0x6B0 - 0x64C) >> 2];
         BGSLocation* unk6B0; // 6B0
         UInt32	unk6B4;
         float    unk6B8; // 6B8 // used as the raycast distance for crosshair/activation, if larger than the INI setting and the player isn't issuing a command to a follower
         float    favorRequestWaitTimer; // 6BC
         UInt32	unk6C0[(0x6D4 - 0x6C0) >> 2];
         GrabType grabType; // 6D4 // 2 = telekinesis
         UInt32 unk6D8;
         UInt32 unk6DC;
         UInt8	unk6E0;							// 6E0
         UInt8	numPerkPoints;					// 6E1
         UInt16 unk6E2; // 6E2 // see: Flags6E2
         UInt32	unk6E4;							// 6E4
         tArray<TintMask*>	tintMasks;			// 6E8		// These are the actual tints
         tArray<TintMask*> overlayTintMasks;	// 6F4		// These apply when overlay head parts is enabled
         UInt32 unk700 = 0;
         UInt32 unk704 = 0;
         UInt32 unk708 = 0x80000000;
         UInt32 unk70C;
         UInt32 unk710;
         UnkFormArray unk714;
         UInt32 unk720 = 0;
         UInt8  unk724; // bitmask
         UInt8  unk725 = 1; // 725
         UInt8  unk726 = 1; // 726 // see: Flags726
         UInt8  unk727; // 727 // bitmask; PlayerCharacter::Unk_71 returns unk727 & 1; value is related to flags on actor's root nodes, somehow; value is related to whether the player is AI driven, too
         UInt8  unk728 = 4; // 728 // see: Flags728
         UInt8  unk729; // 729 // bitmask
         UInt8  unk72A; // padding?
         UInt8  unk72B; // padding?

         bool IsMovingIntoNewSpace() const noexcept {
            if (this->unk728 & 1)
               if (this->unk724 & 2)
                  return true;
            return false;
         }

         // Overlayed tints should be the same as original tints
         // occasionally they can have no type so index matching
         // is required to set anything on the tint
         TintMask* GetOverlayTintMask(TintMask * original);

         // Confirmed - Same as ExtraContainerChanges::EntryData
         // This type is used by scaleform to extend data
         // It can be used to extend more of the "ExtraData"
         /*struct ObjDesc
         {
         TESForm					* form;
         tList<BaseExtraList>	* extraData;
         SInt32					countDelta;

         MEMBER_FN_PREFIX(ObjDesc);
         DEFINE_MEMBER_FN(GenerateName, const char *, 0x00475AA0);
         };*/

         MEMBER_FN_PREFIX(PlayerCharacter);
         DEFINE_MEMBER_FN(IncrementSkill, void, 0x00733270, UInt32 avIndex, UInt32 levels); // same underlying mechanism as Papyrus IncrementSkill and IncrementSkillBy
         DEFINE_MEMBER_FN(GetTintList, tArray<TintMask*>*, 0x0055FF90);
         DEFINE_MEMBER_FN(GetNumTints, UInt32, 0x00735F40, UInt32 tintType);
         DEFINE_MEMBER_FN(GetTintMask, TintMask*, 0x00735F00, UInt32 tintType, UInt32 index);
         DEFINE_MEMBER_FN(GetDamage, double, 0x00730810, InventoryEntryData * pForm);
         DEFINE_MEMBER_FN(GetArmorValue, double, 0x007307E0, InventoryEntryData * pForm);
         //
         DEFINE_MEMBER_FN(GetInfamy,                 SInt32, 0x00744100, TESFaction*); // get lifetime bounty for faction
         DEFINE_MEMBER_FN(GetInfamyViolent,          SInt32, 0x00744160, TESFaction*); // get lifetime bounty for faction (violent crimes)
         DEFINE_MEMBER_FN(GetInfamyNonViolent,       SInt32, 0x007441B0, TESFaction*); // get lifetime bounty for faction (non-violent crimes)
         DEFINE_MEMBER_FN(GetStolenItemValueCrime,   SInt32, 0x00744360, TESFaction*); // total value of all items the player was witnessed stealing from the faction
         DEFINE_MEMBER_FN(GetStolenItemValueNoCrime, SInt32, 0x00744310, TESFaction*); // total value of all items the player stole from the faction without being caught
         //
         DEFINE_MEMBER_FN(HandleLowHealthHeartbeat,          void, 0x00671690, float healthPercentage);
         DEFINE_MEMBER_FN(HandleLowHealthImagespaceModifier, void, 0x00733A60, float healthPercentage);
         DEFINE_MEMBER_FN(HandleLowStatTutorials, void, 0x00737C90, UInt32 avIndex, float avValuePercentage);
         //
         DEFINE_MEMBER_FN(SetPCToddler, void, 0x007339D0, bool); // FO3 leftover
         DEFINE_MEMBER_FN(SetPCYoung,   void, 0x00733A00, bool); // FO3 leftover
         DEFINE_MEMBER_FN(SetPlayerAIDriven, void, 0x0073E0C0, bool);
         DEFINE_MEMBER_FN(SetPoison, void, 0x007505E0, UInt32 whichHand, TESForm*); // whichHand should be 0 or 1 // sets 640 or 644
         //
         DEFINE_MEMBER_FN(IncreaseTeammateCount, void, 0x0073B3C0);
         DEFINE_MEMBER_FN(DecreaseTeammateCount, void, 0x0073B3E0);
         //
         DEFINE_MEMBER_FN(IsAMurderer, bool, 0x00733430); // has the player ever murdered any NPC? used for IsPCAMurderer
         DEFINE_MEMBER_FN(SetWhetherPlayerCrimesAreReported, void, 0x00733E20, bool);

         //
         // Functions related to Z-keying and telekinesis.
         //
         // Two of these use PlayerCharacter::Unk_C1 to get the player-forward direction *and* what appears 
         // to be the camera's position converted to the camera's reference frame (not *quite* the same as 
         // a local or relative position). This data appears to then be fed to the bhkMouseSpringActions 
         // that effect the grabbed object's movement.
         //
         DEFINE_MEMBER_FN(TryGrabCrosshairRef,       void, 0x0074FE50);
         DEFINE_MEMBER_FN(ReleaseGrabbedObject,      void,  0x0074A7F0); // not sure when it's safe to call; almost certainly limited to main thread only
         DEFINE_MEMBER_FN(ReleaseGrabbedObjectIfAny, void, 0x0074C587); // calls ReleaseGrabbedObject only if the current GrabType is "none;" this is probably the one you should use, though again, I don't know when that's safe
         DEFINE_MEMBER_FN(GrabRef,            void*, 0x0074C590, TESObjectREFR* refToGrab, GrabType grabType, float unk570, bool); // Z-keying passes 1000.0 as the float and 0 as the bool
         DEFINE_MEMBER_FN(Subroutine0074D020, void,  0x0074D020); // update grab state per-frame?
   };
   static_assert(sizeof(PlayerCharacter) <= 0x72C, "RE::PlayerCharacter is too large.");
   static_assert(sizeof(PlayerCharacter) >= 0x72C, "RE::PlayerCharacter is too small.");
   static_assert(offsetof(PlayerCharacter, unk42C) <= 0x42C, "RE::PlayerCharacter::unk42C is too late.");
   static_assert(offsetof(PlayerCharacter, unk42C) >= 0x42C, "RE::PlayerCharacter::unk42C is too early.");
   static_assert(offsetof(PlayerCharacter, lastRiddenHorseHandle) <= 0x5AC, "RE::PlayerCharacter::lastRiddenHorseHandle is too late.");
   static_assert(offsetof(PlayerCharacter, lastRiddenHorseHandle) >= 0x5AC, "RE::PlayerCharacter::lastRiddenHorseHandle is too early.");
   static_assert(offsetof(PlayerCharacter, skills) <= 0x614, "RE::PlayerCharacter::skills is too late.");
   static_assert(offsetof(PlayerCharacter, skills) >= 0x614, "RE::PlayerCharacter::skills is too early.");
};