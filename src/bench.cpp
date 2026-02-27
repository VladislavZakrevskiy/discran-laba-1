#include <chrono>
#include <iostream>

#include "contest.hpp"

using TDuration = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

// using TDuration = std::chrono::milliseconds;
// const std::string DURATION_PREFIX = "ms";

int main() {
    TVector<TElement> input, inputStl;
    uint32_t key;
    uint64_t value;
    while (std::cin >> key >> value) {
        input.PushBack(TElement{key, value});
    }
    inputStl = input;

    std::cout << "Count of lines is " << input.Size() << '\n';

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> startTs = std::chrono::system_clock::now();
    NCountingSort::CountingSort(input);
    std::chrono::time_point<std::chrono::system_clock> endTs = std::chrono::system_clock::now();

    uint64_t countingSortTs = std::chrono::duration_cast<TDuration>(endTs - startTs).count();

    // Измеряем время работы stl сортировки.
    startTs = std::chrono::system_clock::now();
    std::stable_sort(inputStl.Begin(), inputStl.End(), NCountingSort::CompareById);
    endTs = std::chrono::system_clock::now();

    uint64_t stlSortTs = std::chrono::duration_cast<TDuration>(endTs - startTs).count();

    std::cout << "Counting sort time: " << countingSortTs << DURATION_PREFIX << '\n';
    std::cout << "STL stable sort time: " << stlSortTs << DURATION_PREFIX << '\n';
}