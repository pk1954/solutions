#pragma once

#include <assert.h>
#include <array>
#include <limits>
#include <functional>

using std::function;

template <typename T>
void DivNonZero( T & op, T const div )
{
    if ( div == 0 )
        op = 0;
    else
        op /= div;
}

template <typename T, size_t SIZE>
void DivNonZero( std::array<T, SIZE> & a, std::array<T, SIZE> const & div )
{
    for ( unsigned int uiRun = 0; uiRun < SIZE; ++uiRun )
        DivNonZero( a[ uiRun ], div[ uiRun ] );
}

template <typename T, size_t SIZE> class XArray
{
public:
    XArray()
    {
        zero();
    }
    
    void zero()
    {
        m_tGeneral = 0;
        m_tArray.fill( 0 );
    }

    void Add( unsigned int uiIndex, T const op )
    {
        m_tArray.at( uiIndex ) += op;
        m_tGeneral += op;
    }

    void DivNonZero( XArray const & div )
    {
        ::DivNonZero( m_tGeneral, div.m_tGeneral );
        ::DivNonZero( m_tArray,   div.m_tArray );
    }

    T & General() { return m_tGeneral; }
    T & operator[] ( unsigned int uiIndex ) { return m_tArray.at( uiIndex ); }

	void Apply2XArray( function< void ( T & ) > const & func )
	{
        for ( auto & elem : m_tArray )
		{
            func( elem );
		}
        func( m_tGeneral );
	}

private:
    T                      m_tGeneral;
    std::array < T, SIZE > m_tArray;
};
