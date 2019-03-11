#include "Shared.h"
#include "skse/GameAPI.h"

namespace RE {
   __declspec(naked) void _stdcall CallDispose(void*, bool) {
      __asm {
         mov  ecx, [esp + 0x8];
         push ecx;
         mov  ecx, [esp + 0x8];
         mov  eax, [ecx];
         mov  eax, [eax];
         call eax; // Arg1->Unk_00(Arg2);
         retn 8;
      };
   };
   void* HeapAllocate(UInt32 size) {
      void* memory = FormHeap_Allocate(size);
      memset(memory, 0, size);
      return memory;
   };
   void* HeapCreateByVtbl(UInt32 size, UInt32 vtbl) {
      void* memory = FormHeap_Allocate(size);
      memset(memory, 0, size);
      ((UInt32*)memory)[0] = vtbl;
      return memory;
   };
};