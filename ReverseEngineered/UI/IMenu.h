#pragma once
#include "skse/GameTypes.h"
#include "skse/ScaleformCallbacks.h"
#include "skse/ScaleformMovie.h"
#include "skse/Utilities.h"

namespace RE {
	class IMenu : public FxDelegateHandler { // sizeof == 0x1C
		struct BSUIScaleformData {
			virtual ~BSUIScaleformData() {}

			//	void	** _vtbl;		// 00
			UInt32 unk04; // 04
			void*  unk08; // 08
		};
		struct UnkData1 {
			BSFixedString name;	// 00
			UInt32        unk04;	// 04
			BSUIScaleformData* data;	// 08 - BSUIScaleformData
		};

	public:
		IMenu();

		enum {
			kType_PauseGame = 1,
			kType_ShowCursor = 2
		};

		virtual ~IMenu(); // Dispose
		virtual void   Accept(CallbackProcessor* processor); // pure
		virtual void   Unk_02();
		virtual void   Unk_03();
		virtual bool   Unk_04();
		virtual UInt32 ProcessUnkData1(UnkData1* data); // 05
		virtual void   Unk_06();
		virtual void   Unk_07(UInt32);

		GFxMovieView* view;	// 08 - init'd to 0, a class, virtual fn 0x114 called in dtor
		UInt8  unk0C;		// 0C - init'd to 3
		UInt8  pad0D[3];	// 0D
		UInt32 flags;		// 10 - init'd to 0
		UInt32 unk14;		// 14 - init'd to 0x12
		GRefCountBase* unk18;	// 18 - holds a reference

		MEMBER_FN_PREFIX(IMenu);
		DEFINE_MEMBER_FN(Destructor, void, 0x00A64A10);
		DEFINE_MEMBER_FN(InitMovie_internal, void, 0xA64A50, GFxMovieView* view);
		DEFINE_MEMBER_FN(NextFrame_internal, void, 0xA64980, UInt32 arg0, UInt32 arg1);
	};
}