// NNetSignal.ixx
//
// NNetModel

export module NNetModel:NNetSignal;

import Observable;
import Signals;
import :NNetSignalSource;

export class NNetSignal : public Signal
{
public:

    NNetSignal(Observable& obs, NNetSignalSource& sigSrc)
        : Signal(obs),
        m_sigSrc(sigSrc)
    {
        m_sigSrc.RegisterObserver(*this);
    }

    ~NNetSignal() final
    {
        m_sigSrc.UnregisterObserver(*this);
    }

    NNetSignalSource const* GetSignalSource() const
    {
        return &m_sigSrc;
    }

    void Notify(bool const bImmediate) final // called by compute thread!
    {
        Add2Signal(m_sigSrc.GetSignalValue());
    }

private:

    NNetSignalSource& m_sigSrc;
};