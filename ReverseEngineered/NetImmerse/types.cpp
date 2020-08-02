#include "types.h"

namespace RE {
   /*static*/ NiMatrix33 NiMatrix33::ConstructFromX(float x) {
      //
      // [ 1,       0,      0,
      //   0,  cos(x), sin(x),
      //   0, -sin(x), cos(x)  ]
      //
      NiMatrix33 out;
      auto& d = out.data;
      float c = std::cos(x);
      float s = std::sin(x);
      d[0][0] = 1.0F;
      d[0][1] = 0.0F;
      d[0][2] = 0.0F;
      d[1][0] = 0.0F;
      d[1][1] = c;
      d[1][2] = s;
      d[2][0] = 0.0F;
      d[2][1] = -s;
      d[2][2] = c;
      return out;
   };
   /*static*/ NiMatrix33 NiMatrix33::ConstructFromY(float y) {
      //
      // [ cos(y), 0, -sin(y),
      //        0, 1,       0,
      //   sin(y), 0,  cos(y) ]
      //
      NiMatrix33 out;
      auto& d = out.data;
      float c = std::cos(y);
      float s = std::sin(y);
      d[0][0] = c;
      d[0][1] = 0.0F;
      d[0][2] = -s;
      d[1][0] = 0.0F;
      d[1][1] = 1.0F;
      d[1][2] = 0.0F;
      d[2][0] = s;
      d[2][1] = 0.0F;
      d[2][2] = c;
      return out;
   };
   /*static*/ NiMatrix33 NiMatrix33::ConstructFromZ(float z) {
      //
      // [ cos(z), sin(z), 0,
      //  -sin(z), cos(z), 0,
      //        0,      0, 1 ]
      //
      NiMatrix33 out;
      auto& d = out.data;
      float c = std::cos(z);
      float s = std::sin(z);
      d[0][0] = c;
      d[0][1] = s;
      d[0][2] = 0.0F;
      d[1][0] = -s;
      d[1][1] = c;
      d[1][2] = 0.0F;
      d[2][0] = 0.0F;
      d[2][1] = 0.0F;
      d[2][2] = 1.0F;
      return out;
   };
}