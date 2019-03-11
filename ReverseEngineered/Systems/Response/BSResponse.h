#pragma once
#include "skse/GameFormComponents.h"
#include "skse/GameTypes.h"
#include "skse/Utilities.h"

namespace RE {
   class Actor;
   //
   // Observed templates:
   //  - BSResponse<BSFixedStringCI,Actor,BSFixedStringCI,DoNothingUnhandledPolicy>
   //
   // See BSResponse definition in GameFormComponents.

   class AnimatedCameraDeltaStartHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class AnimatedCameraEndHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class AnimationDrivenHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class ArrowDetachHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class HeadTrackingOffHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class HeadTrackingOnHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class MotionDrivenHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class MTStateHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };
   class PickNewIdleHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      virtual bool Handle(Actor*, void*); // 01 // signals an idle change to the AI process manager?
      // TODO
   };
   class StaggeredStopHandler : public BSResponse<BSFixedStringCI, Actor, BSFixedStringCI, DoNothingUnhandledPolicy<BSFixedStringCI>> {
      // TODO
   };


   // The actual typename is: class BSTObjectDictionary<class BSTSmartPointer<class BSResponse<class BSFixedStringCI,class Actor,class BSFixedStringCI,class DoNothingUnhandledPolicy>,struct BSTSmartPointerIntrusiveRefCount>,class BSFixedStringCI,class ResponseDefinitionMissPolicy,class PreloadResponsesInitializationPolicy>
   class Singleton01B38884 {
      public:
         virtual void Dispose(bool free); // 00

         static Singleton01B38884* GetInstance() {
            return *(Singleton01B38884**)0x01B38884;
         };

         void*  unk04 = (void*)0x010D6DD4; // 04 // VTBL: class NoInitializationPolicy<class BSTSmartPointer<class IHandlerFunctor<class Actor,class BSFixedStringCI>,struct BSTSmartPointerIntrusiveRefCount>,class BSFixedStringCI>
         UInt32 unk08;
         UInt32 unk0C;
         UInt32 unk10;
         UInt32 unk14 = 0;
         UInt32 unk18 = 0;
         UInt32 unk1C = 0;
         void*  unk20 = (void*)0x012889D8;
         UInt32 unk24;
         UInt32 unk28 = 0;
         UInt32 unk2C = 0;
         UInt32 unk30 = 0;

         MEMBER_FN_PREFIX(Singleton01B38884);
         DEFINE_MEMBER_FN(Subroutine006DA480, bool, 0x006DA480, StringCache::Ref*, void* outSmartPtr);
         DEFINE_MEMBER_FN(Subroutine0077F870, bool, 0x0077F870, void*, void* outSmartPtr); // called by Subroutine006DA480
   };
};