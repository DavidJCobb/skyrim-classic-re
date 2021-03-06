#include "ReverseEngineered/Forms/Actor.h"
#include "ReverseEngineered/Forms/BaseForms/TESNPC.h"
#include "ReverseEngineered/Player/PlayerCharacter.h"
#include "skse/GameData.h"
#include "skse/GameExtraData.h"
#include "skse/GameRTTI.h"
#include "skse/NiNodes.h"

namespace RE {
   DEFINE_SUBROUTINE(NiAVObject*, CreateWeaponNode, 0x0046F530, TESModelTextureSwap*, UInt32 bodyPartIndex, Actor*, NiPointer<NiNode>& bodyPartSlotRenderedArmorPtr, NiNode* rootNode);

   void Struct006F0580::ClearUnk00() {
      auto player = *g_thePlayer;
      if (player) {
         if (player->unk404 == this->unk00)
            player->unk404 = nullptr;
      }

      this->unk00 = nullptr;
   };
   //
   void Actor::AddToFaction(TESFaction* faction) {
      if (!this->IsInFaction(faction))
         CALL_MEMBER_FN(this, SetFactionRank)(faction, 0);
   };
   bool Actor::IsAIEnabled() {
      return (this->flags1 & Actor::kFlags_AIEnabled) == Actor::kFlags_AIEnabled;
   };
   bool Actor::IsLeveledActor() {
      return CALL_MEMBER_FN((RE::BaseExtraList*)&this->extraData, GetExtraLeveledCreature)() != nullptr;
   };
   void Actor::UpdateHairColor() {
      TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
      if (npc && npc->headData) {
         BGSColorForm * hairColor = npc->headData->hairColor; // Non-player actors won't have this
         if (hairColor) {
            NiColorA val;
            val.r = hairColor->color.red / 128.0;
            val.g = hairColor->color.green / 128.0;
            val.b = hairColor->color.blue / 128.0;
            NiColorA * color = &val;
            //
            NiNode * model = GetNiRootNode(0);
            if (model)
               UpdateModelHair((::NiNode*)model, &color);
         }
      }
   }
   void Actor::UpdateSkinColor() {
      TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
      if (npc) {
         NiColorA val;
         val.r = npc->color.red / 255.0;
         val.g = npc->color.green / 255.0;
         val.b = npc->color.blue / 255.0;
         NiColorA * color = &val;
         //
         NiNode * thirdPerson = GetNiRootNode(0);
         NiNode * firstPerson = GetNiRootNode(1);
         if (thirdPerson) {
            UpdateModelSkin((::NiNode*)thirdPerson, &color); // Update for 3rd Person
         }
         if (firstPerson) {
            UpdateModelSkin((::NiNode*)firstPerson, &color); // Update for 1st Person
         }
      }
   }
   bool Actor::VisitFactions(FactionVisitor& visitor) {
      TESNPC* npc = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
      if (npc) {
         for (UInt32 i = 0; i < npc->actorData.factions.count; i++) {
            TESActorBaseData::FactionInfo info;
            npc->actorData.factions.GetNthItem(i, info);
            if (visitor.Accept(info.faction, info.rank))
               return true;
         }
         //
         ExtraFactionChanges* pFactionChanges = static_cast<ExtraFactionChanges*>(extraData.GetByType(kExtraData_FactionChanges));
         if (pFactionChanges) {
            for (UInt32 i = 0; i < pFactionChanges->factions.count; i++) {
               ExtraFactionChanges::FactionInfo info;
               pFactionChanges->factions.GetNthItem(i, info);
               if (visitor.Accept(info.faction, info.rank))
                  return true;
            }
         }
      }
      return false;
   }

   void Actor::SetHeadTracking(bool abEnabled) {
      if (abEnabled)
         this->actorState.flags08 |= ActorState::kFlag_EnableHeadtracking;
      else
         this->actorState.flags08 &= ~ActorState::kFlag_EnableHeadtracking;
   };
};