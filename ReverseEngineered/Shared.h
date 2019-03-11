#pragma once

#define DEFINE_SUBROUTINE_EXTERN(retType, name, address, ...) extern retType(*name##)(__VA_ARGS__);
#define DEFINE_SUBROUTINE(retType, name, address, ...) retType(*name##)(__VA_ARGS__) = (retType(*)(__VA_ARGS__))address;
// Creates a function pointer to an assembly subroutine.
// Usage:
//  
// DEFINE_SUBROUTINE(void, UnknownSubroutineDEADBEEF, 0xDEADBEEF, UInt32, UInt32);
// UnknownSubroutineDEADBEEF(0, 1);
//
// Don't forget to prefix this with "static" if used in Shared.h and outside of a class. Note also 
// that other compilers will complain if you use this to define subroutines without arguments.

namespace RE {
   void _stdcall CallDispose(void*, bool);
   void* HeapAllocate(UInt32 size);
   void* HeapCreateByVtbl(UInt32 size, UInt32 vtbl);
};