#pragma once
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
   class TESAmmo;
   class TESObjectWEAP : public TESBoundObject { // sizeof == 0x134
      public:
         enum { kTypeID = kFormType_Weapon };
         enum WeaponType : UInt8 {
            kType_HandToHandMelee = 0,
            kType_OneHandSword,
            kType_OneHandDagger,
            kType_OneHandAxe,
            kType_OneHandMace,
            kType_TwoHandSword,
            kType_TwoHandAxe,
            kType_Bow,
            kType_Staff,
            kType_CrossBow,
         };
         //
         // Parents:
         TESFullName         fullName;		// 020
         TESModelTextureSwap texSwap;		// 028
         TESIcon             icon;			// 044
         TESEnchantableForm  enchantable;	// 04C
         TESValueForm        value;			// 058
         TESWeightForm       weight;			// 060
         TESAttackDamageForm damage;			// 068
         BGSDestructibleObjectForm destructible;	// 070
         BGSEquipType           equipType;		// 078
         BGSPreloadable         preloadable;	// 080
         BGSMessageIcon         messageIcon;	// 084
         BGSPickupPutdownSounds pickupSounds;	// 090
         BGSBlockBashData       blockBash;		// 09C
         BGSKeywordForm         keyword;		// 0A8
         TESDescription         description;	// 0B4

         // Members:
         //
         struct GameData { // sizeof == 0x34
            enum Flags1 {
               kFlags_PlayerOnly = 0x01,
               kFlags_NPCUseAmmo = 0x02,
               kFlags_NoJamAfterReload = 0x04,
               kFlags_MinorCrime = 0x10,
               kFlags_NotUsedInNormalCombat = 0x40,
               kFlags_OverrideDamageToWeaponMult = 0x80,
               kFlags_Unknown1 = 0x100,
               kFlags_LongBursts = 0x800,
               kFlags_NonHostile = 0x1000,
               kFlags_BoundWeapon = 0x2000,
            };
            enum Flags2 {
               kFlags_Hidebackpack = 0x01,
               kFlags_Automatic = 0x02,
               kFlags_CantDrop = 0x08,
               kFlags_EmbeddedWeapon = 0x20, // "embedded" weapons don't degrade but I otherwise don't know what this means
               kFlags_Unknown2 = 0x40,
               kFlags_NotPlayable = 0x80,
            };
            //
            struct Unk00 { // sizeof == 0x1C
               float  sightFOV;            // 00
               float  unk04;               // 04 // WEAP/DNAM offset 0x40; engine default is 5.0F
               float  rumbleStrengthLeft;  // 08
               float  rumbleStrengthRight; // 0C
               float  rumbleDuration;      // 10
               float  unk14;               // 14 // WEAP/DNAM offset 0x54; engine default is 0.0F
               UInt8  numProjectiles;      // 18
               UInt8  pad19[3];
            };
            //
            Unk00* unk00;	// 00 // the game won't always create this; if all relevant subrecord values match the engine defaults, it may not get created
            float  speed;	// 04
            float  reach;	// 08
            float  minRange;	// 0C
            float  maxRange;	// 10
            float  animationMult; // 14
            float  damageToWeaponMult; // 18 // damage to weapon mult, used only if Flags1::kFlags_OverrideDamageToWeaponMult is set
            float  stagger;// 1C
            UInt32 unk20;	// 20
            SInt32 skill;	// 24 // actor value index (must be skill; validated on load: bad AVs replaced with -1)
            SInt32 resist;	// 28 // actor value index (must be between 0x39 DamageResist and 0x45 NormalWeaponsResist, inclusive; validated on load: bad AVs replaced with -1)
            UInt16 flags1;	// 2C
            UInt8  vatsChance;	// 2E
            UInt8  unk2F;	// 2F
            UInt8  unk30;	// 30
            WeaponType type;	// 31
            UInt8  flags2;	// 32
            UInt8  unk33;	// 33

            MEMBER_FN_PREFIX(GameData);
            DEFINE_MEMBER_FN(InitializeFromSubrecord,   void, 0x004A8600, void* subrecordDataSize0x64);
            DEFINE_MEMBER_FN(InitializeFromSubrecord_B, void, 0x004A8480, void* subrecordDataSize0x88); // Used under unknown circumstances; assumes a subrecord size of 0x88. xEdit has no matching definition.
         };
         struct CritData { // sizeof == 0xC
            float      critMult;			// 00
            SpellItem* critEffect;	// 04
            UInt16     critDamage;			// 08
            UInt8      effectOnDeath;		// 0A
            UInt8      pad0B;				// 0B
         };

         GameData	gameData;	// 0C0
         CritData	critData;	// 0F4
         TESForm* scopeEffect;		// 100
         BGSSoundDescriptorForm* attackSound;		// 104
         BGSSoundDescriptorForm* attackSound2D;	// 108
         BGSSoundDescriptorForm* attackLoopSound;	// 10C
         BGSSoundDescriptorForm* attackFailSound;	// 110
         BGSSoundDescriptorForm* idleSound;		// 114
         BGSSoundDescriptorForm* equipSound;		// 118
         BGSSoundDescriptorForm* unequipSound;		// 11C
         BGSImpactDataSet* impactDataSet;	// 120
         TESObjectSTAT* model;				// 124
         TESObjectWEAP* templateForm;			// 128
         BSFixedString	embeddedNode;			// 12C
         UInt32	pad130;							// 130

         float speed() { return gameData.speed; }
         float reach() { return gameData.reach; }
         float stagger() { return gameData.stagger; }
         float minRange() { return gameData.minRange; }
         float maxRange() { return gameData.maxRange; }
         UInt8 type() { return gameData.type; }
         UInt16 critDamage() { return critData.critDamage; }

         inline bool IsMelee() const noexcept { return gameData.type < kType_Bow; };
         inline bool IsGun() const noexcept { return (gameData.type == kType_Bow || gameData.type == kType_CrossBow); };

         MEMBER_FN_PREFIX(TESObjectWEAP);
         DEFINE_MEMBER_FN(Fire,                  void,   0x004AA6A0, TESObjectREFR* shooter, TESAmmo* ammo, UInt32, UInt32); // can use zero for other args
         DEFINE_MEMBER_FN(GetProjectilesPerShot, UInt8,  0x00493B40);
         DEFINE_MEMBER_FN(GetLoudness,           UInt32, 0x004A8100);
   };

};