#include "ReverseEngineered/Forms/TESPackage.h"
#include "ReverseEngineered/Shared.h"
#include "skse/GameTypes.h"

namespace RE {
   SimpleLock* g_packageLock = (SimpleLock*)0x01B390A4;

   void TESPackage::IncRef() {
      InterlockedIncrement(&this->refcount);
   };
   void TESPackage::DecRef() {
      //
      // Recreated from commonly-inlined code in the executable.
      //
      auto count = InterlockedDecrement(&this->refcount);
      if (!count) {
         if (CALL_MEMBER_FN(this, GetPackageFlag00000800)()) {
            g_packageLock->Lock();
            //
            // Technically, the lock is acquired before modifying the smart 
            // pointer, and released after.
            //
            g_packageLock->Release();
            CallDispose(this, true);
         }
      }
   };
   void TESPackageData::DecRef() {
      //
      // Recreated from commonly-inlined code in the executable.
      //
      g_packageLock->Lock();
      //
      // Technically, the lock is acquired before modifying the smart 
      // pointer, and released after.
      //
      g_packageLock->Release();
      CallDispose(this, true);
   };

   bool TESPackage::IsExtraType() { // 005DEFA0 - 1.9.32
      switch (this->type) {
         case kPackageType_Activate:
         case kPackageType_Alarm:
         case kPackageType_Flee:
         case kPackageType_Trespass:
         case kPackageType_Spectator:
         case kPackageType_ReactToDead:
         case kPackageType_DoNothing:
         case kPackageType_InGameDialogue:
         case kPackageType_Surface:
         case kPackageType_AvoidPlayer:
         case kPackageType_ReactToDestroyedObject:
         case kPackageType_ReactToGrenadeOrMine:
         case kPackageType_StealWarning:
         case kPackageType_PickPocketWarning:
         case kPackageType_MovementBlocked:
         case kPackageType_VampireFeed:
         case kPackageType_CannibalFeed:
            return true;
      }
      return false;
   }
};