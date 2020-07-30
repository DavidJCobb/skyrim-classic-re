#pragma once
#include <cstdint>
#include "ReverseEngineered/UI/IMenu.h"
#include "ReverseEngineered/UI/Miscellaneous.h"
#include "ReverseEngineered/Systems/BSTEvent.h"
#include "ReverseEngineered/Types.h"
#include "skse/GameTypes.h"

namespace RE {
   class InventoryEntryData;
   class TESFurniture;
   namespace CraftingSubMenus {
      class CraftingSubMenu;
   }

   class CraftingMenu : public IMenu {
      public:
         static constexpr uint32_t vtbl = 0x010E4324;
         //
         CraftingSubMenus::CraftingSubMenu* submenu = nullptr; // 1C
         //
         MEMBER_FN_PREFIX(CraftingMenu);
         DEFINE_MEMBER_FN(Constructor, CraftingMenu&, 0x0084EAF0);
   };

   struct Struct0086C790 { // sizeof == 0x28
      uint32_t unk00 = 0;
      uint32_t unk04 = 0;
      uint32_t unk08;
      uint32_t unk0C;
      BSString unk10; // has 0x800 bytes reserved for it
      uint32_t unk18 = 0;
      uint16_t unk1C = 0;
      uint16_t unk1E = 0;
      uint32_t unk20 = 0;
      uint32_t unk24;
      //
      MEMBER_FN_PREFIX(Struct0086C790);
      DEFINE_MEMBER_FN(Constructor, Struct0086C790&, 0x0086C790, void* someRefcountedObject);
   };
   struct Struct0087CE10 { // sizeof == 0x10
      uint32_t unk00 = 0;
      uint32_t unk04 = 0;
      uint32_t unk08;
      uint32_t unk0C;
      //
      MEMBER_FN_PREFIX(Struct0087CE10);
      DEFINE_MEMBER_FN(Constructor, Struct0087CE10&, 0x0087CE10, void* somethingWithAVTBL);
   };
   namespace CraftingSubMenus {
      class CraftingSubMenu : public GRefCountImplCore {
         public:
            static constexpr uint32_t vtbl = 0x010E440C;
            virtual void Dispose(bool); // 00 // virtual destructor
            virtual void Unk_01(uint32_t); // pure
            virtual void Unk_02();
            virtual void Unk_03();
            virtual bool Unk_04(); // returning true is the default
            virtual bool Unk_05(uint32_t); // returning false is the default
            virtual void Unk_06();
            virtual void Unk_07(uint32_t);
            //
            BSTEventSink<TESFurnitureEvent> eventInterface; // 08 // interface; VTBL: 0x010E4410
            GFxMovieView*   view;  // 0C // initialized based on constructor Arg1; not nullptr; has a vtbl
            TESFurniture*   unk10; // constructor Arg2
            Struct0086C790* unk14; // created in the constructor
            Struct0087CE10* unk18; // created in the constructor
            uint32_t unk1C;
            GFxValue unk20; // 20 // for Alchemy, this is the CategoryList
            GFxValue itemList; // 30 // these values are initialized via calls like view->GetVariable(unk30, "Menu.ItemList") where the first arg is an out-variable
            GFxValue unk40;    // 40
            GFxValue itemInfo; // 50
            GFxValue bottomBarInfo; // 60
            GFxValue additionalDescription; // 70 // byte unk08 indicates whether this is weapon smithing (2) or armor smithing (other)
            GFxValue menuName;   // 80
            GFxValue buttonText; // 90
            uint8_t  unkA0 = 0;
            uint8_t  padA1[3];
            uint32_t unkA4;

            MEMBER_FN_PREFIX(CraftingSubMenu);
            DEFINE_MEMBER_FN(Constructor, CraftingSubMenu&, 0x008505F0, GFxValue*, void*, TESFurniture* unk10);
            DEFINE_MEMBER_FN(AdvancePlayerSkill,    void, 0x008516E0, float by); // inlined in some places including ConstructibleObjectMenu::CraftItem
            DEFINE_MEMBER_FN(SetDescription,        void, 0x0084CC70, const char*);
            DEFINE_MEMBER_FN(UpdatePlayerSkillInfo, void, 0x0084C8C0, uint8_t actorValueIndex);
      };
      static_assert(sizeof(CraftingSubMenu) >= 0xA8, "RE::CraftingSubMenus::CraftingSubMenu is too small!");
      static_assert(sizeof(CraftingSubMenu) <= 0xA8, "RE::CraftingSubMenus::CraftingSubMenu is too large!");

      class AlchemyMenu : public CraftingSubMenu { // sizeof == 0x100, allocated on the Scaleform heap
         public:
            static constexpr uint32_t vtbl = 0x010E4688;
            //
            class CraftItemCallback : public IMessageBoxCallback { // sizeof == 0x10, allocated on game heap
               public:
                  static constexpr uint32_t vtbl = 0x010E46AC;
                  //
                  AlchemyMenu* owner; // 08
                  uint8_t unk0C;
                  uint8_t pad0D[3];
            };
            class QuitMenuCallback : public IMessageBoxCallback { // sizeof == 0xC, allocated on game heap
               public:
                  static constexpr uint32_t vtbl = 0x010E46B8;
                  //
                  AlchemyMenu* owner; // 08
                  //
                  MEMBER_FN_PREFIX(QuitMenuCallback);
                  DEFINE_MEMBER_FN(Constructor, QuitMenuCallback&, 0x00853C60, AlchemyMenu**);
            };
            //
            struct EntryData { // sizeof == 0xC
               InventoryEntryData* ingredient = nullptr; // 00
               uint32_t filterFlag = 0;     // 04
               bool     selected   = false; // 08 // indicates whether or not the ingredient is selected
               uint8_t  enabled    = true;  // 09 // indicates whether or not the ingredient is dimmed
               uint16_t pad0A      = 0;
            };
            class EntryDataList : public tArray<EntryData> {
               public:
                  MEMBER_FN_PREFIX(EntryDataList);
                  DEFINE_MEMBER_FN(InsertAt, void, 0x0084EE40, uint32_t index, EntryData& ed); // does not copy the InventoryEntryData. it's actually a move-insert, i.e. (ed) has the pointer taken away from it
            };
            //
            EntryDataList availableIngredients; // A8 // sorted by ingredient name
            BSTSmallArray<uint32_t, 3> selections; // B4 // selected ingredient indices in availableIngredients
            tArray<uint32_t> unkC8; // array of EffectItem*?
            uint32_t unkD4;
            uint32_t unkD8 = 0; // GFxValue?
            uint32_t unkDC = 0; //
            uint32_t unkE0;     //
            uint32_t unkE4;     //
            uint32_t unkE8 = 0;
            uint32_t unkEC;
            uint32_t unkF0 = 0;
            uint32_t unkF4 = 0;
            StringCache::Ref unkF8; // F8 // "<font color=\"#%x\">%s</font>"
            bool     purifyIngredients = false; // FC // indicates whether the player has the Purity Alchemy Ingredients perk, which strips negative effects from potions and positive effects from poisons. is set once, in the constructor
            uint8_t  padFD[3];
            //
            MEMBER_FN_PREFIX(AlchemyMenu);
            DEFINE_MEMBER_FN(Constructor,        AlchemyMenu&, 0x008537C0, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(TryCraftItem,       void,         0x00855290); // attempts to create a potion or poison
            DEFINE_MEMBER_FN(Subroutine0084DEE0, void,         0x0084DEE0, int32_t);
            DEFINE_MEMBER_FN(UpdatePotionRequirementUI, void, 0x0084E030); // display potion requirements?
            DEFINE_MEMBER_FN(RebuildItemList,           void, 0x00854E20, bool preserveSelections);
            DEFINE_MEMBER_FN(RebuildItemList_Internal,  void, 0x00850480);
            DEFINE_MEMBER_FN(ToggleSelection,           void, 0x008546E0, uint32_t availableIngredientIndex);

            // RebuildItemList_Internal is responsible for looping over the player's inventory and adding 
            // all usable crafting ingredients to the (availableIngredients) list.
            //
            // TryCraftItem is responsible for consuming items in the player's inventory, by way of a call 
            // to Unk_56 on g_thePlayer. That function appears to be usable for both dropping items into 
            // the world and straight-up removing them from existence, and is used to consume alchemy 
            // ingredients. Curiously, not all of the other crafting menus use it.
            //
            // We can potentially get the AlchemyMenu to pull from arbitrary containers instead of (or 
            // in addition to) the player by:
            //
            //  - Patch over 0x008504D5 (a call to CountItemTypes) to call ImportIngredientsFrom on all 
            //    desired containers. The patch should return to 0x00850587.
            //
            //  - Shim the call to TESObjectREFR::Unk_56 at 0x0085562D. That call, with the specific 
            //    arguments that it has, is responsible for removing a used ingredient from the player's 
            //    inventory. It should be trivial to loop over whatever containers you want to use, find 
            //    the first one (according to your desired search order) that has one or more of the 
            //    target ingredient, and then call Unk_56 on that container, forwarding the original 
            //    arguments.

            void ImportIngredientsFrom(TESObjectREFR* container, bool merge = false); // does not update UI state; just adds to availableIngredients
            int32_t GetIndexOfIngredientName(EntryData& ed, bool& exists);
      };
      static_assert(sizeof(AlchemyMenu) >= 0x100, "RE::CraftingSubMenus::AlchemyMenu is too small!");
      static_assert(sizeof(AlchemyMenu) <= 0x100, "RE::CraftingSubMenus::AlchemyMenu is too large!");
      static_assert(offsetof(AlchemyMenu, selections) >= 0xB4, "RE::CraftingSubMenus::AlchemyMenu::selections is too early!");
      static_assert(offsetof(AlchemyMenu, selections) <= 0xB4, "RE::CraftingSubMenus::AlchemyMenu::selections is too late!");
      static_assert(offsetof(AlchemyMenu, unkC8) >= 0xC8, "RE::CraftingSubMenus::AlchemyMenu::unkC8 is too early!");
      static_assert(offsetof(AlchemyMenu, unkC8) <= 0xC8, "RE::CraftingSubMenus::AlchemyMenu::unkC8 is too late!");

      class ConstructibleObjectMenu : public CraftingSubMenu { // sizeof == 0xE0, allocated on the Scaleform heap
         //
         // Only used for crafting new items; for tempering, see SmithingMenu.
         //
         public:
            static constexpr uint32_t vtbl = 0x010E4888;
            static constexpr uint32_t rtti = 0x012B2D3C;
            //
            using unknown = uint32_t; // stop-gap solution for making the tHashSet template args readable
            using form_id = uint32_t;
            //
            struct EntryData {
               BGSConstructibleObject* form = nullptr; // 00
               uint32_t filter = 0; // 04
               //
               DEFINE_MEMBER_FN(AttemptCraft, bool, 0x004916D0); // rechecks the player's inventory for required items and returns true if crafting succeeds
            };
            //
            tArray<EntryData> unkA8; // A8
            BSTHashMap<form_id, unknown> unkB4; // B4 // <item, key> == <unknown, keyword form ID>
            uint32_t unkD4 = 0;
            uint32_t unkD8 = 0;
            uint32_t unkDC = 0;
            //
            MEMBER_FN_PREFIX(ConstructibleObjectMenu);
            DEFINE_MEMBER_FN(Constructor, ConstructibleObjectMenu&, 0x00857E70, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(CraftItem,   void, 0x00858E20);
            DEFINE_MEMBER_FN(Subroutine00857C60, void, 0x00857C60); // builds unkA8 and unkB4
            DEFINE_MEMBER_FN(Subroutine00856230, void, 0x00856230, void*);
            DEFINE_MEMBER_FN(UpdateDisplayedRequirements, void, 0x00850C20); // accesses player inventory; updates the required materials list
      };
      static_assert(sizeof(ConstructibleObjectMenu) >= 0xE0, "RE::CraftingSubMenus::ConstructibleObjectMenu is too small!");
      static_assert(sizeof(ConstructibleObjectMenu) <= 0xE0, "RE::CraftingSubMenus::ConstructibleObjectMenu is too large!");

      class EnchantConstructMenu : public CraftingSubMenu { // sizeof == 0x158, allocated on the Scaleform heap
         public:
            static constexpr uint32_t vtbl = 0x010E4460;
            //
            class CreateEffectFunctor { // sizeof >= 0x24
               public:
                  static constexpr uint32_t vtbl = 0x010E435C;
                  //
                  uint32_t unk00;
                  uint16_t unk04 = 0;
                  uint32_t unk08;
                  uint32_t unk0C;
                  uint32_t unk10;
                  uint32_t unk14 = 0;
                  uint32_t unk18 = 0;
                  uint32_t unk1C = 0;
                  uint32_t unk20 = 0;
                  //
                  // No constructor. This is a member of EnchantConstructMenu and is constructed as 
                  // part of that class's constructor.
                  //
                  MEMBER_FN_PREFIX(CreateEffectFunctor);
                  DEFINE_MEMBER_FN(Destructor, void, 0x0084F310);
            };
            class ItemChangeEntry { // sizeof == 0x14, allocated on the game heap
               public:
                  static constexpr uint32_t vtbl = 0x010E4240;
                  virtual ~ItemChangeEntry();
                  virtual void Unk_01();
                  virtual void Unk_02(bool);
                  virtual const char* GetItemName(); // 03
                  virtual void Unk_04(uint32_t);
                  //
                  uint32_t unk04 = 0; // refcount
                  uint32_t unk08; // == 0x40 for soul gems; == 0x04 for armor at least sometimes
                  uint8_t  unk0C = false; // probably "selected", same as AlchemyMenu::EntryData
                  uint8_t  unk0D = true; // probably "enabled", same as AlchemyMenu::EntryData
                  uint16_t pad0E = 0;
                  InventoryEntryData* item = nullptr; // 10 // owned by ItemChangeEntry
                  //
                  MEMBER_FN_PREFIX(ItemChangeEntry);
                  DEFINE_MEMBER_FN(Constructor, ItemChangeEntry&, 0x0084C580, uint32_t unk08, InventoryEntryData* item);
                  //
                  static ItemChangeEntry* make(uint32_t unk08, InventoryEntryData* item);
            };
            class IngredientList : public tArray<ItemChangeEntry*> {
               public:
                  MEMBER_FN_PREFIX(IngredientList);
                  DEFINE_MEMBER_FN(Append, int32_t, 0x0084EC70, ItemChangeEntry*); // returns index
            };
            //
            uint32_t unkA8 = 0x0A;
            uint32_t unkAC = 0;
            uint32_t unkB0 = 5;
            uint32_t unkB4 = 0x30;
            uint32_t unkB8 = 0x40;
            IngredientList inventory; // BC // used for soul gems and armor; probably used for weapons, too
            BSString unkC8; // 3D model path?
            GFxValue unkD0;
            GFxValue unkE0; // entryList?
            struct {
               void* unk00 = nullptr; // 00 // selected item to enchant?
               void* unk04 = nullptr; // 04 // selected soul gem
               tArray<uint32_t> unk08; // 08 // enchantments to apply?
            } unkF0;
            int32_t  maxEnchantments; // 104 // max number of enchantments on a single item. the Mod Applied Enchantments Allowed perk entry point is checked once, in the constructor
            InventoryEntryData* unk108 = 0; // 108 // see SetItem3D
            CreateEffectFunctor unk10C;
            uint32_t unk130 = 0;
            float    unk134 = 1.0F;
            int32_t  unk138 = -1;
            float    unk13C = 1.0F;
            uint32_t unk140 = 0;
            uint32_t unk144 = 0;
            uint32_t unk148 = 0x7F;
            uint32_t unk14C = 0;
            uint32_t unk150 = 0;
            uint8_t  unk154 = 0;
            uint8_t  unk155 = 0;
            uint8_t  unk156 = 0;
            uint8_t  unk157 = 0;
            //
            MEMBER_FN_PREFIX(EnchantConstructMenu);
            DEFINE_MEMBER_FN(Constructor, EnchantConstructMenu&, 0x00850D90, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(Disenchant, void, 0x008530D0);
            DEFINE_MEMBER_FN(Enchant,    void, 0x00852D70);
            DEFINE_MEMBER_FN(SetItem3D,  void, 0x0084EA60, InventoryEntryData*);
            DEFINE_MEMBER_FN(Subroutine0084D2B0, void, 0x0084D2B0);
            DEFINE_MEMBER_FN(Subroutine0084DDB0, void, 0x0084DDB0, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(Subroutine0084E340, void, 0x0084E340, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(Subroutine00852040, void, 0x00852040); // related to renaming items
            DEFINE_MEMBER_FN(RebuildItemList,    void, 0x008500C0, uint32_t alwaysOne);
            //
            ItemChangeEntry* GetItemEntry(TESForm* itemBase);
            static bool IsItemValidForUse(InventoryEntryData*); // doesn't check form-type-specific stuff e.g. empty soul gems
            void ImportSoulGemsFrom(TESObjectREFR* container, bool merge = false); // does not update UI state; just adds to (inventory)

            // Enchant consumes a soul gem by way of a call to TESObjectREFR::Unk_56 on the player, at 
            // 0x00852F4B. It may use a call at 0x00852FBA to notify the player of the soul gem's 
            // removal as well.
            //
            // RebuildItemList uses a single loop to populate unkBC with the player's filled soul gems 
            // and enchantable weapons and armor.
      };
      static_assert(sizeof(EnchantConstructMenu) >= 0x158, "RE::CraftingSubMenus::EnchantConstructMenu is too small!");
      static_assert(sizeof(EnchantConstructMenu) <= 0x158, "RE::CraftingSubMenus::EnchantConstructMenu is too large!");

      class SmithingMenu : public CraftingSubMenu { // sizeof == 0xE8, allocated on the Scaleform heap
         //
         // Only used for tempering existing items; for crafting, see ConstructibleObjectMenu.
         //
         // Items use ExtraHealth to store how much they've been tempered. For the purposes of tempering, 
         // the minimum value is 1.0F.
         //
         // AlchemyMenu iterates over the player's inventory and stores a list of all of their crafting 
         // materials. SmithingMenu doesn't appear to behave in quite the same way; it stores lists of 
         // all COBJs available to the current workbench, and of all of the player's improveable items, 
         // but it doesn't store a list of materials. Rather, when the (improveableItems) list is being 
         // filled, BGSConstructibleObject::PlayerHasNeededItems is called in the list item constructor, 
         // and that accesses the player's inventory to see if they have the needed supplies.
         //
         public:
            static constexpr uint32_t vtbl = 0x010E4778;
            //
            class SmithingConfirmCallback : public IMessageBoxCallback { // sizeof == 0xC
               public:
                  static constexpr uint32_t vtbl = 0x010E479C;
                  static constexpr uint32_t rtti = 0x012B2CF8;
                  //
                  SmithingMenu* owner = nullptr; // 08
                  //
                  MEMBER_FN_PREFIX(SmithingConfirmCallback);
                  DEFINE_MEMBER_FN(Constructor, SmithingConfirmCallback&, 0x00856460, SmithingMenu*);
            };
            //
            struct UnkA8Entry { // sizeof == 0x20
               InventoryEntryData* item; // 00
               uint32_t unk04;
               BGSConstructibleObject* recipe; // 08 // if == nullptr, then cannot temper the item
               float    currentTempering = 1.0F; // 0C
               float    unk10 = 1.0F; // related to ExtraHealth // related to tempering potential
               float    unk14 = 0;
               float    unk18 = 0;
               bool     playerHasNeededItems;  // 1C // == this->unk08->PlayerHasNeededItems();
               bool     playerMeetsConditions; // 1D // == this->unk08->PlayerMeetsConditions();
               bool     unk1E = false; // possibly "is selected"
               uint8_t  pad1F;
               //
               MEMBER_FN_PREFIX(UnkA8Entry);
               DEFINE_MEMBER_FN(Constructor, UnkA8Entry&, 0x00856490, InventoryEntryData* item, BGSConstructibleObject* unk08);
            };
            struct UnkB4Entry { // sizeof == 0x08?
               uint32_t unk00;  // 00
            };
            //
            tArray<UnkA8Entry> improveableItems; // A8 // items in the player's inventory that can be improved
            BSTHashMap<uint32_t, BGSConstructibleObject*> recipes; // B4 // keys are COBJ created objects' form IDs
            uint32_t unkD4;
            uint32_t unkD8 =  0; // index unto unkA8
            int32_t  unkDC = -1; // index into unkA8: index of the item to temper?
            uint32_t smithingFormType; // E0
            InventoryEntryData* unkE4 = nullptr; // E4 // deep copy of an InventoryEntryData in unkA8
            //
            inline BGSConstructibleObject* get_cobj_for(uint32_t formID) const {
               return *this->recipes.table.lookup(formID);
            }
            BGSConstructibleObject* get_cobj_for(TESForm*) const; // based on vanilla; accounts for weapons' and armors' template forms
            //
            MEMBER_FN_PREFIX(SmithingMenu);
            DEFINE_MEMBER_FN(Constructor, SmithingMenu&, 0x008562B0, GFxValue*, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(Destructor, void, 0x008579B0);
            DEFINE_MEMBER_FN(RebuildAllLists,    void, 0x00857A60); // populates unkB4 and calls RebuildItemList
            DEFINE_MEMBER_FN(RebuildItemList,    void, 0x00857350); // scans the player's inventory. if unkB4.lookup(item->formID) exists, then the item is added to improveableItems.
            DEFINE_MEMBER_FN(TemperCurrentItem,  void, 0x00857520);
            DEFINE_MEMBER_FN(Subroutine008570D0, void, 0x008570D0, int32_t index); // resets unkE4 based on unkA8[index]
            DEFINE_MEMBER_FN(UpdateDisplayedRequirements, void, 0x00856F10); // accesses player inventory; updates the required materials list

            // We can potentially get the SmithingMenu to pull from arbitrary containers instead of (or 
            // in addition to) the player by:
            //
            //  - Patch BGSConstructibleObject::PlayerHasNeededItems to search additional containers for 
            //    needed crafting materials. We'll probably have to completely shim that function and 
            //    reimplement its logic, so that we can properly handle the case of the needed items 
            //    being spread out across multiple different containers.
            //
            //  - TemperCurrentItem removes all of a BGSConstructibleObject's required materials from 
            //    the player by way of a call to TESContainer::ForEach, passing a functor that calls 
            //    TESObjectREFR::Unk_56 on *g_thePlayer. We need to patch the call to use a functor 
            //    that we provide.
      };
      static_assert(sizeof(SmithingMenu) >= 0xE8, "RE::CraftingSubMenus::SmithingMenu is too small!");
      static_assert(sizeof(SmithingMenu) <= 0xE8, "RE::CraftingSubMenus::SmithingMenu is too large!");
   };
}