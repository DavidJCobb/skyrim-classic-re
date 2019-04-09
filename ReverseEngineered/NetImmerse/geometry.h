#pragma once
#include "ReverseEngineered/NetImmerse/objects.h"
#include "skse/NiGeometry.h"
#include "skse/Utilities.h"

namespace RE {
   class BSDismemberSkinInstance : public NiSkinInstance { // sizeof == 0x44
      public:
         struct Partition {
            UInt16 flags;
            UInt16 part; // body part index
         };

         UInt32     numPartitions; // 38
         Partition* partitions;    // 3C
         UInt8	unk40;    // 40 // all partitions' upper flags or'd together?
         UInt8	pad41[3]; // 41

         MEMBER_FN_PREFIX(BSDismemberSkinInstance);
         DEFINE_MEMBER_FN(Constructor, BSDismemberSkinInstance*, 0x00ABDBD0);
         DEFINE_MEMBER_FN(OverwriteFlagsFor, void, 0x0046AA60, UInt16 bodyPartIndex, UInt8 flags);
   };
};