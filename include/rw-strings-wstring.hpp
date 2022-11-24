#ifndef RACHELWTZ__STRINGS__WSTRING__HPP
#define RACHELWTZ__STRINGS__WSTRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN
template<>
struct basic_character_encoding<wchar_t> {
    using char_type       = wchar_t;
    using integer_type    = std::conditional_t<sizeof(wchar_t) == sizeof(char16_t), uint16_t, std::conditional_t<sizeof(wchar_t) == sizeof(char32_t), uint32_t, void>>;
    using length_type     = uint8_t;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = char_type&;
    using const_reference = const char_type&;
    using pointer         = char_type*;
    using const_pointer   = const char_type*;

    [[nodiscard]]
    static constexpr length_type length(const_reference elem) {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            integer_type n = (elem >> 8) & 0xFC;
            if (n == 0xD8)
                return 2;
            if (n == 0xDC)
                throw invalid_character_encoding();
            return 1;
        }

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            if (elem >= 0x000000 && elem <= 0x10FFFF)
                return 1;
            throw invalid_character_encoding();
        }

        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr size_type count(const_pointer str, size_type len) {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            const_pointer ptr = str;
            size_type final_count = 0;
            while (ptr && ptr < (str + len)) {
                ptr += length(*ptr);
                ++final_count;
            }
            return final_count;
        }

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            return len;
        }

        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr size_type length(const_pointer str) {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t) || sizeof(wchar_t) == sizeof(char32_t)) {
            const_pointer ptr = str;
            while (ptr && *ptr != L'\0')
                ++ptr;
            return ptr - str;
        }

        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr difference_type difference(const_pointer a, const_pointer b) {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            if (a < b)
                return -static_cast<difference_type>(count(a, b - a));
            if (a > b)
                return static_cast<difference_type>(count(b, a - b));
            return 0;
        }

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            return a - b;
        }

        throw invalid_character_encoding();
    }
};

/*
//
// Assuming that wchar_t holds either UTF16 or UTF32.
//
template<>
struct char_traits<wchar_t> : public std::char_traits<wchar_t> {
    using char_type  = std::char_traits<wchar_t>::char_type;
    using int_type   = std::char_traits<wchar_t>::int_type;
    using off_type   = std::char_traits<wchar_t>::off_type;
    using pos_type   = std::char_traits<wchar_t>::pos_type;
    using state_type = std::char_traits<wchar_t>::state_type;

    [[nodiscard]]
    static constexpr uint8_t code_point_length(const wchar_t& c) noexcept {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            uint16_t n = (c >> 8) & 0xFC;
            if (n == 0xD8)
                return 2;
            if (n == 0xDC)
                return 0;
            return 1;
        }

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            if (c >= 0x000000 && c <= 0x10FFFF)
                return 1;
            return 0;
        }

        return 0;
    }

    [[nodiscard]]
    static constexpr size_t code_point_count(const wchar_t* str, size_t count) noexcept {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            const wchar_t* ptr = str;
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

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            const wchar_t* ptr = str;
            while (ptr && ptr < (str + count)) {
                if (!code_point_length(*ptr))
                    return 0;
                ++ptr;
            }
            return count;
        }

        return 0;
    }

    [[nodiscard]]
    static constexpr bool validate_code_points(const wchar_t* str, size_t count) noexcept {
        if constexpr (sizeof(wchar_t) == sizeof(char16_t)) {
            const wchar_t* ptr = str;
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

        if constexpr (sizeof(wchar_t) == sizeof(char32_t)) {
            const wchar_t* ptr = str;
            while (ptr && ptr < (str + count)) {
                if (!code_point_length(*ptr))
                    return false;
                ++ptr;
            }
            return true;
        }

        return false;
    }

    static constexpr wchar_t byte_order_mark[1] = {
        sizeof(wchar_t) == sizeof(char16_t) ? 0xFEFF : 0x0000FEFF
    };
};
*/

using wstring      = basic_string<wchar_t>;
using wstring_view = basic_string_view<wchar_t>;

RACHELWTZ_STRINGS_END
#endif