// WrapSignal.h 
//
// ModelIO

#pragma once

#include "ErrHndl.h"
#include "NNetWrapperHelpers.h"
#include "NNetWrapperBase.h"

class WrapSignal : public NNetWrapperBase  // Legacy
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        MicroMeterCircle umCircle;
        TrackNr          trackNr { ScrReadTrackNr(script) };
        script.ScrReadString(L"source");
        unsigned long    ulSigSrc { script.ScrReadUlong() };
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
