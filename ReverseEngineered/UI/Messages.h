#pragma once
#include "ReverseEngineered/Forms/TESObjectREFR.h"
#include "ReverseEngineered/UI/Miscellaneous.h"
#include "skse/GameTypes.h"
#include <type_traits>

namespace RE {
	class Actor;

	class IUIMessageData { // sizeof == 0x4 (interface; has no fields)
		public:
			static constexpr uint32_t vtbl = 0x010CF27C;
			virtual ~IUIMessageData();
	};
	class BSUIMessageData : public IUIMessageData { // sizeof == 0x14
		public:
			static constexpr uint32_t vtbl = 0x010CF284;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			BSString*     unk08 = nullptr; // allocated on the game heap; owned by the BSUIMessageData. essentially a parameter, e.g. subtitle text for DialogueMenu "Show Text"
			BSFixedString unk0C = nullptr; // essentially a message type, e.g. "Show Text" or "Hide Text" for DialogueMenu.
			uint32_t      unk10 = 0;
	};
	class ConsoleData : public IUIMessageData { // sizeof == 0x18
		public:
			static constexpr uint32_t vtbl = 0x010E8D3C;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			uint32_t unk08 = 4;
			void*    unk0C = nullptr; // owned
			uint32_t unk10 = 0;
			void*    unk14 = nullptr; // owned; has a virtual destructor
			//
			MEMBER_FN_PREFIX(ConsoleData);
			DEFINE_MEMBER_FN(Destructor, void, 0x008975B0);
	};
	class FaderData : public IUIMessageData { // sizeof == 0x1C
		public:
			static constexpr uint32_t vtbl = 0x010E8D54;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			void*    unk08 = nullptr; // smart pointer
			uint8_t  unk0C = 3;
			uint8_t  pad0D[3];
			uint32_t unk10 = 0;
			float    unk14 = 1.0F;
			uint8_t  unk18 = 1;
			uint8_t  unk19 = 1;
			uint8_t  unk1A = 0;
			uint8_t  unk1B = 0;
	};
	class HUDData : public IUIMessageData { // sizeof == 0x28
		public:
			static constexpr uint32_t vtbl = 0x010E8D4C;
			enum class type_t : uint32_t {
				unk_00 = 0,
				show_notification = 1,
				update_subtitle   = 5,
				unk_06 = 6, // always sent with a nullptr string
			};
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			type_t   unk08 = type_t::unk_00;
			BSString unk0C;
			uint32_t unk14 = 0;
			uint32_t unk18 = 0;
			uint32_t unk1C = 0;
			uint8_t  unk20 = 0;
			uint8_t  pad21[3];
			uint32_t unk24 = 0;
			//
			MEMBER_FN_PREFIX(HUDData);
			DEFINE_MEMBER_FN(Constructor, HUDData&, 0x00897640); // one of the few that didn't get inlined (all of these message-data objects are constructed by factories)
			//
			inline static HUDData* make_with_string(type_t t, const char* s = nullptr) {
				DEFINE_SUBROUTINE(HUDData*, CreateHUDDataWithString, 0x00897480, type_t, const char*);
				return (CreateHUDDataWithString)(t, s);
			}
	};
	class InventoryUpdateData : public IUIMessageData { // sizeof == 0x10
		public:
			static constexpr uint32_t vtbl = 0x010E8D14;
			//
			uint8_t    unk04 = 0;
			uint8_t    unk05 = 0;
			uint16_t   pad06;
			ref_handle unk08 = *g_invalidRefHandle;
			uint32_t   unk0C = 0;
	};
	class KinectStateChangeData : public IUIMessageData {
		public:
			static constexpr uint32_t vtbl = 0x010E8D2C;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			uint32_t unk08;
			BSFixedString unk0C = nullptr;
	};
	class KinectUserEventData : public IUIMessageData { // sizeof == 0x10
		public:
			static constexpr uint32_t vtbl = 0x010E8D34;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			BSFixedString unk08;
			BSFixedString unk0C;
	};
	class LoadingMenuData : public IUIMessageData { // sizeof == 0x10
		public:
			static constexpr uint32_t vtbl = 0x010E8D24;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			uint32_t unk08;
			uint32_t unk0C;
	};
   class MessageBoxData : public IUIMessageData { // sizeof == 0x2C, constructed on the game heap
      public:
         static constexpr uint32_t vtbl = 0x010E452C;
         virtual ~MessageBoxData();
         //
         uint8_t  unk04 = 0;
         uint8_t  unk05 = 0;
         uint8_t  pad06[2];
         BSString text; // 08
         tArray<BSString> buttons; // 10
         uint32_t unk1C = 0;
         scaleform_ptr<IMessageBoxCallback> callback; // 20 // smart pointer to a callback
         uint32_t unk24 = 0xA; // callers often set this to 4
         uint8_t  unk28 = 0;
         uint8_t  unk29 = 0;
         uint8_t  pad2A[2];
         //
         MEMBER_FN_PREFIX(MessageBoxData);
         DEFINE_MEMBER_FN(Constructor,   MessageBoxData&, 0x00852530);
         DEFINE_MEMBER_FN(Constructor_5, MessageBoxData&, 0x0087A940, BSString& text, scaleform_ptr<IMessageBoxCallback>&, uint32_t, uint32_t, uint32_t, BSTArray<BSString>& buttons); // used by Papyrus
   };
	class RefHandleUIData : public IUIMessageData { // sizeof == 0xC
		public:
			static constexpr uint32_t vtbl = 0x010E8D44;
			//
			uint8_t    unk04 = 0;
			uint8_t    unk05 = 0;
			uint16_t   pad06;
			ref_handle unk08 = 0;
	};
	class TESFormUIData : public IUIMessageData { // sizeof == 0xC
		public:
			static constexpr uint32_t vtbl = 0x010E8D1C;
			//
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			uint32_t unk08; // probably the form pointer, but it's not initialized in the constructor, so... :/ 
	};
	template<class C> class UIMessageDataPtr : public IUIMessageData { // sizeof == 0xC
		protected:
			//
			// Bethesda only templates this on Actor, and they use a smart pointer, so I'm going 
			// to assume that (C) must at least be a TESObjectREFR.
			//
			using _requirement = std::enable_if_t<std::is_base_of_v<TESObjectREFR, C>, int>;
			//
		public:
			uint8_t  unk04 = 0;
			uint8_t  unk05 = 0;
			uint16_t pad06;
			refr_ptr unk08;
	};

	struct BSUIScaleformData { // not an IUIMessageData subclass, but fields resemble one
		struct Unk {
			uint32_t unk00;
			uint32_t unk04;
			const char**  unk08;
			BSFixedString unk0C; // DialogueMenu compares this to "Show Text"
		};
		//
		static constexpr uint32_t vtbl = 0x0110DF70;
		virtual ~BSUIScaleformData() {}

		uint8_t  unk04 = 0;
		uint8_t  unk05 = 0;
		uint16_t unk06;
		void*    unk08 = nullptr; // 08 // not owned
	};
}
