#ifndef RACHELWTZ__STRINGS__WSTRING__HPP
#define RACHELWTZ__STRINGS__WSTRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

//
// Assuming that wchar_t holds either UTF16 or UTF32.
//
template<>
struct char_traits<wchar_t> : public std::char_traits<wchar_t> {
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
};

using wstring      = basic_string<wchar_t>;
using wstring_view = basic_string_view<wchar_t>;

RACHELWTZ_STRINGS_END
#endif