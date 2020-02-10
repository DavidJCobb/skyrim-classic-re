#pragma once
#include "skse/GameObjects.h"

namespace RE {
   //
   // For information on grass rendering, see Singleton012E35CC
   //
   class TESGrass : public TESBoundObject { // sizeof == 0x54
      public:
         enum { kTypeID = kFormType_Grass };

         virtual UInt8	GetDensity(); // 52 // return unk34;
         virtual bool	SetDensity(UInt8 data);		// if(data > 100) return false;
                                                   // unk34 = data; return true;
         virtual UInt8	GetSlopeMin(void);				// return unk35;
         virtual bool	SetSlopeMin(UInt8 data);		// if(data > 90) return false;
                                                   // if(data > unk36) return false;
                                                   // unk35 = data; return true;
         virtual UInt8	GetSlopeMax(void);				// return unk36;
         virtual bool	SetSlopeMax(UInt8 data);		// if(data > 90) return false;
                                                   // if(data < unk35) return false;
                                                   // unk36 = data; return true;
         virtual float	GetUnk35_Float(void); // 58		// return unk35 * (M_PI / 180);
         virtual float	GetUnk36_Float(void);		// return unk36 * (M_PI / 180);
         virtual UInt16	GetUnitsFromWater();				// return unk38;
         virtual void	SetUnitsFromWater(UInt16 data);		// unk38 = data;
         virtual UInt32	GetUnk3C(void);				// return unk3C;
         virtual void	SetUnk3C(UInt32 data);		// unk3C = data;
         virtual float	GetPosRange();				// return unk40;
         virtual bool	SetPosRange(float data);		// if((data < 0) || (data > 512)) return false;
                                                   // unk40 = data; return true;
         virtual float	GetHeightRange(); // 60				// return unk44;
         virtual bool	SetHeightRange(float data);		// if((data < 0) || (data > 1)) return false;
                                                   // unk44 = data; return true;
         virtual float	GetColorRange();				// return unk48;
         virtual bool	SetColorRange(float data);		// if((data < 0) || (data > 1)) return false;
                                                   // unk48 = data; return true;
         virtual float	GetWavePeriod();				// return unk4C;
         virtual bool	SetWavePeriod(float data);		// if(data <= 0) return false;
                                                   // unk4C = data; return true;
         virtual bool	GetFlagVertexLighting();			// return (unk50 & 1) != 0;
         virtual void	SetFlagVertexLighting(UInt8 data);	// if(data) unk50 |= 1; else unk50 &= ~1;
         virtual bool	GetFlagUniformScaling();			// return (unk50 & 2) != 0;
         virtual void	SetFlagUniformScaling(UInt8 data);	// if(data) unk50 |= 2; else unk50 &= ~2;
         virtual bool	GetFlagFitToSlope();			// return (unk50 & 4) != 0;
         virtual void	SetFlagFitToSlope(UInt8 data);	// if(data) unk50 |= 4; else unk50 &= ~4;
         
         enum Flags {
            kGrassFlag_VertexLighting = 0x01,
            kGrassFlag_UniformScaling = 0x02,
            kGrassFlag_FitToSlope     = 0x04,
         };

         struct Data { // DATA subrecord
            //
            // TODO: most of the virtual members above just pluck values from here; now that we've 
            // updated this based on the form's loader code, inspect and update those functions.
            //
            UInt8  density;  // 00
            UInt8  slopeMin; // 01
            UInt8  slopeMax; // 02
            UInt8  pad03;
            UInt16 unitsFromWater; // 04
            UInt16 pad06;
            UInt32 unitsFromWaterType; // 08 // enum
            float  posRange; // 0C
            float  heightRange; // 10
            float  colorRange; // 14
            float  wavePeriod; // 18
            UInt8  flags; // 1C
            UInt8  pad1D[3]; // 1D
         };
         //
         TESModel	model; // 20
         Data     data;  // 34
   };
}