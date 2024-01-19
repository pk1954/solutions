// NNetSignal.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:NNetSignal;

import Observable;
import Signals;
import :NNetSignalSource;

export class NNetSignal : public Signal
{
public:

    NNetSignal(NNetSignalSource * const pSigSrc)
      : m_pSigSrc(pSigSrc)
    {
        assert(m_pSigSrc);
        m_pSigSrc->RegisterObserver(*this);
    }

    ~NNetSignal() final
    {
        m_pSigSrc->UnregisterObserver(*this);
    }

    NNetSignalSource const* GetSignalSource() const
    {
        return m_pSigSrc;
    }

    void Notify(bool const bImmediate) final
    {
        Add2Signal(m_pSigSrc->GetSignalValue());
    }

private:

    NNetSignalSource * m_pSigSrc;
};