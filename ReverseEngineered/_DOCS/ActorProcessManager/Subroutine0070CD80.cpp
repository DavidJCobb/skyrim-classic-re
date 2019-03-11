void ActorProcessManager::Subroutine0070CD80(Actor* Arg1) {
   auto esi = this;
   if (!this->unk08)
      return;
   auto edi = Arg1;
   if (edi->actorState.flags08 & 0x000000E0) { // unknown, run, or walk
      edi->DrawSheatheWeapon(false);
      return;
   }
   TESPackage* esp10 = this->Subroutine006F4D40();
   refr_ptr    esp1C;
   esp1C.set_from_handle(&this->unk80); // nullptr, after the softlock
   if (!esp1C) {
      this->Subroutine00727080(Arg1);
      esp1C.set_from_handle(&this->unk80);
      if (!esp1C) {
         // jumped to 0x0070CFBF
         (*g_thePlayer)->TESV_006E8290(1);
         this->TESV_006F4DF0(Arg1, 1);
         Arg1->processManager->TESV_00721FC0(0, *g_thePlayer);
         (*g_thePlayer)->SetPlayerAIDriven(false);
         return;
      }
   }
   // at 0x0070CE02
   eax = esp1C->flags;
   if (!(eax & kFormFlag_IsDisabled) && !(eax & kFormFlag_IsDeleted) && !esp1C->IsDead(false)) { // at 0x0070CE2F
      float  dist = 0.0;
      Actor* ebx = esp1C->formType == 0x3E ? esp1C : nullptr;
      if (*this->GetMiddleProcessFurnitureHandle() != *g_invalidRefHandle) {
         dist = edi->GetDistanceSquared(&this->unk04->unk84);
      }
      //
      // SOFTLOCK INVESTIGATION:
      //
      //  - If GetFlag02InUnk9A always returns true, then we're able to move while feeding, 
      //    we don't play the animation, and we don't softlock.
      //
      //  - If GetUnk08ByteUnk33D always returns true, then feeding appears to work as 
      //    normal, with no softlock.
      //
      //  - The 33D byte is set by PickNewIdleHandler::Unk_01(Actor*). Those objects are 
      //    created by a factory object whose constructor is at 0x007853D0; the constructor 
      //    in turn is only called in one place, to create a static instance at 0x01B3BFD0.
      //
      //     - The factory object is: BSTDerivedCreator<class PickNewIdleHandler,class IHandlerFunctor<class Actor,class BSFixedStringCI>>
      //
      //     - All such factories are registered via Unknown01B3B314::Subroutine005B9EC0. 
      //       That singleton appears to be responsible for storing factories for a number 
      //       of these "handlers." Each handler has a corresponding string, which is 
      //       generally (or always?) the handler's classname.
      //
      //     - 0077FC50 takes an animation event name and looks it up from one of two text 
      //       files stored in Data/meshes/responses/ (one for players and one for NPCs). 
      //       These text files map animation event names to ...Handler classnames. When a 
      //       handler is found, 0077FBB0 is called to acquire and use the relevant factory; 
      //       the created handler instance is stored in a smart pointer, and some weird 
      //       stuff gets done to it.
      //
      //        - Presumably, then, this constitutes a flow from receiving an animation 
      //          event, to creating a handler instance, to using the handler.
      //
      //        - Further studies trace this back to the IAnimationGraphManagerHolder on 
      //          an Actor, and suggest that a BSResponse is created and stored somewhere 
      //          in what SKSE has labeled the MiddleProcess. There can only be one such 
      //          BSProcess there at any given moment; could another animation event clobber 
      //          the one for switching idles?
      //
      //           - Traced the call stack almost to the root:
      //
      //              - [three callers, including an AI linear task thread]
      //              - Unknown012E32E8::Subroutine00756460 + 0x9D
      //              - Actor::Subroutine006D6360
      //              - Actor::Subroutine006D4EA0 + 0x137
      //              - BSTEventSink<BSAnimationGraphEvent>::Subroutine006D21F0
      //              - BSResponse::Subroutine006CE350
      //              - BSResponse::Unk_01
      //
      //           - Is PickNewIdleHandler a subclass of BSResponse? Or a typedef?
      //
      if (dist >= 1600.0 && this->GetFlag02InUnk9A()) { // at 0x0070CE90 // distance needs to be 40 world units or more
         if (this->IsIdleWeAreWaitingForCompleted()) {
            this->Subroutine006F4DF0(Arg1, 1);
            this->ModifyFlag01InUnk9A(true);
            if (edi == *(Actor**)0x01310588) { // the player
               (*g_thePlayer)->SetPlayerAIDriven(false);
            }
            this->TESV_00701FE0();
            Arg1->processManager->TESV_0072A310(*g_thePlayer)
               ebx->TESV_006BB8C0(0);
            return; // cleans up smart pointer: esp1C
         }
      }
      // at 0x0070CEEE
      if (*(this->GetMiddleProcessFurnitureHandle(&esp1C)) == *g_invalidRefHandle) {
         this->TESV_0070C4B0(Arg1, 0x40, 0, 1, 0, 0);
         return; // cleans up smart pointer: esp1C
      }
      eax = this->unk04;
      NiPoint3* ebx = &eax->unk84;
      if (ebx->TESV_0058D530(NiPoint3(0, 0, 0))) {
         Arg1->processManager->TESV_0072AE70(Arg1);
         return; // cleans up smart pointer: esp1C
      }
      if (Arg1 != *g_thePlayer) {
         auto x = ebx->TESV_005E2BA0(Arg1);
         auto y = ebx->TESV_005E2D80(Arg1);
         Arg1->TESV_006ACE20(&this->unk04->unk84, y, x, 40.0, 0);
         return; // cleans up smart pointer: esp1C
      }
      Arg1->Unk_A8(ebx, 1);
      Arg1->SetYaw(this->middleProcess->unk180);
      this->TESV_0070C4B0(Arg1, 0x40, 0, 1, 0, 0);
      this->ModifyFlag02InUnk9A(true);
      return; // cleans up smart pointer: esp1C
   }
   // at 0x0070CFBF
   (*g_thePlayer)->TESV_006E8290(1);
   this->TESV_006F4DF0(Arg1, 1);
   Arg1->processManager->TESV_00721FC0(0, *g_thePlayer);
   (*g_thePlayer)->SetPlayerAIDriven(false);
   // at 0x0070CFF6
}