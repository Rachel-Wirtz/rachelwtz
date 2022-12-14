#ifndef RACHELWTZ__STRINGS__U16STRING__HPP
#define RACHELWTZ__STRINGS__U16STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

template<>
struct basic_character_encoding<char16_t> {
    using char_type       = char16_t;
    using integer_type    = uint16_t;
    using length_type     = uint8_t;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = char_type&;
    using const_reference = const char_type&;
    using pointer         = char_type*;
    using const_pointer   = const char_type*;

    [[nodiscard]]
    static constexpr length_type length(const_reference elem) {
        integer_type n = (elem >> 8) & 0xFC;
        if (n == 0xD8)
            return 2;
        if (n == 0xDC)
            throw invalid_character_encoding();
        return 1;
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
        while (ptr && *ptr != u'\0')
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
struct char_traits<char16_t> : public std::char_traits<char16_t> {
    using char_type  = std::char_traits<char16_t>::char_type;
    using int_type   = std::char_traits<char16_t>::int_type;
    using off_type   = std::char_traits<char16_t>::off_type;
    using pos_type   = std::char_traits<char16_t>::pos_type;
    using state_type = std::char_traits<char16_t>::state_type;

    [[nodiscard]]
    static constexpr uint8_t code_point_length(const char16_t& c) noexcept {
        uint16_t n = (c >> 8) & 0xFC;
        if (n == 0xD8)
            return 2;
        if (n == 0xDC)
            return 0;
        return 1;
    }

    [[nodiscard]]
    static constexpr size_t code_point_count(const char16_t* str, size_t count) noexcept {
        const char16_t* ptr = str;
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
    static constexpr bool validate_code_points(const char16_t* str, size_t count) noexcept {
        const char16_t* ptr = str;
        while (ptr && ptr < (str + count)) {
            switch (code_point_length(*ptr)) {
            case 1: ++ptr; break;
            case 2:
                if ((ptr + 1) >= (str + count) || ((ptr[1] >> 8) & 0xFC) != 0xDC)
                    return false;
                ptr += 2;
                break;

            default:
                return false;
            }
        }
        return true;
    }

    static constexpr char16_t byte_order_mark[1] = { 0xFEFF };
};
*/

using u16string      = basic_string<char16_t>;
using u16string_view = basic_string_view<char16_t>;

RACHELWTZ_STRINGS_END
#endif