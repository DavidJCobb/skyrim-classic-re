#pragma once
#include "skse/GameAPI.h"
#include "skse/GameTypes.h"
#include "NetImmerse/objects.h"
#include "NetImmerse/types.h"
#include "Miscellaneous.h"

namespace RE {
   class BSString { // sizeof == 0x8
      public:
         ~BSString() { // assumes game heap allocation
            if (this->m_data) {
               FormHeap_Free(this->m_data);
               this->m_data = nullptr;
            }
            this->m_dataLen = this->m_bufLen = 0;
         }
         //
         char*  m_data    = nullptr; // 00
         UInt16 m_dataLen = 0; // 04
         UInt16 m_bufLen  = 0; // 06

         MEMBER_FN_PREFIX(BSString);
         DEFINE_MEMBER_FN(assign, bool, 0x00402C00, const char*, UInt32 maxLength); // zero maxLength will copy the entire input string
   };

   struct BSTArrayAllocator {
      virtual bool initial(UInt32, UInt32) = 0; // 00
      virtual bool realloc(UInt32 desiredCount, UInt32 currentCount, UInt32 zeroA, UInt32 zeroB, UInt32) = 0; // 01

      // unk04 is typically a pointer to the array
   };
   struct BSTArrayCount { // actual name not known
      MEMBER_FN_PREFIX(BSTArrayCount);
      DEFINE_MEMBER_FN(Constructor, BSTArrayCount&, 0x00A49B40); // literally just sets itself to zero
      DEFINE_MEMBER_FN(Increment,           SInt32, 0x00A49B83, void* allocator, UInt32 currentCapacity, UInt32 seemsToAlwaysBe4EvenWhenTheArrayElementSizeIsLarger);
      DEFINE_MEMBER_FN(IncrementReturnBool, bool,   0x00A4A0B0, void* allocator, uint32_t, uint32_t currentCapacity, uint32_t index, uint32_t elementSize);
      DEFINE_MEMBER_FN(Remove,      void,   0x00A4A070, void* arrayData, UInt32 startIndex, UInt32 arraySizeMinusCountToRemove, UInt32 countToRemove, UInt32 sizeOfElement);

      UInt32 size; // 00

      inline operator UInt32&() { return this->size; }
      inline operator const UInt32&() const { return this->size; }
   };

   template<typename T, UInt32 minCapacity> struct BSTSmallArray {
      enum {
         kFlagsMask  = 0x80000000,
         //
         kFlag_Local = 0x80000000, // confirmed: this is the only flag
      };
      ~BSTSmallArray() { CALL_MEMBER_FN(this, Destructor)(); };
      //
      UInt32 capacityAndFlags; // 00
      union {
         T  local[minCapacity]; // use if flag is set
         T* arr;
      } data; // 04
      BSTArrayCount count; // 04+(sizeof(T)*minCapacity)

      UInt32 capacity() const {
         return this->capacityAndFlags & ~kFlagsMask;
      }
      void decrement_refcounts(UInt32 start, UInt32 end) {};
      T* items() {
         if (this->count == 0)
            return nullptr;
         if (this->capacityAndFlags & kFlag_Local)
            return &this->data.local;
         return this->data.arr;
      }
      const T* items() const {
         if (this->count == 0)
            return nullptr;
         if (this->capacityAndFlags & kFlag_Local)
            return &this->data.local;
         return this->data.arr;
      }
      inline UInt32 size() const noexcept {
         return this->count;
      }

      T* at(UInt32 index) {
         auto i = this->items();
         if (!i)
            return nullptr;
         return &i[index];
      }
      T& operator[](UInt32 index) { return this->items()[index]; }
      const T& operator[](UInt32 index) const { return this->items()[index]; }

      T* end() const noexcept {
         auto i = this->items();
         if (!i)
            return nullptr;
         return &this->items()[this->count];
      }

      MEMBER_FN_PREFIX(BSTSmallArray);
      DEFINE_MEMBER_FN(Destructor,   void,   0x00A4A8F0);
      DEFINE_MEMBER_FN(AddressOf,    T*,     0x006B0540, UInt32 index);
      DEFINE_MEMBER_FN(Increment,    SInt32, 0x00A49B80, BSTArrayAllocator*, UInt32 currentCapacity, UInt32 elementSize); // returns -1 on failure
      DEFINE_MEMBER_FN(InitialAlloc, bool,   0x00A4A720, UInt32 desiredCount, UInt32 minCapacity, UInt32 elementSize);
      DEFINE_MEMBER_FN(Reallocate,   bool,   0x00A4A7A0, UInt32 desiredCount, UInt32 currentCount, UInt32 zeroA, UInt32 zeroB, UInt32 minCapacity, UInt32 elementSize);
      //
      // Only valid for arrays templated on NiPointer:
      //
      DEFINE_MEMBER_FN(SetElements, void, 0x006B0C90, UInt32 index, UInt32 numCopies, NiPointer<NiRefObject>& value); // assumes you've already allocated enough room
      DEFINE_MEMBER_FN(MassDecreaseRefcounts, void, 0x006B0930, UInt32 startIndex, UInt32 endIndex);
   };

   template<class T> struct tList {
      //
      // The SKSE team did an iterator-LIKE implementation for tLists. 
      // This class definition straightforwardly matches Bethesda's 
      // internal implementation; members aren't made private, so you 
      // can replicate code and processes exactly as you see them in 
      // the executable.
      //
      operator ::tList<T>*() const { return (::tList<T>*) this; } // allow use with SKSE-defined methods
      struct Node {
         T*    data;
         Node* next;
         Node* prev;
         //
         MEMBER_FN_PREFIX(Node);
         // TODO next function is only used for singly-linked lists, not doubly-linked lists; verify that tList itself is supposed to be doubly-linked
         DEFINE_MEMBER_FN(append, void, 0x0042AFA0, T**, void* optionalNodeConstructor, void* constructorState); // if this->data == nullptr, just adopts the input node's data instead; if input node's data is nullptr, does nothing
      };
      Node items;
      //
      MEMBER_FN_PREFIX(tList);
      DEFINE_MEMBER_FN(Destructor, void,     0x00476A70);
      DEFINE_MEMBER_FN(size,       uint32_t, 0x0048CA90); // returns the number of Ts, not the number of Nodes; Nodes with null data pointers are not counted
      //
      T* operator[](int at) {
         Node* node = this->items;
         for (int i = 0; i < at; ++i)
            if (!(node = node->next))
               break;
         return node ? node->data : nullptr;
      }
   };
   class SimpleLockReversed {
      private:
         UInt32 lockCount = 0;
         UInt32 threadID;
      public:
         MEMBER_FN_PREFIX(SimpleLockReversed);
         DEFINE_MEMBER_FN(lock,   void, 0x00A4AEE0);
         DEFINE_MEMBER_FN(unlock, void, 0x00A4B0F0);
   };
   class UnknownLock01 {
      //
      // lockCount is handled differently from SimpleLockReversed.
      //
      private:
         UInt32 lockCount = 0;
         UInt32 threadID;
      public:
         MEMBER_FN_PREFIX(UnknownLock01);
         DEFINE_MEMBER_FN(lock,   void, 0x00A4AF50);
         DEFINE_MEMBER_FN(unlock, void, 0x00A4B100);
         //
         class guard {
            private:
               UnknownLock01* lock = nullptr;
            public:
               inline guard(UnknownLock01* l) : lock(l) { // must not be nullptr
                  CALL_MEMBER_FN(this->lock, lock)();
               };
               inline guard(UnknownLock01& l) : lock(&l) {
                  CALL_MEMBER_FN(this->lock, lock)();
               };
               inline ~guard() {
                  CALL_MEMBER_FN(this->lock, unlock)();
               };
         };
   };
   //
   class simple_lock_rev_guard { // similar to std::lock_guard but for SimpleLock
      private:
         SimpleLockReversed* lock = nullptr;
      public:
         inline simple_lock_rev_guard(SimpleLockReversed* l) : lock(l) { // must not be nullptr
            CALL_MEMBER_FN(this->lock, lock)();
         };
         inline simple_lock_rev_guard(SimpleLockReversed& l) : lock(&l) {
            CALL_MEMBER_FN(this->lock, lock)();
         };
         inline ~simple_lock_rev_guard() {
            CALL_MEMBER_FN(this->lock, unlock)();
         };
   };
   class simple_lock_guard { // similar to std::lock_guard but for SimpleLock
      private:
         SimpleLock* lock = nullptr;
      public:
         inline simple_lock_guard(SimpleLock& l) : lock(&l) {
            l.Lock();
         };
         inline simple_lock_guard(::SimpleLock& l) : lock((RE::SimpleLock*) &l) {
            l.Lock();
         };
         inline ~simple_lock_guard() {
            this->lock->Release();
         };
   };
};