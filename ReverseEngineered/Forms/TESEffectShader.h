#pragma once
#include "skse/GameForms.h"
#include "ReverseEngineered\Shared.h"
#include "ReverseEngineered\Forms\TESForm.h"
#include "ReverseEngineered\Forms\FormComponents\TESTexture.h"

namespace RE {
   class TESEffectShader;
   class TESObjectREFR;

   //
   // Before calling this, Papyrus verifies:
   //
   //  - That the target is not null
   //  - That the target is in a loaded cell
   //  - That the target has 3D
   //
   DEFINE_SUBROUTINE_EXTERN(void, PlayEffectShaderOn, 0x004DDE90, TESObjectREFR* target, TESEffectShader* shader, float duration, UInt32 zero4, UInt32 zero5, UInt32 zero6, UInt32 zero7);

   class TESEffectShader : public TESForm {
      public:
         enum { kTypeID = kFormType_EffectShader }; // needed for SKSE Papyrus integration
         static constexpr form_type_t form_type = form_type::effect_shader;

         enum BlendMode : UInt32 {
            kBlendMode_Unused0 = 0,
            kBlendMode_Zero,
            kBlendMode_One,
            kBlendMode_SourceColor,
            kBlendMode_SourceInvertedColor,
            kBlendMode_SourceAlpha,
            kBlendMode_SourceInvertedAlpha,
            kBlendMode_DestinationAlpha,
            kBlendMode_DestinationInvertedAlpha,
            kBlendMode_DestinationColor,
            kBlendMode_DestinationInvertedColor,
            kBlendMode_SourceAlphaSAT,
         };
         enum BlendOperation : UInt32 {
            kBlendMOperation_Unused0 = 0,
            kBlendOperation_Add,
            kBlendOperation_Subtract,
            kBlendOperation_SubtractReverse,
            kBlendOperation_Minimum,
            kBlendOperation_Maximum,
         };
         enum ZTest : UInt32 {
            kZTestFunction_EqualTo = 3,
            kZTestFunction_Normal  = 4,
            kZTestFunction_Greater = 5,
            kZTestFunction_GreaterOrEqual = 7,
            kZTestFunction_AlwaysShow = 8,
         };

         enum ShaderFlags {
            kShaderFlag_NoMembrane = 1,
            kShaderFlag_GreyscaleMembraneColor = 2,
            kShaderFlag_GreyscaleMembraneAlpha = 4,
            kShaderFlag_NoParticle = 8,
            kShaderFlag_EdgeEffectInverse = 0x0010,
            kShaderFlag_SkinOnly = 0x0020,
            kShaderFlag_IgnoreAlpha = 0x0040,
            kShaderFlag_ProjectUVs = 0x0080,
            kShaderFlag_IgnoreBaseGeometryAlpha = 0x0100,
            kShaderFlag_Lighting = 0x0200, // ???
            kShaderFlag_DontAffectWeapons = 0x0400,
            //
            kShaderFlag_ParticleAnimated = 0x8000,
            kShaderFlag_GreyscaleParticleColor = 0x10000,
            kShaderFlag_GreyscaleParticleAlpha = 0x20000,
            //
            kShaderFlag_UseBloodGeometry = 0x1000000,
         };

         struct Data { // sizeof == 0x190
            UInt8  unk000;
            UInt8  pad001; // these three padding bytes correspond to the "Unknown" entry in the DATA shown in xEdit.
            UInt8  pad002;
            UInt8  pad003;
            BlendMode      membraneSourceBlendMode = (BlendMode)5; // 004 // enum
            BlendOperation membraneBlendOperation  = (BlendOperation)1; // 008 // enum
            ZTest          membraneZTestFunction   = (ZTest)3; // 00C // enum
            UInt32 fillColorKey1            = 0;   // 010 // 0xAABBGGRR, alpha unused
            float  fillAlphaTimeFadeIn      = 0;   // 014
            float  fillAlphaTimeFull        = 0;   // 018
            float  fillAlphaTimeFadeOut     = 0;   // 01C
            float  fillAlphaRatioPersistent = 0;   // 020
            float  fullAlphaPulseAmplitude  = 0;   // 024
            float  fillAlphaPulseFrequency  = 1.0; // 028
            float  fillTextureSpeedU        = 0;   // 02C
            float  fillTextureSpeedV        = 0;   // 030
            float  edgeFalloff              = 1.0; // 034
            UInt32 edgeColor                = 0;   // 038 // 0xAABBGGRR, alpha unused
            float  edgeAlphaTimeFadeIn      = 0;   // 03C
            float  edgeAlphaTimeFull        = 0;   // 040
            float  edgeAlphaTimeFadeOut     = 0;   // 044
            float  edgeAlphaRatioPersistent = 0;   // 048
            float  edgeAlphaPulseAmplitude  = 0;   // 04C
            float  edgeAlphaPulseFrequency  = 1.0; // 050
            float  fillAlphaRatioFull       = 1.0; // 054
            float  edgeAlphaRatioFull       = 1.0; // 058
            BlendMode membraneDestinationBlendMode = (BlendMode)6; // 05C // enum
            BlendMode      particleSourceBlendMode = (BlendMode)5; // 060 // enum
            BlendOperation particleBlendOperation  = (BlendOperation)1; // 064 // enum
            ZTest          particleZTestFunction   = (ZTest)4; // 068 // enum
            UInt32 particleDestinationBlendMode = 6; // 06C // enum
            float  particleBirthTimeRampUp         = 0;   // 070
            float  particleBirthTimeFull           = 0;   // 074
            float  particleBirthTimeRampDown       = 0;   // 078
            float  particleBirthRatioFull          = 1.0; // 07C
            float  particleCountPersistent         = 1.0; // 080
            float  particleLifetime                = 1.0; // 084
            float  particleLifetimeVariance        = 0;   // 088
            float  particleSpeedAlongNormalInitial = 0;   // 08C
            float  particleAccelerationAlongNormal = 0;   // 090
            NiPoint3 particleVelocityInitial;             // 094
            NiPoint3 particleAcceleration;                // 0A0
            float  particleScaleKey1               = 1.0; // 0AC
            float  particleScaleKey2               = 1.0; // 0B0
            float  particleScaleKeyTime1           = 0.0; // 0B4
            float  particleScaleKeyTime2           = 1.0; // 0B8
            UInt32 particleColorKey1        = 0x00FFFFFF; // 0BC // 0xAABBGGRR, alpha unused
            UInt32 particleColorKey2        = 0x00FFFFFF; // 0C0 // 0xAABBGGRR, alpha unused
            UInt32 particleColorKey3        = 0x00FFFFFF; // 0C4 // 0xAABBGGRR, alpha unused
            float  particleColorKeyAlpha1          = 1.0; // 0C8
            float  particleColorKeyAlpha2          = 1.0; // 0CC
            float  particleColorKeyAlpha3          = 1.0; // 0D0
            float  particleColorKeyTime1           = 0.0; // 0D4
            float  particleColorKeyTime2           = 0.5; // 0D8
            float  particleColorKeyTime3           = 1.0; // 0DC
            float  particleInitialSpeed = 0.0; // 0E0 // along normal
            float  particleInitialRotation = 0.0; // 0E4 // degrees
            float  particleInitialRotationVariance = 0.0; // 0E8 // degrees; random magnitude between 0 and this taken and added to a particle's initial rotation
            float  particleRotationSpeed = 0.0; // 0EC // degrees per second
            float  particleRotationSpeedVariance = 0.0; // 0F0 // degrees per second
            void*  addonModel = nullptr; // 0F4 // DEBR
            float  fileHolesTimeStart  = 0.0; // 0F8
            float  fillHolesTimeEnd    = 10.0; // 0FC
            float  fillHolesValueStart = 255.0;     // 100
            float  fillHolesValueEnd   = 0;         // 104
            float  edgeWidthUnknown    = 0;         // 108
            UInt32 edgeColorUnknown    = 0;         // 10C // 0xAABBGGRR, alpha unused
            float  explosionWindSpeed  = 0.0;       // 110
            UInt32 fillTextureCountU   = 1;         // 114
            UInt32 fillTextureCountV   = 1;         // 118
            float  particleAddonTimeFadeIn   = 1.0; // 11C
            float  particleAddonTimeFadeOut  = 1.0; // 120
            float  particleAddonScaleStart   = 1.0; // 124
            float  particleAddonScaleEnd     = 1.0; // 128
            float  particleAddonTimeScaleIn  = 1.0; // 12C
            float  particleAddonTimeScaleOut = 1.0; // 130
            BGSSoundDescriptorForm* ambientSound = NULL; // 134
            UInt32 fillColorKey2 = 0; // 138 // 0xAABBGGRR, alpha unused
            UInt32 fillColorKey3 = 0; // 13C // 0xAABBGGRR, alpha unused
            float  unk140 = 1.0;
            float  unk144 = 1.0;
            float  unk148 = 1.0;
            UInt32 unk14C = 0;
            UInt32 unk150 = 0;
            UInt32 unk154 = 0;
            float  unk158 = 1.0; // 158 // (fill?) color scale
            float  birthPositionOffset         = 0; // 15C
            float  birthPositionOffsetVariance = 0; // 160
            UInt32 unk164 = 0;
            UInt32 unk168 = 0;
            UInt32 unk16C = 0;
            UInt32 unk170 = 0;
            UInt32 unk174 = 0;
            UInt32 unk178 = 0;
            UInt32 unk17C = 0;
            UInt32 flags  = 0; // 180
            float  fillTextureScaleU = 1.0; // 184
            float  fillTextureScaleV = 1.0; // 188
            UInt16 unk18C = 0;
            UInt16 pad18E = 0;
         };

         Data data; // 014 // DATA subrecord, copied out of the file as one whole block
         TESTexture	textureMembraneFill;    // 1A4
         TESTexture	textureParticleShader;  // 1AC
         TESTexture	textureHoles;           // 1B4
         TESTexture	textureMembranePalette; // 1BC
         TESTexture	textureParticlePalette; // 1C4
         UInt32		unk1CC;	// BSIntrusiveRefCounted*
         UInt32		unk1D0;	// BSIntrusiveRefCounted*
   };
};