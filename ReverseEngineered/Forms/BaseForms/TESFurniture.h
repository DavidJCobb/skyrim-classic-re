#pragma once
#include <vector>
#include "skse/GameForms.h"
#include "skse/GameObjects.h"
#include "ReverseEngineered/Forms/BaseForms/TESObjectACTI.h"

namespace RE {
   class BSFurnitureMarkerNode;

   class TESFurniture : public TESObjectACTI {
      public:
         enum class marker_entry : uint16_t {
            none  = 0,
            front = 0x01,
            back  = 0x02,
            right = 0x04,
            left  = 0x08,
            up    = 0x10,
         };
         //
         class ModifiedMarker {
            public:
               uint32_t furnitureMarkerIndex;  // "Which marker is this?" The FURN's list of ModifiedMarkers isn't always in order.
               uint16_t unk04;
               uint16_t disabledEntryPoints;   // EntryPoints values OR'd together
               BGSKeyword* keyword;          // Marker Keyword in CK
         };
         enum { kTypeID = kFormType_Furniture };
         enum {
            kFurnitureFlag_DisablesActivation = 0x02000000,
            kFurnitureFlag_IsPerch            = 0x04000000,
            kFurnitureFlag_MustExitToTalk     = 0x08000000,
            kFurnitureFlag_10000000           = 0x10000000, // tested by member function 0x00499960
            kFurnitureFlag_CanLean            = 0x20000000,
            kFurnitureFlag_CanSit             = 0x40000000,
            kFurnitureFlag_CanSleep           = 0x80000000
         };
         //
         enum WorkbenchType : UInt8 {
            WorkbenchTypeNone, // 0
            WorkbenchTypeCreateObject,
            WorkbenchTypeSmithingWeapon,
            WorkbenchTypeEnchanting,
            WorkbenchTypeEnchantingExperiment,
            WorkbenchTypeAlchemy, // 5
            WorkbenchTypeAlchemyExperiment,
            WorkbenchTypeSmithingArmor,
         };
         enum WorkbenchSkill : UInt8 {
            WorkbenchSkillUnknown0,
            WorkbenchSkillUnknown1,
            WorkbenchSkillUnknown2,
            WorkbenchSkillUnknown3,
            WorkbenchSkillUnknown4,
            WorkbenchSkillUnknown5,
            WorkbenchSkillUnknown6,
            WorkbenchSkillTwoHanded,
            WorkbenchSkillArchery,
            WorkbenchSkillBlock,
            WorkbenchSkillSmithing, // 10
            WorkbenchSkillHeavyArmor,
            WorkbenchSkillLightArmor,
            WorkbenchSkillPickpocket,
            WorkbenchSkillLockpicking,
            WorkbenchSkillSneak, // 15
            WorkbenchSkillAlchemy,
            WorkbenchSkillSpeech,
            WorkbenchSkillAlteration,
            WorkbenchSkillConjuration,
            WorkbenchSkillDestruction, // 20
            WorkbenchSkillIllusion,
            WorkbenchSkillRestoration,
            WorkbenchSkillEnchanting,
            WorkbenchSkillNone = 0xFF
         };
         //
         struct WorkbenchData { // sizeof() == 4
            WorkbenchType  type;    // 0
            WorkbenchSkill skill;   // 1
            UInt8          pad2[2]; // 2
         };
         tArray<ModifiedMarker>  markers;         // 6C // These are just the marker changes made on the FURN. You must access the loaded 3D to find base marker data.
         WorkbenchData           workbench;       // 78
         UInt32                  furnFlags;       // 7C // MNAM
         SpellItem*              associatedSpell; // 80
         //
         inline bool IsMarkerEnabled(UInt8 markerIndex) const noexcept {
            if (markerIndex >= 25)
               return false;
            return (this->furnFlags & (1 << markerIndex)) != 0;
         };
         BGSKeyword* GetMarkerKeyword(UInt8 markerIndex) const noexcept {
            if (markerIndex >= 24)
               return nullptr;
            uint32_t size = this->markers.count;
            for (uint32_t i = 0; i < size; i++)
               if (this->markers.arr.entries[i].furnitureMarkerIndex == markerIndex)
                  return this->markers.arr.entries[i].keyword;
            return nullptr;
         };

         inline void GetEnabledMarkers(std::vector<uint8_t>& out) const noexcept {
            out.clear();
            for (uint8_t i = 0; i < 25; ++i)
               if (this->IsMarkerEnabled(i))
                  out.push_back(i);
         }
         ModifiedMarker* GetMarkerChanges(uint32_t m) const noexcept {
            uint32_t size = this->markers.count;
            for (uint32_t i = 0; i < size; ++i) {
               if (this->markers.arr.entries[i].furnitureMarkerIndex == m)
                  return &this->markers.arr.entries[i];
            }
            return nullptr;
         }

         MEMBER_FN_PREFIX(TESFurniture);
         DEFINE_MEMBER_FN(CanLean,            bool, 0x00499980);
         DEFINE_MEMBER_FN(CanSit,             bool, 0x00499920);
         DEFINE_MEMBER_FN(CanSleep,           bool, 0x00499940);
         DEFINE_MEMBER_FN(GetFurnitureMarkerNode, BSFurnitureMarkerNode*, 0x00499C58, NiNode* node); // technically a static method
         DEFINE_MEMBER_FN(IsMarkerEnabled,    bool, 0x004999C0, UInt8 markerIndex); // tests (flags)
         DEFINE_MEMBER_FN(MustExitToTalk,     bool, 0x004999A0);
         DEFINE_MEMBER_FN(Subroutine004999E0, void, 0x004999E0, UInt32); // render furniture markers? sole caller is TESFurniture::Unk_4B
         DEFINE_MEMBER_FN(Subroutine00499CE0, void, 0x00499CE0, UInt32, bool, UInt32, UInt32);
         DEFINE_MEMBER_FN(Subroutine004EC4A0, void, 0x004EC4A0, void*);
   };
};