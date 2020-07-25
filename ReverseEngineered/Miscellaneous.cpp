#pragma once
#include "Shared.h"
#include "ReverseEngineered/Miscellaneous.h"

class NiNode;
namespace RE {
   namespace native {
      DEFINE_SUBROUTINE(void*,   memmove,     0x00F52E30, void* destination, void* source, uint32_t size);
      DEFINE_SUBROUTINE(void*,   memset,      0x00F52240, void* destination, uint8_t value, uint32_t size);
      DEFINE_SUBROUTINE(size_t,  snprintf,    0x004202A0, char* buffer, size_t bufferSize, const char* format, ...);
      DEFINE_SUBROUTINE(errno_t, strcat_s,    0x00F5372D, char* buffer, size_t bufferSize, const char* toAppend);
      DEFINE_SUBROUTINE(errno_t, strcpy_s,    0x00F52394, char* destination, rsize_t dest_size, const char* source);
      DEFINE_SUBROUTINE(int32_t, strncasecmp, 0x00F543EC, const char* a, const char* b, size_t limit);
      DEFINE_SUBROUTINE(char,    tolower,     0x00F52840, uint32_t glyph);
   };

   DEFINE_SUBROUTINE(const char*, BSExtractPath, 0x00A3F5C0, char* outPath, size_t outBufferSize, const char* inputPath, const char* prefixFolder);

   DEFINE_SUBROUTINE(bool,  IsCellOrWorldspace, 0x00472D10, void* thingToTest, ::TESObjectCELL** outCell, ::TESWorldSpace** outWorld);
   DEFINE_SUBROUTINE(void,  FillBufferWithByte, 0x00F52240, void* buffer, UInt8 byte, UInt32 length);
   DEFINE_SUBROUTINE(const char*, GetActorValueName, 0x005AD370, UInt32 avIndex);
   DEFINE_SUBROUTINE(float, GetRandomFloat,     0x00448A90, float minVal, float maxVal);

   DEFINE_SUBROUTINE(void, TriggerScreenBlood, 0x0048A850, UInt32 papyrusAmountArg, float zeroA, UInt32 zeroB);

   DEFINE_SUBROUTINE(UInt32, LoadModel, 0x0048A850, const char* path, NiPointer<NiNode>& out, LoadModelOptions& options);
};