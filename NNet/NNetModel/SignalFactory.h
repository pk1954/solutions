// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "Observable.h"
#include "Signal.h"

using std::unique_ptr;
using std::make_unique;

class SignalFactory
{
public:

    virtual ~SignalFactory() {};

    void Initialize
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Param                    const & param,
        Observable                     & observable
    )
    {
        m_pModelReaderInterface = & modelReaderInterface;
        m_pParams               = & param;
        m_pObservable           = & observable;
    }

    unique_ptr<Signal>MakeSignal( )
    {
        return make_unique<Signal>( * m_pModelReaderInterface, * m_pParams, * m_pObservable );
    }

private:
    Param                    const * m_pParams               { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Observable                     * m_pObservable           { nullptr };
};
