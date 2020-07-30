#pragma once
#include "ReverseEngineered/Systems/GameData.h"
#include "ReverseEngineered/Miscellaneous.h"

namespace RE {
   TES** g_TES = (TES**) 0x01B2E864;

   /*GridCellArray::GridCellArray() { // Decoded from ASM and listed here purely for my own reference.
      static UInt32* gridsToLoad = (UInt32*)0x01241000;
      {  // Handle grids value.
         UInt32 grids = *gridsToLoad;
         if (grids > 5) {
            grids = 5;
         } else {
            if (grids & 1 == 0)
               grids++;
         }
         *gridsToLoad = grids;
         this->length = grids;
         //
         grids *= grids;
         grids *= 4;
         this->cells = (TESObjectCELL**)CALL_MEMBER_FN(g_formHeap, Allocate)(grids, 0, 0);
         FillBufferWithByte(this->cells, 0, grids);
      }
   };*/

   ::TESObjectCELL** TES::CopyGridCells(UInt32* count) const {
      if (!this->gridCellArray->cells)
         return NULL;
      UInt32 length = this->gridCellArray->length;
      *count = length * length;
      auto list = (::TESObjectCELL**) malloc(*count * 4);
      memcpy(list, this->gridCellArray->cells, *count * 4);
      return list;
   };

   DataHandler* TESDataHandler::GetSingleton() {
      return *((TESDataHandler**)0x012E2CB8);
   };
   UInt32 TESDataHandler::GetNextID() {
      RE::simple_lock_guard scopedLock(*(RE::SimpleLock*)0x012E32D4);
      while (true) {
         //
         // Loop over all form IDs within a given load order prefix, 
         // starting at a stored value. If we hit the end of the range, 
         // restart from the beginning of the range. If there isn't any 
         // usable form ID, loop endlessly and hang.
         //
         do {
            auto edi = this->unk68C;
            auto eax = LookupFormByID(edi);
            if (!eax) {
               if (!CALL_MEMBER_FN(BGSSaveLoadManager::GetSingleton(), ChangeDataExistsForFormID)(edi)) {
                  //
                  // Found a free form ID. Increment the cached unk68C ID, and then return the 
                  // found ID.
                  //
                  edi = this->unk68C;
                  this->unk68C++;
                  if ((this->unk68C & 0x00FFFFFF) >= 0x003FFFFF) {
                     this->unk68C = (this->unk68C & 0xFF000000) + 0x800;
                  }
                  return edi;
               }
            }
            this->unk68C++;
         } while ((this->unk68C & 0x00FFFFFF) < 0x003FFFFF);
         //
         // Restart search from form ID xx000800 within the same load 
         // order prefix. Yes, this would just search endlessly if 
         // the whole ID space were taken up.
         //
         this->unk68C = (this->unk68C & 0xFF000000) + 0x800;
      }
   };
};