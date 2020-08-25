#pragma once
#include <array>
#include "skse/Utilities.h"
#include "ReverseEngineered/Types.h"

class BSCullingProcess;
class BSShaderAccumulator;
class NiCamera;

namespace RE {
   class ShadowSceneNode;

   class Singleton01B2E998 { // sizeof >= 0x1B8
      public:
         static Singleton01B2E998* get() {
            return *(Singleton01B2E998**)0x01B2E998;
         }

         //
         // Possibly a singleton for managing UI-related 3D.
         //

         uint32_t unk00;
         BSCullingProcess* unk04;
         BSShaderAccumulator* unk08;
         NiCamera* unk0C;
         SimpleLock unk10;
         ShadowSceneNode* unk18; // yes, ShadowSceneNode exists as a singleton. this is a different instance from that one.
         std::array<NiNode*, 8> unk1C; // unk1C[2] is the parent node for Inventory3DManager rendered objects
         tArray<uint32_t> unk3C;
         uint32_t unk48;
         tArray<uint32_t> unk4C;
         tArray<uint32_t> unk58;
         uint32_t unk64[0xC];
         uint32_t unk94; // struct...
         // ...

         MEMBER_FN_PREFIX(Singleton01B2E998);
         DEFINE_MEMBER_FN(Constructor, Singleton01B2E998&, 0x00896B20);
         DEFINE_MEMBER_FN(Initialize, void, 0x00895D20); // called after construction; creates basically all pointed-to objects up to and including unk1C, and possibly more as well
         DEFINE_MEMBER_FN(AppendNode, void, 0x00895F90, NiNode* nodeToAppend, uint32_t whichNodeInUnk1CToAppendTo);
         DEFINE_MEMBER_FN(RemoveNode, void, 0x00895FE0, NiNode* nodeToRemove, uint32_t whichNodeInUnk1CToRemoveFrom);
   };
}