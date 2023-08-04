// WrapNrOfTracks.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapNrOfTracks;

import WrapBase;
import Script;
import NNetModelIO;
import NNetModel;

using std::wostream;
using std::endl;

export class WrapNrOfTracks : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        unsigned int const uiNrOfTracks{ script.ScrReadUint() };
        for (unsigned int ui = 0; ui < uiNrOfTracks; ++ui)
            NNetModelIO::GetImportNMWI().GetMonitorData().InsertTrack(TrackNr(0));
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << NNetModelIO::GetExportNMRI().GetMonitorDataC().GetNrOfTracks() << endl;
    };
};
