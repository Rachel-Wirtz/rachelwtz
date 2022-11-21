#ifndef RACHELWTZ__U16STRING__HPP
#define RACHELWTZ__U16STRING__HPP

#include "rw-string-base.hpp"

RACHELWTZ_BEGIN

template<>
struct char_traits<char16_t> : public std::char_traits<char16_t> {
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
};

using u16string					 = basic_string<char16_t>;
using u16string_view = basic_string_view<char16_t>;

RACHELWTZ_END
#endif