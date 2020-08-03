#include "geometry.h"

namespace RE {
   void NiGeometry::SetAllVertexColors(const NiColorA& color) {
      if (this->data.m_pObject)
         this->data->SetAllVertexColors(color);
   }
   void NiGeometryData::RecalcBounds() {
      auto v = this->vertices;
      if (!v || !this->vertexCount)
         return;
      CALL_MEMBER_FN(&this->bounds, Recalculate)(this->vertexCount, v);
   }
   void NiGeometryData::SetAllVertexColors(const NiColorA& color) {
      auto vc = this->vertexColors;
      if (!vc)
         return;
      uint16_t count = this->vertexCount;
      for (uint16_t i = 0; i < count; ++i)
         vc[i] = color;
   }
   //
   /*static*/ NiLines* NiLines::make(uint16_t vertexCount) {
      auto vertices     = (NiPoint3*)FormHeap_Allocate(vertexCount * sizeof(NiPoint3));
      auto vertexColors = (NiColorA*)FormHeap_Allocate(vertexCount * sizeof(NiColorA));
      memset(vertices,     0, vertexCount * sizeof(NiPoint3));
      memset(vertexColors, 0, vertexCount * sizeof(NiColorA));
      return NiLines::make(vertexCount, vertices, vertexColors, nullptr);
   }
   /*static*/ NiLines* NiLines::make(uint16_t vertexCount, NiPoint3* vertices, NiColorA* vertexColors, bool* optionalConnections) {
      auto instance = (NiLines*)FormHeap_Allocate(sizeof(NiLines));
      CALL_MEMBER_FN(instance, Constructor4)(vertexCount, vertices, vertexColors, optionalConnections);
      return instance;
   }

   void NiLinesData::SetConnections(std::initializer_list<bool> list) {
      if (!this->connections)
         return;
      uint32_t i = 0;
      for (auto it = list.begin(); it != list.end(); ++it) {
         if (i >= this->vertexCount)
            break;
         this->connections[i] = *it;
         ++i;
      }
      for (; i < this->vertexCount; ++i)
         this->connections[i] = false;
   }
}