#ifndef RACHELWTZ__STRINGS__U32STRING__HPP
#define RACHELWTZ__STRINGS__U32STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

template<>
struct char_traits<char32_t> : public std::char_traits<char32_t> {
    [[nodiscard]]
    static constexpr uint8_t code_point_length(const char32_t& c) noexcept {
        if (c >= 0x000000 && c <= 0x10FFFF)
            return 1;
        return 0;
    }

    [[nodiscard]]
    static constexpr size_t code_point_count(const char32_t* str, size_t count) noexcept {
        const char32_t* ptr = str;
        while (ptr && ptr < (str + count)) {
            if (!code_point_length(*ptr))
                return 0;
            ++ptr;
        }
        return count;
    }

    [[nodiscard]]
    static constexpr bool validate_code_points(const char32_t* str, size_t count) noexcept {
        const char32_t* ptr = str;
        while (ptr && ptr < (str + count)) {
            if (!code_point_length(*ptr))
                return false;
            ++ptr;
        }
        return true;
    }
};

using u32string      = basic_string<char32_t>;
using u32string_view = basic_string_view<char32_t>;

RACHELWTZ_STRINGS_END
#endif