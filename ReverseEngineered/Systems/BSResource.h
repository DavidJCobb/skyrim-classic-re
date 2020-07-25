#pragma once
#include "skse/Utilities.h"
#include "ReverseEngineered/Systems/BSTEvent.h"
#include "ReverseEngineered/Miscellaneous.h"
#include "ReverseEngineered/Shared.h"

namespace RE {
	class NiNode;

	class BSResource {
		public:
			virtual ~BSResource();

			struct ArchiveStreamOpenedEvent {};

			template<typename D> class EntryDBBase {
				public:
					virtual ~EntryDBBase();
					virtual void Unk_01(void);	// Pure
					virtual void Unk_02(void);
					virtual void Unk_03(void);
					virtual void Unk_04(void);
					virtual void Unk_05(void);
			};

			template<typename D> class EntryDB : public EntryDBBase<D> {
				public:
					virtual ~EntryDB();
					virtual void Unk_01(void);
					virtual void Unk_02(void);
					virtual void Unk_03(void);
					virtual void Unk_04(void);
					virtual void Unk_05(void);

					D data;	// 04
			};
	};
	
   struct LoadedMeshResource { // a VERY wild guess
      UInt32 unk00; // atomic flags mask
      UInt32 unk04;
      UInt32 unk08;
      void*  unk0C;
      UInt32 unk10; // integer, possibly flags including a lock bit
      UInt32 unk14;
      UInt32 unk18;
      NiNode* loadedNode; // 1C
   };
   struct LoadedMeshResourcePtr {
		using data_t = LoadedMeshResource;

		data_t* data = nullptr;

      MEMBER_FN_PREFIX(LoadedMeshResourcePtr);
      DEFINE_MEMBER_FN(Destructor, void, 0x00407EC0);

      ~LoadedMeshResourcePtr() { CALL_MEMBER_FN(this, Destructor)(); }

      inline NiNode* getModel() const noexcept {
         if (this->data)
            return this->data->loadedNode;
         return nullptr;
      }

		inline operator data_t* ()  const noexcept { return this->data; }
		inline data_t& operator*()  const noexcept { return *this->data; }
		inline data_t* operator->() const noexcept { return this->data; }
   };

	struct BSModelDB {
		struct DBTraits {
			// TODO
		};
		// TODO
	};
	class Singleton012E2CF8 : public BSResource::EntryDB<BSModelDB::DBTraits> {
		public:
			inline static Singleton012E2CF8& get()     { return **(Singleton012E2CF8**)0x012E2CF8; }
			inline static Singleton012E2CF8* pointer() { return (Singleton012E2CF8*)0x01B92DB0; }
			static void create() { // probably shouldn't call this
				DEFINE_SUBROUTINE(void, f, 0x00AF4FD0);
				f();
			}

			struct Unk04 { // sizeof == 0x64
				struct entry {
					UInt32 lock  = 0; // 00 // lock via InterlockedCompareExchange
					UInt32 unk04 = 0;
					void*  unk08; // initialized to &unk04
				};

				entry  unk00;
				entry  unk0C;
				entry  unk18;
				entry  unk24;
				entry  unk30;
				entry  unk3C;
				entry  unk48;
				entry  unk54;
				UInt32 unk60 = 0;

				MEMBER_FN_PREFIX(Unk04);
				DEFINE_MEMBER_FN(Constructor, Unk04&, 0x00B01450);
				DEFINE_MEMBER_FN(Subroutine00461720, UInt32, 0x00461720, void*);
				DEFINE_MEMBER_FN(Subroutine004629D0, UInt32, 0x004629D0, void*);
			};
			struct Unk68 { // sizeof == 0x5004
				struct UnkA4 {
					UInt32 unk0000[0x1000];
					UInt32 unk4000[0x0400];
					UInt32 unk5000 = 0x400;

					UnkA4() {
						memset(this->unk0000, 0xCD, sizeof(this->unk0000));
						this->unk5000 = 0x400;
						for (int i = 0; i < 0x400; ++i)
							this->unk4000[i] = (UInt32)this + (i * 0x10);
					}

					MEMBER_FN_PREFIX(UnkA4);
					DEFINE_MEMBER_FN(Constructor, UnkA4&, 0x00AF5D80);
				};

				UInt32 unk0000;
				void*  unk0004; // = malloc(0x100);
				UInt32 unk0008 = 0x40;
				UInt32 unk000C = 0;
				UInt32 unk0010 = 0;
				UInt32 unk0014 = 0;
				UInt32 unk0018 = 0;
				UInt32 unk001C = 0;
				Unk04  unk0020;
				Unk04::entry unk0084[2];
				UInt32 unk00A0 = 0;
				UnkA4  unk00A4;
				UInt32 unk50A4;
				UInt32 unk50A8 = 0;
				UInt32 unk50AC = 0;

				MEMBER_FN_PREFIX(Unk68);
				DEFINE_MEMBER_FN(Constructor, Unk68&, 0x00AF5DD0);
				DEFINE_MEMBER_FN(Subroutine00462C70,     bool,   0x00462C70, void*, UInt32& sameVal, bool zero);
				DEFINE_MEMBER_FN(Subroutine00462FE0,     UInt32, 0x00462FE0, UInt32 zero, UInt32 sameVal, LoadedMeshResourcePtr&, LoadModelOptions&);
				DEFINE_MEMBER_FN(DestroyLoadedMeshResourcePtr, void,   0x006483F0, LoadedMeshResource&); // DestroyLoadedMeshResourcePtr::~DestroyLoadedMeshResourcePtr just passes its LoadedMeshResource* to this
				DEFINE_MEMBER_FN(Subroutine00AF6350,     UInt32, 0x00AF6350, UInt32, LoadedMeshResourcePtr&, bool, LoadModelOptions&);
			};

			Unk04 unk04;
			
			MEMBER_FN_PREFIX(Singleton012E2CF8);
			DEFINE_MEMBER_FN(Subroutine00408020, UInt32, 0x00408020, LoadedMeshResourcePtr&, UInt32 zero);
	};

	DEFINE_SUBROUTINE_EXTERN(bool, GetAlreadyLoadedMeshResource, 0x00AF5030, const char* pathOrName, LoadedMeshResourcePtr&);
}