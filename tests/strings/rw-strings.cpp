#define RACHELWTZ_TEST_STRINGS

#include "rw-strings.hpp"
#include "rw-testing.hpp"

USING_RACHELWTZ;
USING_RACHELWTZ_STRINGS;
USING_RACHELWTZ_TESTING;

int main() {
    test test{};

    test.add([]() constexpr {
        constexpr string_view ansi = "Hello, World!";
        constexpr auto ansi_size  = ansi.size();
        constexpr auto ansi_len   = ansi.length();
        constexpr auto ansi_count = ansi.count();
        static_assert(ansi_size  == 14, "FAIL: ANSI string size incorrect.");
        static_assert(ansi_len   == 14, "FAIL: ANSI string length incorrect.");
        static_assert(ansi_count == 13, "FAIL: ANSI string count incorrect.");
        static_assert(ansi.is_valid(),  "FAIL: ANSI string validated incorrect.");

        string ansi_str{ ansi };
        size_t ansi_str_size  = ansi_str.size();
        size_t ansi_str_len   = ansi_str.length();
        size_t ansi_str_count = ansi_str.count();
        RACHELWTZ_ASSERT(ansi_str_size  == 14);
        RACHELWTZ_ASSERT(ansi_str_len   == 14);
        RACHELWTZ_ASSERT(ansi_str_count == 13);
        RACHELWTZ_ASSERT(ansi_str.is_valid());

        string_view ansi_sv{ ansi };
        size_t ansi_sv_size  = ansi_sv.size();
        size_t ansi_sv_len   = ansi_sv.length();
        size_t ansi_sv_count = ansi_sv.count();
        RACHELWTZ_ASSERT(ansi_sv_size  == 14);
        RACHELWTZ_ASSERT(ansi_sv_len   == 14);
        RACHELWTZ_ASSERT(ansi_sv_count == 13);
        RACHELWTZ_ASSERT(ansi_sv.is_valid());
    });

    test.add([]() constexpr {
        constexpr wstring_view wide = L"Hello, World!";
        constexpr auto wide_size  = wide.size();
        constexpr auto wide_len   = wide.length();
        constexpr auto wide_count = wide.count();
        static_assert(wide_size  == 28, "FAIL: Wide string size incorrect.");
        static_assert(wide_len   == 14, "FAIL: Wide string length incorrect.");
        static_assert(wide_count == 13, "FAIL: Wide string count incorrect.");
        static_assert(wide.is_valid(),  "FAIL: Wide string validated incorrect.");

        wstring wide_str{ wide };
        size_t wide_str_size  = wide_str.size();
        size_t wide_str_len   = wide_str.length();
        size_t wide_str_count = wide_str.count();
        RACHELWTZ_ASSERT(wide_str_size  == 28);
        RACHELWTZ_ASSERT(wide_str_len   == 14);
        RACHELWTZ_ASSERT(wide_str_count == 13);
        RACHELWTZ_ASSERT(wide_str.is_valid());

        wstring_view wide_sv{ wide_str };
        size_t wide_sv_size  = wide_sv.size();
        size_t wide_sv_len   = wide_sv.length();
        size_t wide_sv_count = wide_sv.count();
        RACHELWTZ_ASSERT(wide_sv_size  == 28);
        RACHELWTZ_ASSERT(wide_sv_len   == 14);
        RACHELWTZ_ASSERT(wide_sv_count == 13);
        RACHELWTZ_ASSERT(wide_sv.is_valid());
    });

    test.add([]() constexpr {
        constexpr u8string_view utf8 = u8"Hello, World! €";
        constexpr auto utf8_size  = utf8.size();
        constexpr auto utf8_len   = utf8.length();
        constexpr auto utf8_count = utf8.count();
        static_assert(utf8_size  == 18, "FAIL: UTF-8 string size incorrect.");
        static_assert(utf8_len   == 18, "FAIL: UTF-8 string length incorrect.");
        static_assert(utf8_count == 15, "FAIL: UTF-8 string count incorrect.");
        static_assert(utf8.is_valid(),  "FAIL: UTF-8 string validated incorrect.");

        u8string utf8_str{ utf8 };
        size_t utf8_str_size  = utf8_str.size();
        size_t utf8_str_len   = utf8_str.length();
        size_t utf8_str_count = utf8_str.count();
        RACHELWTZ_ASSERT(utf8_str_size  == 18);
        RACHELWTZ_ASSERT(utf8_str_len   == 18);
        RACHELWTZ_ASSERT(utf8_str_count == 15);
        RACHELWTZ_ASSERT(utf8_str.is_valid());

        u8string_view utf8_sv{ utf8_str };
        size_t utf8_sv_size  = utf8_sv.size();
        size_t utf8_sv_len   = utf8_sv.length();
        size_t utf8_sv_count = utf8_sv.count();
        RACHELWTZ_ASSERT(utf8_sv_size  == 18);
        RACHELWTZ_ASSERT(utf8_sv_len   == 18);
        RACHELWTZ_ASSERT(utf8_sv_count == 15);
        RACHELWTZ_ASSERT(utf8_sv.is_valid());
    });

    test.add([]() constexpr {
        constexpr u16string_view utf16 = u"Hello, World! \xD852\xDF62";
        constexpr auto utf16_size  = utf16.size();
        constexpr auto utf16_len   = utf16.length();
        constexpr auto utf16_count = utf16.count();
        static_assert(utf16_size  == 34, "FAIL: UTF-16 string size incorrect.");
        static_assert(utf16_len   == 17, "FAIL: UTF-16 string length incorrect.");
        static_assert(utf16_count == 15, "FAIL: UTF-16 string count incorrect.");
        static_assert(utf16.is_valid(),  "FAIL: UTF-16 string validated incorrect.");

        u16string utf16_str{ utf16 };
        size_t utf16_str_size  = utf16_str.size();
        size_t utf16_str_len   = utf16_str.length();
        size_t utf16_str_count = utf16_str.count();
        RACHELWTZ_ASSERT(utf16_str_size  == 34);
        RACHELWTZ_ASSERT(utf16_str_len   == 17);
        RACHELWTZ_ASSERT(utf16_str_count == 15);
        RACHELWTZ_ASSERT(utf16_str.is_valid());

        u16string_view utf16_sv{ utf16_str };
        size_t utf16_sv_size  = utf16_sv.size();
        size_t utf16_sv_len   = utf16_sv.length();
        size_t utf16_sv_count = utf16_sv.count();
        RACHELWTZ_ASSERT(utf16_sv_size  == 34);
        RACHELWTZ_ASSERT(utf16_sv_len   == 17);
        RACHELWTZ_ASSERT(utf16_sv_count == 15);
        RACHELWTZ_ASSERT(utf16_sv.is_valid());
    });

    test.add([]() constexpr {
        constexpr u32string_view utf32 = U"Hello, World! \x092A";
        constexpr auto utf32_size  = utf32.size();
        constexpr auto utf32_len   = utf32.length();
        constexpr auto utf32_count = utf32.count();
        static_assert(utf32_size  == 64, "FAIL: UTF-32 string size incorrect.");
        static_assert(utf32_len   == 16, "FAIL: UTF-32 string length incorrect.");
        static_assert(utf32_count == 15, "FAIL: UTF-32 string count incorrect.");
        static_assert(utf32.is_valid(),  "FAIL: UTF-32 string validated incorrect.");

        u32string utf32_str{ utf32 };
        size_t utf32_str_size  = utf32_str.size();
        size_t utf32_str_len   = utf32_str.length();
        size_t utf32_str_count = utf32_str.count();
        RACHELWTZ_ASSERT(utf32_str_size  == 64);
        RACHELWTZ_ASSERT(utf32_str_len   == 16);
        RACHELWTZ_ASSERT(utf32_str_count == 15);
        RACHELWTZ_ASSERT(utf32_str.is_valid());

        u32string_view utf32_sv{ utf32_str };
        size_t utf32_sv_size  = utf32_sv.size();
        size_t utf32_sv_len   = utf32_sv.length();
        size_t utf32_sv_count = utf32_sv.count();
        RACHELWTZ_ASSERT(utf32_sv_size  == 64);
        RACHELWTZ_ASSERT(utf32_sv_len   == 16);
        RACHELWTZ_ASSERT(utf32_sv_count == 15);
        RACHELWTZ_ASSERT(utf32_sv.is_valid());
    });

    return test.run();
}