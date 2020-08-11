#pragma once
#include <cstdint>
#include "skse/GameStreams.h"

namespace RE {
   class FastBSNiBinaryReader {
      //
      // Benchmarking shows that just using a BSResourceNiBinaryStream, as SKSE does, takes 
      // about 2400 microseconds to read my translation file. Using this class, which buffers 
      // file content 512 bytes at a time, takes about 1200 microseconds.
      //
      public:
         static constexpr int buffer_size = 512;
      protected:
         void _load_next_chunk() noexcept;
         //
         BSResourceNiBinaryStream& stream;
         uint8_t  buffer[buffer_size];
         uint16_t offset   = 0; // how far into (buffer) we are
         uint16_t contains = 0; // if the data doesn't end on a (buffer_size)-byte boundary, and if we've read up to EOF, then this is how far into (buffer) the EOF is
         bool     did_initial_read = false;
         //
      public:
         FastBSNiBinaryReader(BSResourceNiBinaryStream& s) : stream(s) {
            memset(this->buffer, 0, buffer_size);
         }
         //
         uint32_t read(uint8_t* out, uint32_t maxcount) noexcept;
         bool     read_widechars_until(wchar_t* out, uint32_t maxcount, wchar_t terminator);
         //
         template<typename T> uint32_t read(T& out) noexcept { return this->read((uint8_t*)&out, sizeof(T)); }
   };
}
