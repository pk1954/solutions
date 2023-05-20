// NNetSimuRunning.cpp
//
// NNetWindows

export module NNetWin32:NNetSimuRunning;

import NNetSignals;
import :ComputeThread;

export class NNetSimuRunning : public SimuRunning
{
public:
    void Initialize(ComputeThread const* pComputeThread)
    {
        m_pComputeThread = pComputeThread;
    }

    bool IsRunning() const final 
    {
        return m_pComputeThread->IsRunning();
    };

private:
    ComputeThread const * m_pComputeThread;
};
