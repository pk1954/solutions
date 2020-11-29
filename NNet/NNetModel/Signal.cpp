// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Signal.h"

Signal::Signal
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

Signal::~Signal( )
{
    m_pObservable->UnregisterObserver( this );
}

void Signal::SetSignalSource( ShapeId const id )
{
    m_SignalShapeId = id;
    m_data.clear();
    m_timeStart = m_pModelReaderInterface->GetSimulationTime( );
}

float const Signal::GetDataPoint( fMicroSecs const time ) const
{
    int index { time2index( time ) };
    return ( index < 0 ) ? NAN : m_data[ index ];
}

fMicroSecs const Signal::FindNextMaximum( fMicroSecs const time ) const
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

void Signal::Notify( bool const bImmediate )
{
    Neuron const * pNeuron { m_pModelReaderInterface->GetConstShapePtr<Neuron const *>( m_SignalShapeId ) };
    mV     const   voltage { pNeuron ? pNeuron->GetVoltage() : 0.0_mV };
    m_data.push_back( voltage.GetValue() );
}

void Signal::CheckSignal( ) 
{
#ifdef _DEBUG
    assert( m_pModelReaderInterface );
    if ( (unsigned long long)m_pModelReaderInterface == 0xdddddddddddddddd )
    {
        int x = 42;
    }
    if ( ! m_pModelReaderInterface->IsValidShapeId( m_SignalShapeId ) )
    {
        int x = 42;
    }
#endif
}
