#define RACHELWTZ_TEST_CONCURRENCY

#include "rw-concurrency.hpp"
#include "rw-testing.hpp"

USING_RACHELWTZ;
USING_RACHELWTZ_CONCURRENCY;
USING_RACHELWTZ_TESTING;

#include <vector>

int main() {    
    test test{};

    test.add([]() {
        wrapper<std::vector<int>> storage = std::initializer_list{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
        wrapper<std::vector<int>> storage_copy(storage);
        wrapper<std::vector<int>> storage_move(std::move(storage_copy));
        storage = std::vector<int>{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
        storage_copy = storage;
        storage_move = std::move(storage_copy);

        std::size_t size = storage([&storage](const std::vector<int>& vec) {
            RACHELWTZ_ASSERT(!storage.m_Mtx.try_lock());
            return vec.size(); 
        });

        RACHELWTZ_DEBUG_ASSERT(size == 10);

        size = storage([&storage](std::vector<int>& vec) {
            RACHELWTZ_ASSERT(!storage.m_Mtx.try_lock());
            vec.push_back(110);
            return vec.size(); 
        });

        RACHELWTZ_DEBUG_ASSERT(size == 11);
    });

    return test.run();
}