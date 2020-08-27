#pragma once
#include "skse/Utilities.h"
#include "ReverseEngineered/Forms/TESForm.h"
#include "ReverseEngineered/ExtraData.h"
#include "ReverseEngineered/Types.h"

namespace RE {
	class ExtraDataList;
	class InventoryEntryData;
   class TESObjectREFR;
	class TESObjectSTAT;

	class NewInventoryMenuItemLoadTask; // used to load 3D

	class Inventory3DManager { // sizeof == 0xE4 // also handles magic 3D
		public:
			virtual ~Inventory3DManager();
			//
			static Inventory3DManager* get() {
				return *((Inventory3DManager**)0x01B2E99C);
			}

			//
			// The manager retains information, including nodes, on all recently rendered items (or perhaps just 
			// items it's likely to render), so these will exist for more than just the thing currently being 
			// shown. The last one in the list (i.e. unk40[unk40.count - 1]) is the one currently being displayed.
			//
			struct ItemData { // sizeof == 0x14
				TESForm*       formToDisplay;      // 00 // form to render (SpellItem, TESObjectWEAP, etc.)
				TESObjectSTAT* inventoryModelForm; // 04
				NiNode*  renderedNode; // 08
				uint32_t unk0C; // 0C // number or integer, possibly the same as unk30 on the manager itself
				float    unk10; // 10
				//
				MEMBER_FN_PREFIX(ItemData);
			};

			//	void			** _vtbl;	// 00
			uint32_t unk04;
			uint32_t unk08; // This appears to be 1 when a menu is open
			uint32_t unk0C;
			NiPoint3 unk10;        // 10 // copy of (basePosition)
			NiPoint3 basePosition; // 1C // INI:Interface:fInventory3DItemPosX and friends, or INI:Interface:fInventory3DItemPosXWide and friends
			float    unk28;        // 28 // copy of (baseScale)
			float    baseScale;    // 2C // INI:Interface:fInventory3DItemPosScale or INI:Interface:fInventory3DItemPosScaleWide
			uint32_t unk30;        // 30 // actual int; probably an enum. 1 for inventory, 2 for magic?
			TESObjectREFR* object; // 34 // a temporary TESObjectREFR created by the manager itself. flagged to not be written to the save, but still has a ref handle
			ExtraDataList  unk38;  // 38 // probably only used for inventory items, as opposed to magic
			BSTSmallArray<ItemData, 7> unk40; // 40 // data at 44 // count at D0
			float    unkD4; // D4 // related to menu fade, maybe?
			float    unkD8; // D8 // related to menu fade, maybe?
			NiPointer<NiRefObject> unkDC; // DC // NOT a NiNode. unkDC->unk50 is a TESForm* base form. unkDC could be some non-NetImmerse smart pointer
			uint8_t  unkE0;
			uint8_t  unkE1; // Somekind of mode (0 for MagicMenu)
			bool     unkE2; // E2
			uint8_t  padE3;

			MEMBER_FN_PREFIX(Inventory3DManager);
			DEFINE_MEMBER_FN(UpdateItem3D, void, 0x00867C00, InventoryEntryData*);
			DEFINE_MEMBER_FN(UpdateForm3D, void, 0x00867930, TESForm* form, ExtraDataList* extra); // type on the second arg may be slightly wrong but it's basically the extra data
			DEFINE_MEMBER_FN(Clear3D, void, 0x008668C0);
			DEFINE_MEMBER_FN(Render, UInt32, 0x00867730); // this only seems to update the node's position. when is it actually attached to the scene graph?

			DEFINE_MEMBER_FN(GetInventoryModelForForm, TESObjectSTAT*, 0x00866060, const TESForm*); // but why is it a member function? lol

			DEFINE_MEMBER_FN(Subroutine008667E0, void, 0x008667E0, decltype(unk30)); // creates (object) and resets the object position values. contacts MenuControls, probably to take ownership of the "look" stick
			DEFINE_MEMBER_FN(Subroutine00867110, void, 0x00867110); // abandons (object), probably destroying it. also clears unk30 and contacts MenuControls, probably to do the opposite of whatever 0x008667E0 did

			DEFINE_MEMBER_FN(Subroutine008418D0, bool, 0x008418D0); // return (this->unkD0 == 0.0F) && (this->unkD4 == 1.0F);

			DEFINE_MEMBER_FN(SetRenderPositionAndScale, void, 0x00866000, const NiPoint3& position, float scale);
			DEFINE_MEMBER_FN(GetRenderWorldPosition, void,  0x00866330, NiPoint3& out); // out = (this->unkDC || !this->unk40.count) ? NiPoint3(0,0,0) : this->unk40.last()->renderedNode->worldTransform.pos;
			DEFINE_MEMBER_FN(GetRenderWorldRotation, void,  0x00866380, NiMatrix33& out); // same deal as GetRenderWorldPosition but with the rotation matrixs
			DEFINE_MEMBER_FN(GetRenderScale,         float, 0x008663E0); // return (this->unkDC || !this->unk40.count) ? 1.0F : this->unk40.last()->renderedNode->localTransform.scale;
			DEFINE_MEMBER_FN(Subroutine00866430, void, 0x00866430, TESEffectShader*, ExtraDataList* optional); // plays a shader on the currently displayed object
			DEFINE_MEMBER_FN(Subroutine008672E0, void, 0x008672E0, decltype(ItemData::unk0C), ItemData*, NiPointer<NiNode>&);
			DEFINE_MEMBER_FN(Subroutine008664E0, void, 0x008664E0, decltype(ItemData::unk0C), decltype(ItemData::unk10));
			DEFINE_MEMBER_FN(Subroutine00866580, void, 0x00866580, NiPointer<NiNode>&); // updates the node position based on scale settings, etc.?

			DEFINE_MEMBER_FN(FormTypeIsMagic, bool, 0x00865FE0, form_type_t); // returns true for these types, false otherwise: spell, shout, magic_effect

			/*DEFINE_MEMBER_FN(Unk1, void, 0x008667E0, UInt32 unk1);
			DEFINE_MEMBER_FN(Unk2, void, 0x00867110);
			DEFINE_MEMBER_FN(Unk3, bool, 0x008664C0);
			DEFINE_MEMBER_FN(Unk4, double, 0x008663E0);
			DEFINE_MEMBER_FN(Unk5, bool, 0x008418D0);
			DEFINE_MEMBER_FN(Unk6, int, 0x00867730);*/
	};
}

//
// Scattered observations:
//
//  - There are "Menu" and "Menu3D" scene graphs, but the game doesn't appear to use them at all; I wouldn't 
//    be surprised if they were Oblivion leftovers. Instead, Inventory3DManager's nodes are appended to an 
//    unnamed hardcoded node. All ItemData nodes share that same parent, that same node without a name, and 
//    the ones not in use are just culled.
//
//     - That nameless node? Seems like it's pointed to by Singleton01B2E998::get()->unk1C[2].
//
//        - Inventory3DManager::Subroutine008672E0 calls Singleton01B2E998::AppendNode.
//
//        - Inventory3DManager::Subroutine00866960 calls Singleton01B2E998::RemoveNode.
//