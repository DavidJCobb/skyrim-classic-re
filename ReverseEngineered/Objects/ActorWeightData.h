#pragma once
#include <cstdint>
#include <string>
#include "skse/Utilities.h"
#include "ReverseEngineered/Forms/FormComponents/BGSBipedObjectForm.h"
#include "ReverseEngineered/NetImmerse/types.h"
#include "ReverseEngineered/Objects/SimpleAnimationGraphManagerHolder.h"
#include "ReverseEngineered/Systems/BSResource.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
   class Actor;
   class BGSBodyPartData;
   class BGSTextureSet;
   class NiAVObject;
   class NiNode;
   class TESForm;
   class TESModelTextureSwap;
   class TESNPC;
   class TESObjectARMA;
   class TESObjectARMO;
   class TESObjectREFR;
   class TESRace;

   class ActorWeightData { // sizeof >= 0xA8C
      public:
         using body_part_index   = BGSBipedObjectForm::body_part_index;
         using body_part_index_t = BGSBipedObjectForm::body_part_index_t;

         struct BodyPart { // sizeof == 0x20
            TESForm*       item;  // 00
            TESObjectARMA* addon; // 04
            TESModelTextureSwap* model; // 08
            BGSTextureSet* textureSet; // 0C
            NiPointer<NiNode> renderedArmor; // 10 // created at run-time for the armor; node name format is " (%08X)[%d]/ (%08X)[%d] [%d%]" given: form ID; unknown; form ID; unknown; weight from 0 to 100
            NiPointer<SimpleAnimationGraphManagerHolder> unk14; // seen: TailAnimationGraphManagerHolder*
            UInt8    unk18;
            UInt8    pad19[3];
            LoadedMeshResourcePtr unk1C; // gets fed to something near/on BSResource::EntryDB<BSModelDB::DBTraits>

            MEMBER_FN_PREFIX(BodyPart);
            DEFINE_MEMBER_FN(CopyAssign, BodyPart&, 0x0046DF90, BodyPart& other); // BodyPart& operator=(BodyPart& other);
            DEFINE_MEMBER_FN(Reset,      void,      0x0046D930);

            inline BodyPart& operator=(BodyPart& other) {
               CALL_MEMBER_FN(this, CopyAssign)(other);
               return *this;
            };
         };

         struct IdentifyDominantArmorTypeVisitor {
            BodyPart* shield     = nullptr; // 00
            BodyPart* body       = nullptr; // 04
            UInt32    foundLight = 0;       // 08
            UInt32    foundHeavy = 0;       // 0C

            MEMBER_FN_PREFIX(IdentifyDominantArmorTypeVisitor);
            DEFINE_MEMBER_FN(Visit,            bool,   0x006E12C0, BodyPart*); // return true to keep looping; false to break out of the loop early
            DEFINE_MEMBER_FN(GetResultAVIndex, SInt32, 0x006E1B00); // call after the visitor has run
         };

         volatile SInt32 refCount; // 00
         NiNode*  npcRootNode; // 04
         BodyPart bodyParts[0x2A]; // 08 // index is a body_part_index
         BodyPart unk548[0x2A]; // 548 // testing indicates that this IS NOT used for race switches or as a queue
         UInt32   ownerHandle; // A88 // refHandle

         MEMBER_FN_PREFIX(ActorWeightData);
         DEFINE_MEMBER_FN(Constructor, ActorWeightData&, 0x0046D9B0, Actor* owner, NiNode* ownerNode_optional);
         DEFINE_MEMBER_FN(Destructor,       void, 0x0046DAA0);
         DEFINE_MEMBER_FN(StoreRootNodeFor, void, 0x0046BF40, NiNode* ownerNode);
         DEFINE_MEMBER_FN(UpdateWeightData, void, 0x0046D690);
         // DEFINE_MEMBER_FN(Unk_02, void, 0x004145F0);

         DEFINE_MEMBER_FN(BodyPartHasFacegenHeadModelFlag, bool, 0x0046AE90, body_part_index_t);
         DEFINE_MEMBER_FN(GetBodyByWhichIMeanTorso,  BodyPart*, 0x0046C1D0);
         DEFINE_MEMBER_FN(GetShield,                 BodyPart*, 0x0046C130);
         DEFINE_MEMBER_FN(IdentifyDominantArmorType, void,      0x006E1930, IdentifyDominantArmorTypeVisitor*);

         DEFINE_MEMBER_FN(CreateArmorNode, NiAVObject*, 0x0046F0B0, NiNode* maybeParent, body_part_index_t, bool isFirstPerson, UInt32 arg4_zero, UInt32 arg5_zero);
         DEFINE_MEMBER_FN(InstallArmor,    void,        0x00470050, float overrideWeight, UInt32 zero); // seems responsible for updating visuals
         DEFINE_MEMBER_FN(InstallWeapon,   void,        0x0046F870, TESForm*, UInt32);
         
         DEFINE_MEMBER_FN(Subroutine0046C270, void, 0x0046C270, body_part_index_t);
         DEFINE_MEMBER_FN(RemoveBodyPart3D,   void, 0x0046C8A0, BodyPart* part, bool alsoClearItemAndAddon, TESModelTextureSwap* alwaysNullptr);
         DEFINE_MEMBER_FN(Subroutine0046D0B0, void, 0x0046D0B0, NiNode*, UInt32 bodyPartIndex);  // performs some process on a 3D node tree
         DEFINE_MEMBER_FN(Subroutine0046D250, void, 0x0046D250, body_part_index_t, bool, bool);
         DEFINE_MEMBER_FN(Subroutine0046D370, void, 0x0046D370, body_part_index_t);
         DEFINE_MEMBER_FN(Subroutine0046D570, void, 0x0046D570, NiNode* bodyPartNode, UInt32 bodyPartIndexAlt, UInt32 bodyPartIndex); // updates visibility of partitions in the node's BSDismemberSkinInstance / geometry
         DEFINE_MEMBER_FN(Subroutine0046D700, void, 0x0046D700, TESObjectARMA* addon);
         DEFINE_MEMBER_FN(Subroutine0046D750, bool, 0x0046D750, body_part_index_t, void*);
         DEFINE_MEMBER_FN(Subroutine0046DC10, bool, 0x0046DC10, body_part_index_t, TESObjectREFR* myActor); // update pertinent animation graph?
         DEFINE_MEMBER_FN(Subroutine0046E1A0, void, 0x0046E1A0); // loops over body slots; calls 0046DC10 on any that have no unk14, passing the unkA88 ref as arg2
         DEFINE_MEMBER_FN(Subroutine0046E260, void, 0x0046E260, TESRace* race, TESObjectARMO* skin, bool isFemale); // reset body and apply skin? // if the actor has 3D, copies (bodyParts) to (unk548) and resets (bodyParts); else, resets both
         DEFINE_MEMBER_FN(Subroutine0046E4E0, void, 0x0046E4E0, TESObjectARMO* armor, TESObjectARMA* addon, TESModelTextureSwap* model, BGSTextureSet* textureSwap);
         DEFINE_MEMBER_FN(Subroutine0046F4E0, void, 0x0046F4E0, UInt32, UInt32, body_part_index_t);
         DEFINE_MEMBER_FN(Subroutine0046FD10, void, 0x0046FD10, TESForm* baseTorch, bool isShieldBodyPart);

         static void makeArmorNodeName(std::string& out, TESNPC* wearer, TESForm* baseItem, TESObjectARMA* addon, float weight);
   };
   typedef ActorWeightData Struct0046D9B0;
   DEFINE_SUBROUTINE_EXTERN(NiAVObject*, CreateWeaponNode, 0x0046F530, TESModelTextureSwap*, UInt32 bodyPartIndex, Actor*, NiPointer<NiNode>& bodyPartSlotRenderedArmorPtr, NiNode* rootNode);
}