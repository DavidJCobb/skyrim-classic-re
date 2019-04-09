#pragma once
#include "Shared.h"
#include "ReverseEngineered/Miscellaneous.h"

namespace RE {
   namespace native {
      DEFINE_SUBROUTINE(errno_t, strcpy_s, 0x00F52394, char *dest, rsize_t dest_size, const char *src);
   };

   DEFINE_SUBROUTINE(bool, IsCellOrWorldspace, 0x00472D10, void* thingToTest, ::TESObjectCELL** outCell, ::TESWorldSpace** outWorld);
   DEFINE_SUBROUTINE(void, FillBufferWithByte, 0x00F52240, void* buffer, UInt8 byte, UInt32 length);
};