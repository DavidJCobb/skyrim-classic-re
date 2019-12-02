#pragma once
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BaseForms/TESActorBase.h"
#include "skse/GameObjects.h"

namespace RE {
   class ActorWeightData;
   class InventoryEntryData;
   class TESObjectREFR;

   class TESNPC : public TESActorBase { // sizeof == 0x164
      public:
	      virtual void Unk_56(TESForm*); // set combat style?
         // ...?

         struct FaceMorphs {
            float  option[19];
            UInt32 presets[4];
         };
         struct HeadData {
            BGSColorForm*  hairColor; // HCLF
            BGSTextureSet* headTexture; // Only seems to apply to the player
         };
         //
         // Parents:
         //
         TESRaceForm race;	// 0C0
         BGSOverridePackCollection overridePacks;	// 0C8
         BSTEventSink<void*> menuOpenCloseEvent;	// 0DC - MenuOpenCloseEvent
         //
         // Members:
         //
         UInt8 skillValues[0x12];  // 0E0 - init'd to 5 // DNAM
         UInt8 skillOffsets[0x12]; // 0F2 - init'd to 0 // DNAM
         UInt16 health;  // 104 // DNAM
         UInt16 magicka; // 106 // DNAM
         UInt16 stamina; // 108 // DNAM
         UInt16 farAwayModelDistance; // 10A // DNAM // loaded as a float, yes, but clamped to 65535 and kept as a UInt16
         TESClass* npcClass; // 10C // CNAM
         HeadData* headData;		// 110
         BGSListForm* giftFilter; // 114 // GNAM
         TESCombatStyle*	combatStyle;// 118
         UInt32 unk11C;			// 11C
         TESRace* overlayRace;	// 120
         TESNPC* nextTemplate;	// 124
         float height;			// 128 // NAM6
         float weight;			// 12C // NAM7
         UInt32 pad130;			// 130
         StringCache::Ref shortName;	// 134
         TESObjectARMO* skinFar;	// 138 // ANAM
         BGSOutfit* defaultOutfit;		// 13C
         BGSOutfit* sleepOutfit;		// 140
         UInt32 unk144;			// 144
         TESFaction* crimeFaction; // 148 // CRIF
         BGSHeadPart** headparts;	// 14C
         UInt8 numHeadParts;    // 150
         UInt8 gearedUpWeapons; // 151 // DNAM
         UInt8 unk152;			// 152
         UInt8 soundLevel; // 153 // NAM8 // clamped to the range of [0, 3] on load
         struct Color { // 797979 Transparent
            UInt8 red, green, blue; // 154 - 156 - Skin Color
         } color; // 154, 155, 156 // QNAM
         UInt8 pad157;			// 157
         UInt32 unk158;			// 158 // Relationships?
         FaceMorphs* faceMorph;	// 15C // NAM9 and NAMA?
         UInt32 unk160;			// 160

         MEMBER_FN_PREFIX(TESNPC);
         //DEFINE_MEMBER_FN(GetHeadPartByType, BGSHeadPart *, 0x00561270, UInt32);
         DEFINE_MEMBER_FN(GetSex, char, 0x0055B510);
         DEFINE_MEMBER_FN(HasOverlays, bool, 0x005681C0);

         // Swaps a headPart of the same type as target with target
         DEFINE_MEMBER_FN(ChangeHeadPart, void, 0x00567CE0, BGSHeadPart * target);

         struct MorphAction {
            BSFaceGenNiNode * faceNode;
            TESNPC * npc;
            const char ** morphName;
            float	value;
         };

         // Applies a morph to all parts of a head
         DEFINE_MEMBER_FN(ApplyMorph, void, 0x005A4870, MorphAction * morphAction);

         // Updates the neck seam when weight changed
         DEFINE_MEMBER_FN(UpdateNeck, void, 0x00567C30, BSFaceGenNiNode * faceNode);

         // Computes RGB SkinTone from RGBA TintMask
         DEFINE_MEMBER_FN(SetSkinFromTint, void, 0x005643C0, NiColorA * result, TintMask * tintMask, UInt32 compute, UInt32 unk1);

         DEFINE_MEMBER_FN(GetLoudness, UInt32, 0x00560F50);
         DEFINE_MEMBER_FN(GetSkin, TESObjectARMO*, 0x00560F70); // checks the actor base and the race
         DEFINE_MEMBER_FN(PrepForArmorAddonChange, bool, 0x005620A0, TESObjectREFR* target, void**, InventoryEntryData*);
         DEFINE_MEMBER_FN(SetDefaultOutfit, void, 0x005610B0, BGSOutfit*);
         DEFINE_MEMBER_FN(SetSleepOutfit,   void, 0x005610D0, BGSOutfit*);
         DEFINE_MEMBER_FN(Subroutine00560F90, const char*, 0x00560F90, UInt32); // related to pronouns
         DEFINE_MEMBER_FN(Subroutine005665E0, void, 0x005665E0, Actor*, ActorWeightData**, bool actorProcessEquipFlag0x40);

         void SetFaceTexture(BGSTextureSet * textureSet);
         void SetHairColor(BGSColorForm * hairColor);

         BGSHeadPart * GetHeadPartByType(UInt32 type);
         BGSHeadPart * GetHeadPartOverlayByType(UInt32 type);
         BGSHeadPart * GetCurrentHeadPartByType(UInt32 type);
         TESNPC * GetRootTemplate();
   };
}