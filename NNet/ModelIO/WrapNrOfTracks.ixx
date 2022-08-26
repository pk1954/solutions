// WrapNrOfTracks.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "NNetModelIO.h"

export module WrapNrOfTracks;

import NNetWrapperBase;
import TrackNr;
import Script;

using std::wostream;
using std::endl;

export class WrapNrOfTracks : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        unsigned int const uiNrOfTracks{ script.ScrReadUint() };
        for (unsigned int ui = 0; ui < uiNrOfTracks; ++ui)
            m_modelIO.GetImportNMWI().GetMonitorData().InsertTrack(TrackNr(0));
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << m_modelIO.GetExportNMRI().GetMonitorDataC().GetNrOfTracks() << endl;
    };
};
