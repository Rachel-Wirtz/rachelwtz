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

template<typename Char, typename Traits = std::char_traits<Char>>
class basic_string_view;

template<typename Char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
class basic_string;

class invalid_character_encoding : public std::exception {
public:
    invalid_character_encoding()
        : std::exception("invalid character encoding")
    {
    }
};

template<typename Char>
struct basic_character_encoding {
    using char_type       = Char;
    using length_type     = uint8_t;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = char_type&;
    using const_reference = const char_type&;
    using pointer         = char_type*;
    using const_pointer   = const char_type*;

    [[nodiscard]]
    static constexpr length_type length(const_reference elem) {
        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr size_type length(const_pointer str) {
        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr size_type count(const_pointer str, size_type len) {
        throw invalid_character_encoding();
    }

    [[nodiscard]]
    static constexpr difference_type difference(const_pointer lhs, const_pointer rhs) {
        throw invalid_character_encoding();
    }
};

template<typename Char, typename Traits = std::char_traits<Char>>
class basic_character_iterator {
public:
    using encoding_type   = basic_character_encoding<Char>;
    using traits_type     = Traits;
    using char_type       = typename encoding_type::char_type;
    using length_type     = typename encoding_type::length_type;
    using size_type       = typename encoding_type::size_type;
    using difference_type = typename encoding_type::difference_type;
    using const_reference = typename encoding_type::const_reference;
    using const_pointer   = typename encoding_type::const_pointer;

    constexpr basic_character_iterator()                                noexcept = default;
    constexpr basic_character_iterator(const basic_character_iterator&) noexcept = default;
    constexpr basic_character_iterator(basic_character_iterator&&)      noexcept = default;
    constexpr ~basic_character_iterator()                               noexcept = default;

    constexpr basic_character_iterator& operator=(const basic_character_iterator&) noexcept = default;
    constexpr basic_character_iterator& operator=(basic_character_iterator&&)      noexcept = default;

    constexpr basic_character_iterator(const_pointer beg, const size_type len, const_pointer ptr) noexcept
        : m_Beg(beg)
        , m_End(beg + len)
        , m_Ptr(ptr)
    {
    }

    [[nodiscard]]
    constexpr basic_string_view<char_type, traits_type> view(void) const {
        return basic_string_view<char_type, traits_type>(m_Ptr, static_cast<size_type>(encoding_type::length(*m_Ptr)));
    }

protected:
    const_pointer m_Beg{};
    const_pointer m_End{};
    const_pointer m_Ptr{};
};

template<typename StringT>
class basic_string_const_iterator : public basic_character_iterator<typename StringT::char_type, typename StringT::traits_type> {
public:
    using basic_character_iterator<typename StringT::char_type, typename StringT::traits_type>::basic_character_iterator;

    using iterator_base   = basic_character_iterator<typename StringT::char_type, typename StringT::traits_type>;
    using traits_type     = typename iterator_base::traits_type;
    using char_type       = typename iterator_base::char_type;
    using length_type     = typename iterator_base::length_type;
    using size_type       = typename iterator_base::size_type;
    using difference_type = typename iterator_base::difference_type;
    using encoding_type   = typename iterator_base::encoding_type;
    using const_pointer   = typename iterator_base::const_pointer;
    using const_reference = typename iterator_base::const_reference;

    using iterator_concept  = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = char_type;
    using element_type      = const value_type;
    using pointer           = element_type*;
    using reference         = element_type&;

    constexpr basic_string_const_iterator(const_pointer ptr, const StringT* str)
        : basic_character_iterator<char_type, traits_type>(str->data(), str->length(), ptr)
    {
    }

    [[nodiscard]]
    pointer operator->() const {
        return this->m_Ptr;
    }

    [[nodiscard]]
    reference operator*() const {
        return *this->m_Ptr;
    }

    [[nodiscard]]
    constexpr reference operator[](const difference_type offset) const {
        if (offset == 0) {
            return *this->m_Ptr;
        }

        difference_type n = 0;
        size_type len = 0;
        while (n < offset) {
            len += encoding_type::length(offset < 0 ? *(this->m_Ptr - len) : *(this->m_Ptr + len));
            ++n;
        }

        if (offset < 0) {
            if (this->m_Ptr - len < this->m_Beg) {
                throw std::out_of_range("iterator out of range");
            }
            return *(this->m_Ptr - len);
        }

        if ((this->m_Ptr + len) > this->m_End) {
            throw std::out_of_range("iterator out of range");
        }

        return *(this->m_Ptr + len);
    }

    constexpr basic_string_const_iterator& operator++() {
        (*this) += 1;
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_const_iterator operator++(int) {
        auto temp = *this;
        ++this;
        return temp;
    }

    constexpr basic_string_const_iterator& operator--() {
        (*this) -= 1;
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_const_iterator operator--(int) {
        auto temp = *this;
        --(*this);
        return temp;
    }

    constexpr basic_string_const_iterator& operator+=(const difference_type offset) {
        if (offset < 0) {
            return this->operator-=(+offset);
        }

        difference_type n = 0;
        size_type len = 0;
        while (n < offset) {
            len += encoding_type::length(*(this->m_Ptr + len));
            ++n;
        }

        if ((this->m_Ptr + len) > this->m_End) {
            throw std::out_of_range("iterator out of range");
        }

        this->m_Ptr += len;

        return *this;
    }

    constexpr basic_string_const_iterator& operator-=(const difference_type offset) {
        if (offset < 0) {
            return this->operator+=(+offset);
        }

        difference_type n = 0;
        size_type len = 0;
        while (n < offset) {
            len += encoding_type::length(*(this->m_Ptr - len));
            ++n;
        }

        if (this->m_Ptr - len < this->m_Beg) {
            throw std::out_of_range("iterator out of range");
        }

        this->m_Ptr -= len;

        return *this;
    }

    [[nodiscard]]
    constexpr bool operator<=(const basic_string_const_iterator& other) const {
        return this->m_Ptr <= other.m_Ptr;
    }

    [[nodiscard]]
    constexpr bool operator>=(const basic_string_const_iterator& other) const {
        return this->m_Ptr >= other.m_Ptr;
    }

    [[nodiscard]]
    constexpr bool operator==(const basic_string_const_iterator& other) const {
        return this->m_Ptr == other.m_Ptr;
    }

    [[nodiscard]]
    constexpr bool operator<(const basic_string_const_iterator& other) const {
        return this->m_Ptr < other.m_Ptr;
    }

    [[nodiscard]]
    constexpr bool operator>(const basic_string_const_iterator& other) const {
        return this->m_Ptr > other.m_Ptr;
    }

    [[nodiscard]]
    constexpr difference_type operator-(const basic_string_const_iterator& other) const {
        return encoding_type::difference(this->m_Ptr, other.m_Ptr);
    }

    [[nodiscard]]
    constexpr basic_string_const_iterator operator+(const difference_type offset) const {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    [[nodiscard]]
    constexpr basic_string_const_iterator operator-(const difference_type offset) const {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    [[nodiscard]]
    friend constexpr basic_string_const_iterator operator+(const difference_type offset, const basic_string_const_iterator& it) {
        it += offset;
        return it;
    }
};

template<typename StringT>
class basic_string_iterator : public basic_string_const_iterator<StringT> {
public:
    using basic_string_const_iterator<StringT>::basic_string_const_iterator;

    using iterator_base     = basic_string_const_iterator<StringT>;
    using traits_type       = typename iterator_base::traits_type;
    using char_type         = typename iterator_base::char_type;
    using length_type       = typename iterator_base::length_type;
    using size_type         = typename iterator_base::size_type;
    using difference_type   = typename iterator_base::difference_type;
    using const_reference   = typename iterator_base::const_reference;
    using const_pointer     = typename iterator_base::const_pointer;
    using encoding_type     = typename iterator_base::encoding_type;

    using iterator_concept  = typename iterator_base::iterator_concept;
    using iterator_category = typename iterator_base::iterator_category;
    using value_type        = typename iterator_base::value_type;
    using element_type      = value_type;
    using pointer           = element_type*;
    using reference         = element_type&;

    [[nodiscard]]
    pointer operator->() const {
        return const_cast<pointer>(iterator_base::operator->());
    }

    [[nodiscard]]
    reference operator*() const {
        return const_cast<reference>(iterator_base::operator*());
    }

    [[nodiscard]]
    constexpr reference operator[](const difference_type offset) const {
        return const_cast<reference>(iterator_base::operator[](offset));
    }

    constexpr basic_string_iterator& operator++() {
        iterator_base::operator++();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_iterator operator++(int) {
        auto temp = *this;
        iterator_base::operator++();
        return temp;
    }

    constexpr basic_string_iterator& operator--() {
        iterator_base::operator--();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_iterator operator--(int) {
        auto temp = *this;
        iterator_base::operator--();
        return temp;
    }

    constexpr basic_string_iterator& operator+=(const difference_type offset) {
        iterator_base::operator+=(offset);
        return *this;
    }

    constexpr basic_string_iterator& operator-=(const difference_type offset) {
        iterator_base::operator-=(offset);
        return *this;
    }

    [[nodiscard]]
    constexpr bool operator<=(const basic_string_iterator& other) const {
        return iterator_base::operator<=(other);
    }

    [[nodiscard]]
    constexpr bool operator>=(const basic_string_iterator& other) const {
        return iterator_base::operator>=(other);
    }

    [[nodiscard]]
    constexpr bool operator==(const basic_string_iterator& other) const {
        return iterator_base::operator==(other);
    }

    [[nodiscard]]
    constexpr bool operator<(const basic_string_iterator& other) const {
        return iterator_base::operator<(other);
    }

    [[nodiscard]]
    constexpr bool operator>(const basic_string_iterator& other) const {
        return iterator_base::operator>(other);
    }

    [[nodiscard]]
    constexpr difference_type operator-(const basic_string_iterator& other) const {
        return iterator_base::operator-(other);
    }

    [[nodiscard]]
    constexpr basic_string_iterator operator+(const difference_type offset) const {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    [[nodiscard]]
    constexpr basic_string_iterator operator-(const difference_type offset) const {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    [[nodiscard]]
    friend constexpr basic_string_iterator operator+(const difference_type offset, const basic_string_iterator& it) {
        it += offset;
        return it;
    }
};

template<typename Char, typename Traits, typename Allocator>
class basic_string {
public:
    using char_type              = Char;
    using traits_type            = Traits;
    using allocator_type         = Allocator;
    using encoding_type          = basic_character_encoding<char_type>;
    using size_type              = typename encoding_type::size_type;
    using difference_type        = typename encoding_type::difference_type;
    using reference              = typename encoding_type::reference;
    using const_reference        = typename encoding_type::const_reference;
    using pointer                = typename encoding_type::pointer;
    using const_pointer          = typename encoding_type::const_pointer;
    using iterator               = basic_string_iterator<basic_string<char_type, traits_type, allocator_type>>;
    using const_iterator         = basic_string_const_iterator<basic_string<char_type, traits_type, allocator_type>>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::basic_string<char_type, traits_type, allocator_type>::npos;

    constexpr basic_string() noexcept(std::is_nothrow_default_constructible_v<Allocator>)
        : basic_string(Allocator())
    {
    }

    explicit constexpr basic_string(const allocator_type& alloc) noexcept
        : m_String(alloc)
    {
    }

    constexpr basic_string(size_type len, char_type elem, const allocator_type& alloc = allocator_type())
        : m_String(std::forward<size_type>(len), std::forward<char_type>(elem), alloc)
    {
    }

    constexpr basic_string(const basic_string& other, size_type pos, size_type len, const allocator_type& alloc = allocator_type())
        : m_String(other.m_String, std::forward<size_type>(pos), std::forward<size_type>(len), alloc)
    {
    }

    constexpr basic_string(const_pointer str, size_type len, const allocator_type& alloc = allocator_type())
        : m_String(str, std::forward<size_type>(len), alloc)
    {
    }

    constexpr basic_string(const_pointer str, const allocator_type& alloc = allocator_type())
        : m_String(str, alloc)
    {
    }

    template<std::input_iterator InputIterator>
    constexpr basic_string(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
        : m_String(std::forward<InputIterator>(first), std::forward<InputIterator>(last), alloc)
    {
    }

    constexpr basic_string(const basic_string& other, const allocator_type& alloc)
        : m_String(other.m_String, alloc)
    {
    }

    constexpr basic_string(basic_string&& other, const allocator_type& alloc) noexcept
        : m_String(std::move(other.m_String), alloc)
    {
    }

    constexpr basic_string(std::initializer_list<char_type> initlist, const allocator_type& alloc = allocator_type())
        : m_String(std::forward<std::initializer_list<char_type>>(initlist), alloc)
    {
    }

    constexpr basic_string(const basic_string_view<char_type, traits_type>& sv, const allocator_type& alloc = allocator_type())
        : m_String(sv.m_StringView, alloc)
    {
    }

    constexpr basic_string(const basic_string_view<char_type, traits_type>& sv, size_type pos, size_type len, const allocator_type& alloc = allocator_type())
        : m_String(sv.m_StringView, std::forward<size_type>(pos), std::forward<size_type>(len), alloc)
    {
    }

    constexpr basic_string(std::nullptr_t)               = delete;
    constexpr basic_string(const basic_string&)          = default;
    constexpr basic_string(basic_string&&)      noexcept = default;
    constexpr ~basic_string()                   noexcept = default;

    constexpr basic_string& operator=(std::nullptr_t)               = delete;
    constexpr basic_string& operator=(const basic_string&)          = default;
    constexpr basic_string& operator=(basic_string&&)      noexcept = default;

    constexpr basic_string& operator=(const_pointer str) {
        m_String = str;
        return *this;
    }

    constexpr basic_string& operator=(char_type elem) {
        m_String = std::forward<char_type>(elem);
        return *this;
    }

    constexpr basic_string& operator=(std::initializer_list<char_type> initlist) {
        m_String = std::forward<std::initializer_list<char_type>>(initlist);
        return *this;
    }

    constexpr basic_string& operator=(const basic_string_view<char_type, traits_type>& sv) {
        m_String = static_cast<const std::basic_string_view<char_type, traits_type&>>(sv);
        return *this;
    }

    constexpr basic_string& assign(size_type len, char_type elem) {
        m_String.assign(std::forward<size_type>(len), std::forward<char_type>(elem));
        return *this;
    }

    constexpr basic_string& assign(const basic_string& str) {
        m_String.assign(str.m_String);
        return *this;
    }

    constexpr basic_string& assign(const basic_string& str, size_type pos, size_type len = npos) {
        m_String.assign(str.m_String, std::forward<size_type>(pos), std::forward<size_type>(len));
        return *this;
    }

    constexpr basic_string& assign(basic_string&& str) noexcept {
        m_String.assign(std::move(str.m_String));
        return *this;
    }

    constexpr basic_string& assign(const_pointer str, size_type len) {
        m_String.assign(str, std::forward<size_type>(len));
        return *this;
    }

    constexpr basic_string& assign(const_pointer str) {
        m_String.assign(str);
        return *this;
    }

    template<std::input_iterator InputIterator>
    constexpr basic_string& assign(InputIterator first, InputIterator last) {
        m_String.assign(std::forward<InputIterator>(first), std::forward<InputIterator>(last));
        return *this;
    }

    constexpr basic_string& assign(std::initializer_list<char_type> initlist) {
        m_String.assign(std::forward<std::initializer_list<char_type>>(initlist));
        return *this;
    }

    constexpr basic_string& assign(const basic_string_view<char_type, traits_type>& sv) {
        m_String.assign(static_cast<const std::basic_string_view<char_type, traits_type&>>(sv));
        return *this;
    }

    constexpr basic_string& assign(const basic_string_view<char_type, traits_type>& sv, size_type pos, size_type len = npos) {
        m_String.assign(sv.m_StringView, std::forward<size_type>(pos), std::forward<size_type>(len));
        return *this;
    }

    [[nodiscard]]
    constexpr reference at(size_type pos) {
        return *(begin() + pos);
    }

    [[nodiscard]]
    constexpr const_reference at(size_type pos) const {
        return *(begin() + pos);
    }

    [[nodiscard]]
    constexpr reference operator[](size_type pos) {
        return at(pos);
    }

    [[nodiscard]]
    constexpr const_reference operator[](size_type pos) const {
        return at(pos);
    }

    [[nodiscard]]
    constexpr reference front(void) {
        return *begin();
    }

    [[nodiscard]]
    constexpr const_reference front(void) const {
        return *begin();
    }

    [[nodiscard]]
    constexpr reference back(void) {
        return *(end() - 1);
    }

    [[nodiscard]]
    constexpr const_reference back(void) const {
        return *(end() - 1);
    }

    [[nodiscard]]
    constexpr const_pointer data(void) const noexcept {
        return m_String.data();
    }

    [[nodiscard]]
    constexpr pointer data(void) noexcept {
        return m_String.data();
    }

    [[nodiscard]]
    constexpr const_pointer c_str(void) const noexcept {
        return m_String.c_str();
    }

    [[nodiscard]]
    constexpr operator basic_string_view<char_type, traits_type>() const noexcept {
        return { m_String.data(), m_String.size() };
    }

    [[nodiscard]]
    constexpr iterator begin(void) noexcept {
        return iterator(data(), this);
    }

    [[nodiscard]]
    constexpr iterator end(void) noexcept {
        return iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr const_iterator begin(void) const noexcept {
        return const_iterator(data(), this);
    }

    [[nodiscard]]
    constexpr const_iterator end(void) const noexcept {
        return const_iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr const_iterator cbegin(void) const noexcept {
        return const_iterator(data(), this);
    }

    [[nodiscard]]
    constexpr const_iterator cend(void) const noexcept {
        return const_iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr reverse_iterator rbegin(void) noexcept {
        return reverse_iterator(end());
    }

    [[nodiscard]]
    constexpr reverse_iterator rend(void) noexcept {
        return reverse_iterator(begin());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator rbegin(void) const noexcept {
        return const_reverse_iterator(end());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator rend(void) const noexcept {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator crbegin(void) const noexcept {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator crend(void) const noexcept {
        return const_reverse_iterator(cbegin());
    }

    [[nodiscard]]
    constexpr bool empty(void) const noexcept {
        return m_String.empty();
    }

    [[nodiscard]]
    constexpr size_type size(void) const noexcept {
        return m_String.size() * sizeof(char_type);
    }

    [[nodiscard]]
    constexpr size_type length(void) const noexcept {
        return m_String.length();
    }

    [[nodiscard]]
    constexpr size_type count(void) const noexcept {
        return encoding_type::count(data(), length());
    }

    [[nodiscard]]
    constexpr size_type max_size(void) const noexcept {
        return m_String.max_size() * sizeof(char_type);
    }

    [[nodiscard]]
    constexpr size_type max_length(void) const noexcept {
        return m_String.max_size();
    }

    constexpr void reserve(size_type new_length) {
        m_String.reserve(new_length);
    }

    [[nodiscard]]
    constexpr size_type capacity(void) const noexcept {
        return m_String.capacity();
    }

    constexpr void shrink_to_fit(void) {
        m_String.shrink_to_fit();
    }

    constexpr void clear(void) noexcept {
        m_String.clear();
    }

protected:
    std::basic_string<char_type, traits_type, allocator_type> m_String{};
};

template<typename StringViewT>
class basic_string_view_const_iterator : public basic_string_const_iterator<StringViewT> {
public:
    using basic_string_const_iterator<StringViewT>::basic_string_const_iterator;

    using iterator_base     = basic_string_const_iterator<StringViewT>;
    using traits_type       = typename iterator_base::traits_type;
    using char_type         = typename iterator_base::char_type;
    using length_type       = typename iterator_base::length_type;
    using size_type         = typename iterator_base::size_type;
    using difference_type   = typename iterator_base::difference_type;
    using const_reference   = typename iterator_base::const_reference;
    using const_pointer     = typename iterator_base::const_pointer;
    using encoding_type     = typename iterator_base::encoding_type;

    using iterator_concept  = typename iterator_base::iterator_concept;
    using iterator_category = typename iterator_base::iterator_category;
    using value_type        = typename iterator_base::value_type;
    using element_type      = const value_type;
    using pointer           = element_type*;
    using reference         = element_type&;

    [[nodiscard]]
    pointer operator->() const {
        return iterator_base::operator->();
    }

    [[nodiscard]]
    reference operator*() const {
        return iterator_base::operator*();
    }

    [[nodiscard]]
    constexpr reference operator[](const difference_type offset) const {
        return iterator_base::operator[](offset);
    }

    constexpr basic_string_view_const_iterator& operator++() {
        iterator_base::operator++();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_view_const_iterator operator++(int) {
        auto temp = *this;
        iterator_base::operator++();
        return temp;
    }

    constexpr basic_string_view_const_iterator& operator--() {
        iterator_base::operator--();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_view_const_iterator operator--(int) {
        auto temp = *this;
        iterator_base::operator--();
        return temp;
    }

    constexpr basic_string_view_const_iterator& operator+=(const difference_type offset) {
        iterator_base::operator+=(offset);
        return *this;
    }

    constexpr basic_string_view_const_iterator& operator-=(const difference_type offset) {
        iterator_base::operator-=(offset);
        return *this;
    }

    [[nodiscard]]
    constexpr bool operator<=(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator<=(other);
    }

    [[nodiscard]]
    constexpr bool operator>=(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator>=(other);
    }

    [[nodiscard]]
    constexpr bool operator==(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator==(other);
    }

    [[nodiscard]]
    constexpr bool operator<(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator<(other);
    }

    [[nodiscard]]
    constexpr bool operator>(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator>(other);
    }

    [[nodiscard]]
    constexpr difference_type operator-(const basic_string_view_const_iterator& other) const {
        return iterator_base::operator-(other);
    }

    [[nodiscard]]
    constexpr basic_string_view_const_iterator operator+(const difference_type offset) const {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    [[nodiscard]]
    constexpr basic_string_view_const_iterator operator-(const difference_type offset) const {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    [[nodiscard]]
    friend constexpr basic_string_view_const_iterator operator+(const difference_type offset, const basic_string_view_const_iterator& it) {
        it += offset;
        return it;
    }
};

template<typename StringViewT>
class basic_string_view_iterator : public basic_string_view_const_iterator<StringViewT> {
public:
    using basic_string_view_const_iterator<StringViewT>::basic_string_view_const_iterator;
    using iterator_base     = basic_string_view_const_iterator<StringViewT>;
    using traits_type       = typename iterator_base::traits_type;
    using char_type         = typename iterator_base::char_type;
    using length_type       = typename iterator_base::length_type;
    using size_type         = typename iterator_base::size_type;
    using difference_type   = typename iterator_base::difference_type;
    using const_reference   = typename iterator_base::const_reference;
    using const_pointer     = typename iterator_base::const_pointer;
    using encoding_type     = typename iterator_base::encoding_type;

    using iterator_concept  = typename iterator_base::iterator_concept;
    using iterator_category = typename iterator_base::iterator_category;
    using value_type        = typename iterator_base::value_type;
    using element_type      = value_type;
    using pointer           = element_type*;
    using reference         = element_type&;

    [[nodiscard]]
    pointer operator->() const {
        return const_cast<pointer>(iterator_base::operator->());
    }

    [[nodiscard]]
    reference operator*() const {
        return const_cast<reference>(iterator_base::operator*());
    }

    [[nodiscard]]
    constexpr reference operator[](const difference_type offset) const {
        return const_cast<reference>(iterator_base::operator[](offset));
    }

    constexpr basic_string_view_iterator& operator++() {
        iterator_base::operator++();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_view_iterator operator++(int) {
        auto temp = *this;
        iterator_base::operator++();
        return temp;
    }

    constexpr basic_string_view_iterator& operator--() {
        iterator_base::operator--();
        return *this;
    }

    [[nodiscard]]
    constexpr basic_string_view_iterator operator--(int) {
        auto temp = *this;
        iterator_base::operator--();
        return temp;
    }

    constexpr basic_string_view_iterator& operator+=(const difference_type offset) {
        iterator_base::operator+=(offset);
        return *this;
    }

    constexpr basic_string_view_iterator& operator-=(const difference_type offset) {
        iterator_base::operator-=(offset);
        return *this;
    }

    [[nodiscard]]
    constexpr bool operator<=(const basic_string_view_iterator& other) const {
        return iterator_base::operator<=(other);
    }

    [[nodiscard]]
    constexpr bool operator>=(const basic_string_view_iterator& other) const {
        return iterator_base::operator>=(other);
    }

    [[nodiscard]]
    constexpr bool operator==(const basic_string_view_iterator& other) const {
        return iterator_base::operator==(other);
    }

    [[nodiscard]]
    constexpr bool operator<(const basic_string_view_iterator& other) const {
        return iterator_base::operator<(other);
    }

    [[nodiscard]]
    constexpr bool operator>(const basic_string_view_iterator& other) const {
        return iterator_base::operator>(other);
    }

    [[nodiscard]]
    constexpr difference_type operator-(const basic_string_view_iterator& other) const {
        return iterator_base::operator-(other);
    }

    [[nodiscard]]
    constexpr basic_string_view_iterator operator+(const difference_type offset) const {
        auto temp = *this;
        temp += offset;
        return temp;
    }

    [[nodiscard]]
    constexpr basic_string_view_iterator operator-(const difference_type offset) const {
        auto temp = *this;
        temp -= offset;
        return temp;
    }

    [[nodiscard]]
    friend constexpr basic_string_view_iterator operator+(const difference_type offset, const basic_string_view_iterator& it) {
        it += offset;
        return it;
    }
};

template<typename Char, typename Traits>
class basic_string_view {
public:
    using char_type              = Char;
    using traits_type            = Traits;
    using encoding_type          = basic_character_encoding<char_type>;
    using size_type              = typename encoding_type::size_type;
    using difference_type        = typename encoding_type::difference_type;
    using reference              = typename encoding_type::reference;
    using const_reference        = typename encoding_type::const_reference;
    using pointer                = typename encoding_type::pointer;
    using const_pointer          = typename encoding_type::const_pointer;
    using iterator               = basic_string_iterator<basic_string_view<char_type, traits_type>>;
    using const_iterator         = basic_string_const_iterator<basic_string_view<char_type, traits_type>>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = std::basic_string_view<char_type, traits_type>::npos;

    constexpr basic_string_view(std::nullptr_t)                               = delete;
    constexpr basic_string_view()                                    noexcept = default;
    constexpr basic_string_view(const basic_string_view&)            noexcept = default;
    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;

    constexpr basic_string_view(const_pointer str, size_type len)
        : m_StringView(str, std::forward<size_type>(len))
    {
    }

    constexpr basic_string_view(const_pointer str)
        : m_StringView(str)
    {
    }

    constexpr basic_string_view(auto first, auto last)
        : m_StringView(std::forward<decltype(first)>(first), std::forward<decltype(last)>(last))
    {
    }

    [[nodiscard]]
    constexpr iterator begin(void) noexcept {
        return iterator(m_StringView.data(), this);
    }

    [[nodiscard]]
    constexpr iterator end(void) noexcept {
        return iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr const_iterator begin(void) const noexcept {
        return const_iterator(data(), this);
    }

    [[nodiscard]]
    constexpr const_iterator end(void) const noexcept {
        return const_iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr const_iterator cbegin(void) const noexcept {
        return const_iterator(data(), this);
    }

    [[nodiscard]]
    constexpr const_iterator cend(void) const noexcept {
        return const_iterator(data() + length(), this);
    }

    [[nodiscard]]
    constexpr reverse_iterator rbegin(void) noexcept {
        return reverse_iterator(end());
    }

    [[nodiscard]]
    constexpr reverse_iterator rend(void) noexcept {
        return reverse_iterator(begin());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator rbegin(void) const noexcept {
        return const_reverse_iterator(end());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator rend(void) const noexcept {
        return const_reverse_iterator(begin());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator crbegin(void) const noexcept {
        return const_reverse_iterator(cend());
    }

    [[nodiscard]]
    constexpr const_reverse_iterator crend(void) const noexcept {
        return const_reverse_iterator(cbegin());
    }

    [[nodiscard]]
    constexpr size_type size(void) const noexcept {
        return m_StringView.size() * sizeof(char_type);
    }

    [[nodiscard]]
    constexpr size_type length(void) const noexcept {
        return m_StringView.length();
    }

    [[nodiscard]]
    constexpr size_type count(void) const noexcept {
        return encoding_type::count(data(), length());
    }

    [[nodiscard]]
    constexpr const_pointer data(void) const noexcept {
        return m_StringView.data();
    }

protected:
    std::basic_string_view<char_type, traits_type> m_StringView;

    friend class basic_string<char_type, traits_type>;
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