#pragma once
#include "skse/GameEvents.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Systems/AttackProcessing.h"

namespace RE {
   class refr_ptr;
   class Actor;
   class TESObjectCELL;
   class TESObjectREFR;

   template <typename EventStruct> class BSTEventSource; // forward-declare for BSTEventSink

   class BSTEventSinkBase {};
   template <typename EventStruct> class BSTEventSink : public BSTEventSinkBase {
      public:
         inline static EventStruct* convertEvent(void* a) { return (EventStruct*)a; };
         inline static BSTEventSource<EventStruct>* convertSource(void* a) { return (BSTEventSource<EventStruct>*)a; };
         //
         virtual ~BSTEventSink() {}
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
         DEFINE_MEMBER_FN(Destructor,  void, 0x00695990);
         //
         DEFINE_MEMBER_FN(AddEventSink,    void, 0x006E3E30, pSink sink);
         DEFINE_MEMBER_FN(RemoveEventSink, void, 0x008CE0C0, pSink eventSink);
         DEFINE_MEMBER_FN(SendEvent,       void, 0x006EBC10, void* eventObj);
   };
   static_assert(sizeof(BSTEventSource<UInt32>) >= 0x30, "BSTEventSource is too small!");
   static_assert(sizeof(BSTEventSource<UInt32>) <= 0x30, "BSTEventSource is too large!");

   // RTTI confirms that the following events exist as structs named "Event" inside of 
   // namespaces with specific names. Unknown if the functions or BSTEventSources were 
   // also nested in them; I'm doing it that way for simplicity.
   namespace BooksRead {
      struct Event; // TODO
      //
      static DEFINE_SUBROUTINE(void, AddSink, 0x004A3A00, BSTEventSink<Event>*); // constructs the event source if it hasn't been constructed yet
   }
   namespace DefaultObjectsReadyEvent {
      struct Event; // TODO
      static auto ptrEventSource = (BSTEventSource<Event>*)0x01B107B4;
      //
      static DEFINE_SUBROUTINE(void, AddSink, 0x004F91A0, BSTEventSink<Event>*); // constructs the event source if it hasn't been constructed yet
   }
   namespace LocationCleared {
      struct Event; // TODO
      static auto ptrEventSource = (BSTEventSource<Event>*)0x012E736C;
      //
      static DEFINE_SUBROUTINE(void, AddSink, 0x004B0730, BSTEventSink<Event>*); // constructs the event source if it hasn't been constructed yet
   }
   namespace ObjectiveState {
      struct Event; // TODO
      static auto ptrEventSource = (BSTEventSource<Event>*)0x01B10FE0;
      //
      static DEFINE_SUBROUTINE(void, AddSink, 0x00552110, BSTEventSink<Event>*); // constructs the event source if it hasn't been constructed yet
   }
   namespace QuestStatus {
      struct Event; // TODO
      static auto ptrEventSource = (BSTEventSource<Event>*)0x01B113A0;
      //
      static DEFINE_SUBROUTINE(void, AddSink, 0x0056E820, BSTEventSink<Event>*); // constructs the event source if it hasn't been constructed yet
   }

   struct BGSActorCellEvent { // fired by PlayerCharacter::Unk_98; likely doesn't exist on other actors
      enum Type : UInt32 {
         kType_EnteringCell = 0,
         kType_ExitingCell = 1,
      };
      UInt32 actorRefHandle; // 00
      UInt32 cellFormID;     // 04
      Type   eventType;      // 08
   };
   struct BGSEventProcessedEvent {
      UInt32 unk00;
      UInt8  unk04;
   };
   struct PositionPlayerEvent {
      uint32_t unk00; // enum. if == 3, an event listener asks the garbage collector to ClearAll. highest value checked for in code is 4.
   };
   struct TESActivateEvent {
      TESObjectREFR* target;        // 00
      TESObjectREFR* whoInteracted; // 04
   };
   struct TESActiveEffectApplyRemoveEvent {
      TESObjectREFR* unk00;
      TESObjectREFR* unk04;
      UInt16         unk08;
      UInt8          unk0A; // probably indicates whether we're applying or removing
   };
   struct TESActorLocationChangeEvent {
      TESObjectREFR* unk00; // refcount incremented just before event is sent and decremented after event is sent
      UInt32 unk04;
      UInt32 unk08;
   };
   struct TESBookReadEvent { // powers Papyrus ObjectReference.OnRead
      //
      // TODO: I can't seem to get this to fire at ALL. I should reverse-engineer the code 
      // that sends the event.
      //
      TESObjectREFR* book;  // 00
      UInt32         unk04;
      UInt16         unk08;
   };
   struct TESCellAttachDetachEvent {
      enum class attach_state : uint8_t {
         detach = 0, // at 004D087C // Papyrus treats this as OnCellDetach
         attach = 1, // at 004D09F6 // Papyrus treats this as OnCellAttach
         unk_02 = 2, // at 004D0AF5 // Papyrus treats this as OnCellAttach
         unk_03 = 3, // at 004D0F17 // Papyrus treats this as OnCellAttach
      };
      //
      // Fires when an object's parent cell is attached or detached. DOES NOT refer to the 
      // object itself being "attached" or "detached;" moving an object between cells will 
      // not fire this event, and the cell that an object is in when it attaches may not 
      // be the cell that it's in when it later detaches.
      //
      TESObjectREFR* ref;   // 00 // refcount should be managed by whatever fires the event
      attach_state   state; // 04 // whether we're attaching (true) or detaching (false)
      //
      inline bool is_attachment() const noexcept { return this->state != attach_state::detach; }
      inline bool is_detachment() const noexcept { return this->state == attach_state::detach; }
   };
   struct TESCellFullyLoadedEvent {
      TESObjectCELL* unk00;
   };
   struct TESCellReadyToApplyDecalsEvent {
      TESObjectCELL* unk00;
   };
   struct TESCombatEvent { // Papyrus OnCombatStateChanged
      Actor* subject;     // 00
      Actor* target;      // 04 // The target of the subject actor's scrutiny and hostility; the person the subject is/was searching for.
      SInt32 combatState; // 08
   };
   struct TESContainerChangedEvent {
      //
      // This BSTEvent powers Papyrus OnContainerChanged, OnItemAdded, and OnItemRemoved. It 
      // runs in any situation that would trigger those Papyrus events, including:
      //
      //  - Dropping an item
      //  - Transferring an item between containers
      //  - Picking up an item
      //  - Adding or removing items from a container using scripts or the console
      //  - Using an item that gets destroyed on use (potions, spell tomes, etc.)
      //  - Firing an arrow
      //
      // Miscellaneous observations:
      //
      //  - Eating an ingredient fires two identical events?
      //
      UInt32 sourceFormID; // 00 // container giving up the item; when picking up items from the world, this is 00000000
      UInt32 targetFormID; // 04 // container receiving the item; when dropping   items into the world, this is 00000000
      UInt32 itemFormID;   // 08 // item base form
      UInt32 countMoved;   // 0C
      UInt32 itemRefID;    // 10 // form ID of the item TESObjectREFR, if there is one; 00000000 otherwise
      UInt16 unk14;        // 14 // apparently the return value of ExtraContainerChangesData::Subroutine0047A4D0
   };
   struct TESDeathEvent { // OnDying, OnDeath
      //
      // This fires TWICE for an actor's death: once when they are fated to die, and 
      // again when they're actually dead; this is the distinction between Papyrus 
      // OnDying and OnDeath. The (isDead) bool indicates whether the actor is dying 
      // (false) or dead (true). Calls to victim->IsDead(0) should return the same 
      // result.
      //
      TESObjectREFR* victim; // 00
      TESObjectREFR* killer; // 04
      bool           isDead; // 08
   };
   struct TESDestructionStageChangedEvent {
      TESObjectREFR* ref;      // 00
      SInt32         oldStage; // 04
      SInt32         newStage; // 08
   };
   struct TESEnterBleedoutEvent {
      TESObjectREFR* actor;
   };
   struct TESEquipEvent {
      //
      // Fired for unequipping or equipping items. When switching from one item to 
      // another, BSTEvents are fired in order (unequip the old; equip the new). 
      // Expect these to fire when entering an area for the first time, as actors 
      // are apparently considered to equip their armor at that moment.
      //
      // Looting an equipped item off of a corpse sends an unequip event for that 
      // corpse. Killing an actor who has spells equipped and drawn will send un-
      // equip events for their spells.
      //
      // Furthermore, this fires when "using" any item in an inventory or container, 
      // including books, ingredients, and misc-items; using them fires an equip 
      // event but no matching unequip event. Be sure to check the item type when 
      // listening to this event.
      //
      // TODO: Does this fire when using a spell tome stored in another container?
      //
      enum Type : UInt8 {
         kType_Unequip = 0,
         kType_Equip   = 1,
      };
      TESObjectREFR* actor;
      UInt32 formID; // 04 // the weapon, armor, or spell that has been (un)equipped
      UInt32 unk08;
      UInt16 unk0C;
      Type   type; // 0E
      UInt8  pad0F;
   };
   struct TESFormDeleteEvent {
      UInt32 refrFormID; // 00
   };
   struct TESFurnitureEvent {
      enum Type : UInt32 {
         kType_Enter = 0,
         kType_Exit  = 1,
      };
      TESObjectREFR* actor;     // 00
      TESObjectREFR* furniture; // 04
      Type           type;      // 08
      //
      MEMBER_FN_PREFIX(TESFurnitureEvent);
      DEFINE_MEMBER_FN(Destructor, void, 0x0044BB80); // decrements refcounts on actor and furniture
   };
   struct TESGrabReleaseEvent { // Fires when the player starts or stops Z-keying an object; doesn't fire for telekinesis
      TESObjectREFR* ref;        // 00 // grabbed/released ref
      bool           isGrabbing; // 04 // whether grabbing or releasing
   };
   struct TESHitEvent {
      //
      // Fires when any reference is attacked, including static objects like walls and floors. This 
      // doesn't fire if you grab an object using telekinesis, but if you launch an object at some-
      // one using telekinesis, it counts as you hitting them with [WEAP:000001F4]Unarmed. Launching 
      // objects at Statics with telekinesis does not send a hit event, even though punching Statics 
      // while unarmed does.
      //
      // For concentration spells, seems to fire on a per-projectile basis, so you can receive a lot 
      // of these! Papyrus docs say this fires per-enchantment for enchanted weapons as well, but 
      // they also say that sourceFormID is always the weapon in that case; I should test that.
      //
      enum Flags : UInt8 {
         kFlag_PowerAttack = 0x01,
         kFlag_Unk10       = 0x10,
         kFlag_Unk20       = 0x20,
         kFlag_Unk40       = 0x40,
         kFlag_Unk80       = 0x80,
      };
      //
      // Constructor at 006E11A0 with 5 args, and another inlined in 006E3FF0
      //
      TESObjectREFR* target;   // 00
      TESObjectREFR* attacker; // 04
      UInt32 sourceFormID;     // 08 // can be a weapon or a spell; Papyrus docs say poisons, ingredients, enchantments, and explosions can show up here as well
      UInt32 projectileFormID; // 0C // for spells only? seems to be nullptr for projectiles from ranged weapons
      UInt8  flags;            // 10 // flags as high as 0xE0 seen; Papyrus offers four bool args, but this clearly has more
      //
      // Observed flags: 11110001
      //
      MEMBER_FN_PREFIX(TESHitEvent);
      DEFINE_MEMBER_FN(Destructor, void, 0x0044BB80); // decrements refcounts on first two refs // shared with TESFurnitureEvent
   };
   struct TESInitScriptEvent {
      TESObjectREFR* ref; // 00
   };
   struct TESLoadGameEvent { // OnPlayerLoadGame
      // This is a zero-size struct. Skyrim allocates a single byte for it in order to obey specific 
      // C/C++ standards (i.e. zero-size structs must be compiled to have size, so that they can be 
      // allocated).
   };
   struct TESLockChangedEvent {
      TESObjectREFR* ref; // 00
   };
   struct TESMagicEffectApplyEvent {
      //
      // I haven't investigated thoroughly enough, but I know that Cobb Rim Lighting causes this 
      // event to spam HARD. Cloak spells are dirt-cheap, but it's worth knowing about: don't do 
      // anything expensive in a sink for this event.
      //
      TESObjectREFR* target; // 00
      TESObjectREFR* caster; // 04
      UInt32         effectFormID; // 08
   };
   struct TESMagicWardHitEvent {
      enum Status : SInt32 {
         kStatus_FriendlyHit = 0, // per CK wiki; can wards detect/block friendly healing, or is this for friendly fire?
         kStatus_Absorbed    = 1,
         kStatus_WardBroke   = 2,
         //
         // I've seen Drain Life pass 0 when it was cast on the player by a hostile actor. Maybe 
         // 0 just means that the spell passed through the ward, but Bethesda had friendly heals 
         // in mind when they wrote the docs?
         //
      };
      TESObjectREFR* defender; // 00 // Actor casting the ward
      TESObjectREFR* attacker; // 04 // Actor casting the spell that hit the ward
      UInt32         incomingSpellFormID; // 08
      Status         status;   // 0C
   };
   struct TESMoveAttachDetachEvent {
      TESObjectREFR* ref;   // 00 // refcount should be managed by whatever fires the event
      UInt8          unk08; // 04
   };
   struct TESObjectLoadedEvent {
      //
      // Powers the ObjectReference.OnLoad and ObjectReference.OnUnload events in Papyrus.
      //
      UInt32 refrFormID; // 00
      bool   loading;    // 04 // if false, then this is an unload event
   };
   struct TESObjectREFRTranslationEvent {
      TESObjectREFR* ref;   // 00
      UInt32         unk04; // 04
   };
   struct TESOpenCloseEvent { // fires when the door completes its opening/closing animation
      enum Type : UInt8 {
         kType_Close = 0,
         kType_Open  = 1,
      };
      TESObjectREFR* door; // 00
      TESObjectREFR* whoInteracted; // 04
      Type           type; // 08
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
   struct TESPerkEntryRunEvent {
      TESObjectREFR* perkTarget;    // 00 // for an Activate perk entry point, the thing activated; not sure what this would be for other perk entry points
      TESObjectREFR* perkHaver;     // 04
      UInt32         perkFormID;    // 08
      UInt16         fragmentIndex; // 0C // perk script fragment index
      //
      MEMBER_FN_PREFIX(TESPerkEntryRunEvent);
      DEFINE_MEMBER_FN(Destructor, void, 0x0044BB80); // handles refcounts // same destructor as TESFurnitureEvent
   };
   struct TESPlayerBowShotEvent {
      UInt32 weaponFormID; // 00
      UInt32 ammoFormID;   // 04
      float  shotPower;    // 08 // by how much the player pulled the arrow; ranges from fArrowMinPower to 1.0F
      bool   isSungazing;  // 0C
   };
   struct TESQuestInitEvent {
      UInt32 questFormID; // 00
   };
   struct TESQuestStageEvent {
      UInt32 unk00; // 00
      UInt32 unk04; // 04 // form ID, probably for the quest -- but if so, then what is unk00?
      UInt16 stage; // 08
      UInt8  unk0A; // 0A
   };
   struct TESQuestStageItemDoneEvent {
      UInt32 questFormID; // 00
      UInt16 stage; // 04
      UInt8  unk06; // 06
      UInt8  pad07;
   };
   struct TESQuestStartStopEvent {
      UInt32 questFormID; // quest formID
      bool   unk04; // 04 // start/stop; not sure what value has what meaning
      UInt8  unk05; // 05
      UInt16 pad06;
   };
   struct TESResetEvent {
      TESObjectREFR* unk00;
   };
   struct TESResolveNPCTemplatesEvent {
      UInt32 unk00;
   };
   struct TESSceneEvent {
      UInt32 sceneFormID; // 00
      UInt32 unk04; // 04 // 0 = begin, 1 = end?
      void*  unk08; // 08
   };
   struct TESSceneActionEvent {
      UInt32 sceneFormID; // 00
      UInt32 actionIndex; // 04 // INAM subrecord; indices are not contiguous
      UInt32 questFormID; // 08
      UInt32 unk0C;       // 0C // either an enum or the action type
      TESObjectREFR* unk10;
   };
   struct TESScenePhaseEvent {
      UInt32 sceneFormID; // 00
      UInt32 unk04;
      UInt32 unk08;
      TESObjectREFR* unk0C;
   };
   struct TESSellEvent {
      TESObjectREFR* unk00;
      UInt32 unk04;
      UInt16 unk08;
   };
   struct TESSleepStartEvent {
      float  unk00;
      float  unk04; // time to sleep? same unit of measurement as gamehour, i.e. number of hours / 24.0F
   };
   struct TESSleepStopEvent {
      bool   interrupted;
   };
   struct TESSpellCastEvent { // fires once when casting starts; it's not once per projectile (i.e. concentration spells don't spam)
      TESObjectREFR* caster;      // 00
      UInt32         spellFormID; // 04 // actually, this can also be the base form ID of an eaten ingredient or, presumably, a potion/poison
   };
   struct TESSwitchRaceCompleteEvent {
      Actor* subject; // 00
   };
   struct TESTopicInfoEvent {
      //
      // These can fire out of order: if you initiate dialogue with an NPC and then select a topic 
      // before their currently-playing info finishes, then you get the new info's start event before 
      // the old info's end event.
      //
      // "Start" events fire when a decapitated actor tries to speak, even though they produce no 
      // sound or subtitles.
      //
      enum Type : UInt32 { // hm, i wanna check this a bit more
         kType_Start = 0,
         kType_End   = 1,
      };
      TESObjectREFR* speaker;    // 00
      void*          unk04;      // 04 // something refcounted; maybe sound data?
      UInt32         infoFormID; // 08
      Type           type;       // 0C
   };
   struct TESTrackedStatsEvent {
      StringCache::Ref statName; // 00
      SInt32           value;    // 04
   };
   struct TESTrapHitEvent {
      //
      // Constructor: 005894E0, takes 8 args
      //
      TESObjectREFR* unk00;
      TESObjectREFR* unk04;
      UInt32 unk08;
      UInt32 unk0C; // not set?
      float  unk10;
      float  unk14;
      float  unk18;
      float  unk1C;
      float  unk20;
      float  unk24;
      float  unk28;
      float  unk2C;
      SInt32 unk30; // either material or motion type
      SInt32 unk34; // either material or motion type
      bool   isInitialHit; // 38
      UInt8  unk39[3];
   };
   struct TESTriggerEvent {
      TESObjectREFR* unk00;
      TESObjectREFR* unk04;
   };
   struct TESTriggerEnterEvent {
      TESObjectREFR* unk00;
      TESObjectREFR* unk04;
   };
   struct TESTriggerLeaveEvent {
      TESObjectREFR* unk00;
      TESObjectREFR* unk04;
   };
   struct TESUniqueIDChangeEvent {
      UInt32 refrFormID; // 00 // form ID of a created   TESObjectREFR
      UInt32 unk04;      // 04 // form ID of a preplaced TESObjectREFR -- possibly a container for the created ref
      UInt32 baseFormID; // 08
      UInt16 unk0C; // 0C // maybe "old unique ID?"
      UInt16 unk0E; // 0E // maybe "new unique ID?"
   };
   struct TESWaitStartEvent {
      float  unk00; // 00
      float  days;  // 04 // seems wrong, based on testing; it's a float, but i don't have the meaning right?
   };
   struct TESWaitStopEvent {
      bool   interrupted;
   };

   extern BSTEventSource<UInt32>* const g_dayPassedEventSource;
   extern BSTEventSource<UInt32>* const g_menuModeChangeEventSource;

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
         BSTEventSource<BGSEventProcessedEvent>          eventProcessed;          // 000 (0x012E4C30) // BGSEventProcessedEvent
         BSTEventSource<TESActivateEvent>                activate;                // 030 (0x012E4C60) // 
         BSTEventSource<TESActiveEffectApplyRemoveEvent> activeEffectApplyRemove; // 060 (0x012E4C90) // TESActiveEffectApplyRemoveEvent
         BSTEventSource<TESActorLocationChangeEvent>     actorLocationChange;     // 090 (0x012E4CC0) // TESActorLocationChangeEvent
         BSTEventSource<TESBookReadEvent>                bookRead;                // 0C0 (0x012E4CF0) // TESBookReadEvent
         BSTEventSource<TESCellAttachDetachEvent>        cellAttachDetach;        // 0F0 (0x012E4D20) // TESCellAttachDetachEvent
         BSTEventSource<TESCellFullyLoadedEvent>         cellFullyLoaded;         // 120 (0x012E4D50) // TESCellFullyLoadedEvent
         BSTEventSource<TESCellReadyToApplyDecalsEvent>  cellReadyToApplyDecals;  // 150 (0x012E4D80) // TESCellReadyToApplyDecalsEvent
         BSTEventSource<TESCombatEvent>                  combat;                  // 180 (0x012E4DB0) // TESCombatEvent
         BSTEventSource<TESContainerChangedEvent>        containerChanged;        // 1B0 (0x012E4DE0) // TESContainerChangedEvent
         BSTEventSource<TESDeathEvent>                   death;                   // 1E0 (0x012E4E10) // 
         BSTEventSource<TESDestructionStageChangedEvent> destructionStageChanged; // 210 (0x012E4E40) // 
         BSTEventSource<TESEnterBleedoutEvent>           enterBleedout;           // 240 (0x012E4E70) // 
         BSTEventSource<TESEquipEvent>                   equip;                   // 270 (0x012E4EA0) // equip or unequip
         BSTEventSource<TESFormDeleteEvent>              formDelete;              // 2A0 (0x012E4ED0) // TESObjectREFR only, though
         BSTEventSource<TESFurnitureEvent>               furniture;               // 2D0 (0x012E4F00) // 
         BSTEventSource<TESGrabReleaseEvent>             grabRelease;             // 300 (0x012E4F30) //  
         BSTEventSource<TESHitEvent>                     hit;                     // 330 (0x012E4F60) // hit event
         BSTEventSource<TESInitScriptEvent>              initScript;              // 360 (0x012E4F90) // 
         BSTEventSource<TESLoadGameEvent>                loadGame;                // 390 (0x012E4FC0) // 
         BSTEventSource<TESLockChangedEvent>             lockChanged;             // 3C0 (0x012E4FF0) // 
         BSTEventSource<TESMagicEffectApplyEvent>        magicEffectApply;        // 3F0 (0x012E5020) // 
         BSTEventSource<TESMagicWardHitEvent>            magicWardHit;            // 420 (0x012E5050) // 
         BSTEventSource<TESMoveAttachDetachEvent>        moveAttachDetach;        // 450 (0x012E5080)
         BSTEventSource<TESObjectLoadedEvent>            objectLoaded;            // 480 (0x012E50B0) // 
         BSTEventSource<TESObjectREFRTranslationEvent>   objectReferenceTranslationEvent; // 4B0 (0x012E50E0) // fired by Papyrus:ObjectReference:TranslateTo and related Papyrus functions; maybe by other stuff?
         BSTEventSource<TESOpenCloseEvent>               openClose;               // 4E0 (0x012E5110) //
         BSTEventSource<TESPackageEvent>                 package;                 // 510 (0x012E5140) // 
         BSTEventSource<TESPerkEntryRunEvent>            perkEntryRun;            // 540 (0x012E5170) // 
         BSTEventSource<TESQuestInitEvent>               questInit;               // 570 (0x012E51A0) // 
         BSTEventSource<TESQuestStageEvent>              questStage;              // 5A0 (0x012E51D0) // 
         BSTEventSource<TESQuestStageItemDoneEvent>      questStageItemDone;      // 5D0 (0x012E5200) // TESQuestStageItemDoneEvent
         BSTEventSource<TESQuestStartStopEvent>          questStartStop;          // 600 (0x012E5230) // TESQuestStartStopEvent
         BSTEventSource<TESResetEvent>                   reset;                   // 630 (0x012E5260)
         BSTEventSource<TESResolveNPCTemplatesEvent>     resolveNPCTemplates;     // 660 (0x012E5290)
         BSTEventSource<TESSceneEvent>                   scene;                   // 690 (0x012E52C0) // sent by Scene::Stop, and by one other Scene method (possibly for scene completion)
         BSTEventSource<TESSceneActionEvent>             sceneAction;             // 6C0 (0x012E52F0)
         BSTEventSource<TESScenePhaseEvent>              scenePhase;              // 6F0 (0x012E5320) // related to scene phases; possibly scene phase completion
         BSTEventSource<TESSellEvent>                    sell;                    // 720 (0x012E5350)
         BSTEventSource<TESSleepStartEvent>              sleepStart;              // 750 (0x012E5380)
         BSTEventSource<TESSleepStopEvent>               sleepStop;               // 780 (0x012E53B0)
         BSTEventSource<TESSpellCastEvent>               spellCast;               // 7B0 (0x012E53E0)
         BSTEventSource<TESPlayerBowShotEvent>           playerBowShotEvent;      // 7E0 (0x012E5410)
         BSTEventSource<TESTopicInfoEvent>               topicInfo;               // 810 (0x012E5440)
         BSTEventSource<TESTrackedStatsEvent>            trackedStats;            // 840 (0x012E5470)
         BSTEventSource<TESTrapHitEvent>                 trapHit;                 // 870 (0x012E54A0)
         BSTEventSource<TESTriggerEvent>                 trigger;                 // 8A0 (0x012E54D0)
         BSTEventSource<TESTriggerEnterEvent>            triggerEnter;            // 8D0 (0x012E5500)
         BSTEventSource<TESTriggerLeaveEvent>            triggerLeave;            // 900 (0x012E5530)
         BSTEventSource<TESUniqueIDChangeEvent>          uniqueIDChange;          // 930 (0x012E5560)
         BSTEventSource<TESWaitStartEvent>               waitStart;               // 960 (0x012E5590)
         BSTEventSource<TESWaitStopEvent>                waitStop;                // 990 (0x012E55C0)
         BSTEventSource<TESSwitchRaceCompleteEvent>      switchRaceComplete;      // 9C0 (0x012E55F0)

         // all members are BSTEventSource instances, sizeof == 0x30

         MEMBER_FN_PREFIX(BSTEventSourceHolder);
         DEFINE_MEMBER_FN(Destructor,   void, 0x00436DE0);
         //
         DEFINE_MEMBER_FN(SendActivateEvent,                void, 0x004E0450, refr_ptr&, refr_ptr&);
         DEFINE_MEMBER_FN(SendActiveEffectApplyRemoveEvent, void, 0x00656860, refr_ptr&, refr_ptr&, UInt16, UInt8);
         DEFINE_MEMBER_FN(SendFormDeleteEvent,              void, 0x00690AFD, UInt32 formID);
         DEFINE_MEMBER_FN(SendPackageEvent,                 void, 0x0070C280, UInt32 packageFormID, refr_ptr& actor, TESPackageEvent::Type eventType); // actually a smart refr_ptr
         DEFINE_MEMBER_FN(SendActorLocationChangeEvent,     void, 0x004E04B0, refr_ptr& actor, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendBookReadEvent,                void, 0x00845CC0, refr_ptr&, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendCellAttachDetachEvent,        void, 0x004CB130, refr_ptr& ref, UInt8); // also fired at 004CEB5F
         DEFINE_MEMBER_FN(SendCellFullyLoadedEvent,         void, 0x00437650, TESObjectCELL*);
         DEFINE_MEMBER_FN(SendCellReadyToApplyDecalsEvent,  void, 0x004CB1A0, TESObjectCELL*);
         DEFINE_MEMBER_FN(SendCombatEvent,                  void, 0x006E3F90, refr_ptr&, refr_ptr&, SInt32 actorCombatState);
         DEFINE_MEMBER_FN(SendContainerChangedEvent,        void, 0x0047E570, UInt32 sourceFormID, UInt32 targetFormID, UInt32 itemFormID, UInt32 countMoved, UInt32 itemRefID, UInt32);
         DEFINE_MEMBER_FN(SendDeathEvent,                   void, 0x006C46C0, UInt32, UInt32); //
         DEFINE_MEMBER_FN(SendDeathEvent_B,                 void, 0x006C4660, UInt32, UInt32); // the difference between these two is a bool they store on the event struct -- a "murder" or "detected" bool?
         DEFINE_MEMBER_FN(SendDestructionStageChangedEvent, void, 0x00449A60, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendEnterBleedoutEvent,           void, 0x006C4720, refr_ptr& actor);
         DEFINE_MEMBER_FN(SendEquipEvent,                   void, 0x006C4780, refr_ptr& actor, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendFurnitureEvent,               void, 0x00725000, refr_ptr&, refr_ptr&, bool);
         DEFINE_MEMBER_FN(SendGrabReleaseEvent,             void, 0x00742080, refr_ptr& target, UInt8);
         DEFINE_MEMBER_FN(SendInitScriptEvent,              void, 0x004E0540, refr_ptr&);
         DEFINE_MEMBER_FN(SendHitEvent,                     void, 0x006E3FF0, refr_ptr& target, refr_ptr& attacker, UInt32 sourceFormID, UInt32 projectileFormID);
         DEFINE_MEMBER_FN(SendHitEvent_B,                   void, 0x006E4060, refr_ptr& target, refr_ptr& attacker, UInt32 sourceFormID, UInt32 projectileFormID, Struct00797220* hitDataCopy);
         DEFINE_MEMBER_FN(SendLockChangedEvent,             void, 0x004E05A0, refr_ptr&);
         DEFINE_MEMBER_FN(SendMagicEffectApplyEvent,        void, 0x006646E0, refr_ptr&, refr_ptr&, UInt32);
         DEFINE_MEMBER_FN(SendMagicWardHitEvent,            void, 0x006EBE70, refr_ptr&, refr_ptr&, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendMoveAttachDetachEvent,        void, 0x004E0600, refr_ptr& ref, UInt8);
         DEFINE_MEMBER_FN(SendObjectReferenceTranslationEvent, void, 0x004CB220, refr_ptr& ref, UInt32); // TESObjectREFRTranslationEvent
         DEFINE_MEMBER_FN(SendOpenCloseEvent,               void, 0x0044BC10, refr_ptr&, refr_ptr&, UInt8);
         DEFINE_MEMBER_FN(SendPerkEntryRunEvent,            void, 0x0054C460, refr_ptr&, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendPlayerBowShotEvent,           void, 0x004AA660, UInt32, UInt32, UInt32, bool isSungazing);
         DEFINE_MEMBER_FN(SendQuestStageEvent,              void, 0x00576290, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendResetEvent,                   void, 0x004CB1C0, refr_ptr&);
         DEFINE_MEMBER_FN(SendSceneEvent,                   void, 0x00557350, void*& smartPtr_not_a_refr, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendSceneActionEvent,             void, 0x0055A0A0, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendScenePhaseEvent,              void, 0x0055A570, void**, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendSellEvent,                    void, 0x0047E5C0, void**, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendSpellCastEvent,               void, 0x006591E0, refr_ptr&, UInt32);
         DEFINE_MEMBER_FN(SendSwitchRaceCompleteEvent,      void, 0x00665180, refr_ptr& actor);
         DEFINE_MEMBER_FN(SendTopicInfoEvent,               void, 0x0057DDD0, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendTrapHitEvent,                 void, 0x00589A80, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(SendTriggerEvent,                 void, 0x00589AE0, refr_ptr&, refr_ptr&);
         DEFINE_MEMBER_FN(SendTriggerEnterEvent,            void, 0x00589B40, refr_ptr&, refr_ptr&);
         DEFINE_MEMBER_FN(SendTriggerLeaveEvent,            void, 0x0058B9A0, refr_ptr&, refr_ptr&);
         DEFINE_MEMBER_FN(SendWaitStartEvent,               void, 0x0047E630, UInt32, UInt32, UInt32, UInt32, UInt32);
   };
   static_assert(offsetof(BSTEventSourceHolder, combat) >= 0x180, "BSTEventSourceHolder::combat is too early!");
   static_assert(offsetof(BSTEventSourceHolder, combat) <= 0x180, "BSTEventSourceHolder::combat is too late!");

   DEFINE_SUBROUTINE_EXTERN(void, AddHarvestEventSink, 0x00455C80, void*); // verify
}