#include "ReverseEngineered/NetImmerse/objects.h"
#include "ReverseEngineered/NetImmerse/NiExtraData.h"
#include "skse/NiExtraData.h"

namespace RE {
   DEFINE_SUBROUTINE(bool, NiObjectIs, 0x0042A960, const NiRTTI*, const NiObject*);

   //
   // untested code from SKSE:
   //
	int ExtraDataCompare(const void* ls, const void* rs)
	{
		return (int)((*(NiExtraData**)ls)->name - (*(NiExtraData**)rs)->name);
	}
	void NiObjectNET::AddExtraData(NiExtraData* extraData)
	{
		extraData->IncRef();

		// No capacity, allocate and grow
		UInt32 newSize = 0;
		if (m_extraDataCapacity == 0) {
			newSize = 1;
			NiExtraData** extraDataList = (NiExtraData**)FormHeap_Allocate(newSize * sizeof(NiExtraData*));
			extraDataList[0] = extraData;
			m_extraData = extraDataList;
			m_extraDataCapacity = newSize;
			m_extraDataLen = m_extraDataCapacity;
			return;
		}

		// Reached capacity, reallocate and grow
		if (m_extraDataLen == m_extraDataCapacity) {
			newSize = (m_extraDataCapacity * 2) + 1;
			NiExtraData** extraDataList = (NiExtraData**)FormHeap_Allocate(newSize * sizeof(NiExtraData*));
			// Copy the entries over
			memcpy_s(extraDataList, newSize * sizeof(NiExtraData*), m_extraData, m_extraDataLen * sizeof(NiExtraData*));
			// Zero the allocated entries
			for (UInt16 i = m_extraDataLen; i < newSize; i++)
				extraDataList[i] = NULL;

			FormHeap_Free(m_extraData);
			m_extraData = extraDataList;
			m_extraDataCapacity = newSize;
		}

		m_extraData[m_extraDataLen++] = extraData;
		qsort(m_extraData, m_extraDataLen, sizeof(NiExtraData*), ExtraDataCompare);
	}

	SInt32 NiObjectNET::GetIndexOf(NiExtraData* extraData)
	{
		if (!extraData)
			return -1;
		if (!extraData->name)
			return -1;

		SInt16 min = 0;
		SInt16 max = (SInt16)m_extraDataLen - 1;
		SInt16 r = -1;
		while (max >= min) { // Iterative binary search
			SInt16 mid = (min + max) >> 1;
			if (m_extraData[mid]->name == extraData->name)
				r = mid; // Found entry
			else if (m_extraData[mid]->name < extraData->name)
				min = mid + 1;
			else
				max = mid - 1;
		}

		return r;
	}

	bool NiObjectNET::RemoveExtraData(NiExtraData* extraData)
	{
		SInt32 index = GetIndexOf(extraData);
		if (index >= 0) { // Remove the entry
			extraData->DecRef();
			// Shift everything up
			for (UInt16 i = index; i < m_extraDataLen - 1; i++)
				m_extraData[i] = m_extraData[i + 1];
			// Remove the duplicate tail entry
			m_extraData[--m_extraDataLen] = NULL;
			return true;
		}

		return false;
	}

	// This should be a BSFixedString before passing to this
	NiExtraData* NiObjectNET::GetExtraData(BSFixedString name)
	{
		SInt16 min = 0;
		SInt16 max = (SInt16)m_extraDataLen - 1;

		while (max >= min) { // Iterative binary search
			SInt16 mid = (min + max) >> 1;
			if (m_extraData[mid]->name == name.data)
				return m_extraData[mid];
			else if (m_extraData[mid]->name < name.data)
				min = mid + 1;
			else
				max = mid - 1;
		}

		return NULL;
	}
};