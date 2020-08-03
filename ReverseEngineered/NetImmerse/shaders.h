#pragma once
#include "objects.h"

class BSShaderMaterialBase;
class BSLightingShaderMaterial;
namespace RE {
   class NiProperty : public NiObjectNET {
      public:
         enum class type : int32_t {
            alpha = 0,
            shade = 1,
         };

         virtual type GetType(void);
   };

   class NiAlphaProperty : public NiProperty {
      protected:
         inline bool _get_flag(uint8_t i) const noexcept { return (this->alphaFlags >> i) & 1; };
         inline void _mod_flag(uint8_t i, bool s) noexcept { this->alphaFlags = (this->alphaFlags & ~(1 << i)) | ((s ? 1 : 0) << i); }
         inline uint8_t _get_bits(uint8_t i, uint8_t mask) const noexcept { return (this->alphaFlags >> i) & mask; }
         inline void    _set_bits(uint8_t i, uint8_t mask, uint8_t v) noexcept { this->alphaFlags = (this->alphaFlags & ~mask) | (v << i); }
         //
      public:
         static constexpr uint32_t vtbl = 0x0107D61C;
         enum blend_mode {
            one,
            zero,
            source_color,
            source_color_inverse,
            destination_color,
            destination_color_inverse,
            source_alpha,
            source_alpha_inverse,
            destination_alpha,
            destination_alpha_inverse,
            source_alpha_saturate,
         };
         enum test_mode {
            always,
            less,
            equal,
            less_or_equal,
            greater,
            not_equal,
            greater_or_equal,
            never,
         };
         //
         uint16_t alphaFlags;     // 18
         uint16_t alphaThreshold; // 1A
         //
         static NiAlphaProperty* make();
         //
         MEMBER_FN_PREFIX(NiAlphaProperty);
         DEFINE_MEMBER_FN(Constructor, NiAlphaProperty&, 0x00461440);
         //
         inline bool get_alpha_blending_enabled() const noexcept { return this->_get_flag(0); }
         inline void set_alpha_blending_enabled(bool s) noexcept { this->_mod_flag(0, s); }
         inline blend_mode get_source_blend_mode() const noexcept { return (blend_mode)this->_get_bits(1, 0b1111); }
         inline void       set_source_blend_mode(blend_mode bm) noexcept { this->_set_bits(1, 0b1111, (uint8_t)bm); }
         inline blend_mode get_destination_blend_mode() const noexcept { return (blend_mode)this->_get_bits(5, 0b1111); }
         inline void       set_destination_blend_mode(blend_mode bm) noexcept { this->_set_bits(5, 0b1111, (uint8_t)bm); }
         inline bool get_alpha_testing_enabled() const noexcept { return this->_get_flag(9); }
         inline void set_alpha_testing_enabled(bool s) noexcept { this->_mod_flag(9, s); }
         inline test_mode get_alpha_test_mode() const noexcept { return (test_mode)this->_get_bits(10, 0b111); }
         inline void      set_alpha_test_mode(test_mode bm) noexcept { this->_set_bits(10, 0b111, (test_mode)bm); }
         inline bool get_no_sorting() const noexcept { return this->_get_flag(13); }
         inline void set_no_sorting(bool s) noexcept { this->_mod_flag(13, s); }
   };

   class NiShadeProperty : public NiProperty {
   };

   class BSShaderProperty : public NiShadeProperty {
      public:
         static constexpr uint32_t vtbl = 0x0114EB18;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA743C;
         //
      public:
	      enum ShaderFlags1 {
		      kSLSF1_Specular						= 1 << 0,
		      kSLSF1_Skinned						= 1 << 1,
		      kSLSF1_Temp_Refraction				= 1 << 2,
		      kSLSF1_Vertex_Alpha					= 1 << 3,
		      kSLSF1_Greyscale_To_PaletteColor	= 1 << 4,
		      kSLSF1_Greyscale_To_PaletteAlpha	= 1 << 5,
		      kSLSF1_Use_Falloff					= 1 << 6,
		      kSLSF1_Environment_Mapping			= 1 << 7,
		      kSLSF1_Recieve_Shadows				= 1 << 8,
		      kSLSF1_Cast_Shadows					= 1 << 9,
		      kSLSF1_Facegen_Detail_Map			= 1 << 10,
		      kSLSF1_Parallax						= 1 << 11,
		      kSLSF1_Model_Space_Normals			= 1 << 12,
		      kSLSF1_Non_Projective_Shadows		= 1 << 13,
		      kSLSF1_Landscape					= 1 << 14,
		      kSLSF1_Refraction					= 1 << 15,
		      kSLSF1_Fire_Refraction				= 1 << 16,
		      kSLSF1_Eye_Environment_Mapping		= 1 << 17,
		      kSLSF1_Hair_Soft_Lighting			= 1 << 18,
		      kSLSF1_Screendoor_Alpha_Fade		= 1 << 19,
		      kSLSF1_Localmap_Hide_Secret			= 1 << 20,
		      kSLSF1_FaceGen_RGB_Tint				= 1 << 21,
		      kSLSF1_Own_Emit						= 1 << 22,
		      kSLSF1_Projected_UV					= 1 << 23,
		      kSLSF1_Multiple_Textures			= 1 << 24,
		      kSLSF1_Remappable_Textures			= 1 << 25,
		      kSLSF1_Decal						= 1 << 26,
		      kSLSF1_Dynamic_Decal				= 1 << 27,
		      kSLSF1_Parallax_Occlusion			= 1 << 28,
		      kSLSF1_External_Emittance			= 1 << 29,
		      kSLSF1_Soft_Effect					= 1 << 30,
		      kSLSF1_ZBuffer_Test					= 1 << 31
	      };
	      enum ShaderFlags2 {
		      kSLSF2_ZBuffer_Write					= 1 << 0,
		      kSLSF2_LOD_Landscape					= 1 << 1,
		      kSLSF2_LOD_Objects						= 1 << 2,
		      kSLSF2_No_Fade							= 1 << 3,
		      kSLSF2_Double_Sided						= 1 << 4,
		      kSLSF2_Vertex_Colors					= 1 << 5,
		      kSLSF2_Glow_Map							= 1 << 6,
		      kSLSF2_Assume_Shadowmask				= 1 << 7,
		      kSLSF2_Packed_Tangent					= 1 << 8,
		      kSLSF2_Multi_Index_Snow					= 1 << 9,
		      kSLSF2_Vertex_Lighting					= 1 << 10,
		      kSLSF2_Uniform_Scale					= 1 << 11,
		      kSLSF2_Fit_Slope						= 1 << 12,
		      kSLSF2_Billboard						= 1 << 13,
		      kSLSF2_No_LOD_Land_Blend				= 1 << 14,
		      kSLSF2_EnvMap_Light_Fade				= 1 << 15,
		      kSLSF2_Wireframe						= 1 << 16,
		      kSLSF2_Weapon_Blood						= 1 << 17,
		      kSLSF2_Hide_On_Local_Map				= 1 << 18,
		      kSLSF2_Premult_Alpha					= 1 << 19,
		      kSLSF2_Cloud_LOD						= 1 << 20,
		      kSLSF2_Anisotropic_Lighting				= 1 << 21,
		      kSLSF2_No_Transparency_Multisampling	= 1 << 22,
		      kSLSF2_Unused01							= 1 << 23,
		      kSLSF2_Multi_Layer_Parallax				= 1 << 24,
		      kSLSF2_Soft_Lighting					= 1 << 25,
		      kSLSF2_Rim_Lighting						= 1 << 26,
		      kSLSF2_Back_Lighting					= 1 << 27,
		      kSLSF2_Unused02							= 1 << 28,
		      kSLSF2_Tree_Anim						= 1 << 29,
		      kSLSF2_Effect_Lighting					= 1 << 30,
		      kSLSF2_HD_LOD_Objects					= 1 << 31
	      };
         //
      public:
         virtual UInt32 Unk_21(); // always returns 1 // not overridden by BSLightingShaderProperty
         virtual void   Unk_22(UInt32);
         virtual bool   Unk_23(UInt32); // always returns true
         virtual void   Unk_24(UInt32, UInt32, UInt32) = 0;
         virtual UInt32 Unk_25(UInt32, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_26(UInt32, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_27(UInt32); // always returns 1
         virtual UInt32 Unk_28(UInt32); // always returns 0
         virtual bool   Unk_29(UInt32); // most likely an operator== overload
         virtual void   Unk_2A(UInt32); // no-op
         virtual float  Unk_2B(); // always returns 1.0F on this class; on BSLightingShaderProperty, returns this->unk3C->unk54
         virtual UInt32 Unk_2C(UInt32); // always returns 1
         virtual void   Unk_2D();
         virtual UInt32 Unk_2E(); // always returns 0 on this class; on BSLightingShaderProperty, always returns 0xB
         virtual UInt32 Unk_2F(NiGeometry* owner, UInt32, UInt32); // always returns 0
         virtual UInt32 Unk_30(); // always returns 0 on this class; on BSLightingShaderProperty, returns this->unk3C->unk3C
         virtual UInt32 Unk_31(UInt32); // always returns 0
         virtual bool   Unk_32(); // always returns false
         virtual void   Unk_33(); // no-op
         virtual void   Unk_34(UInt32, UInt32, UInt32, UInt32, UInt32, UInt32); // no-op
         virtual UInt32 Unk_35(); // always returns 0
         virtual UInt32 Unk_36(); // always returns 0 on this class; on BSLightingShaderProperty, always returns 2
         //
         float  alpha = 1.0F; // 18 // confirmed to be emissive alpha. all alpha?
         UInt32 unk1C = 0x7FFFFFFF;
         UInt32 shaderFlags1 = 0; // 20
         UInt32 shaderFlags2 = 0; // 24
         UInt32 unk28 = 0;
         UInt32 unk2C = 0;
         BSFadeNode* esp30; // set by the constructor to some unknown node at a static pointer
         UInt32 unk34 = 0;
         tArray<UInt32>* unk38 = nullptr; // BSLightingShaderProperty sets this to its own unk98; BSGrassShaderProperty sets this to its own unkC0
         BSShaderMaterialBase* unk3C = nullptr; // type varies depending on shader type
         UInt32 unk40 = 0;
         UInt32 unk44;
         //
         MEMBER_FN_PREFIX(BSShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSShaderProperty&, 0x00C6F9F0);
         DEFINE_MEMBER_FN(HasShaderFlag, bool, 0x0045C640, UInt8 flagIndex); // tests for flag (1 << flagIndex); if flagIndex < 32 then it's in shaderFlags1; else, tests for (1 << (flagIndex - 32)) in shaderFlags2
         DEFINE_MEMBER_FN(SetShaderFlag, void, 0x00C6EEA0, UInt8 flagIndex, bool);
         DEFINE_MEMBER_FN(Subroutine00C6F920, void, 0x00C6F920, BSLightingShaderMaterial*, UInt32); // called by BSGrassShaderProperty constructor with args (this->unk3C, 1)
   };
   static DEFINE_SUBROUTINE(void, SetBSShaderFlags, 0x00C6F2F0, NiAVObject*, uint8_t flagIndex, bool, bool mustBeLightingShader); // if you pass in a NiNode, recurses on all children; for a NiGeometry, sets the shader flag
   //
   class BSLightingShaderProperty : public BSShaderProperty { // sizeof == 0xB0
      public:
         static constexpr uint32_t vtbl = 0x01153680;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA82E0;
         //
      public:
         //
         // This class does not define new virtual member functions, but does override some of them.
         //
         virtual void Unk_24(UInt32, UInt32, UInt32) override;
         //
         UInt32 unk48 = 0;
         UInt32 unk4C = 0;
         UInt32 unk50 = 0;
         UInt32 unk54 = 0;
         UInt32 unk58 = 0;
         void*  unk5C; // three floats; possibly RGB
         float  unk60 = 1.0F;
         UInt32 unk64 = 0;
         float  unk68 = 1.0F;
         float  unk6C = 1.0F;
         UInt32 unk70 = 0;
         UInt32 unk74 = 0;
         UInt32 unk78 = 0;
         UInt32 unk7C = 0;
         float  unk80;
         float  unk84;
         float  unk88;
         float  unk8C = 1.0F;
         SInt32 unk90 = -1;
         UInt16 unk94 = 0xFFFF;
         UInt8  unk96 = 1;
         UInt8  unk97 = 2;
         tArray<UInt32> unk98; // unknown type
         UInt32 unkA4;
         UInt32 unkA8;
         UInt32 unkAC;
         //
         MEMBER_FN_PREFIX(BSLightingShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSLightingShaderProperty&, 0x00C8C350);
         DEFINE_MEMBER_FN(Subroutine004611B0, void, 0x004611B0, UInt32);
         DEFINE_MEMBER_FN(Subroutine00C6EEA0, void, 0x00C6EEA0, UInt32, UInt32);
         DEFINE_MEMBER_FN(Subroutine00C89DD0, void, 0x00C89DD0, UInt32); // Unk_23 wraps this
   };
   static_assert(sizeof(BSLightingShaderProperty) >= 0xB0, "RE::BSLightingShaderProperty is too small!");
   static_assert(sizeof(BSLightingShaderProperty) <= 0xB0, "RE::BSLightingShaderProperty is too large!");
   //
   class BSGrassShaderProperty : public BSLightingShaderProperty { // sizeof == 0xE0
      public:
         static constexpr uint32_t vtbl = 0x011538A8;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA8304;
         //
      public:
         virtual void* GetUnkB4();       // 37 // returns this->unkB4
         virtual void  SetUnkB4(void*); // 38 // sets this->unkB4
         //
         UInt32 unkB0 = 0;
         void*  unkB4; // warning: this is not initialized by the constructor!
         UInt32 unkB8;
         UInt32 unkBC = 0;
         tArray<UInt32> unkC0; // type unknown
         UInt32 unkCC;
         UInt32 unkD0;
         UInt32 unkD4;
         UInt32 unkD8 = 0;
         UInt32 unkDC;
         //
         MEMBER_FN_PREFIX(BSGrassShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSGrassShaderProperty&, 0x00C8CB10);
         DEFINE_MEMBER_FN(Destructor, void, 0x00C8CAB0);
   };
   static_assert(sizeof(BSGrassShaderProperty) >= 0xE0, "RE::BSGrassShaderProperty is too small!");
   static_assert(sizeof(BSGrassShaderProperty) <= 0xE0, "RE::BSGrassShaderProperty is too large!");
   //
   class BSDistantTreeShaderProperty : public BSShaderProperty { // sizeof == 0x58
      public:
         static constexpr uint32_t vtbl = 0x011561A8;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA8824;
         //
      public:
         UInt32 unk48 = 0x4B;
         NiPointer<NiRefObject> unk4C = nullptr;
         UInt32 unk50 = 0;
         UInt32 unk54;
         //
         MEMBER_FN_PREFIX(BSDistantTreeShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSDistantTreeShaderProperty&, 0x00CAE810);
   };

   class BSEffectShaderProperty : public BSShaderProperty { // sizeof == 0x50
      public:
         static constexpr uint32_t vtbl = 0x01154358;
         static constexpr NiRTTI*  rtti = (NiRTTI*)0x01BA849C;
         //
         uint32_t unk48 = 0;
         uint32_t unk4C;
         //
         static BSEffectShaderProperty* make();
         NiColorA* GetEmissiveColor();
         //
         MEMBER_FN_PREFIX(BSEffectShaderProperty);
         DEFINE_MEMBER_FN(Constructor, BSEffectShaderProperty&, 0x00C9A890);
   };
   static_assert(sizeof(BSEffectShaderProperty) >= 0x50, "RE::BSEffectShaderProperty is too small!");
   static_assert(sizeof(BSEffectShaderProperty) <= 0x50, "RE::BSEffectShaderProperty is too large!");
}