#ifndef TIMSORT_TIMSORTNEW_H
#define TIMSORT_TIMSORTNEW_H

#include "timsort_params.h"
#include "InplaceMerge.h"
#include <stack>

using std::stack;

template <class RAI, class Compare>
void validateStack(RAI last, stack <size_t> &runSizes, const Compare &comp, const ITimSortParams &params){
    size_t sizeZ = runSizes.top();
    runSizes.pop();
    size_t sizeY = runSizes.top();
    runSizes.pop();
    size_t sizeX = runSizes.top();
    runSizes.pop();

    EWhatMerge whatMerge = params.whatMerge(sizeX, sizeY, sizeZ);
    if (whatMerge == WM_NoMerge){
        runSizes.push(sizeX);
        runSizes.push(sizeY);
        runSizes.push(sizeZ);
        return;

    }
    if (whatMerge == WM_MergeYZ) {
        inplaceMerge(last - sizeY - sizeZ, last - sizeZ, last, comp);
        runSizes.push(sizeX);
        runSizes.push(sizeY + sizeZ);
        validateStack(last, runSizes, comp, params);
    } else {
        inplaceMerge(last - sizeX - sizeY - sizeZ, last - sizeY - sizeZ, last - sizeZ, comp);
        runSizes.push(sizeX + sizeY);
        validateStack(last - sizeZ, runSizes, comp, params);
        runSizes.push(sizeZ);
    }
}

template <class RAI, class Compare>
void timSort(RAI first, RAI last, const Compare& comp, const ITimSortParams& params){
    if (last - first <= 1)
        return;
    size_t size = last - first;
    size_t newRunSize = 0;
    size_t minRun = params.minRun(size);
    stack <size_t> runSizes;
    runSizes.push(2 * (size + 1));
    runSizes.push(size + 1);
    RAI endXYZ = first;
    for (size_t i = 0; i < size; i += newRunSize){
        newRunSize = buildNextRun(endXYZ, last, minRun, comp);
        endXYZ += newRunSize;
        runSizes.push(newRunSize);
        validateStack(endXYZ, runSizes, comp, params);
    }
    while (runSizes.size() > 3){
        size_t curRunSize = runSizes.top();
        runSizes.pop();
        size_t prevRunSize =  runSizes.top();
        runSizes.pop();
        inplaceMerge(last - curRunSize - prevRunSize, last - curRunSize, last, comp);
        runSizes.push(curRunSize + prevRunSize);
    }
};


template <class RAI, class Compare>
void timSort(RAI first, RAI last, Compare comp){
    timSort(first, last, comp, DefaultTimSortParams());
};

template <class RAI>
void timSort(RAI first, RAI last){
    timSort(first, last, Comparator<typename std::iterator_traits<RAI>::value_type>(), DefaultTimSortParams());
}


#endif //TIMSORT_TIMSORTNEW_H
