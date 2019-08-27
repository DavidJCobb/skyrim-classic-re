#pragma once
#include "TESForm.h"
#include "ReverseEngineered/NetImmerse/types.h"

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
         enum PartType : UInt8 { // Bethesda calls this LIMB_ENUM
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
               kFlag_IKData_BipedData     =  4, // Is the body part used for locomotion?
               kFlag_Explodable           =  8,
               kFlag_IKData_IsHead        = 16,
               kFlag_IKData_Headtracking  = 32, // Will the body part turn toward whatever the actor is headtracking?
               kFlag_ChanceToHit_Absolute = 64,
            };
            //
            float         damageMult = 1.0F;  // 00 // FO3: The damage multiplier used for damage done to this body part.
            UInt8         flags;              // 04
            PartType      type;               // 05
            UInt8         healthPercent;      // 06 // Strikes to this limb do this much bonus damage to the actor's health. Scaled by the Adjust Limb Damage perk entry point. Applied at 0x006E4BAB.
            UInt8         actorValue;         // 07 // FO3: The actor value used for the limb's health.
            UInt8         chanceToHit;        // 08 // FO3: The base percentage chance to hit this body part in VATS. // Apparently used by melee attacks? See 0079807E
            UInt8         chanceToExplode;    // 09 // FO3: chance to explode the limb into gibs
            UInt16        explodeDebrisCount; // 0A
            BGSDebris*    explodeDebris;      // 0C // type assumed from xEdit specs
            BGSExplosion* explodeExplosion;   // 10 // type assumed from xEdit specs
            float         trackingAngleMax;   // 14 // the maximum angle that this body part will rotate in order to look at what the actor is headtracking
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
            StringCache::Ref partNode;    // 00 // BPNN: node in the actor's skeleton
            StringCache::Ref vatsTarget;  // 04 // BPNT
            StringCache::Ref ikStartNode; // 08 // BPNI
            StringCache::Ref partName;    // 0C // BPTN
            StringCache::Ref goreBone;    // 10 // NAM4
            TESModel         model;    // 14
            TESModelRDT      modelRDT; // 28 // PNAM
            BPND             limbData; // 3C // BPND

            MEMBER_FN_PREFIX(Data);
            DEFINE_MEMBER_FN(Constructor, Data*, 0x0054A180);
            DEFINE_MEMBER_FN(Destructor,  void,  0x00549EA0);
            DEFINE_MEMBER_FN(Load,        bool,  0x0054A1F0, BGSLoadFormBuffer*);
         };

         Data*  parts[kPartType_Max]; // 2C - init'd to 0
         UInt32 unk40; // 40 - init'd to 0

         MEMBER_FN_PREFIX(BGSBodyPartData);
         DEFINE_MEMBER_FN(GetPart,             Data*, 0x0054A7D0, PartType);
         DEFINE_MEMBER_FN(GetPartByActorValue, Data*, 0x0054A8E0, UInt32 avIndex); // loops over parts; returns the first part to match
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
   //  - BGSBodyPartData::LoadForm doesn't seem to handle extended type values 
   //    correctly. Inspection of the code establishes that given a form (form) and 
   //    a body part entry (entry),
   //
   //       entry = new BGSBodyPartData::Data();
   //       if (!entry->Load(buffer)) {
   //          if (entry) {
   //             entry->~Data();
   //             delete entry;
   //          }
   //          return false; // if an entry fails to load, the form fails to load
   //       } else {
   //          auto type = entry->type;
   //          if (type <= kPartType_Max) {
   //             if (form->part[type]) {
   //                //
   //                // We can only have one body part of any given type. The last-
   //                // loaded one is the one we keep; the others present before it 
   //                // are discarded (and properly freed).
   //                //
   //                form->part[type]->~Data();
   //                delete form->part[type];
   //             }
   //             form->part[type] = entry;
   //          }
   //       }
   //
   //    It appears that entries with out-of-bounds type values won't get freed 
   //    (unless they fail to load, which counts as the entire form failing to 
   //    load). That's better than crashing, but memory leaks are... not ideal.
   //
   // The decapitation function doesn't let you specify the part to sever -- it's 
   // always the head -- so I'd have to duplicate the function almost in its entire-
   // ty, with the requisite changes to control the severed limb and prevent CTDs.
   //
   // ---
   //
   // Implementation ideas for dismemberment:
   //
   //  - Provide a Papyrus API to register a BodyPartData to serve as dismemberment 
   //    data for a race. (This is the only kind of association that can survive a 
   //    mod merge.) This means that a race would potentially have two BGSBodyPartData 
   //    forms associated with it: one accessible to the vanilla engine, used for 
   //    vanilla purposes including decapitation; and one that we keep track of, 
   //    where we co-opt the body part type values for our own ends (e.g. treating 
   //    "0" as "left leg" even though canonically it means "torso").
   //
   //    This in turn would allow mod authors to use Bethesda's own dismemberment 
   //    configuration.
   //
   //  - We need to see if any locational-damage-related processing still exists in 
   //    the vanilla game.
   //
   //    I would *expect* Bethesda not to strip out Fallout-exclusive dismemberment 
   //    code just because it's unused. They iterate on the same engine for Fallout 
   //    and TES: Skyrim is an upgraded New Vegas is an upgraded Oblivion, and 
   //    Fallout 4 is an upgraded Skyrim; so removing one franchise's unused code 
   //    from the other would just complicate things, no? That doesn't mean they 
   //    can't lock locational damage stuff behind an #ifdef, though, in which case 
   //    it wouldn't make it into TESV.exe.
   //
   //    If there is any locational damage stuff, it probably works like this:
   //
   //     - Perform a raycast or collision check to determine that one actor has 
   //       attacked another. By definition this means we have access to the node 
   //       on the target actor that was hit.
   //
   //     - Traverse up the node tree, checking the node and its ancestors against 
   //       all of the body part nodes cached on the actor's MiddleProcess, to 
   //       identify the body part that was hit. (In fact, this would explain why 
   //       MiddleProcess even bothers to cache those NiNode pointers.)
   //
   //     - If we find no match, then use standard damage. Otherwise, apply any 
   //       pertinent damage modifiers that weren't disabled from Skyrim's final 
   //       build, and perform any limb-related effects that weren't disabled in 
   //       Skyrim's final build.
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
   //    where the first argument is a dismemberment bit index
   //
   // Actor::Subroutine006DB530 also does stuff related to dismemberment.
   //
   // For code related to loading decapitation state from the save, check 004DEEE0.
   //
   // Actor::ResetHealthAndLimbs restores health but doesn't appear to modify limb 
   // state in any way.
   //
   // Actor::Subroutine006E6FC0 is part of the processing for an actor being hit.
   //
   //  - 006E74A0
   //
   //  - 006E57D0 determines if a swing connects with its target? returns a refr
   //
   //     - If the struck refr is an Actor, then it's obtained at 006E5A20, which 
   //       calls 004D7C90 on a NiNode returned by a raycast process. The callee 
   //       traverses up the node tree until it finds a BSFadeNode with a 
   //       TESObjectREFR* pointer. If that ref isn't an Ammo, then it is returned; 
   //       if it's an Ammo, then 004D7C90 recurses on its parent node.
   //
   //       004D7C90 is called all over the executable. If we wanted to get the 
   //       hit body part along with the hit actor, we might try replacing this 
   //       specific call with one that behaves identically while also IDing the 
   //       body part and caching that in a static variable somewhere.
   //
   //     - 006E57D0 is only called if the attacker's "forced target" doesn't 
   //       exist or is a ghost actor -- or so it seems. The actor being checked 
   //       CAN be the forced target, but maybe it can be other things, too?
   //
   //     - Testing indicates that 006E57D0 is not what we're looking for.
   //
   // 01275A6C is the bDisableAllGore setting's bool value. Might be worth looking 
   // at everything that reads it.
   //
   // How does the ShouldAttackKill function work? Surely the damage of an attack 
   // must be stored somewhere that it can check... If we can find where the damage 
   // is stored, then we can find what stores it.
   //
   //  - Analyze Actor::ShouldAttackKill.
   //
   // Subroutine 00798180 handles most or all damage calculations, including for 
   // sneak attacks, criticals, perk entry points, and a seemingly redundant kill-
   // move damage multiplier.
   //
   // Research code near 00797CBA; it looks up a body part index and is involved 
   // in the struct that also does damage calcs
   //
   //  - Searching near this turned up Actor::FindBodyPartTypeMatchingNode
   //
   // NEW PLAN:
   //
   //  - Use Papyrus to register one BGSBodyPartData as an "addendum" to another. 
   //    The addendum will hold locational damage and dismemberment data.
   //
   //  - Patch BGSBodyPartData::GetPart so that extended part indices look up 
   //    from the addendum, i.e. part 6 in DefaultBodyPartData == part 0 in the 
   //    addendum.
   //
   //  - Patch Actor::FindBodyPartTypeMatchingNode to be able to return extended 
   //    body part indices. Examine all callers to ensure we know exactly where 
   //    those indices will end up and how they will be used.
   //
   //     - Check nodes for our limbs before we check the vanilla nodes; it's 
   //       important that our Head override the vanilla Head
   //
   //  - Look for any calls to BGSBodyPartData::GetParts and verify that they 
   //    don't access by index.
   //
   //     - The sole caller is ActorProcessManager::InitBodyPartNodes, which 
   //       loops over the parts by index; we'd have to patch that function 
   //       to handle extended body parts anyway.
   //
};
