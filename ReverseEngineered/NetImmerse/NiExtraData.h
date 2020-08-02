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
         enum class marker_entry : uint16_t {
            none  = 0,
            front = 0x01,
            back  = 0x02,
            right = 0x04,
            left  = 0x08,
            up    = 0x10,
         };
         using direction_t = std::underlying_type_t<marker_entry>;
         enum class marker_type : uint16_t {
            lean = 0,
            sit,
            sleep,
         };
         //
         struct FurnitureMarker { // sizeof == 0x14
            NiPoint3    position;
            float       yaw; // rotation in radians.
            marker_type type;
            direction_t directions; // bitmask
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