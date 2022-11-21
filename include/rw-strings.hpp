#ifndef RACHELWTZ__STRINGS__HPP
#define RACHELWTZ__STRINGS__HPP

#include "rw-core.hpp"

#include <cctype>
#include <cuchar>
#include <cwchar>
#include <string>

#define RACHELWTZ_STRINGS_NAMESPACE strings
#define RACHELWTZ_STRINGS_BEGIN     namespace RACHELWTZ_NAMESPACE::RACHELWTZ_STRINGS_NAMESPACE {
#define RACHELWTZ_STRINGS_END       }
#define USING_RACHELWTZ_STRINGS     using namespace ::RACHELWTZ_NAMESPACE::RACHELWTZ_STRINGS_NAMESPACE

RACHELWTZ_STRINGS_BEGIN

//
// Specified for each character type.
// Make sure to implement according to ChartTraits.
//
template<typename CharT>
struct char_traits { };

template<typename CharT, typename TraitsT = char_traits<CharT>, typename AllocT = std::allocator<CharT>>
class basic_string : public std::basic_string<CharT, TraitsT, AllocT> {
public:    
    using std::basic_string<CharT, TraitsT, AllocT>::basic_string;

    using base_type              = std::basic_string<CharT, TraitsT, AllocT>;
    using traits_type            = typename base_type::traits_type;
    using value_type             = typename base_type::value_type;
    using allocator_type         = typename base_type::allocator_type;
    using size_type              = typename base_type::size_type;
    using difference_type        = typename base_type::difference_type;
    using reference              = typename base_type::reference;
    using const_reference        = typename base_type::const_reference;
    using pointer                = typename base_type::pointer;
    using const_pointer          = typename base_type::const_pointer;
    using iterator               = typename base_type::iterator;
    using const_iterator         = typename base_type::const_iterator;
    using reverse_iterator       = typename base_type::reverse_iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;

    constexpr size_type size(void) const noexcept {
        return (base_type::size() + 1) * sizeof(value_type);
    }

    constexpr size_type length(void) const noexcept {
        return (base_type::length() + 1);
    }

    constexpr size_type count(void) const noexcept {
        return traits_type::code_point_count(this->data(), this->length() - 1);
    }

    constexpr bool is_valid(void) const noexcept {
        return traits_type::validate_code_points(this->data(), this->length() - 1);
    }
};

template<typename CharT, typename TraitsT = char_traits<CharT>>
class basic_string_view : public std::basic_string_view<CharT, TraitsT> {
public:
    using std::basic_string_view<CharT, TraitsT>::basic_string_view;

    using base_type              = std::basic_string_view<CharT, TraitsT>;
    using traits_type            = typename base_type::traits_type;
    using value_type             = typename base_type::value_type;
    using pointer                = typename base_type::pointer;
    using const_pointer          = typename base_type::const_pointer;
    using reference              = typename base_type::reference;
    using const_reference        = typename base_type::const_reference;
    using const_iterator         = typename base_type::const_iterator;
    using iterator               = typename base_type::iterator;
    using const_reverse_iterator = typename base_type::const_reverse_iterator;
    using reverse_iterator       = typename base_type::reverse_iterator;
    using size_type              = typename base_type::size_type;
    using difference_type        = typename base_type::difference_type;

    constexpr size_type size(void) const noexcept {
        return (base_type::size() + 1) * sizeof(value_type);
    }

    constexpr size_type length(void) const noexcept {
        return (base_type::length() + 1);
    }

    constexpr size_type count(void) const noexcept {
        return traits_type::code_point_count(this->data(), this->length() - 1);
    }

    constexpr bool is_valid(void) const noexcept {
        return traits_type::validate_code_points(this->data(), this->length() - 1);
    }
};

RACHELWTZ_STRINGS_END

//
// Strings Libraries
//
#include "rw-strings-string.hpp"
#include "rw-strings-wstring.hpp"
#include "rw-strings-u8string.hpp"
#include "rw-strings-u16string.hpp"
#include "rw-strings-u32string.hpp"

#endif