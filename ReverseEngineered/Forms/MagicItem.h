#pragma once
#include "TESForm.h"
#include "BaseForms/base.h"

class EffectSetting;
namespace RE {

   //
   // MagicItem
   //  - AlchemyItem
   //  - EnchantmentItem
   //  - IngredientItem
   //  - SpellItem
   //     - ScrollItem
   //

   class MagicItem : public TESBoundObject { // sizeof == 0x50
      public:
         enum class Type : UInt32 {
            alchemy    = 0x7, // AlchemyItem
            ingredient = 0x8, // IngredientItem
            //
            scroll     = 0xD, // ScrollItem
         };
         //
      public:
	      virtual Type   GetType(); // 54 // pure; returns enum or: for EnchantmentItem, Data::unk14; for SpellItem, unk74
	      virtual void	Unk_55(UInt32 arg);
	      virtual UInt32	Unk_56();	// pure
	      virtual void	Unk_57(UInt32 arg);
	      virtual UInt32	Unk_58(void);	// pure
	      virtual bool	Unk_59(UInt32 arg);
	      virtual float	Unk_5A(void);
	      virtual float	Unk_5B(void);
	      virtual bool	Unk_5C(void);
	      virtual bool	Unk_5D(void);
	      virtual bool	Unk_5E(void);
	      virtual bool	Unk_5F(void);
	      virtual bool	Unk_60(void);
	      virtual bool	Unk_61(UInt32 arg);
	      virtual bool	Unk_62(void);
	      virtual bool	Unk_63(void);
	      virtual void	Unk_64(UInt32 arg0, UInt32 arg1);
	      virtual float	Unk_65(void);
	      virtual UInt32	Unk_66(void);
	      virtual UInt32	Unk_67(void);
	      virtual bool	Unk_68(void);
	      virtual UInt32	Unk_69(void);	// pure, returns char code
	      virtual void	Unk_6A(MagicItem * src);	// pure, copy data if possible?
	      virtual void	Unk_6B(UInt32 arg0, UInt32 arg1);	// loading-related
	      virtual void	Unk_6C(UInt32 arg);
	      virtual void *	Unk_6D(void);	// pure, returns data
	      virtual void *	Unk_6E(void);	// pure, returns data
	      virtual UInt32	Unk_6F(void);	// pure, return size of data
	      virtual void	Unk_70(void);	// pure, byteswap?

	      // parents
	      TESFullName		fullName;	// 20
	      BGSKeywordForm	keyword;	// 28

	      // members
	      struct EffectItem {
		      float  magnitude = 0.0F; // 00
		      UInt32 area      = 0;    // 04 // measured in feet; is multiplied by GMST:fMagicUnitsPerFoot to convert to world units
		      UInt32 duration  = 0;    // 08
		      EffectSetting* mgef = nullptr; // 0C
		      float	 cost  = 0.0F;     // 10 - ?
		      UInt32 unk14 = 0;        // 14 // most likely a pointer to a condition list
            
            MEMBER_FN_PREFIX(EffectItem);
            DEFINE_MEMBER_FN(GetArea, UInt32, 0x00402F50);
	      };

	      tArray<EffectItem*> effectItemList;	// 34
	      UInt32				hostile;	// 40
	      EffectSetting*		unk44;	// 44
	      UInt32				unk48;	// 48
	      UInt32				unk4C;	// 4C

	      MEMBER_FN_PREFIX(MagicItem);
         DEFINE_MEMBER_FN(Constructor, MagicItem&, 0x004071C0);
	      DEFINE_MEMBER_FN(GetCostliestEffectItem, EffectItem*, 0x00407860, int arg1, bool arg2);
	      DEFINE_MEMBER_FN(GetEffectiveMagickaCost, double, 0x00406EF0, Character* caster);
   };
}
