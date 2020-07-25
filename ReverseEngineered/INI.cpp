#include "INI.h"

namespace RE {
   namespace INI {
      namespace BackgroundLoad {
         Setting* const bLoadHelmetsInBackground = (Setting*) 0x01240DD8;
         Setting* const bUseMultiThreadedFaceGen = (Setting*) 0x0125D2A0;
      }
      namespace Display {
         Setting* const fLightLODStartFade = (Setting*) 0x012CF7B0;
         Setting* const fLightLODRange     = (Setting*) 0x012CF7BC;
      }
      namespace General {
         Setting* const bUseBodyMorphs = (Setting*)0x01240DE4;
      }
   }
}