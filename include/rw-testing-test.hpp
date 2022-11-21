#ifndef RACHELWTZ__TESTING__TEST__HPP
#define RACHELWTZ__TESTING__TEST__HPP

#include "rw-testing.hpp"

RACHELWTZ_TESTING_BEGIN

class test {
public:
    using function_type = std::function<void()>;
    using pair_type     = std::pair<const char*, function_type>;
    using list_type     = std::vector<pair_type>;

    test(const char* name) noexcept
        : m_Name(name)
        , m_Tests{}
    {
    }

    int run(void) const noexcept {        
        bool ok = true;

        std::cout << "==============================================================================================" << std::endl;
        std::cout << " RUNNING TEST: " << m_Name << std::endl;
        std::cout << "==============================================================================================" << std::endl;

        for (const auto& f : m_Tests) {
            try {
                f.second();
                std::cout << f.first << ": OK!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << f.first << ": " << e.what() << std::endl;
                ok = false;
            }
        }

        return ok ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    void add(const char* name, std::invocable auto&& f) noexcept {
        m_Tests.emplace_back(pair_type(name, std::function(std::forward<decltype(f)>(f))));
    }

private:
    const char* m_Name{};
    list_type   m_Tests{};
};

RACHELWTZ_TESTING_END
#endif