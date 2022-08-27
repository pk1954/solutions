// WrapSignal.ixx
//
// ModelIO

module;

#include <string>
#include "Signal.h"

export module WrapSignal;

import ErrHndl;
import TrackNr;
import MoreTypes;
import NNetWrapperBase;
import NNetWrapperHelpers;

using std::wstring;

export class WrapSignal : public NNetWrapperBase  // Legacy
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        MicroMeterCircle umCircle;
        TrackNr          trackNr{ ScrReadTrackNr(script) };
        script.ScrReadString(L"source");
        unsigned long    ulSigSrc{ script.ScrReadUlong() };
        if (ulSigSrc == Signal::SIGSRC_CIRCLE)
        {
            umCircle = ScrReadMicroMeterCircle(script);
        }
        else
        {
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101"));
        }
    }
};
