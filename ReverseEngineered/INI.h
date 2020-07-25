#pragma once
#include "skse/GameSettings.h"

namespace RE {
   namespace INI {
      namespace BackgroundLoad {
         extern Setting* const bLoadHelmetsInBackground;
         extern Setting* const bUseMultiThreadedFaceGen;
      }
      namespace Display {
         extern Setting* const fLightLODStartFade;
         extern Setting* const fLightLODRange;
      }
      namespace General {
         extern Setting* const bUseBodyMorphs;
      }
   }
}