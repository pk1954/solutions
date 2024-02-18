// NNetSimuRunning.cpp
//
// NNetWindows

export module NNetWin32:NNetSimuRunning;

import NNetSignals;
import :Compute;

export class NNetSimuRunning : public SimuRunning
{
public:
    void Initialize(Compute const* pCompute)
    {
        m_pCompute = pCompute;
    }

    bool IsRunning() const final 
    {
        return m_pCompute->IsRunning();
    };

private:
    Compute const * m_pCompute;
};
