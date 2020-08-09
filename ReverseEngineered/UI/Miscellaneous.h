#pragma once
#include <cstdint>
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Shared.h"
#include "skse/ScaleformCallbacks.h"
#include "skse/ScaleformTypes.h"
#include "skse/Utilities.h"

namespace RE {
   static DEFINE_SUBROUTINE(void, PlayUISound, 0x00899620, const char* editorID);

   DEFINE_SUBROUTINE_EXTERN(void, ShowNotification, 0x008997A0, const char* message, const char* soundEditorID, bool one);

   class FxResponseArgsBase {
      public:
         virtual ~FxResponseArgsBase() {};
         virtual uint32_t GetValues(GFxValue*& params) { return 0; } // pure in vanilla; dummy here, to allow construction. params is set to the first of (args); the return value is (curArg)
   };
   template <UInt32 arg_count> class FxResponseArgs : public FxResponseArgsBase { // sizeof == 0xC + ((arg_count + 1) * 0x10)
      protected:
         template<int I> static constexpr typename std::enable_if_t<I == 0, uint32_t> _get_vtbl_address() { return 0x010E37A4; }
         template<int I> static constexpr typename std::enable_if_t<I == 1, uint32_t> _get_vtbl_address() { return 0x010E37B0; }
         //
         // Destructors listed here for reference purposes only. In some cases they're inlined into the 
         // virtual Dispose method and thus can't be listed separately.
         //
         template<int I> static constexpr typename std::enable_if_t<I == 1, uint32_t> _get_destructor_address() { return 0x00841B00; }
         //
      public:
         static constexpr uint32_t vtbl = _get_vtbl_address<arg_count>();
         //
         FxResponseArgs() {
            *(uint32_t*)this = vtbl;
            this->curArg = 1;
         }
         virtual ~FxResponseArgs() {}

         uint32_t align04;             // 04 // needed because GFxValue contains a double and so must be 8-byte-aligned
         GFxValue args[arg_count + 1];	// 08
         uint32_t curArg;              // n8 // offsets assume one argument

         MEMBER_FN_PREFIX(FxResponseArgs);
         DEFINE_MEMBER_FN(Destructor, void, _get_destructor_address()); // for documentation purposes only
   };

   static DEFINE_SUBROUTINE(void, Subroutine00A63C40, 0x00A63C40, GFxMovieView*, const char*, FxResponseArgsBase&);

   template<typename type> class scaleform_ptr {
      protected:
         using mapped_type = type*;
         //
         mapped_type data = nullptr;
         inline void _inc() {
            if (data)
               data->AddRef();
         };
         inline void _dec() {
            if (data)
               data->Release();
         };
      public:
         scaleform_ptr() {}
         scaleform_ptr(mapped_type p) { *this = p; }
         scaleform_ptr& operator=(mapped_type p) noexcept {
            if (data == p)
               return *this;
            _dec();
            data = p;
            _inc();
            return *this;
         }
         ~scaleform_ptr() { *this = nullptr; }
         //
         inline mapped_type operator->() { return data; }; // implies comparison operators with pointers, because it allows for an implicit cast
         inline operator bool() const { return data != nullptr; };
         inline bool operator!() const { return data == nullptr; }
         inline operator mapped_type() const { return data; }
         //
         void adopt_without_increment(mapped_type p) {
            this->_dec();
            this->data = p;
         }
   };
   
   class IMessageBoxCallback : public GRefCountImplCore { // sizeof == 0x08
      public:
         virtual ~IMessageBoxCallback() {};
         virtual void Callback(uint8_t buttonIndex) = 0;

         IMessageBoxCallback() { this->refCount = 0; } // SKSE's GRefCountImplCore constructor incorrectly initializes this to 1, probably because they based it on code where the use of a scaleform_ptr was not obvious (i.e. the object was constructed inline and immediately stuffed into a smart pointer)

         //
         // Make (new) and (delete) use the game heap for this class and its subclasses:
         //
         static void* operator new(std::size_t size) {
            return FormHeap_Allocate(size);
         }
         static void* operator new(std::size_t size, const std::nothrow_t&) {
            return FormHeap_Allocate(size);
         }
         static void* operator new(std::size_t size, void* ptr) { // placement new
            return ptr;
         }
         static void operator delete(void* ptr) {
            FormHeap_Free(ptr);
         }
         static void operator delete(void* ptr, const std::nothrow_t&) {
            FormHeap_Free(ptr);
         }
         static void operator delete(void* ptr, void* place) {} // called if placement new throws an exception; shouldn't do anything
   };
   class MessageBoxData; // see "Messages.h"
   static DEFINE_SUBROUTINE(void, SendMessageBoxData, 0x0087A9D0, MessageBoxData&);
   static DEFINE_SUBROUTINE(void, ShowMessage, 0x0087ABA0, BSString& text, scaleform_ptr<IMessageBoxCallback>&, uint32_t papyrusUses0, uint32_t papyrusUses0x19, uint32_t papyrusUses4, BSTArray<BSString>& buttons); // same args as MessageBoxData::Constructor_5; constructs and then calls SendMessageBoxData
};

/*

   NOTES:

   BarterMenu::unk1C is most likely an instance of struct InventoryData whose unk28 is a BStArray of StandardItemData.
   
   ---

   void sub00843AD0(InventoryData*, ExtraContainerChanges::Data*, void*, BarterParams&)

   This function most likely loops over the barter-target's items somehow and stores information somewhere. I can't
   ID the type of the third parameter, but the fourth is:

   struct BarterParams {
      TESObjectREFR* barterTarget; // 00 // actually a smart pointer
      bool unk04; // 04
      bool bypassVendorStolenCheck;  // 05 // player actor value BypassVendorStolenCheck != 0?
      bool bypassVendorKeywordCheck; // 06 // player actor value BypassVendorKeywordCheck != 0?

      MEMBER_FN_PREFIX(BarterParams);
      DEFINE_MEMBER_FN(Constructor, BarterParams&, 0x00842D80, UInt32 refHandle);
      DEFINE_MEMBER_FN(TestItem,    bool,          0x00842830, void* item);
   }

   the subroutine at 0x00843AD0 eventually calls member function 0x008433B0 on its first argument, passing an inventory 
   item and a pointer to other structure (possibly an out-variable); that member function creates a StandardItemData 
   instance (I think; SKSE hooked the function to hell and back and I don't know what those hooks are offhand) and stores 
   it in a tArray at offset 0x28 on this.

*/