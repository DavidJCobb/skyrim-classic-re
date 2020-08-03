#pragma once
#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/NetImmerse/shaders.h"
#include "ReverseEngineered/NetImmerse/types.h"
#include "ReverseEngineered/Shared.h"
#include "skse/NiGeometry.h"
#include "skse/Utilities.h"

class NiColorA;
namespace RE {
   class NiGeometryData;
   class NiLinesData;

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
         NiPointer<NiGeometryData>   data;         // B0
         NiSkinInstancePtr m_spSkinInstance;		// B4

         // ... materials

         MEMBER_FN_PREFIX(NiGeometry);
         DEFINE_MEMBER_FN(Constructor, NiGeometry&, 0x00AD94E0, NiGeometryData*);
         DEFINE_MEMBER_FN(SetBSShaderProperty, void, 0x004614A0, BSShaderProperty*);

         void SetAllVertexColors(const NiColorA&);
   };
   class NiTriBasedGeom : public NiGeometry {
      public:
   };

   class NiGeometryData : public NiObject { // sizeof == 0x48
      public:
         static constexpr uint32_t vtbl = 0x011188C4;
         //
         struct data_flag {
            data_flag() = delete;
            enum type : uint16_t {
               has_uvs = 0x0001,
               has_nbt = 0x0002,
            };
         };
         //
         uint16_t   vertexCount; // 08
         uint16_t   id;          // 0A
         uint16_t   flagsDirty = 0; // 0C
         uint16_t   flagsData  = 0; // 0E
         NiBound    bounds;         // 10
         NiPoint3*  vertices;       // 20 // instance owns this pointer
         NiPoint3*  normals;        // 24 // instance owns this pointer // all normals, then all binormals, etc.
         NiColorA*  vertexColors;   // 28 // instance owns this pointer
         NiPoint2*  vertexUVs;      // 2C // instance owns this pointer
         SimpleLock unk30;          // 30 // type not 100% verified; it could just be two zero dwords
         uint32_t   unk38 = 0;
         NiPointer<NiAdditionalGeometryData> additionalData = nullptr; // 3C
         uint32_t   unk40 = 0;
         uint8_t    flagsKeep       = 0;    // 44
         uint8_t    flagsCompress   = 0;    // 45
         bool       hasGeometryData = true; // 46
         uint8_t    pad47;
         //
         void RecalcBounds();
         void SetAllVertexColors(const NiColorA&);
         //
         MEMBER_FN_PREFIX(NiGeometryData);
         DEFINE_MEMBER_FN(Constructor,  NiGeometryData&, 0x00AC17C0); // default-constructs with vertex count 0 and nullptr arrays
         DEFINE_MEMBER_FN(Constructor7, NiGeometryData&, 0x00AC16D0, uint16_t vertexCount, NiPoint3* vertices, NiPoint3* normals, NiColorA* vertexColors, NiPoint2* vertexUVs, bool hasUVs, uint16_t dataFlags); // takes ownership of any pointers passed in
   };
   static_assert(offsetof(NiGeometryData, vertexCount) == 8, "RE::NiGeometryData::vertexCount is at the wrong offset!");

   class NiLines : public NiTriBasedGeom { // sizeof == 0xB8
      public:
         static constexpr uint32_t vtbl = 0x01118D9C;
         //
         static NiLines* make(uint16_t vertexCount);
         static NiLines* make(uint16_t vertexCount, NiPoint3* vertices, NiColorA* vertexColors, bool* optionalConnections);
         //
         MEMBER_FN_PREFIX(NiLines);
         DEFINE_MEMBER_FN(Constructor,  NiLines&, 0x00AC38E0);
         DEFINE_MEMBER_FN(Constructor1, NiLines&, 0x00AC38C0, NiLinesData*); // possibly copy-constructor
         DEFINE_MEMBER_FN(Constructor4, NiLines&, 0x00AC3A60, uint16_t vertexCount, NiPoint3* vertices, NiColorA* vertexColors, bool* optionalConnections);
   };
   static_assert(sizeof(NiLines) == 0xB8, "RE::NiLines is the wrong size!");
   class NiLinesData : public NiGeometryData { // sizeof == 0x4C
      public:
         bool* connections; // 48 // count == this->vertexCount // if this->connections[vertexIndex] == true, then the vertex is connected to the next one
         //
         void SetConnections(std::initializer_list<bool>);
         //
         MEMBER_FN_PREFIX(NiLinesData);
         DEFINE_MEMBER_FN(Constructor,  NiLinesData&, 0x00ADFF40);
         DEFINE_MEMBER_FN(Constructor4, NiLinesData&, 0x00AE0170, uint16_t vertexCount, NiPoint3* vertices, NiColorA* vertexColors, bool* optionalConnections); // takes ownership of any pointers passed in
         DEFINE_MEMBER_FN(SetConnections, void, 0x00ADFEF0, bool*); // if you pass in nullptr, default connections are generated forming a dotted line
   };
   static_assert(sizeof(NiLinesData) == 0x4C, "RE::NiLinesData is the wrong size!");
};