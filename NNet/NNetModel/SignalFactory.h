// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "Observable.h"
#include "ShapeId.h"
#include "AnimationInterface.h"
#include "SingleSignal.h"
#include "SumSignal.h"

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
        Observable                     & observable,
        AnimationInterface             & animationInterface
    )
    {
        m_pMRI                = & modelReaderInterface;
        m_pParams             = & param;
        m_pObservable         = & observable;
        m_pAnimationInterface = & animationInterface;
    }

    unique_ptr<SingleSignal>MakeSignal( ShapeId const id )
    {
        return make_unique<SingleSignal>( * m_pMRI, * m_pParams, * m_pObservable, * m_pAnimationInterface, id );
    }

    unique_ptr<SumSignal>MakeSignal( MicroMeterCircle const & umCircle )
    {
        return make_unique<SumSignal>( * m_pMRI, * m_pParams, * m_pObservable, * m_pAnimationInterface, umCircle );
    }

private:
    Param                    const * m_pParams             { nullptr };
    NNetModelReaderInterface const * m_pMRI                { nullptr };
    Observable                     * m_pObservable         { nullptr };
    AnimationInterface             * m_pAnimationInterface { nullptr };
};
