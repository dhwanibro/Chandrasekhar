#pragma once
#include <cstdlib>
#include <stdexcept>
#include <iterator>
#include <utility>  // For std::move

template <typename T>
class DynamicArray {
    public:
    T* data;
    size_t _capacity;
    size_t _size;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        _capacity = new_capacity;
    }

    // ✅ Default Constructor
    DynamicArray() : data(new T[1]), _capacity(1), _size(0) {}

    // ✅ Copy Constructor (Deep Copy)
    DynamicArray(const DynamicArray& other) 
        : _capacity(other._capacity), _size(other._size) {
        data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            data[i] = other.data[i];
        }
    }

    // ✅ Copy Assignment Operator (Deep Copy)
    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this; // Avoid self-assignment

        delete[] data; // Free old memory

        _capacity = other._capacity;
        _size = other._size;
        data = new T[_capacity];

        for (size_t i = 0; i < _size; ++i) {
            data[i] = other.data[i];
        }
        
        return *this;
    }

    // ✅ Move Constructor (Steal Resources)
    DynamicArray(DynamicArray&& other) noexcept 
        : data(other.data), _capacity(other._capacity), _size(other._size) {
        other.data = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    // ✅ Move Assignment Operator (Steal Resources)
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this == &other) return *this;

        delete[] data; // Free old memory

        data = other.data;
        _capacity = other._capacity;
        _size = other._size;

        other.data = nullptr;
        other._capacity = 0;
        other._size = 0;

        return *this;
    }

    // ✅ Destructor
    ~DynamicArray() { 
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            resize(_capacity * 2);
        }
        data[_size++] = value;
    }

    T& operator[](size_t index) {
        if (index >= _size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    
    // ✅ New empty() function
    bool empty() const { return _size == 0; }

    class Iterator {
        T* ptr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        Iterator(T* p) : ptr(p) {}

        reference operator*() { return *ptr; }
        pointer operator->() { return ptr; }

        Iterator& operator++() { ++ptr; return *this; }
        Iterator operator++(int) { Iterator temp = *this; ++ptr; return temp; }
        Iterator& operator--() { --ptr; return *this; }
        Iterator operator--(int) { Iterator temp = *this; --ptr; return temp; }

        Iterator operator+(size_t n) const { return Iterator(ptr + n); }
        Iterator operator-(size_t n) const { return Iterator(ptr - n); }
        difference_type operator-(const Iterator& other) const { return ptr - other.ptr; }

        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        bool operator<(const Iterator& other) const { return ptr < other.ptr; }
        bool operator>(const Iterator& other) const { return ptr > other.ptr; }
        bool operator<=(const Iterator& other) const { return ptr <= other.ptr; }
        bool operator>=(const Iterator& other) const { return ptr >= other.ptr; }
    };

    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + _size); }

    Iterator begin() const { return Iterator(data); }
    Iterator end() const { return Iterator(data + _size); }
};
