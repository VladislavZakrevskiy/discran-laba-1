#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <utility>

constexpr short GROWTH_FACTOR = 2;
constexpr short DEFAULT_CAPACITY = 1;
constexpr size_t INPUT_RESERVE = 100000;

template <typename T> class TVector {
  public:
    TVector() = default;

    TVector(size_t count, const T &value) {
        Reserve(count);
        for (size_t i = 0; i < count; ++i) {
            std::allocator_traits<TAllocator>::construct(allocator_, data_ + i, value);
        }
        size_ = count;
    }

    TVector(const TVector &other) {
        Reserve(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            std::allocator_traits<TAllocator>::construct(allocator_, data_ + i, other.data_[i]);
        }
        size_ = other.size_;
    }

    TVector(TVector &&other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    TVector &operator=(const TVector &other) {
        if (this == &other) {
            return *this;
        }

        TVector copy(other);
        Swap(copy);
        return *this;
    }

    TVector &operator=(TVector &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        DestroyStorage();
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    ~TVector() {
        DestroyStorage();
    }

    void PushBack(const T &value) {
        EnsureCapacityForOneMore();
        std::allocator_traits<TAllocator>::construct(allocator_, data_ + size_, value);
        ++size_;
    }

    void PushBack(T &&value) {
        EnsureCapacityForOneMore();
        std::allocator_traits<TAllocator>::construct(allocator_, data_ + size_, std::move(value));
        ++size_;
    }

    void Reserve(size_t newCapacity) {
        if (newCapacity <= capacity_) {
            return;
        }
        Reallocate(newCapacity);
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    T &operator[](size_t index) {
        return data_[index];
    }

    const T &operator[](size_t index) const {
        return data_[index];
    }

    T *Begin() {
        return data_;
    }

    const T *Begin() const {
        return data_;
    }

    T *End() {
        return data_ + size_;
    }

    const T *End() const {
        return data_ + size_;
    }

    void Swap(TVector &other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

  private:
    using TAllocator = std::allocator<T>;

    void EnsureCapacityForOneMore() {
        if (size_ == capacity_) {
            Reserve(capacity_ == 0 ? DEFAULT_CAPACITY : capacity_ * GROWTH_FACTOR);
        }
    }

    void Reallocate(size_t newCapacity) {
        T *newData = std::allocator_traits<TAllocator>::allocate(allocator_, newCapacity);

        for (size_t i = 0; i < size_; ++i) {
            std::allocator_traits<TAllocator>::construct(allocator_, newData + i,
                                                         std::move_if_noexcept(data_[i]));
            std::allocator_traits<TAllocator>::destroy(allocator_, data_ + i);
        }

        if (data_ != nullptr) {
            std::allocator_traits<TAllocator>::deallocate(allocator_, data_, capacity_);
        }

        data_ = newData;
        capacity_ = newCapacity;
    }

    void DestroyStorage() {
        for (size_t i = 0; i < size_; ++i) {
            std::allocator_traits<TAllocator>::destroy(allocator_, data_ + i);
        }

        if (data_ != nullptr) {
            std::allocator_traits<TAllocator>::deallocate(allocator_, data_, capacity_);
        }

        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

  private:
    TAllocator allocator_;
    T *data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

struct TElement {
    uint32_t id;
    uint64_t value;
};

namespace NCountingSort {

bool CompareById(const TElement &left, const TElement &right) {
    return left.id < right.id;
}

void CountingSort(TVector<TElement> &arr) {
    if (arr.Empty()) {
        return;
    }

    auto minMax = std::minmax_element(arr.Begin(), arr.End(), CompareById);

    uint32_t min = minMax.first->id;
    uint32_t max = minMax.second->id;
    size_t range = static_cast<size_t>(max - min) + 1;

    TVector<uint32_t> count(range, 0);
    TVector<TElement> output(arr.Size(), TElement{});

    for (size_t i = 0; i < arr.Size(); i++) {
        count[arr[i].id - min]++;
    }

    for (size_t i = 1; i < count.Size(); i++) {
        count[i] += count[i - 1];
    }

    for (size_t i = arr.Size(); i-- > 0;) {
        output[count[arr[i].id - min] - 1] = arr[i];
        count[arr[i].id - min]--;
    }

    arr = std::move(output);
}

} // namespace NCountingSort
