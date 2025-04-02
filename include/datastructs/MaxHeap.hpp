#pragma once
#include "DynamicArray.hpp"

template <typename T, typename Compare = std::less<T>>
class MaxHeap {
    DynamicArray<T> heap;
    Compare comp;

    void heapify_up(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (comp(heap[parent], heap[index])) {
                std::swap(heap[parent], heap[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapify_down(size_t index) {
        size_t left, right, largest;
        while (true) {
            left = 2 * index + 1;
            right = 2 * index + 2;
            largest = index;

            if (left < heap.size() && comp(heap[largest], heap[left])) {
                largest = left;
            }

            if (right < heap.size() && comp(heap[largest], heap[right])) {
                largest = right;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    MaxHeap() = default;
    
    void push(const T& value) {
        heap.push_back(value);
        heapify_up(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapify_down(0);
    }

    const T& top() const {
        if (heap.empty()) throw std::out_of_range("Heap is empty");
        return heap[0];
    }

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
};