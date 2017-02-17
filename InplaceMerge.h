#ifndef TIMSORT_INPLACEMERGENEW_H
#define TIMSORT_INPLACEMERGENEW_H

#include "Runs.h"
#include <algorithm>

using std::swap_ranges;


template <class RAI, class Compare>
bool less(RAI first1, RAI last1, RAI first2, RAI last2, const Compare& comp){
    return (comp(*first1, *first2) ? true :
            (comp(*first2, *first1) ? false :
             (comp(last1[-1], last2[-1]))));    //*first1 == *first2
}


template <class RAI, class Compare>
void selectionSegmentSort(RAI first, size_t count, size_t segSize, const Compare& comp){
    size_t curBlockInd = 0;
    for (RAI i = first, indMin; curBlockInd < count; i += segSize, ++curBlockInd){
        indMin = i;
        size_t blockInd = curBlockInd + 1;
        for (RAI j = i + segSize; blockInd < count; j += segSize, ++blockInd)
            if (less(j, j + segSize, indMin, indMin + segSize, comp))
                indMin = j;
        if (indMin != i)
            swap_ranges(i, i + segSize, indMin);
    }
}

template <class RAI, class Compare>
void mergeSegmentWithBuffer(RAI first, RAI middle, RAI last, RAI buffer, const Compare& comp){
    RAI ResIt;
    RAI bufIt, bufSideIt;
    RAI bigIt, bigSideIt;
    ssize_t inc;
    size_t size1, size2;
    size1 = middle - first;
    size2 = last - middle;

    bool rightMerge = size1 <= size2;

    if (rightMerge){
        inc = 1;
        swap_ranges(first, middle, buffer);

        bigIt = middle;
        bigSideIt = last;

        bufIt = buffer;
        bufSideIt = buffer + size1;

        ResIt = first;
    } else {
        inc = -1;
        swap_ranges(middle, last, buffer);

        bigIt = middle - 1;
        bigSideIt = first - 1;

        bufIt = buffer + size2 - 1;
        bufSideIt = buffer - 1;

        ResIt = last - 1;
    }

    bool fromBuf;

    bool bufLessBig;
    bool bigLessBuf;
    while ((bufIt != bufSideIt) || (bigIt != bigSideIt)){
        if (bufIt == bufSideIt)
            fromBuf = false;
        else if (bigIt == bigSideIt)
            fromBuf = true;
        else {
            bufLessBig = comp(*bufIt, *bigIt);
            bigLessBuf = comp(*bigIt, *bufIt);
            fromBuf = (!bufLessBig && !bigLessBuf) || (bufLessBig == rightMerge);
        }
        if (fromBuf){
            std::swap(*ResIt, *bufIt);
            bufIt += inc;
        } else {
            std::swap(*ResIt, *bigIt);
            bigIt += inc;
        }
        ResIt += inc;
    }
}

//[begin, middle), [middle, end)
template <class RAI, class Compare>
void inplaceMerge(RAI first, RAI middle, RAI last, const Compare& comp){
    if (last - first <= 64){
        insertionSort(first, last, comp);
        return;
    }
    size_t size = last - first;
    size_t segSize = static_cast<size_t>(sqrt(size));

    RAI bufferIt = first + (size / segSize - 1) * segSize;
    size_t bufferSize = segSize + size % segSize;
    if ((middle - first) % segSize){
        RAI notSortedBlockIt = first + ((middle - first) / segSize) * segSize;
        swap_ranges(notSortedBlockIt, notSortedBlockIt + segSize, bufferIt);
    }
    selectionSegmentSort(first, size / segSize - 1, segSize, comp);

    for (RAI i = first; i < bufferIt; i += segSize)
        mergeSegmentWithBuffer(i, i + segSize, i + 2 * segSize, bufferIt, comp);
    for (RAI i = first; i < bufferIt -1; ++i){
        if (comp(i[1], *i))
            break;
    }
    insertionSort(bufferIt - bufferSize, last, comp);

    mergeSegmentWithBuffer(first, bufferIt - bufferSize, bufferIt, bufferIt, comp);
    insertionSort(bufferIt, last, comp);
}


#endif //TIMSORT_INPLACEMERGENEW_H
