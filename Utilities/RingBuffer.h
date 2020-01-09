// RingBuffer.h
//
// Utilities

#pragma once

#include <vector>

using std::vector;

class RingBuffer
{
public:

RingBuffer( size_t size )
	:	m_ringbufSum( 0.0 )
{
	m_ringbuf = new tRingbuf( size, 0.0 );
	m_iter = m_ringbuf->begin();
}

~RingBuffer( )
{
	delete m_ringbuf;
	m_ringbuf = nullptr;
}

void Add( double const dValue )
{
	m_ringbufSum -= * m_iter;
	* m_iter = dValue;
	m_ringbufSum += * m_iter;
	if ( ++m_iter == m_ringbuf->end() )
		m_iter = m_ringbuf->begin();
}

double GetAverage( )
{
	return m_ringbufSum / m_ringbuf->size();
}

private:
    typedef vector< double > tRingbuf;

    tRingbuf::iterator m_iter;
    tRingbuf         * m_ringbuf;
    double             m_ringbufSum;
};
