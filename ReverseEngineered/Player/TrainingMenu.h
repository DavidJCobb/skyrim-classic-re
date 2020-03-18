#pragma once
#include "skse/GameMenus.h"

namespace RE {
   class Actor;

   class TrainingMenu : public IMenu, BSTEventSink<MenuOpenCloseEvent> { // sizeof == 0x98 on g_scaleformHeap
      public:
         static constexpr UInt32 vtbl = 0x010E8004;
         //
      public:
         // 1C: BSTEventSink<MenuOpenCloseEvent>
         Actor*   trainer = nullptr; // 20
         UInt32   skillToTrain; // 24 // actor value index
         GFxValue unk28;
         GFxValue unk38; // 38 // skill name and player base value
         GFxValue unk48;
         GFxValue unk58; // 58 // trainer's rank (e.g. Journeyman, Expert) based on the max training level in their TESClass
         GFxValue unk68; // 68 // times trained this level / times allowed to train per level
         GFxValue unk78; // 78 // cost to train
         GFxValue unk88; // 88 // player gold amount

         MEMBER_FN_PREFIX(TrainingMenu);
         DEFINE_MEMBER_FN(Accept, void, 0x00893E80); // when the "Accept" button is clicked; carries out training if possible; shows error notif if not
         DEFINE_MEMBER_FN(UpdateSkillProgressBar, void, 0x008938D0, bool animate, bool forceToFull); // if (!forceToFull) then we do (points / pointsMax)% based on PlayerSkills::GetSkillData
         DEFINE_MEMBER_FN(Subroutine00893540, void, 0x00893540); // updates text for times trained this level, trainer rank, player gold, etc.
   };
}
