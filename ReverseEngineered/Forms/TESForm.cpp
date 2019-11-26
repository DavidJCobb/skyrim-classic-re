#include "TESForm.h"

namespace RE {
   DEFINE_SUBROUTINE(::BGSKeywordForm*, GetKeywordListFor, 0x0044B2D0, ::TESForm*);
   DEFINE_SUBROUTINE(BGSEquipSlot*, GetEquipSlotFor, 0x0044AB00, TESForm*);
   DEFINE_SUBROUTINE(BGSEquipType*, GetEquipTypeFor, 0x0044AA10, TESForm*);
};