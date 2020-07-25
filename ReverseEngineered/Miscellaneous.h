#pragma once
#include "Shared.h"
#include "skse/Utilities.h"
#include "skse/NiTypes.h"

class SimpleLock;
class TESObjectCELL;
class TESWorldSpace;
namespace RE {
   class NiNode;
   
   namespace native { // here for documentation purposes; don't bother actually using these
      DEFINE_SUBROUTINE_EXTERN(void*,   memmove,     0x00F52E30, void* destination, void* source, uint32_t size);
      DEFINE_SUBROUTINE_EXTERN(errno_t, memmove_s,   0x00F522BA, void* destination, size_t destSize, void* source, size_t sourceSize);
      DEFINE_SUBROUTINE_EXTERN(void*,   memset,      0x00F52240, void* destination, uint8_t value, uint32_t size);
      DEFINE_SUBROUTINE_EXTERN(size_t,  snprintf,    0x004202A0, char* buffer, size_t bufferSize, const char* format, ...); // unless it's sprintf_s
      DEFINE_SUBROUTINE_EXTERN(errno_t, strcat_s,    0x00F5372D, char* buffer, size_t bufferSize, const char* toAppend);
      DEFINE_SUBROUTINE_EXTERN(errno_t, strcpy_s,    0x00F52394, char* destination, rsize_t dest_size, const char* source);
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

   struct SimpleLock { // sizeof == 8
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

   struct BSRandomNumberGenerator { // sizeof > 0x9CC
      static BSRandomNumberGenerator* GetInstance() {
         return (BSRandomNumberGenerator*)0x012E4238;
      };
      static BSRandomNumberGenerator* GetOrCreate() { // is often inlined
         DEFINE_SUBROUTINE(BSRandomNumberGenerator*, f, 0x00432A90);
         return f();
         //
         // This is often inlined, so here's the implementation:
         //
         /*__declspec(naked) static BSRandomNumberGenerator* GetOrCreate() {
            _asm {
               mov  eax, 1;
               mov  ecx, 0x012E4C08;
               test byte ptr [ecx], eax;
               jnz  lExists;
               or   dword ptr [ecx], eax;
               mov  ecx, 0x012E4238;
               mov  eax, 0x00A4B6E0;
               call eax;
               push 0x0103D150;
               mov  eax, 0x00F520BC;
               call eax;
               add  esp, 4;
            lExists:
               mov  eax, 0x012E4238;
               retn;
            };
         };*/
      };

      MEMBER_FN_PREFIX(BSRandomNumberGenerator);
      DEFINE_MEMBER_FN(GetInt,             UInt32, 0x00A4B900, UInt32 maximum);
      DEFINE_MEMBER_FN(Subroutine00A4B6E0, void,   0x00A4B6E0); // This is only called by GetOrCreate, so if you see calls to it, then that's GetOrCreate inlined
   };

   // ExtractPath:
   //
   // This would typically be called like:
   //
   //    ExtractPath(buffer, sizeof(buffer), "data/meshes/whatever", "meshes/"); // "meshes/whatever" gets copied into buffer
   //
   // It's case-insensitive and honors both forward and backward slashes.
   //
   DEFINE_SUBROUTINE_EXTERN(const char*, BSExtractPath, 0x00A3F5C0, char* outPath, size_t outBufferSize, const char* inputPath, const char* prefixFolder);

   DEFINE_SUBROUTINE_EXTERN(bool,   IsCellOrWorldspace,  0x00472D10, void* thingToTest, ::TESObjectCELL** outCell, ::TESWorldSpace** outWorld);
   DEFINE_SUBROUTINE_EXTERN(void,   FillBufferWithByte,  0x00F52240, void* buffer, UInt8 byte, UInt32 length); // TODO: this is just memset, above, so rename it to memset in my disassembler and then remove it from here
   DEFINE_SUBROUTINE_EXTERN(const char*, GetActorValueName, 0x005AD370, UInt32 avIndex);
   DEFINE_SUBROUTINE_EXTERN(float,  GetRandomFloat,      0x00448A90, float minVal, float maxVal);
   DEFINE_SUBROUTINE_EXTERN(UInt32, GetRandomIntBetween, 0x0045CEF0, UInt32 minVal, UInt32 maxVal);
   DEFINE_SUBROUTINE_EXTERN(float, GetTrainingCost, 0x0059B570, UInt32 playerSkill);

   DEFINE_SUBROUTINE_EXTERN(void, TriggerScreenBlood, 0x0048A850, UInt32 papyrusAmountArg, float zeroA, UInt32 zeroB);

   struct LoadModelOptions {
      UInt32 unk00 = 3;
      UInt32 unk04 = 3;
      bool   unk08 = false;
      bool   isFaceGenHead = false; // 09
      bool   unk0A = false;
      bool   unk0B = true;
   };
   struct LoadedMeshResourcePtr; // see BSResource.h
   //
   // This attempts to load a NIF file, writes the root into the NiPointer argument, and returns 0 if it succeeds. It 
   // appears to load the file synchronously, i.e. there will be a hitch while the file loads. You should probably 
   // only run it from the main thread. It's used to handle actor ArmorAddons.
   //
   DEFINE_SUBROUTINE_EXTERN(UInt32, LoadModel,   0x00AF5820, const char* path, NiPointer<NiNode>& out, LoadModelOptions& options);
   DEFINE_SUBROUTINE_EXTERN(UInt32, LoadModel_B, 0x00AF5680, const char* path, LoadedMeshResourcePtr& out, LoadModelOptions& options); // possibly an asynchronous load
};