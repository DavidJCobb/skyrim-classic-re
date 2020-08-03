#include "shaders.h"
#include "skse/NiMaterial.h"
#include "skse/NiRTTI.h"

namespace RE {
   /*static*/ NiAlphaProperty* NiAlphaProperty::make() {
      auto instance = (NiAlphaProperty*)FormHeap_Allocate(sizeof(NiAlphaProperty));
      CALL_MEMBER_FN(instance, Constructor)();
      return instance;
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