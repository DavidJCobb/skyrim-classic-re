#include "ReverseEngineered/Systems/DefaultObjects.h"

namespace RE {
   DefaultObjectList* DefaultObjectList::GetSingleton(void) {
      return (DefaultObjectList*)0x01245C60;
   }
   BGSDefaultObjectManager* BGSDefaultObjectManager::GetSingleton(void) {
      return (BGSDefaultObjectManager*)0x012E2500;
   }
   TESForm* DefaultObjectList::GetBySignature(UInt32 signature) const {
      auto index = this->GetIndexOfSignature(signature);
      if (index < 0)
         return nullptr;
      return GetDOBJByIndex(index);
   }
   SInt32 DefaultObjectList::GetIndexOfSignature(UInt32 signature) const {
      signature = _byteswap_ulong(signature);
      for (UInt32 i = 0; i < DefaultObjectList::count; i++) {
         if (this->objects[i].signature == signature)
            return i;
      };
      return -1;
   };
};