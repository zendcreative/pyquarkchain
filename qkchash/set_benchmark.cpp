#include <cstdint>
#include <cassert>

#include <array>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <set>

using namespace std;

#define ELEMENT_TYPE uint32_t

int main() {

    set<ELEMENT_TYPE> oset;

    uint32_t key_size = 64 * 1024;

    std::uniform_int_distribution<ELEMENT_TYPE> dist(0, numeric_limits<ELEMENT_TYPE>::max());
    std::uniform_int_distribution<uint64_t> dist64(0, numeric_limits<uint64_t>::max());

    std::default_random_engine generator(475);

    cout << "Inserting to set ..." << endl;
    auto t_start = std::chrono::steady_clock::now();
    std::vector<ELEMENT_TYPE> inserted;
    for (uint32_t i = 0; i < key_size; i++) {
        ELEMENT_TYPE v = dist(generator);
        inserted.push_back(v);
        oset.insert(v);
    }
    auto used_time = std::chrono::steady_clock::now() - t_start;
    std::cout << std::chrono::duration<double, std::milli>(used_time).count() << std::endl;

    uint32_t count = 1 * 1024 * 1024;
    t_start = std::chrono::steady_clock::now();

    uint64_t total_count = 0;
    while (1) {
        for (uint32_t i = 0; i < count; i++) {
            uint64_t p = dist64(generator) % oset.size();
            oset.erase(inserted[p]);
            inserted[p] = dist(generator);
            oset.insert(inserted[p]);
        }

        total_count += count;
        used_time = std::chrono::steady_clock::now() - t_start;
        std::cout << total_count * 1000 / (uint64_t)std::chrono::duration<double, std::milli>(used_time).count() << std::endl;
    }

    return 0;
}
