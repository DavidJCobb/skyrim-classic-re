#pragma once
#include "TESForm.h"

namespace RE {
   class BGSRefAlias : public BGSBaseAlias {
      public:
         enum { kTypeID = kFormType_ReferenceAlias };
         // virtual unk 01 == read (BGSLoadFormBuffer*)
         enum {
            kFillType_None = 0,
            kFillType_PresetObjectReference = 1,
            kFillType_AnotherLocAliasInSameQuest = 2, // ALFA
            kFillType_FromEvent = 3, // ALFE
            kFillType_CreateObject = 4,
            kFillType_AnotherRefAliasInOtherQuest = 5,
            kFillType_PresetUniqueActorBase = 6,
            kFillType_FindMatchingReference = 7,
         };
         //
         UInt32 fillType; // 14
         UInt32 unk18[0x03]; // One of these is the filltype/filltype filter
   };
   class BGSLocAlias : public BGSBaseAlias {
      public:
         enum { kTypeID = kFormType_LocationAlias };
         // virtual unk 01 == read (BGSLoadFormBuffer*)
         enum {
            kFillType_None = 0,
            kFillType_PresetLocation = 1, // ALFL
            kFillType_AnotherLocAliasInSameQuest = 2, // ALFA
            kFillType_FromEvent = 3, // ALFE
            kFillType_AnotherLocAliasInOtherQuest = 4, // ALEQ
         };
         //
         // BNAM and ONAM cause flags to bet set in unk10 on BGSBaseAlias
         UInt32 fillType; // 14
         UInt32 unk18; // 18 // ALFL - BGSLocation form ID (converted to pointer in second load pass?)
         UInt32 unk1C = -1; // 1C // ALFA
         UInt32 unk20; // 20 // KNAM - BGSKeyword form ID (converted to pointer in second load pass?)
         UInt32 unk24 = -1; // 24 // ALFE - from event (type)
         UInt32 unk28 = -1; // 28 // ALFD - from event (data)
         UInt32 unk2C; // 2C // ALEQ - TESQuest form ID (converted to pointer in second load pass?)
         UInt32 unk30 = -1; // 30 // ALEA - alias ID in other quest
         void*  unk34; // 34 // CTDA
   };

   class BGSStoryManagerTreeForm : public TESForm { // sizeof == 0x18
      public:
         virtual void *	Unk_3B(void);	// pure
         virtual void	Unk_3C(void);	// pure

         UInt32	unk14;	// 14
   };

   struct Struct0060D490 {
      tArray<UInt32> unk00;
      tArray<UInt32> unk0C;
      //
      MEMBER_FN_PREFIX(Struct0060D490);
      DEFINE_MEMBER_FN(Constructor, Struct0060D490&, 0x0060D490);
   };

   class TESQuest : public BGSStoryManagerTreeForm { // sizeof == 0x158
      public:
	      enum { kTypeID = kFormType_Quest };

	      struct Data03C { // sizeof == 0x20
		      UInt8	data[0x20];	// ### todo
	      };
	      struct Data05C { // sizeof == 0x20
		      UInt8	data[0x20];	// ### todo
	      };
	      struct Data07C { // sizeof == 0x08 // DNAM
		      UInt32 unk00;    // 00
		      UInt16 flags;    // 04
		      UInt8  priority; // 06
		      UInt8  type;     // 07
	      };
         struct Target {
            UInt8  unk00 = 0; // low byte is QSTA flags; only the low byte of the flags is kept
            UInt8  pad01[3];
            void*  conditions; // 04
            UInt32 unk08;
            UInt32 unk0C = 0;
            Struct0060D490 unk10;
            //
            MEMBER_FN_PREFIX(Target);
            DEFINE_MEMBER_FN(Constructor, Target&, 0x00576650);
            DEFINE_MEMBER_FN(Load, void, 0x00576680, BGSLoadFormBuffer*);
         };
	      struct Objective {
		      StringCache::Ref displayText; // 00
		      TESQuest* owner       = nullptr; // 04
		      Target**  targets     = nullptr; // 08
		      UInt32    targetCount = 0; // 0C
		      UInt16    index; // 10
            UInt8     unk12; // 12
            UInt8     unk13; // 13
		      UInt32    flags; // 14
            //
            MEMBER_FN_PREFIX(Objective);
            DEFINE_MEMBER_FN(Constructor,  Objective&, 0x00551B90);
            DEFINE_MEMBER_FN(AppendTarget, void, 0x00551B10, Target*);
            DEFINE_MEMBER_FN(Load, void, 0x00551C70, BGSLoadFormBuffer*);
	      };
         struct LogEntry {
            UInt32 unk00;
            UInt32 unk04;
            UInt32 unk08 = -1;
            UInt8  unk0C;
            UInt8  unk0D; // 0D
            bool   hasText = false; // 0E
            UInt8  pad0F;
            UInt32 unk10;
            UInt32 unk14;
            //
            MEMBER_FN_PREFIX(LogEntry);
            DEFINE_MEMBER_FN(Constructor, LogEntry&, 0x00575C30);
            DEFINE_MEMBER_FN(Load,     void, 0x00575DE0, BGSLoadFormBuffer*); // opens another record, too -- messy!
            DEFINE_MEMBER_FN(LoadText, void, 0x00575AF0, BGSLoadFormBuffer*); // CNAM
         };
         struct Stage { // sizeof == 0x0C
            UInt16 index; // 00
            UInt8  unk02; // 02
            UInt8  pad03;
            tList<LogEntry> logEntries; // 04
            //
            MEMBER_FN_PREFIX(Stage);
            DEFINE_MEMBER_FN(Constructor, Stage&, 0x00575B40);
            DEFINE_MEMBER_FN(Load, void, 0x00575D00, BGSLoadFormBuffer*);
         };
	      struct Data0A0 { // sizeof == 0x20
		      UInt8	data[0x1C];	// ### todo
	      };

         // Parents:
         TESFullName	fullName; // 18
         //
         // Members:
	      UnkArray	unk020;		// 020
	      UInt32		unk02C;		// 02C
	      tArray<BGSBaseAlias*> aliases;		// 030
	      Data03C		unk03C;		// 03C
	      Data05C		unk05C;		// 05C
	      Data07C		data;		// 07C
	      UInt32           smEventCode; // 84 // ENAM
	      tList<Stage>     stages;      // 88
	      tList<Objective> objectives;  // 90
	      void* unk098;	// 098 - linked list
	      void* unk09C;	// 09C - linked list
	      Data0A0		unk0A0;		// 0A0
	      void* unk0BC;
	      Data0A0		unk0C0;
	      UInt32		unk0DC;
	      UnkArray	unk0E0[6];	// 0E0
	      UnkArray	unk128;		// 128
	      tArray<UInt32>* unk134;	// 134 // QTGL
	      UInt16		unk138;		// 138
	      UInt8		unk13A;		// 13A
	      UInt8		pad13B;		// 13B
	      BSString	questID;	// 13C
	      UInt32		unk144;		// 144
	      UInt32		unk148;		// 148
	      UnkArray	unk14C;		// 14C

	      MEMBER_FN_PREFIX(TESQuest);
	      DEFINE_MEMBER_FN(ForceRefTo, UInt32, 0x005728C0, UInt32 aliasId, TESObjectREFR * reference);
	      DEFINE_MEMBER_FN(NewGame_Internal, UInt8, 0x575A20, UInt8 * unk1, UInt8 unk2);

         DEFINE_MEMBER_FN(AppendObjective, void, 0x0056C600, Objective&); // used during load

	      UInt8 NewGame_Hook(UInt8 * unk1, UInt8 unk2);
      };
};