// sorting.hpp
#pragma once
#include "../datastructs/DynamicArray.hpp"

namespace algo {
    // Improved QuickSort implementation
    template <typename Iterator, typename Compare>
    void quick_sort(Iterator first, Iterator last, Compare comp) {
        if (first >= last) return;

        Iterator pivot = first;
        Iterator left = first + 1;
        Iterator right = last;

        while (left <= right) {
            while (left <= right && comp(*left, *pivot)) ++left;
            while (left <= right && comp(*pivot, *right)) --right;
            if (left <= right) {
                std::swap(*left, *right);
                ++left;
                --right;
            }
        }

        std::swap(*pivot, *right);
        quick_sort(first, right - 1, comp);
        quick_sort(left, last, comp);
    }

    // Basic quick_sort without comparator (uses operator<)
    template <typename Iterator>
    void quick_sort(Iterator first, Iterator last) {
        quick_sort(first, last, [](const auto& a, const auto& b) { return a < b; });
    }

    // Merge function for merge_sort
    template <typename Iterator>
    void merge(Iterator first, Iterator middle, Iterator last) {
        using ValueType = typename Iterator::value_type;
        DynamicArray<ValueType> temp(std::distance(first, last));
        
        Iterator left = first;
        Iterator right = middle;
        typename DynamicArray<ValueType>::Iterator temp_it = temp.begin();

        while (left != middle && right != last) {
            *temp_it++ = (*left < *right) ? *left++ : *right++;
        }

        while (left != middle) *temp_it++ = *left++;
        while (right != last) *temp_it++ = *right++;

        std::copy(temp.begin(), temp.end(), first);
    }

    // MergeSort implementation
    template <typename Iterator>
    void merge_sort(Iterator first, Iterator last) {
        auto size = std::distance(first, last);
        if (size <= 1) return;

        Iterator middle = first + size / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        merge(first, middle, last);
    }
}