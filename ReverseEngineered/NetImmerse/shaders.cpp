#include "shaders.h"
#include "skse/NiMaterial.h"
#include "skse/NiRTTI.h"

namespace RE {
   /*static*/ NiAlphaProperty* NiAlphaProperty::make() {
      auto instance = (NiAlphaProperty*)FormHeap_Allocate(sizeof(NiAlphaProperty));
      CALL_MEMBER_FN(instance, Constructor)();
      return instance;
   }
   void NiAlphaProperty::dump() const noexcept {
      _MESSAGE("DUMPING NiAlphaProperty DATA...");
      _MESSAGE("Bits (raw): %u (0x%04X) (swapped: 0x%04X)", this->alphaFlags, this->alphaFlags, _byteswap_ushort(this->alphaFlags));
      _MESSAGE("Alpha blending is%s enabled.", this->get_alpha_blending_enabled() ? "" : " not");
      _MESSAGE("Source Mode: %u", this->get_source_blend_mode());
      _MESSAGE("Dest.n Mode: %u", this->get_destination_blend_mode());
      _MESSAGE("Alpha testing is%s enabled.", this->get_alpha_testing_enabled() ? "" : " not");
      _MESSAGE("Threshold: %u", this->alphaThreshold);
      _MESSAGE("\"No Sorting\" is %sabled.", this->get_no_sorting() ? "en" : "dis");
   }

   NiColorA* BSEffectShaderProperty::GetEmissiveColor() {
      auto material = this->unk3C;
      if (!material)
         return nullptr;
      if (material->GetShaderType() != BSShaderMaterial::kShaderType_Default)
         return nullptr;
      auto esm = (BSEffectShaderMaterial*) this->unk3C; // no RTTI, and the type value for "effect shader material" is not clear. guess we just gotta hope, lol
      return &esm->emissiveColor;
   }
   /*static*/ BSEffectShaderProperty* BSEffectShaderProperty::make() {
      auto instance = (BSEffectShaderProperty*)FormHeap_Allocate(sizeof(BSEffectShaderProperty));
      CALL_MEMBER_FN(instance, Constructor)();
      return instance;
   }
}