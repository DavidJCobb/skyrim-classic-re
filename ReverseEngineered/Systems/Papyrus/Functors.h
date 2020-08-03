#pragma once
#include <cstdint>
#include "ReverseEngineered/Types.h"

class VMClassRegistry;
class VMValue;
namespace RE {
   namespace SkyrimScript {
      class DelayFunctor {
         public:
            static constexpr uint32_t vtbl = 0x012B64A4;
            virtual ~DelayFunctor();
            virtual VMValue& Execute(VMValue& result); // 01 // pure
            virtual bool     Unk_02(); // 02 // pure
            virtual bool     Unk_03();
            virtual bool     Unk_04(void*); // 04 // most likely serialization
            virtual uint8_t  Unk_05(); // 05 // pure // actually returns an integer
            virtual bool     Unk_06(void*, uint32_t); // 06 // maybe serialization?
            //
            uint32_t unk04;
            uint32_t stackID; // 08
      };
      //
      // Technically, the ones below are in an anonymous namespace inside of SkyrimScript.
      //
      class SetMotionTypeFunctor : public DelayFunctor { // sizeof == 0x1C
         public:
            static constexpr uint32_t vtbl = 0x010F629C;
            //
            ref_handle target;     // 0C
            uint32_t   motionType; // 10
            VMClassRegistry* registry; // 14 // must not be nullptr
            bool       allowActivate; // 18
            uint8_t    pad19[3];
            //
            MEMBER_FN_PREFIX(SetMotionTypeFunctor);
            DEFINE_MEMBER_FN(Constructor, SetMotionTypeFunctor&, 0x00904E30, const ref_handle&, uint32_t motionType, bool allowActivate, VMClassRegistry*, uint32_t stackID);
            //
            static SetMotionTypeFunctor* make(const ref_handle&, uint32_t motionType, bool allowActivate, VMClassRegistry*, uint32_t stackID);
      };
      static_assert(sizeof(SetMotionTypeFunctor) == 0x1C, "RE::SkyrimScript::SetMotionTypeFunctor is the wrong size!");
   }
}