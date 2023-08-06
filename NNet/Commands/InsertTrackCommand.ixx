// InsertTrackCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:InsertTrackCommand;

import Types;
import IoUtil;
import :NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class InsertTrackCommand: public NNetCommand
{
public:

    InsertTrackCommand(TrackNr const trackNr)
        : m_trackNr(trackNr)
    {}

    void Do() final 
    {
        m_pNMWI->GetMonitorData().InsertTrack(m_trackNr);
        PlaySound(L"SNAP_IN_SOUND");
    };

    void Undo() final
    { 
        m_pNMWI->GetMonitorData().DeleteTrack(m_trackNr);
        PlaySound(L"DISAPPEAR_SOUND");
    };

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(TrackNr const trackNr)
	{
		if (IsTraceOn())
			TraceStream() << NAME << trackNr << endl;
		PushCommand(make_unique<InsertTrackCommand>(trackNr));
	}

private:

	inline static const wstring NAME { L"InsertTrack" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			InsertTrackCommand::Push(ScrReadTrackNr(script));
		}
	} m_wrapper { NAME };

	TrackNr m_trackNr;
};
