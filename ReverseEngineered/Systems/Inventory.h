#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"

namespace RE {
   class Actor;
   class BGSKeyword;
   class TESNPC;

   class InventoryEntryData;
   class InventoryChanges;

   namespace InventoryUtils {
      class InventoryVisitor {
         //
         // NOTE: These visitors see only the ExtraContainerChanges data itself. These are container CHANGES: 
         // if there are items initially in the container, the visitors won't see them; and if items were 
         // removed from the container, the visitors will see negative amounts.
         //
         public:
            virtual void Dispose(bool) {}; // 00
            virtual BOOL Visit(InventoryEntryData*) { return true; }; // 01 // return false to "break" from the loop
            virtual bool Unk_02(UInt32, UInt32) { return true;  }; // 02
            virtual void Unk_03(InventoryEntryData*, UInt32, bool* out) {}; // 03 // *out = true; this->Execute(Arg1);
      };
      //
      #pragma region Classes that were in an anonymous namespace
         class CountObjectsWithKeywordFunctor : public InventoryVisitor {
            public:
               BGSKeyword* keyword; // 04
               UInt32      result;  // 08
         };
         class FindBaseObjectVisitor : public InventoryVisitor {
            public:
               TESForm* baseObject; // 04
               UInt16   unk08;
         };
         class FindBestSoulGemVisitor : public InventoryVisitor { // sizeof == 0xC; used by Bethesda to get the soul gem to fill during a soul trap
            public:
               Actor*      soulTrapTarget; // 04
               TESSoulGem* result = nullptr; // 08
         };
         class FindRefrObjectVisitor : public InventoryVisitor {
            public:
               UInt32 unk04; // 04
               UInt16 unk08;
         };
         class InitWornVisitor : public InventoryVisitor {
            public:
               TESNPC*        unk04; // 04
               TESObjectREFR* unk08; // 08 // the actor whose ArmorAddon we want to change
               UInt32 unk0C; // 0C // same as unk04?
               UInt32 unk10; // 10 // race
         };
      #pragma endregion
      #pragma region Classes that were actually in InventoryUtils
         class ItemFilter {
            public:
               static constexpr uint32_t vtbl = 0x0107F41C;
               //
               ItemFilter() { *(uint32_t*)this = vtbl; }
               //
               virtual ~ItemFilter() {};
               virtual bool Allow(InventoryEntryData&) { return true; }
               //
               // In order to construct instances of ItemFilter or any of its subclasses on the stack, we have 
               // to define bodies for its virtual functions. We're overwriting the vtbl directly, so we'll still 
               // end up using the vanilla functions (including any patches that other DLLs may make to them); 
               // these function bodies are just a formality, since the compiler can't understand what we're 
               // doing.
               //
         };
         class QuestItemFilter : public ItemFilter { // excludes quest objects
            public:
               static constexpr uint32_t vtbl = 0x010811D4;
               //
               QuestItemFilter() { *(uint32_t*)this = vtbl; }
         };
      #pragma endregion
   }
   
   class InventoryEntryData { // sizeof == 0xC
      public:
         TESForm* type; // 00
         tList<ExtraDataList>* extendDataList; // 04 // TODO: this is a singly-linked list, not a doubly-linked list
         SInt32   countDelta; // 08

         MEMBER_FN_PREFIX(InventoryEntryData);
         DEFINE_MEMBER_FN(Constructor_Copy, InventoryEntryData&, 0x00476AB0, const InventoryEntryData&); // copy-construct (this is a deep copy)
         DEFINE_MEMBER_FN(Constructor,      InventoryEntryData&, 0x004750C0, TESForm*, int32_t count);
         DEFINE_MEMBER_FN(Destructor, void, 0x00475110);
         DEFINE_MEMBER_FN(AbandonExtraData,    void, 0x00476A70); // use this instead of (Destructor) if your InventoryEntryData is a shallow copy
         DEFINE_MEMBER_FN(AppendExtraDataList, void, 0x00476BA0, ExtraDataList*, bool simplifyFirst); // if (simplifyFirst == true), then runs an (inlined) call to SimplifyExtendDataList before appending
         DEFINE_MEMBER_FN(GenerateName,    const char*, 0x00475AA0);
         DEFINE_MEMBER_FN(GetFirstOwner,   void*,  0x004755A0); // returns this->extendDataList[0]->GetExtraOwnership()
         DEFINE_MEMBER_FN(GetSoulLevel,    SInt32, 0x00475740); // charge amount
         DEFINE_MEMBER_FN(GetSoulSize,     UInt32, 0x004756F0); // enum
         DEFINE_MEMBER_FN(GetTemperHealth, float,  0x00475360); // returns the highest ExtraHealth, or 1.0F if that's larger
         DEFINE_MEMBER_FN(GetValue,        SInt32, 0x00475450);
         DEFINE_MEMBER_FN(IsQuestObject,   bool,   0x004759B0);
         DEFINE_MEMBER_FN(IsOwnedBy,       bool,   0x00477010, TESForm* actor, bool unk1);
         DEFINE_MEMBER_FN(IsWorn,          bool,   0x004758C0); // checks extra data
         DEFINE_MEMBER_FN(SimplifyExtendDataList, int32_t, 0x00476D80); // removes any BaseExtraLists that consist solely of ExtraCount. return value is an integer; meaning is unclear.
         DEFINE_MEMBER_FN(Subroutine004751F0, bool, 0x004751F0);

         ~InventoryEntryData() { CALL_MEMBER_FN(this, Destructor)(); }
         inline void Delete() {
            CALL_MEMBER_FN(this, Destructor)();
            FormHeap_Free(this);
         }

         void Merge(InventoryEntryData& other); // (other) will have its extra-data lists removed and brought over here. you should generally only call this when working with shallow-copy InventoryEntryData objects
         UInt8 GetSoulSize() const;
   };
   class InventoryChanges {
      public:
         tList<InventoryEntryData>* objList; // 00 // TODO: this is a singly-linked list, not a doubly-linked list
         TESObjectREFR* container; // 04
         float totalWeight; // 08 // -1.0F means "needs to be recomputed"
         float armorWeight; // 0C

         MEMBER_FN_PREFIX(InventoryChanges);
         DEFINE_MEMBER_FN(ContainsQuestObject,   bool,   0x00476110);
         DEFINE_MEMBER_FN(CountItemsWithKeyword, UInt32, 0x00475EB0, BGSKeyword*); // uses CountObjectsWithKeywordFunctor
         DEFINE_MEMBER_FN(ExecuteVisitor,        void,   0x00475D20, void* visitor);
         DEFINE_MEMBER_FN(ExecuteVisitorOnWorn,  void,   0x00475D50, void* visitor);
         DEFINE_MEMBER_FN(GetEquippedItemsWeight, float, 0x00476160, Actor* owner); // actor ref needed for perk checks
         DEFINE_MEMBER_FN(GetMatchingItemCount,  BOOL,   0x0047A4F0, TESForm* baseForm, InventoryUtils::ItemFilter&);
         DEFINE_MEMBER_FN(GetTotalWeight,        float,  0x0047B5F0);
         DEFINE_MEMBER_FN(RemoveAllLeveledItems, void,   0x0047B990);
         DEFINE_MEMBER_FN(RemoveAllNotInAlias,   void,   0x00478B10);
         DEFINE_MEMBER_FN(SetUniqueID,           void,   0x00482050, BaseExtraList* itemList, TESForm* oldForm, TESForm* newForm);
         DEFINE_MEMBER_FN(UnequipArmorFromSlot,  void,   0x00475F30, UInt32 bodyPartIndex, Actor* target);
         DEFINE_MEMBER_FN(GetItemCount,          SInt32, 0x0047A4D0, TESForm*);
         DEFINE_MEMBER_FN(CountItemTypes, UInt32, 0x0047A510, bool includeNonPlayableItems);
         DEFINE_MEMBER_FN(GetInventoryEntryAt, InventoryEntryData*, 0x0047D9C0, uint32_t index); // (index) is relative to (CountItemTypes) // returns a shallow copy: new entry data, but pointers to the BaseExtraLists of the others
         DEFINE_MEMBER_FN(FindEntry, InventoryEntryData*, 0x00477B20, TESForm* baseItem, uint32_t, uint32_t refrFormID);
   };

   static DEFINE_SUBROUTINE(InventoryChanges*, GetInventory,            0x00476800, TESObjectREFR*);
   static DEFINE_SUBROUTINE(InventoryChanges*, GetOrCreateInventoryFor, 0x00477780, TESObjectREFR*);
}