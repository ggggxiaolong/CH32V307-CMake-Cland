#pragma once
#include <cstdint>
#include <initializer_list>

#include "core/platform.hpp"

namespace ymd::hal {

class Crc {
   protected:
    Crc() {};

   public:
    static Crc& get_instance() {
        static auto instance = Crc();
        return instance;
    }

    void init();
    void clear();

    uint32_t update(const uint32_t* begin, const uint32_t* end);

    uint32_t update(const std::initializer_list<uint32_t>& list) { return update(list.begin(), list.end()); }
};

extern Crc& crc;
}  // namespace ymd::hal