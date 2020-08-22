#pragma once

#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/Forms/BGSLightingTemplate.h"

class BGSAcousticSpace;
class BGSMusicType;
class TESImageSpace;
class TESObjectCELL;
namespace RE {
   class  bhkWorld;
   class  BSMultiBoundNode;
   class  BSPortalGraph; // constructor: 0x00B03E00
   struct MapMarkerOperation; // see TESWorldSpace.h
   class  NiNode;
   class  TESObjectREFR; // Forward-declare instead of #including, so the compiler doesn't choke on a circular dependency
   class  TESWaterForm;
   class  TESWorldSpace;
   //
   class TESObjectCELL : public TESForm {
      public:
         operator ::TESObjectCELL*() const { return (::TESObjectCELL*) this; }
         enum { kTypeID = kFormType_Cell }; // needed for SKSE Papyrus compatibility
         static constexpr form_type_t form_type = form_type::cell;
         //
         struct cell_node_type {
            //
            // Cells have a root node, and a small number of child nodes, each of which is responsible 
            // for handling different types of objects. TESObjectCELL is careful to check that a given 
            // child node exists before using it, but does NOT run correctness checks on their contents.
            //
            // These indices also line up with the enum used for the ToggleCellNode console command, 
            // which no doubt is deliberate.
            //
            cell_node_type() = delete;
            enum type : uint32_t {
               actor,
               marker,
               land,
               water,
               statik,
               active,
               unk_06,
               rooms, // children are BSMultiBoundRoom*
               unk_08, // collision markers?
            };
         };
         //
         struct UnknownData {
            UInt32 unk00;
            UInt32 unk04;
         };

         enum {
            kLightingTemplateUsageFlag_Ambient = 1, // also directional ambient colors
            kLightingTemplateUsageFlag_Directional = 2,
            kLightingTemplateUsageFlag_FogColor = 4,
            kLightingTemplateUsageFlag_FogNearDistance = 8,
            kLightingTemplateUsageFlag_FogFarDistance = 16,
            kLightingTemplateUsageFlag_DirectionalRotation = 32,
            kLightingTemplateUsageFlag_DirectionalFade = 64,
            kLightingTemplateUsageFlag_FogClipDistance = 128,
            kLightingTemplateUsageFlag_FogPower = 256,
            kLightingTemplateUsageFlag_FogMax = 512,
            kLightingTemplateUsageFlag_LightFadeDistances = 1024,
         };

         struct MaxHeightData { // MHDT subrecord
            float offset; // 00
            UInt8 data[32][32]; // 04 // a grid, though i don't know whether it's [x][y] or [y][x]
         };

         struct CellData {};
         struct ExteriorData : CellData { // sizeof == 0x1C; see 004C18E4
            struct Struct0063A0B0 { // sizeof == 0x4 // TVDT subrecord
               struct Struct0062DB00{ // sizeof == 0xC
                  void*  unk00;
                  void*  unk04; // data read verbatim from the ESP file
                  UInt32 count; // == ((TVDT subrecord size * 8 + 31) / 32); if > 1, then unk04 == unk00 == malloc(count * 4) i.e. they're the same pointer; else unk04 is a single data field
               };
               Struct0062DB00* unk00;

               MEMBER_FN_PREFIX(Struct0063A0B0);
               DEFINE_MEMBER_FN(Load, void, 0x0062DB50, BGSLoadFormBuffer*);
            };
            //
            SInt32 cellX; // 00 // TESObjectCELL has an exterior-specific getter: 0x004C0450
            SInt32 cellY; // 04 // TESObjectCELL has an exterior-specific getter: 0x004C0470
            MaxHeightData*  maxHeightData; // 08 // raw contents of MHDT subrecord, read verbatim
            Struct0063A0B0* occlusionData; // 0C
            float  unitX; // 10 // TESObjectCELL has an exterior-specific getter: 0x004C04D0
            float  unitY; // 14 // TESObjectCELL has an exterior-specific getter: 0x004C04F0
            UInt8  forceHideLandQuads; // 18 // one bit per quad
            UInt8  pad19[3];
         };
         struct InteriorData : CellData { // sizeof == 0x60; constructor at 0x004AD710; this is XCLL data literally copied verbatim from the ESP/ESM
            InteriorData() {};
            union Color {
               struct {
                  UInt8 alpha; // typically unused, so typically zero
                  UInt8 blue;
                  UInt8 green;
                  UInt8 red;
               } color;
               UInt32 abgr;    // 1C
               //
               Color() : abgr(0) {};
               Color(int a) : abgr(a) {};
            };
            //
            Color  ambientColor     = 0;
            Color  directionalColor = 0;
            Color  fogColorNear; // 08 // 0x00BBGGRR
            float  fogPlaneNear; // 0C
            float  fogPlaneFar;  // 10
            SInt32 directionalRotXY; // 14 // directional rotation XY
            SInt32 directionalRotZ; // 18 // directional rotation Z
            float  directionalFade  = 1.0F; // 1C // directional fade
            float  fogClipDistance; // 20
            float  fogPow = 1.0F; // 24 // getter: TESObjectCELL::Subroutine004C0430
            union {
               struct {
                  Color xPos;
                  Color xNeg;
                  Color yPos;
                  Color yNeg;
                  Color zPos;
                  Color zNeg;
               } directionalAmbient;
               Color directionalAmbientColors[6];
            }; // 28
            Color  specular; // 40 // unused
            float  fresnelPower = 1.0F; // 44 // unused; purpose confirmed from CK wiki docs
            Color  fogColorFar; // 48
            float  fogMax = 1.0;  // 4C
            float  lightFadeBegin; // 50 // light fade begin
            float  lightFadeEnd; // 54 // light fade end
            UInt32 inheritFromTemplate = 0x79F; // 58 // bitmask
            UInt32 unk5C = 0; // 5C
         };

         enum CellFlags : UInt16 {
            //
            // These seem to match the flags in the ESP/ESM file format.
            //
            kCellFlag_IsInterior     = 0x0001,
            kCellFlag_Unknown0002    = 0x0002, // indicates relationship between land and the water level
            kCellFlag_NoLODWater     = 0x0008, // assumed
            kCellFlag_Unknown0010    = 0x0010, // possibly "can't travel from here," but that's not what it is in the ESM/ESP file format
            kCellFlag_IsPublic       = 0x0020,
            kCellFlag_HandChanged    = 0x0040, // assumed
            kCellFlag_ShowSky        = 0x0080, // assumed
            kCellFlag_UseSkyLighting = 0x0100, // meaning inferred from documentation for Papyrus fog setters
         };

         struct Struct004C70B0 { // sizeof == 0xC4
            NiPointer<BSPortalGraph> unk00; // 00 // created in TESObjectCELL member function 0x004C20A0
            BSMultiBoundNode* multiboundNode; // 04
            NiNode*  unk08; // 08
            NiNode*  unk0C; // 0C
            uint32_t unk10 = 0;
            tList<uint32_t> unk14; // entries removed by TESObjectCELL::Subroutine004D3250. either is a list of ref_handle, or a list of structs that contain handles
            tArray<ref_handle> unk1C; // lots of entries
            tArray<ref_handle> unk28; // possibly room and portal markers?
            NiTMap<TESForm*, ref_handle> unk34; // 34 // type confirmed via RTTI // related to ExtraEmittanceSource on refs? see TESObjectCELL::Subroutine004D2110
            NiTMap<ref_handle, NiNode*>  unk44; // 44 // type confirmed via RTTI; this specifically uses a bare node pointer
            NiTMap<ref_handle, NiPointer<BSMultiBoundNode>> unk54; // 54 // type confirmed via RTTI; this specifically uses a smart node pointer
            NiTMap<BSMultiBoundNode*, ref_handle> unk64; // 64 // type confirmed via RTTI; this specifically uses a bare node pointer
            tList<uint64_t> unk74; // 74 // sizeof(item) == 8
            tList<ref_handle> unk7C; // 7C // see TESObjectCELL::Subroutine004D1810
            tArray<uint32_t> unk84;
            tArray<uint32_t> unk90; // very likely an array of ref_handles. related to decals.
            tArray<ref_handle> unk9C; // 9C
            BGSEncounterZone* unkA8 = nullptr; // A8
            void*    unkAC;
            uint32_t unkB0 = 0;
            uint32_t unkB4 = 0; // atomic?
            uint32_t unkB8 = 0;
            uint32_t unkBC = 0xFFFFFFFF;
            uint8_t  unkC0 = 0;
            uint8_t  padC1[3];
            //
            MEMBER_FN_PREFIX(Struct004C70B0);
            DEFINE_MEMBER_FN(Constructor, Struct004C70B0&, 0x004C70B0);
            DEFINE_MEMBER_FN(Destructor, void, 0x004C73C0);
         };
         
         // Parents:
         TESFullName    fullName;      // 14
         // Members:
         UnknownData    unk1C;			// 1C
         UnknownData    unk24;			// 24
         CellFlags      unk2C;			// 2C // cell flags.
         UInt16         unk2E;			// 2E
         UInt8          unk30;			// 30 // loaded state, or whatever you want to call it. 0x07 means the cell is attached.
         UInt8          unk31;			// 31
         UInt8	         unk32;			// 32
         UInt8          pad33;			// 33
         BaseExtraList  extraData; // 34
         CellData*      unk3C;		    // 3C // NOTE: This data's values have different meanings for interior and exterior cells.
         TESObjectLAND* unk40;		    // 40
         float          waterLevel;     // 44 // XCLW subrecord
         void*          unk48;		    // 48 // related to navmesh data!
         tArray<RE::TESObjectREFR*> objectList; // 4C
         UnkArray       unk58;			// 58
         UnkArray       unk64;			// 64
         UnkArray       unk70;			// 70
         SimpleLock     cellRefLock;    // 7C
         TESWorldSpace* parentWorld;	// 84
         Struct004C70B0* unk88;			// 88 // memory inspection shows it holds portal and multibound pointers; investigate subroutine 0x004C20A0 to learn more about it
         BGSLightingTemplate* lightingTemplate;	// 8C
         //
         bool IsLoaded();
         //
         inline TESImageSpace* GetImageSpace() {
            auto data = (RE::ExtraCellImageSpace*) CALL_MEMBER_FN(&this->extraData, GetByType)(kExtraData_CellImageSpace);
            if (data)
               return data->data;
            return nullptr;
         };
         inline BGSMusicType* GetMusicType() {
            auto data = (RE::ExtraCellMusicType*) CALL_MEMBER_FN(&this->extraData, GetByType)(kExtraData_CellMusicType);
            if (data)
               return data->data;
            return nullptr;
         };
         NiNode* GetMultiBoundRoomContainer() noexcept; // every child of this node is guaranteed to be a BSMultiBoundRoom*
         //
         MEMBER_FN_PREFIX(TESObjectCELL);
         //
         DEFINE_MEMBER_FN(CellRefLockEnter,  void, 0x004C0180); // Call this before modifying the cell's object list.
         DEFINE_MEMBER_FN(CellRefLockExit,   void, 0x004C0190); // Call this after  modifying the cell's object list.
         DEFINE_MEMBER_FN(AddRefToList,      void, 0x004D3EA0, RE::TESObjectREFR* reference, UInt32 unknown); // Second argument is zero. You need to call CellRefLockEnter before, and CellRefLockExit after.
         DEFINE_MEMBER_FN(RemoveRefFromList, void, 0x004CB7B0, RE::TESObjectREFR* reference); // Calls CellRefLockEnter and CellRefLockExit for you.
         //
         DEFINE_MEMBER_FN(CreateCellNodes, void, 0x004CFBE0); // used internally; listed here for documentation; do not call
         DEFINE_MEMBER_FN(GetUsablePortalGraph, BSPortalGraph*, 0x004C2020); // gets the cell's BSPortalGraph if the cell is loaded, or one from the parent world if any
         DEFINE_MEMBER_FN(GetRootNode,   BSMultiBoundNode*, 0x004C2230); // gets the cell's root 3D node; each child has a special purpose; you probably shouldn't append to it directly
         DEFINE_MEMBER_FN(GetCellNode,   NiNode*, 0x00432BD0, cell_node_type::type which); // GetNode()->children[which]
         DEFINE_MEMBER_FN(GetActorNode,  NiNode*, 0x004C3FC0); // GetNode()->children[cell_node_type::actor];
         DEFINE_MEMBER_FN(GetMarkerNode, NiNode*, 0x00432C00); // GetNode()->children[cell_node_type::marker]; apparently used during the weapon-firing process
         DEFINE_MEMBER_FN(GetWaterNode,  NiNode*, 0x004C3FF0); // GetNode()->children[cell_node_type::water];
         DEFINE_MEMBER_FN(GetStaticNode, NiNode*, 0x004C4020); // GetNode()->children[cell_node_type::statik];
         //
         DEFINE_MEMBER_FN(GetHavokWorld, bhkWorld*, 0x004C5430);
         //
         DEFINE_MEMBER_FN(GetAcousticSpace,    BGSAcousticSpace*, 0x004C0760);
         DEFINE_MEMBER_FN(GetEncounterZone,    BGSEncounterZone*, 0x004C2240); // checks unk88::unkA8, extra data, and parent worldspace
         DEFINE_MEMBER_FN(GetOwner,            void*, 0x004C4DC0); // Returns TESNPC* or TESFaction*.
         DEFINE_MEMBER_FN(GetMapMarkerHandles, void,  0x004C8230, MapMarkerOperation* out);
         DEFINE_MEMBER_FN(GetRegionList,       TESRegionList*, 0x004C23E0, bool createIfMissing);
         DEFINE_MEMBER_FN(Reset,               void, 0x004C0650, UInt32 detachTime, bool setDetachTime);
         DEFINE_MEMBER_FN(SetImageSpace,       void, 0x004C07F0, UInt32 formID);
         //
         DEFINE_MEMBER_FN(GetExteriorData, ExteriorData*, 0x004C0390); // Returns unk3C for exterior cells only.
         DEFINE_MEMBER_FN(GetInteriorData, InteriorData*, 0x004C03A0); // Returns unk3C for interior cells only.
         //
         DEFINE_MEMBER_FN(SetIsInterior,   void,  0x004C0240, bool flag); // Sets or modifies flag 0x0001 in unk2C. WARNING: interior and exterior cells store data differently, and this function DOES NOT CONVERT EXISTING DATA. It just changes the flag.
         DEFINE_MEMBER_FN(SetPublic,       void,  0x004C0260, bool flag); // Sets or modifies flag 0x0020 in unk2C.
         //
         DEFINE_MEMBER_FN(GetDirectionalAmbientColor, InteriorData::Color, 0x004C1930, UInt32 whichColor);
         DEFINE_MEMBER_FN(GetDirectionalLightAngleXY, SInt32, 0x004C1D70); // honors lighting templates
         DEFINE_MEMBER_FN(GetDirectionalLightAngleZ,  SInt32, 0x004C1DA0); // honors lighting templates
         DEFINE_MEMBER_FN(GetLightFadeBeginDistance,  float,  0x004C1DD0); // honors lighting templates
         DEFINE_MEMBER_FN(GetLightFadeEndDistance,    float,  0x004C1E00); // honors lighting templates
         DEFINE_MEMBER_FN(SetFogColorNear, void,  0x004C03B0, UInt32 color); // Sets the fog color (as 0x00BBGGRR) for interior cells. (Exterior cells don't use that value.)
         DEFINE_MEMBER_FN(SetFogColorFar,  void,  0x004C03D0, UInt32 color); // Sets the fog color (as 0x00BBGGRR) for interior cells. (Exterior cells don't use that value.)
         DEFINE_MEMBER_FN(SetFogPlaneNear, void,  0x004C03F0, float); // Sets the near fog plane for interior cells. (Exterior cells don't use that value.)
         DEFINE_MEMBER_FN(SetFogPlaneFar,  void,  0x004C0410, float); // Sets the far  fog plane for interior cells. (Exterior cells don't use that value.)
         //
         DEFINE_MEMBER_FN(GetCellCoordinates, void,  0x004C0490, SInt32*, SInt32*); // Gets the cell-coordinates of an exterior cell. (Interior cells don't use these values.)
         DEFINE_MEMBER_FN(SetCellCoordinates, void,  0x004C0550, SInt32,  SInt32);  // Sets the cell-coordinates of an exterior cell. (Interior cells don't use these values.)
         DEFINE_MEMBER_FN(GetLandHeightAt,    bool,  0x004C2EE0, NiPoint3& position, float& out); // calls TESObjectLAND::GetHeightAt
         DEFINE_MEMBER_FN(GetUnitCoordinates, void,  0x004C0510, float*,  float*);  // Gets the unit-coordinates of an exterior cell. (Interior cells don't use these values.)
         DEFINE_MEMBER_FN(GetNorthRotation,   float, 0x004C0FC0);
         DEFINE_MEMBER_FN(GetWaterLevel,      float, 0x004C0710); // Returns water level, checking the parent world if needed; returns FLOAT_MAX if no water level. (Interior cells don't have a water level.)
         DEFINE_MEMBER_FN(GetWaterType,       TESWaterForm*, 0x004C0800);
         //
         DEFINE_MEMBER_FN(HasExtraGIDBuffer, bool, 0x004C1050);
         //
         DEFINE_MEMBER_FN(Subroutine004C05A0, UInt32, 0x004C05A0); // Interiors: returns (this->unk88 + 0x54). Exteriors: returns (this->parentWorld->unkBC).
         DEFINE_MEMBER_FN(Subroutine004C05C0, UInt32, 0x004C05C0); // Interiors: returns (this->unk88 + 0x64). Exteriors: returns (this->parentWorld->unkC0).
         DEFINE_MEMBER_FN(Subroutine004C0650, void,   0x004C0650, UInt32, UInt32); // Func(-1, 0) queues the cell to reset.
         DEFINE_MEMBER_FN(ResetCellData,      void,   0x004C1820);
         DEFINE_MEMBER_FN(Subroutine004C2470, void,   0x004C2470); // loops over all objects in cell; performs unknown operation on 3D; unloads 3D
         DEFINE_MEMBER_FN(Subroutine004C25D0, void,   0x004C25D0); // loops over all objects in cell; performs unknown operation on any that aren't: disabled; deleted; or doors that are marked as destroyed
         DEFINE_MEMBER_FN(Subroutine004C2660, void,   0x004C2660, bool); // loops over all objects in cell; performs one of two unknown operations on them based on bool
         DEFINE_MEMBER_FN(Subroutine004C26F0, void,   0x004C26F0, float, float, float, float, UInt32); // loops over all objects in cell; performs unknown operation on them involving mathematical checks on their positions
         DEFINE_MEMBER_FN(Subroutine004C28F0, void,   0x004C28F0); // loops over all objects in cell; performs unknown operation them
         DEFINE_MEMBER_FN(Subroutine004C29A0, void,   0x004C29A0); // loops over all objects in cell; performs unknown operation them
         DEFINE_MEMBER_FN(Subroutine004C2B00, char*,  0x004C2B00); // returns the cell's name(?), or "Wilderness" if it's a nameless exterior cell
         DEFINE_MEMBER_FN(Subroutine004C31B0, void,   0x004C31B0, UInt32, UInt32, UInt32); // something to do with whether the cell's been seen; nearby subroutines (scroll up) also are related
         DEFINE_MEMBER_FN(Subroutine004C32C0, void,   0x004C32C0); // loops over all objects in cell; performs unknown operation them
         DEFINE_MEMBER_FN(Subroutine004D1810, void,   0x004D1810, ref_handle);
         DEFINE_MEMBER_FN(Subroutine004D2110, void,   0x004D2110, ref_handle);
         DEFINE_MEMBER_FN(Subroutine004D2A80, BSMultiBoundRoom*, 0x004D2A80, ref_handle); // calls Subroutine004D2760
         DEFINE_MEMBER_FN(Subroutine004D2760, BSMultiBoundRoom*, 0x004D2760, ref_handle, bool); // GetOrCreateRoomNode?
         DEFINE_MEMBER_FN(Subroutine004D2C80, void,   0x004D2C80, ref_handle); // related to decals
         DEFINE_MEMBER_FN(Subroutine004D3250, void,   0x004D3250, ref_handle);
         //
         DEFINE_MEMBER_FN(ModifyPersistentFlag, void, 0x004C0CC0, bool); // persistent flag?
   };
   static_assert(offsetof(TESObjectCELL, parentWorld) >= 0x84, "RE::TESObjectCELL::parentWorld is too early.");
   static_assert(offsetof(TESObjectCELL, parentWorld) <= 0x84, "RE::TESObjectCELL::parentWorld is too late.");
};