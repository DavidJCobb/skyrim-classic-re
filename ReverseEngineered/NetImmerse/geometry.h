#pragma once
#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/NetImmerse/shaders.h"
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

   class NiGeometry : public NiAVObject { // sizeof == 0xB8+
      public:
         virtual void Unk_33(void); // call controller vtbl+0xA0?
         virtual void Unk_34(void); // ret 0
         virtual void Unk_35(void); // same as Unk_33
         virtual void * Unk_36(void); // ret call m_spModelData vtbl+0x9C
         virtual void SetGeometryData(NiGeometryData * unk1); // set and AddRef geometry data
         virtual void * Unk_38(void); // ret call m_spModelData vtbl+0x94
         virtual UInt16 Unk_39(bool unk1); // ??

         NiPointer<NiProperty>       niProp;       // A8
         NiPointer<BSShaderProperty> bsShaderProp; // AC
         NiGeometryDataPtr m_spModelData;		// B0
         NiSkinInstancePtr m_spSkinInstance;		// B4

         // ... materials

         MEMBER_FN_PREFIX(NiGeometry);
         DEFINE_MEMBER_FN(SetBSShaderProperty, void, 0x004614A0, BSShaderProperty*);
   };
};