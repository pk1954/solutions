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
        m_pModelReaderInterface = & modelReaderInterface;
        m_pParams               = & param;
        m_pObservable           = & observable;
        m_pAnimationInterface   = & animationInterface;
    }

    unique_ptr<SingleSignal>MakeSignal( ShapeId const id )
    {
        auto upSignal { make_unique<SingleSignal>( * m_pModelReaderInterface, * m_pParams, * m_pObservable, * m_pAnimationInterface ) };
        upSignal->SetSignalSource( id );
        return move( upSignal );
    }

    unique_ptr<SumSignal>MakeSignal( MicroMeterCircle const & umCircle )
    {
        auto upSignal { make_unique<SumSignal>( * m_pModelReaderInterface, * m_pParams, * m_pObservable ) };
        upSignal->SetSignalSource( umCircle );
        return move( upSignal );
    }

private:
    Param                    const * m_pParams               { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Observable                     * m_pObservable           { nullptr };
    AnimationInterface             * m_pAnimationInterface   { nullptr };
};
