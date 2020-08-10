#pragma once
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Systems/BSTEvent.h"

namespace RE {
	class TESObjectREFR;

   class MenuTopicManager {
		public:
			virtual ~MenuTopicManager();
			virtual void Unk_01(void);

			inline static MenuTopicManager* get() noexcept {
				return *((MenuTopicManager**)0x013105D8);
			}

			BSTEventSink<PositionPlayerEvent> playerPositionEvent; // 04 // event source is MenuManager+04
			uint32_t unk08;
			void*    unk0C;
			uint32_t unk10;
			uint32_t unk14;
			TESTopicInfo* unk18; // has member function 0057DD70
			void*    unk1C;
			CRITICAL_SECTION unk20;
			uint32_t unk34;
			ref_handle talkingHandle; // 38 // very likely the actor that the player is in dialogue with, as opposed to just whoever's subtitles are showing at the moment
			ref_handle handle2;							// 3C
			void* unk40;
			decltype(unk40) unk44;
			tArray<uint32_t> unk48;
			tArray<uint32_t> unk54;
			UInt8	unk60;
			UInt8	unk61;
			UInt8	unk62;
			UInt8	unk63;
			UInt8	unk64;
			UInt8	unk65;
			UInt8	unk66;
			UInt8	unk67;
			UInt8	unk68;
			UInt8	unk69;
			UInt8	unk6A;
			UInt8	unk6B;

			MEMBER_FN_PREFIX(MenuTopicManager);
		private:
			DEFINE_MEMBER_FN(Constructor, MenuTopicManager&, 0x00674520); // for documentation purposes only
		public:
			DEFINE_MEMBER_FN(SetTalkingHandle,   void, 0x00673510, TESObjectREFR*); // found in TESObjectREFR::Unk_40 which passes nullptr.
			DEFINE_MEMBER_FN(Subroutine006734F0, void, 0x006734F0, uint32_t); // called from an input handler?
			DEFINE_MEMBER_FN(Subroutine00673630, void, 0x00673630, uint32_t zero); // found in TESObjectREFR::Unk_40.
			DEFINE_MEMBER_FN(Subroutine006736B0, void, 0x006736B0, void*);
			DEFINE_MEMBER_FN(Subroutine00673900, bool, 0x00673900, uint32_t zero); // related to UI; possibly handles topic list
			DEFINE_MEMBER_FN(Subroutine00673A40, void, 0x00673A40); // clears unk48 and unk54
			DEFINE_MEMBER_FN(Subroutine00673AD0, uint32_t, 0x00673AD0); // returns an actual unsigned int
			DEFINE_MEMBER_FN(Subroutine00673FD0, void, 0x00673FD0);
			DEFINE_MEMBER_FN(Subroutine00674000, void, 0x00674000, uint32_t); // called from an input handler?
			DEFINE_MEMBER_FN(Subroutine006740E0, void, 0x006740E0, int32_t);            // call is wrapped in critical section unk20?
			DEFINE_MEMBER_FN(Subroutine006741B0, void, 0x006741B0, uint32_t, uint32_t); // call is wrapped in critical section unk20?
			DEFINE_MEMBER_FN(Subroutine006743E0, bool, 0x006743E0, void*); // argument is allocated on the stack and has a vtbl
			DEFINE_MEMBER_FN(Subroutine00674910, void, 0x00674910, TESObjectREFR*, bool); // found in TESObjectREFR::Unk_4C. contains the debug string "setlast speaker in the shut menu". maybe RunLastTopicEndScript
			DEFINE_MEMBER_FN(Subroutine00674EB0, void, 0x00674EB0, uint32_t, uint32_t, uint32_t zero);
			DEFINE_MEMBER_FN(Subroutine00675430, void, 0x00675430); // call is wrapped in critical section unk20
			DEFINE_MEMBER_FN(Subroutine006758A0, bool, 0x006758A0, TESObjectREFR*, bool, uint32_t, uint32_t zero); // found in TESObjectREFR::Unk_41.
			DEFINE_MEMBER_FN(Subroutine00675D00, void, 0x00675D00); // caller is Singleton012E32E8::DoAIProcessing
			DEFINE_MEMBER_FN(HideSubtitleText,      void, 0x00898190); // creates a BSUIMessageData with string "Hide Text" and sends it to UIManager, to be dispatched to the current DialogueMenu instance
			DEFINE_MEMBER_FN(SendVoiceReadyUIEvent, void, 0x008982E0); // creates a BSUIMessageData with string "Voice Ready" and sends it to UIManager, to be dispatched to the current DialogueMenu instance
			DEFINE_MEMBER_FN(SetHandle3C,   void, 0x00673540, TESObjectREFR*);
			DEFINE_MEMBER_FN(GetUnk0CUnk00, void*, 0x006735A0);
			DEFINE_MEMBER_FN(GetActorInDialogueWithPlayer, TESObjectREFR*, 0x00674450); // gets talkingHandle

			// 0070D3E0 accesses this object and runs position checks on the player?
			// 0073C3DF gets the actor in dialogue with the player and does stuff to the player camera
			// 0073C736, ditto
	};
	static_assert(offsetof(MenuTopicManager, talkingHandle) >= 0x38, "RE::MenuTopicManager::talkingHandle is too early!");
	static_assert(offsetof(MenuTopicManager, talkingHandle) <= 0x38, "RE::MenuTopicManager::talkingHandle is too late!");
}
