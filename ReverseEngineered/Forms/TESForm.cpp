#include "TESForm.h"

namespace RE {
   FormSignatureInfo* g_formSignatureInfo = (FormSignatureInfo*)0x0123F2C4; // array up to 0x87
   DEFINE_SUBROUTINE(const char*, FormTypeToSignatureString, 0x00678129, SInt32 formType);
   FormFactoryInfo**  g_formFactoryInfo   = (FormFactoryInfo**)0x012E57C0; // array up to 0x87; entries can be nullptr, indicating that a form type isn't actually loaded

   DEFINE_SUBROUTINE(::BGSKeywordForm*, GetKeywordListFor, 0x0044B2D0, ::TESForm*);
   DEFINE_SUBROUTINE(BGSEquipSlot*, GetEquipSlotFor, 0x0044AB00, TESForm*);
   DEFINE_SUBROUTINE(BGSEquipType*, GetEquipTypeFor, 0x0044AA10, TESForm*);
};