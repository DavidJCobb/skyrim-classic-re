#pragma once
#include "TESForm.h"

namespace RE {
   class TopicInfo;
   class TESTopic : public TESForm { // sizeof == 0x38
      public:
	      enum { kTypeID = kFormType_Topic };

	      // parents
	      TESFullName			fullName;	// 14

	      // members
	      UInt8				unk1C[4];	// 1C
	      UInt32				unk20;		// 20
	      UInt32				unk24;		// 24
	      UInt32				unk28;		// 28
	      TopicInfo** infos; // 2C - buf[unk30]
	      UInt32      infoCount; // 30 - len
	      StringCache::Ref	unk34;		// 34

         MEMBER_FN_PREFIX(TESTopic);
         DEFINE_MEMBER_FN(OnInfoLoaded,     bool, 0x0057CF90, TopicInfo*, SInt32 previousInfoFormID); // record layout is DIAL followed by GRUP<INFO>; the DIAL writes itself to what looks to be a "last potential parent form" pointer, and each INFO then calls this during its load to add itself to the DIAL
         DEFINE_MEMBER_FN(FindInfoByFormID, TopicInfo*, 0x0057BFC0, UInt32 formID, bool searchFromEnd); // used by OnInfoLoaded to handle INFO/PNAM (Previous Info).

         // Called multiple times, apparently redundantly, between OnInfoLoaded and InsertInfo.
         // Copies all non-nullptr TopicInfo*s from (infos) to a scratch BSTArray and enlarges that array by one.
         // If at that point the scratch array's length doesn't match the TESTopic::infos length, then the latter 
         // is reallocated and repopulated with the contents of the scratch array.
         //
         // TBH I'm not sure if this is meant to ensure that there's always room for one more info, or if it's 
         // just meant to clear nullptrs from the array. Actually, it may be a shrink_to_fit function.
         DEFINE_MEMBER_FN(EnsureRoomForNewInfo, void, 0x0057CA90);

         // Called by OnInfoLoaded.
         // Calls EnsureRoomForNewInfo, but then reallocates the array itself anyway.
         // If (position) is out-of-bounds, then it's capped to the array's length prior to reallocation (i.e. insert at end).
         DEFINE_MEMBER_FN(InsertInfo, void, 0x0057CD50, UInt32 position, TopicInfo* info);
   };

   class TopicInfo : public TESForm {
      public:
         TESTopic* owner; // 14
         //
         // ... TODO ...
         //
   };
}