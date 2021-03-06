#include "ReverseEngineered/Forms/TESObjectREFR.h"

#include <algorithm>
#include <cstdlib>

#include "skse/GameRTTI.h"
#include "skse/Utilities.h"

#include "ReverseEngineered/Miscellaneous.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/BGSListForm.h"
#include "ReverseEngineered/Forms/TESObjectCELL.h"
#include "ReverseEngineered/Forms/TESWorldSpace.h"
#include "ReverseEngineered/NetImmerse/havok.h"
#include "ReverseEngineered/NetImmerse/nodes.h"
#include "ReverseEngineered/Systems/Savedata/BGSSaveLoadManager.h"
#include "ReverseEngineered/Systems/ChangeForms.h"
#include "ReverseEngineered/Systems/Inventory.h"
#include "ReverseEngineered/Systems/Savedata/BGSSaveLoadManager.h"

namespace ReleaseDebug {
   //
   // Namespace for debug code for things I literally wasn't able to test.
   //
   namespace TESObjectREFR {
      namespace GetActivateParents {
         static bool echoed = false;
         void error(std::exception& exception, RE::TESObjectREFR* reference) {
            if (echoed || reference == NULL)
               return;
            UInt32 formID = reference->formID;
            if (formID >> 0x18 != 0xFF) {
               echoed = true;
               _MESSAGE("CobbPos: An error occurred while getting a reference's activate-parents. ExtraMissingRefIds was handled improperly. If you happen to check this log, please tell the mod author about this error, and let them know what mod (in your load order) this reference ID belongs to.\n   Exception: %s\n   Reference: 0x%08X", exception.what(), formID);
            }
         };
      };
   };
};

namespace RE {
   namespace Functors {
      //
      // These functors aren't reverse-engineered, but rather exist for methods we've defined on 
      // reverse-engineered classes.
      //
      namespace TESObjectREFR {
         namespace GetMotionType {
            struct State {
               SInt8 result = 0;
            };
            void Iterator(RE::bhkCollisionObject* collision, State* parameters) {
               auto obj = ((hkpRigidBody*)collision->rigidBody->asHkpWorldObject());
               if (!obj)
                  return;
               UInt8 current = obj->unk0E0.motionSystem;
               if (parameters->result == 0) {
                  parameters->result = current;
               } else if (parameters->result != current) {
                  parameters->result = -1;
               }
            };
         }
         namespace HasCollision {
            struct State {
               bool result = false;
            };
            void Iterator(RE::bhkCollisionObject* collision, State* parameters) {
               if (collision)
                  parameters->result = true;
            };
         }
         namespace SetMotionType {
            struct State {
               explicit State(UInt32 a, bool b) : motionType(a), simulate(b) {};
               //
               UInt8 motionType = 0;
               bool  simulate = false;
            };
            void Iterator(RE::bhkCollisionObject* collision, State* parameters) {
               collision->SetMotionSystem(parameters->motionType, nullptr, parameters->simulate);
            };
         }
      }
   };

   // -----------------------------------------------------------------------------------------------------------------------------------

   DEFINE_SUBROUTINE(BGSDestructibleObjectForm*, GetBGSDestructibleObjectFormForRef, 0x00448090, TESObjectREFR*);
   DEFINE_SUBROUTINE(void, Update3DBasedOnHarvestedFlag, 0x00455BD0, TESObjectREFR*, NiNode* root); // finds and updates the NiSwitchNode in the model
   
   extern tList<TESObjectREFR*>* const g_referencesPendingDisable  = (tList<TESObjectREFR*>*)0x01B10C80;
   extern tList<TESObjectREFR*>* const g_referencesPendingEnable   = (tList<TESObjectREFR*>*)0x01B10C88;
   extern tList<TESObjectREFR*>* const g_referencesPendingDeletion = (tList<TESObjectREFR*>*)0x01B10C90;

   // -----------------------------------------------------------------------------------------------------------------------------------

   #pragma region Ref handle system
   bool RefHandleSystem::is_handle_valid(const ref_handle& handle) {
      auto esi = handle.index();
      auto ecx = RefHandleSystem::GetEntries()[esi].refHandle;
      if (!ecx.is_in_use())
         return false;
      if (ecx ^ (uint32_t)handle & ref_handle::mask_reuse)
         return false;
      auto ref = RefHandleSystem::GetEntries()[esi].refObject;
      return ref->GetRefHandle() == handle.index();

   }
   bool RefHandleSystem::ExchangeHandleForRef(ref_handle* refHandlePtr, refr_ptr& out) {
      auto handle = *refHandlePtr;
      if (handle) {
         auto ebx = handle.reuse_bits();
         auto esi = handle.index();
         TESObjectREFR* ref = nullptr;
         {
            auto eax = RefHandleSystem::GetEntries()[esi].refObject;
            if (eax)
               ref = eax->GetReference();
         }
         out = ref;
         //
         // Validate the passed-in handle:
         //
         auto storedHandle = RefHandleSystem::GetEntries()[esi].refHandle;
         if (storedHandle.is_in_use()) {
            if (storedHandle.reuse_bits() == ebx) {
               if (out->handleRefObject.GetRefHandle() == esi)
                  //
                  // Handle is valid.
                  //
                  return true;
            }
         }
         *refHandlePtr = 0;
      }
      out = nullptr;
      return false;
      //
      // Technically, all branches that return do it as: return (out != nullptr).
   };
   bool RefHandleSystem::GetRefByHandle(ref_handle* refHandlePtr, refr_ptr& out) {
      auto handle = *refHandlePtr;
      if (handle) {
         auto ebx = handle.reuse_bits();
         auto esi = handle.index();
         TESObjectREFR* ref = nullptr;
         {
            auto eax = RefHandleSystem::GetEntries()[esi].refObject;
            if (eax)
               ref = eax->GetReference();
         }
         out = ref;
         //
         // Validate the passed-in handle:
         //
         auto storedHandle = RefHandleSystem::GetEntries()[esi].refHandle;
         if (storedHandle.is_in_use()) {
            if (storedHandle.reuse_bits() == ebx) {
               if (out->handleRefObject.GetRefHandle() == esi)
                  //
                  // Handle is valid.
                  //
                  return true;
            }
         }
      }
      out = nullptr; // inlined
      return false;
      //
      // Technically, all branches that return do it as: return (out != nullptr).
   };
   ref_handle* RefHandleSystem::CreateRefHandleByREFR(ref_handle* refHandlePtr, TESObjectREFR* ref) {
      SInt32* const unk = (SInt32*)0x0131050C;
      //
      *refHandlePtr = 0;
      if (!ref)
         return refHandlePtr;
      auto eax = ref->handleRefObject.m_uiRefCount;
      if (eax & 0x400) {
         auto index = eax >> 0xB;
         auto edx = RefHandleSystem::GetEntries()[index].refHandle;
         *refHandlePtr = edx.reuse_bits() | index;
         return refHandlePtr;
      }
      if (*nextIndex == -1) {
         *refHandlePtr = 0;
         return refHandlePtr;
      }
      {
         RE::UnknownLock01::guard scopedLock(RefHandleSystem::GetInstance()->lock);
         //
         auto edx = &(RefHandleSystem::GetEntries()[*nextIndex]);
         ref_handle eax = edx->refHandle;
         eax.increment_reuse_count();
         eax.set_in_use();
         //
         // eax = that same handle, but with its validation value incremented by one.
         //
         edx->refHandle = eax;
         //
         auto ecx = *nextIndex | eax.reuse_bits();
         *refHandlePtr = ecx;
         //
         {  // This is actually handled by a smart pointer method, but I'm too lazy to define the smart pointer struct given that it's literally only used here
            //
            // TODO: The x86 code checks whether either edx or edx->refObject is a valid pointer; 
            // should take another look at it at some point and replicate things more exactly here.
            //
            BSHandleRefObject* p = &ref->handleRefObject;
            edx->refObject->DecRefHandle();
            p->IncRef();
            edx->refObject = p;
         }
         ref->handleRefObject.m_uiRefCount = ref->handleRefObject.GetRefCount() | (*nextIndex << 0xB) | 0x400;
         //
         auto index = RefHandleSystem::GetEntries()[*nextIndex].refHandle.index();
         if (index == *nextIndex) {
            *nextIndex = -1;
            *lastIndex = -1;
         }
         *nextIndex = index;
      }
   };
   //
   void RefHandleSystem::Init() {
      memset(RefHandleSystem::GetEntries(), 0, 0x00800000);
      UInt32 i = 0;
      do {
         auto entry = &RefHandleSystem::GetEntries()[i];
         entry->refHandle.set_index(i); // inlined: entry->refHandle ^= ((entry->refHandle ^ i) & kMask_Index); // (a & ~mask) | (b & mask) === a ^ ((a ^ b) & mask)
      } while (++i <= kMask_Index);
      *lastIndex = -1;
   };
   void RefHandleSystem::ReleaseHandle(const ref_handle& refHandlePtr) {
      auto handle = refHandlePtr;
      if (!handle)
         return;
      {
         RE::UnknownLock01::guard scopedLock(RefHandleSystem::GetInstance()->lock);
         auto  list  = RefHandleSystem::GetEntries();
         auto  index = handle.index();
         auto& entry = list[index];
         if (!entry.refHandle.is_in_use())
            return;
         if ((entry.refHandle ^ handle) & kMask_ReuseCount)
            return;
         entry.refHandle.set_unused();
         auto refObj = entry.refObject;
         if (refObj) {
            refObj->m_uiRefCount &= 0x3FF; // clear handle and flags
            refObj->DecRefHandle();
            entry.refObject = nullptr;
         }
         //
         if (*lastIndex == -1) {
            *nextIndex = index;
         } else {
            list[*lastIndex].refHandle.set_index(index); // inlined
         }
         entry.refHandle.set_index(index); // inlined
         *lastIndex = index;
      }
   };
   void RefHandleSystem::ClearActiveHandles() {
      RE::UnknownLock01::guard scopedLock(RefHandleSystem::GetInstance()->lock);
      //
      auto   list = RefHandleSystem::GetEntries();
      UInt32 i    = 0;
      do {
         auto& current = list[i];
         if (!current.refHandle.is_in_use())
            continue;
         auto refObj = current.refObject;
         if (refObj) {
            refObj->m_uiRefCount &= 0x3FF; // clear handle and flags
            refObj->DecRefHandle();
         }
         current.refObject = nullptr;
         current.refHandle.set_unused();
         //
         if (*lastIndex == -1) {
            *nextIndex = i;
         } else {
            list[*lastIndex].refHandle.set_index(i); // inlined
         }
         current.refHandle.set_index(i); // inlined
         *lastIndex = i;
      } while (++i <= kMask_Index);
   };
   void RefHandleSystem::ReleaseAndLoseHandle(ref_handle& refHandlePtr) {
      auto handle = refHandlePtr;
      if (!handle)
         return;
      {
         RE::UnknownLock01::guard scopedLock(RefHandleSystem::GetInstance()->lock);
         auto  list  = RefHandleSystem::GetEntries();
         auto  index = handle.index();
         auto& entry = list[index];
         if (entry.refHandle.is_in_use()) {
            auto eax = entry.refHandle ^ refHandlePtr;
            if (eax) {
               //
               // Handles have different flags or validation (reuse) counts.
               //
               auto refObj = entry.refObject;
               if (refObj) {
                  refObj->m_uiRefCount &= 0x3FF; // clear handle and flags
                  refObj->DecRefHandle();
                  entry.refObject = nullptr;
               }
               entry.refHandle.set_unused();
               //
               if (*lastIndex == -1) {
                  *nextIndex = index;
               } else {
                  list[*lastIndex].refHandle.set_index(index); // inlined
               }
               entry.refHandle.set_index(index); // inlined
               *lastIndex = index;
            }
         }
         refHandlePtr = 0;
      }
   };
   #pragma endregion

   // -----------------------------------------------------------------------------------------------------------------------------------

   void TESObjectREFR::CreateDetectionEvent(Actor* owner, SInt32 soundLevel) {
      struct ProcessManager {
         DEFINE_MEMBER_FN_LONG(ProcessManager, Subroutine00702070, void, 0x00702070, Actor*, NiPoint3* position, SInt32 soundLevel, TESObjectREFR*);
      };
      if (!owner)
         return;
      ActorProcessManager* manager = owner->processManager;
      if (!manager)
         return;
      CALL_MEMBER_FN((ProcessManager*)manager, Subroutine00702070)(owner, &this->pos, soundLevel, this);
   };
   std::vector<::TESObjectREFR*> TESObjectREFR::GetActivateParents(::TESForm* baseFormFilter) {
      std::vector<::TESObjectREFR*> result;
      {  // Get references that are listed by handle.
         ExtraActivateRef* extraActivate = (ExtraActivateRef*) this->extraData.GetByType(0x52);
         if (!extraActivate) {
            return result;
         }
         ExtraActivateRef::Entry* current = &(extraActivate->firstEntry);
         while (current != nullptr) {
            if (current->refHandle == nullptr) {
               current = current->nextEntry;
               continue;
            }
            UInt32 handle = *(current->refHandle);
            if (handle && handle != *g_invalidRefHandle) {
               refr_ptr ref;
               ref.set_from_handle(handle);
               if (ref)
                  if (baseFormFilter == nullptr || ref->baseForm == (RE::TESForm*) baseFormFilter)
                     result.push_back(ref.get_base());
            }
            current = current->nextEntry;
         }
      }
      {  // Get references that are listed by ID instead of handle.
         //
         // NOTE: I wasn't able to actually find or construct a test-case for this pre-release. We basically do all data 
         // handling in try-catch blocks; if we error on a non-created reference and we have never echoed an error before, 
         // we echo that error; otherwise, we're silent. This DOES mean that we'll have a perf hit on the untested code.
         //
         ExtraMissingRefIds* extraMissing = (ExtraMissingRefIds*) this->extraData.GetByType(0xB3);
         if (!extraMissing)
            return result;
         UInt32 count = extraMissing->count;
         if (count <= 1) {
            try {
               ::TESObjectREFR* reference = (::TESObjectREFR*) DYNAMIC_CAST(LookupFormByID(extraMissing->first.formID), TESForm, TESObjectREFR);
               if (reference)
                  if (baseFormFilter == nullptr || reference->baseForm == baseFormFilter)
                     result.push_back(reference);
            } catch (std::exception& e) {
               ReleaseDebug::TESObjectREFR::GetActivateParents::error(e, this);
            };
         } else {
            try {
               ExtraMissingRefIds::Entry* current = &(extraMissing->first);
               for (UInt32 i = 0; i < count; i++) {
                  ::TESObjectREFR* reference = (::TESObjectREFR*) DYNAMIC_CAST(LookupFormByID(current->formID), TESForm, TESObjectREFR);
                  if (reference)
                     if (baseFormFilter == nullptr || reference->baseForm == baseFormFilter)
                        result.push_back(reference);
                  current = current->next;
               }
            } catch (std::exception& e) {
               ReleaseDebug::TESObjectREFR::GetActivateParents::error(e, this);
            };
         }
      }
      {  // Remove duplicates.
         std::sort(result.begin(), result.end());
         auto last = std::unique(result.begin(), result.end());
         result.erase(last, result.end());
      }
      return result;
   };
   UInt32 __declspec(noinline) TESObjectREFR::GetChangeFlags() { // TODO: This should probably be moved to TESForm*
      auto* man = RE::BGSSaveLoadManager::GetSingleton();
      if (!man || !man->unk3E8)
         return 0;
      UInt32 changeFlags;
      CALL_MEMBER_FN(man->unk3E8, GetChangeFlags)(&changeFlags, this->formID);
      return changeFlags;
   };
   SInt32 TESObjectREFR::GetCurrentDestructionStage() {
      DEFINE_SUBROUTINE(SInt32, Subroutine00448260, 0x00448260, TESObjectREFR*);
      return Subroutine00448260(this);
   };
   TESObjectREFR* TESObjectREFR::GetDestinationDoor() {
      RE::refr_ptr otherDoor;
      this->GetDestinationDoor(otherDoor);
      return (TESObjectREFR*) otherDoor.abandon();
   };
   void TESObjectREFR::GetDestinationDoor(refr_ptr& out) {
      out = nullptr;
      if (!this->extraData.HasType(kExtraData_Teleport))
         return;
      RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(this->extraData)), GetExtraTeleportData)();
      if (!data)
         return;
      out.set_from_handle(data->refHandle);
   };
   bool TESObjectREFR::GetEditorCoordinateData(NiPoint3* pos, NiPoint3* rot, ::TESWorldSpace** worldspace, ::TESObjectCELL** cell) {
      void* couldBeAnything = nullptr;
      if (!this->GetEditorCoordinates(pos, rot, &couldBeAnything, 0))
         return false;
      IsCellOrWorldspace(couldBeAnything, cell, worldspace);
      if (*worldspace)
         *cell = (::TESObjectCELL*) CALL_MEMBER_FN((RE::TESWorldSpace*)worldspace, GetCellThatContainsPoint)(pos);
      return true;
   };
   void TESObjectREFR::GetEditorCoordinateDataAlways(NiPoint3* pos, NiPoint3* rot, ::TESWorldSpace** worldspace, ::TESObjectCELL** cell) {
      void* couldBeAnything = nullptr;
      if (!this->GetEditorCoordinates(pos, rot, &couldBeAnything, 0)) {
         *pos = this->pos;
         *rot = this->rot;
         *cell = (::TESObjectCELL*) this->parentCell;
         //*worldspace = NULL;
         //if (this->parentCell != NULL)
         //   *worldspace = (::TESWorldSpace*) this->parentCell->parentWorld;
         *worldspace = (::TESWorldSpace*) CALL_MEMBER_FN(this, GetWorldspace)();
         return;
      }
      IsCellOrWorldspace(couldBeAnything, cell, worldspace);
      if (*worldspace)
         *cell = (::TESObjectCELL*) CALL_MEMBER_FN((RE::TESWorldSpace*)worldspace, GetCellThatContainsPoint)(pos);
      return;

   };
   UInt32 TESObjectREFR::GetItemCount(TESForm* item) {
      if (!item)
         return 0;
      if (!CALL_MEMBER_FN(this, GetBaseContainerData)()) // if this reference is not a container
         return 0;
      auto data = GetExtraContainerChangesData(this);
      if (!data)
         return 0;
      if (item->IsInventoryItemBase()) // virtual method 0x27
         return std::abs(CALL_MEMBER_FN(data, GetItemCount)(item));
      if (item->formType == form_type::formlist) {
         return ((RE::BGSListForm*)item)->CountMatchingItemsInInventory(*data);
      }
      auto cast = item->GetAsObjectReference();
      if (cast) {
         ref_handle my_handle;
         ref_handle check;
         this->GetOrCreateRefHandle(my_handle);
         CALL_MEMBER_FN((RE::BaseExtraList*)&cast->extraData, GetExtraReferenceHandle)(check);
         return my_handle == check ? 1 : 0;
      }
      if (item->formType == form_type::keyword)
         return CALL_MEMBER_FN(data, CountItemsWithKeyword)((BGSKeyword*)item);
      //
      // (item) is not a valid form for this query.
      //
      return 0;
   }
   UInt32 TESObjectREFR::GetItemCountFast(TESForm* item) {
      if (!item)
         return 0;
      auto data = GetExtraContainerChangesData(this);
      if (!data)
         return 0;
      return std::abs(CALL_MEMBER_FN(data, GetItemCount)(item));
   }
   TESKey* TESObjectREFR::GetKey() {
      DEFINE_SUBROUTINE(TESKey*, PapyrusGetKey, 0x00902730, UInt32 dummy1, UInt32 dummy2, TESObjectREFR*);
      return PapyrusGetKey(0, 0, this);
   };
   SInt32 TESObjectREFR::GetLockLevel() {
      RE::ExtraLock* lock = (RE::ExtraLock*) CALL_MEMBER_FN(this, GetExtraLock)();
      if (!lock)
         return 0;
      return CALL_MEMBER_FN(lock, Subroutine00422390)(this);
   };
   float  TESObjectREFR::GetMass() {
      DEFINE_SUBROUTINE(float, GetMassOfNode, 0x00588C40, NiNode*);
      return GetMassOfNode(this->GetNiNode());
   };
   SInt8 __declspec(noinline) TESObjectREFR::GetMotionType() {
      //
      //   -1: internally inconsistent motion types
      //    0: no collision or no loaded 3D
      // else: valid motion type; refer to official docs for Papyrus ObjectReference.SetMotionType.
      //
      NiNode* node = this->GetNiNode();
      if (!node)
         return 0;
      node->IncRef();
      Functors::TESObjectREFR::GetMotionType::State parameters;
      IterateOverBhkCollisionObjects(node, &parameters, (BhkCollisionIteratorFunction*)&Functors::TESObjectREFR::GetMotionType::Iterator);
      node->DecRef();
      return parameters.result;
   };
   UInt32 TESObjectREFR::GetOpenState() {
      DEFINE_SUBROUTINE(UInt32, GetOpenState, 0x0044BE80, TESObjectREFR*);
      return GetOpenState(this);
   };
   bool TESObjectREFR::HasActivateChild(TESObjectREFR* child) {
      if (!child)
         return false;
      DEFINE_SUBROUTINE(bool, CheckActivateParenthood, 0x0090C920, UInt32 parentFormID, TESObjectREFR* child);
      return CheckActivateParenthood(this->formID, child);
   };
   bool __declspec(noinline)  TESObjectREFR::HasCollision() {
      NiNode* node = this->GetNiNode();
      if (!node)
         return false;
      node->IncRef();
      Functors::TESObjectREFR::HasCollision::State parameters;
      IterateOverBhkCollisionObjects(node, &parameters, (BhkCollisionIteratorFunction*)&Functors::TESObjectREFR::HasCollision::Iterator);
      node->DecRef();
      return parameters.result;
   };
   bool TESObjectREFR::IsActivationBlocked() {
      return CALL_MEMBER_FN((RE::BaseExtraList*)(&this->extraData), TestExtraFlags)(1);
   };
   bool TESObjectREFR::IsTeleportMarkerInAttachedCell(refr_ptr& destination) {
      if (!this->extraData.HasType(kExtraData_Teleport))
         return false;
      NiPoint3 pos;
      TESObjectCELL* targetCell = nullptr;
      {  // Get cell and world.
         {  // Get marker position and destination door.
            UInt32 handle = 0;
            {
               RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(this->extraData)), GetExtraTeleportData)();
               if (!data)
                  return false;
               pos = data->markerPosition;
               handle = data->refHandle;
            }
            if (!destination) {
               destination.set_from_handle(&handle);
               if (!destination)
                  return false;
            }
         }
         targetCell = destination->GetParentOrPersistentCell();
         if (!targetCell)
            return false;
         {  // Get marker's probable parent cell.
            TESWorldSpace* worldspace = targetCell->parentWorld;
            if (worldspace)
               targetCell = CALL_MEMBER_FN(worldspace, GetCellThatContainsPoint)(&pos);
         }
      }
      if (targetCell)
         return (targetCell->unk30 == 0x07);
      return false;
   };
   bool TESObjectREFR::IsDestinationTeleportMarkerInAttachedCell() {
      if (!this->extraData.HasType(kExtraData_Teleport))
         return false;
      NiPoint3 pos;
      TESObjectCELL* targetCell = this->GetParentOrPersistentCell();
      if (!targetCell)
         return false;
      TESWorldSpace* worldspace = targetCell->parentWorld;
      if (!worldspace)
         return (targetCell->unk30 == 0x07);
      {  // Get position.
         refr_ptr otherDoor;
         this->GetDestinationDoor(otherDoor);
         if (!otherDoor)
            return false;
         RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(otherDoor->extraData)), GetExtraTeleportData)();
         if (!data)
            return false;
         pos = data->markerPosition;
      }
      if (worldspace)
         targetCell = CALL_MEMBER_FN(worldspace, GetCellThatContainsPoint)(&pos);
      if (targetCell)
         return (targetCell->unk30 == 0x07);
      return false;
   };
   bool TESObjectREFR::IsValidLoadDoor(bool quick) {
      if (!this->extraData.HasType(kExtraData_Teleport))
         return false;
      RE::ExtraTeleport::TeleportData* data = CALL_MEMBER_FN(((RE::BaseExtraList*)&(this->extraData)), GetExtraTeleportData)();
      if (!data)
         return false;
      if (!quick) {  // Make sure the destination door is valid.
         refr_ptr otherDoor;
         otherDoor.set_from_handle(data->refHandle);
         if (!otherDoor)
            return false;
         RE::ExtraTeleport::TeleportData* otherData = CALL_MEMBER_FN(((RE::BaseExtraList*)&(otherDoor->extraData)), GetExtraTeleportData)();
         if (!otherData)
            return false;
      }
      return true;
   };
   void TESObjectREFR::KnockAreaEffect(float magnitude, float radius) {
      CALL_MEMBER_FN(RE::BGSImpactManager::GetInstance(), KnockAreaEffect)(this, magnitude, radius);
   };
   void TESObjectREFR::MoveTo(UInt32* pTargetHandle, void* parentCell, void* worldSpace, NiPoint3* position, NiPoint3* rotation) {
      MoveRefrToPosition((::TESObjectREFR*) this, pTargetHandle, parentCell, worldSpace, position, rotation);
      //
      // MoveRefrToPosition will not modify an object's rotation if the object is being moved to an unloaded 
      // cell. We'll do it forcibly. Note that this has not been tested on actors.
      //
      // Note that MoveRefrToPosition WILL break a moved object's collision if it's not run at an appropriate 
      // time. In general, it is only safe to run whenever BSTaskPool is processing stuff.
      //
      CALL_MEMBER_FN(this, SetRotation)(rotation);
   };
   bool TESObjectREFR::MoveToMyEditorLocation(bool native) {
      if (native) {
         //
         // WARNING: This has been observed to cause issues with some door models: the collision 
         // gets left behind, and only the reference and its display model get moved. This may 
         // be related to the MotionType/Move bug.
         //
         return CALL_MEMBER_FN(this, MoveToMyEditorLocation)(NULL, 0);
      } else {
         {  // Carry out the move.
            NiPoint3 pos;
            NiPoint3 rot;
            ::TESWorldSpace* worldspace;
            ::TESObjectCELL* targetCell;
            if (!this->GetEditorCoordinateData(&pos, &rot, &worldspace, &targetCell))
               return false;
            UInt32 nullHandle = *g_invalidRefHandle;
            MoveRefrToPosition((::TESObjectREFR*)this, &nullHandle, parentCell, worldspace, &pos, &rot);
         }
         {  // Clear changeflags.
            this->UnmarkChanged(2); // MOVE
            this->UnmarkChanged(8); // CELL_CHANGED
         }
         return true;
      }
   };
   void __declspec(noinline)  TESObjectREFR::Reload3D() {
      //
      // The below is a really simplified version of what Bethesda does under the hood, when moving a reference. 
      // 
      //  - First, they check if the reference is enabled. If so, they call Unk_89(), which sets the "is disabled" 
      //    flag, marks the reference as having moved, and unloads 3D (via a call to Unk_6C(0, 0)).
      //
      //  - Then, they do some other stuff. We don't care about any of it.
      //
      //  - Finally, they call the member function at 004E0E30, and as far as I can gather, that literally just 
      //    enables the bloody reference.
      //
      // They literally just disable and enable the reference to reload its 3D. Like, there isn't a more direct 
      // way of going about it at all. I'm kind of astounded.
      //
      if ((this->flags & form_flag::disabled) != 0) { // do not act on disabled references
         return;
      }
      this->Unk_6C(0, 0); // unloads 3D
      this->flags |= form_flag::disabled;
      CALL_MEMBER_FN(this, Subroutine004E0E30)();
   };
   void __declspec(noinline) TESObjectREFR::SetMotionType(uint8_t motionType, bool simulateImmediately, bool markChanged) {
      NiPointer<NiNode> node = this->GetNiNode(); // use a smart pointer so the node isn't deleted out from under us
      if (node.m_pObject) {
         SetNodeMotionType(node.m_pObject, motionType, true, false, simulateImmediately);
         if (markChanged)
            this->MarkChanged(4);
      }
   };

   TESObjectREFR* TESObjectREFR::make() {
      auto buffer = FormHeap_Allocate(sizeof(TESObjectREFR));
      auto ref    = (TESObjectREFR*) buffer;
      CALL_MEMBER_FN(ref, Constructor)();
      return ref;
   }

   TESObjectREFR* refr_ptr::operator->() {
      return ref;
   }
   refr_ptr::operator RE::TESObjectREFR*() const { return ref; }
   refr_ptr& refr_ptr::operator=(RE::TESObjectREFR* rhs) {
      _dec();
      if (ref != rhs) {
         ref = rhs;
         _inc();
      }
      return *this;
   }
   refr_ptr& refr_ptr::operator=(const refr_ptr& rhs) {
      _dec();
      if (ref != rhs.ref) {
         ref = rhs.ref;
         _inc();
      }
      return *this;
   }
   bool refr_ptr::operator!() {
      return ref == nullptr;
   }
   TESObjectREFR* refr_ptr::abandon() noexcept {
      auto r = this->ref;
      this->ref = nullptr;
      return r;
   };
   TESObjectREFR* refr_ptr::copy_bare() {
      _inc();
      return ref;
   };
   void refr_ptr::set_from_handle(const UInt32 handle) {
      _dec();
      UInt32 scrap = handle;
      LookupREFRByHandle(&scrap, (::TESObjectREFR**) &this->ref); // increments the refcount for us
   }
   void refr_ptr::set_from_handle(UInt32* handle) {
      _dec();
      LookupREFRByHandle(handle, (::TESObjectREFR**) &this->ref); // increments the refcount for us
   }
   refr_ptr::~refr_ptr() {
      _dec();
      ref = nullptr;
   };
};

