#ifndef RACHELWTZ__STRINGS__U8STRING__HPP
#define RACHELWTZ__STRINGS__U8STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

template<>
struct basic_character_encoding<char8_t> {
    using char_type       = char8_t;
    using integer_type    = uint8_t;
    using length_type     = uint8_t;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = char_type&;
    using const_reference = const char_type&;
    using pointer         = char_type*;
    using const_pointer   = const char_type*;

    [[nodiscard]]
    static constexpr length_type length(const_reference elem) {
        integer_type n = elem >> 4;
        if ((n & 0x0F) == 0x0F)
            return 4;
        if ((n & 0x0E) == 0x0E)
            return 3;
        if ((n & 0x0C) == 0x0C)
            return 2;
        if ((n & 0x08) == 0x00)
            return 1;
        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr size_type count(const_pointer str, size_type len) {
        const_pointer ptr = str;
        size_type final_count = 0;
        while (ptr && ptr < (str + len)) {
            ptr += length(*ptr);
            ++final_count;
        }
        return final_count;
    }

    [[nodiscard]]
    static constexpr size_type length(const_pointer str) {
        const_pointer ptr = str;
        while (ptr && *ptr != u8'\0')
            ++ptr;
        return ptr - str;
    }

    [[nodiscard]]
    static constexpr difference_type difference(const_pointer a, const_pointer b) {
        if (a < b)
            return -static_cast<difference_type>(count(a, b - a));
        if (a > b)
            return static_cast<difference_type>(count(b, a - b));
        return 0;
    }
};

/*
template<>
struct char_traits<char8_t> : public std::char_traits<char8_t> {
    using char_type  = std::char_traits<char8_t>::char_type;
    using int_type   = std::char_traits<char8_t>::int_type;
    using off_type   = std::char_traits<char8_t>::off_type;
    using pos_type   = std::char_traits<char8_t>::pos_type;
    using state_type = std::char_traits<char8_t>::state_type;

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

    static constexpr const char8_t byte_order_mark[3] = {
        0xEF, 0xBB, 0xBF
    };
};
*/

using u8string      = basic_string<char8_t>;
using u8string_view = basic_string_view<char8_t>;

RACHELWTZ_STRINGS_END
#endif