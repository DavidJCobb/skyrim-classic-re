#pragma once

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BGSBodyPartData.h"
#include "ReverseEngineered/Systems/BGSImpactManager.h"
#include "ReverseEngineered/Systems/Decals.h"
#include "ReverseEngineered/Systems/Magic.h"
#include "ReverseEngineered/Shared.h"
#include "ReverseEngineered/Types.h"

#include "skse/GameObjects.h"
#include "skse/GameReferences.h"
#include "skse/NiTypes.h"
#include "skse/Utilities.h"

#define skyrim_re_clear_refr(v) if (v) v->handleRefObject.DecRefHandle(); v = nullptr;

class ExtraLock;
class ExtraMapMarker;
class MovementParameters;
class TESContainer;
class TESObjectREFR;
namespace RE {
   class ActorMagicCaster;
   class BGSLoadGameBuffer;
   class BGSSaveFormBuffer;
   class BSHandleRefObject;
   class InventoryEntryData;
   class TESObjectCELL; // Forward-declare instead of #including, so the compiler doesn't choke on a circular dependency
   class TESWorldSpace;
   class TESObjectREFR;
   class refr_ptr;

   DEFINE_SUBROUTINE_EXTERN(BGSDestructibleObjectForm*, GetBGSDestructibleObjectFormForRef, 0x00448090, TESObjectREFR*);
   DEFINE_SUBROUTINE_EXTERN(void, Update3DBasedOnHarvestedFlag, 0x00455BD0, TESObjectREFR*, NiNode* root); // finds and updates the NiSwitchNode in the model

   // These are likely part of a larger struct.
   //
   extern tList<TESObjectREFR*>* const g_referencesPendingDisable;
   extern tList<TESObjectREFR*>* const g_referencesPendingEnable;
   extern tList<TESObjectREFR*>* const g_referencesPendingDeletion;

   class TESObjectREFRHandleInterface {
      //
      // Basic concept:
      //
      //  - Handles and the refr_ptr struct are analogous to the smart pointers 
      //    introduced in C++11. Specifically, handles are analogous to weak 
      //    pointers, refr_ptrs are analogous to shared pointers, and the RefHand-
      //    leSystem converts between the two akin to make_shared.
      //
      //  - Handles can be used to refer to TESObjectREFRs that are not currently 
      //    loaded (whereas pointers would break when references are unloaded).
      //
      //  - Handles can be used as identifiers within savedata, whereas pointers 
      //    obviously cannot. Granted, I'm not sure why one wouldn't use form IDs.
      //
      //  - Because handles are (at least in theory) reusable, it's possible that 
      //    a handle could be released "out from under" one system and then assign-
      //    ed to another TESObjectREFR. The "validation value" built into the 
      //    handles appears to guard against this.
      //
      //  - Because indices range from 1 to 0xFFFFF, there can be 1,048,575 handles 
      //    active at any given moment. The statically-allocated array of (Entry) 
      //    structs needed to store these should consume 8388600 bytes (just under 
      //    8MB). Not bad!
      //
      // Miscellaneous notes:
      //
      //  - CreateRefHandleByREFR increases the reference's refcount.
      //
      //  - ExchangeHandleForRef and GetRefByHandle increase the reference's ref-
      //    count by virtue of using a smart pointer.
      //
      //  - The TESObjectREFR constructor calls CreateRefHandleByREFR on the newly-
      //    created reference, so it will have a handle and a non-zero refcount 
      //    immediately upon creation.
      //
      //     - Curiously, TESObjectREFRs are not allocated inside of an array. I 
      //       suppose that's needed since they can vary in size (i.e. the Actor and 
      //       PlayerCharacter subclasses), but it does lose one of the benefits of 
      //       using handles.
      //
      public:
         enum {
            kMask_Index      = 0x000FFFFF,
            kMask_IsInUse    = 0x04000000,
            kMask_ReuseCount = 0x03F00000,
            kMask_ReuseUnit  = 0x00100000,
         };
         struct Entry {
            ref_handle         refHandle; // 00
            BSHandleRefObject* refObject; // 04 // TODO: this is actually a smart pointer that modifies the BSHandleRefObject::m_uiRefCount.
         };
         //
         Entry entries[kMask_Index + 1]; // sizeof == 0x00800000 // at 0x01310638
         RE::UnknownLock01 lock;         // offset == 0x00800000 // at 0x01B10638
         //
         static constexpr SInt32* nextIndex = (SInt32*)(0x0131050C);
         static constexpr SInt32* lastIndex = (SInt32*)(0x01310510);
         //
         // Technically, this struct has no member functions; everything is a static method.
         //
         inline static TESObjectREFRHandleInterface* GetInstance() {
            return (TESObjectREFRHandleInterface*) 0x01310638;
         };
         //
         static Entry* GetEntries() { // access as result[0], result[1], ...
            return TESObjectREFRHandleInterface::GetInstance()->entries;
         };
         //
         static bool lookup_c(const ref_handle& handle, refr_ptr& p) {
            DEFINE_SUBROUTINE(bool, f, 0x004A9180, const ref_handle&, refr_ptr&);
            return f(handle, p);
         }
         static inline bool lookup(ref_handle& handle, refr_ptr& p) {
            DEFINE_SUBROUTINE(bool, f, 0x004951F0, const ref_handle&, refr_ptr&);
            return f(handle, p);
         }
         static inline bool lookup(UInt32 handle, refr_ptr& p) {
            return lookup(ref_handle(handle), p);
         }
         static bool is_handle_valid(const ref_handle&); // seen inlined in Actor::Unk_6C
         static inline void release(const ref_handle& handle) {
            DEFINE_SUBROUTINE(void, f, 0x0043C400, const ref_handle&);
            f(handle);
         }
         //
      protected:
         #pragma region Subroutines recreated for documentation
         //
         // SKSE identifies this as "LookupREFRByHandle." If the handle you pass in is 
         // invalid, then the handle will be destroyed (i.e. set to zero).
         // 
         // Don't use this! Just call the SKSE LookupREFRByHandle. This version exists 
         // for documentation purposes only, and could be wrong.
         //
         static bool ExchangeHandleForRef(ref_handle* refHandlePtr, refr_ptr&);
         //
         // SKSE identifies this as "LookupREFRObjectByHandle," and their definition 
         // states that it yields a BSHandleRefObject. This is incorrect! It yields a 
         // TESObjectREFR, and is almost exactly identical to the other function; the 
         // only apparent difference is that it doesn't modify the handle you pass in 
         // even if that handle is invalid.
         // 
         // Don't use this! Just call the SKSE LookupREFRObjectByHandle. This version 
         // exists for documentation purposes only, and could be wrong.
         //
         static bool GetRefByHandle(ref_handle* refHandlePtr, refr_ptr&);
         //
         // Don't use this! Just call the SKSE CreateRefHandleByREFR. This version 
         // exists for documentation purposes only, and could be wrong.
         //
         static ref_handle* CreateRefHandleByREFR(ref_handle* refHandlePtr, TESObjectREFR* ref); // returns refHandlePtr
         //
         // Don't use this! It's just here for documentation.
         static void Init(); // at 0x006930C0 // Called on application startup.
         //
         // Forcibly marks a handle as invalid.
         //
         // Don't use this! It's just here for documentation.
         //
         static void ReleaseHandle(const ref_handle& refHandlePtr); // at 0x0043C400
         //
         // Clears all handles that are currently active.
         //
         // Don't use this! It's just here for documentation.
         //
         static void ClearActiveHandles(); // at 0x00474B60
         //
         // Forcibly marks a handle as invalid, and sets the input handle to zero.
         //
         // Don't use this! It's just here for documentation.
         //
         static void ReleaseAndLoseHandle(ref_handle& refHandlePtr); // at 0x0079DDF0
         #pragma endregion
   };
   typedef TESObjectREFRHandleInterface RefHandleSystem;
   class BSHandleRefObject : public NiRefObject {
      public:
         enum {
            kMask_RefCount = 0x3FF,
            kFlag_HandleIsActive = 0x400,
         };
         //
         UInt32 GetRefCount() const {
            return m_uiRefCount & kMask_RefCount;
         }
         UInt32 GetRefHandle() const {
            return m_uiRefCount >> 0xB;
         };
         //
         TESObjectREFR* GetReference() const {
            return (TESObjectREFR*)(this - 0x14); // BSHandleRefObject is a multiple-parent of TESObjectREFR
         };
         //
         void DecRefHandle() {
            if ((InterlockedDecrement(&m_uiRefCount) & kMask_RefCount) == 0)
               DeleteThis();
         }
   };

   class IAnimationGraphManagerHolder {
      public:
         virtual ~IAnimationGraphManagerHolder();

         //
         // VTBL for PlayerCharacter specialization is at 010D1FB4.
         //

         virtual bool   Unk_01(UInt32);
         virtual bool   Unk_02(void* outSmartPtr);
         virtual bool   Unk_03(UInt32);
         virtual bool   Unk_04(UInt32);
         virtual bool   Unk_05(UInt32);
         virtual bool   Unk_06();
         virtual bool   Unk_07(UInt32);
         virtual bool   Unk_08(void*); // for PlayerCharacter, adds the transformDeltaEvent event sink
         virtual void   Unk_09();
         virtual bool   Unk_0A(UInt32);
         virtual void   Unk_0B(UInt32); // 0B // related to vampire softlock
         virtual void   Unk_0C(); // related to save/load
         virtual void   Unk_0D(UInt32, UInt32);
         virtual bool   Unk_0E(); // for PlayerCharacter, checks for flag 0x100 in Actor::flags2
         virtual UInt32 Unk_0F(); // for PlayerCharacter, returns INI:Animation:uVariableCacheSize; ergo, actually an unsigned int
         virtual bool   Unk_10(UInt32, UInt32);
         virtual bool   Unk_11(UInt32, UInt32);
         virtual bool   Unk_12(UInt32, UInt32);
   };
   class IMovementInterface { // sizeof == 0x4
      public:
         virtual ~IMovementInterface();
         //
         // Confirmed not to have any virtual methods of its own. Odd.
         //
         //	void** _vtbl; // 00
   };
   class IMovementState : public IMovementInterface { // sizeof == 0x4
      public:
         virtual ~IMovementState();
         //
         // the below are all pure methods; meanings are derived from ActorState
         //
         virtual UInt32 Unk_01(); // 01 // returns the formID of whatever this IMovementState is a part of?
         virtual void   Unk_02(UInt32); // 02 // arg unknown
         virtual void   GetPosition(NiPoint3* out); // 03
         virtual void   GetRotation(NiPoint3* out); // 04
         virtual float  Unk_05(); // 05 // used by IsMoving; values >= 5.0F cause IsMoving to return 0
         virtual float  Unk_06(); // 06 // used by IsTurning; can be converted into an angle?
         virtual void   Unk_07(NiPoint3* out); // 07 // used by IsMoving; possibly a velocity getter?
         virtual bool   Unk_08(UInt32 unk); // 08 // messes with the string cache?
         virtual float  Unk_09(); // 09 // possibly related to height
         virtual float  Unk_0A(); // 0A // possibly related to height
         virtual float  Unk_0B(); // 0B // possibly related to height
         virtual float  GetComputedHeight(); // 0C
         virtual float  Unk_0D(); // 0D // returns computed height * 0.75
         virtual bool   Unk_0E(MovementParameters* p); // 0E // decrements the refcount on p, and then replaces it with a new MovementParameters*
         virtual void*  Unk_0F(); // 0F
         virtual UInt32 Unk_10(UInt32); // 10 // arg unknown // return value is int, but it can only be 0 or 1?
         virtual UInt32 Unk_11(); // 11 // return value unknown
         virtual bool   Unk_12(); // 12
         virtual bool   Unk_13(); // 13 // checks flags in the actor's race
   };

   class TESObjectREFR : public TESForm {
      public:
         operator ::TESObjectREFR*() const { return (::TESObjectREFR*) this; }
         enum { kTypeID = kFormType_Reference };
         static constexpr form_type_t form_type = form_type::reference;
         //
         struct form_flag : public TESForm::form_flag {
            enum type : form_flags_t {
               marked_for_delete      = 0x00000020, // this is just another name for TESForm::form_flag::deleted, intended to clarify how "deletion" of TESObjectREFR at run-time works
               hide_from_local_map    = 0x00000040,
               has_flames             = 0x00000080, // see the two function calls at 004E729A
               inaccessible           = 0x00000100, // for DOOR references
               dont_light_water       = 0x00000100, // for LIGH references
               unknown_00000100       = 0x00000100, // for anything that isn't a DOOR or a LIGH. gets modified (can be set or cleared) on all created refs after they're created
               motion_blur            = 0x00000200, // for MSTT references
               casts_shadows          = 0x00000200, // for LIGH references
               persistent             = 0x00000400,
               disabled               = 0x00000800, // same flag is used in ESPs for "initially disabled"
               //
               light_never_fades      = 0x00010000, // also gets set on references when Papyrus marks them for deletion?
               dont_light_landscape   = 0x00020000, // for LIGH references
               unknown_00040000       = 0x00040000, // possibly "marked for disable"? maybe?
               unknown_00080000       = 0x00080000, // getter at 00401A60
               ignoring_friendly_hits = 0x00100000, // actor only?
               //
               unknown_00400000       = 0x00400000, // always set on ash piles when they're created; can be set on references when Papyrus marks them for deletion
               destroyed              = 0x00800000,
               unknown_01000000       = 0x01000000, // always set on ash piles when they're created
               no_ai_acquire          = 0x02000000, // for item references
               //
               unknown_08000000       = 0x08000000,
               //
               dont_havok_settle      = 0x20000000,
               no_respawn             = 0x40000000,
               multibound             = 0x80000000
            };
         };
         //
         virtual void	Unk_3B(); // sets the baseForm and parentCell to nullptr, among so many other things... "unload" method?
         virtual BGSLocation* GetEditorLocation(); // 3C
         virtual bool	GetEditorCoordinates(NiPoint3* outPos, NiPoint3* outRot, void** outWorldOrCell, TESObjectCELL* veryRarelyUsedFallback); // 3D // This will fail if the reference has never been moved, because starting-position data isn't attached until it's moved for the first time.
         virtual void	Unk_3E(UInt32, UInt32); // possibly SetEditorCoordinates
         virtual void	Unk_3F(bool);
         virtual void	Unk_40();
         virtual void	Unk_41(bool, UInt32, UInt32); // related to dialogue. seems to handle GMST:fAIInDialogueModewithPlayerTimer
         virtual void	Unk_42(float, UInt32); // related to destructible objects' health
         virtual bool	Unk_43(); // Checks flag 0x10000 for non-actor, non-light refs only; returns false for actors and lights.
         virtual void	Unk_44(bool); // Modifies flag 0x10000 for non-actor, non-light refs. If the flag is set to true, the reference will also be made persistent. Does nothing if the reference is not in a loaded cell.
         virtual UInt32 Unk_45(); // returns unk08 in unknown extra data type 0x8D
         virtual bool	Unk_46(); // no-op on TESObjectREFR; returns true
         virtual bool	Unk_47(); // no-op on TESObjectREFR; returns true
         virtual bool	HasKeyword(BGSKeyword*); // 48
         virtual UInt32 Unk_49(); // no-op on TESObjectREFR; returns 0
         virtual UInt32 Unk_4A(); // returns ExtraSceneData::unk08
         virtual void   Unk_4B(UInt32); // modifies ExtraSceneData
         virtual bool   Unk_4C(UInt32, UInt32); // related to ExtraSayTopicInfo
         virtual UInt32 Unk_4D(); // related to ExtraSayTopicInfo
         virtual void	Unk_4E(UInt32); // related to ExtraSayTopicInfo
         virtual void	Unk_4F(); // related to ExtraSayTopicInfo and ExtraSound
         virtual void	SetActorCause(void*); // 50 // when a projectile is created, this is passed a brand new heap-allocated Struct005ACB60*
         virtual UInt32 Unk_51(); // 51 // related to ExtraActorCause
         virtual void	GetStartingPos(NiPoint3& out); // 52
         virtual void	SetStartingPos(NiPoint3&); // 53
         virtual void	UpdateStartingPos(NiPoint3&); // 54 // same as SetStartingPos, except: we update the start rotation and location to the reference's current rotation and location; and if the argument is a zero NiPoint3, we use the reference's current position
         virtual void	Unk_55(); // 55 // related to lights
         virtual ref_handle& Unk_56(ref_handle& outHandle, TESForm* baseItem, uint32_t maybeCount, int, BaseExtraList* item, TESObjectREFR* transferTo, bool, bool); // modifies and returns outHandle // int == 3: drop?
         virtual bool	Unk_57(UInt32, UInt32, UInt32, UInt32, UInt32); // related to inventory
         virtual void	Unk_58(void*); // 58 // related to hit events?
         virtual void	Unk_59(UInt32, UInt32);
         virtual void	Unk_5A(UInt32, UInt32, UInt32, UInt32); // related to ownership and inventory
         virtual void	GetMarkerPosition(NiPoint3* pos); // 5B
         virtual ActorMagicCaster*  GetOrCreateMagicCaster(UInt32 index); // index must be between 0 and 3 inclusive; specifying 4 returns index 0; otherwise, undefined behavior
         virtual MagicTarget* GetSelfAsMagicTarget(); // 5D // Creates and attaches a NonActorMagicTarget for the reference if it is not an actor
         virtual bool   IsChild(); // 5E // used for Actor; is this also used for things that are flagged as child-suitable/child-only?
         virtual UInt32 Unk_5F();
         virtual void	Unk_60(UInt32);
         virtual BSFaceGenNiNode* GetFaceGenNiNode();
         virtual BSFaceGenNiNode* Unk_62();
         virtual BSFaceGenAnimationData* GetFaceGenAnimationData(void);
         virtual bool   SnapToHeightmappedGround(); // 64
         virtual bool   Unk_65();
         virtual void	Unk_66(); // Related to setting up 3D, including primitives' collision. // ASM at 0x004E4FF0.
         virtual void	Unk_67();
         virtual void	Unk_68();
         virtual void	Unk_69();
         virtual void*  Unk_6A(bool); // handles a lot of 3D state stuff; probably loads/creates the reference's model
         virtual void 	Unk_6B();
         virtual void	Unk_6C(NiNode*, bool); // 6C // Unk_6C(nullptr, 0) will instantly unload 3D. // Unk_6C(newNode, 1); is used for newly-created BSFadeNodes?
         virtual bool	ParentCellIsLoaded(); // 6D
         virtual void	Unk_6E();
         virtual NiNode* GetNiRootNode(bool firstPerson); // 6F
         virtual NiNode* GetNiNode() const; // 70 // Root of the skeleton (Get3D)
         virtual bool   Unk_71(); // 71 // PlayerCharacter::Unk_71 returns unk727 & 1; if true, PlayerCharacter::GetNiNode returns the firstPersonSkeleton instead
         virtual bool	Unk_72(UInt32, UInt32);
         virtual NiPoint3* GetMinBounds(NiPoint3* out); // 73
         virtual NiPoint3* GetMaxBounds(NiPoint3* out); // 74
         virtual UInt32 Unk_75();
         virtual bool	Unk_76(UInt32); // related to NiControllerManagers
         virtual bool	Unk_77(UInt32); // related to 3D
         virtual void	Unk_78(UInt32);
         virtual void	Unk_79(UInt32);
         virtual bool   Unk_7A();
         virtual bool   Unk_7B();
         virtual bool   Unk_7C();
         virtual void	Unk_7D(float);
         virtual ActorWeightModel* GetWeightModel(UInt32 weightModel); // 0 Small 1 Large
         virtual ActorWeightData*  Unk_7F();
         virtual ActorWeightModel* Unk_80(); // seen in use by the code to fire arrows from a (cross)bow.
         virtual void	SetWeightData(ActorWeightData** smartPtr); // 81
         virtual void	Unk_82(UInt32);
         virtual void	Unk_83(UInt32);
         virtual void	SetBaseForm(TESBoundObject*); // 84
         virtual void	Unk_85(void);
         virtual void	Unk_86(NiPoint3* out); // related to bhkRigidBody
         virtual void	Unk_87(UInt32);
         virtual void	Unk_88(UInt32);
         virtual void	Disable(); // 89 // ASM at 0x004E1250. Called to initially disable a reference. Calls Unk_6C(0, 0) in the process. // Does nothing to the player-actor.
         virtual void	ResetInventory(bool); // 8A
         virtual void*  Unk_8B();
         virtual void	Unk_8C(UInt32);
         virtual NiNode* Unk_8D(); // most likely returns NiNode*; some code conditionally uses it instead of GetNiNode
         virtual void*  Unk_8E();
         virtual Projectile* AsProjectile(); // returns this
         virtual bool	Unk_90(UInt32);
         virtual void	Unk_91();
         virtual void	Unk_92();
         virtual bool	Unk_93();
         virtual void	Unk_94(UInt32);
         virtual void	Unk_95(); // Actor::Unk_95 conditionally calls Actor::Subroutine006B5910, which checks the "never resets" flag on the actor's current encounter zone...
         virtual void	Unk_96(); // checks inventory and ExtraLock; for Actor, kills the actor, supplying g_invalidRefHandle for the killer handle
         virtual TESObjectCELL* GetParentOrPersistentCell(); // 97 // used by 0x004D5EB0 (TESObjectREFR::GetParentWorldspace) when this->parentCell == NULL
         virtual void   Unk_98(TESObjectCELL*); // setter for parentCell?
         virtual bool	IsDead(UInt8 unk1); // 99
         virtual void*  Unk_9A(); // 9A // related to anim note receivers
         virtual void*  Unk_9B(); // 9B // getter related to "AnimNoteReceiver" extra data
         //
         // Apparent new functions from here onward; these are also listed on Actor* and I may have more information there.
         //
         virtual bool	Unk_9C(void* arg1, float waterHeight_notSureIfHavokUnits, float);  // 9C // new function? // related to underwater ambience
         virtual bool	Unk_9D(UInt32 arg1, UInt32 arg2, UInt32 arg3); // 9D // new function? // no-op (returns false) for TESObjectREFR
         virtual TESAmmo* Unk_9E(); // 9E // new function? // no-op for TESObjectREFR // pulls from the actor's process manager
         virtual DecalGroup* GetDecalGroup(); // 9F // new function? // getter related to "DecalGroup" extra data
         virtual void	AdjustProjectileFireTrajectory(NiNode* firingFromNode, float* pitch, float* yaw, NiPoint3* firingFromPos); // A0 // new function? // allows a reference to modify the initial angle and position of a projectile it is about to fire
         virtual void	Unk_A1(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4); // A1 // new function? // no-op for TESObjectREFR

         struct LoadedState { // sizeof == 0x28
            //
            // For layout, look at TESObjectREFR::CreateLoadedStateIfMissing
            //
            tArray<UInt32> unk00; // 00
            UInt32   unk0C;	// 0C
            float    unk10;	// 10 // related to cell water levels; see subroutine 004D9FE0 // can be FLT_MIN
            float    unk14;	// 14
            UInt16   unk18;   // 18 // bitmask
            UInt16   unk1A;   // 1A // int? tested by TESObjectREFR::Subroutine004D6320
            void*    unk1C;	// 1C
            NiNode*  node;    // 20 // This is what is returned by TESObjectREFR::GetNiNode.
            UInt32   unk24;   // 24
         };
         //
         // Parents:
         BSHandleRefObject handleRefObject;	// 14
         BSTEventSink<BSAnimationGraphEvent> animGraphEventSink;  // 1C
         IAnimationGraphManagerHolder        animGraphHolder;     // 20
         //
         // Members:
         TESForm* baseForm;          // 24
         NiPoint3 rot;               // 28
         NiPoint3 pos;               // 34
         TESObjectCELL* parentCell;  // 40
         LoadedState*   loadedState; // 44
         ::BaseExtraList extraData;  // 48
         UInt16 scale;               // 50 // scale * 100
         UInt16 referenceFlags;      // 52 // flags, or possibly two UInt8s; if the latter, then unk53 = bool unloading
         //
         bool   AttachScript(const char* scriptName);
         void   CreateDetectionEvent(Actor* owner, SInt32 soundLevel);
         UInt32 CreateRefHandle(void);
         std::vector<::TESObjectREFR*> GetActivateParents(::TESForm* baseFormFilter = NULL);
         UInt32 GetChangeFlags();
         SInt32 GetCurrentDestructionStage();
         TESObjectREFR* GetDestinationDoor(); // if the current reference is a load door // the returned reference will have its refcount incremented
         void           GetDestinationDoor(refr_ptr& out);
         bool   GetEditorCoordinateData(NiPoint3* pos, NiPoint3* rot, ::TESWorldSpace** worldspace, ::TESObjectCELL** cell);
         void   GetEditorCoordinateDataAlways(NiPoint3* pos, NiPoint3* rot, ::TESWorldSpace** worldspace, ::TESObjectCELL** cell); // Return editor coordinates if any, or current coordinates otherwise
         UInt32 GetItemCount(TESForm* item); // supports all of the same things as Papyrus, and detects all of the same invalid arguments
         UInt32 GetItemCountFast(TESForm* item); // only supports item base forms; doesn't check any form types; doesn't check whether the reference is a container
         TESKey* GetKey();
         SInt32 GetLockLevel();
         float  GetMass();
         SInt8  GetMotionType();
         UInt32 GetOpenState();
         bool   HasActivateChild(TESObjectREFR*);
         bool   HasCollision();
         bool   IsActivationBlocked();
         bool   IsDestinationTeleportMarkerInAttachedCell();
         bool   IsTeleportMarkerInAttachedCell(refr_ptr& destination); // pass destination to micro-optimize
         bool   IsValidLoadDoor(bool quick = false);
         void   KnockAreaEffect(float magnitude, float radius);
         void   MoveTo(UInt32* pTargetHandle, void* parentCell, void* worldSpace, NiPoint3* postion, NiPoint3* rotation); // MoveRefrToPosition wrapper with some additions
         bool   MoveToMyEditorLocation(bool native = false);
         void   Reload3D();
         void   SetMotionType(uint8_t motionType, bool simulateImmediately, bool markChanged = true); // asynch; careful about this

         inline Actor* AsActor() const noexcept { return (this->formType == form_type::actor) ? (Actor*)this : nullptr; }

         inline UInt32& GetOrCreateRefHandle(UInt32& handle) {
            return *(UInt32*)&CALL_MEMBER_FN(this, GetOrCreateRefHandle)(*(ref_handle*)(&handle));
         }
         inline ref_handle& GetOrCreateRefHandle(ref_handle& handle) {
            return CALL_MEMBER_FN(this, GetOrCreateRefHandle)(handle);
         }

         inline bool IsEnabled() const noexcept { return !(this->flags & form_flag::disabled); }

         static TESObjectREFR* make(); // creates a TESObjectREFR with a refhandle and a refcount of 1
         
         MEMBER_FN_PREFIX(TESObjectREFR);
         DEFINE_MEMBER_FN(Constructor, TESObjectREFR&, 0x004E6930);
         DEFINE_MEMBER_FN(Activate,                void,             0x004E4230, TESObjectREFR* activatedBy, UInt32 Arg2_papyrusUses0, UInt32 Arg3_papyrusUses0, UInt32 Arg4_papyrusUses1, bool defaultOnly);
         DEFINE_MEMBER_FN(IsLimbSevered,           bool,             0x004D5B90, BGSBodyPartData::PartType limb);
         DEFINE_MEMBER_FN(ClearDestruction,        void,             0x00449630);
         DEFINE_MEMBER_FN(ContainsQuestItem,       bool,             0x004D6020);
         DEFINE_MEMBER_FN(CountItemTypes,          uint32_t,         0x004D9F40, bool useDataHandlerInsteadOfInventory, bool includeNonPlayableItems);
         DEFINE_MEMBER_FN(CreateLoadedStateIfMissing, void, 0x004E5AA0);
         DEFINE_MEMBER_FN(DoesRespawn,             bool,             0x004D5270); // always false for created refs; checks base form flags for NPCs and containers, and the reference's NoRespawn form flag otherwise
         DEFINE_MEMBER_FN(Enable,                  void,             0x004E0E30); // internal. you should call the virtual method instead
         DEFINE_MEMBER_FN(GetBaseContainerData,    TESContainer*,    0x004D4A30); // returns &(this->baseForm.container) for NPCs and container references
         DEFINE_MEMBER_FN(GetBodyBodyPartIndex,    SInt32,           0x004D6660); // returns a body part index (for Body) pulled from the race data, or -1 if the reference isn't a valid actor
         DEFINE_MEMBER_FN(GetCurrentEncounterZone, BGSEncounterZone*, 0x004EA990); // checks extra-data, parent cell, and parent world, preferring the first found non-null
         DEFINE_MEMBER_FN(GetCurrentLocation,      BGSLocation*,     0x004D83C0);
         DEFINE_MEMBER_FN(GetDistance,             float,            0x004D7ED0, TESObjectREFR* other, bool evenIfDisabled, bool oftenFalse);
         DEFINE_MEMBER_FN(GetDistanceSquared,      float,            0x004D7ED0, TESObjectREFR* other, bool evenIfDisabled, bool oftenFalse);
         DEFINE_MEMBER_FN(GetExtraMultiBoundRef,   TESObjectREFR*,   0x004EA490); // if A uses B as its multibound ref, then A will render from B's roombound regardless of its own coordinates
         DEFINE_MEMBER_FN(GetFormWeight,           float,            0x004EA1A4); // == GetFormWeight(this->baseForm);
         DEFINE_MEMBER_FN(GetHealth,               float,            0x004E9F90);
         DEFINE_MEMBER_FN(GetHealthPercent,        float,            0x004EA050);
         DEFINE_MEMBER_FN(GetInventoryEntryAt, InventoryEntryData*, 0x004D9EE0, uint32_t index, bool useDataHandlerInsteadOfInventory); // (index) is relative to (CountItemTypes) // returns a shallow copy: new entry data, but pointers to the BaseExtraLists of the others
         DEFINE_MEMBER_FN(GetLinkedRef,            TESObjectREFR*,   0x004EA4B0, BGSKeyword*);
         DEFINE_MEMBER_FN(GetNiTransform,          void,             0x004D5380, NiTransform& out); // exports position, rotation, and size
         DEFINE_MEMBER_FN(GetOrCreateRefHandle, ref_handle&, 0x006BD6C0, ref_handle&); // <-- Modifies *out; returns out. Calls CreateRefHandleByREFR if the reference's refcount is non-zero.
         DEFINE_MEMBER_FN(GetScale,                float,            0x004D5230);
         DEFINE_MEMBER_FN(GetShieldBodyPartIndex,  SInt32,           0x004D6630); // returns a body part index (for Shield) pulled from the race data, or -1 if the reference isn't a valid actor
         DEFINE_MEMBER_FN(GetWeight,               float,            0x004EA180);
         DEFINE_MEMBER_FN(GetReferenceName,        const char*,      0x004DE820);
         DEFINE_MEMBER_FN(GetRotationAsMatrix,     NiMatrix33*,      0x004D5300, NiMatrix33* out);
         DEFINE_MEMBER_FN(GetTriggerObjectCount,   SInt32,           0x004DFB90);
         DEFINE_MEMBER_FN(GetWorldspace,           TESWorldSpace*,   0x004D5EB0);
         DEFINE_MEMBER_FN(HasAnimations, bool, 0x004E88B0); // tests animation graph holder and, if necessary, searches for any NiController anywhere in the reference's node
         DEFINE_MEMBER_FN(HasEffectKeyword,        bool,             0x004D57A0, BGSKeyword*);
         DEFINE_MEMBER_FN(HasWaterCurrents,        bool,             0x006D66F0); // base form IsWaterActivator or HasWaterCurrents
         DEFINE_MEMBER_FN(IsDisabledDeletedOrDestroyed, bool, 0x004D6280);
         DEFINE_MEMBER_FN(IsLocked,                bool,             0x004EB5B0); // if the reference is a teleport door, this checks the door on the other side, too
         DEFINE_MEMBER_FN(IsOffLimits,             bool,             0x004DA760);
         DEFINE_MEMBER_FN(IsPerchFurniture,        bool,             0x004E9F20);
         DEFINE_MEMBER_FN(IsQuestObject,           bool,             0x004D5140);
         DEFINE_MEMBER_FN(ModifyLoadedStateFlagsUnk18, void, 0x004D48F0, UInt16 flags, bool clearOrSet);
         DEFINE_MEMBER_FN(ModifyPersistentFlag,    void,             0x004D50A0, bool setTo);
         DEFINE_MEMBER_FN(MoveToMyEditorLocation,  bool,             0x004E6270, TESObjectCELL*, UInt32); // both arguments are zeroes?
         DEFINE_MEMBER_FN(SetDestroyed,            void,             0x00450E30, bool);
         DEFINE_MEMBER_FN(SetDisabledFlag,         void,             0x00450FB0, bool); // sets disabled flag and marks changed; DOES NOT ACTUALLY DISABLE THE REFERENCE
         DEFINE_MEMBER_FN(SetEncounterZone,        void,             0x004EA9E0, BGSEncounterZone*); // also sets the reference's EXTRA_ENCOUNTER_ZONE changeflag
         DEFINE_MEMBER_FN(SetExtraMultiBoundRef,   void,             0x004EB100, TESObjectREFR*); // validates the other ref's base form and ensures you don't set a ref's multibound ref to itself
         DEFINE_MEMBER_FN(SetOwner,                void,             0x004EC310, void*); // Argument is any form that is a valid owner, e.g. TESFaction. This function sets ownership on the ref; if the ref is a load door, its matching door is also altered.
         DEFINE_MEMBER_FN(SetPitch,                void,             0x004DC9D0, float); // X // Does not honor actor-specific limitations like the race "immobile" flag. Does call Unk_54 for actors, though. Sets change-flag 2 (MOVE).
         DEFINE_MEMBER_FN(SetRoll,                 void,             0x004DCA60, float); // Y // Does not honor actor-specific limitations like the race "immobile" flag. Does call Unk_54 for actors, though. Sets change-flag 2 (MOVE).
         DEFINE_MEMBER_FN(SetYaw,                  void,             0x004DCAF0, float); // Z // Does not honor actor-specific limitations like the race "immobile" flag. Does call Unk_54 for actors, though. Sets change-flag 2 (MOVE).
         DEFINE_MEMBER_FN(SetUpCollisionVolume3D,  void,             0x004E2910); // listed for research purposes; probably shouldn't call it directly. is called for [STAT]CollisionMarker refs with ExtraPrimitive data
         DEFINE_MEMBER_FN(SetUpTriggerVolume3D,    void,             0x004E3CE0); // listed for research purposes; probably shouldn't call it directly. is called for Activators with ExtraPrimitive data
         //
         // 4D9CF0 returns true if:
         //  - The reference is temporary (form ID starts with 0xFF)
         //  - The reference's base form is not SCOL, TREE, FLOR, FURN, or STAT
         //  - The reference's base form is STAT, and the base form's Unk_43() returns true
         //     - I don't fully understand Unk_43(), but I know it checks the reference's 
         //       health and its destructible state.
         // 
         // Used by SetPos, and by Papyrus MoveTo and MoveToMyEditorLocation: they all refuse to move 
         // references if this is false.
         //
         DEFINE_MEMBER_FN(Subroutine004D9CF0,   bool,             0x004D9CF0);
         DEFINE_MEMBER_FN(Subroutine00450F60,   void,             0x00450F60, UInt32);
         DEFINE_MEMBER_FN(Subroutine004D6140,   void,             0x004D6140, char*);      // <-- Sets non-persistent activate prompt? I've seen it used to set "Open" or "Close".
         DEFINE_MEMBER_FN(Subroutine004D7D20,   TESModel*,        0x004D7D20);
         DEFINE_MEMBER_FN(Subroutine004D9A80,   const char*,      0x004D9A80); // returns a model path from 4D7D20, or nullptr
         DEFINE_MEMBER_FN(Subroutine004DBDB0,   void,             0x004DBDB0, TESForm* baseTorch, bool isShieldBodyPart);
         DEFINE_MEMBER_FN(Subroutine004EC4E0,   void,             0x004EC4E0, UInt32, UInt32); // <-- related to "used" and "reserved" furniture markers
         DEFINE_MEMBER_FN(SetFlag00000002State, void,             0x00450ED0, bool);       // <-- Setter for flag 0x02, wrapped in TLS stuff.
         DEFINE_MEMBER_FN(SetPosition,          void,             0x004DCBA0, NiPoint3*);  // <-- Does something with worldspace data, too. DOES NOT UPDATE THE REFERENCE'S LOADED 3D.
         DEFINE_MEMBER_FN(SetRotation,          void,             0x004DC8F0, NiPoint3*);  // <-- Does something with loadedState, but I'm not sure if it updates the loaded 3D. Has special handling for characters.
         DEFINE_MEMBER_FN(StartsDead,           bool,             0x004D4E60);             // <-- returns true if the reference is an actor that starts dead
         DEFINE_MEMBER_FN(Subroutine004E07D0,   void,             0x004E07D0);             // <-- Called as part of the Reset3DState console command; seems to do the bulk of the work there.
         DEFINE_MEMBER_FN(Subroutine004E0E30,   void,             0x004E0E30);             // <-- Apparently enables a reference. If called after Unk_89, reloads 3D.
         DEFINE_MEMBER_FN(GetExtraLock,         ExtraLock*,       0x004EB5B0);             // <-- Gets the lock data on this reference. If this reference is a teleport door and has no lock, gets the lock on the other end (if any).
         DEFINE_MEMBER_FN(GetExtraMapMarker,    ExtraMapMarker*,  0x004EA3F0);
         DEFINE_MEMBER_FN(GetParentWorldspace,  TESWorldSpace*,   0x004D5EB0);             // <-- Return type confirmed. Purpose assumed based on usage in subroutine 0x0090CC60.
         DEFINE_MEMBER_FN(Subroutine004DFE80,   void,             0x004DFE80, bool);       // <-- Unknown. Not related to un-/re-loading 3D, though. Usually takes false as an argument.
         //
         DEFINE_MEMBER_FN(GetExtraEnableStateChildren,      RE::ExtraEnableStateChildren::Entry*, 0x004EA870);
         DEFINE_MEMBER_FN(GetExtraEnableStateParentFlag1,   bool, 0x004EA860);

         DEFINE_MEMBER_FN(SeverLimb, void, 0x004D5B10, SInt32 bodyPartType, TESForm* maybeKiller, SInt32, SInt32 bodyPartType_unkFallback, UInt8); // also sets the ExtraDismemberedLimbs::killingBlowLimb
   };
   
   class refr_ptr { // smart pointer for TESObjectREFRs
      protected:
         RE::TESObjectREFR* ref = nullptr;
         inline void _inc() {
            if (ref)
               ref->handleRefObject.IncRef();
         };
         inline void _dec() {
            if (ref)
               ref->handleRefObject.DecRefHandle();
         };
      public:
         refr_ptr() {};
         refr_ptr(RE::TESObjectREFR* a) : ref(a) { _inc(); }
         refr_ptr(const refr_ptr& a) : ref(a.ref) { _inc(); }
         refr_ptr(const UInt32& refHandle) { // does NOT exchange the handle; uses a copy
            this->set_from_handle(refHandle);
         }
         //
         RE::TESObjectREFR* operator->(); // implies comparison operators with pointers, because it allows for an implicit cast
         operator RE::TESObjectREFR*() const;
         refr_ptr& operator=(RE::TESObjectREFR* rhs);
         refr_ptr& operator=(const refr_ptr& rhs);
         inline refr_ptr& operator=(::TESObjectREFR* rhs) { return *this = (RE::TESObjectREFR*)rhs; }
         inline refr_ptr& operator=(std::nullptr_t rhs) { return *this = (RE::TESObjectREFR*) rhs; }
         //
         /*//
         inline bool operator==(const refr_ptr& rhs) { return this->ref == rhs.ref; }
         inline bool operator==(const std::nullptr_t&) { return this->ref == nullptr; }
         inline bool operator==(const RE::TESObjectREFR* rhs) { return this->ref == rhs; }
         inline bool operator==(const ::TESObjectREFR* rhs) { return this->ref == (RE::TESObjectREFR*) rhs; }
         inline bool operator==(const void* rhs) { return this->ref == (RE::TESObjectREFR*) rhs; }
         //*/
         //
         bool operator!();
         //
         ~refr_ptr();
         //
         TESObjectREFR* abandon() noexcept; // stops refcounting the pointer, and returns it
         TESObjectREFR* copy_bare(); // increments the refcount and then returns the bare pointer
         inline TESObjectREFR* get() const noexcept { return this->ref; };
         inline ::TESObjectREFR* get_base() const noexcept { return (::TESObjectREFR*) this->ref; };
         inline void set_from_already_incremented(TESObjectREFR* a) noexcept { this->ref = a; };
         void set_from_handle(const UInt32 handle); // does NOT exchange the handle; uses a copy
         void set_from_handle(UInt32* handle); // exchanges the handle
         inline void swap(refr_ptr& other) noexcept {
            auto a = this->ref;
            auto b = other.ref;
            other.ref = a;
            this->ref = b;
         };
         inline long int use_count() const {
            if (!ref)
               return 0;
            return ref->handleRefObject.GetRefCount();
         };

         inline void set_from_handle(const uint32_t handle) { this->set_from_handle((UInt32)handle); }
         inline void set_from_handle(uint32_t* handle) { this->set_from_handle((UInt32*)handle); }
   };
};