// SignalInterface.cpp 
//
// NNetModel

#include "stdafx.h"
#include "SignalInterface.h"

SignalInterface::SignalInterface
( 
    NNetModelReaderInterface const & modelReaderInterface,
    Param                    const & param,
    Observable                     & observable
)
{
    m_pModelReaderInterface = & modelReaderInterface;
    m_pParams = & param;
    m_pObservable = & observable;
    m_pObservable->RegisterObserver( this );
}

SignalInterface::~SignalInterface( )
{
    m_pObservable->UnregisterObserver( this );
}

int const SignalInterface::time2index( fMicroSecs const usParam ) const
{
    fMicroSecs const timeTilStart { usParam - m_timeStart };
    float      const fNrOfPoints  { timeTilStart / m_pParams->GetTimeResolution( ) };
    int              index        { static_cast<int>( roundf( fNrOfPoints ) ) };
    if ( index >= m_data.size() )
        index = Cast2UnsignedInt( m_data.size() - 1 );
    return index;
}

fMicroSecs const SignalInterface::index2time( int const index ) const
{
    float      const fNrOfPoints  { static_cast<float>( index ) };
    fMicroSecs const timeTilStart { m_pParams->GetTimeResolution( ) * fNrOfPoints };
    fMicroSecs const usResult     { timeTilStart + m_timeStart };
    return usResult;
}

float const SignalInterface::GetDataPoint( fMicroSecs const time ) const
{
    int index { time2index( time ) };
    return ( index < 0 ) ? NAN : m_data[ index ];
}

fMicroSecs const SignalInterface::FindNextMaximum( fMicroSecs const time ) const
{
    int index { time2index( time ) };
    if ( index < 0 )
        return fMicroSecs::NULL_VAL();
    if ( (index > 0) && (m_data[index-1] > m_data[index]) ) // falling values, go left
    {   
        while ( (--index > 0) && (m_data[index-1] >= m_data[index]) );
    }
    else   // climbing values, go right
    {
        while ( (index < m_data.size() - 1) && (m_data[index] <= m_data[index+1]) )
            ++index;
    }
    return index2time( index );
}

void SignalInterface::Notify( bool const bImmediate )
{
    m_data.push_back( GetSignalValue() );
}

void SignalInterface::CheckSignal( ) 
{
#ifdef _DEBUG
    assert( m_pModelReaderInterface );
    if ( (unsigned long long)m_pModelReaderInterface == 0xdddddddddddddddd )
    {
        int x = 42;
    }
#endif
}
