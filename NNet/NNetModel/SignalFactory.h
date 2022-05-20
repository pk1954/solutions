// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "NNetParameters.h"
#include "observable.h"
#include "NobId.h"
#include "Signal.h"

using std::unique_ptr;
using std::make_unique;

class NNetModelReaderInterface;

class SignalFactory
{
public:

    static void Initialize(Observable & observable)
    {
        m_pObservable = & observable;
    }

    static unique_ptr<Signal>MakeSignal(Sensor & sensor)
    {
        return make_unique<Signal>(* m_pObservable, sensor);
    }

private:
    inline static Observable * m_pObservable { nullptr };
};
