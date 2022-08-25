// WrapNrOfTracks.h 
//
// ModelIO

#pragma once

import NNetWrapperBase;

class WrapNrOfTracks : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        for (unsigned int ui = 0; ui < uiNrOfTracks; ++ui)
            m_modelIO.GetImportNMWI().GetMonitorData().InsertTrack(TrackNr(0));
    }

    void Write(wostream & out) const final 
    {
        WriteCmdName(out);
        out << m_modelIO.GetExportNMRI().GetMonitorDataC().GetNrOfTracks() << endl;
    };
};
