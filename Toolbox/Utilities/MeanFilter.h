// MeanFilter.h : 
//
// Utilities

#pragma once

#include "assert.h"
#include <iostream>
#include <vector>

using std::vector;
using std::wcout;
using std::endl;

using SIG_INDEX = long; 

inline static SIG_INDEX INVALID_SIG_INDEX { -1 };

class MeanFilter
{
public:

    size_t    GetFilterSize  ()                const;
    float     GetRaw         (SIG_INDEX const) const;
    float     GetFiltered    (SIG_INDEX const) const;
    size_t    GetNrOfElements()                const;
    SIG_INDEX GetLastIndex   ()                const;

    void Reset();
    void Recalc();
    void Add(float const);
    void SetFilterSize(size_t const);
    void Dump() const;

private:
    vector<float> m_raw;
    vector<float> m_filtered;
    float         m_sum        { 0.0f };
    float         m_divisor    { 0.0f };
    size_t        m_filterSize { 1 };

    void add2Filtered(float const);
    void resetFiltered();
};
