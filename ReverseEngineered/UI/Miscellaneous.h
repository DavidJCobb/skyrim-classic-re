#pragma once
#include <cstdint>
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Shared.h"
#include "skse/ScaleformTypes.h"
#include "skse/Utilities.h"

namespace RE {
   static DEFINE_SUBROUTINE(void, PlayUISound, 0x00899620, const char* editorID);

   DEFINE_SUBROUTINE_EXTERN(void, ShowNotification, 0x008997A0, const char* message, const char* soundEditorID, bool one);

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
               return;
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
   };
   
   class IMessageBoxCallback : public GRefCountImplCore {
      public:
         virtual void Dispose(bool);
         virtual void Callback(uint32_t buttonIndex) = 0;
   };
   class MessageBoxData { // sizeof == 0x2C
      public:
         static constexpr uint32_t vtbl = 0x010E452C;
         //
         uint8_t  unk04 = 0;
         uint8_t  unk05 = 0;
         uint8_t  pad06[2];
         BSString text; // 08
         tArray<BSString> buttons; // 10
         uint32_t unk1C = 0;
         scaleform_ptr<IMessageBoxCallback> callback;  // 20 // smart pointer to a callback
         uint32_t unk24 = 0xA; // callers often set this to 4
         uint8_t  unk28 = 0;
         uint8_t  unk29 = 0;
         uint8_t  pad2A[2];
         //
         MEMBER_FN_PREFIX(MessageBoxData);
         DEFINE_MEMBER_FN(Constructor, MessageBoxData&, 0x00852530);
   };
   static DEFINE_SUBROUTINE(void, SendMessageBoxData, 0x0087A9D0, MessageBoxData&);
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