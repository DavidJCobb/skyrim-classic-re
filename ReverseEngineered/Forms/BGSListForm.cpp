#include "BGSListForm.h"
#include "ReverseEngineered/ExtraData.h"

namespace RE {
   SInt32 BGSListForm::CountMatchingItemsInInventory(InventoryChanges& inventory) {
      CountMatchingItemsInInventoryVisitor visitor(inventory);
      CALL_MEMBER_FN(this, CountMatchingItemsInInventory)(visitor);
      return visitor.count;
   }
}