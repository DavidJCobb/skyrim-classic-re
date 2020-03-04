#pragma once
#include "skse/GameTypes.h"
#include "skse/NiObjects.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Miscellaneous.h"
#include "ReverseEngineered/Shared.h"

class TESObjectREFR;
namespace RE {
   class BSGeometryListCullingProcess;
   class BSPortalGraphEntry;
   class BSShaderAccumulator;

   struct BSTCommonScrapHeapMessageQueue {
      virtual void Dispose(bool free); // 00
      //
      // more vtbl...

      UInt32 unk04 = 0;
      void*  unk08;
      UInt32 unk0C = 0; // ref handle?
      void*  unk10;
      HANDLE unk14; // 14 // semaphore
      UInt32 unk18 = 0; // 18
      UInt32 unk1C = 0x64; // 1C
      UInt32 unk20 = 0x006A12A0;
   };

   struct Struct00A4EE70 { // just a vtbl
      virtual void Dispose(bool free); // 00
      virtual UInt32 Unk_01(UInt32) = 0; // 01
      virtual void Unk_02(UInt32) = 0; // 02
      virtual bool Unk_03(UInt32) = 0; // 03
      virtual void Unk_04() = 0; // 04 // just calls the constructor again?
      virtual void Unk_05(UInt32) = 0; // 05
      virtual UInt32 Unk_06(UInt32, UInt32); // 06 // no-op; returns 0
   };
   struct Struct00A49940 : public Struct00A4EE70 {
      virtual void   Dispose(bool free); // 00
      virtual UInt32 Unk_01(UInt32); // 01
      virtual void   Unk_02(UInt32); // 02
      virtual bool   Unk_03(UInt32); // 03
      virtual void   Unk_04(); // 04
      virtual void   Unk_05(UInt32); // 05
      virtual UInt32 Unk_06(UInt32, UInt32); // 06
      //
      UInt32 unk04;
      UInt32 unk08; // 08
      UInt32 unk0C; // 0C
      UInt32 unk10; // 10
      UInt32 unk14 = 0; // 14
      UInt32 unk18 = 0; // 18
   };

   class Main { // sizeof == 0x128
      //
      // VTBL: 0x010CE13C
      //
      public:
         virtual void Dispose(bool free); // 00
         virtual void Unk_01(UInt32 arg1, UInt32); // 01 // if arg1 == 3, do something with the shader accumulator at unkA0; otherwise, a no-op; always returns 0
         //
         //
         // Some notes:
         //
         //  - Either unk05 or unk06 is very likely quitToMainMenuQueued, based on Oblivion RE.
         //
         //
         bool   queuedForceQuit = false; // 04 // If this is set to true, the game will force-quit to desktop ASAP. This is how the QQQ console command works.
         bool   unk05 = false; // 05 // the ForceReset console command sets this to true
         bool   unk06 = false; // 06 // the ForceReset console command sets this to true
         bool   unk07 = false; // 07
         UInt8  unk08 = 0;
         UInt8  unk09 = 0;
         bool   freezeWorldForFlycam = false; // 0A // related to ToggleFlyCam
         UInt8  unk0B;
         UInt32 unk0C;
         UInt32 unk10;
         UInt32 mainThreadID; // 14 // thread ID that the constructor ran in
         UInt32 unk18 = 0; // 18
         UInt32 unk1C = 0; // 1C
         Struct00A49940 unk20; // 20
         BSTCommonScrapHeapMessageQueue unk3C; // 3C
         Struct00A49940 unk60; // 60
         BSTCommonScrapHeapMessageQueue unk7C; // 7C
         BSShaderAccumulator* unkA0; // A0 // its unk180 member is a bool indicating whether decal rendering is enabled
         BSShaderAccumulator* unkA4; // A4
         BSShaderAccumulator* unkA8; // A8
         BSShaderAccumulator* unkAC; // AC
         BSShaderAccumulator* unkB0; // B0
         BSShaderAccumulator* unkB4; // B4
         BSShaderAccumulator* unkB8; // B8
         BSShaderAccumulator* unkBC; // BC
         UInt8  unkC0 = 0; // C0
         UInt8  unkC1;
         UInt8  unkC2;
         UInt8  unkC3;
         NiCamera* unkC4; // C4
         tArray<UInt32> unkC8;
         tArray<UInt32> unkD4;
         tArray<UInt32> unkE0;
         BSGeometryListCullingProcess* unkEC;
         BSGeometryListCullingProcess* unkF0;
         BSGeometryListCullingProcess* unkF4;
         UInt32 unkF8 = 0;
         UInt32 unkFC = 0;
         UInt32 unk100 = 0;
         UInt32 unk104 = 0;
         UInt32 unk108 = 0;
         UInt32 unk10C = 0;
         UInt32 unk110 = 0;
         UInt32 unk114 = 0;
         UInt32 unk118 = 0;
         UInt32 unk11C = 0;
         UInt32 unk120 = 0;
         UInt32 unk124 = 0;
         //
         MEMBER_FN_PREFIX(Main);
         DEFINE_MEMBER_FN(Constructor,                Main*, 0x0069BFE0, UInt32, UInt32);
         DEFINE_MEMBER_FN(ProcessReferenceStateQueue, void,  0x00515E30); // Handles pending enable/disable/garbage-collect operations for TESObjectREFRs. Called by AI linear task threads and BGSSaveLoadManager::ProcessEvents.
         DEFINE_MEMBER_FN(UpdateLODFadeOutMults,      void,  0x006919D0); // Updates three floats representing the current fade multipliers for objects, items, and actors, based on whether we're in an interior and based on the presence of any ComplexSceneMarkers.
         DEFINE_MEMBER_FN(Subroutine00691030, void, 0x00691030); // initializes the renderer?
         DEFINE_MEMBER_FN(Subroutine006910B0, void, 0x006910B0); // related to the Sky
         DEFINE_MEMBER_FN(Subroutine006910F0, void, 0x006910F0); // called by AI linear task threads; in turn, calls ProcessReferenceStateQueue
         DEFINE_MEMBER_FN(Subroutine00691930, void, 0x00691930);
         DEFINE_MEMBER_FN(Subroutine00691AB0, void, 0x00691AB0);
         DEFINE_MEMBER_FN(Subroutine006928A0, void, 0x006928A0); // forcibly exits menu mode?
         DEFINE_MEMBER_FN(Subroutine00692A90, bool, 0x00692A90, NiNode*); // related to lighting/rendering?
         DEFINE_MEMBER_FN(Subroutine006937C0, void, 0x006937C0);
         DEFINE_MEMBER_FN(Subroutine006937E0, void, 0x006937E0); // related to input? modifies unk07
         DEFINE_MEMBER_FN(Subroutine00694470, void, 0x00694470);
         DEFINE_MEMBER_FN(Subroutine006944F0, void, 0x006944F0); // related to menus
         DEFINE_MEMBER_FN(Subroutine00694550, void, 0x00694550); // related to garbage collection
         DEFINE_MEMBER_FN(Subroutine006946F0, void, 0x006946F0); // related to complex scene markers
         DEFINE_MEMBER_FN(Subroutine00694770, void, 0x00694770);
         DEFINE_MEMBER_FN(Subroutine00694880, void, 0x00694880);
         DEFINE_MEMBER_FN(Subroutine006948C0, void, 0x006948C0); // related to menus and the player camera
         DEFINE_MEMBER_FN(Subroutine00694AD0, void, 0x00694AD0);
         DEFINE_MEMBER_FN(Subroutine00694B20, void, 0x00694B20, bool);
         DEFINE_MEMBER_FN(Subroutine00694BE0, void, 0x00694BE0);
         DEFINE_MEMBER_FN(Subroutine006958F0, void, 0x006958F0, float); // advance in-game time? argument is the same as passed to Actor::AdvanceTime
         DEFINE_MEMBER_FN(Subroutine00696810, void, 0x00696810);
         DEFINE_MEMBER_FN(Subroutine00699100, void, 0x00699100, UInt32); // related to the initial load?
         DEFINE_MEMBER_FN(Subroutine0069BDF0, void, 0x0069BDF0, UInt32, UInt32, UInt32); // related to menus
         DEFINE_MEMBER_FN(Subroutine0069C930, void, 0x0069C930); // related to menus
         DEFINE_MEMBER_FN(Subroutine0069CBD0, void, 0x0069CBD0); // (mis?)identified by SKSE as BGSSaveLoadManager::ProcessEvents
   };

   DEFINE_SUBROUTINE_EXTERN(UInt32, GetGameMainThreadId, 0x0068C600);

   extern Main** const ptrMain;
   extern BStList<TESObjectREFR>* const g_pendingRefEnables;
   extern BStList<TESObjectREFR>* const g_pendingRefDisables;
   extern BStList<TESObjectREFR>* const g_pendingRefGarbageCollects;
}