// FixedPipeline.ixx
//
// Toolbox\Utilities

module;

#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>

export module FixedPipeline;

using std::vector;
using std::ranges::fill;

export template <typename T>
class FixedPipeline
{
public:
    FixedPipeline()
    {
    }

    size_t Size() const { return m_data.size(); }

    void Resize(size_t const newSize, T const elem)
    {
        assert(newSize > 0);
        m_data.resize(newSize, elem);
        m_iter = m_data.begin();
    }

    void Fill(T const elem)
    {
        fill(m_data, elem);
    }

    size_t GetSize() const { return m_data.size(); }

    void Push(T const& elem)
    {
        assert(m_iter < m_data.end());
        *m_iter = elem;
        if (m_iter == m_data.begin())
            m_iter = m_data.end() - 1;
        else
            --m_iter;
        assert(m_iter < m_data.end());
    }

    T Get() const
    {
        return *m_iter;
    }

    T Get(size_t const nr) const
    {
        assert(nr < m_data.size());
        size_t index { static_cast<size_t>(m_iter - m_data.begin()) + nr };
        if (index < m_data.size())
            return *(m_iter + nr);
        else
            return *(m_iter - (m_data.size() - nr));
    }

    void Apply2All(auto const& func) const
    {
        for (auto it = m_iter; it < m_data.end(); ++it)
            func(it);
        for (auto it = m_data.begin(); it < m_iter; ++it)
            func(it);
    }

    size_t GetElemNr(vector<T>::const_iterator const it)
    {
        return it - m_data.begin();
    }

private:
             vector<T>           m_data;
    typename vector<T>::iterator m_iter;
};