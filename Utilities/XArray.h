#pragma once

#include <array>

template <typename T, size_t SIZE>
void operator/= ( std::array<T, SIZE> &a, T const op )
{
    for ( auto &f : a )
        f /= op;
}

template <typename T, size_t SIZE>
void operator+= ( std::array<T, SIZE> &a, T const op )
{
    for ( auto &f : a )
        f += op;
}

template <typename T, size_t SIZE>
 void operator*= ( std::array<T, SIZE> &a, T const op )
{
    for ( auto &f : a )
        f *= op;
}

template <typename T, size_t SIZE>
T SumArray( std::array<T, SIZE> &a )
{
    T sum = 0;
    for ( auto &f : a )
        sum += f;
    return sum;
}

template <typename T>
void Scale( T & op, T const div )
{
    if ( div == 0 )
        op = 0;
    else
        op *= static_cast< T >( 100 )/ div;
}

template <typename T>
void DivNonZero( T & op, T const div )
{
    if ( div == 0 )
        op = 0;
    else
        op /= div;
}

template <typename T, size_t SIZE>
void DivNonZero( std::array<T, SIZE> & a, T const div )
{
    if ( div == 0 )
        a.fill( 0 );
    else
        a /= div;
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
    XArray( )
    {
        zero( );
    }
    
    void zero( )
    {
        m_tGeneral = 0;
        m_tArray.fill( 0 );
    }

    void Add( unsigned int uiIndex, T const op )
    {
        m_tArray[ uiIndex ] += op;
        m_tGeneral += op;
    }

    void DivNonZero( T const op )
    {
        if ( op == 0 )
        {
            zero( );
        }
        else
        {
            m_tGeneral /= op;
            m_tArray /= op;
        }
    }

    void DivNonZero( XArray const & div )
    {
        ::DivNonZero( m_tGeneral, div.m_tGeneral );
        ::DivNonZero( m_tArray,   div.m_tArray );
    }

    T & General( ) { return m_tGeneral; }
    T & operator[] ( unsigned int uiIndex )  { return m_tArray[ uiIndex ]; }

 private:
    T                      m_tGeneral;
    std::array < T, SIZE > m_tArray;
};
