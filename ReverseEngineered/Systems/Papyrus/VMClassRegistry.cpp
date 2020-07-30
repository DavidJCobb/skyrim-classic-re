#include "VMClassRegistry.h"
#include "ReverseEngineered/Types.h"

namespace RE {
	VMStackInfo* VMClassRegistry::GetStackInfo(UInt32 stackId) {
		auto guard = simple_lock_guard(this->stackLock);
		//
		VMStackInfo*      result = nullptr;
		VMStackTableItem* item   = allStacks.Find(&stackId);
		if (item && item->data && item->data->unkData)
			result = item->data->unkData->stackInfo;
		return result;
	}
}