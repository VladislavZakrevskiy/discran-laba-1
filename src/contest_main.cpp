#include "contest.hpp"

constexpr int ID_WIDTH = 6;
constexpr int SCAN_ITEMS = 2;

int main() {
    TVector<TElement> arr;
    arr.Reserve(INPUT_RESERVE);

    TElement element{};

    while (std::scanf("%u%llu", &element.id, &element.value) == SCAN_ITEMS) {
        arr.PushBack(element);
    }

    NCountingSort::CountingSort(arr);

    for (size_t i = 0; i < arr.Size(); i++) {
        std::printf("%0*u\t%llu\n", ID_WIDTH, arr[i].id,
                    static_cast<unsigned long long>(arr[i].value));
    }

    return 0;
}