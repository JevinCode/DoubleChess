#pragma once
#include <array>

template <class IndexType, class ValueType>
class EnumArray {
public:
    EnumArray()
        :
        size_(static_cast<int>(IndexType::kMaxValue) + 1),
        array_{ 0 }
    {}
    EnumArray(std::array<ValueType, static_cast<int>(IndexType::kMaxValue) + 1> arr)
        :
        array_(arr)
    {}
    ValueType& operator[](IndexType i) {
        return array_[static_cast<int>(i)];
    }

    const ValueType& operator[](IndexType i) const {
        return array_[static_cast<int>(i)];
    }

    template<class T>
    ValueType& operator[](T t)
    {
        return array_[static_cast<int>(t)];
    }
    int size() const { return size_; }

private:
    int size_ = static_cast<int>(IndexType::kMaxValue) + 1;
    std::array<ValueType, static_cast<int>(IndexType::kMaxValue) + 1> array_;
};