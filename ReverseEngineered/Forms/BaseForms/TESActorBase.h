#pragma once
#include "skse/GameObjects.h"

namespace RE {
   class TESActorBaseData : public BaseFormComponent { // sizeof == 0x38
      public:
         virtual void	Unk_04(UInt32 arg);
         virtual bool	GetFlag20000000(void);
         virtual bool	GetFlag80000000(void);
         virtual bool	Unk_07(void);
         virtual UInt32	GetUnk20(void);

         enum {
            kFlag_Female        = 0x00000001,
            kFlag_Essential     = 0x00000002,
            kFlag_IsPreset      = 0x00000004,
            kFlag_Respawn       = 0x00000008,
            kFlag_AutoCalcStats = 0x00000010,
            kFlag_Unique        = 0x00000020,
            kFlag_DoesntAffectStealthMeter = 0x00000040,
            kFlag_PCLevelMult   = 0x00000080,
            kFlag_UsesTemplate  = 0x00000100,
            //
            kFlag_Protected     = 0x00000800,
            //
            kFlag_Summonable    = 0x00004000,
            //
            kFlag_DoesntBleed   = 0x00010000,
            //
            kFlag_BleedoutOverride    = 0x00040000,
            kFlag_OppositeGenderAnims = 0x00080000,
            kFlag_SimpleActor         = 0x00100000,
            //
            kFlag_IsGhost = 0x20000000,
            //
            kFlag_IsInvulnerable = 0x80000000,
         };

         // ACBS:
         UInt32				flags;	// 04 - init'd to 0
         SInt16				offsetMagicka;	// 08 - init'd to 0
         SInt16				offsetStamina;	// 0A - init'd to 0
         UInt16				level;	// 0C - init'd to 1	(CK Multiplier * 1000 if PCLevelMult is true)
         UInt16				minLevel;	// 0E - init'd to 0
         UInt16				maxLevel;	// 10 - init'd to 0
         UInt16				speedMult;	// 12 - init'd to 0x64 (100)
         UInt16				dispositionBase;	// 14 - init'd to iBaseDisposition (35)
         UInt16				templateFlags;	// 16 - init'd to 0
         SInt16				offsetHealth;	// 18 - init'd to 0
         UInt16				bleedoutOverride;	// 1A - init'd to 0
         //
         TESForm*				deathItem;	// 1C // INAM
         BGSVoiceType* voiceType;	// 20
         TESForm*				unk24;	// 24 // can be a LeveledCharacter
         UInt32				unk28;	// 28 - init'd to 0

         struct FactionInfo {
            TESFaction* faction;	// 00
            SInt8 rank;		// 04
            UInt8 pad[3];		// 05
         };

         tArray<FactionInfo>	factions;	// 2C // TESActorBaseData::Subroutine0044DE30 appends?

         MEMBER_FN_PREFIX(TESActorBaseData);
         DEFINE_MEMBER_FN(GetLevel, UInt16, 0x0044D4D0);
   };
   class TESActorBase : public TESBoundAnimObject { // sizeof == 0xC0
      public:
         // Parents:
         TESActorBaseData	actorData;	// 20
         TESContainer		container;	// 58
         TESSpellList		spellList;	// 64
         TESAIForm			aiForm;		// 6C
         TESFullName			fullName;	// 84
         ActorValueOwner				actorValueOwner;	// 8C
         BGSDestructibleObjectForm	destructible;		// 90
         BGSSkinForm			skinForm;	// 98 // WNAM
         BGSKeywordForm		keyword;	// A0
         BGSAttackDataForm	attackData;	// AC
         BGSPerkRankArray	perkRanks;	// B4

         virtual void			Unk_54(UInt32 arg);
         virtual TESActorBase *	Unk_55(void);
   };
   static_assert(sizeof(TESActorBase) == 0xC0, "RE::TESActorBase is the wrong size.");
   static_assert(offsetof(TESActorBase, keyword) == 0xA0, "RE::TESActorBase::keyword has the wrong offset.");
};