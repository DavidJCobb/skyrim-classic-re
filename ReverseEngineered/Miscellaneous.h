#pragma once
#include "Shared.h"
#include "skse/Utilities.h"

class SimpleLock;
class TESObjectCELL;
class TESWorldSpace;
namespace RE {
   namespace native { // here for documentation purposes; don't bother actually using these
      DEFINE_SUBROUTINE_EXTERN(void*,   memmove,     0x00F52E30, void* destination, void* source, uint32_t size);
      DEFINE_SUBROUTINE_EXTERN(void*,   memset,      0x00F52240, void* destination, uint8_t value, uint32_t size);
      DEFINE_SUBROUTINE_EXTERN(errno_t, strcpy_s,    0x00F52394, char *dest, rsize_t dest_size, const char *src);
      DEFINE_SUBROUTINE_EXTERN(int32_t, strncasecmp, 0x00F543EC, const char* a, const char* b, size_t limit);
      DEFINE_SUBROUTINE_EXTERN(char,    tolower,     0x00F52840, uint32_t glyph);
   };

   struct RTTI {
      UInt32 unk04;
      UInt32 unk08;
      char* className;
      // ...?
      virtual void Unk_00(void);
      virtual void Unk_01(void);
      virtual void Unk_02(void);
      virtual void Unk_03(void);
   };

   template<typename T> struct BStList { // sizeof == 0x8
      T*       data; // 00
      BStList* next; // 04
      //
      MEMBER_FN_PREFIX(BStList);
      DEFINE_MEMBER_FN(Append,            void, 0x004EEAC0, T* item, void* customAllocatorFunc_orNullptr, void* customAllocatorFuncArg);
      DEFINE_MEMBER_FN(DestroyAllAfterMe, void, 0x0048CB30, void* customDeallocatorFunc_orNullptr, void* customDeallocatorFuncArg);
      DEFINE_MEMBER_FN(FindAndRemoveItem, void, 0x00456380, T* item, void* customDeallocatorFunc_orNullptr, void* customDeallocatorFuncArg);
      DEFINE_MEMBER_FN(InsertAfterMe,     void, 0x0042AFA0, T* item, void* customAllocatorFunc_orNullptr, void* customAllocatorFuncArg);
   };

   struct SimpleLock {
      operator ::SimpleLock*() const { return (::SimpleLock*) this; }

      UInt32 owningThreadID;     // thread that has ownership of the lock
      UInt32 numberOfLocksHeld;  // number of times the owning thread has locked this mechanism
      //
      // This member function checks if the lock is owned by the calling thread. If so, it immediately increments the lock count. 
      // Otherwise, it sleeps until the lock count hits zero, assigns ownership to the calling thread, and increments the count.
      //
      DEFINE_MEMBER_FN_LONG(SimpleLock, Lock, void, 0x00401710, const char* unusedDebugOutputIdentifyingMyCaller);
      //
      inline void Lock() { CALL_MEMBER_FN(this, Lock)(nullptr); };
      void Release() {
         //
         // Functions that increment the counter will always decrement it when they're done. 
         // This is pretty much how they do it.
         //
         // (Bethesda actually doesn't run the thread ID check: if they just incremented the 
         // counter, then they must have ownership of the lock. I've included the check just 
         // because when it comes to these things, I like a "safety first" approach.)
         //
         if (GetCurrentThreadId() == this->owningThreadID) {
            this->numberOfLocksHeld--;
            if (!this->numberOfLocksHeld) {
               InterlockedCompareExchange(&this->owningThreadID, 0, this->owningThreadID);
            }
         }
      };
   };

   DEFINE_SUBROUTINE_EXTERN(bool, IsCellOrWorldspace, 0x00472D10, void* thingToTest, ::TESObjectCELL** outCell, ::TESWorldSpace** outWorld);
   DEFINE_SUBROUTINE_EXTERN(void, FillBufferWithByte, 0x00F52240, void* buffer, UInt8 byte, UInt32 length);
};