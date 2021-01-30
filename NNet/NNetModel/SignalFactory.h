// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "NNetParameters.h"
#include "Observable.h"
#include "ShapeId.h"
#include "AnimationInterface.h"
#include "Signal.h"

using std::unique_ptr;
using std::make_unique;

class NNetModelReaderInterface;

class SignalFactory
{
public:

    virtual ~SignalFactory() {};

    static void Initialize
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Observable                     & observable
    )
    {
        m_pNMRI       = & modelReaderInterface;
        m_pObservable = & observable;
    }

    static unique_ptr<Signal>MakeSignal( Signal const & src )
    {
        return make_unique<Signal>( src );
    }

    static unique_ptr<Signal>MakeSignal( MicroMeterCircle const & umCircle )
    {
        return make_unique<Signal>( * m_pNMRI, * m_pObservable, umCircle );
    }

private:
    inline static NNetModelReaderInterface const * m_pNMRI       { nullptr };
    inline static Observable                     * m_pObservable { nullptr };
};
