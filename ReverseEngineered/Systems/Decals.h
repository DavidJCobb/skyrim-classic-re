#pragma once
#include "skse/Utilities.h"

class BSShaderAccumulator;
namespace RE {
   class BGSLoadGameBuffer;
   class BSFadeNode;
   class NiCamera;
   class TESObjectCELL;

   struct Struct0045CDB0;

   struct Struct0045CFA0 {
      Struct0045CDB0* unk00; // a
      bool   unk04; // INI:Decals:bForceAllDecals | b
      bool   unk05; // a->unkA0 > 0
      UInt16 pad06;
      UInt32 unk08; // c
      UInt32 unk0C; // d
      UInt32 unk10; // e // counter of some kind, possibly number of attempts to apply the decal
      UInt32 unk14; // f->unk00 // gets compared to max decals per frame
      UInt32 unk18 = 0;
      UInt32 unk1C; // g
      //
      MEMBER_FN_PREFIX(Struct0045CFA0);
      DEFINE_MEMBER_FN(Constructor, Struct0045CFA0&, 0x0045CFA0, Struct0045CDB0* a, UInt8 b, UInt32 c, UInt32 d, UInt32 e, UInt32 f, UInt32 g);
      DEFINE_MEMBER_FN(Subroutine0045EF60, UInt32, 0x0045EF60, NiNode*); // returns enum; 2 seems to mean "can't apply decals to this node"
   };

   struct Struct0045CDB0 {
      //
      // This most likely represents a request to emit a decal. It can't represent an actual emitted decal 
      // unless it gets copied, because it's constructed on the stack, not the heap.
      //
      NiPoint3 position; // 00 // for decals emitted from a decal emitter, position of the emitter
      NiPoint3 unk0C; // 0C // leftmost column of the decal emitter's rotation matrix, all multiplied by -1
      NiPoint3 unk18;
      UInt32   unk24; // refhandle
      NiPointer<NiNode> targetNode = nullptr; // 28
      UInt32   unk2C = 0;
      BGSTextureSet* textureSet = nullptr; // 30
      UInt32   unk34 = 0;
      UInt32   unk38 = -1;
      float    width  = 0; // 3C // random float between FormDecalData::widthMin  and FormDecalData::widthMax
      float    height = 0; // 40 // random float between FormDecalData::heightMin and FormDecalData::heightMax
      float    depth  = 0; // 44 // FormDecalData::depth // a.k.a. max range
      NiMatrix33 unk48; // for decals emitted from a decal emitter, emitter's rotation as a matrix (TESObjectREFR::GetRotationAsMatrix)
      TESObjectCELL* unk6C; // NOT initialized by constructor
      float    parallaxScale; // 70 // FormDecalData::parallaxScale
      UInt32   unk74;
      float    unk78 =  4.0F; // 78
      float    unk7C = 15.0F; // 7C
      float    unk80 = 16.0F; // 80
      float    red   = 1.0F; // 84 // float between 0 and 1 // red?
      float    green = 1.0F; // 88 // float between 0 and 1
      float    blue  = 1.0F; // 8C // float between 0 and 1 // blue?
      UInt32   unk90 = 0;
      SInt8    subtextureIndex = 0; // 94 // if the TextureSet has no subtextures, use -1; otherwise, GetRandomIntBetween(0, 4)
      UInt8    unk95 = 0; // 95
      UInt8    unk96 = 1; // 96
      bool     parallax       = false; // 97 // FormDecalData::flags
      UInt8    alphaTesting   = true;  // 98 // FormDecalData::flags
      bool     alphaBlending  = false; // 99 // FormDecalData::flags
      UInt8    parallaxPasses = 0;     // 9A // FormDecalData::parallaxPasses
      UInt8    unk9B = 0; // 9B
      UInt8    unk9C = 0; // 9C
      UInt8    unk9D = 0; // 9D
      UInt8    unk9E = 0; // 9E
      UInt8    unk9F = 0; // 9F
      UInt32   unkA0 = 0;
      //
      MEMBER_FN_PREFIX(Struct0045CDB0);
      DEFINE_MEMBER_FN(Constructor, Struct0045CDB0&, 0x0045CDB0);
   };
   DEFINE_SUBROUTINE_EXTERN(void, LoadDecalRequest, 0x005F32D0, BGSLoadGameBuffer*, Struct0045CDB0& out);

   class DecalGroup { // sizeof == 0x1C // used by ExtraDecalGroup
      public:
         bool   unk00;
         bool   unk01;
         UInt16 pad02;
         tArray<UInt32> unk04; // unknown type
         tArray<UInt32> unk10; // unknown type
         //
         MEMBER_FN_PREFIX(DecalGroup);
         DEFINE_MEMBER_FN(Constructor, DecalGroup&, 0x004DADD0, bool, bool);
   };

   class Singleton012E5B8C {
      public:
         inline Singleton012E5B8C* Get() {
            return *(Singleton012E5B8C**)0x012E5B8C;
         }
         static constexpr Singleton012E5B8C* instance = (Singleton012E5B8C*)0x01B37B20;
         //
         UInt32 unk00;
         UInt32 unk04 = 0;
         UInt32 unk08 = 0;
         UInt32 unk0C = 0;
         UInt32 unk10 = 0;
         UInt32 unk14 = 0;
         UInt32 unk18 = 0;
         UInt32 unk1C;
         tArray<NiPointer<NiRefObject>> unk20; // BSTempEffectSimpleDecal* ?
         tArray<UInt32> unk2C;
         tArray<NiPointer<NiAVObject>> unk38; // this is the only thing that's actually serialized to the savegame; these are likely the rendered decals given that we use NiNode::RemoveChild to discard them
         BSShaderAccumulator* unk44;
         NiCamera* unk48;
         UInt32 unk4C = 0; // integer
         //
         MEMBER_FN_PREFIX(Singleton012E5B8C);
         DEFINE_MEMBER_FN(Constructor, Singleton012E5B8C&, 0x0045E190);
         DEFINE_MEMBER_FN(Destructor, void, 0x0045ED30);
         DEFINE_MEMBER_FN(Save,  void, 0x0045D530, BGSSaveFormBuffer*);
         DEFINE_MEMBER_FN(Load,  void, 0x0045F310, BGSLoadGameBuffer*); // called by member function on the AI singleton 0x012E32E8
         DEFINE_MEMBER_FN(Clear, void, 0x0045EC00, bool isNotDestructor); // BSTask type 0x43; called by ClearTempEffects; clears all decals
         DEFINE_MEMBER_FN(Subroutine0045CC50, void, 0x0045CC50, UInt32, UInt8);
         DEFINE_MEMBER_FN(Subroutine0045D000, UInt32, 0x0045D000, BSFadeNode*, UInt32 one);
         DEFINE_MEMBER_FN(Subroutine0045D090, void, 0x0045D090, UInt32);
         DEFINE_MEMBER_FN(Subroutine0045D280, void, 0x0045D280); // prunes unk38 somehow
         DEFINE_MEMBER_FN(Subroutine0045D370, void, 0x0045D370, UInt32); // searches unk38 and calls 0045CC50 on elements that match some criteria related to the argument
         DEFINE_MEMBER_FN(Subroutine0045D480, void, 0x0045D480);
         DEFINE_MEMBER_FN(MakeBGSDecalNodeFor, UInt32, 0x0045D7F0, BSFadeNode*, UInt32 one);
         DEFINE_MEMBER_FN(Subroutine0045D870, UInt32, 0x0045D870, UInt32, UInt32);
         DEFINE_MEMBER_FN(Subroutine0045DB00, void, 0x0045DB00, Struct0045CDB0*, BSFadeNode* shaderPropFadeNode, NiTriBasedGeom* niNodeAsGeom);
         DEFINE_MEMBER_FN(Subroutine0045DCD0, void, 0x0045DCD0);
         DEFINE_MEMBER_FN(Subroutine0045E340, void, 0x0045E340);
         DEFINE_MEMBER_FN(Subroutine0045E520, bool, 0x0045E520, Struct0045CFA0*);
         DEFINE_MEMBER_FN(Subroutine0045EE80, void, 0x0045EE80);
         DEFINE_MEMBER_FN(Subroutine0045F1A0, void, 0x0045F1A0);
         DEFINE_MEMBER_FN(Subroutine0045F1C0, void, 0x0045F1C0, Struct0045CDB0*, bool, DecalGroup* group); // group is optional; it's not used when loading decals from the save

         //
         // Flow for emitting a decal:
         // 
         //  - A variety of callers, including the code to load decals.
         //     - Create and populate instance of Struct0045CDB0.
         //     - Singleton012E5B8C::Subroutine0045F1C0
         //        - Create instance of Struct0045CFA0.
         //        - sub0045F130 // calls next; recurses on target node children if failure code is returned
         //           - Struct0045CFA0::Subroutine0045EF60 // calls next if possible; returns a result enum
         //              - Singleton012E5B8C::Subroutine0045E520 // makes a BGSDecalNode and calls next if possible
         //                 - Singleton012E5B8C::Subroutine0045DB00 // Creates a BSTempEffectGeometryDecal and a NiNode named "Skinned Decal Node"
         //                    - ...
         //
   };
}