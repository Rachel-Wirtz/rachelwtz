#ifndef RACHELWTZ__STRINGS__STRING__HPP
#define RACHELWTZ__STRINGS__STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

//
// Assuming that char holds either ANSI or UTF8.
//
template<>
struct char_traits<char> : public std::char_traits<char> {
    using char_type  = std::char_traits<char>::char_type;
    using int_type   = std::char_traits<char>::int_type;
    using off_type   = std::char_traits<char>::off_type;
    using pos_type   = std::char_traits<char>::pos_type;
    using state_type = std::char_traits<char>::state_type;

    [[nodiscard]]
    static constexpr uint8_t code_point_length(const char& c) noexcept {
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
    static constexpr size_t code_point_count(const char* str, size_t count) noexcept {
        const char* ptr = str;
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
    static constexpr bool validate_code_points(const char* str, size_t count) noexcept {
        const char* ptr = str;
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

    static constexpr const char byte_order_mark[3] = {
        static_cast<char>(0xEF),
        static_cast<char>(0xBB),
        static_cast<char>(0xBF)
    };
};

using string      = basic_string<char>;
using string_view = basic_string_view<char>;

RACHELWTZ_STRINGS_END
#endif