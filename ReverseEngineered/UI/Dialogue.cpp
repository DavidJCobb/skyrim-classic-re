#include "Dialogue.h"
#include "ReverseEngineered/UI/Miscellaneous.h"
#include "skse/GameMenus.h"

namespace RE {
   void DialogueMenu::SetSpeakerName(const char* name) {
      FxResponseArgs<1> args;
      args.args[1].SetString(name); // yes, #1, not #0
      Subroutine00A63C40(this->view, "SetSpeakerName", args);
      /*//
      //
      // TODO: Is the below necessary, or is it just part of displaying the menu to begin with?
      //
      DEFINE_SUBROUTINE(void*, sub, 0x00897430, int, int, const char*, int);
      auto data = sub(9, 1, "", 0);
      CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&(UIStringHolder::GetSingleton()->hudMenu), 0, data);
      //*/
   }
   void DialogueMenu::HideDialogueText() { // modeled on non-member function 0x00859C70
      FxResponseArgs<0> args;
      Subroutine00A63C40(this->view, "HideDialogueText", args);
   }
}