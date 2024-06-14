// WrapEvent.ixx
//
// ModelIO

module;

#include <iostream>
#include <iomanip>

export module WrapEvent;

import Commands;
import Types;
import Script;
import IoUtil;
import Signals;
import NNetModel;
import NNetModelIO;
import NNetWrapperHelpers;

using std::setprecision;
using std::wostream;
using std::endl;

export class WrapEvent : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        wstring    const wstrType    { script.ScrReadString() };
        fMicroSecs const umTimeStamp { ScrReadfMicroSecs(script) };
        EventType  const type        { NNetEvent::GetTypeFromName(wstrType) };
        if (type == EventType::stimulus)
        {
			SigGenId const id { ScrReadSigGenId(script) };
            NNetModelIO::GetImportNMWI().AddStimulusEvent(umTimeStamp, id);
        }
        else
        {
            NNetModelIO::GetImportNMWI().AddEvent(type, umTimeStamp);
        }
    }

    void Write(wostream & out) const final
    {
		NNetModelIO::GetExportNMRI().Apply2allEvents
		(
			[this, &out](NNetEvent const* pEvent)
			{
                WriteCmdName(out);
                out << Quoted(pEvent->GetEventTypeName()) << SPACE;
                out << pEvent->GetTime();
                if (pEvent->Type() == EventType::stimulus)
                    out << SPACE << static_cast<StimulusEvent const *>(pEvent)->GetId();
                out << endl;
			}
		);   
    };
};
