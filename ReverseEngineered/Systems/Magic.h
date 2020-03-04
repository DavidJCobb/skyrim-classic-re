#pragma once
#include "skse/GameTypes.h"

class Character;
namespace RE {
   class ActiveEffect;
   class Actor;
   class MagicItem;
   class NiAVObject;
   class BGSLoadGameBuffer;
   class BGSSaveFormBuffer;
   
   class MagicCaster { // sizeof == 0x30
      public:
         virtual void   Dispose(bool free);
         virtual void   Unk_01(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32, UInt32);
         virtual UInt32 Unk_02();
         virtual void   Unk_03();
         virtual bool   Unk_04();
         virtual void   Unk_05(void); // signature unknown
         virtual void   Unk_06();
         virtual void   Unk_07(); // no-op on base class
         virtual void   Unk_08(UInt32); // no-op on base class
         virtual void   Unk_09(UInt32, UInt32, UInt32); // no-op on base class
         virtual bool   Unk_0A(UInt32, UInt32, UInt32, UInt32, UInt32);
         virtual UInt32 Unk_0B(); // no-op (returns zero) on base class
         virtual Actor* Unk_0C(); // no-op (returns nullptr) on base class // ActorMagicCaster: returns unk68
         virtual UInt32 Unk_0D(UInt32);
         virtual NiAVObject* Unk_0E(); // no-op (returns zero) on base class; defined on ActorMagicCaster // may be NiNode instead
         virtual void   Unk_0F(); // no-op on base class
         virtual void   Unk_10(UInt32); // no-op on base class
         virtual void   Unk_11(); // no-op on base class
         virtual void   Unk_12(); // no-op on base class
         virtual void   Unk_13(); // no-op on base class
         virtual void   Unk_14();
         virtual UInt32 Unk_15(); // no-op (returns zero) on base class
         virtual bool   Unk_16(); // no-op (returns false) on base class
         virtual void   Unk_17(UInt32); // no-op on base class
         virtual void   WriteSavedata(BGSSaveFormBuffer*);
         virtual void   LoadSavedata(BGSLoadGameBuffer*);
         virtual void   Unk_1A(UInt32);
         virtual void   Unk_1B(UInt32, UInt32); // related to sounds
         virtual bool   Unk_1C(UInt32, UInt32, UInt32);
         //
         tArray<UInt32> unk04;
         UInt32 unk10 = 0;
         UInt32 unk14 = 0;
         UInt32 unk18 = 0;
         UInt32 unk1C = 0;
         float  unk20 = 1.0F;
         UInt32 unk24 = 0;
         UInt32 unk28 = 0;
         UInt32 unk2C = 0;
   };

   class MagicTarget { // sizeof == 0xC
      public:
         virtual ~MagicTarget();

         virtual UInt8      Unk_01(int);
         virtual Character* GetTargetActor(); // 02
         virtual bool       IsActor(); // 03 // indicates whether this MagicTarget is a member/parent of Actor
         virtual bool       Unk_04(void);
         virtual int        Unk_05(int);
         virtual bool       Unk_06(void); // pure
         virtual tList<ActiveEffect>* GetActiveEffects(); // 07 // pure
         virtual int        Unk_08(int);
         virtual void       Unk_09(int);
         virtual double     Unk_0A(int, int, int);
         virtual UInt8      Unk_0B(int, int, int);

         //	void	** _vtbl;	// 00
         UInt32 unk04; // 04
         UInt32 unk08; // 08

         class ForEachActiveEffectVisitor {
            public:
               virtual ~ForEachActiveEffectVisitor(); // 00 // Dispose(bool)
               virtual bool Visit(ActiveEffect*); // 01 // return value isn't used?
         };

         MEMBER_FN_PREFIX(MagicTarget);
         DEFINE_MEMBER_FN(ExecuteFunctor, void, 0x00663410, ForEachActiveEffectVisitor*);
         DEFINE_MEMBER_FN(AsActor, Actor*, 0x00662870); // if this MagicTarget is a member/parent of Actor, returns the Actor instance
         DEFINE_MEMBER_FN(AdvanceTime, void, 0x00664470, float realTimeSeconds); // only called if actor AI is globally enabled; might do other stuff as well; not sure yet
   };

   //
   // Functors:
   //
   class FindAppropriateDisplaceEffect : public MagicTarget::ForEachActiveEffectVisitor {
      //
      // Contained in anonymous namespace AnonymousNamespace0x6426b6db in the compiled 
      // executable.
      //
      // Given an alchemy effect and an addiction, this searches the magic-target's active 
      // effects for any ongoing effect that has the same addiction. It returns true if a 
      // match is found.
      //
      // ActiveEffect::PlayerHasTakenMyChem runs this functor and returns its result. It 
      // supplies its source spell as the addiction and its unk44 field as the alchemy 
      // effect. It is only called by ActiveEffect::DoConditionUpdate, and only in the 
      // event that the effect's conditions fail to match; if the functor returns true, 
      // then the effect doesn't turn off despite its conditions not matching.
      //
      public:
         MagicItem* alchEffect; // 04
         MagicItem* addiction;  // 08
         bool       result = false; // 0C
   };
}