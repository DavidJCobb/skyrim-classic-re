#pragma once
#include "ReverseEngineered/Forms/TESForm.h"

namespace RE {
	class BGSBipedObjectForm : public BaseFormComponent {
		public:
			// applicable to DefaultRace
         struct body_part_index {
            body_part_index() = delete;
            enum type {
               head = 0,
               hair,
               body,
               hands,
               forearms,
               amulet,
               ring,
               feet,
               calves,
               shield,
               tail = 0x0A,
               long_hair,
               circlet,
               ears,
               //
					fx01 = 0x1F,
               unk_20 = 0x20, // extended parts used internally e.g. for ammo
               //
               unk_26 = 0x26,
               maybe_ammo = 0x29,
            };
         };
         using body_part_index_t = std::underlying_type_t<body_part_index::type>;
         struct weight_class {
            weight_class() = delete;
            enum {
               light,
               heavy,
               none,
            };
         };

		   UInt32 parts       = 0; // bitmask of (body_part_index) values i.e. ((1 << body_part_index::whatever) | (1 << body_part_index::whatever_else))
			UInt32 weightClass = weight_class::none;

         inline void modify_part(body_part_index_t p, bool removeOrAdd) noexcept {
            if (removeOrAdd)
               this->parts |= 1 << p;
            else
               this->parts &= ~(1 << p);
         }
         inline bool has_part(body_part_index_t p) const noexcept {
            return this->parts & (1 << p);
         }
	};
}
