#pragma once
#include "skse/GameEvents.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Shared.h"

class TESObjectREFR;
namespace RE {
   class refr_ptr;

   template <typename EventStruct> class BSTEventSource; // forward-declare for BSTEventSink

   class BSTEventSinkBase {};
   template <typename EventStruct> class BSTEventSink : public BSTEventSinkBase {
      public:
      inline static EventStruct* convertEvent(void* a) { return (EventStruct*)a; };
      inline static BSTEventSource<EventStruct>* convertSource(void* a) { return (BSTEventSource<EventStruct>*)a; };
      //
      virtual ~BSTEventSink() { /* TODO */ }
      virtual EventResult Handle(void* aEv, void* aSource) {
         //
         // Using templated arguments seems to prevent us from being able to override this 
         // function, so your overrides must instead convert non-templated args like so:
         //
         auto ev = convertEvent(aEv);
         auto source = convertSource(aSource);
         //
         return EventResult::kEvent_Continue;
      };
   };
   static_assert(sizeof(BSTEventSink<UInt32>) == 4, "BSTEventSink is the wrong size!");

   template <typename EventStruct> class BSTEventSource { // sizeof == 0x30
      typedef BSTEventSink<EventStruct>* pSink;
      public:
      SimpleLock lock; // 00
      tArray<pSink> activeSinks; // tArray<BSTEventSink*> ? // 08
      tArray<pSink> sinksToAdd;
      tArray<pSink> sinksToRemove;
      UInt8  unk2C = 0; // actually an int
      UInt8  pad2D[3];

      MEMBER_FN_PREFIX(BSTEventSource);
      DEFINE_MEMBER_FN(Constructor, void, 0x0073E790);
      DEFINE_MEMBER_FN(Destructor, void, 0x00695990);
      //
      DEFINE_MEMBER_FN(AddEventSink, void, 0x006E3E30, pSink sink);
      DEFINE_MEMBER_FN(RemoveEventSink, void, 0x008CE0C0, pSink eventSink);
      DEFINE_MEMBER_FN(SendEvent, void, 0x006EBC10, void* eventObj);
   };
   static_assert(sizeof(BSTEventSource<UInt32>) >= 0x30, "BSTEventSource is too small!");
   static_assert(sizeof(BSTEventSource<UInt32>) <= 0x30, "BSTEventSource is too large!");

   struct BGSActorCellEvent { // fired by PlayerCharacter::Unk_98; likely doesn't exist on other actors
      enum Type : UInt32 {
         kType_EnteringCell = 0,
         kType_ExitingCell = 1,
      };
      UInt32 actorRefHandle; // 00
      UInt32 cellFormID;     // 04
      Type   eventType;      // 08
   };
   struct PositionPlayerEvent {}; // TODO
   struct TESActiveEffectApplyRemoveEvent {}; // TODO
   struct TESActorLocationChangeEvent {}; // TODO
   struct TESBookReadEvent {}; // TODO
   struct TESCellAttachDetachEvent {}; // TODO
   struct TESCellFullyLoadedEvent {}; // TODO
   struct TESCellReadyToApplyDecalsEvent {}; // TODO
   struct TESCombatEvent {}; // TODO
   struct TESContainerChangedEvent {}; // TODO
   struct TESDeathEvent {}; // TODO
   struct TESDestructionStageChangedEvent {}; // TODO
   struct TESEnterBleedoutEvent {}; // TODO
   struct TESEquipEvent {}; // TODO
   struct TESFormDeleteEvent {
      UInt32 refrFormID;
   };
   struct TESFurnitureEvent {}; // TODO
   struct TESGrabReleaseEvent {}; // TODO
   struct TESHitEvent {}; // TODO
   struct TESInitScriptEvent {}; // TODO
   struct TESLoadGameEvent {}; // TODO
   struct TESLockChangedEvent {}; // TODO
   struct TESMagicEffectApplyEvent {}; // TODO
   struct TESMagicWardHitEvent {}; // TODO
   struct TESMoveAttachDetachEvent {}; // TODO
   struct TESObjectLoadedEvent {}; // TODO
   struct TESObjectREFRTranslationEvent {}; // TODO
   struct TESOpenCloseEvent {}; // TODO
   struct TESPackageEvent {
      enum Type : UInt32 {
         kType_PackageStart = 0, // package form ID is for the new package
         kType_PackageEnd = 1, // package form ID is for the old package
         kType_PackageChange = 2, // package form ID is for the old package
      };
      //
      TESObjectREFR* target; // 00
      UInt32 packageFormID; // 04
      Type   eventType; // 08
   };
   struct TESPerkEntryRunEvent {}; // TODO
   struct TESPlayerBowShotEvent {}; // TODO
   struct TESQuestInitEvent {}; // TODO
   struct TESQuestStageEvent {}; // TODO
   struct TESQuestStageItemDoneEvent {
      UInt32 questFormID; // 00
      UInt16 stage; // 04
      UInt8  unk06; // 06
      UInt8  pad07; // 07
   };
   struct TESQuestStartStopEvent {
      UInt32 questFormID; // quest formID
      bool   unk04; // 04 // start/stop; not sure what value has what meaning
      UInt8  unk05;
      UInt16 pad06;
   };
   struct TESResetEvent {}; // TODO
   struct TESResolveNPCTemplatesEvent {}; // TODO
   struct TESSceneEvent {}; // TODO
   struct TESSceneActionEvent {}; // TODO
   struct TESScenePhaseEvent {}; // TODO
   struct TESSellEvent {}; // TODO
   struct TESSleepStartEvent {}; // TODO
   struct TESSleepStopEvent {}; // TODO
   struct TESSpellCastEvent {}; // TODO
   struct TESSwitchRaceCompleteEvent {}; // TODO
   struct TESTopicInfoEvent {}; // TODO
   struct TESTrackedStatsEvent {}; // TODO
   struct TESTrapHitEvent {}; // TODO
   struct TESTriggerEvent {}; // TODO
   struct TESTriggerEnterEvent {}; // TODO
   struct TESTriggerLeaveEvent {}; // TODO
   struct TESUniqueIDChangeEvent {
      UInt32 unk00;
      UInt32 unk04;
      UInt32 unk08;
      UInt16 unk0C;
      UInt16 unk0E;
   };
   struct TESWaitStartEvent {
      UInt32 unk00;
      float  days; // 04 // number of hours divided by 24
   };
   struct TESWaitStopEvent {
      uint8_t unk00;
   };

   extern BSTEventSource<UInt32>* g_menuModeChangeEventSource; // 0x01271CE8

   class BSTEventSourceHolder { // sizeof >= 0x9F0
      public:
         static BSTEventSourceHolder* GetOrCreate() { // used by the game
            DEFINE_SUBROUTINE(BSTEventSourceHolder*, f, 0x00438740); // the game often inlines this
            return f();
         };
         static BSTEventSourceHolder* GetInstance() { // we can use this if we know for a fact the instance already exists
            return (BSTEventSourceHolder*)0x012E4C30;
         };
         //
         BSTEventSource<UInt32> eventProcessed; // 000 // BGSEventProcessedEvent
         BSTEventSource<UInt32> activate; // 030 // 
         BSTEventSource<UInt32> activeEffectApplyRemove; // 060 // TESActiveEffectApplyRemoveEvent
         BSTEventSource<UInt32> actorLocationChange; // 090 // TESActorLocationChangeEvent
         BSTEventSource<UInt32> bookRead; // 0C0 // TESBookReadEvent
         BSTEventSource<UInt32> cellAttachDetach; // 0F0 // TESCellAttachDetachEvent
         BSTEventSource<UInt32> cellFullyLoaded; // 120 // TESCellFullyLoadedEvent
         BSTEventSource<UInt32> cellReadyToApplyDecals; // 150 // TESCellReadyToApplyDecalsEvent
         BSTEventSource<UInt32> combat; // 180 // TESCombatEvent
         BSTEventSource<UInt32> containerChanged; // 1B0 // TESContainerChangedEvent
         BSTEventSource<UInt32> death; // 1E0 // 
         BSTEventSource<UInt32> destructionStageChanged; // 210 // 
         BSTEventSource<UInt32> enterBleedout; // 240 // 
         BSTEventSource<UInt32> equip; // 270 // equip or unequip
         BSTEventSource<TESFormDeleteEvent> formDelete; // 2A0 // TESObjectREFR only, though
         BSTEventSource<UInt32> furniture; // 2D0 // 
         BSTEventSource<UInt32> grabRelease; // 300 //  
         BSTEventSource<UInt32> hit; // 330 // hit event
         BSTEventSource<UInt32> initScript; // 360 // 
         BSTEventSource<UInt32> loadGame; // 390 // 
         BSTEventSource<UInt32> lockChanged; // 3C0 // 
         BSTEventSource<UInt32> magicEffectApply; // 3F0 // 
         BSTEventSource<UInt32> magicWardHit; // 420 // 
         BSTEventSource<UInt32> moveAttachDetach; // 450
         BSTEventSource<UInt32> objectLoaded; // 480 // 
         BSTEventSource<UInt32> objectReferenceTranslationEvent; // 4B0 // fired by Papyrus:ObjectReference:TranslateTo and related Papyrus functions; maybe by other stuff?
         BSTEventSource<UInt32> openClose; // 4E0 //
         BSTEventSource<TESPackageEvent> package; // 510 // 
         BSTEventSource<UInt32> perkEntryRun; // 540 // 
         BSTEventSource<UInt32> questInit; // 570 // 
         BSTEventSource<UInt32> questStage; // 5A0 // 
         BSTEventSource<UInt32> questStageItemDone; // 5D0 // TESQuestStageItemDoneEvent
         BSTEventSource<UInt32> questStartStop; // 600 // TESQuestStartStopEvent
         BSTEventSource<UInt32> reset; // 630
         BSTEventSource<UInt32> resolveNPCTemplates; // 660
         BSTEventSource<UInt32> scene; // sent by Scene::Stop, and by one other Scene method (possibly for scene completion)
         BSTEventSource<UInt32> sceneAction; // 6C0
         BSTEventSource<UInt32> scenePhase; // 6F0 // related to scene phases; possibly scene phase completion
         BSTEventSource<UInt32> sell; // 720
         BSTEventSource<UInt32> sleepStart; // 750
         BSTEventSource<UInt32> sleepStop; // 780
         BSTEventSource<UInt32> spellCast; // 7B0
         BSTEventSource<UInt32> playerBowShotEvent; // 7E0
         BSTEventSource<UInt32> topicInfo; // 810
         BSTEventSource<UInt32> trackedStats; // 840
         BSTEventSource<UInt32> trapHit; // 870
         BSTEventSource<UInt32> trigger; // 8A0
         BSTEventSource<UInt32> triggerEnter; // 8D0
         BSTEventSource<UInt32> triggerLeave; // 900
         BSTEventSource<UInt32> uniqueIDChange; // 930
         BSTEventSource<UInt32> waitStart; // 960
         BSTEventSource<UInt32> waitStop; // 990
         BSTEventSource<UInt32> switchRaceComplete; // 9C0

         // all members are BSTEventSource instances, sizeof == 0x30

         MEMBER_FN_PREFIX(BSTEventSourceHolder);
         DEFINE_MEMBER_FN(Destructor,   void, 0x00436DE0);
         //
         DEFINE_MEMBER_FN(SendActivateEvent,   void, 0x004E0450, refr_ptr*, void*); // actually a smart refr_ptr
         DEFINE_MEMBER_FN(SendFormDeleteEvent, void, 0x00690AFD, UInt32 formID);
         DEFINE_MEMBER_FN(SendPackageEvent,    void, 0x0070C280, UInt32 packageFormID, refr_ptr*, TESPackageEvent::Type eventType); // actually a smart refr_ptr
         DEFINE_MEMBER_FN(SendActorLocationChangeEvent,     void, 0x004E04B0, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendCellFullyLoadedEvent,         void, 0x00437650, UInt32);
         DEFINE_MEMBER_FN(SendCellReadyToApplyDecalsEvent,  void, 0x004CB1A0, UInt32);
         DEFINE_MEMBER_FN(SendCombatEvent,                  void, 0x006E3F90, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendContainerChangedEvent,        void, 0x0047E570, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendDestructionStageChangedEvent, void, 0x00449A60, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEnterBleedoutEvent,           void, 0x006C4720, void**);
         DEFINE_MEMBER_FN(SendEquipEvent,                   void, 0x006C4780, void**, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendFurnitureEvent,               void, 0x00725000, void**, UInt32, bool);
         DEFINE_MEMBER_FN(SendGrabReleaseEvent,             void, 0x00742080, void**, UInt32);
         DEFINE_MEMBER_FN(SendEvent330, void, 0x006E3FF0, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent330_B, void, 0x006E4060, void**, refr_ptr*, UInt32 formID_maybeWeapon, UInt32, void*);
         DEFINE_MEMBER_FN(SendLockChangedEvent,             void, 0x004E05A0, void**);
         DEFINE_MEMBER_FN(SendMoveAttachDetachEvent,        void, 0x004E0600, void**, UInt32);
         DEFINE_MEMBER_FN(SendTranslationEvent, void, 0x004CB220, void**, UInt32); // TESObjectREFRTranslationEvent
         DEFINE_MEMBER_FN(SendOpenCloseEvent, void, 0x0044BC10, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendPerkEntryRunEvent, void, 0x0054C560, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendResetEvent, void, 0x004CB1C0, void**);
         DEFINE_MEMBER_FN(SendSceneEvent, void, 0x00557350, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendScenePhaseEvent, void, 0x0055A570, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendSellEvent, void, 0x0047E5C0, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendPlayerBowShotEvent, void, 0x004AA660, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent960, void, 0x0047E630, UInt32, UInt32, UInt32, UInt32, UInt32);
   };
   static_assert(offsetof(BSTEventSourceHolder, combat) >= 0x180, "BSTEventSourceHolder::combat is too early!");
   static_assert(offsetof(BSTEventSourceHolder, combat) <= 0x180, "BSTEventSourceHolder::combat is too late!");

   DEFINE_SUBROUTINE_EXTERN(void, AddHarvestEventSink, 0x00455C80, void*); // verify
}