// StdSigGen.ixx
//
// NNetModel

export module NNetModel:StdSigGen;

import :SignalGenerator;

export class StdSigGen // Singleton
{
public:
    ~StdSigGen()                           = default;
    StdSigGen           (const StdSigGen&) = delete;
    StdSigGen& operator=(const StdSigGen&) = delete;

    static SignalGenerator * Get()
    {
        static SignalGenerator instance(L"Standard");
        return & instance;
    };

private:

    StdSigGen() = default;
};
