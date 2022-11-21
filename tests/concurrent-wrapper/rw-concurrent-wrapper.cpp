#include "rw-concurrent-wrapper.hpp"
using namespace RACHELWTZ_NAMESPACE;

#include <vector>

int main() {    
    try {
        concurrent_wrapper<std::vector<int>> storage = std::initializer_list{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
        concurrent_wrapper<std::vector<int>> storage_copy(storage);
        concurrent_wrapper<std::vector<int>> storage_move(std::move(storage_copy));
        storage = std::vector<int>{ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
        storage_copy = storage;
        storage_move = std::move(storage_copy);

        std::size_t size = storage([](const std::vector<int>& vec) noexcept { 
            return vec.size(); 
        });

        if (size != 10) 
            return 1;

        size = storage([](std::vector<int>& vec) noexcept {
            vec.push_back(110);
            return vec.size(); 
        });

        if (size != 11)
            return 1;
    }
    catch(...) {
        return 1;
    }
    return 0;
}