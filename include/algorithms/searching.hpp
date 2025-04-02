#pragma once

namespace algo {
    template <typename Iterator, typename T>
    Iterator binary_search(Iterator first, Iterator last, const T& value) {
        Iterator not_found = last;
        while (first < last) {
            Iterator mid = first + (last - first)/2;
            if (*mid == value) return mid;
            if (*mid < value) first = mid + 1;
            else last = mid;
        }
        return not_found;
    }
}