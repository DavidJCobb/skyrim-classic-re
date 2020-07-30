#pragma once
#include <cstdint>
#include "TESForm.h"

namespace RE {// 30
   class TESFurniture;
   //
   class BGSConstructibleObject : public TESForm {
      //
      // This form is used to represent both crafting recipes (wherein materials are expended to 
      // create a new item) and tempering recipes (wherein materials are expended to improve an 
      // existing item). How does the game tell the two apart? Each recipe can specify the type 
      // of workbench that is allowed to use it (by way of a BGSKeyword*); in turn, different 
      // workbenches activate different menus, which in turn treat the recipes differently.
      //
      // For a tempering recipe, the (createdObject) must be the item to be improved.
      //
      public:
         static constexpr form_type_t form_type = form_type::constructible_object;
         //
         TESContainer requirements;  // 14 // materials that must be spent to use this recipe
         void*        conditions;    // 20 // linked list
         TESForm*     createdObject; // 24
         BGSKeyword*  workbenchKeyword; // 28
         uint16_t     quantity;      // 2C
         uint16_t     pad2E;         // 2E
         //
         MEMBER_FN_PREFIX(BGSConstructibleObject);
         DEFINE_MEMBER_FN(CanCraftAtWorkbench,   bool, 0x00491050, TESFurniture*, bool checkConditions);
         DEFINE_MEMBER_FN(PlayerHasNeededItems,  bool, 0x00491690); // accesses the player's inventory to see if they have all of the needed materials
         DEFINE_MEMBER_FN(PlayerMeetsConditions, bool, 0x004910A0);
   };
}