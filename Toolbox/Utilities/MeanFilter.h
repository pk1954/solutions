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

class MeanFilter
{
public:

    MeanFilter()
    {

    }

    ~MeanFilter()
    {

    }

    void Reset()
    {
        m_raw.clear();
        m_filterSize = 1;
        resetFiltered();
    }

    void Recalc()
    {
        resetFiltered();
        for (float raw : m_raw)
        {
            m_sum += raw;
            if (m_raw.size() > m_filterSize) // normal case
            {
                m_sum -= m_raw[m_raw.size() - m_filterSize - 1];
            }
            //            else // startup phase
            if (m_filtered.size() < m_filterSize) // normal case
            {
                ++m_divisor;
            }
            m_filtered.push_back(m_sum / m_divisor);
        }

        //            add2Filtered(raw);
    }

    void Add(float const raw)
    {
        m_raw.push_back(raw);
        {
            m_sum += raw;
            if (m_raw.size() > m_filterSize) // normal case
            {
                m_sum -= m_raw[m_raw.size() - m_filterSize - 1];
            }
            else // startup phase
            {
                ++m_divisor;
            }
            m_filtered.push_back(m_sum / m_divisor);
        }
//        add2Filtered(raw);
    }

    float GetRaw(size_t const index) const 
    {
        assert(index < m_raw.size());
        return m_raw[index];
    }

    float GetFiltered(size_t const index) const
    {
        assert(index < m_filtered.size());
        return m_filtered[index];
    }

    size_t GetNrOfElements() const
    { 
        return m_raw.size(); 
    }

    size_t GetLastIndex() const
    { 
        return m_raw.size() - 1; 
    }

    void SetFilterSize(size_t const newSize)
    {
        m_filterSize = newSize;
        Recalc();
    }

    size_t GetFilterSize() const
    { 
        return m_filterSize;
    }

    void Dump() const
    {
        wcout << L"raw" << endl;
        wcout << '(' << endl;
        for (auto it : m_raw)
            wcout << it << endl;
        wcout << ')' << endl;
        wcout << L"filtered" << endl;
        wcout << '(' << endl;
        for (auto it : m_filtered)
            wcout << it << endl;
        wcout << ')' << endl;
    }

private:
    vector<float> m_raw;
    vector<float> m_filtered;
    float         m_sum        { 0.0f };
    float         m_divisor    { 0.0f };
    size_t        m_filterSize { 1 };

    void add2Filtered(float const raw)
    {
        m_sum += raw;
        if (m_raw.size() > m_filterSize) // normal case
        {
            m_sum -= m_raw[m_raw.size() - m_filterSize - 1];
        }
        else // startup phase
        {
            ++m_divisor;
        }
        m_filtered.push_back(m_sum / m_divisor);
    }

    void resetFiltered()
    {
        m_filtered.clear();
        m_divisor = 0.0f;
        m_sum     = 0.0f;
    }
};