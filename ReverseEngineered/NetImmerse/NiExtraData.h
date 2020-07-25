#pragma once
#include <cstdint>
#include "objects.h"
#include "skse/Utilities.h"

namespace RE {
   class NiExtraData : public NiObject {
      public:
         static constexpr uint32_t vtbl = 0x011189AC;
         //
         char* name; // 08
         //
         MEMBER_FN_PREFIX(NiExtraData);
         DEFINE_MEMBER_FN(Constructor, NiExtraData&, 0x00AC28C0);
         //
         static NiExtraData* make();
   };
   class BSXFlags : public NiExtraData {
      public:
         static constexpr uint32_t vtbl = 0x0111DB34;
         struct flag {
            flag() = delete;
            enum type : uint32_t {
               animated      = 0x00000001,
               havok         = 0x00000002,
               ragdoll       = 0x00000004,
               complex       = 0x00000008,
               addon         = 0x00000010,
               editor_marker = 0x00000020,
               dynamic       = 0x00000040,
               articulated   = 0x00000080,
               needs_transform_updates = 0x00000100,
               external_emit = 0x00000200,
            };
         };
         //
         uint32_t flags = 0; // 0C
         //
         MEMBER_FN_PREFIX(BSXFlags);
         DEFINE_MEMBER_FN(Constructor, BSXFlags&, 0x00AFD8B0);
         //
         static BSXFlags* make();
   };

   class BSFurnitureMarkerNode : public NiExtraData {
      public:
         enum MarkerEntry : UInt16 {
            None = 0,
            Front,
            Behind,
            Right,
            Left,
            Up
         };
         enum MarkerType : UInt16 {
            Unknown = 0, // Lean?
            Sit,
            Sleep
         };
         // 14 bytes:
         struct FurnitureMarker {
            NiPoint3   position;
            float      yaw; // rotation in radians.
            MarkerType type;
            UInt16     directions; // MarkerEntry values OR'd together.
         };
         tArray<FurnitureMarker> markers;
         //
         // ...?
         //
   };

   class BSInvMarker : public NiExtraData {
      public:
         float zoom;
         NiPoint3 rotation;
         //
         // ...?
         //
   };
};