#pragma once
#include "skse/Utilities.h"

namespace RE {
   class TESGlobal;
   //
   extern float* const g_timeNow; // in-game time, in seconds
   //
   struct TimeGlobals {
      UInt32     unk00;
      TESGlobal* year;  // 04 // [GLOB:00000035]GameYear
      TESGlobal* month; // 08 // [GLOB:00000036]GameMonth
      TESGlobal* day;   // 0C // [GLOB:00000037]GameDay
      TESGlobal* hour;  // 10 // [GLOB:00000038]GameHour
      TESGlobal* daysPassed; // 14 // [GLOB:00000039]GameDaysPassed
      TESGlobal* timeScale;  // 18 // [GLOB:0000003A]TimeScale
      UInt32 unk1C = 0; // 1C
      float  unk20 = 0.0F; // 20

      static TimeGlobals* GetInstance() {
         return *(TimeGlobals**)0x012E35DC;
      };

      MEMBER_FN_PREFIX(TimeGlobals);
      DEFINE_MEMBER_FN(Constructor, TimeGlobals&, 0x0068D580);
      DEFINE_MEMBER_FN(GetDayOfMonth,      UInt32, 0x0068CF10); // if the global is missing, returns 17
      DEFINE_MEMBER_FN(GetGameDaysPassed,  SInt32, 0x0068CF60);
      DEFINE_MEMBER_FN(GetHour,            float,  0x0068CF40);
      DEFINE_MEMBER_FN(GetMonth,           SInt32, 0x0068CEE0);
      DEFINE_MEMBER_FN(GetTimeScale,       float,  0x0068CE90);
      DEFINE_MEMBER_FN(GetYear,            SInt32, 0x0068CEA0);
      DEFINE_MEMBER_FN(Subroutine004B8560, UInt32, 0x004B8560);
      //
      DEFINE_MEMBER_FN(PassRealSeconds, void, 0x0068DB40, float seconds); // advances in-game time by (seconds * timescale); used by the main thread and by some places that skip in-game time e.g. fast-travel
   };
   constexpr TimeGlobals** g_timeGlobals = (TimeGlobals**)0x012E35DC;
};
