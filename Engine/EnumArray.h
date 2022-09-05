#pragma once
#include <array>
#include <fstream>
#include <iostream>
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

    EnumArray& operator=(const EnumArray& other)
    {
        size_ = other.size_;
        array_ = std::copy(std::begin(other.array_), std::begin(other.array_), array_);
        return *this;
    }
    bool operator==(EnumArray<IndexType, ValueType>& other)
    {
        for (int i = 0; i < size_; i++)
        {
            if (array_[i] != other[i])
                return false;
        }
        return true;
    }
    bool operator!=(EnumArray<IndexType, ValueType>& other)
    {
        return !(*this == other);
    }
    void Barf() const
    {
        std::ofstream out;
        out.open("BitBoards.txt", std::ios_base::app);
        for (int i = 0; i < size_; i++)
        {
            out << std::hex << array_[i] << std::endl;
        }
    }

private:
    int size_ = static_cast<int>(IndexType::kMaxValue) + 1;
    std::array<ValueType, static_cast<int>(IndexType::kMaxValue) + 1> array_;
};