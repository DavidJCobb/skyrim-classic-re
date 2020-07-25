#include "ActorWeightData.h"
#include "ReverseEngineered/Forms/BaseForms/TESNPC.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/TESObjectARMA.h"
#include "ReverseEngineered/Forms/TESRace.h"
#include "ReverseEngineered/Player/PlayerCharacter.h"
#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/NetImmerse/nodes.h"
#include "ReverseEngineered/Systems/DefaultObjects.h"
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/INI.h"
#include "ReverseEngineered/Miscellaneous.h"
#include "skse/NiNodes.h"
#include "helpers/strings.h"

namespace {
   bool* _updateEquipmentBool = (bool*)0x012E5CA4; // bool is only set by ActorProcessManager::UpdateEquipment
   //
   inline bool _weird_compare(RE::ActorWeightData::BodyPart& bp) {
      return (void*)bp.item != (void*)bp.addon;
   }
}
namespace RE {
   class NiGeometryData;

   /*static*/ void ActorWeightData::makeArmorNodeName(std::string& out, TESNPC* wearer, TESForm* baseItem, TESObjectARMA* addon, float weight) {
      double f6 = weight * 100.0F;
      UInt32 f5 = baseItem ? baseItem->formID : 0; // eax
      UInt32 f2 = addon    ? addon->formID    : 0; // ebp
      auto   f4 = baseItem ? baseItem->GetEditorID() : ""; // at 0x00470571
      SInt32 f3 = wearer   ? CALL_MEMBER_FN(wearer, GetSex)() : -1;
      auto   f1 = addon    ? addon->GetEditorID() : ""; // at 0x00470587
      cobb::sprintf(out, "%s (%08X)[%d]/%s (%08X) [%2.0f%%]", f1, f2, f3, f4, f5, f6); // at 0x0047059C // this gets treated like a file path, but it blatantly isn't
   }
}