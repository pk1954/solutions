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
        Resize(1, T(0));
    }

    ~FixedPipeline()
    {
        int x = 42;
    }

    size_t Size() const { return m_data.size(); }

    void Resize(size_t const newSize, T const elem)
    {
        assert(newSize > 0);
        m_data.resize(newSize, elem);
        m_index = 0;
    }

    void Fill(T const elem)
    {
        fill(m_data, elem);
    }

    size_t GetSize() const { return m_data.size(); }

    void Push(T const& elem)
    {
        assert(m_index < m_data.size());
        m_data[m_index] = elem;
        if (m_index == 0)
            m_index = m_data.size() - 1;
        else
            --m_index;
        assert(m_index < m_data.size());
    }

    T Get() const
    {
        return m_data[m_index];
    }

    T Get(size_t const nr) const
    {
        assert(nr < m_data.size());
        size_t index { m_index + nr };
        if (index >= m_data.size())
            index -= m_data.size();
        return m_data[index];
    }

    void Apply2All(auto const& func) const
    {
        for (auto i = m_index+1; i < m_data.size(); ++i)
            func(m_data[i]);
        for (auto i = 0; i <= m_index; ++i)
            func(m_data[i]);
    }

private:
    vector<T> m_data;
    size_t    m_index;
};