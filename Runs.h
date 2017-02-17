#ifndef TIMSORT_RUNSNEW_H
#define TIMSORT_RUNSNEW_H

#include <algorithm>

//[first, middle) - sorted part, [middle, last) - not sorted part
template <class RAI, class Compare>
void insertionSort(RAI first, RAI middle, RAI last, const Compare& comp){
    ssize_t median, left, right;
    for (RAI it = middle; it != last; ++it){
        left = -1;
        right = it - first;
        while (left + 1 < right){
            median = (left + right) / 2;
            if (comp(it[0], first[median]))
                right = median;
            else
                left = median;
        }
        for (RAI j = it; j != first + right; --j)
            std::swap(j[0], j[-1]);
    }
}

template <class RAI, class Compare>
void insertionSort(RAI first, RAI last, const Compare& comp){
    insertionSort(first, first, last, comp);
}

//_size next run
template <class RAI, class Compare>
size_t buildNextRun(RAI first, RAI last, size_t minRun, const Compare& comp){
    size_t size = last - first;
    if (size <= 1)
        return size;

    bool decrease = comp(first[1], first[0]);
    RAI sorted = first + 1;
    while ((sorted + 1 != last) && (decrease == comp(sorted[1], sorted[0])))
        ++sorted;
    ++sorted;

    if (decrease)
        std::reverse(first, sorted);

    size_t sortedSize = sorted - first;
    if ((sortedSize >= minRun) && (size - sortedSize >= minRun))
        return sortedSize;

    size_t sizeRun;
    if (size < minRun)
        sizeRun = size;
    else if ((sortedSize < minRun) && (size - minRun >= minRun))
        sizeRun = minRun;
    else
        sizeRun = size;
    insertionSort(first, sorted, first + sizeRun, comp);
    return sizeRun;
}

#endif //TIMSORT_RUNSNEW_H








































