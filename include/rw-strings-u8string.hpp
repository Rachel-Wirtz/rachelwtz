#ifndef RACHELWTZ__STRINGS__U8STRING__HPP
#define RACHELWTZ__STRINGS__U8STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

template<>
struct char_traits<char8_t> : public std::char_traits<char8_t> {
    [[nodiscard]]
    static constexpr uint8_t code_point_length(const char8_t& c) noexcept {
        uint8_t n = c >> 4;
        if ((n & 0x0F) == 0x0F)
            return 4;
        if ((n & 0x0E) == 0x0E)
            return 3;
        if ((n & 0x0C) == 0x0C)
            return 2;
        if ((n & 0x08) == 0x00)
            return 1;
        return 0;
    }

    [[nodiscard]]
    static constexpr size_t code_point_count(const char8_t* str, size_t count) noexcept {
        const char8_t* ptr = str;
        size_t len = 0;
        while (ptr && ptr < (str + count)) {
            uint8_t l = code_point_length(*ptr);
            if (l == 0)
                return 0;
            ptr += l;
            ++len;
        }
        return len;
    }

    [[nodiscard]]
    static constexpr bool validate_code_points(const char8_t* str, size_t count) noexcept {
        const char8_t* ptr = str;
        while (ptr && ptr < (str + count)) {
            uint8_t l = code_point_length(*ptr);
            if (l == 0)
                return false;
            if (l > 1) {
                uint8_t n = 1;
                while (n < l) {
                    if (!((ptr + n) < (str + count)) || ((ptr[n] >> 6) & 0x03) != 0x02)
                        return false;
                     ++n;
                }
            }
            ptr += l;
        }
        return true;
    }
};

using u8string      = basic_string<char8_t>;
using u8string_view = basic_string_view<char8_t>;

RACHELWTZ_STRINGS_END
#endif