UInt32 ExtraContainerChanges::Data::Subroutine00477380(uint8_t bodyPartIndex, UInt32 Arg2, bool skipWeapons, bool Arg4) {
   ebp = 0;
   tList<InventoryEntryData>* esp14 = this->objList;
   if (!esp14)
      return 0;
   do {
      InventoryEntryData* edi = esp14->data;
      esp18 = edi;
      if (!edi)
         return 0;
      esi = edi->next;
      bool bl = false;
      esp10 = esi;
      esp24 = 0;
      bool al = false;
      while (esi) {
         if (!esi->type)
            break;
         al = esi->type->TESV_0040C170();
         esi = esi->next;
         if (al)
            break;
      }
      if (al && edi->countDelta < 0) { // at 0x00477400
         esi = edi->extendDataList;
         if (esi) {
            bool found = false;
            do {
               if (!esi->data)
                  break;
               found = esi->data->CheckExtraWorn(true, false);
            } while (!found && (esi = esi->next));
            if (found) {
               // at 0x0047742B
               esi = edi->extendDataList;
               found = false;
               if (esi) {
                  BaseExtraList* esp30;
                  do {
                     if (!esi->data)
                        break;
                     esp30 = esi->data;
                     found = esi->data->CheckExtraWorn(false, false);
                  } while (!found && (esi = esi->next));
                  if (found) {
                     // at 0x00477452
                     esi->FindAndRemoveItem(&esp30, 0, 0);
                     esp30->~BaseExtraList();
                     delete esp30;
                  }
               }
            }
         }
      }
      // at 0x00477473
      if (!esp10)
         continue;
      do {
         if (!esp10->data)
            break;
         if (bl)
            jump to 0x004775BA;
         BaseExtraList* esp24 = esp10->data;
         bool al = esp24->CheckExtraWorn(false, Arg4);
         if (!al)
            continue;
         TESForm* esi = (*esp18)->type;
         switch (esi->formType) {
            default:
               // at 0x004774CC
               edi = esp30;
               if (edi == -1)
                  continue;
               auto eax = ToBGSBipedObjectForm(esi);
               if (eax) {
                  bl = eax->TestBodyPartByIndex(bodyPartIndex);
                  continue;
               }
               bl = false;
               continue;
            case kFormType_Ammo:
               if (bodyPartIndex != 0x29) // hair, long
                  continue;
               break;
            case kFormType_Weapon:
               if (skipWeapons)
                  continue;
               edi = bodyPartIndex;
               if (bodyPartIndex == 0x20) { // body
                  bool al = TESV_0046AAE0(bodyPartIndex);
                  if (!al)
                     continue;
                  al = esi->unkF1;
                  if (al == 7)
                     continue;
                  if (al != 9)
                     bl = true;
                  continue;
               }
               bool al = TESV_0046AAE0(bodyPartIndex);
               if (al) {
                  eax = esi->unkF1;
                  bl = (*(0x01243428)[eax] == edi);
                  continue;
               }
               if (edi != -1)
                  continue;
               if (esp34)
                  continue;
               eax = esi->unkF1;
               if (eax == 7)
                  bl = true;
               else if (eax == 9)
                  bl = true;
         }
      } while (esp10 = esp10->next); // at 0x00477583
      if (bl)
         jump to 0x004775BA;
   } while (esp14 = esp14->next); // at 0x0047759A
   return 0;
   //
   // at 0x004775BA:
   //
   if (esp34) {
      ecx = esp18->data;
      if (ecx->formType != 0x1A) {
         return 0;
      }
   }
   // at 0x004775E7
   edi = new InventoryEntryData();
   edi->type = esp18->unk00;
   edi->extendDataList = new tList<BaseExtraList>*;
   ebx = esp24;
   esi = edi->extendDataList;
   if (ebx) {
      //
      // ...some inlined code that never runs is here; i've omitted it...
      //
      esi->data = esp24; // at 0x00477667
   }
   // at 0x00477669
   uint16_t ax = ebx->GetExtraCount();
   edi->countDelta = ax;
   return edi;
}