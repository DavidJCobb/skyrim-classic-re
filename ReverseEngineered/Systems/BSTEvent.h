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
            auto ev     = convertEvent(aEv);
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
         DEFINE_MEMBER_FN(Destructor,  void, 0x00695990);
         //
         DEFINE_MEMBER_FN(AddEventSink,    void, 0x006E3E30, pSink sink);
         DEFINE_MEMBER_FN(RemoveEventSink, void, 0x008CE0C0, pSink eventSink);
         DEFINE_MEMBER_FN(SendEvent,       void, 0x006EBC10, void* eventObj);
   };
   static_assert(sizeof(BSTEventSource<UInt32>) >= 0x30, "BSTEventSource is too small!");
   static_assert(sizeof(BSTEventSource<UInt32>) <= 0x30, "BSTEventSource is too large!");

   struct BGSActorCellEvent { // fired by PlayerCharacter::Unk_98; likely doesn't exist on other actors
      enum Type : UInt32 {
         kType_EnteringCell = 0,
         kType_ExitingCell  = 1,
      };
      UInt32 actorRefHandle; // 00
      UInt32 cellFormID;     // 04
      Type   eventType;      // 08
   };
   struct TESFormDeleteEvent {
      UInt32 refrFormID;
   };
   struct TESPackageEvent {
      enum Type : UInt32 {
         kType_PackageStart  = 0, // package form ID is for the new package
         kType_PackageEnd    = 1, // package form ID is for the old package
         kType_PackageChange = 2, // package form ID is for the old package
      };
      //
      TESObjectREFR* target; // 00
      UInt32 packageFormID; // 04
      Type   eventType; // 08
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
         BSTEventSource<UInt32> unk000;
         BSTEventSource<UInt32> activate;
         BSTEventSource<UInt32> unk060;
         BSTEventSource<UInt32> unk090;
         BSTEventSource<UInt32> unk0C0;
         BSTEventSource<UInt32> unk0F0;
         BSTEventSource<UInt32> unk120;
         BSTEventSource<UInt32> unk150;
         BSTEventSource<UInt32> combat; // 180
         BSTEventSource<UInt32> containerChanged; // 1B0
         BSTEventSource<UInt32> death; // 1E0
         BSTEventSource<UInt32> unk210; // 210
         BSTEventSource<UInt32> unk240;
         BSTEventSource<UInt32> unk270; // 270 // equip or unequip
         BSTEventSource<TESFormDeleteEvent> formDelete; // 2A0 // TESObjectREFR only, though
         BSTEventSource<UInt32> unk2D0;
         BSTEventSource<UInt32> unk300;
         BSTEventSource<UInt32> unk330;
         BSTEventSource<UInt32> unk360;
         BSTEventSource<UInt32> unk390;
         BSTEventSource<UInt32> unk3C0;
         BSTEventSource<UInt32> unk3F0;
         BSTEventSource<UInt32> unk420;
         BSTEventSource<UInt32> unk450; // possibly "respawn?"
         BSTEventSource<UInt32> unk480;
         BSTEventSource<UInt32> unk4B0; // fired by Papyrus:ObjectReference:TranslateTo and related Papyrus functions; maybe by other stuff?
         BSTEventSource<UInt32> unk4E0;
         BSTEventSource<TESPackageEvent> package;
         BSTEventSource<UInt32> unk540;
         BSTEventSource<UInt32> unk570;
         BSTEventSource<UInt32> unk5A0;
         BSTEventSource<UInt32> unk5D0;
         BSTEventSource<UInt32> unk600;
         BSTEventSource<UInt32> unk630;
         BSTEventSource<UInt32> unk660;
         BSTEventSource<UInt32> unk690; // sent by Scene::Stop, and by one other Scene method (possibly for scene completion)
         BSTEventSource<UInt32> unk6C0;
         BSTEventSource<UInt32> unk6F0; // related to scene phases; possibly scene phase completion
         BSTEventSource<UInt32> unk720;
         BSTEventSource<UInt32> unk750;
         BSTEventSource<UInt32> unk780;
         BSTEventSource<UInt32> unk7B0;
         BSTEventSource<UInt32> unk7E0;
         BSTEventSource<UInt32> unk810;
         BSTEventSource<UInt32> unk840;
         BSTEventSource<UInt32> unk870;
         BSTEventSource<UInt32> unk8A0;
         BSTEventSource<UInt32> unk8D0;
         BSTEventSource<UInt32> unk900;
         BSTEventSource<UInt32> unk930;
         BSTEventSource<UInt32> unk960;
         BSTEventSource<UInt32> unk990;
         BSTEventSource<UInt32> unk9C0;

         // all members are BSTEventSource instances, sizeof == 0x30

         MEMBER_FN_PREFIX(BSTEventSourceHolder);
         DEFINE_MEMBER_FN(Destructor,   void, 0x00436DE0);
         //
         DEFINE_MEMBER_FN(SendActivateEvent,   void, 0x004E0450, refr_ptr*, void*); // actually a smart refr_ptr
         DEFINE_MEMBER_FN(SendFormDeleteEvent, void, 0x00690AFD, UInt32 formID);
         DEFINE_MEMBER_FN(SendPackageEvent,    void, 0x0070C280, UInt32 packageFormID, refr_ptr*, TESPackageEvent::Type eventType); // actually a smart refr_ptr
         DEFINE_MEMBER_FN(SendEvent090, void, 0x004E04B0, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent120, void, 0x00437650, UInt32);
         DEFINE_MEMBER_FN(SendEvent150, void, 0x004CB1A0, UInt32);
         DEFINE_MEMBER_FN(SendEvent180, void, 0x006E3F90, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent1B0, void, 0x0047E570, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent210, void, 0x00449A60, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent240, void, 0x006C4720, void**);
         DEFINE_MEMBER_FN(SendEvent270, void, 0x006C4780, void**, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent2D0, void, 0x00725000, void**, UInt32, bool);
         DEFINE_MEMBER_FN(SendEvent300, void, 0x00742080, void**, UInt32);
         DEFINE_MEMBER_FN(SendEvent330, void, 0x006E3FF0, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent330_B, void, 0x006E4060, void**, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent3C0, void, 0x004E05A0, void**);
         DEFINE_MEMBER_FN(SendEvent450, void, 0x004E0600, void**, UInt32);
         DEFINE_MEMBER_FN(SendEvent4B0, void, 0x004CB220, void**, UInt32);
         DEFINE_MEMBER_FN(SendEvent4E0, void, 0x0044BC10, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent540, void, 0x0054C560, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent630, void, 0x004CB1C0, void**);
         DEFINE_MEMBER_FN(SendEvent690, void, 0x00557350, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent6F0, void, 0x0055A570, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent720, void, 0x0047E5C0, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent7E0, void, 0x004AA660, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEvent960, void, 0x0047E630, UInt32, UInt32, UInt32, UInt32, UInt32);
   };
   static_assert(offsetof(BSTEventSourceHolder, combat) >= 0x180, "BSTEventSourceHolder::combat is too early!");
   static_assert(offsetof(BSTEventSourceHolder, combat) <= 0x180, "BSTEventSourceHolder::combat is too late!");

   DEFINE_SUBROUTINE_EXTERN(void, AddHarvestEventSink, 0x00455C80, void*); // verify
}