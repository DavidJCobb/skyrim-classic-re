#pragma once
#include "skse/PapyrusVM.h"

namespace RE {
   class VMClassRegistry {
		public:
			struct function_flag {
				function_flag() = delete;
				enum type {
					no_wait = 0x01, // set this only if your function is thread-safe
				};
			};
			enum class log_level : uint32_t {
				info = 0,
				warning,
				error,
				fatal,
			};
			using stack_table_t = tHashSet<VMStackTableItem, UInt32>;

			VMClassRegistry();
			virtual ~VMClassRegistry();
			virtual void	Unk_01(void);
			virtual void	PrintToDebugLog(const char* text, UInt32 stackId, log_level); // 02
			virtual void	Unk_03(void);
			virtual void	Unk_04(void);
			virtual void	Unk_05(void);
			virtual void	Unk_06(void);
			virtual void	Unk_07(void);
			virtual void	RegisterForm(UInt32 typeID, const char * papyrusClassName); // 08
			virtual void	Unk_09(void);
			virtual bool	GetFormTypeClass(UInt32 formType, VMClassInfo ** outClass); // 0A
			virtual void	Unk_0B(void);
			virtual void	Unk_0C(void);
			virtual bool	Unk_0D(StringCache::Ref * className, UInt32 * unk); // 0D
			virtual void	Unk_0E(void);
			virtual void	Unk_0F(void);
			virtual void	Unk_10(void);
			virtual void	Unk_11(void);
			virtual void	Unk_12(void);
			virtual void	Unk_13(void);
			virtual bool	Unk_14(StringCache::Ref * className, VMIdentifier ** identifier);
			virtual bool	CreateArray(VMValue * value, UInt32 size, VMValue::ArrayData ** unk1); // 15
			virtual void	Unk_16(void);
			virtual void	RegisterFunction(IFunction * fn); // 17
			virtual void	SetFunctionFlagsEx(const char * className, UInt32 unk0, const char * fnName, UInt32 flags); // 18
			virtual void	SetFunctionFlags(const char * className, const char * fnName, UInt32 flags); // 19
			virtual void	VisitScripts(UInt64 handle, IForEachScriptObjectFunctor * functor); // 1A
			virtual bool	Unk_1B(UInt64 handle, const char * className, VMIdentifier ** identifier);
			virtual void	Unk_1C(void);
			virtual void	Unk_1D(void);
			virtual void	Unk_1E(void);
			virtual void	Unk_1F(void);
			virtual void	Unk_20(void);
			virtual void	Unk_21(void);
			virtual bool	ExtractValue(UInt64 handle, StringCache::Ref * className, SInt32 variableIndex, VMValue * out); // 22	// unk probably script context object?
			virtual void	QueueEvent(UInt64 handle, const StringCache::Ref * eventName, IFunctionArguments * args); // 23
			virtual void	Unk_24(void);
			virtual void	Unk_25(void);
			virtual void	Unk_26(void);
			virtual void	Unk_27(void);
			virtual void	Unk_28(void);
			virtual void	Unk_29(void);
			virtual void	ResumeStack(UInt32 stackId, VMValue& outResult); // 2A
			virtual void	Unk_2B(void);
			virtual IObjectHandlePolicy *	GetHandlePolicy(void); // 2C
			virtual void	Unk_2D(void);
			virtual ObjectBindPolicy *		GetObjectBindPolicy(void); // 2D
			virtual void	Unk_2F(void);
			virtual void	Unk_30(void);
			virtual void	Unk_31(void);
			virtual void	Unk_32(void);
			virtual void	Unk_33(void);
			virtual void	Unk_34(void);

			UInt32		unk0004;		// 0004 - refcount?
			void		** vtbl0008;	// 0008
			void		** vtbl000C;	// 000C
			void		** vtbl0010;	// 0010
			UInt32		unk0014[(0x006C - 0x0014) >> 2];	// 0014
			VMUnlinkedClassList	unlinkedClassList;			// 006C
			UInt32		unk00B4[(0x49B8 - 0x00B4) >> 2];	// 00B4

			SimpleLock    stackLock;		// 49B8
			UInt32        unk49C0;		// 49C0
			stack_table_t allStacks;		// 49C4
			UInt32        unk49E0;		// 49E0
			stack_table_t waitingStacks;	// 49E4
			UInt32        unk4A00[(0x4B04 - 0x4A00) >> 2];	// 4A00

			VMStackInfo* GetStackInfo(UInt32 stackId);

			void LogError(const char* message, UInt32 stackId)   { this->PrintToDebugLog(message, stackId, log_level::error); }
			void LogWarning(const char* message, UInt32 stackId) { this->PrintToDebugLog(message, stackId, log_level::warning); }
		};
}