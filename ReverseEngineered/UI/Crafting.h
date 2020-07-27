#pragma once
#include <cstdint>
#include "ReverseEngineered/UI/IMenu.h"
#include "ReverseEngineered/Systems/BSTEvent.h"
#include "ReverseEngineered/Types.h"

namespace RE {
   class InventoryEntryData;
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
            //
            BSTEventSink<TESFurnitureEvent> eventInterface; // 08 // interface; VTBL: 0x010E4410
            GFxMovieView*   view;  // 0C // initialized based on constructor Arg1; not nullptr; has a vtbl
            uint32_t        unk10; // constructor Arg2 // actor value
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
            DEFINE_MEMBER_FN(Constructor, CraftingSubMenu&, 0x008505F0, GFxValue*, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(SetDescription, void, 0x0084CC70, const char*);
            DEFINE_MEMBER_FN(UpdatePlayerSkillInfo, void, 0x0084C8C0, uint8_t actorValueIndex);
      };
      static_assert(sizeof(CraftingSubMenu) >= 0xA8, "RE::CraftingSubMenus::CraftingSubMenu is too small!");
      static_assert(sizeof(CraftingSubMenu) <= 0xA8, "RE::CraftingSubMenus::CraftingSubMenu is too large!");

      class AlchemyMenu : public CraftingSubMenu { // sizeof == 0x100, allocated on the Scaleform heap
         public:
            static constexpr uint32_t vtbl = 0x010E4688;
            //
            class CraftItemCallback { // sizeof == 0x10, allocated on game heap
               public:
                  static constexpr uint32_t vtbl = 0x010E46AC;
                  //
                  uint32_t unk04 = 0;
                  AlchemyMenu* unk08;
                  uint8_t  unk0C;
                  uint8_t  pad0D[3];
            };
            class QuitMenuCallback { // sizeof == 0xC, allocated on game heap
               public:
                  static constexpr uint32_t vtbl = 0x010E46B8;
                  //
                  uint32_t     unk04 = 0;
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
      static_assert(offsetof(AlchemyMenu, selections) >= 0xB4, "RE::CraftingSubMenus::AlchemyMenu::selections is too early!");
      static_assert(offsetof(AlchemyMenu, selections) <= 0xB4, "RE::CraftingSubMenus::AlchemyMenu::selections is too late!");
      static_assert(offsetof(AlchemyMenu, unkC8) >= 0xC8, "RE::CraftingSubMenus::AlchemyMenu::unkC8 is too early!");
      static_assert(offsetof(AlchemyMenu, unkC8) <= 0xC8, "RE::CraftingSubMenus::AlchemyMenu::unkC8 is too late!");

      class EnchantConstructMenu : public CraftingSubMenu { // sizeof == 0x158, allocated on the Scaleform heap
         public:
            static constexpr uint32_t vtbl = 0x010E4460;
            //
            uint32_t unkA8 = 0x0A;
            uint32_t unkAC = 0;
            uint32_t unkB0 = 5;
            uint32_t unkB4 = 0x30;
            uint32_t unkB8 = 0x40;
            tArray<uint32_t> unkBC;
            BSString unkC8; // 3D model path?
            GFxValue unkD0;
            GFxValue unkE0; // entryList?
            struct {
               uint32_t unk00 = 0;
               uint32_t unk04 = 0;
               tArray<uint32_t> unk08;
            } unkF0;
            int32_t  maxEnchantments; // 104 // max number of enchantments on a single item. the Mod Applied Enchantments Allowed perk entry point is checked once, in the constructor
            InventoryEntryData* unk108 = 0; // 108 // see SetItem3D
            uint32_t unk10C; // virtual
            uint16_t unk110 = 0;
            uint32_t unk114;
            uint32_t unk118;
            uint32_t unk11C;
            uint32_t unk120 = 0;
            uint32_t unk124 = 0;
            uint32_t unk128 = 0;
            uint32_t unk12C = 0;
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
      };

      class SmithingMenu : public CraftingSubMenu { // sizeof == 0xE8, allocated on the Scaleform heap
         public:
            static constexpr uint32_t vtbl = 0x010E4778;
            //
            struct UnkA8Entry { // sizeof == 0x20
               uint32_t unk00;
               uint32_t unk04;
               uint32_t unk08; // 08
               uint32_t unk0C;
               uint32_t unk10;
               float    unk14;
               float    unk18;
               uint8_t  unk1C;
               uint8_t  unk1D;
               uint8_t  pad1E[2];
            };
            //
            tArray<UnkA8Entry> unkA8;
            uint32_t unkB4;
            uint32_t unkB8;
            uint32_t unkBC = 0;
            uint32_t unkC0 = 0;
            uint32_t unkC4 = 0;
            void*    unkC8; // initialized to point to 0xDEADBEEF
            uint32_t unkCC;
            uint32_t unkD0 = 0;
            void*    unkD4; // TESObjectREFR*?
            uint32_t unkD8 = 0;
            int32_t  unkDC = -1; // int
            uint32_t smithingFormType; // E0
            uint32_t unkE4 = 0;
            //
            MEMBER_FN_PREFIX(SmithingMenu);
            DEFINE_MEMBER_FN(Constructor, SmithingMenu&, 0x008562B0, GFxValue*, uint32_t, uint32_t);
            DEFINE_MEMBER_FN(CraftItem, void, 0x00858E20);
            DEFINE_MEMBER_FN(TemperCurrentItem, void, 0x00857520);
            DEFINE_MEMBER_FN(AdvancePlayerSkill, void, 0x008516E0, float by); // inlined in some places including CraftItem
            DEFINE_MEMBER_FN(Subroutine00857350, void, 0x00857350);
            DEFINE_MEMBER_FN(Subroutine00857C60, void, 0x00857C60);
            DEFINE_MEMBER_FN(Subroutine00850C20, void, 0x00850C20); // accesses player inventory; is this what builds the list of items you can use/craft?
            DEFINE_MEMBER_FN(Subroutine00856F10, void, 0x00856F10); // accesses player inventory; has something to do with displaying a recipe's requirements
      };
   };
}