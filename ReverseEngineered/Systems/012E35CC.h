#pragma once
#include "skse/GameFormComponents.h"
#include "skse/GameForms.h"
#include "skse/Utilities.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
   class TESObjectCELL;

   extern constexpr uint32_t* g_cachedGrassCellRadius = (uint32_t*)0x012E5BFC;

   //
   // Locations of interest:
   //
   // Opcode at 0x008A2C09: hides grass when you open the world map
   // Opcode at 0x008A2DC7: un-hides grass, presumably when you close the world map
   //
   // Constructor for BSMultiStreamInstanceTriShape at 0x00B0A000: related to loading grass; search from 0x00464990
   //
   // Opcode at 0x00464266: calls constructor for BSGrassShaderProperty
   //
   // Notes:
   //
   // Per Nukem, the reason grass doesn't accept lights is because its shader ignores any non-ambient lights. 
   // The shader is also responsible for wind effects on grass. Per me, in theory we could coerce the game 
   // into using the default shader on grass, but first, we'd have to find where it even sets the grass 
   // shader. The BSMultiStreamInstanceTriShape class should be what's used to render grass as instanced 
   // geometry, so I expect we'd need to find where that has a NiWhateverShader ir BSWhateverShader applied. 
   // It bears reiterating that if we do manage to force grass to use the default shader, it'll lose wind 
   // effects.
   //
   // I found the constructor call for BSGrassShaderProperty; however, if I blindly replace that with a call 
   // to the BSLightingShaderProperty constructor, then the game CTDs. This happens even when I remember to 
   // get rid of the SetUnkB4 call after the constructor, which uses a virtual function that exists only on 
   // the grass shader property class. Perhaps there are other parts of the grass code which assume that 
   // there's a grass shader property?
   //
   //  - BSGrassShaderProperty::unkB8 is accessed without checking the shader property type, at 00465412
   //
   //     - If we fix this, then we stop crashing, but grass doesn't render in-game at all.
   //
   //        - BSGrassShaderProperty's constructor calls this->sub00C6F920(this->unk3C, 1). Adding that 
   //          after the tampered-with constructor call has no visible effect.
   //
   //     - If we try using BSDistantTreeShaderProperty instead of BSLightingShaderProperty, then we get the 
   //       obvious result: a crash the very instant any grass 3D is set up.
   //

   class Singleton012E35CC {
      //
      // Singleton which manages, or contributes heavily to managing, grass.
      //
      Singleton012E35CC* GetInstance() {
         return *(Singleton012E35CC**)0x012E35CC; // singleton is always accessed through here...
      };
      static constexpr Singleton012E35CC* instance = (Singleton012E35CC*)0x01B37AC0; // ...but the singleton instance is always constructed here
      //
      MEMBER_FN_PREFIX(Singleton012E35CC);
      DEFINE_MEMBER_FN(Constructor, Singleton012E35CC&, 0x004632A0);
      DEFINE_MEMBER_FN(Destructor,  void,               0x00463C10);
      DEFINE_MEMBER_FN(RefreshShaderGrassEnabledFlag, void, 0x004612A0); // rechecks pertinent INI settings and sets (shaderGrassEnabled)
      DEFINE_MEMBER_FN(Subroutine00461200, void, 0x00461200);
      DEFINE_MEMBER_FN(Subroutine00461AB0, void, 0x00461AB0);
      DEFINE_MEMBER_FN(Subroutine00462960, void, 0x00462960);
      DEFINE_MEMBER_FN(Subroutine00463510, void, 0x00463510, UInt32); // runs when ToggleGrass disables grass, among other places
      DEFINE_MEMBER_FN(Subroutine004644F0, bool, 0x004644F0, TESObjectCELL*); // checks for stuff related to a cell's "GID buffer"
      DEFINE_MEMBER_FN(Subroutine00464990, bool, 0x00464990, TESObjectCELL*); // only called if Subroutine004644F0 returns true
      DEFINE_MEMBER_FN(Subroutine00464D40, void, 0x00464D40, UInt32, UInt32); // called by AddCellGrassTask::Unk_01; worth noting that ToggleGrass to enabled will trigger the creation of AddCellGrassTasks
      //
         //
         // Computed as:
         // 
         // uint32_t eax = 1;
         // uint32_t ecx = 0x1F;
         // do {
         //    if (!ecx--)
         //       break;
         // } while ((eax *= 2) < 0x40);
         // initial_unk0C = eax;
         //
      static constexpr UInt32 initial_unk0C = 64;
      //
      UInt8  unk00;
      bool   bGenerateGrassDataFiles; // 01 // INI setting INI:Grass:bGenerateGrassDataFiles, cached by constructor
      UInt8  unk02;
      UInt8  unk03;
      UInt32 unk04;
      UInt32 unk08; // 08
      UInt32 unk0C = initial_unk0C; // 0C
      UInt32 unk10 = initial_unk0C; // 10
      UInt32 unk14 = initial_unk0C; // 14
      void*  unk18; // 18 // initialized as a pointer to 0xDEADBEEF
      UInt32 unk1C;
      void*  unk20; // 20 // malloc'd; there are (initial_unk0C) entries, each being 0x18 bytes
      UInt32 unk24;
      UInt32 unk28;
      tArray<UInt32> unk2C; // 2C // type unknown
      UInt32 unk30;
      UInt32 unk34;
      UInt32 unk38;
      NiPointer<NiNode> grassNode; // 3C // constructor sets flag 0x00000800 (Always Draw) on node // ToggleGrass toggles "culled" flag on this node
      UInt32 unk40 = 1;       // 40 // from constant at 0x01244000
      UInt32 unk44 = 1 * 1;   // 44 // square of constant at 0x01244000
      UInt32 unk48 = 1 * 128; // 48 // shifted constant at 0x01244000; NOT a flags mask
      void*  unk4C = nullptr; // 4C // malloc'd if bAllowCreateGrass is enabled
      bool   shaderGrassEnabled; // 50 // based on some INI settings
      UInt8  unk51[3];
   };
};