#pragma once
#include "skse/NiTypes.h"
#include "skse/Utilities.h"

namespace RE {
   struct NiPoint3 : public ::NiPoint3 {
      public:
         NiPoint3() {};
         NiPoint3(float X, float Y, float Z) {
            ::NiPoint3::NiPoint3(X, Y, Z);
         };
         NiPoint3(const ::NiPoint3& other) {
            *this = other;
         }
         //
         inline static NiPoint3* as(::NiPoint3& other) { return (NiPoint3*) &other; };
         //
         inline NiPoint3& operator=(const ::NiPoint3& other) {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            return *this;
         }
         //
         MEMBER_FN_PREFIX(NiPoint3);
         DEFINE_MEMBER_FN(Add,              NiPoint3*, 0x00420320, NiPoint3& out, const NiPoint3& other);
         DEFINE_MEMBER_FN(Add_B,            NiPoint3&, 0x00420470, const NiPoint3& other); // modifies and returns self
         DEFINE_MEMBER_FN(GetLength,        float,     0x0040A4B0);
         DEFINE_MEMBER_FN(GetLengthSquared, float,     0x004310E0); // if you're comparing the length to a constant, it's faster to compare the squared length to the squared constant
         DEFINE_MEMBER_FN(Multiply,         NiPoint3*, 0x004203A0, NiPoint3& out, const float other);
         DEFINE_MEMBER_FN(Negate,           NiPoint3*, 0x0045C680, NiPoint3& out);
         DEFINE_MEMBER_FN(NotEqualTo,       bool,      0x0040B7D0, const NiPoint3& other); // Bethesda operator!=
         DEFINE_MEMBER_FN(Subtract,         NiPoint3*, 0x00420360, NiPoint3& out, const NiPoint3& other);
         DEFINE_MEMBER_FN(VectorNormalize,  void,      0x00420500);
         DEFINE_MEMBER_FN(VectorNormalize2, float,     0x00431120); // returns old length

         NiPoint3& operator+(const NiPoint3& other) {
            return CALL_MEMBER_FN(this, Add_B)(other);
         };
   };
   class NiQuaternion {
      public:
         float w;
         float x;
         float y;
         float z;

         MEMBER_FN_PREFIX(NiQuaternion);
         DEFINE_MEMBER_FN(FromMatrix, void, 0x00AC0260, const NiMatrix33&);
         DEFINE_MEMBER_FN(ToMatrix, NiMatrix33&, 0x004719A0, NiMatrix33& out);
   };
   class NiMatrix33 : public ::NiMatrix33 { // sizeof == 0x24
      public:
         MEMBER_FN_PREFIX(NiMatrix33);
         DEFINE_MEMBER_FN(ConstructFromEuler, void,        0x00AAB810, float x, float y, float z);
         DEFINE_MEMBER_FN(ConstructFromPitch, void,        0x004A7E70, float);
         DEFINE_MEMBER_FN(ConstructFromYaw,   void,        0x0045C6C0, float);
         DEFINE_MEMBER_FN(ConvertPositionToThisFrame, NiPoint3*, 0x00474CD0, NiPoint3& out, const NiPoint3& in);
         DEFINE_MEMBER_FN(Multiply,           NiMatrix33*, 0x00447ED0, NiMatrix33& out, const NiMatrix33& other);

         inline NiMatrix33& operator=(const NiMatrix33& other) {
            memcpy(this, &other, sizeof(NiMatrix33));
         }
         inline NiMatrix33& operator=(const ::NiMatrix33& other) {
            memcpy(this, &other, sizeof(NiMatrix33));
         }

         static NiMatrix33 ConstructFromEuler(float x, float y, float z) {
            NiMatrix33 mX = ConstructFromX(x);
            NiMatrix33 mY = ConstructFromY(y);
            NiMatrix33 mZ = ConstructFromZ(z);
            NiMatrix33 scrap;
            CALL_MEMBER_FN(&mY, Multiply)(scrap, mZ);
            return *(CALL_MEMBER_FN(&mZ, Multiply)(mY, scrap));
         }
         static NiMatrix33 ConstructFromX(float x) {
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
         static NiMatrix33 ConstructFromY(float y) {
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
         static NiMatrix33 ConstructFromZ(float z) {
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

         //
         // Relative direction getters, given a matrix that represents a reference frame 
         // (e.g. an object rotation, camera rotation, trajectory, etc.):
         //
         NiPoint3 Forward() const {
            NiPoint3 dir;
            dir.x = data[0][1]; // Y+ == forward
            dir.y = data[1][1];
            dir.z = data[2][1];
            CALL_MEMBER_FN(RE::NiPoint3::as(dir), VectorNormalize)();
            return dir;
         };
         NiPoint3 Up() const {
            NiPoint3 dir;
            dir.x = data[0][2]; // Z+ == up
            dir.y = data[1][2];
            dir.z = data[2][2];
            CALL_MEMBER_FN(RE::NiPoint3::as(dir), VectorNormalize)();
            return dir;
         };
         //
         // could add getters for down, left, right, etc.; maybe later
         //
   };
}