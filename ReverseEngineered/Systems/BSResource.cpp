#include "BSResource.h"

namespace RE {
	BSResource::EntryDB<BSModelDB::DBTraits>*  ptrSingleton012E2CF8 = (BSResource::EntryDB<BSModelDB::DBTraits>*)  0x01B92DB0;
	BSResource::EntryDB<BSModelDB::DBTraits>** Singleton012E2CF8    = (BSResource::EntryDB<BSModelDB::DBTraits>**) 0x012E2CF8;

	DEFINE_SUBROUTINE(bool, GetAlreadyLoadedMeshResource, 0x00AF5030, const char* pathOrName, LoadedMeshResourcePtr&);
}