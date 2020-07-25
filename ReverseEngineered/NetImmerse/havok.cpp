#include "havok.h"
#include "skse/GameAPI.h"

namespace RE {
   __declspec(naked) bhkWorldM* GetDefaultHavokWorld() {
      _asm {
         mov  eax, 0x012E35C8; // this is either a bhkWorldM*** or a bhkWorldM**, and I can't be bothered to figure out which
         mov  eax, dword ptr [eax];
         mov  eax, dword ptr [eax];
         retn;
      }
   }

   /*static*/ bhkBoxShape* bhkBoxShape::make(const hkVector4& vec) {
      auto memory = (bhkBoxShape*) FormHeap_Allocate(sizeof(bhkBoxShape));
      CALL_MEMBER_FN(memory, Constructor)(vec);
      return memory;
   }
   /*static*/ bhkRigidBody* bhkRigidBody::make(Struct00D378B0& params) {
      auto memory = (bhkRigidBody*) FormHeap_Allocate(sizeof(bhkRigidBody));
      CALL_MEMBER_FN(memory, Constructor)(params);
      return memory;
   }
   /*static*/ bhkNiCollisionObject* bhkNiCollisionObject::make(NiNode* node) {
      auto memory = (bhkNiCollisionObject*) FormHeap_Allocate(sizeof(bhkNiCollisionObject));
      CALL_MEMBER_FN(memory, Constructor)(node);
      return memory;
   }
   /*static*/ bhkCollisionObject* bhkCollisionObject::make(NiNode* node) {
      auto memory = bhkNiCollisionObject::make(node);
      //
      // I know this looks stupid, but this is actually how it's done: bhkCollisionObjects 
      // aren't created in especially many places and everywhere they're made, their 
      // constructors are effectively inlined as calls to the superclass constructor 
      // followed by a manual write to the vtbl pointer.
      //
      *(uint32_t*)memory = vtbl;
      //
      return (bhkCollisionObject*) memory;
   }
}