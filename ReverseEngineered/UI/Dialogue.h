#pragma once
#include "ReverseEngineered/UI/IMenu.h"
#include "ReverseEngineered/Systems/BSTEvent.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
   class DialogueMenu : public IMenu { // sizeof == 0x2C
      public:
         static constexpr uint32_t vtbl = 0x010E4C9C;
         //
         BSTEventSink<MenuOpenCloseEvent> openCloseSink; // 1C
         tArray<uint32_t> unk20;
         //
         void SetSpeakerName(const char* s);
         void HideDialogueText(); // listed here for reference purposes only
         //
         MEMBER_FN_PREFIX(DialogueMenu);
         DEFINE_MEMBER_FN(ShowDialogueText, void, 0x00859BD0, const char**); // sets the subtitle text
         DEFINE_MEMBER_FN(HideDialogueText, void, 0x00859C70);
         DEFINE_MEMBER_FN(SetSpeakerNameAndDuckOtherAudio, void, 0x00859EC0, void*); // arg is likely BSUIScaleformData*
   };
   //
   // Related addresses:
   // 0x01B3E8B0  Bool. Meaning unknown. Menu constructor sets it to false.
   //
   static DEFINE_SUBROUTINE(void, SetCurrentDialogueMenuSubtitle, 0x008980D0, const char* text);

   static DEFINE_SUBROUTINE(bool, AreSubtitlesEnabled, 0x00892710);

   inline constexpr char* dialogue_subtitle_format = "<font color='#%.6X'>%s</font>: %s"; // stored at 0x010E7E64. params: subtitle color RGB as 0x00RRGGBB; speaker name; dialogue text
   void SetHUDSubtitle(const char* full_text);
   void SetHUDSubtitle(const char* speaker, const char* text);
};
