#pragma once
#include "ReverseEngineered/Shared.h"
#include "skse/Utilities.h"

class BGSTextureSet;
class NiAVObject;
class SpellItem;
namespace RE {
   class ActiveEffect;
   class Actor;
   class NiNode;
   struct NiPoint3;
   struct Struct00797220;
   class TESAmmo;
   class TESObjectREFR;
   class TESObjectWEAP;
   class TESWeather;
   class TESWorldSpace;

   enum TaskType : UInt32 {
      kTaskType_RemoveNodeFromScene = 0x05, // remove NiNode from its parent (and from the ShadowSceneNode?)
      kTaskType_10            = 0x10, // Actor::ProcessIncomingAttack
      kTaskType_34            = 0x34, // for created water ref? and three other things
      kTaskType_35            = 0x35, // related to magic; acts on a single ActiveEffect; possibly dispels an individual effect
      kTaskType_DispelSpell   = 0x5B,
      kTaskType_FireWeapon    = 0x1F,
      kTaskType_KillActor     = 0x0A,
      kTaskType_RemoveSpell   = 0x5A,
      kTaskType_SetEyeTexture = 0x5E,
   };

   struct BSTask;
   class BSTaskPool { // sizeof == 0xC
      public:
         static BSTaskPool* GetSingleton(void) {
            return *((BSTaskPool**)0x01B38308);
         }
         //
         UInt32 unk00;
         void*  unk04;
         UInt32 unk08;
         //
         MEMBER_FN_PREFIX(BSTaskPool);
         DEFINE_MEMBER_FN(SetNiGeometryTexture, UInt32, 0x006A4590, NiAVObject* geometry, BGSTextureSet* textureSet);
         DEFINE_MEMBER_FN(ProcessTaskQueue_HookTarget, void, 0x006A0B30);
         //
         // The subroutine 006A12A0 contains a switch-case on task type numbers: this is where tasks are executed.
         //
         DEFINE_MEMBER_FN(QueueBSTask,            void, 0x006A2B30, BSTask*);
         DEFINE_MEMBER_FN(QueueDispelSpellTask,   void, 0x006A3ED0, UInt32, UInt32); // used by Papyrus
         DEFINE_MEMBER_FN(QueueFireWeaponTask,    void, 0x006A2E20, TESObjectWEAP*, TESObjectREFR* shooter, TESAmmo*); // used by Papyrus and the console
         DEFINE_MEMBER_FN(QueueKillActorTask,     void, 0x006A3280, Actor* dyingActor, float, UInt32, UInt32, UInt32); // powers Actor::Kill
         DEFINE_MEMBER_FN(QueueRemoveSpellTask,   void, 0x006A3EA0, UInt32 actorRefHandle, SpellItem* spell);
         DEFINE_MEMBER_FN(QueueSetEyeTextureTask, void, 0x006A4590, UInt32, UInt32); // used by Papyrus
         DEFINE_MEMBER_FN(QueueTaskType03,      void, 0x006A2E70, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType04,      void, 0x006A2EB0, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType05,      void, 0x006A2F10, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType06,      void, 0x006A3060, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType08,      void, 0x006A30A0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType09,      void, 0x006A31F0, UInt32, UInt32, UInt32); // damage object?
         DEFINE_MEMBER_FN(QueueTaskType0B,      void, 0x006A3390, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType0C,      void, 0x006A33E0, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType0D,      void, 0x006A3440, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType0E,      void, 0x006A3480, TESObjectREFR*);
         DEFINE_MEMBER_FN(QueueTaskType0F,      void, 0x006A34C0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType10,      void, 0x006A3100, Actor*, Struct00797220*);
         DEFINE_MEMBER_FN(QueueTaskType11,      void, 0x006A3190, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType12,      void, 0x006A3500, NiNode*, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType13,      void, 0x006A3560, NiNode*); // related to doors
         DEFINE_MEMBER_FN(QueueTaskType14,      void, 0x006A35A0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType15,      void, 0x006A38F0, UInt32, UInt32, UInt32, bool);
         DEFINE_MEMBER_FN(QueueTaskType16,      void, 0x006A39E0, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType17,      void, 0x006A3940, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType18,      void, 0x006A3A30, UInt32); // used by Papyrus ObjectReference.Delete
         DEFINE_MEMBER_FN(QueueTaskType19,      void, 0x006A3A70, TESObjectREFR* victim); // related to actor decapitations; just calls Actor::Subroutine006DA980 with argument false
         DEFINE_MEMBER_FN(QueueTaskType1A,      void, 0x006A3AB0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType1B,      void, 0x006A3AF0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType1C,      void, 0x006A3B30, Actor*);
         DEFINE_MEMBER_FN(QueueTaskType1D,      void, 0x006A3C00, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType20,      void, 0x006A3C50, TESObjectREFR*, NiPoint3&, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType21,      void, 0x006A3CB0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType22,      void, 0x006A3680, NiNode*, UInt32 actuallyANumber, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType23,      void, 0x006A36D0, NiNode*, UInt32 actuallyANumber);
         DEFINE_MEMBER_FN(QueueTaskType25,      void, 0x006A3720, UInt32, UInt32 flags);
         DEFINE_MEMBER_FN(QueueTaskType26,      void, 0x006A3770, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType29,      void, 0x006A3CE0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType2A,      void, 0x006A3D20, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType2B,      void, 0x006A3D60, UInt32, UInt32); // related to water splashing
         DEFINE_MEMBER_FN(QueueTaskType2C,      void, 0x006A3DA0, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType2D,      void, 0x006A3DF0, Actor*);
         DEFINE_MEMBER_FN(QueueTaskType2E,      void, 0x006A3E30, Actor*);
         DEFINE_MEMBER_FN(QueueTaskType2F,      void, 0x006A3800, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType30,      void, 0x006A3840, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType32,      void, 0x006A35E0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType33,      void, 0x006A3620, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType34,      void, 0x006A37C0, TESObjectREFR*);
         DEFINE_MEMBER_FN(QueueTaskType35,      void, 0x006A3E70, ActiveEffect*);
         DEFINE_MEMBER_FN(QueueTaskType36,      void, 0x006A2F90, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType37,      void, 0x006A2FD0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType38,      void, 0x006A3010, TESObjectREFR* light, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType39,      void, 0x006A3240, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType3B,      void, 0x006A3F00, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType3C,      void, 0x006A2F50, UInt32); // related to arrow detach, weapon nodes, etc.
         DEFINE_MEMBER_FN(QueueTaskType3D,      void, 0x006A3F50, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType3E,      void, 0x006A3FB0, TESWeather*, bool asOverride); // used by Papyrus Weather.ForceActive
         DEFINE_MEMBER_FN(QueueTaskType3F,      void, 0x006A3FE0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType40,      void, 0x006A31C0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType41,      void, 0x006A4030, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType42,      void, 0x006A4070, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType43,      void, 0x006A40A0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType44,      void, 0x006A38B0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType45,      void, 0x006A40D0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType47,      void, 0x006A4100, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType48,      void, 0x006A4140, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType49,      void, 0x006A4180, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType4A,      void, 0x006A41B0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType4B,      void, 0x006A41F0, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType4C,      void, 0x006A42C0, void*); // arg is a ShaderReferenceEffect*?
         DEFINE_MEMBER_FN(QueueTaskType4D,      void, 0x006A4300, UInt32); // related to effectshaders
         DEFINE_MEMBER_FN(QueueTaskType4E,      void, 0x006A3B70, UInt32); // related to Actor::Unk_89
         DEFINE_MEMBER_FN(QueueTaskType50,      void, 0x006A3350, Actor*); // related to respawns, perhaps?
         DEFINE_MEMBER_FN(QueueTaskType51,      void, 0x006A3BB0, UInt32, UInt32, TESWorldSpace*);
         DEFINE_MEMBER_FN(QueueTaskType52,      void, 0x006A39A0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType53,      void, 0x006A4340, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType54,      void, 0x006A4380, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType55,      void, 0x006A43B0, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType56,      void, 0x006A4440, Actor*);
         DEFINE_MEMBER_FN(QueueTaskType57,      void, 0x006A4480, UInt32, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType58,      void, 0x006A44B0, UInt32, UInt32); // related to werewolf feeding
         DEFINE_MEMBER_FN(QueueTaskType59,      void, 0x006A44E0, UInt32);
         DEFINE_MEMBER_FN(QueueTaskType5C,      void, 0x006A43E0, UInt32, UInt32, UInt32, UInt32);
   };
   struct BSTask {
      inline BSTask(TaskType t) : type(t) {};
      //
      UInt32 type;
   };
   struct RemoveSpellTask : public BSTask {
      inline RemoveSpellTask() : BSTask(kTaskType_RemoveSpell) {};
      //
      UInt32     actorRefHandle; // 04
      SpellItem* spell; // 08
   };

   DEFINE_SUBROUTINE_EXTERN(bool, ShouldUseBSTaskPoolForTask, 0x006F3590); // several places in the game just perform a task themselves if this returns false, instead of queueing it
};
