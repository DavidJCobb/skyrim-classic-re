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
   template<typename T> struct BSTArray {
      protected:
         static constexpr int ce_grow_by = 5;
         //
         inline void _default_construct_at(uint32_t i) {
            new (&this->_data[i]) T(); // default-construct in place
         }
         //
      public:
         T*            _data     = nullptr;
         uint32_t      _capacity = 0;
         BSTArrayCount _size     = 0;
         //
         inline uint32_t capacity() const noexcept { return this->_capacity; };
         inline uint32_t size() const noexcept { return this->_size; }
         //
         __declspec(noinline) T& append() {
            if (this->_size + 1 >= this->_capacity)
               this->reserve(this->_capacity + ce_grow_by);
            this->_default_construct_at(this->_size);
            ++this->_size;
            return this->_data[this->_size - 1];
         }
         void push_back(const T& item) {
            this->append();
            this->_data[this->_size - 1] = item;
         }
         __declspec(noinline) void reserve(uint32_t cap) {
            if (cap <= this->_capacity)
               return;
            auto* migrate = FormHeap_Allocate(sizeof(T) * cap);
            if (this->_data) {
               memcpy(migrate, this->_data, sizeof(T) * this->_capacity);
               FormHeap_Free(this->_data);
            }
            this->_data     = (T*)migrate;
            this->_capacity = cap;
         }
         //
         inline T& operator[](int i) noexcept { return this->_data[i]; }
         inline const T& operator[](int i) const noexcept { return this->_data[i]; }
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

   template <typename Key> uint32_t BSTDefaultScatterTableDefaultHashFunction(const Key& key) {
      uint32_t crc;
      CRC32_Calc4((UInt32*)&crc, key); // what a stupid cast to have to do
      return crc;
   };
   template <typename Key> using BSTDefaultScatterTableHashFunction = decltype(&BSTDefaultScatterTableDefaultHashFunction<Key>);
   template <typename Key, typename Value, BSTDefaultScatterTableHashFunction<Key> _hash_function = BSTDefaultScatterTableDefaultHashFunction> class BSTDefaultScatterTable { // sizeof == 0x1C
      //
      // This is the class that SKSE's class definitions call "tHashSet." However, their class 
      // definition has a pretty significant problem: it frequently uses values as keys instead 
      // of using, uh, keys as keys.
      //
      // As far as I can tell, Bethesda doesn't pass the hash function as a template argument; 
      // I've done that here in the hopes of making this class marginally less painful to use 
      // (i.e. not having to create structs for form IDs just so I can give them a member 
      // function that templated code here could call).
      //
      public:
         class entry_t;
         //
		protected:
         inline static const entry_t* const sentinel_entry_pointer = (const entry_t*)0x012889D8; // (item) == 0xDEADBEEF
         //
      public:
         using key_type   = Key;
         using value_type = Value;
         class entry_t {
            public:
               key_type   key;  // 00
               value_type data;
               entry_t*   next = sentinel_entry_pointer;
         };
         //
      public:
         uint32_t unk00;           // 00
		   uint32_t _size       = 0; // 04
		   uint32_t _freeCount  = 0; // 08
		   uint32_t _freeOffset = 0; // 1C
         entry_t* _end        = sentinel_entry_pointer; // 10
		   uint32_t unk14;           // 14
         entry_t* _entries    = nullptr; // 18
         //
         uint32_t size() const noexcept { return this->_size; }
         uint32_t free_count() const noexcept { return this->_freeCount; }
         uint32_t fill_count() const noexcept { return this->_size - this->_freeCount; }
         //
         [[nodiscard]] entry_t* entry_by_hash(uint32_t hash) const {
            return this->_entries ? &this->_entries[hash & (size() - 1)] : nullptr;
         }
         [[nodiscard]] value_type* lookup(key_type& key) const {
            auto hash = _hash_function(key);
            auto e    = this->entry_by_hash(hash);
            if (!e)
               return nullptr;
            do {
               if (e->key == key)
                  return &e->data;
            } while ((e = e->next) != this->_end);
            return nullptr;
         }
         //
         MEMBER_FN_PREFIX(BSTDefaultScatterTable);
         DEFINE_MEMBER_FN(Destructor, void, 0x00CB55B0);
         DEFINE_MEMBER_FN(Subroutine0084E630, void, 0x0084E630, uint32_t count); // count == this->fill_count();
         DEFINE_MEMBER_FN(Subroutine00856230, void, 0x00856230, void*);

         ~BSTDefaultScatterTable() { CALL_MEMBER_FN(this, Destructor)(); }

         //
         // The functions below are known to work when key_type is uint32_t, and 
         // when it is nay struct whose first field is a hashable dword.
         //

         // Bethesda always passes the same value for both value_type& arguments. 
         // May only work if sizeof(value_type) == 4.
         DEFINE_MEMBER_FN(insert,      void, 0x007B0480, key_type&, value_type& id, value_type& value);
         DEFINE_MEMBER_FN(insert_impl, bool, 0x005015F0, entry_t& after, uint32_t hash, key_type&, value_type& id, value_type& value); // internal; used by 0x007B0480

         DEFINE_MEMBER_FN(remove_impl, void, 0x00B9FC10, entry_t& after, uint32_t hash, key_type&, value_type& maybeOut); // internal. remove an element?
   };
   static_assert(sizeof(BSTDefaultScatterTable<void*, void*>) >= 0x1C);
   static_assert(sizeof(BSTDefaultScatterTable<void*, void*>) <= 0x1C);
   //
	template <typename Key, typename Value, typename Table = BSTDefaultScatterTable<Key, Value>> class BSTHashMap { // sizeof == 0x20
      //
      // Bethesda uses BSTHashMapBase<BSTHashMapTraits<...>>, where the traits' template arguments 
      // appear to be a key type, a value type, and BSTDefaultScatterTable templated on the same 
      // key and value types.
      //
      // I lucked out with this one. BSTHashMap isn't virtual, but it has RTTI by virtue of being 
      // one of the parent classes for AttackAnimationArrayMap, which is virtual. Examination of 
      // the full RTTI established that BSTHashMap appears 0x08 bytes into AttackAnimationArrayMap, 
      // and further established that the actual "scatter table" class appears 0x0C bytes into the 
      // AttackAnimationArrayMap class.
      //
      public:
         using key_type   = Key;
         using value_type = Value;
         using table_type = Table;
         //
		public:
		   uint32_t unk00; // 00
         Table    table; // 04
         //
         MEMBER_FN_PREFIX(BSTHashMap);
         DEFINE_MEMBER_FN(remove, void, 0x00D9BE20, key_type&, value_type&); // see remove_impl above
	};
	static_assert(sizeof(BSTHashMap<void*, void*>) >= 0x20);
   static_assert(sizeof(BSTHashMap<void*, void*>) <= 0x20);

   /*
	template <typename Item, typename Key> class BSTObjectDictionary { // sizeof == 0x1C
      //
      // Bethesda's template arguments are: Item, Key, MissPolicy, InitializationPolicy
      //
		protected:
		   class _Entry {
			   public:
				   Item    item;
				   _Entry* next = nullptr;
		   };
         static const _Entry* const sentinel_entry_pointer = (const _Entry*)0x012889D8; // (item) == 0xDEADBEEF
         //
		public:
         using key_type   = Key;
         using value_type = Item;
         //
		   uint32_t unk00; // 000
		   uint32_t _size       = 0; // 004
		   uint32_t _freeCount  = 0; // 008
		   uint32_t _freeOffset = 0; // 00C
		   _Entry*  _endPtr     = sentinel_entry_pointer; // 010
		   uint32_t unk14       = 0; // 014
		   _Entry*  _entries    = nullptr; // 018
         //
         uint32_t size() const noexcept { return this->_size; }
         uint32_t free_count() const noexcept { return this->_freeCount; }
         uint32_t fill_count() const noexcept { return this->_size - this->_freeCount; }
         //
         MEMBER_FN_PREFIX(BSTObjectDictionary);
         DEFINE_MEMBER_FN(Insert, void, 0x007B0480, key_type&, value_type&, value_type&); // bethesda usually passes the same value for both value_type params
         DEFINE_MEMBER_FN(Subroutine00458550, void, 0x00458550, decltype(_entries)&, uint32_t& key_hash, const key_type&, value_type& out);
         //
         inline void lookup(key_type& key, value_type& out) {
            uint32_t crc;
            CRC32_Calc4(&crc, *(uint32_t*)&key); // TODO: find a way to specify the hash function to use as a template argument
            CALL_MEMBER_FN(this, Subroutine00458550)(this->_entries, crc, key, value);
         }
	};
	static_assert(sizeof(BSTObjectDictionary<void*, void*>) >= 0x1C);
   static_assert(sizeof(BSTObjectDictionary<void*, void*>) <= 0x1C);
   */
};