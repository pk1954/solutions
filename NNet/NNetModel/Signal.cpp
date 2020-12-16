// Signal.cpp 
//
// NNetModel

#include "stdafx.h"
#include "DrawContext.h"
#include "Signal.h"

Signal::Signal
( 
    NNetModelReaderInterface const & modelReaderInterface,
    Param                    const & param,
    Observable                     & observable,
    AnimationInterface             & animationInterface,
    MicroMeterCircle         const & circle
) :
    m_pMRI(& modelReaderInterface),
    m_pParams( & param ),
    m_pObservable( & observable ),
    m_pAnimationInterface(& animationInterface),
    m_circle(circle)
{
    Reset();
    m_pObservable->RegisterObserver( this );
}

Signal::~Signal( )
{
    m_pObservable->UnregisterObserver( this );
}

float Signal::GetSignalValue( ) const
{
    float fResult   { 0.0f };
    float fDsBorder { m_circle.GetRadius().GetValue() * m_circle.GetRadius().GetValue() };

    m_pMRI->Apply2All<BaseKnot>
        ( 		
            [&](BaseKnot const & b) 
            {  
                float fDsBaseKnot { DistSquare( b.GetPosition(), m_circle.GetPosition() ) };
                if ( fDsBaseKnot < fDsBorder )  // is b in circle?
                {
                    mV    voltage { b.GetVoltage() };
                    float fFactor { 1.0f - fDsBaseKnot / fDsBorder };
                    fResult += voltage.GetValue() * fFactor;
                }
            } 
    );

    return fResult;
}

void Signal::WriteSignalData( wostream & out ) const
{
    out << L"Signal " << m_circle;
}

void Signal::Draw( DrawContext const & context ) const
{
    D2D1::ColorF color1 { D2D1::ColorF::Green  };
    D2D1::ColorF color2 { D2D1::ColorF::Yellow };
    color1.a = 0.8f;
    color2.a = 0.4f;
    context.FillGradientCircle( m_circle, color1, color2 );
}

void Signal::Animate( bool const bOn ) const
{
    if ( bOn )
        m_pAnimationInterface->Start( m_circle );
    else
        m_pAnimationInterface->Stop( );
}

int const Signal::time2index( fMicroSecs const usParam ) const
{
    fMicroSecs const timeTilStart { usParam - m_timeStart };
    float      const fNrOfPoints  { timeTilStart / m_pParams->GetTimeResolution( ) };
    int              index        { static_cast<int>( roundf( fNrOfPoints ) ) };
    if ( index >= m_data.size() )
        index = Cast2UnsignedInt( m_data.size() - 1 );
    return index;
}

fMicroSecs const Signal::index2time( int const index ) const
{
    float      const fNrOfPoints  { static_cast<float>( index ) };
    fMicroSecs const timeTilStart { m_pParams->GetTimeResolution( ) * fNrOfPoints };
    fMicroSecs const usResult     { timeTilStart + m_timeStart };
    return usResult;
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
    m_data.push_back( GetSignalValue() );
}

void Signal::CheckSignal( ) 
{
#ifdef _DEBUG
    assert( m_pMRI );
    if ( (unsigned long long)m_pMRI == 0xdddddddddddddddd )
    {
        int x = 42;
    }
    if ( m_circle.IsNull() )
    {
        int x = 42;
    }
#endif
}
