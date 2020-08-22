#pragma once
#include "skse/GameFormComponents.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
	class TESTexture : public BaseFormComponent { // sizeof == 0x08
		public:
			virtual UInt32			Unk_04(void);
			virtual void			GetNormalMapName(BSString * out);	// might have const char * retn type
			virtual const char*	GetSearchDir(void);

			StringCache::Ref str; // 04

			MEMBER_FN_PREFIX(TESTexture);
			DEFINE_MEMBER_FN(SetPath, void, 0x00447B80, const char*);

			void SetPath(const char* path) {
				CALL_MEMBER_FN(&this->str, Set)(path);
			}
	};
}