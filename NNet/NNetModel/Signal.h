// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "Observable.h"
#include "MoreTypes.h"
#include "NNetParameters.h"

using std::vector;

class Signal: public Observable
{
public:

    void Initialize( Param const & params )
    {
        m_pParams = & params;
    }

    void Reset( fMicroSecs const time )
    {
        m_data.clear();
        m_timeStart = time;
        NotifyAll( false  );
    }

    fMicroSecs const GetStartTime( ) const
    {
        return m_timeStart;
    }

    void Add( float const val )
    {
        m_data.push_back( val );
        NotifyAll( false  );
    }

    unsigned int GetPointIndex( fMicroSecs const time ) const
    {
        fMicroSecs const timeTilStart { time - m_timeStart };
        float      const fNrOfPoints  { timeTilStart / m_pParams->GetTimeResolution( ) };
        return static_cast<unsigned int>( roundf( fNrOfPoints ) );
    }

    float const GetDataPoint( unsigned int uiIndex ) const
    {
        if ( m_data.empty() )
            return NAN;
        if ( uiIndex >= m_data.size() )
            uiIndex = CastToUnsignedInt( m_data.size() - 1 );
        return m_data[ uiIndex ];
    }

    float const GetDataPoint( fMicroSecs const time ) const
    {
        return GetDataPoint( GetPointIndex( time ) );
    }

private: 

    fMicroSecs     m_timeStart { 0._MicroSecs };
    Param  const * m_pParams   { nullptr };
    vector <float> m_data      {};
};