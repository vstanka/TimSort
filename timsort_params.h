#ifndef TIMSORT_TIMSORT_PARAMS_H
#define TIMSORT_TIMSORT_PARAMS_H

#include <ctime>
#include <cstdlib>


template <class value>
struct Comparator{
    Comparator(){}
    bool operator()(const value& a, const value& b) const {
        return a < b;
    }
};

enum EWhatMerge{
    WM_NoMerge,
    WM_MergeXY,
    WM_MergeYZ
};

 struct ITimSortParams {
     virtual size_t minRun(size_t n) const = 0;

     virtual EWhatMerge whatMerge(size_t lenX, size_t lenY, size_t lenZ) const = 0;

     virtual size_t GetGallop() const = 0;
     virtual ~ITimSortParams(){}
 };

struct DefaultTimSortParams : public ITimSortParams{

    size_t minRun(size_t n) const{
        return 16;
    }
    EWhatMerge whatMerge(size_t lenX, size_t lenY, size_t lenZ) const {
        if (lenY <= lenZ)
            return WM_MergeYZ;
        else if (lenX <= lenY + lenZ)
            return WM_MergeXY;
        return WM_NoMerge;
    }
    size_t GetGallop() const{
        return 0;
    }
};

#endif //TIMSORT_TIMSORT_PARAMS_H
