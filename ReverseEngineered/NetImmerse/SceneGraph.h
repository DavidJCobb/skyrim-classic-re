#pragma once
#include <cstdint>
#include "skse/Utilities.h"
#include "ReverseEngineered/NetImmerse/nodes.h"

namespace RE {
   class BSSceneGraph : public NiNode {
      public:
         //
         // TODO
         //

         MEMBER_FN_PREFIX(BSSceneGraph);
         DEFINE_MEMBER_FN(Constructor_B, SceneGraph&, 0x00B18070, const char* name, uint32_t, uint32_t);

         //
         // The constructor creates three objects of the form "%sRoot Node", "%sRoot CameraNode", and 
         // "%sRoot Camera", using the name provided to the constructor.
         //
   };

   class SceneGraph : public BSSceneGraph { // sizeof == 0xD8
      public:
         //
         // TODO
         //

         MEMBER_FN_PREFIX(SceneGraph);
         DEFINE_MEMBER_FN(Constructor_B, SceneGraph&, 0x0069F1F0, const char* name, uint32_t, uint32_t);
   };
}