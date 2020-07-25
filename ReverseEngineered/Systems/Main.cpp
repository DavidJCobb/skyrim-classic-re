#include "Main.h"

namespace RE {
   DEFINE_SUBROUTINE(UInt32, GetGameMainThreadId, 0x0068C600);

   extern Main** const ptrMain = (Main**) 0x01B2E860;
   extern BStList<TESObjectREFR>* const g_pendingRefEnables  = (BStList<TESObjectREFR>*) 0x01B10C88;
   extern BStList<TESObjectREFR>* const g_pendingRefDisables = (BStList<TESObjectREFR>*) 0x01B10C80;
   extern BStList<TESObjectREFR>* const g_pendingRefGarbageCollects = (BStList<TESObjectREFR>*) 0x01B10C98;

   extern NiPointer<SceneGraph>* const g_worldSceneGraphPtr = (NiPointer<SceneGraph>*) 0x01B2E8BC;
}