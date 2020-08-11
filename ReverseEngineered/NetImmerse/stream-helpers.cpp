#include "stream-helpers.h"

namespace RE {
   void FastBSNiBinaryReader::_load_next_chunk() noexcept {
      this->contains = CALL_MEMBER_FN(&this->stream, Read)(&this->buffer, buffer_size);
      this->offset   = 0;
   }
   //
   uint32_t FastBSNiBinaryReader::read(uint8_t* out, uint32_t maxcount) noexcept {
      if (!this->did_initial_read) {
         this->did_initial_read = true;
         this->_load_next_chunk();
      } else {
         if (this->offset == buffer_size)
            this->_load_next_chunk();
      }
      if (this->offset >= this->contains)
         return 0;
      //
      uint32_t remainder = this->contains - this->offset;
      if (remainder >= maxcount) {
         memcpy(out, this->buffer + this->offset, maxcount);
         this->offset += maxcount;
         return maxcount;
      }
      memcpy(out, this->buffer + this->offset, remainder);
      out += remainder;
      //
      uint32_t bytes_read = remainder;
      do {
         this->_load_next_chunk();
         if (!this->contains)
            break;
         remainder = this->contains;
         if (remainder > maxcount)
            remainder = maxcount;
         memcpy(out, this->buffer, remainder);
         out        += remainder;
         bytes_read += remainder;
      } while (bytes_read < maxcount);
      return bytes_read;
   }
   bool FastBSNiBinaryReader::read_widechars_until(wchar_t* out, uint32_t maxcount, wchar_t terminator) {
      wchar_t c;
      do {
         if (!this->read(c))
            return false;
         if (c == terminator) {
            *out = '\0';
            return true;
         }
         *out = c;
         ++out;
      } while (c);
      return false;
   }
}