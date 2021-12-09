// RingBuffer.h
//
// Utilities

#pragma once

#include <assert.h>
#include <vector>

using std::vector;

using tRingbuf = vector<double>;

class RingBuffer
{
public:

explicit RingBuffer(size_t size)
{
	m_ringbuf.resize(size, 0.0);
	m_iter = m_ringbuf.begin();
}

~RingBuffer() = default;

void Add(double const dValue)
{
	m_ringbufSum -= * m_iter;
	* m_iter = dValue;
	m_ringbufSum += * m_iter;
	if (++m_iter == m_ringbuf.end())
		m_iter = m_ringbuf.begin();
}

double GetAverage() const
{
	assert(m_ringbuf.size() > 0);
	return m_ringbufSum / static_cast<double>(m_ringbuf.size());
}

private:
    tRingbuf::iterator m_iter;
    tRingbuf           m_ringbuf;
    double             m_ringbufSum { 0.0 };
};
