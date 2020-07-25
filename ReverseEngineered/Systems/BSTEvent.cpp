#include "BSTEvent.h"

namespace RE {
   BSTEventSource<UInt32>* const g_dayPassedEventSource      = (BSTEventSource<UInt32>*)0x01B2E728;
   BSTEventSource<UInt32>* const g_menuModeChangeEventSource = (BSTEventSource<UInt32>*)0x01271CE8;

   DEFINE_SUBROUTINE(void, AddHarvestEventSink, 0x00455C80, void*); // verify
}