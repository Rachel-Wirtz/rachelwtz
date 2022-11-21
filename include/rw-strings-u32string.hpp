#ifndef RACHELWTZ__STRINGS__U32STRING__HPP
#define RACHELWTZ__STRINGS__U32STRING__HPP

#include "rw-strings.hpp"

RACHELWTZ_STRINGS_BEGIN

template<>
struct char_traits<char32_t> : public std::char_traits<char32_t> {
    [[nodiscard]]
    static constexpr uint8_t code_point_length(const char32_t& c) noexcept {
        return 1;
    }

    [[nodiscard]]
    static constexpr size_t code_point_count(const char32_t* str, size_t count) noexcept {
        return count;
    }

    [[nodiscard]]
    static constexpr bool validate_code_points(const char32_t* str, size_t count) noexcept {
        return true;
    }
};

using u32string      = basic_string<char32_t>;
using u32string_view = basic_string_view<char32_t>;

RACHELWTZ_STRINGS_END
#endif