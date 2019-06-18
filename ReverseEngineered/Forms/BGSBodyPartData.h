#pragma once
#include "TESForm.h"

namespace RE {
   //
   // So a few details here:
   //
   // "Body Parts," in Fallout 3 and onward, were the system by which the game 
   // handled VATS, the dismemberment of limbs, and the exploding of limbs into 
   // unrecognizable gibs. This class contains all of the requisite data for 
   // these functions -- damage modifiers, particle details, and so on. There is, 
   // however, one thing to note:
   //
   // Body parts have a "type," and Bethesda stripped out the types for severable 
   // limbs from Skyrim. The only types that remain are types used specifically 
   // for engine functions (mostly) unrelated to dismemberment, like dragons' 
   // ability to snatch people off the ground.
   //
   // Actors' ActorProcessManagers contain pointers to MiddleProcess objects. The 
   // MiddleProcess objects have a fixed-length list of NiNodes exactly correspond-
   // ing to these body part types. When an actor's limb is severed, the newly-
   // created node for the severed limb is written into this list (and in practice, 
   // this only works for severed heads). Accessor functions exist on ActorProcess-
   // Manager, not MiddleProcess.
   //
   class BGSBodyPartData : public TESForm { // sizeof == 0x44
      public:
         enum { kTypeID = kFormType_BodyPartData };
         enum PartType : UInt8 {
            kPartType_Torso = 0,
            kPartType_Head,
            kPartType_Eye,
            kPartType_LookAt,
            kPartType_FlyGrab,
            kPartType_Saddle,
            //
            kPartType_Max   = kPartType_Saddle,
            kPartType_Count = kPartType_Max + 1,
         };

         // Parents:
         //
         TESModel       model;       // 14
         BGSPreloadable preloadable; // 28

         // Members:
         //
         struct BPND { // sizeof == 0x54 // this subrecord gets copied out of the file verbatim
            enum Flags {
               kFlag_Severable            =  1, // Seems to be unused. It isn't set on Skyrim's default head data even though humanoids can be decapitated.
               kFlag_IKData               =  2,
               kFlag_IKData_BipedData     =  4,
               kFlag_Explodable           =  8,
               kFlag_IKData_IsHead        = 16,
               kFlag_IKData_Headtracking  = 32,
               kFlag_ChanceToHit_Absolute = 64,
            };
            //
            float         damageMult = 1.0F;  // 00 // FO3: The damage multiplier used for damage done to this body part.
            UInt8         flags;              // 04
            PartType      type;               // 05
            UInt8         healthPercent;      // 06 // FO3: The percentage of the actor's total health used as the limb's health.
            UInt8         actorValue;         // 07 // FO3: The actor value affected when the limb's health is reduced to zero (i.e. for crippled limbs).
            UInt8         chanceToHit;        // 08 // FO3: The base percentage chance to hit this body part in VATS.
            UInt8         chanceToExplode;    // 09 // FO3: chance to explode the limb into gibs
            UInt16        explodeDebrisCount; // 0A
            BGSDebris*    explodeDebris;      // 0C // type assumed from xEdit specs
            BGSExplosion* explodeExplosion;   // 10 // type assumed from xEdit specs
            float         trackingAngleMax;   // 14
            float         explodeDebrisScale; // 18
            SInt32        severDebrisCount;   // 1C // FO3: related to dismemberment
            BGSDebris*    severDebris;        // 20
            BGSExplosion* severExplosion;     // 24
            float         severDebrisScale;   // 28
            struct {
               NiPoint3 pos;
               NiPoint3 rot;
            } goreEffectsPositioning; // 2C
            TESForm*      severImpactDataSet;   // 44 // TODO: switch pointer to right form type
            TESForm*      explodeImpactDataSet; // 48 // TODO: switch pointer to right form type
            UInt8         severDecalCount;      // 4C
            UInt8         explodeDecalCount;    // 4D
            UInt16        unk4E;                // 4E
            float         limbReplacementScale; // 50
         };
         struct Data { // sizeof == 0x90
            //
            StringCache::Ref partNode;    // 00 // BPNN: node in the actor's skeleton
            StringCache::Ref vatsTarget;  // 04 // BPNT
            StringCache::Ref ikStartNode; // 08 // BPNI
            StringCache::Ref partName;    // 0C // BPTN
            StringCache::Ref unk10;		// 10
            TESModel         model;    // 14
            TESModelRDT      modelRDT; // 28 // PNAM
            BPND             limbData; // 3C // BPND

            MEMBER_FN_PREFIX(Data);
            DEFINE_MEMBER_FN(Constructor, void, 0x0054A180);
            DEFINE_MEMBER_FN(Load,        bool, 0x0054A1F0, BGSLoadFormBuffer*);
         };

         Data*  part[kPartType_Max]; // 2C - init'd to 0
         UInt32 unk40; // 40 - init'd to 0

         MEMBER_FN_PREFIX(BGSBodyPartData);
         DEFINE_MEMBER_FN(GetPart, Data*, 0x0054A7D0, PartType);
   };
   //
   // If, uh, hypothetically, I wanted to add dismemberment back into the game for 
   // fun, I would have to fully decode the decapitation process. (Simple edits are 
   // not enough; for example, patching the getter/setter calls for the body part 
   // node to pull the Torso instead of the Head leads to access-violation CTDs.) 
   // I would also have to heavily tamper with how the game stores body part nodes, 
   // since the game only has room for the six body parts used in vanilla:
   //
   //  - When Actors load, we'd need to allocate an alternate storage space for 
   //    body parts with extended type values.
   //
   //  - The following member functions on ActorProcessManager would need to be 
   //    patched to handle extended type values by going to our secondary storage 
   //    space: InitBodyPartNodes, GetBodyPartNode, SetBodyPartNode.
   //
   //  - ActorProcessManager::ClearBodyPartNodes would need similar patching, but 
   //    it's *probably* used by destructor code as well, so we may need to instead 
   //    have it delete run-time body part data entirely.
   //
   // The decapitation function doesn't let you specify the part to sever -- it's 
   // always the head -- so I'd have to duplicate the function almost in its entire-
   // ty, with the requisite changes to control the severed limb and prevent CTDs.
   //
   // ---
   //
   // Scattered findings:
   //
   // 4D5B10 sets ExtraDismemberedLimbs flags appropriately for severing a limb. How-
   // ever, the physical limb-severing happens as a result of BSTask type 0x19, which 
   // calls Actor::Subroutine006DA980(bool).
   //
   // 4D5B10 is only called by one function (for decapitation) with args:
   //
   // 1, this (actor being decapped), 2, 1, 0
   //
   // Actor::Subroutine006DB530 also does stuff related to dismemberment.
   //
   // For code related to loading decapitation state from the save, check 004DEEE0.
   //
   // Investigate Papyrus Actor.ResetHealthAndLimbs to see how it works.
   //
};
