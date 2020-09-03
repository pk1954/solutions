// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "NNetModelReaderInterface.h"

using std::vector;

class Signal
{
public:

    void Reset( )
    {
        m_timeStart = 0._MicroSecs;
        m_data.clear();
    }

    void Add( float const val )
    {
        m_data.push_back( val );
    }

private: 

    fMicroSecs     m_timeStart;
    vector <float> m_data {};
};