#include "TESObjectCELL.h"
#include "ReverseEngineered\Systems\GameData.h"

namespace RE {
   bool TESObjectCELL::IsLoaded() {
      return CALL_MEMBER_FN(*g_TES, CheckCellLoaded)(this, 0);
   }
}