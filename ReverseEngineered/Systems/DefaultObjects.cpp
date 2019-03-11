#include "ReverseEngineered/Systems/DefaultObjects.h"

namespace RE {
   DefaultObjectList* DefaultObjectList::GetSingleton(void) {
      return (DefaultObjectList*)0x01245C60;
   };
   BGSDefaultObjectManager* BGSDefaultObjectManager::GetSingleton(void) {
      return (BGSDefaultObjectManager*)0x012E2500;
   };
   SInt32 DefaultObjectList::GetIndexOfSignature(UInt32 signature) {
      signature = _byteswap_ulong(signature);
      for (UInt32 i = 0; i < DefaultObjectList::kNumDefaultObjects; i++) {
         if (this->objects[i].signature == signature)
            return i;
      };
      return -1;
   };
};