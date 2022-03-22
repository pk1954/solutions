// MeanFilter.cpp 
//
// Utilities

#include "stdafx.h"
#include "MeanFilter.h"

void MeanFilter::Reset()
{
    m_raw.clear();
    resetFiltered();
}

void MeanFilter::Recalc()
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
}

void MeanFilter::Add(float const raw)
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
}

float MeanFilter::GetRaw(size_t const index) const 
{
    assert(index < m_raw.size());
    return m_raw[index];
}

float MeanFilter::GetFiltered(size_t const index) const
{
    assert(index < m_filtered.size());
    return m_filtered[index];
}

size_t MeanFilter::GetNrOfElements() const
{ 
    return m_raw.size(); 
}

size_t MeanFilter::GetLastIndex() const
{ 
    return m_raw.size() - 1; 
}

void MeanFilter::SetFilterSize(size_t const newSize)
{
    m_filterSize = newSize;
    Recalc();
}

size_t MeanFilter::GetFilterSize() const
{ 
    return m_filterSize;
}

void MeanFilter::Dump() const
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

void MeanFilter::add2Filtered(float const raw)
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

void MeanFilter::resetFiltered()
{
    m_filtered.clear();
    m_divisor = 0.0f;
    m_sum     = 0.0f;
}
