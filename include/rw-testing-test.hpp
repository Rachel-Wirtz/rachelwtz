#ifndef RACHELWTZ__TESTING__TEST__HPP
#define RACHELWTZ__TESTING__TEST__HPP

#include "rw-testing.hpp"

RACHELWTZ_TESTING_BEGIN

class test {
public:
    using function_type = std::function<void()>;
    using list_type     = std::vector<function_type>;

    int run(void) const noexcept {        
        bool ok = true;

        for (const auto& f : m_Tests) {
            try {
                f();
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                ok = false;
            }
        }
        return ok ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    void add(std::invocable auto&& f) noexcept {
        m_Tests.emplace_back(function_type(std::forward<decltype(f)>(f)));
    }

private:
    list_type m_Tests{};
};

RACHELWTZ_TESTING_END
#endif