#pragma once
#include <vector>

#include "Shared.h"
#include "Types.h"

#include "skse/GameBSExtraData.h"
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/GameObjects.h"
#include "skse/Utilities.h"

#include "ReverseEngineered/Forms/BGSBodyPartData.h"

//
// Notes:
//
//  - The third virtual method in BSExtraData* appears to be something like {bool BSExtraData::IsSameAs(BSExtraData* other)}.
//
//  - ExtraData type 0x31 is MagicCaster*, a subclass of BSExtraData, per dynamic casts seen in-game.
//
//  - ExtraData type 0x32 is used for non-actor magic targets; the BSExtraData* is dynamic-cast to NonActorMagicTarget*, a 
//    subclass of BSExtraData.
//

class BGSAcousticSpace;
class BGSEncounterZone;
class BGSMusicType;
namespace RE {
   //
   // Forward declarations, so the compiler doesn't choke:
   //
   class BSMultiBoundRoom;
   class DecalGroup;
   class ExtraDataList;
   class InventoryChanges;
   class TESNPC;
   class TESObjectREFR;
   //
   class IExtraDataSingleFormID : public BSExtraData { // helper/accessor
      public:
         UInt32 id; // 08
   };
   //
   #pragma region A
   class ExtraAction : public BSExtraData { // sizeof: 0x10
      public:
         ExtraAction(); // vanilla constructor at 0x00422650.
         virtual ~ExtraAction();
         //
         UInt32 unk08 = 1;
         UInt32 unk0C = 0;
   };
   class ExtraActivateRef : public BSExtraData { // sizeof == 0x14
      //
      // In some cases, the game will initialize this and even add the correct number 
      // of elements, but the refHandles will all be null. Of course, this leads to 
      // crashes when the game tries to use the data.
      //
      // Research needed. Subroutines currently of interest:
      //  - 0x00417990 (acts on the ExtraActivateRef and ExtraMissingRefIds extra-data on a reference)
      //  - 0x00?????? (code at 0x00417C90 acts on a reference's ExtraActivateRef)
      //  - 0x00?????? (code at 0x0041DE19 acts on a reference's ExtraActivateRef, creating it if it doesn't exist)
      //
      public:
         struct Entry { // sizeof == 0x08
            UInt32* refHandle; // 00 // actually this may be a UInt32*.
            Entry* nextEntry; // 04
            //
            MEMBER_FN_PREFIX(Entry);
            DEFINE_MEMBER_FN(InsertItemAfterMe, void, 0x0042AFA0, UInt32* newRefHandle, void* optionalConstructor, void* constructorArg); // may replace self instead // functions are optional; pass NULL for those args
            //
            Entry* SearchFromHere(TESObjectREFR* target) { // Untested. // CTD if there are any null refHandles in the list.
               return ( (Entry*(*)(TESObjectREFR*, Entry*))0x00422A50 )(target, this);
            };
         };
         Entry  firstEntry;    // 08
         bool   unk10 = false; // 10 // Parent Activate Only?
         UInt8  pad11[3];      // 11
         //
         // Use the version on BaseExtraList instead.
         //DEFINE_MEMBER_FN_LONG(ExtraActivateRef, GetEntryFor, Entry*, 0x00422A90, TESObjectREFR*); // CTD if there are any null refHandles in the list. // returns TESV_00422A50(Arg1, &this->unk08);
   };
   class ExtraActivateRefChildren : public BSExtraData { // sizeof == 0x14
      //
      // With all the code analysis we've done, this SHOULD be correct, but the game doesn't return any 
      // bloody references!
      //
      // Code of interest:
      //  - 0x0040D4A0 // returns extraData->unk08; maybe check callers to determine usage?
      //     - Caller: 0x004DDC10
      //     - Caller: 0x004EB940
      //     - Caller: 0x004EBA40
      //  - 0x0040D4C0 // setter for extraData->unk10
      //  - 0x0040D4E0 // getter for float extraData->unk10
      //  - 0x00411A70
      //  - 0x00417AED
      //
      public:
         struct Entry { // Structure based on code near 0x00417AED and confirmed by 0x004DDC10.
            UInt32 refHandle;
            Entry* nextEntry;
         };
         Entry* firstEntry; // 08
         UInt32 unk0C; // ?
         float  unk10; // per 0x0040D4E0
   };
   class ExtraAliasInstanceArray : public BSExtraData {
      public:
         //
         // ...
         //
         MEMBER_FN_PREFIX(ExtraAliasInstanceArray);
         DEFINE_MEMBER_FN(IsQuestObject, bool, 0x00425BB0);
   };
   class ExtraAshPileRef : public BSExtraData {
      public:
         struct Data {
            UInt32 handle;
         };
         Data data;
   };
   class ExtraAttachRefChildren : public BSExtraData {
      public:
         struct Entry {
            UInt32 unk00;
            // ...?
            Entry* nextEntry;
         };
         Entry firstEntry; // 08 // first entry in linked list
   };
   #pragma endregion
   #pragma region C
   class ExtraCellAcousticSpace : public BSExtraData { // sizeof == 0xC
      public:
         BGSAcousticSpace* data; // 08
   };
   class ExtraCellImageSpace : public BSExtraData { // sizeof == 0xC
      public:
         TESImageSpace* data; // 08
   };
   class ExtraCellMusicType : public BSExtraData { // sizeof == 0xC
      public:
         BGSMusicType* data; // 08
   };
   class ExtraCharge : public BSExtraData { // sizeof == 0x0C
      public:
         ExtraCharge(UInt32 arg1) : unk08(arg1) {};
         virtual ~ExtraCharge();
         //
         UInt32 unk08;
   };
   class ExtraCollisionData : public BSExtraData {
      public:
         ExtraCollisionData();
         virtual ~ExtraCollisionData();
         //
         struct Data {
            UInt32 collisionLayer; // LayerId
         };
         //
         Data* data = NULL;
         //
         static ExtraCollisionData* Create();
   };
   class ExtraContainerChanges : public BSExtraData {
      //
      // How the heck does inventory work?
      //
      //  - ExtraContainerChanges stores the difference between a reference's current 
      //    inventory and the initial inventory defined by its TESContainer (i.e. the 
      //    base form).
      //
      //  - InventoryEntryData holds all of the item data for a given item base form. 
      //    Its countDelta is the total difference in the item's count.
      //
      //  - An array of BaseExtraLists describes the items.
      //
      // So if you have three greatswords of the same base form, one of which has an 
      // enchantment, then you have...
      //
      //  - One InventoryExtraData, countDelta 3 (assuming you didn't spawn with any 
      //    of this sword).
      //
      //  - Two BaseExtraLists...
      //
      //      - ...one with ExtraCount = 2.
      //
      //      - ...one with ExtraCount = 1 and an enchantment.
      //
      public:
         using Data = InventoryChanges;
         Data* data; // 08
   };
   static DEFINE_SUBROUTINE(ExtraContainerChanges::Data*, GetExtraContainerChangesData,            0x00476800, TESObjectREFR*);
   static DEFINE_SUBROUTINE(ExtraContainerChanges::Data*, GetOrCreateExtraContainerChangesDataFor, 0x00477780, TESObjectREFR*);
   #pragma endregion
   #pragma region D
   class ExtraDismemberedLimbs : public BSExtraData { // sizeof == 0x24 -- smaller than in FO3
      public:
         enum DismembermentBits : UInt16 { // these are (1 << BGSBodyPartData::PartType).
            kDismembermentBit_MaybeHead = 2,
         };
         struct LimbState { // sizeof == 0x10
            tArray<TESObjectARMO*> wornArmor; // 00 // all worn armors, at the moment the limb was severed
            BGSBodyPartData::PartType limb; // 0C
            UInt8 unk0D; // 0D
            bool  unk0E; // 0E
            UInt8 unk0F; // 0F // maybe just padding?
         };
         
         UInt16   dismemberedLimbs; // 08 // bitmask
         bool     hasBeenEaten; // 0A // HasBeenEaten checks this. WerewolfFeedEffect sets it; maybe cannibalism does, too?
         UInt8    pad0B;
         SInt32   unk0C = -1; // 0C
         TESForm* unk10 = nullptr; // 10 // memory inspection shows this is the dismembered actor. why is this stored here? IsKilledObject checks whether a FormList contains this.
         tArray<LimbState*> unk14; // 14
         SInt32   killingBlowLimb = -1; // 20 // BGSBodyPartData::PartType; returned by GetKillingBlowLimb

         MEMBER_FN_PREFIX(ExtraDismemberedLimbs);
         DEFINE_MEMBER_FN(IsLimbSevered, bool, 0x00420A10, BGSBodyPartData::PartType limb);
         DEFINE_MEMBER_FN(SeverLimb,     void, 0x004269F0, Actor* subject, BGSBodyPartData::PartType limb, UInt8 entryUnk0D);
   };
   class ExtraDroppedItemList : public BSExtraData {
      public:
         struct Entry {
            UInt32 refHandle; // 00
            Entry* next;      // 04
            //
            DEFINE_MEMBER_FN_LONG(Entry, IsEmpty, bool, 0x006F48A0); // meant to be called on the list head; true if all fields are 0/nullptr
            DEFINE_MEMBER_FN_LONG(Entry, Subroutine0048CB30, void, 0x0048CB30, UInt32, UInt32); // Called after an Entry has been removed. Deallocates?
         };
         Entry firstEntry; // 08 // first entry in linked list
   };
   #pragma endregion
   #pragma region E
   class ExtraEditorID : public BSExtraData { // sizeof == 0xC
      public:
         BSFixedString data; // 08
         //
         MEMBER_FN_PREFIX(ExtraEditorID);
         DEFINE_MEMBER_FN(Constructor, ExtraEditorID, 0x0040A2E0, const char*);
   };
   class ExtraEnableStateChildren : public BSExtraData {
      public:
         struct Entry {
            UInt32 refHandle;
            Entry* nextEntry;
            //
            DEFINE_MEMBER_FN_LONG(Entry, Subroutine0048CB30, void, 0x0048CB30, UInt32, UInt32); // Called after an Entry has been removed. Deallocates?
         };
         UInt32 firstHandle;
         Entry* nextEntry;   // 08 // first entry in linked list
   };
   class ExtraEnableStateParent : public BSExtraData { // TODO: VERIFY THIS
      public:
         struct Entry {
            UInt32 handle;
            Entry* next = NULL;
         };
         UInt32 unk08;        // 08 // first child handle? or flags?
         Entry* firstEntry;   // 0C // are we sure there's a linked list here? I mean, why would there be?
   };
   #pragma endregion
   class ExtraFlags : public BSExtraData { // sizeof == 0xC
      public:
         ExtraFlags();
         virtual ~ExtraFlags();

         UInt32 flags; // 08

         enum Flags : UInt32 {
            //
            // In Skyrim Classic, these are the only flags, and ExtraFlags is seemingly only ever added 
            // to references (as opposed to cells, which can also have extra-data).
            //
            kFlag_REFR_ActivationBlocked = 0x00000001,
            kFlag_REFR_HasAshPile        = 0x00000002, // seems redundant. why not just GetExtraAshPileRef straightaway? bethesda probably had their reasons... :\ 
         };
   };
   class ExtraGhost : public BSExtraData { // sizeof: 0x0C
      public:
         ExtraGhost(bool isGhost) : isGhost(isGhost) {};
         virtual ~ExtraGhost();
         //
         bool isGhost;   // 08
         UInt8 pad09[3];
   };
   #pragma region L
   class ExtraLeveledCreature : public BSExtraData {
      public:
         virtual ~ExtraLeveledCreature();
         //
         TESActorBase* unk08; // can be nullptr
         //
         // ...?
   };
   class ExtraLightData : public BSExtraData {
      //
      // Applied to references to specify changes in light properties (relative to the values on the LIGH base form). 
      // Does not include radius data -- that's stored in the separate ExtraRadius.
      //
      public:
         ExtraLightData();
         virtual ~ExtraLightData();
         //
         float  modifiedFOV;  // 08
         float  modifiedFade; // 0C
         UInt32 unk10;        // 10 // Not any of the flags that the CK offers for light references.
         float  depthBias;    // 14
         UInt8  unk18;        // 18 // Not any of the flags that the CK offers for light references.
         UInt8  pad19[3];     // 19, 20, 21
         //
         static ExtraLightData* Create();
   };
   class ExtraLinkedRefChildren : public BSExtraData { // sizeof == 0x18
      //
      // Subroutines of interest:
      //  - BaseExtraList->TESV_004118C0(?), function unknown
      //
      public:
         struct Unk08 { // sizeof cannot be more than 0x10
            struct Item {
               UInt32 unk00;
               UInt32 unk04;
            };
            UInt32 unk00 = 0x80000000; // bitmask?
            tArray<Item> unk04;
            UInt32 unk0C = 0;
            //
            DEFINE_MEMBER_FN_LONG(Unk08, Subroutine00419390, bool, 0x00419390, void*);
         };
         //
         Unk08 unk08; // per 00419A60
         // ...?
   };
   class ExtraLock : public BSExtraData {
      public:
         ExtraLock(void* arg1) : unk08(arg1) {};
         virtual ~ExtraLock();
         //
         void* unk08;
         //
         MEMBER_FN_PREFIX(ExtraLock);
         DEFINE_MEMBER_FN(Subroutine00422390, SInt32, 0x00422390, TESObjectREFR*); // returns lock level?
   };
   #pragma endregion
   #pragma region M
   class ExtraMapMarker : public BSExtraData {
      public:
         enum {
            kFlag_IsVisible     = 0x01,
            kFlag_CanFastTravel = 0x02,
            kFlag_ShowAllHidden = 0x04, // what does this mean?
         };
         enum Type : UInt16 {
            kMapMarkerType_None = 0,
            kMapMarkerType_City = 1,
            kMapMarkerType_Town = 2,
            kMapMarkerType_Settlement = 3,
            kMapMarkerType_Cave = 4,
            kMapMarkerType_Camp = 5,
            kMapMarkerType_Fort = 6,
            kMapMarkerType_NordicRuin = 7,
            kMapMarkerType_DwemerRuin = 8,
            kMapMarkerType_Shipwreck = 9,
            kMapMarkerType_Grove = 10,
            kMapMarkerType_Landmark = 11,
            kMapMarkerType_DragonLair = 12,
            kMapMarkerType_Farm = 13,
            kMapMarkerType_WoodMill = 14,
            kMapMarkerType_Mine = 15,
            kMapMarkerType_ImperialCamp = 16,
            kMapMarkerType_StormcloakCamp = 17,
            kMapMarkerType_Doomstone = 18,
            kMapMarkerType_WheatMill = 19,
            kMapMarkerType_Smelter = 20,
            kMapMarkerType_Stable = 21,
            kMapMarkerType_ImperialTower = 22,
            kMapMarkerType_Clearing = 23,
            kMapMarkerType_Pass = 24,
            kMapMarkerType_Altar = 25,
            kMapMarkerType_Rock = 26,
            kMapMarkerType_Lighthouse = 27,
            kMapMarkerType_OrcStronghold = 28,
            kMapMarkerType_GiantCamp = 29,
            kMapMarkerType_Shack = 30,
            kMapMarkerType_NordicTower = 31,
            kMapMarkerType_NordicDwelling = 32,
            kMapMarkerType_Docks = 33,
            kMapMarkerType_Shrine = 34,
            kMapMarkerType_RiftenCastle = 35,
            kMapMarkerType_RiftenCapitol = 36,
            kMapMarkerType_WindhelmCastle = 37,
            kMapMarkerType_WindhelmCapitol = 38,
            kMapMarkerType_WhiterunCastle = 39,
            kMapMarkerType_WhiterunCapitol = 40,
            kMapMarkerType_SolitudeCastle = 41,
            kMapMarkerType_SolitudeCapitol = 42,
            kMapMarkerType_MarkarthCastle = 43,
            kMapMarkerType_MarkarthCapitol = 44,
            kMapMarkerType_WinterholdCastle = 45,
            kMapMarkerType_WinterholdCapitol = 46,
            kMapMarkerType_MorthalCastle = 47,
            kMapMarkerType_MorthalCapitol = 48,
            kMapMarkerType_FalkreathCastle = 49,
            kMapMarkerType_FalkreathCapitol = 50,
            kMapMarkerType_DawnstarCastle = 51,
            kMapMarkerType_DawnstarCapitol = 52,
            kMapMarkerType_DLC02_TempleOfMiraak = 53,
            kMapMarkerType_DLC02_RavenRock = 54,
            kMapMarkerType_DLC02_AllMakerStone = 55,
            kMapMarkerType_DLC02_TelMithryn = 56,
            kMapMarkerType_DLC02_ToSkyrim = 57,
            kMapMarkerType_DLC02_ToSolstheim = 58,
         };
         struct Data { // sizeof == 0xC
            TESFullName name; // 00
            UInt8  flags; // 08 // flags
            UInt8  pad09;
            Type   type; // 0A // possibly type

            MEMBER_FN_PREFIX(Data);
            //
            // When using setters, don't forget to call mapMarkerRefr->MarkChanged(0x80000000);
            //
            DEFINE_MEMBER_FN(GetFastTravelEnabled, bool, 0x00428FD0);
            DEFINE_MEMBER_FN(SetFastTravelEnabled, void, 0x00428FE0, bool enabled);
            DEFINE_MEMBER_FN(GetIsVisible, bool, 0x00428FA0);
            DEFINE_MEMBER_FN(SetIsVisible, void, 0x00428FB0, bool visible);
            DEFINE_MEMBER_FN(GetIsShowAllHidden, bool, 0x00429000);
            DEFINE_MEMBER_FN(GetType, Type, 0x00429010);
            DEFINE_MEMBER_FN(SetType, void, 0x00429020, Type);
            //
            bool IsSettlement() {
               switch (this->type) {
                  case kMapMarkerType_City:
                  case kMapMarkerType_Town:
                  case kMapMarkerType_Settlement:
                  case kMapMarkerType_DawnstarCapitol:
                  case kMapMarkerType_FalkreathCapitol:
                  case kMapMarkerType_MarkarthCapitol:
                  case kMapMarkerType_MorthalCapitol:
                  case kMapMarkerType_RiftenCapitol:
                  case kMapMarkerType_SolitudeCapitol:
                  case kMapMarkerType_WhiterunCapitol:
                  case kMapMarkerType_WindhelmCapitol:
                  case kMapMarkerType_WinterholdCapitol:
                  case kMapMarkerType_DLC02_RavenRock:
                  case kMapMarkerType_DLC02_TelMithryn:
                     return true;
               }
               return false;
            };
         };
         Data data; // 08
   };
   class ExtraMissingRefIds : public BSExtraData { // sizeof == 0x1C
      //
      // Intimately related to ExtraActivateRef, but I don't understand how.
      //
      // Subroutines of interest:
      //  - BaseExtraList::TESV_004155D0
      //
      public:
         struct Entry {
            Entry* next;
            UInt32 formID; // confirmed via memory examination
         };
         Entry  first; // confirmed via memory and code examination
         UInt32 count; // confirmed via memory and code examination
         UInt32 unk14; // 14 // a form ID // See 0x004DDD2A
         void*  unk18; // 18
         //
         MEMBER_FN_PREFIX(ExtraMissingRefIds);
         DEFINE_MEMBER_FN(FindFormByID, bool, 0x00414640, UInt32 formID, UInt32* outItemUnk04);
         DEFINE_MEMBER_FN(AppendItem,   void, 0x004277A0, UInt32 formID, UInt32 itemUnk04);
   };
   #pragma endregion
   class ExtraNorthRotation : public BSExtraData { // sizeof == 0xC
      public:
         float northRotation;
   };
   #pragma region P
   class ExtraPackage : public BSExtraData { // sizeof == 0x18
      public:
         struct Data { // sizeof == 0x10
            UInt32 unk00     = 0; // 00
            UInt32 refHandle = 0; // 04 // what reference?
            SInt16 unk08     = 0; // 08
            UInt16 pad0A; // 0A
            UInt8  unk0C; // 0C // Flags.
            UInt8  pad0D; // 0D
            UInt8  pad0E; // 0E
            UInt8  pad0F; // 0F
         };
         Data data; // 08
   };
   class ExtraPackageStartLocation : public BSExtraData { // sizeof: 0x1C
      public:
         ExtraPackageStartLocation(UInt32 arg1, UInt32 arg2, UInt32* arg3, UInt32 arg4) { // based on 0x00422840
            this->unk08 = arg2;
            this->unk0C = *(arg3 + 0); //
            this->unk10 = *(arg3 + 4); // Arg3 is a pointer to a data structure; not sure what
            this->unk14 = *(arg3 + 8); //
            this->unk18 = arg4;
         };
         virtual ~ExtraPackageStartLocation();
         //
         UInt32 unk08;
         UInt32 unk0C;
         UInt32 unk10;
         UInt32 unk14;
         UInt32 unk18;
   };
   class ExtraPoison : public BSExtraData { // sizeof: 0x10
      public:
         ExtraPoison(); // constructor is apparently at 0x00425920, but I can't understand it fully
         virtual ~ExtraPoison();
         //
         UInt32 unk08; // if it's zero, gets forced to 1? see 0x0040C880
         UInt32 unk0C; // form ID
   };
   class ExtraPrimitive : public BSExtraData {
      public:
         ExtraPrimitive();
         virtual ~ExtraPrimitive();
         //
         ::BGSPrimitive* primitive = nullptr;
         //
         // The primitive data only contains the size and type. For the collision 
         // layer, you also need a ExtraCollisionData.
         //
         static ExtraPrimitive* Create();
   };
   #pragma endregion
   #pragma region R
   class ExtraRadius : public BSExtraData {
      public:
         ExtraRadius();
         virtual ~ExtraRadius();
         //
         float radiusModifier;
         //
         static ExtraRadius* Create();
   };
   class ExtraRoom : public BSExtraData { // sizeof == 0xC
      //
      // This extra-data is assigned to a TESObjectREFR that is a room marker, and allows 
      // that reference to find its room node.
      //
      public:
         NiPointer<BSMultiBoundRoom> unk08; // 08
   };
   class ExtraRunOncePacks : public BSExtraData {
      public:
         struct Entry {
            UInt32 unk00;
            Entry* nextEntry;
         };
         Entry firstEntry; // 08 // first entry in linked list
   };
   #pragma endregion
   #pragma region S
   class ExtraShouldWear : public BSExtraData {
      public:
         struct Data {
            bool  unk00; // remember: a bool is stored as a byte
            // Could there be padding bytes after?
         };
         Data data; // 08
   };
   class ExtraSoul : public BSExtraData { // sizeof == 0x0C
      //
      // VTBL: 0x010796DC
      //
      public:
         enum SoulSize : UInt8 {
            kSoulSize_Empty = 0,
            kSoulSize_Petty = 1,
            kSoulSize_Lesser,
            kSoulSize_Common,
            kSoulSize_Greater,
            kSoulSize_Grand,
         };
         //
         ExtraSoul(SoulSize arg1) { // based on 0x00422730
            this->soulSize = arg1;
         };
         virtual ~ExtraSoul();
         //
         SoulSize soulSize; // 08 // known to be a UInt8; byte gets zero-extended to dword registers in some places
         UInt8    pad09[3];
   };
   class ExtraSound : public BSExtraData {
      public:
         struct Data { // sizeof == 0xC
            SInt32 unk00;
            UInt8  unk04;
            UInt8  pad05[3];
            UInt32 unk08;
         };
         Data data; // 08
   };
   class ExtraStartingPosition : public BSExtraData {
      //
      // VTBL: 0x01079710
      //
      public:
         NiPoint3 pos; // 08
         NiPoint3 rot; // 14
         BGSLocation* location; // 20
   };
   class ExtraStartingWorldOrCell : public BSExtraData {
      //
      // VTBL: 0x01079378
      //
      public:
         void* startingWorldOrCell; // 08
   };
   #pragma endregion
   #pragma region T
   class ExtraTeleport : public BSExtraData { // sizeof: 0x0C
      public:
         //
         // Note: Given two linked doors A and B, A's teleport marker exists in B's cell and 
         //       B's teleport marker exists in A's cell. A door's teleport marker is the 
         //       place you end up when leaving via the door, NOT the place you end up when 
         //       you're sent to the door.
         //
         struct TeleportData{
            UInt32 refHandle; // linked door?
            NiPoint3 markerPosition;
            NiPoint3 markerRotation;
            //
            // ...?
         };
         ExtraTeleport(TeleportData* arg1) : data(arg1) {};
         virtual ~ExtraTeleport();
         //
         TeleportData* data;
   };
   class ExtraTimeLeft : public BSExtraData { // sizeof: 0x0C
      public:
         ExtraTimeLeft(UInt32 arg1) : unk08(arg1) {};
         virtual ~ExtraTimeLeft();
         //
         UInt32 unk08;
   };
   #pragma endregion
   class ExtraWaterData : public BSExtraData {
      public:
         struct Unk08 {
            UInt32 pad00;
            UInt32 pad04;
            void*  unk08; // Probably a TESForm*; maybe a TESWaterForm*? Has an unkE0 field and at least five virtual methods: the fifth takes no arguments and returns void.
            void*  unk0C;
            //
            DEFINE_MEMBER_FN_LONG(Unk08, Subroutine006310E0, void, 0x006310E0, UInt32); // Sets bit 1 in this->unk08->unk04 to !!argument.
         };
         Unk08* unk08;
   };
   class NonActorMagicTarget : public BSExtraData {
      public:
         virtual ~NonActorMagicTarget();
         //
         MagicTarget data;
         UInt32 unk14;
         UInt32 unk18;
         UInt32 unk1C;
   };
   //
   class BaseExtraList {
      public:
         struct PresenceBitfield {
            UInt8 bits[0x18];
            //
            bool HasType(UInt32 type) const {
               UInt32 index = (type >> 3);
               UInt8 bitMask = 1 << (type % 8);
               return (bits[index] & bitMask) != 0;
            }
         };
         //
         BSExtraData*      m_data;     // 00
         PresenceBitfield* m_presence; // 04
         //
         inline BSExtraData* GetByType(UInt32 type) {
            return CALL_MEMBER_FN(this, GetByType)(type);
         };
         //
         MEMBER_FN_PREFIX(BaseExtraList);
         //
         // Core stuff.
         //
         DEFINE_MEMBER_FN(AddExtra,         void,     0x0040A790, void* extraDataObjectToAdd);// Can be passed NULL, in which case it does nothing. Handles types within the range of 0xB to 0x2B with a switch-case; not sure what it does with others
         DEFINE_MEMBER_FN(GetByType,        BSExtraData*, 0x0040A8A0, UInt32 type);               // Returns the specified extra-data object (analogous to SKSE's GetByType).
         DEFINE_MEMBER_FN(GetPrevExtraData, void*,    0x0040A970, UInt32 unknown);            // Returns the previous extra-data object. (SKSE calls it "next," but Beth calls it "previous.")
         DEFINE_MEMBER_FN(HasType,          bool,     0x0040A760, UInt32 type);
         DEFINE_MEMBER_FN(ItemsInList,      UInt32,   0x0040A710);                            // Returns the number of extra-data objects in the list.
         DEFINE_MEMBER_FN(RemoveAll,        void,     0x0040B890, UInt32 unknown);            // Not sure if this removes ALL, or just all of a specified type.
         DEFINE_MEMBER_FN(RemoveAllDefault, void,     0x0040B940, UInt32 unknown);            // Haven't looked into this. Name comes from a debug logging command.
         //
         DEFINE_MEMBER_FN(RemoveExtra_Sig1, void,     0x0040BB00, UInt32 arg1, UInt32 arg2);  // Haven't looked into this. Name "RemoveExtra" comes from a debug logging command.
         DEFINE_MEMBER_FN(RemoveExtra_Sig2, void,     0x0040BC10, UInt32 type);               // Not sure if this removes all of a specified type, or just the first one found.
         void RemoveExtra(UInt32 arg1, UInt32 arg2) {
            CALL_MEMBER_FN(this, RemoveExtra_Sig1)(arg1, arg2);
         };
         void RemoveExtra(UInt32 type) {
            CALL_MEMBER_FN(this, RemoveExtra_Sig2)(type);
         };
         //
         DEFINE_MEMBER_FN(HasExtraCannotWear, bool, 0x0040C460);
         DEFINE_MEMBER_FN(HasExtraFromAlias,  bool, 0x0040DEA0);
         //
         DEFINE_MEMBER_FN(GetActivateRefEntryFor, ExtraActivateRef::Entry*, 0x00417990, TESObjectREFR* ref); // if it returns NULL, then ref isn't an activate parent of the thing we're calling on
         DEFINE_MEMBER_FN(GetDecalGroup, DecalGroup*, 0x0040D3B0);
         DEFINE_MEMBER_FN(SetDecalGroup, void, 0x004178A0, DecalGroup*);
         //
         // Many or most of the void* return types are unverified. The values returned are four bytes, but they could be pointers 
         // to data structures or just single values (i.e. UInt32, SInt32, or a float returned without relying on the FPU stack).
         //
         DEFINE_MEMBER_FN(GetOrCreateExtraAction,           ExtraAction*,        0x0040C5F0);
         DEFINE_MEMBER_FN(GetOrCreateExtraMissingRefIds,    ExtraMissingRefIds*, 0x004155D0);
         DEFINE_MEMBER_FN(GetExtraActionUnk08,              UInt32,  0x0040CAB0); // Returns action byte (or 1 if no extra data).
         DEFINE_MEMBER_FN(TestExtraActionUnk08,             bool,    0x0040CAD0, UInt32); // Returns (action byte & Arg1), or (Arg1 & 1) if no extra data.
         DEFINE_MEMBER_FN(GetExtraActionUnk0C,              void*,   0x0040CB00); // Return type not verified.
         DEFINE_MEMBER_FN(GetExtraActivateLoopSound,        void,    0x0040C3A0, ExtraSound::Data* out);
         DEFINE_MEMBER_FN(GetExtraActivateRef,              ExtraActivateRef::Entry*, 0x0040D430); // Returns NULL if no extra data.
         DEFINE_MEMBER_FN(GetExtraActivateRefDelay,         float,   0x0040D450, TESObjectREFR*); // Returns zero if no extra data. Crashes if the data is malformed, as it calls GetEntryFor above.
         DEFINE_MEMBER_FN(GetExtraActivateRefChildren,      ExtraActivateRefChildren::Entry*, 0x0040D4A0); // Returns NULL if no extra data.
         DEFINE_MEMBER_FN(GetExtraActivateRefChildrenDelay, float,   0x0040D4E0); // Returns unk10 on the data. Returns -1 if no extra data.
         DEFINE_MEMBER_FN(GetExtraAshPileRef,               UInt32*, 0x00411850, UInt32* out); // Returns out after modifying its value. Not sure if the value is a ref handle.
         DEFINE_MEMBER_FN(GetExtraAttachRefChildren,        ExtraAttachRefChildren::Entry, 0x0040D350);
         DEFINE_MEMBER_FN(GetExtraCachedScaleUnk08,         float,   0x0040C240); // Returns extraDataObject->unk08 (or 1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraCachedScaleUnk0C,         float,   0x0040C240); // Returns extraDataObject->unk0C (or 1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraCellAcousticSpace,        BGSAcousticSpace*,   0x0040CDB0);
         DEFINE_MEMBER_FN(GetExtraCellImageSpace,           TESImageSpace*,   0x0040CDE0);
         DEFINE_MEMBER_FN(GetExtraCellMusicType,            BGSMusicType*,   0x0040CDA0);
         DEFINE_MEMBER_FN(GetExtraCellSkyRegion,            void*,   0x0040CDC0); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraCellWaterType,            void*,   0x0040D390); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraCharge,                   float,   0x0040C1D0); // Returns charge (or -1 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraContainerChangesData,     ExtraContainerChanges::Data*, 0x0040C090); // can be nullptr
         DEFINE_MEMBER_FN(GetExtraCount,                    UInt32,  0x0040C190); // Returns count, or 1 if no extra data.
         DEFINE_MEMBER_FN(GetExtraCreatureAwakeSound,       void,    0x0040C2E0, ExtraSound::Data* out);
         DEFINE_MEMBER_FN(GetExtraDecalGroup,               void*,   0x0040D3B0); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraDroppedItemList,          ExtraDroppedItemList::Entry, 0x0040D370);
         DEFINE_MEMBER_FN(GetExtraEditorID,                 void*,   0x0040CDF0); // Definitely returns a pointer or NULL.
         DEFINE_MEMBER_FN(GetExtraEnableStateChildren,      ExtraEnableStateChildren::Entry, 0x0040D330);
         DEFINE_MEMBER_FN(GetExtraEnableStateParentFlag1,   bool,    0x0040D2B0); // Returns extraDataObject->unk08 & 1, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraEnableStateParentFlag2,   bool,    0x0040D2D0); // Returns extraDataObject->unk08 & 2, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraEnableStateParentFlag4,   bool,    0x0040D2F0); // Returns extraDataObject->unk08 & 4, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraEncounterZone,            BGSEncounterZone*, 0x0040D9F0);
         DEFINE_MEMBER_FN(GetExtraForcedTargetRefHandle,    UInt32*, 0x0040E7C0, UInt32& refHandle); // returns &refHandle
         DEFINE_MEMBER_FN(GetExtraGhost,                    bool,    0x0040C3E0); // Returns ghost flag, or false if no extra data.
         DEFINE_MEMBER_FN(GetExtraGlobal,                   void*,   0x0040C0F0); // Return type not verified. Could be a form ID, I suppose.
         DEFINE_MEMBER_FN(GetExtraHealth,                   float,   0x0040C130); // Returns health (or -1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraHealthPerc,               float,   0x0040CB10); // Returns health percentage (or 1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraLastFinishedSequence,     void*,   0x0040DBC0); // Returns extraDataObject->unk08 (a char*?).
         DEFINE_MEMBER_FN(GetExtraLeveledCreature,          TESActorBase*, 0x0040D920);
         DEFINE_MEMBER_FN(GetExtraLeveledItem,              void*,   0x0040C152); // Returns the extra-data object itself. Typically, retval->unk08 would be either a void* or the start of the data.
         DEFINE_MEMBER_FN(GetExtraLeveledItemUnk08,         SInt32,  0x0040D070); // Returns extraDataObject->unk08  from a signed word.
         DEFINE_MEMBER_FN(GetExtraLightFOVModifier,         float,   0x0040BFF0); // Returns FOV modifier (or 0 if no extra data) directly, without using the FPU stack.
         DEFINE_MEMBER_FN(GetExtraLinkedRef,                TESObjectREFR*, 0x00415700, BGSKeyword*); // GetLinkedRef()
         DEFINE_MEMBER_FN(GetExtraLocation,                 void*,   0x0040DA10);
         DEFINE_MEMBER_FN(GetExtraLocationRefType,          void*,   0x0040D3D0); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraLock,                     void*,   0x0040C030);
         DEFINE_MEMBER_FN(GetExtraLockList,                 void*,   0x0040C0D0); // Return type not verified. Could be a form ID, I suppose.
         DEFINE_MEMBER_FN(GetExtraMapMarkerData,            ExtraMapMarker::Data*, 0x0040C070);
         DEFINE_MEMBER_FN(GetExtraObjectHealth,             float,   0x0040CB30); // Returns health (or -1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraOutfitItem,               void*,   0x0040C160); // Returns the extra-data object itself. Typically, retval->unk08 would be either a void* or the start of the data.
         DEFINE_MEMBER_FN(GetExtraOwnership,                void*,   0x0040C0B0); // Returns TESNPC* or TESFaction*.
         DEFINE_MEMBER_FN(GetExtraPackageUnk08,             void*,   0x0040CE00); // Returns extraDataObject->unk08 (or zero if no extra data).
         DEFINE_MEMBER_FN(GetExtraPackageUnk0C,             UInt32*, 0x0040CE40, UInt32* out); // Returns out. (Value is set to a reference handle.)
         DEFINE_MEMBER_FN(GetExtraPackageUnk10,             SInt32,  0x0040CE20); // Returns extraDataObject->unk10 (or zero if no extra data)  from a signed word.
         DEFINE_MEMBER_FN(GetExtraPackageUnk14Flag1,        bool,    0x0040CE70); // Returns extraDataObject->unk14 & 1, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraPackageUnk14Flag2,        bool,    0x0040CE90); // Returns extraDataObject->unk14 & 2, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraPackageUnk14Flag4,        bool,    0x0040CEB0); // Returns extraDataObject->unk14 & 4, or zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraPackageStartLocation,     void*,   0x0040C4C0); // Returns &extraDataObject->unk08, or NULL if no extra data.
         DEFINE_MEMBER_FN(GetExtraPersistentCell,           void*,   0x0040D130); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraPlayerCrimeList,          void*,   0x0040CFB0); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraPrimitive,                void*,   0x0040D5D0);
         DEFINE_MEMBER_FN(GetExtraRadius,                   float,   0x0040DB60); // Returns radius via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraRagdollData,              void*,   0x0040D150); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraRandomTeleportMarker,     void*,   0x0040D410); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraRank,                     UInt32,  0x0040C110); // Returns rank (or -1 if no extra data) directly, without using the FPU stack. Not sure if float.
         DEFINE_MEMBER_FN(GetExtraReferenceHandle,          ref_handle&, 0x004110F0, ref_handle& out); // Never modify *retval. Usually retval == out; sometimes it == g_invalidRefHandle.
         DEFINE_MEMBER_FN(GetExtraRegionList,               void*,   0x0040CD90); // Return type not verified.
         DEFINE_MEMBER_FN(GetExtraRoomNode,                 BSMultiBoundRoom*,   0x0040D6E0);
         DEFINE_MEMBER_FN(GetExtraScale,                    float,   0x0040C220); // Returns scale (or 1.0 if no extra data) via the FPU stack.
         DEFINE_MEMBER_FN(GetExtraSceneData,                void*,   0x0040D800); // Return type not verified. Returns NULL/zero if no extra data.
         DEFINE_MEMBER_FN(GetExtraShouldWear,               bool,    0x0040C480, ExtraShouldWear::Data* out);
         DEFINE_MEMBER_FN(GetExtraSoulSize,                 SInt32,  0x0040C200); // Returns soul (or 0 if no extra data).
         DEFINE_MEMBER_FN(GetExtraSound,                    void,    0x0040C2A0, ExtraSound::Data* out);
         //
         DEFINE_MEMBER_FN(GOCESP_Sig1, NiPoint3*,              0x004107C0, NiPoint3* out, TESObjectREFR* reference); // Returns out after altering it. If no extra-data present, creates it based on the reference's position and rotation.
         DEFINE_MEMBER_FN(GOCESP_Sig2, ExtraStartingPosition*, 0x0040F720, TESObjectREFR* reference);                // If no extra-data present, creates it based on the reference's position and rotation.
         NiPoint3*                     GetOrCreateExtraStartingPosition(NiPoint3* out, TESObjectREFR* reference) {
            return CALL_MEMBER_FN(this, GOCESP_Sig1)(out, reference);
         }
         ExtraStartingPosition*        GetOrCreateExtraStartingPosition(TESObjectREFR* reference) {
            return CALL_MEMBER_FN(this, GOCESP_Sig2)(reference);
         }
         //
         DEFINE_MEMBER_FN(GetExtraStartingWorldOrCell,      void*,   0x0040CA90); // Return type not verified. Could be a form ID, I suppose.
         DEFINE_MEMBER_FN(GetExtraTeleport,                 void*,   0x0040C052);
         DEFINE_MEMBER_FN(GetExtraTeleportData,             ExtraTeleport::TeleportData*, 0x0040C050);
         DEFINE_MEMBER_FN(GetExtraTextDisplayData,          ExtraTextDisplayData*, 0x00418590);
         DEFINE_MEMBER_FN(GetExtraTimeLeft,                 float,   0x0040C1B0); // Returns time left, or -1.0 if no extra data.
         DEFINE_MEMBER_FN(GetExtraTrespassPackageUnk08,     void*,   0x0040CF70); // Returns extraDataObject->unk08, or zero/null if no extra data.
         DEFINE_MEMBER_FN(GetExtraWaterData,                void*,   0x0040EAD0); // Returns extraDataObject->unk08.
         DEFINE_MEMBER_FN(GetExtraWeaponAttackSound,        void,    0x0040C360, ExtraSound::Data* out);
         DEFINE_MEMBER_FN(GetExtraWeaponIdleSound,          void,    0x0040C320, ExtraSound::Data* out);
         //
         DEFINE_MEMBER_FN(GetExtraUnknown3F, void*, 0x0040C010);
         //
         DEFINE_MEMBER_FN(IsQuestObject,  bool, 0x00418FE0);
         DEFINE_MEMBER_FN(TestExtraFlags, bool, 0x0040E850, UInt32 flags); // Returns true if any of the specified flags are set. Returns false if no extra data.
         //
         // FINDERS:
         //
         DEFINE_MEMBER_FN(ExtraRunOncePacksContains, bool, 0x0040D1F0, void* searchFor); // Checks searchFor against the unk00 of each linked list entry.
         //
         // SETTERS / CONSTRUCTORS:
         //
         DEFINE_MEMBER_FN(AddExtraPrimitive,                void,             0x0040D560, ::BGSPrimitive*); // does not check for an existing primitive!
         DEFINE_MEMBER_FN(RemoveExtraInfoGeneralTopic,      void,             0x00413050);
         DEFINE_MEMBER_FN(RemoveExtraSavedAnimation,        void,             0x00412E10);
         DEFINE_MEMBER_FN(RemoveExtraSavedHavokData,        void,             0x00412F40);
         DEFINE_MEMBER_FN(RemoveExtraSayTopicInfo,          void,             0x004133A0);
         DEFINE_MEMBER_FN(SetExtraCellAcousticSpace,        void,             0x00410DD0, BGSAcousticSpace*);
         DEFINE_MEMBER_FN(SetExtraCellImageSpace,           void,             0x00410F50, TESImageSpace*);
         DEFINE_MEMBER_FN(SetExtraCellMusicType,            void,             0x00410D10, BGSMusicType*);
         DEFINE_MEMBER_FN(SetExtraCharge,                   ExtraCharge*,     0x0040C780, UInt32);       // Creates the new extra-data if needed.
         DEFINE_MEMBER_FN(SetExtraCollisionData,            void,             0x00411FA0, ExtraCollisionData::Data*); // Destroys any existing extra-data, and then creates some if the supplied pointer is not null.
         DEFINE_MEMBER_FN(SetExtraCount,                    void,             0x0040FCE0, int32_t);
         DEFINE_MEMBER_FN(SetExtraEnableStateParentFlags,   void,             0x0040D310, UInt8 flags);  // Does nothing if no extra-data present.
         DEFINE_MEMBER_FN(SetExtraFlags,                    void,             0x00416C50, UInt32 flagsMask, bool value); // Creates the new extra-data if needed.
         DEFINE_MEMBER_FN(SetExtraForcedTargetRefHandle,    void,             0x00413F30, UInt32);
         DEFINE_MEMBER_FN(SetExtraGhost,                    ExtraGhost*,      0x0040C940, bool isGhost); // Sets the ghost flag. Creates the new extra-data if needed.
         DEFINE_MEMBER_FN(SetExtraHealth, void, 0x0040FC10, float health); // health == -1.0F results in removing the extra data
         DEFINE_MEMBER_FN(SetExtraItemDropper, void, 0x00415420, TESObjectREFR*); // Set who dropped this item.
         DEFINE_MEMBER_FN(SetExtraLock,                     ExtraLock*,       0x0040C560, void*);        // Deletes the unk08 on any existing lock data, and then sets a new unk08 pointer. Creates the new extra-data if needed.
         DEFINE_MEMBER_FN(SetExtraMapMarkerData,            void,             0x0040F960, ExtraMapMarker::Data*); // If argument is NULL, deletes existing data.
         DEFINE_MEMBER_FN(SetExtraMultiBoundRef, void, 0x00412860, uint32_t); // during load, receives a ref form ID; not sure if it needs a form post-load. if ref A sets ref B as its "extra multibound ref," then A will render from B's room marker regardless of its own position
         DEFINE_MEMBER_FN(SetExtraRadius,                   void,             0x00412A70, float value);  // Sets radius data. Creates new data if needed, or destroys it if setting to 0.
         DEFINE_MEMBER_FN(SetExtraSoul,                     ExtraSoul*,       0x0040C820, UInt8 value);  // Sets soul data. Creates the new extra-data if needed.
         DEFINE_MEMBER_FN(SetExtraStartingWorldOrCell,      void,             0x00414DA0, void* startingWorldOrCell); // If argument is NULL, deletes existing data.
         DEFINE_MEMBER_FN(SetExtraTimeLeft,                 ExtraTimeLeft*,   0x0040C6E0, UInt32);       // Creates the new extra-data if needed.
   };
   class ExtraDataList : public BaseExtraList {
      //
      // Does the type "BaseExtraList" ever actually appear anywhere? It seems like ExtraDataList is 
      // the same class: instances of ExtraDataList are constructed just by calling the constructor 
      // for BaseExtraList.
      //
      // The name "ExtraDataList" appears in some debug strings.
      //
      public:
         DEFINE_MEMBER_FN(CompareListForContainer,          bool, 0x0040BDF0, BaseExtraList* other, UInt32 unknown); // Returns true if both lists have the exact same contents in the same order; false otherwise.
         DEFINE_MEMBER_FN(RemoveAllCopyableExtra,           void, 0x0040B940, UInt32 unknown);                       // Haven't looked into this. Name comes from a debug logging command.
         DEFINE_MEMBER_FN(CopyList,                         void, 0x0041E6C0, void* unknown);                        // Haven't looked into this. Name comes from a debug logging command.
         DEFINE_MEMBER_FN(CopyListForContainer,             void, 0x0041E750, BaseExtraList* other, void* unknown);  // Haven't looked into this. Name comes from a debug logging command.
         DEFINE_MEMBER_FN(DuplicateExtraListForContainer,   void, 0x0041E8D0, void* unknown);                        // Haven't looked into this. Name comes from a debug logging command.
         DEFINE_MEMBER_FN(CopyListForReference,             void, 0x0041EA00, void* arg1, void* arg2);               // Haven't looked into this. Name comes from a debug logging command.
         //
         // SETTERS FOR EXTRA DATA:
         //
         DEFINE_MEMBER_FN(SetEncounterZone,            void, 0x004125A0, void* unknown);
         DEFINE_MEMBER_FN(RemoveNonPersistentCellData, void, 0x00414940);
   };
};