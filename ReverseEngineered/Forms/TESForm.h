#pragma once
#include "ReverseEngineered/Shared.h"

#include "skse/GameForms.h"
#include "skse/GameFormComponents.h"

#pragma region Form type constants
namespace RE {
   struct form_type {
      form_type() = delete;
      enum type : uint8_t {
         none = 0, // NONE
         tes4,     // TES4: File Header
         group,    // GRUP: File Group
         setting,  // GMST: Game Setting
         keyword           = 0x04, // KYWD: BGSKeywprd
         location_ref_type,        // LCRT: BGSLocationRefType
         action,                   // AACT: BGSAction
         texture_set,              // TXST: BGSTextureSet
         menu_icon,                // MICN: BGSMenuIcon
         global, // GLOB: TESGlobal
         combat_class,             // CLAS: TESClass
         faction,                  // FACT: TESFaction
         head_part,                // HDPT: BGSHeadPart
         eyes,                     // EYES: TESEyes
         race,                     // RACE: TESRace
         sound,                    // SOUN: TESSound
         acoustic_space    = 0x10, // ASPC: BGSAcousticSpace
         skill,                    // SKIL
         magic_effect,             // MGEF: EffectSetting
         script,                   // SCPT: Script
         land_texture,             // LTEX: TESLandTexture
         enchantment,              // ENCH: EnchantmentItem
         spell,                    // SPEL: SpellItem
         scroll,                   // SCRL: ScrollItem
         activator         = 0x18, // ACTI: Activator
         talking_activator,        // TACT: BGSTalkingActivator
         armor,                    // ARMO: TESObjectARMO
         book,                     // BOOK: TESObjectBOOK
         container,                // CONT: TESObjectCONT
         door,                     // DOOR: TESObjectDOOR
         ingredient,               // INGR: IngredientItem
         light             = 0x1F, // LIGH: TESObjectLIGH
         misc_item         = 0x20, // MISC: TESObjectMISC
         apparatus,                // APPA: BGSApparatus
         //
         static_scenery,           // STAT: TESObjectSTAT
         statik = static_scenery,
         //
         static_collection,        //       BGSStaticCollection
         movable_static,           // MSTT: BGSMovableStatic
         grass,                    // GRAS: TESGrass
         tree,                     // TREE: TESObjectTREE
         flora,                    // FLOR: TESFlora
         furniture,                // FURN: TESFurniture
         weapon            = 0x29, // WEAP: TESObjectWEAP
         ammo,                     // AMMO: TESAmmo
         actor_base,               // NPC_: TESNPC
         leveled_character,        // LVLN: TESLevCharacter
         key,                      // KEYM: TESKey
         potion,                   // ALCH: AlchemyItem
         idle_marker       = 0x2F, // IDLM: BGSIdleMarker
         note              = 0x30, // NOTE: BGSNote
         constructible_object,     // COBJ: BGSConstructibleObject
         projectile,               // PROJ: BGSProjectile
         hazard,                   // HAZD: BGSHazard
         soul_gem,                 // SLGM: TESSoulGem
         leveled_item,             // LVLI: TESLevItem
         weather,                  // WTHR: TESWeather
         climate,                  // CLMT: TESClimate
         shader_particle_geometry_data, // SPGD: BGSShaderParticleGeometryData
         reference_effect,         // RFCT: BGSReferenceEffect
         region,                   // REGN: TESRegion
         navmesh_info_map,         // NAVI: NavMeshInfoMap
         cell              = 0x3C, // CELL: TESObjectCELL
         reference         = 0x3D, // REFR: TESObjectREFR
         actor             = 0x3E, // ACHR: Actor / Character (previous games distinguished between Characters and Creatures)
         missile           = 0x3F, // PMIS: MissileProjectile
         arrow             = 0x40, // PARW: ArrowProjectile
         grenade,                  // PGRE: GrenadeProjectile
         beam,                     // PBEA: BeamProjectile
         flame,                    // PLFA: FlamePrjoectile
         cone,                     // PCON: ConeProjectile
         barrier,                  // PBAR: BarrierProjectile
         placed_hazard,            // PHZD: Hazard
         worldspace        = 0x47, // WRLD: TESWorldSpace
         land,                     // LAND: TESObjectLAND
         navmesh,                  // NAVM: NavMesh
         tlod,                     // TLOD
         topic,                    // DIAL: TESTopic
         topic_info,               // INFO: TESTopicInfo
         quest,                    // QUST: TESQuest
         idle,                     // IDLE: TESIdleForm
         package           = 0x4F, // PACK: TESPackage
         combat_style      = 0x50, // CSTY: TESCombatStyle
         load_screen,              // LSCR: TESLoadScreen
         leveled_spell,            // LVSP: TESLevSpell
         anio,                     // ANIO: TESObjectANIO
         water_type,               // WATR: TESWaterForm
         effect_shader,            // EFSH: TESEffectShader
         toft,                     // TOFT
         explosion,                // EXPL: BGSExplosion
         debris,                   // DEBR: BGSDebris
         imagespace,               // IMGS: TESImageSpace
         imagespace_modifier,      // IMAD: TESImageSpaceModifier
         formlist,                 // FLST: BGSListForm
         perk,                     // PERK: BGSPerk
         body_part_data,           // BPTD: BGSBodyPartData
         addon_node,               // ADDN: BGSAddonNode
         actor_value_info  = 0x5F, // AVIF: ActorValueInfo
         camera_shot       = 0x60, // CAMS: BGSCameraShot
         camera_path,              // CPTH: BGSCameraPath
         voicetype,                // VTYP: BGSVoiceType
         material_type,            // MATT: BGSMaterialType
         impact_data,              // IPCT: BGSImpactData
         impact_data_set,          // IPDS: BGSImpactDataSet
         armor_addon,              // ARMA: TESObjectARMA
         encounter_zone,           // ECZN: BGSEncounterZone
         location,                 // LCTN: BGSLocation
         message,                  // MESG: BGSMessage
         ragdoll,                  // RGDL: BGSRagdoll
         default_object_manager,   // DOBJ
         lighting_template,        // LGTM: BGSLightingTemplate
         music_type,               // MUSC: BGSMusicType
         footstep,                 // FSTP: BGSFootstep
         footstep_set      = 0x6F, // FSTS: BGSFootstepSet
         story_branch_node = 0x70, // SMBN: BGSStoryManagerBranchNode
         story_quest_node,         // SMQN: BGSStoryManagerQuestNode
         story_event_node,         // SMEN: BGSStoryManagerEventNode
         dialogue_branch,          // DLBR: BGSDialogueBranch
         music_track,              // MUST: BGSMusicTrackFormWrapper
         dialogue_view,            // DLVW
         word_of_power,            // WOOP: TESWordOfPower
         shout,                    // SHOU: TESShout
         equip_slot,               // EQUP: BGSEquipSlot
         relationship,             // RELA: BGSRelationship
         scene,                    // SCEN: BGSScene
         association_type,         // ASTP: BGSAssociationType
         outfit,                   // OTFT: BGSOutfit
         art_object,               // ARTO: BGSArtObject
         material_object,          // MATO: BGSMaterialObject
         movement_type     = 0x7F, // MOVT: BGSMovementType
         sound_descriptor  = 0x80, // SNDR: BGSSoundDescriptorForm
         dual_cast_data,           // DUAL: BGSDualCastData
         sound_category,           // SNCT: BGSSoundCategory
         sound_output,             // SOPM: BGSSoundOutput
         collision_layer,          // COLL: BGSCollisionLayer
         color,                    // CLFM: BGSColorForm
         reverb_parameters,        // REVB: BGSReverbParameters
         unk87,                    // 
         alias,                    //       BGSBaseAlias
         reference_alias,          //       BGSRefAlias
         location_alias,           //       BGSLocAlias
         active_magic_effect,      //       ActiveMagicEffect
      };
   };
   using form_type_t = form_type::type;
}
#pragma endregion

class BGSLoadFormBuffer;
class BGSSaveFormBuffer;
class BSString;
class EnchantmentItem;
class TESForm;
namespace RE {
   class BaseExtraList;
   class BSFadeNode;
   class TESObjectREFR;

   struct FormSignatureInfo { // sizeof == 0xC
      const char* asString; // signature as string
      UInt32      signature;
      UInt8       formType;
      UInt8       pad09[3];
   };
   extern FormSignatureInfo* g_formSignatureInfo; // array up to 0x87
   static DEFINE_SUBROUTINE(const char*, FormTypeToSignatureString, 0x00678120, UInt32 formType);
   //
   struct FormFactoryInfo {
      void*       formFactoryVtbl;
      const char* formTypeName; // friendly name, not internal name
   };
   extern FormFactoryInfo** g_formFactoryInfo; // array up to 0x87; entries can be nullptr, indicating that a form type isn't actually loaded

   class TESForm;
   class BaseFormComponent {
      public:
         BaseFormComponent();
         virtual ~BaseFormComponent();

         virtual void Init();
         virtual void ReleaseRefs();
         virtual void CopyFromBase(BaseFormComponent* rhs);
         //
         MEMBER_FN_PREFIX(BaseFormComponent);
         DEFINE_MEMBER_FN(LoadSubrecordStringContent, void, 0x0044F9E0, BSString* out, TESForm* thisAsForm, UInt32 bswappedSignature); // second argument is not optional and cannot be nullptr
   };
   class TESForm : public BaseFormComponent {
      public:
         operator ::TESForm*() const { return (::TESForm*) this; }
         enum { kTypeID = 0 };	// special-case
         //
         enum {
            kFormFlag_Unk00000001 = 0x00000001, // set when marking for delete, but could be more general
            kFormFlag_Unk00000002 = 0x00000002, // "modified?" // setting this on a REFR also sets it on its CELL, if the refr doesn't have flag 0x8000; setting it on a CELL sets it on the cell's parent WRLD
            //
            kFormFlag_IsDeleted   = 0x00000020, // form is marked for deletion
            kFlagPlayerKnows  = 0x000040,
            kFlagUnk_0x400    = 0x000400,
            kFormFlag_IsDisabled  = 0x000800,
            kFlagUnk_0x4000   = 0x004000, // prevents the form from being saved
            kFlag_0x010000    = 0x010000,
            kFlag_0x100000    = 0x100000, // Actor: is ignoring friendly hits
            kFlagIsMarker     = 0x800000,
         };
         enum FormFlagsACHR : UInt32 { // flags for Actors; overrides FormFlagsREFR
            kAchrFlag_TalkedToPC = 0x00000040,
         };
         enum FormFlagsCELL : UInt32 {
            kCellFlag_Persistent      = 0x00000400,
            kCellFlag_OffLimits       = 0x00020000,
            kCellFlag_DisallowWaiting = 0x00080000,
         };
         enum FormFlagsREFR : UInt32 {
            kRefrFlag_MarkedForDelete    = 0x00000020,
            kRefrFlag_HideFromLocalMap   = 0x00000040,
            kRefrFlag_HasFlames          = 0x00000080, // see the two function calls at 004E729A
            kRefrFlag_Inaccessible       = 0x00000100, // for DOOR references
            kRefrFlag_DontLightWater     = 0x00000100, // for LIGH references
            kRefrFlag_Unk00000100        = 0x00000100, // for all other references; gets modified (can be set or cleared) on all created refs after they're created
            kRefrFlag_MotionBlur         = 0x00000200, // for MSTT references
            kRefrFlag_CastsShadows       = 0x00000200, // for LIGH references
            kRefrFlag_Persistent         = 0x00000400,
            kRefrFlag_Disabled           = 0x00000800, // same flag is used in ESPs for "initially disabled"
            //
            kRefrFlag_Unk00040000        = 0x00040000, // possibly "marked for disable"
            //
            kRefrFlag_LightNeverFades    = 0x00010000, // also gets set on references when Papyrus marks them for deletion
            kRefrFlag_DontLightLandscape = 0x00020000, // for LIGH references
            //
            kRefrFlag_Unk00080000        = 0x00080000, // getter at 00401A60
            //
            kRefrFlag_IgnoringFriendlyHits = 0x00100000, // maybe actor only?
            //
            kRefrFlag_Unk00400000        = 0x00400000, // always set on ash piles when they're created; can be set on references when Papyrus marks them for deletion
            kRefrFlag_Destroyed          = 0x00800000,
            kRefrFlag_Unk01000000        = 0x01000000, // always set on ash piles when they're created
            kRefrFlag_NoAIAcquire        = 0x02000000, // for item references
            //
            kRefrFlag_Unk08000000        = 0x08000000,
            //
            kRefrFlag_DontHavokSettle    = 0x20000000,
            kRefrFlag_NoRespawn          = 0x40000000,
            kRefrFlag_Multibound         = 0x80000000,
         };
         enum FormFlagsWATR : UInt32 {
            kWatrFlag_Dangerous = 0x00020000,
         };
         //
         virtual void         Unk_04(void);		// reset/init? would leak pointers if called on a live object
         virtual void         Unk_05();		// release pointers?
         virtual bool         LoadForm(BGSLoadFormBuffer*); // 06 // Loads the form from the ESP/ESM files. Note that if the form has fields referring to other forms, those may be loaded as just UInt32 form IDs at first, in which case they'll be resolved during TESForm::InitItem.
         virtual bool         Unk_07(UInt32 arg); // 07 // no-op for Actor
         virtual bool         Unk_08(BGSLoadFormBuffer*); // 08 // for Actor, just calls LoadForm
         virtual TESForm*     Unk_09(UInt32 arg1, void * arg2);
         virtual bool         MarkChanged(UInt32 changeFlags);   // 0A
         virtual void         UnmarkChanged(UInt32 changeFlags); // 0B
         virtual bool         Unk_0C(UInt32 arg); // no-op for Actor
         virtual bool         Unk_0D(BGSSaveFormBuffer*); // 0D // possibly a "should save?" function // called by BGSSaveLoadManager::SaveGame_HookTarget+0x264 // TESObjectREFR has override; Actor doesn't
         virtual void         WriteSavedata(BGSSaveFormBuffer*); // 0E // handles actually saving the form's data // called by BGSSaveLoadManager::SaveGame_HookTarget+0x2DC if the Unk_0D call returns true
         virtual void         ReadSavedata(void* savedata); // 0F // restores form state from savedata. for TESObjectREFR*, restores extra data, flora flags and 3D state, etc.
         virtual void         Unk_10(UInt32 arg);
         virtual void         Unk_11(UInt32 arg);
         virtual void         Unk_12(UInt32 arg); // 12 // for Actor, seems to reset the actor?
         virtual void         InitItem(void); // 13 // called once all dependent forms are loaded
         virtual BGSLoadFormBuffer* Unk_14(); // 14
         virtual UInt32       GetFormType(); // 15
         virtual void         GetFormDesc(char* buf, UInt32 bufLen); // 16
         virtual bool         GetFormFlag00000040(); // 17
         virtual bool         Unk_18(); // 18
         virtual bool         IsPlayable();      // 19
         virtual bool         Unk_1A();          // 1A
         virtual bool         GetFormFlag00020000(); // 1B // if called on TESObjectREFR, retrieves the base form's flag
         virtual bool         HasWaterCurrents(); // 1C // reads the "has currents" flag for STAT and MSTT
         virtual bool         GetFormFlag00000200(); // 1D // if called on TESObjectREFR, retrieves the base form's flag
         virtual bool         GetFormFlag00000100(void); // 1E
         virtual void         SetFormFlag00000200(bool set); // 1F
         virtual bool         Unk_20(void);              // 20
         virtual void         SetFormFlag00000020(bool set); // 21
         virtual bool         Unk_22();                  // 22 // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual void         SetDeletedFlag(bool); // 23 // Just sets the flag; doesn't carry out any other tasks. // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual void         SetFormFlag00000002(bool set); // 24
         virtual void         Unk_25();
         virtual void         Unk_26(BGSLoadFormBuffer*); // 26 // loads OBND?
         virtual bool         IsInventoryItemBase(); // 27
         virtual bool         Unk_28(); // 28
         virtual bool         Unk_29(); // 29
         virtual bool         IsWaterActivator(); // 2A // returns true if the form (or its base) is a TESObjectACTI with a water type
         virtual TESObjectREFR* Unk_2B(); // 2B // Appears to be a cast of some kind. Identical to 2C for most subclasses I've checked.
         virtual TESObjectREFR* Unk_2C(); // 2C // Appears to be a cast of some kind. 2C is usually used; what's the difference between it and 2B?
         virtual UInt32       Unk_2D(); // 2D
         virtual const char*  GetFullName(UInt32 arg); // 2E
         virtual void         CopyFrom(TESForm* srcForm); // 2F // guessed
         virtual bool         Unk_30(void* arg0, UInt32 arg1, UInt32 arg2);
         virtual void         Unk_31(void* dst, UInt32 unk);
         virtual const char*  GetEditorID();            // 32 // gets an actor's short name?
         virtual bool         SetEditorID(const char*); // 33 // used to set a cell's editor ID when loading/initializing it; cells store it via ExtraEditorID
         virtual bool         Unk_34();
         virtual bool         Unk_35();
         virtual bool         Unk_36(UInt32 arg);
         virtual bool         HandleActivation(TESObjectREFR* refBeingActivated, TESObjectREFR* actorActivatingRef, UInt32 arg3, UInt32 arg4, UInt32 arg5); // 37 // TESObjectREFR::Activate calls this on the base form of the reference being activated
         virtual void         SetFormID(UInt32 newFormID, bool); // 38
         virtual const char*  Unk_39(); // 39 // This function was inserted into the VTBL after the SKSE team last mapped it.
         virtual bool         Unk_3A(); // 3A // This function was inserted into the VTBL after the SKSE team last mapped it.
         //
         // Based on a look at the VTBL and some errors in SKSE's documentation, it looks like Bethesda patched 
         // two virtual functions halfway into the VTBL -- Unk_22 and Unk_23 -- and the SKSE team didn't notice 
         // it. I've listed Unk_39 and Unk_3A here, but if we ever rename them for TESForm* OR ANY SUBCLASS, then 
         // we'll need to rename them for ALL OF THEM.
         //
         // This also means that calls to anything after Unk_21 on a non-RE pointer are probably going to be bad, 
         // unless they're on a subclass (in which case MAYBE they'll be fine?).

         inline bool IsWeapon() const { return formType == kFormType_Weapon; }
         inline bool IsAmmo()   const { return formType == kFormType_Ammo; }
         inline bool IsArmor()  const { return formType == kFormType_Armor; }

         inline bool IsTemporary() const { return (this->formID & 0xFF000000) == 0xFF000000; }

         // Bethesda removed most of the functionality from their code; this reimplements it for simple classes.
         void CopyFromEx(TESForm * rhs);

         void*    unk04;    // 04
         UInt32   flags;    // 08
         UInt32   formID;   // 0C
         UInt16   unk10;    // 10 // more flags // TESLoadScreen treats this as a counter, though?
         UInt8    formType;	// 12
         UInt8    pad13;    // 13

         MEMBER_FN_PREFIX(TESForm);
         DEFINE_MEMBER_FN(GetBodyPartsMask,   UInt32, 0x00475280);
         DEFINE_MEMBER_FN(ModifyFlag00000008, void,   0x004507F0, bool clearOrSet);
         DEFINE_MEMBER_FN(SetFormHighFlags,   void,   0x00450A60, UInt32 flagsMask, bool clearOrSet);
         //
         // Maybe avoid calling these:
         //
         DEFINE_MEMBER_FN(LoadVMAD, void, 0x00451760, BGSLoadFormBuffer*);
   };
   class TESObject : public TESForm {
      public:
         static constexpr uint32_t vtbl = 0x01083D14;
         //
         // SKSE VTBL displaced downward by two functions added to TESForm.
         //
         virtual UInt32 Unk_3B(); // 3B // no-op on TESObject; returns null
         virtual bool   Unk_3C(); // 3C // no-op on TESObject; returns false
         virtual UInt32 Unk_3D(); // 3D // no-op on TESObject; returns null
         virtual bool   Unk_3E(); // 3E // no-op on TESObject; returns false
         virtual void   Unk_3F(void* arg); // 3F // no-op on TESObject
         virtual UInt32 Unk_40(void*, void*); // 40 // no-op on TESObject; returns null
         virtual void   Unk_41(void*); // 41
         virtual bool   Unk_42(); // 42 // returns true if the "is marker" flag is set. otherwise, returns true for all form_types in the range of [texture_set, idle_marker] EXCEPT lights with models, activators with the "EditorMarker.NIF" model, and statics
         virtual bool   Unk_43(); // 43 // checks if the TESObject is a Static and is one of three executable-defined forms
         virtual bool   Unk_44(); // 44
         virtual void   Unk_45(UInt32 arg0, UInt32 arg1); // 45 // no-op on TESObject
         virtual UInt32 Unk_46(); // 46 // no-op on TESObject; returns null
         virtual UInt32 Unk_47(); // 47 // no-op on TESObject; returns null
         virtual BSFadeNode* Unk_48(void*); // 48
   };
   class TESBoundObject : public TESObject {
      public:
         struct Bound {
            SInt16	x;
            SInt16	y;
            SInt16	z;
         };

         // SKSE VTBL displaced downward by two functions added to TESForm.
         //
         virtual void   Unk_49(UInt32 arg); // 49
         virtual UInt32 Unk_4A(void); // 4A
         virtual UInt32 Unk_4B(UInt32 arg); // 4B	// return Unk_40(arg, 0);
         virtual bool   Unk_4C(UInt32 arg); // 4C
         virtual bool   Unk_4D(void* arg0, BSString* dst, void*); // 4D // steal/take string // assumes that it's working with a TESObjectREFR*
         virtual bool   Unk_4E(void* arg0, UInt8 arg1, UInt32 arg2, float arg3); // 4E
         virtual void   Unk_4F(UInt32 arg); // 4F
         virtual void   Unk_50(UInt32 arg); // 50
         virtual void   Unk_51(void); // 51
         virtual void   Unk_52(void); // 52
         virtual void   Unk_53(void); // 53

         Bound boundsMin; // 14
         Bound boundsMax; // 1A
   };
   static_assert(sizeof(TESBoundObject) == 0x20, "RE::TESBoundObject is the wrong size.");

   class TESBoundAnimObject : public TESBoundObject {
      public:
   };

   //static DEFINE_SUBROUTINE(::BGSDestructibleObjectForm*, GetBGSDestructibleObjectForm, 0x00448090, RE::TESForm*);
   DEFINE_SUBROUTINE_EXTERN(::BGSKeywordForm*, GetKeywordListFor, 0x0044B2D0, ::TESForm*);
   DEFINE_SUBROUTINE_EXTERN(BGSEquipSlot*, GetEquipSlotFor, 0x0044AB00, TESForm*);
   DEFINE_SUBROUTINE_EXTERN(BGSEquipType*, GetEquipTypeFor, 0x0044AA10, TESForm*);
   static DEFINE_SUBROUTINE(EnchantmentItem*, GetFirstEnchantmentForForm, 0x0044FF90, TESForm*, BaseExtraList*);
   static DEFINE_SUBROUTINE(const char*, GetFormName, 0x00452300, TESForm*);
   DEFINE_SUBROUTINE_EXTERN(BOOL, FormHasModel, 0x00453EF0, TESForm*); // checks the model path in TESModel and returns the length or, if there's no TESModel: checks if the form is a TESObjectREFR, and then things get complicated
   static DEFINE_SUBROUTINE(bool, FormHasName, 0x004522D0, TESForm*);
};