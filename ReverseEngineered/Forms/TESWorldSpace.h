#pragma once
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Forms/TESForm.h"

class TESWorldSpace;
namespace RE {
   class TESObjectCELL; // Forward-declare instead of #including, so the compiler doesn't choke on a circular dependency
   class TESObjectREFR;
   class TESWaterForm;

   struct MapMarkerOperation {
      UInt32 unk00;
      tArray<UInt32> markerHandles; // 04
      UInt32 unk10;
      UInt32 unk14; // show/hide
      UInt32 unk18; // enable travel?
      UInt32 unk1C;
      UInt32 unk20; // affect already-hidden markers?
   };

   class TESWorldSpace;
   typedef bool(*MapMarkerIteratorCallback)(RE::TESWorldSpace* currentWorld, RE::TESObjectREFR* ref, RE::ExtraMapMarker::Data*, void* state);

   class TESWorldSpace : public TESForm {
      public:
         operator ::TESWorldSpace*() const { return (::TESWorldSpace*) this; }
         enum { kTypeID = kFormType_WorldSpace };
         //
         // Parents:
         TESFullName fullName; // 14
         TESTexture  texture;  // 1C

         enum ParentFlags : UInt16 {
            kParentFlag_UseLandData       = 0x0001,
            kParentFlag_UseLODData        = 0x0002,
            kParentFlag_DontUseMapData    = 0x0004,
            kParentFlag_UseWaterData      = 0x0008,
            kParentFlag_UseClimateData    = 0x0010,
            kParentFlag_UseImagespaceData = 0x0020,
            kParentFlag_UseSkyCell        = 0x0040,
         };

         struct UnkRecurring { // sizeof == 0x10
            //
            // This is BSTHashMap; see below
            //
            UInt32 unk00 = 0;
            UInt32 unk04 = 0;
            UInt32 unk08 = 0; // actually an int
            void*  unk0C = (void*) 0x012456E8; // *member == 0xDEADBEEF
         };
         UInt32 unk24;
         UInt32 unk28;
         UInt32 unk2C;
         UnkRecurring unk30;
         UInt32 unk40;
         void*  unk44; // points to 0xDEADBEEF
         UInt32 unk48;
         UInt32 unk4C = 0; // 4C // related to loaded cells and CRCs
         TESObjectCELL* persistentCell = nullptr; // 50 // related to map markers
         UInt32 unk54 = 0; // 54 // related to LOD; getter at 4F0960 traverses parent worlds, checking kParentFlag_UseLODData, for this
         UInt32 unk58 = 0;
         UInt8  worldFlags = 0; // 5C // flags (DATA subrecord); flag 1 means form ID is in the "hardcoded" space (i.e. form ID <= 0x7FF)
         UInt8  unk5D;
         UInt16 parentFlags = 0; // 5E
         SInt16 worldCenterCellX = 0; // 60 // WCTR subrecord X
         SInt16 worldCenterCellY = 0; // 62 // WCTR subrecord Y
         UInt32 unk64;
         UInt32 unk68;
         UnkRecurring unk6C;
         UInt32 unk7C;
         UInt32 unk80 = 0;
         tArray<UInt32> unk84;
         UInt32 unk90 = 0;
         UInt32 unk94 = 0;
         UInt32 unk98;
         UInt32 unk9C;
         UnkRecurring unkA0;
         UInt32 unkB0;
         UInt32 unkB4 = 0;
         void*  unkB8 = nullptr; // NiRefObject* subclass?
         UInt32 unkBC;
         UInt32 unkC0;
         UInt32 unkC4 = 0;
         UInt32 unkC8 = 0;
         TESWorldSpace* parentWorld = nullptr; // CC // related to map markers
         UInt32 unkD0 = 0;
         TESWaterForm* waterType = nullptr; // D4 // subrecord NAM2
         UInt32 unkD8 = 0;
         UInt32 unkDC = 0;
         UInt32 unkE0 = 0;
         struct { // sizeof == 0x1C
            SInt32 sizeX;
            SInt32 sizeY;
            SInt16 northwestX;
            SInt16 northwestY;
            SInt16 southeastX;
            SInt16 southeastY;
            float  cameraMinHeight = 50000.0F;
            float  cameraMaxHeight = 80000.0F;
            float  cameraInitialPitch = 50.0F;
         } mapData; // E4 // MNAM
         struct { // sizeof == 0x10
            float scale;
            float cellX;
            float cellY;
            float cellZ;
         } worldMapOffset; // 100 // ONAM
         UInt32 unk110 = 0; // 110 // ZNAM: Music
         float  unk114 = FLT_MAX; // NAM0 bounds
         float  unk118 = FLT_MAX; // NAM0 bounds
         float  unk11C = FLT_MIN; // NAM9 bounds
         float  unk120 = FLT_MIN; // NAM9 bounds
         UInt32 unk124;
         UInt32 unk128;
         UnkRecurring unk12C; // 12C
         UInt32 unk13C;
         UInt32 unk140 = 0;
         BSString unk144;
         float  defaultLandHeight  = -2048.0F; // 14C // DNAM
         float  defaultWaterHeight =     0.0F; // 150 // DNAM
         float  distantLODMult = 1.0F; // 154 // NAMA subrecord
         BGSEncounterZone* unk158 = nullptr;
         UInt32 unk15C = 0;
         TESTexture treeCanopyShadow; // 160 // NNAM
         TESTexture unk168; // 160
         struct Struct004AD6A0 { // sizeof == 0x60
            //
            // Per aers, the recurring structures in here are BSTHashMaps; 
            // the DEADBEEF pointer is a sentinel value for them; the 
            // structs are:
            //
            // UInt32 size
            // UInt32 freecount
            // UInt32 offset
            // uintptr_t sentinel
            // <some alignment padding probably>
            // uintptr_t entries
            //
            // In the SKSE source, these are called tHashSets.
            //
            // In this case, again per aers, these hashmaps are often used 
            // for TESObjectREFRs -- but this is based on their Skyrim Special 
            // work, and that game has five of these hashmaps.
            //
            // Somewhere in the worldspace, there should be a map of TESFile 
            // pointers to "some struct, struct+0x18" where the struct defines 
            // the offset into the file at which the WRLD record appears.
            //
            UInt32 unk00;
            UInt32 unk04;
            UInt32 unk08 = 0;
            UInt32 unk0C = 0;
            UInt32 unk10 = 0;
            void*  unk14 = (void*) 0x01243754; // *member == 0xDEADBEEF
            UInt32 unk18;
            UInt32 unk1C = 0;
            UInt32 unk20;
            UInt32 unk24;
            UInt32 unk28 = 0;
            UInt32 unk2C = 0;
            UInt32 unk30 = 0;
            void*  unk34 = (void*)0x01243754; // *member == 0xDEADBEEF
            UInt32 unk38;
            UInt32 unk3C = 0;
            UInt32 unk40;
            UInt32 unk44;
            UInt32 unk48 = 0;
            UInt32 unk4C = 0;
            UInt32 unk50 = 0;
            void*  unk54 = (void*)0x01243754; // *member == 0xDEADBEEF
            UInt32 unk58;
            UInt32 unk5C = 0;
         } unk170; // 170
         float northRotation = 0; // 1D0
         void* unk1D4 = nullptr; // 1D4 // MHDT subrecord
         //
         MEMBER_FN_PREFIX(TESWorldSpace);
         DEFINE_MEMBER_FN(GetMapMarkerHandles,        void,               0x004F2450, MapMarkerOperation* out, bool shouldBeFalse); // if the bool is true, we pass the markers to DataHandler?
         DEFINE_MEMBER_FN(GetUnk2CFlagFlag00000010,   bool,               0x004C0C50); 
         DEFINE_MEMBER_FN(GetCellAtMergedCoordinates, RE::TESObjectCELL*, 0x004375D0, UInt32); // Argument is: 0xXXXXYYYY.
         DEFINE_MEMBER_FN(GetCellThatContainsPoint,   RE::TESObjectCELL*, 0x004F3410, NiPoint3*); // Assumed, but it's a pretty good assumption to make
         DEFINE_MEMBER_FN(GetWaterLevel,              float,              0x004F0930); // There is also a similar subroutine at 0x004F0900, but I don't know what it returns.
         //
         DEFINE_MEMBER_FN(AddToPersistentCell,       void,           0x004F5600, TESObjectREFR*);
         DEFINE_MEMBER_FN(RemoveFromPersistentCell,  void,           0x004F0A80, TESObjectREFR*); // purpose assumed; needs verification
         DEFINE_MEMBER_FN(GetOrCreatePersistentCell, TESObjectCELL*, 0x004F4420);
         DEFINE_MEMBER_FN(SetPersistentCell,         void,           0x004F3F10, TESObjectCELL*);
         //
         DEFINE_MEMBER_FN(GetDefaultLandHeight, float, 0x004F0900); // traverses up the parent worldspaces based on parentFlags
         DEFINE_MEMBER_FN(GetWaterHeight,       float, 0x004F0930); // returns defaultWaterHeight; traverses up the parent worldspaces based on parentFlags
         DEFINE_MEMBER_FN(GetWaterType, TESWaterForm*, 0x004F08A0); // returns defaultWaterHeight; traverses up the parent worldspaces based on parentFlags
         DEFINE_MEMBER_FN(Subroutine004F5330,   void*, 0x004F5330, UInt32, UInt32); // Arguments are cell coordinates.

         void ForEachMapMarker(MapMarkerIteratorCallback, void* state); // all map markers in all worlds found by GetAllRelatedWorldspaces
         void GetAllRelatedWorldspaces(std::vector<TESWorldSpace*>& out); // includes self
         TESWorldSpace* GetRootWorldspace() {
            auto root = this;
            while (root->parentWorld)
               root = root->parentWorld;
            return root;
         };
   };
   static_assert(sizeof(TESWorldSpace) >= 0x1D8, "RE::TESWorldSpace is too small.");
   static_assert(sizeof(TESWorldSpace) <= 0x1D8, "RE::TESWorldSpace is too large.");
};